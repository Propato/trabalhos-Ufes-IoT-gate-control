#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define RST_PIN         D1          // Pino de reset
#define SS_PIN          D8          // Pino SS (Slave Select)
#define BUTTON_WRITE    D2          // Pino do botão de escrita (porta analógica)
#define BUTTON_READ     D4          // Pino do botão de leitura
#define SERVO_PIN       D3

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância do MFRC522
MFRC522::MIFARE_Key key;
Servo s1;

// String que será escrita no cartão
String textToWrite = "Invalid";
// Buffer para armazenar dados lidos
String readText = "";

// Variáveis para debounce dos botões
int lastButtonWriteState = HIGH;
int lastButtonReadState = HIGH;

// WiFi settings
const char *ssid = "Marcia";             // Replace with your WiFi name
const char *password = "504617cm";  // Replace with your WiFi password

// MQTT Broker settings
const char *mqtt_broker = "broker.hivemq.com"; // EMQX broker endpoint
const char *mqtt_base_topic = "propato/ufes/iot/projeto";     // MQTT topic
// const char *mqtt_username = "emqx"; // MQTT username for authentication
// const char *mqtt_password = "public"; // MQTT password for authentication
const char *DEVICE_ID = "G1";
const int mqtt_port = 1883; // MQTT port (TCP)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    SPI.begin();
    mfrc522.PCD_Init();
    s1.attach(SERVO_PIN);

    // Configura os pinos dos botões
    pinMode(BUTTON_READ, INPUT_PULLUP);  // Botão de leitura com pull-up interno
    pinMode(BUTTON_WRITE, INPUT_PULLUP);       // Botão de escrita (A0 não tem pull-up interno)

    // Prepara a chave padrão FFFFFFFFFFFFh
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
}

void loop() {
    if (!mqtt_client.connected()) {
       connectToMQTTBroker();
    }

    // Verifica o estado dos botões com debounce
    checkButtons();

    mqtt_client.loop();
    // Pequeno delay para estabilidade
    delay(100);
}

void checkButtons() {
    // Lê o estado dos botões (invertido para A0 por causa do pull-down)
    int currentWriteState = digitalRead(BUTTON_WRITE);
    int currentReadState = digitalRead(BUTTON_READ);

    // Botão de escrita pressionado (LOW porque está com pull-up)
    if (currentWriteState == LOW && lastButtonWriteState == HIGH) {
        handleWriteButton();
    }

    // Botão de leitura pressionado (LOW porque está com pull-up)
    if (currentReadState == LOW && lastButtonReadState == HIGH) {
        handleReadButton();
    }

    lastButtonWriteState = currentWriteState;
    lastButtonReadState = currentReadState;
}

void handleWriteButton() {
    Serial.println(F("\nBotao ESCREVER pressionado"));

    // Aguarda até que um cartão seja aproximado
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        delay(50);
    }

    // Configuração do bloco para escrita
    byte sector = 1;
    byte blockAddr = 4;
    byte trailerBlock = 7;

    // Autentica usando a chave A
    if (!authenticate(trailerBlock)) return;

    char request_topic[100];
    snprintf(request_topic, sizeof(request_topic), "%s/parking/entry", mqtt_base_topic);

    if (!mqtt_client.connected()) {
       connectToMQTTBroker();
    }
    mqtt_client.publish(request_topic, DEVICE_ID);

    while (textToWrite == "Invalid"){
        mqtt_client.loop();
    }

    // Escreve a string no cartão
    writeStringToCard(blockAddr, textToWrite);

    // Finaliza a operação
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    Serial.print(F("\nESCRITA concluida: '"));
    Serial.print(textToWrite);
    Serial.println(F("'"));
    textToWrite = "Invalid";

    s1.write(180);
    delay(2000);
    s1.write(0);
}

void handleReadButton() {
    Serial.println(F("\nBotao LER pressionado"));

    // Aguarda até que um cartão seja aproximado
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        delay(50);
    }

    // Configuração do bloco para leitura
    byte sector = 1;
    byte blockAddr = 4;
    byte trailerBlock = 7;

    // Autentica usando a chave A
    if (!authenticate(trailerBlock)) return;

    // Lê a string do cartão
    readText = readStringFromCard(blockAddr);
    // Finaliza a operação
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    if (readText == "Erro na leitura") {
        Serial.println(F("Erro ao ler o cartão"));
        return;
    }

    char exit_topic[100];
    snprintf(exit_topic, sizeof(exit_topic), "%s/parking/exit", mqtt_base_topic);

    if (!mqtt_client.connected()) {
       connectToMQTTBroker();
    }
    mqtt_client.publish(exit_topic, readText.c_str());


    Serial.print(F("\nLEITURA concluida: '"));
    Serial.print(readText);
    Serial.println(F("'"));

    s1.write(180);
    delay(2000);
    s1.write(0);
}

bool authenticate(byte trailerBlock) {
    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Falha na autenticação: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
    Serial.println(F("Autenticado"));
    return true;
}

void writeStringToCard(byte blockAddr, String str) {
    byte buffer[18];
    byte size = sizeof(buffer);

    // Limpa o buffer
    for (byte i = 0; i < size; i++) {
        buffer[i] = ' ';
    }

    // Copia a string para o buffer
    str.getBytes(buffer, str.length() + 1);

    // Serial.print(F("Escrevendo no bloco ")); Serial.print(blockAddr);
    // Serial.print(F(": \"")); Serial.print(str); Serial.println(F("\""));

    MFRC522::StatusCode status = mfrc522.MIFARE_Write(blockAddr, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Erro na escrita: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    // } else {
    //     Serial.println(F("Escrita realizada com sucesso!"));
    }
}

String readStringFromCard(byte blockAddr) {
    byte buffer[18];
    byte size = sizeof(buffer);
    String str = "";

    // Serial.print(F("Lendo do bloco ")); Serial.println(blockAddr);

    MFRC522::StatusCode status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Erro na leitura: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return "Erro na leitura";
    }

    for (byte i = 0; i < size && buffer[i] != 0; i++) {
        // Opcional: filtra apenas caracteres ASCII imprimíveis
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            str += (char)buffer[i];
        }
    }

    return str;
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
   }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());

        // Conexão sem autenticação
        if (mqtt_client.connect(client_id.c_str())) {  // Sem username/password
            Serial.println("Connected to MQTT broker");

            char response_topic[100];
            snprintf(response_topic, sizeof(response_topic), "%s/parking/entry/%s/response", mqtt_base_topic, DEVICE_ID);
            mqtt_client.subscribe(response_topic);
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    char message[length + 1];
    for (unsigned int i = 0; i < length; i++) {
       message[i] = (char)payload[i];
    }
    message[length] = '\0';  // Null-terminate the string

    // Parse do JSON
    StaticJsonDocument<200> doc; // Ajuste o tamanho conforme seu JSON
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Extrai o valor da chave "slot"
    if (doc.containsKey("slot")) {
        String slotValue = doc["slot"].as<String>();
        textToWrite = slotValue;
    } else {
        Serial.println("JSON doesn't contain 'slot' key");
    }
}

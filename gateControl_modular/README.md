# Gate Control

Este projeto implementa um sistema de controle de portão automatizado utilizando ESP8266, RFID, MQTT e máquina de estados.

## Estrutura do Projeto

- `gateControl_modular.ino`: Arquivo principal, integra todos os módulos e implementa a lógica central.
- `GateControl.h/.cpp`: Gerencia a máquina de estados e orquestra os módulos.
- `RFIDManager.h/.cpp`: Gerencia leitura e escrita de cartões RFID.
- `ButtonManager.h/.cpp`: Gerencia os botões físicos de leitura e escrita.
- `WiFiManager.h/.cpp`: Gerencia conexão WiFi.
- `MQTTManager.h/.cpp`: Gerencia comunicação MQTT (broker, tópicos, mensagens).
- `ServoManager.h/.cpp`: Gerencia o servo motor responsável pela abertura/fechamento do portão.

## Funcionamento

1. **Inicialização**: Todos os módulos são configurados.
2. **Escrita no cartão**: Ao pressionar o botão de escrita, o sistema solicita um slot via MQTT, aguarda resposta e grava no cartão RFID.
3. **Leitura do cartão**: Ao pressionar o botão de leitura, o sistema lê o cartão RFID e envia o dado via MQTT para registrar a saída.
4. **Abertura/Fechamento do portão**: O servo é acionado conforme o fluxo de entrada/saída.

## Requisitos
- ESP8266
- Leitor RFID MFRC522
- Servo motor
- Botões físicos
- Broker MQTT (ex: HiveMQ)
- Bibliotecas: `ESP8266WiFi`, `PubSubClient`, `MFRC522`, `Servo`, `ArduinoJson`

## Como usar
1. Faça upload dos arquivos para o ESP8266.
2. Ajuste as credenciais WiFi e tópicos MQTT conforme necessário.
3. Conecte os componentes conforme os pinos definidos nos arquivos `.h`.
4. Execute o sistema e monitore via Serial ou MQTT.

---
David Propato e Klarine Mendonça - IoT Gate Control

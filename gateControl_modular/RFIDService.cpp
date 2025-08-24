#include "RFIDService.h"

void RFIDService::setup() {
    SPI.begin();

    if (mfrc522) delete mfrc522;
    mfrc522 = new MFRC522(SS_PIN, RST_PIN);

    mfrc522->PCD_Init();

    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    Serial.println(F("RFID Service initialized"));
}

bool RFIDService::isCardPresent() {
    if (!mfrc522) return false;
    return mfrc522->PICC_IsNewCardPresent() && mfrc522->PICC_ReadCardSerial();
}

bool RFIDService::authenticate(byte trailerBlock) {
    if (!mfrc522) return false;

    while (!isCardPresent()) delay(50);

    MFRC522::StatusCode status = mfrc522->PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522->uid)
    );

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522->GetStatusCodeName(status));
        return false;
    }

    Serial.println(F("Authenticated"));
    return true;
}

bool RFIDService::writeStringToCard(const String& str, byte blockAddr) {
    if (!mfrc522) return false;

    byte buffer[18];
    byte size = sizeof(buffer);

    for (byte i = 0; i < size; i++) buffer[i] = ' ';
    str.getBytes(buffer, str.length() + 1);

    MFRC522::StatusCode status = mfrc522->MIFARE_Write(blockAddr, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Writing failed: "));
        Serial.println(mfrc522->GetStatusCodeName(status));
        return false;
    }

    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();

    return true;
}

String RFIDService::readStringFromCard(byte blockAddr) {
    if (!mfrc522) return "Reading failed";

    byte buffer[18];
    byte size = sizeof(buffer);
    String str = "";

    MFRC522::StatusCode status = mfrc522->MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522->GetStatusCodeName(status));
        return "Reading failed";
    }

    for (byte i = 0; i < size && buffer[i] != 0; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) str += (char)buffer[i];
    }

    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();

    return str;
}

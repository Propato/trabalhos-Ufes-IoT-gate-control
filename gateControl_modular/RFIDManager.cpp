#include "RFIDManager.h"
#include <SPI.h>
#define SS_PIN D8
#define RST_PIN D1

void RFIDManager::setup() {
    SPI.begin();
    mfrc522 = MFRC522(SS_PIN, RST_PIN);
    mfrc522.PCD_Init();
}

bool RFIDManager::isCardPresent() {
    return mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial();
}

void RFIDManager::writeCard(const String& data) {
    // Implementação simplificada
    // ...
}

String RFIDManager::readCard() {
    // Implementação simplificada
    return "";
}

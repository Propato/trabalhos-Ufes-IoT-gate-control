#ifndef RFIDSERVICE_H
#define RFIDSERVICE_H

#include <MFRC522.h>
#include <SPI.h>

#define DEFAULT_BLOCK 4
#define DEFAULT_TRAILER 7
#define SS_PIN D8
#define RST_PIN D1

class RFIDService {
public:
    void setup(); // passa pinos para setup

    bool authenticate(byte trailerBlock = DEFAULT_TRAILER);

    bool writeStringToCard(const String& str, byte blockAddr = DEFAULT_BLOCK);
    String readStringFromCard(byte blockAddr = DEFAULT_BLOCK);

private:
    MFRC522* mfrc522 = nullptr; // ponteiro, será criado no setup
    MFRC522::MIFARE_Key key;

    bool isCardPresent();
};

#endif

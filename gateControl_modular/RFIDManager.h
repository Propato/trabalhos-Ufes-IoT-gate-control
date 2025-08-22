#ifndef RFIDMANAGER_H
#define RFIDMANAGER_H
#include <MFRC522.h>
class RFIDManager {
public:
    void setup();
    bool isCardPresent();
    void writeCard(const String& data);
    String readCard();
private:
    MFRC522 mfrc522;
};
#endif

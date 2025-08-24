#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

class WiFiManager {
public:
    bool setup();
    bool isConnected() const { return connected; }
private:
    bool connected = false;
    const char *ssid = "Marcia";
    const char *password = "504617cm";
};
#endif

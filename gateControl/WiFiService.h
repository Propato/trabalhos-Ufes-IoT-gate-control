#ifndef WIFISERVICE_H
#define WIFISERVICE_H

class WiFiService {
public:
    bool setup();
    bool isConnected() const { return connected; }
private:
    bool connected = false;
    const char *ssid = "Marcia";
    const char *password = "504617cm";
};
#endif

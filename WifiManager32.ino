#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


static void wifisetup32() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(38400);

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;



    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("SmartMeter", "password"); // password protected ap

    if (!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    }
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");

        // Start de webserver
        server.on("/", handleRoot); // Deze functie wordt uitgevoerd bij toegang tot de root van het IP-adres
        server.begin(); // Start de webserver
        Serial.println("Webserver gestart.");
    }
    
    
    // Stel de root route in
    server.on("/", handleRoot);

    // Stel de reset route in voor WiFi-instellingen
    server.on("/reset", HTTP_POST, handleReset);

    // Start de webserver
    server.begin();



    // reset settings - wipe stored credentials for testing
           // these are stored by the esp library
    if (tuneflag == true) {
            wm.resetSettings();
     }
     else {
     //WiFiManager wm;
        }
    }


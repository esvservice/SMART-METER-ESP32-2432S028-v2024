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
    res = wm.autoConnect("SmartMeter", "smartadmin"); // password protected ap

    if (!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    }
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        
        // If connection is successful, get the IP address
        MyipAddress = WiFi.localIP().toString();
        screen1text(); // refresh screen

        // Start de HTTP-server op poort 80
        server.on("/", handleMeterPage); // Webpagina met meters / root pagina
        server.on("/terminal", handleRoot); // testpagina 
        // Stel de reset route in voor WiFi-instellingen
        server.on("/reset", HTTP_POST, handleReset);
        
        server.begin();
        Serial.println("Webserver gestart. poort 80");

        // Start de WebSocket-server op poort 81
        webSocket.begin();
        webSocket.onEvent(webSocketEvent);
        Serial.println("WebSocketserver gestart. poort 81");
        
    }
    
  
    }


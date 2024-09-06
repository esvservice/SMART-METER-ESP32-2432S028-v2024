/*
 Name:		SMART_METER_ESP32_2432S028_v2024.ino
 Created:	9/6/2024 7:45:53 AM
 Author:	jess
This is a sketch for 7 bargraph meters on a 240 x 320, 2.8 TFT display (touch is not used) with ILI9341 for the Yaesu FT-991a (and FT-991).
  Make sure your display uses the ILI9341 driver, otherwise it will not work.
  There are also files available at Github for the FTdx-101MP and FTdx-101D.
  It uses CAT commands through the RS232 port of the radio (The USB connection of the radio stays fully operational).
  It measures all 7 meters of the radio and also shows the present power setting.
  It checks with a green/red indication if there is connection with the radio with the correct baudrate.
  It also has an external tune button possibility. Just add a momentary pushbutton (if desired).
  On button press: it reads present power and mode setting, then sets PWR to a selected value, mode to FM-N, then enables the MOX.
  On button release: it disables the TX and restores original power and mode settings.
  This software is written by Versailles Jess, ON4JES and is in the public domain.
  You must set the transceiver's RS232 connection to 38400 bps.
  This is version 1.0 first release, only tested on a FT-991a, but it should also work on a FT-991.


  Info over de ESP32-2432S028: https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
  intressante site met samples: https://cool-web.de/esp8266-esp32/esp32-2432s028-cheap-yellow-display-touchscreen-programmierung-platformio-library-tft-espi.htm
*/





#include "WiFi.h"
#include "WifiManager.h"
#include "WebServer.h"    // WebServer library for hosting a simple web page on ESP32
#include "stdio.h"     
#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include "LovyanGFX.hpp"  // https://github.com/lovyan03/LovyanGFX
#include "lgfx_ESP32_2432S028.h" 
#include "colourcode.h"
#include "touch.h"
#include "variables.h"



WebServer server(80); // Creëer een WebServer object op poort 80


// Functie voor de root (homepage)
void handleRoot() {
    String html = "<html><head>";

    // CSS Styling voor de Matrix look en het terminal venster
    html += "<style>";
    html += "body { background-color: black; color: green; font-family: 'Courier New', Courier, monospace; text-align: center; }";
    html += "h1 { font-size: 2.5em; margin-top: 50px; }";
    html += "p { font-size: 1.5em; }";
    html += ".matrix { font-size: 1.2em; }";
    html += "@keyframes flicker { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }";
    html += ".flicker { animation: flicker 1.5s infinite alternate; }";
    html += "input[type='submit'] { background-color: green; color: black; border: none; padding: 15px; font-size: 1.5em; cursor: pointer; }";
    html += "input[type='submit']:hover { background-color: #00ff00; }";

    // Terminal stijl voor de textarea
    html += ".terminal { width: 90%; height: 300px; background-color: black; color: green; font-family: 'Courier New', Courier, monospace; border: 1px solid green; padding: 10px; overflow-y: scroll; margin-top: 20px; }";
    html += "</style>";

    html += "<title>SMART DISPLAY - Matrix Mode</title>";
    html += "</head><body>";

    // Matrix header en tekst
    html += "<h1 class='flicker'>SMART DISPLAY TERMINAL</h1>";
    html += "<p class='flicker'>Welkom bij de SMART DISPLAY</p>";
    html += "<p>Dit is de homepage voor de SMART DISPLAY.</p>";
    html += "<p>Er zal nog heel veel aan gewerkt worden...</p>";

    // Terminal window waar de seriële communicatie wordt weergegeven
    html += "<textarea id='terminal' class='terminal' readonly></textarea>";

    // Knop om WiFi-instellingen te wissen
    html += "<form action='/reset' method='POST'>";
    html += "<input type='submit' value='Wis WiFi-instellingen'>";
    html += "</form>";

    // JavaScript voor het bijwerken van de terminal-tekst
    html += "<script>";

    // Simuleer het ontvangen van seriële data in de terminal
    html += "let terminal = document.getElementById('terminal');";
    html += "let serialData = '';";

    // Voeg een functie toe om nieuwe data toe te voegen aan de terminal
    html += "function addToTerminal(data) {";
    html += "  terminal.value += data + '\\n';"; // Voeg nieuwe tekst toe
    html += "  terminal.scrollTop = terminal.scrollHeight;"; // Scroll automatisch naar de onderkant
    html += "}";

    // Simuleer seriële communicatie - vervang dit met echte data als nodig
    html += "setInterval(function() {";
    html += "  let simulatedData = 'Serial: Verbonden met SMART DISPLAY...';";
    html += "  addToTerminal(simulatedData);";
    html += "}, 2000);";  // Voeg elke 2 seconden nieuwe seriële data toe

    html += "</script>";

    html += "</body></html>";

    server.send(200, "text/html", html);
}



// Functie om de WiFi-instellingen te wissen
void handleReset() {
    server.send(200, "text/html", "<html><body><h1>WiFi-instellingen gewist!</h1><p>Herstart de ESP32 om nieuwe instellingen in te voeren.</p></body></html>");
    delay(3000); // Wacht even zodat de gebruiker de bevestiging ziet
    WiFiManager wm;
    wm.resetSettings(); // Wis de WiFi-instellingen
    ESP.restart();      // Herstart de ESP32
}


void loop() {

    TouchScreen();
    Touchtune();//touchswtich
    Tuner(); //hardware swtich 
    serialcontrol(); //serial control
    Display();
    show_status();
    cat(); // cat controller

    //debug(); //to setup touch


    server.handleClient();  // Webserver aan de gang houden om aanvragen te verwerken




}

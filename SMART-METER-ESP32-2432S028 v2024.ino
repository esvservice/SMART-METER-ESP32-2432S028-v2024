/*
 Name:		SMART_METER_ESP32_2432S028_v2024.ino
 Created:	9/6/2024 11:29:53 AM
 Author:	ON4JES / ON7LSP / ON3VNA

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
  This is version 1234.T first release, only tested on a FT-991a, but it should also work on a FT-991.

  bij opstart eerst wifisettings invullen
  hotspotnaam: SmartMeter 
  Wachtwoord: smartadmin

  Daarna als deze correct zijn heb je toegang tot de webserver van de SmartMeter => 192.168.0.***
  todo jess => ipadres laten verschijnen op display van de webserver


  Info over de ESP32-2432S028: https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
  intressante site met samples: https://cool-web.de/esp8266-esp32/esp32-2432s028-cheap-yellow-display-touchscreen-programmierung-platformio-library-tft-espi.htm
*/





#include "WiFi.h"
#include "WifiManager.h"
#include "WebServer.h"    // WebServer library for hosting a simple web page on ESP32
#include "ArduinoJson.h"  // JSON library for sending data
#include "stdio.h"     
#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include "LovyanGFX.hpp"  // https://github.com/lovyan03/LovyanGFX
#include "lgfx_ESP32_2432S028.h" 
#include "colourcode.h"
#include "touch.h"
#include "variables.h"



WebServer server(80); // Creëer een WebServer object op poort 80


// Functie voor de terminal (homepage)
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


// Functie om meterwaarden als JSON te verzenden
void handleGetValues() {
    // Maak een JSON-object om de waarden op te slaan
    StaticJsonDocument<200> json;
    json["SWR"] = SWR;
    json["Comp"] = Comp;
    json["IDD"] = IDD;
    json["VDD"] = VDD;
    json["ALC"] = ALC;
    json["SMM"] = SMM;
    json["PO"] = PO;
    json["in_tx"] = in_tx;
    json["FreqA"] = FreqA;  // Voeg FreqA toe aan het JSON-object
    json["FreqB"] = FreqB;  // Voeg FreqB toe aan het JSON-object

    String jsonOutput;
    serializeJson(json, jsonOutput);

    // Verstuur de JSON-data
    server.send(200, "application/json", jsonOutput);
}

// Root HTML-pagina met meters met AJAX om de waarden te verversen
void handleMeterPage() {
    String html = "<html><head>";
    html += "<style>";

    // Algemene pagina opmaak (Dark Mode)
    html += "body { background-color: #1e1e1e; color: #ffffff; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 0; }";

    // Display layout voor VFO-A en VFO-B (bovenaan)
    html += ".vfo-display { display: flex; justify-content: center; padding: 20px; background-color: #111; }";
    html += ".vfo-display .vfo { margin: 0 20px; text-align: center; padding: 15px; background-color: #333; border-radius: 12px; width: 800px; }";
    html += ".vfo-display .vfo h2 { font-size: 16px; margin-bottom: 10px; color: #00ff00; }";  // Groene kleur voor titel
    html += ".vfo-display .vfo .freq { font-size: 48px; font-weight: bold; color: #FFD700; }";  // Goudkleurige frequentie

    // Taakbalk layout
    html += ".navbar { background-color: #333333; padding: 10px; display: flex; justify-content: space-around; align-items: center; }";
    html += ".navbar button { background-color: #007acc; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; }";
    html += ".navbar button:hover { background-color: #005f9e; }"; // Hover effect

    // Dashboard layout voor de meters
    html += ".dashboard { display: grid; grid-template-columns: repeat(3, 1fr); gap: 20px; padding: 40px; text-align: center; }";

    // Kaartjes (meter-containers)
    html += ".card { background-color: #252526; border-radius: 12px; padding: 20px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5); }";

    // Meters
    html += ".meter { width: 100%; height: 20px; background-color: #333; border-radius: 15px; position: relative; margin-top: 15px; }";
    html += ".progress { height: 100%; border-radius: 15px; transition: width 0.5s ease; }";
    html += ".meter, .progress { box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.6); }";

    // Specifieke meter kleuren
    html += "#swr .progress { background-color: #4caf50; }";     // Groen voor SWR
    html += "#comp .progress { background-color: #ffeb3b; }";    // Geel voor COMP
    html += "#idd .progress { background-color: #2196f3; }";     // Blauw voor IDD
    html += "#vdd .progress { background-color: #03a9f4; }";     // Lichter blauw voor VDD
    html += "#alc .progress { background-color: #ff9800; }";     // Oranje voor ALC
    html += "#smm .progress { background-color: #8bc34a; }";     // Groene kleur voor S meter
    html += "#po .progress { background-color: #00bcd4; }";      // Blauw voor PO meter

    // Schaalverdeling in meters
    html += ".scale { font-size: 12px; color: white; position: absolute; top: -20px; width: 100%; display: flex; justify-content: space-between; }";

    html += "</style>";

    // JavaScript voor AJAX updates
    html += "<script>";
    html += "function updateMeters(data) {";
    html += "document.getElementById('freq-display_A').innerText = data.FreqA + ' Hz';";
    html += "document.getElementById('freq-display_B').innerText = data.FreqB + ' Hz';";
    html += "document.getElementById('swr-progress').style.width = data.SWR * 100 / 255 + '%';";
    html += "document.getElementById('comp-progress').style.width = data.Comp * 100 / 255 + '%';";
    html += "document.getElementById('idd-progress').style.width = data.IDD * 100 / 255 + '%';";
    html += "document.getElementById('vdd-progress').style.width = data.VDD * 100 / 255 + '%';";
    html += "document.getElementById('alc-progress').style.width = data.ALC * 100 / 255 + '%';";
    html += "if (!data.in_tx) { document.getElementById('smm-progress').style.width = data.SMM * 100 / 255 + '%'; }";
    html += "else { document.getElementById('po-progress').style.width = data.PO * 100 / 255 + '%'; }";
    html += "}";
    html += "function fetchData() {";
    html += "fetch('/values').then(response => response.json()).then(data => updateMeters(data));";
    html += "}";
    html += "setInterval(fetchData, 10);";  // FAST
    html += "</script></head><body>";

    // Taakbalk container met knoppen
    html += "<div class='navbar'>";
    html += "<form action='/reset' method='POST' style='display:inline-block;'>";
    html += "<button type='submit'>Delete WiFi Settings</button>";
    html += "</form>";
    html += "</div>";  // Einde van navbar
    
    // VFO Displays (bovenaan)
    html += "<div class='vfo-display'>";
    html += "<div class='vfo'><h2>VFO-A</h2><div id='freq-display_A' class='freq'>Loading...</div></div>";
    html += "<div class='vfo'><h2>VFO-B</h2><div id='freq-display_B' class='freq'>Loading...</div></div>";
    html += "</div>";

    

    // Dashboard met meters
    html += "<div class='dashboard'>";

    // SWR meter
    html += "<div class='card'><h3>SWR Meter</h3>";
    html += "<div class='meter' id='swr'><div class='scale'><span>1</span><span>1.5</span><span>2</span><span class='yellow'>3</span><span class='red'>5</span><span class='red'>&#8734;</span></div>";
    html += "<div id='swr-progress' class='progress'></div></div></div>";

    // COMP meter
    html += "<div class='card'><h3>COMP Meter</h3>";
    html += "<div class='meter' id='comp'><div class='scale'><span>0</span><span>5</span><span class='yellow'>10</span><span class='yellow'>15</span><span class='red'>20</span></div>";
    html += "<div id='comp-progress' class='progress'></div></div></div>";

    // IDD meter
    html += "<div class='card'><h3>IDD Meter</h3>";
    html += "<div class='meter' id='idd'><div class='scale'><span>0</span><span>5</span><span>10</span><span>15</span><span>20</span><span>25</span><span>A</span></div>";
    html += "<div id='idd-progress' class='progress'></div></div></div>";

    // VDD meter
    html += "<div class='card'><h3>VDD Meter</h3>";
    html += "<div class='meter' id='vdd'><div class='scale'><span>12</span><span>13</span><span class='circled'>13.8</span><span>V</span></div>";
    html += "<div id='vdd-progress' class='progress'></div></div></div>";

    // ALC meter
    html += "<div class='card'><h3>ALC Meter</h3>";
    html += "<div class='meter' id='alc'><div class='scale'><span>0</span><span>70</span><span>100</span></div>";
    html += "<div id='alc-progress' class='progress'></div></div></div>";

    // S Meter (RX)
    html += "<div class='card'><h3>S Meter (RX)</h3>";
    html += "<div class='meter' id='smm'><div class='scale'><span>1</span><span>2</span><span>3</span><span>5</span><span>7</span><span>9</span><span class='red'>20</span><span class='red'>40</span><span class='red'>60</span></div>";
    html += "<div id='smm-progress' class='progress'></div></div></div>";

    // PO Meter (TX)
    html += "<div class='card'><h3>PO Meter (TX)</h3>";
    html += "<div class='meter' id='po'><div class='scale'><span>0</span><span>10</span><span>50</span><span>100</span><span>150</span><span>W</span></div>";
    html += "<div id='po-progress' class='progress'></div></div></div>";

    html += "</div>";  // Einde van dashboard container

    html += "</body></html>";
    server.send(200, "text/html", html);
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

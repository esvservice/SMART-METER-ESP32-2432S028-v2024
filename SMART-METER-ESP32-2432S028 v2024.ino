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





#include <WebSocketsServer.h>
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
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket server op poort 81

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
    json["pwrsetting"] = pwrsetting; // Voeg pwrsetting toe aan het JSON-object

    String jsonOutput;
    serializeJson(json, jsonOutput);

    // Verstuur de JSON-data
    server.send(200, "application/json", jsonOutput);
}

// Root HTML-pagina met meters met AJAX om de waarden te verversen
void handleMeterPage() {
    String html = "<html><head><style>";

    // Algemene pagina opmaak (Dark Mode)
    html += "body { background-color: #1e1e1e; color: #ffffff; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 0; }";
    html += ".vfo-display { display: flex; justify-content: space-around; padding: 20px; background-color: #111; gap: 20px; }";
    html += ".vfo-display .vfo { flex: 1; position: relative; padding: 15px; background-color: #333; border-radius: 12px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5); }";
    html += ".vfo-display .vfo h2 { position: absolute; top: 10px; left: 15px; font-size: 16px; margin: 0; transition: color 0.3s ease; }";
    html += ".vfo-display .vfo .freq { display: flex; align-items: center; justify-content: center; width: 100%; height: 100px; font-size: 5vw; font-weight: bold; color: #FFD700; position: relative; }";
    html += ".vfo-display .vfo .freq .small { font-size: 0.5em; vertical-align: sub; }";
    html += ".vfo-display .vfo .freq .unit { position: absolute; top: 10px; right: 15px; font-size: 0.5em; }";
    html += ".band-buttons-container { display: flex; justify-content: flex-start; background-color: #222; padding: 10px; }";
    html += ".band-buttons { display: flex; justify-content: space-between; width: 100%; max-width: 100%; }";
    html += ".band-buttons button { background-color: #333; color: #ffffff; border: 2px solid #555; border-radius: 5px; padding: 5px 10px; cursor: pointer; font-size: 12px; position: relative; }";
    html += ".band-buttons button::after { content: ''; position: absolute; top: -5px; left: 50%; transform: translateX(-50%); width: 10px; height: 10px; border-radius: 50%; background-color: #555; box-shadow: 0 0 5px #555; }";
    html += ".band-buttons button.active::after { background-color: #00ff00; box-shadow: 0 0 10px #00ff00; }";
    html += ".panel-container { display: flex; justify-content: space-around; padding: 20px; gap: 20px; }";
    html += ".panel { flex: 1; background-color: #222; padding: 15px; border-radius: 12px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5); }";
    html += ".panel-content { display: flex; flex-direction: column; gap: 10px; }";
    html += ".meters { width: 100%; display: flex; flex-direction: column; gap: 10px; }";
    html += ".meter { background-color: #333; border-radius: 12px; padding: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5); position: relative; }";
    html += ".meter h3 { margin: 0; font-size: 16px; color: #ffffff; display: inline-block; }";
    html += ".meter-bar { width: 100%; height: 20px; background-color: #252526; border-radius: 15px; margin-top: 10px; position: relative; }";
    html += ".progress { height: 100%; border-radius: 15px; transition: width 0.5s ease; }";
    html += ".scale { font-size: 12px; color: white; position: absolute; top: 0; width: 100%; display: flex; justify-content: space-between; padding: 0 5px; }";
    html += ".meter-row { display: flex; gap: 10px; }";
    html += ".half-meter { flex: 1; }";
    html += "#swr .progress { background-color: #4caf50; }";
    html += "#comp .progress { background-color: #ffeb3b; }";
    html += "#idd .progress { background-color: #2196f3; }";
    html += "#vdd .progress { background-color: #03a9f4; }";
    html += "#alc .progress { background-color: #ff9800; }";
    html += "#smm .progress { background-color: #8bc34a; }";
    html += "#po .progress { background-color: #00bcd4; }";
    html += ".scale span.yellow { color: #ffeb3b; }";
    html += ".scale span.red { color: #ff0000; }";
    html += ".pwr-setting { display: inline-block; float: right; font-size: 16px; color: #ffffff; transition: color 0.3s ease; }";
    html += "</style>";

    // JavaScript voor AJAX updates en WebSocket communicatie
    html += "<script>";
    html += "let previousPwrSetting = null;";
    html += "let socket;";

    html += "function initWebSocket() {";
    html += "  socket = new WebSocket('ws://' + window.location.hostname + ':81/');";
    html += "  socket.onopen = function() { console.log('WebSocket connection open'); };";
    html += "  socket.onmessage = function(event) { console.log('Message from server ', event.data); };";
    html += "  socket.onclose = function() { console.log('WebSocket connection closed'); };";
    html += "}";

    html += "function sendBandSelectCommand(band) {";
    html += "  if (socket && socket.readyState === WebSocket.OPEN) {";
    html += "    socket.send(band);";
    html += "  } else {";
    html += "    console.error('WebSocket is not open');";
    html += "  }";
    html += "}";

    html += "function formatFrequency(freq) {";
    html += "  if (freq >= 1000000) {";
    html += "    let freqMHz = (freq / 1000000).toFixed(6);";
    html += "    let parts = freqMHz.split('.');";
    html += "    let mainPart = parts[0] + '.' + parts[1].slice(0, 3);";
    html += "    let smallPart = parts[1].slice(3, 5);";
    html += "    return mainPart + '<span class=\"small\">' + smallPart + '</span><span class=\"unit\">Mhz</span>';";
    html += "  } else {";
    html += "    return freq + ' Hz';";
    html += "  }";
    html += "}";

    html += "function updateMeters(data) {";
    html += "  console.log('Updating meters with data:', data);";
    html += "  document.getElementById('freq-display_A').innerHTML = formatFrequency(data.FreqA);";
    html += "  document.getElementById('freq-display_B').innerHTML = formatFrequency(data.FreqB);";
    html += "  document.getElementById('swr-progress').style.width = (data.SWR * 100 / 255) + '%';";
    html += "  document.getElementById('comp-progress').style.width = (data.Comp * 100 / 255) + '%';";
    html += "  document.getElementById('idd-progress').style.width = (data.IDD * 100 / 255) + '%';";
    html += "  document.getElementById('vdd-progress').style.width = (data.VDD * 100 / 255) + '%';";
    html += "  document.getElementById('alc-progress').style.width = (data.ALC * 100 / 255) + '%';";
    html += "  if (!data.in_tx) { document.getElementById('smm-progress').style.width = (data.SMM * 100 / 255) + '%'; }";
    html += "  else { document.getElementById('po-progress').style.width = (data.PO * 100 / 255) + '%'; }";

    html += "  const bands = {";
    html += "    '160m': [1810000, 2000000],";
    html += "    '80m': [3500000, 3800000],";
    html += "    '60m': [5351500, 5366500],";
    html += "    '40m': [7000000, 7200000],";
    html += "    '30m': [10100000, 10150000],";
    html += "    '20m': [14000000, 14350000],";
    html += "    '17m': [18068000, 18168000],";
    html += "    '15m': [21000000, 21450000],";
    html += "    '12m': [24890000, 24990000],";
    html += "    '10m': [28000000, 29700000],";
    html += "    '6m': [50000000, 54000000],";
    html += "    '2m': [144000000, 146000000],";
    html += "    '70cm': [430000000, 440000000]";
    html += "  };";
    html += "  for (let band in bands) {";
    html += "    let button = document.getElementById('button-' + band);";
    html += "    let range = bands[band];";
    html += "    if (data.FreqA >= range[0] && data.FreqA <= range[1]) {";
    html += "      button.classList.add('active');";
    html += "    } else {";
    html += "      button.classList.remove('active');";
    html += "    }";
    html += "  }";

    html += "  let vfoAHeader = document.querySelector('.vfo-display .vfo h2');";
    html += "  if (data.in_tx) {";
    html += "    vfoAHeader.style.color = '#ff0000';";
    html += "  } else if (data.SMM > 0) {";
    html += "    vfoAHeader.style.color = '#00ff00';";
    html += "  } else {";
    html += "    vfoAHeader.style.color = '#ffffff';";
    html += "  }";

    html += "  const pwrSettingElement = document.getElementById('pwr-setting-value');";
    html += "  const newPwrSetting = data.pwrsetting;";
    html += "  if (previousPwrSetting === null) {";
    html += "    document.getElementById('po-progress').style.width = (newPwrSetting * 100 / 75) + '%';";
    html += "  }";
    html += "  if (previousPwrSetting !== newPwrSetting) {";
    html += "    pwrSettingElement.innerText = newPwrSetting + ' W';";
    html += "    pwrSettingElement.style.color = '#00ff00';";
    html += "    setTimeout(() => { pwrSettingElement.style.color = '#ffffff'; }, 1000);";
    html += "    previousPwrSetting = newPwrSetting;";
    html += "  }";
    html += "}";

    html += "function fetchData() {";
    html += "  fetch('/values').then(response => response.json()).then(data => updateMeters(data)).catch(error => console.error('Error fetching data:', error));";
    html += "}";

    html += "window.onload = function() {";
    html += "  initWebSocket();";
    html += "  setInterval(fetchData, 10);"; // Haal elke 10 milliseconde data op
    html += "};";
    html += "</script></head><body>";

    // HTML voor de band knoppen
    html += "<div class='band-buttons-container'>";
    html += "<div class='band-buttons'>";
    String bands[] = { "160m", "80m", "60m", "40m", "30m", "20m", "17m", "15m", "12m", "10m", "6m", "2m", "70cm" };
    for (String band : bands) {
        html += "<button id='button-" + band + "' onclick=\"sendBandSelectCommand('" + band + "')\">" + band + "</button>";
    }
    html += "</div>";
    html += "</div>";

    // VFO Displays
    html += "<div class='vfo-display'>";
    html += "<div class='vfo'><h2>VFO-A</h2><div id='freq-display_A' class='freq'>Loading...</div></div>";
    html += "<div class='vfo'><h2>VFO-B</h2><div id='freq-display_B' class='freq'>Loading...</div></div>";
    html += "</div>";

    // Voorpaneel voor meters en knoppen
    html += "<div class='panel-container'>";

    // Linker paneel voor meters
    html += "<div class='panel'>";
    html += "<div class='panel-content meters'>";
    html += "<div class='meter' id='swr'><h3>SWR Meter</h3><div class='meter-bar'><div class='scale'><span>1</span><span>1.5</span><span>2</span><span class='yellow'>3</span><span class='red'>5</span><span class='red'>&#8734;</span></div><div id='swr-progress' class='progress'></div></div></div>";
    html += "<div class='meter' id='comp'><h3>COMP Meter</h3><div class='meter-bar'><div class='scale'><span>0</span><span>5</span><span class='yellow'>10</span><span class='yellow'>15</span><span class='red'>20</span></div><div id='comp-progress' class='progress'></div></div></div>";
    html += "<div class='meter-row'>";
    html += "  <div class='meter half-meter' id='vdd'><h3>VDD Meter</h3><div class='meter-bar'><div class='scale'><span>12</span><span>13</span><span class='circled'>13.8</span><span>V</span></div><div id='vdd-progress' class='progress'></div></div></div>";
    html += "  <div class='meter half-meter' id='idd'><h3>IDD Meter</h3><div class='meter-bar'><div class='scale'><span>0</span><span>5</span><span>10</span><span>15</span><span>20</span><span>25</span><span>A</span></div><div id='idd-progress' class='progress'></div></div></div>";
    html += "</div>";
    html += "<div class='meter' id='alc'><h3>ALC Meter</h3><div class='meter-bar'><div class='scale'><span>0</span><span>70</span><span>100</span></div><div id='alc-progress' class='progress'></div></div></div>";
    html += "<div class='meter' id='smm'><h3>S Meter (RX)</h3><div class='meter-bar'><div class='scale'><span>1</span><span>2</span><span>3</span><span>5</span><span>7</span><span>9</span><span class='red'>+20</span><span class='red'>+40</span><span class='red'>+60</span></div><div id='smm-progress' class='progress'></div></div></div>";
    html += "<div class='meter' id='po'><h3>PO Meter</h3><div class='pwr-setting'>PWR Setting: <span id='pwr-setting-value'>Loading...</span></div><div class='meter-bar'><div class='scale'><span>0</span><span>10</span><span>25</span><span>50</span><span>75</span><span>W</span></div><div id='po-progress' class='progress'></div></div></div>";
    html += "</div>";
    html += "</div>";  // Einde van linker paneel voor meters

    // Rechter paneel voor andere knoppen
    html += "<div class='panel'>";
    html += "<div class='panel-content'>";
    html += "<h3>Control Buttons</h3>";
    html += "<button>Button 1</button>";
    html += "<button>Button 2</button>";
    html += "<button>Button 3</button>";
    html += "</div>";
    html += "</div>";  // Einde van rechter paneel voor knoppen

    html += "</div>";  // Einde van panel-container

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


    server.handleClient(); // Afhandelen van inkomende HTTP-verzoeken
    webSocket.loop(); // Beheer van WebSocket-verzoeken en -verbindingen




}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        String command = String((char*)payload);

        if (command == "160m") {
            band_select_160m();
        }
        else if (command == "80m") {
            band_select_80m();
        }
        else if (command == "60m") {
            band_select_60m();
        }
        else if (command == "40m") {
            band_select_40m();
        }
        else if (command == "30m") {
            band_select_30m();
        }
        else if (command == "20m") {
            band_select_20m();
        }
        else if (command == "17m") {
            band_select_17m();
        }
        else if (command == "15m") {
            band_select_15m();
        }
        else if (command == "12m") {
            band_select_12m();
        }
        else if (command == "10m") {
            band_select_10m();
        }
        else if (command == "6m") {
            band_select_6m();
        }
        else if (command == "2m") {
            band_select_2m();
        }
        else if (command == "70cm") {
            band_select_70cm();
        }
    }
}
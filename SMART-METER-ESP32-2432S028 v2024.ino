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





#include "WebSocketsServer.h"
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

// Websocket event functie
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        String command = String((char*)payload);
        handleCommand(command);
    }
    else if (type == WStype_DISCONNECTED) {
        Serial.printf("[%u] Disconnected!\n", num);
    }
}

//Verwerk het command
void handleCommand(String command) {
    
    if (command.startsWith("FA")) {
        FreqA = command.substring(2);
        //Serial.println("Setting FreqA to: " + FreqA);
        sendSerialCommand("FA" + FreqA + ";");
        get_radio_response();
    }
    else if (command.startsWith("FB")) {
        FreqB = command.substring(2);
        //Serial.println("Setting FreqB to: " + FreqB);
        sendSerialCommand("FB" + FreqB + ";");
        get_radio_response();
    }

    if (command.startsWith("MD0")) {
        String newMode = command.substring(3, 4);
        MD = newMode;
        sendSerialCommand("MD0" + newMode + ";");
        get_radio_response();
    }

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

// Functie om frequentie aan te passen en te versturen
void changeFrequency(String& freq, int change) {
    long freqValue = freq.toInt();
    freqValue += change;
    if (freqValue < 0) freqValue = 0; // Zorg ervoor dat frequentie niet negatief wordt
    freq = String(freqValue);
    while (freq.length() < 9) {
        freq = "0" + freq; // Voeg leidende nullen toe tot er 9 cijfers zijn
    }
}

// Functie om seriële commando's te versturen
void sendSerialCommand(String command) {
    Serial.println(command);
}

// Functie om meetwaarden te verzenden via websockets
void sendValues() {
    StaticJsonDocument<200> json;
    json["SWR"] = SWR;
    json["Comp"] = Comp;
    json["IDD"] = IDD;
    json["VDD"] = VDD;
    json["ALC"] = ALC;
    json["SMM"] = SMM;
    json["PO"] = PO;
    json["in_tx"] = in_tx;
    json["prevfreqA"] = prevfreqA.toInt();
    json["prevfreqB"] = prevfreqB.toInt();
    json["FreqA"] = FreqA.toInt();
    json["FreqB"] = FreqB.toInt();
    json["pwrsetting"] = pwrsetting;
    json["MD"] = MD;
    String jsonOutput;
    serializeJson(json, jsonOutput);
    webSocket.broadcastTXT(jsonOutput);
}

// Root HTML-pagina met meters met AJAX om de waarden te verversen
void handleMeterPage() {
    String html = "<html><head>" + getCSS() + getScript() + "</head><body>";
    html += getBandButtons();
    html += getVfoDisplays();
    html += getPanelContainer();
    html += "</body></html>";
    server.send(200, "text/html", html);
}

// Functies voor de HTML-pagina
String getCSS() {
    return "<style>"
        "body { background-color: #1e1e1e; color: #ffffff; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 0; }"
        ".vfo-display { display: flex; justify-content: space-around; padding: 20px; background-color: #1e1e1e; gap: 20px; }"
        ".vfo { flex: 1; position: relative; padding: 15px; background-color: #333; border-radius: 12px; box-shadow: inset 0 4px 8px rgba(0, 0, 0, 0.7), 0 4px 8px rgba(0, 0, 0, 0.5); min-width: 250px; display: flex; flex-direction: column; align-items: center; }"
        ".freq { position: relative; display: flex; align-items: center; justify-content: center; height: 100px; font-size: 4vw; font-weight: bold; color: #FFD700; background-color: #222; border-radius: 10px; box-shadow: inset 2px 2px 5px rgba(0, 0, 0, 0.7); width: 100%; }"
        ".freq::before { content: attr(data-label); position: absolute; top: 10px; left: 10px; font-size: 14px; color: #FFD700; }"
        ".freq .small { font-size: 0.5em; vertical-align: sub; }"
        ".freq .unit { font-size: 0.5em; margin-left: 10px; }"
        ".vfo-controls { position: relative; display: flex; align-items: center; gap: 5px; margin-top: 10px; }"
        ".button-container { display: flex; gap: 5px; }"
        ".freq-btn { background-color: #333; color: #FFD700; border: 2px solid #555; border-radius: 50%; padding: 5px; cursor: pointer; font-size: 14px; width: 28px; height: 28px; display: flex; align-items: center; justify-content: center; box-shadow: inset 0 2px 4px rgba(255, 255, 255, 0.3), 0 4px 8px rgba(0, 0, 0, 0.7); transition: background-color 0.2s, box-shadow 0.2s; }"
        ".freq-btn:hover { background-color: #444; box-shadow: inset 0 2px 4px rgba(255, 255, 255, 0.4), 0 6px 12px rgba(0, 0, 0, 0.9); }"
        ".step-select, #mode-select-A { background-color: #333; color: #FFD700; border: 2px solid #555; border-radius: 50%; padding: 5px; font-size: 12px; width: 70px; height: 28px; display: flex; align-items: center; justify-content: center; box-shadow: inset 0 2px 4px rgba(255, 255, 255, 0.3), 0 4px 8px rgba(0, 0, 0, 0.7); transition: background-color 0.2s, box-shadow 0.2s; appearance: none; cursor: pointer; }"
        ".step-select:focus, #mode-select-A:focus { outline: none; border-color: #00ff00; }"
        ".band-buttons-container { display: flex; justify-content: center; background-color: #1e1e1e; padding: 10px; margin-top: 5px; }"
        ".band-buttons { display: flex; justify-content: space-between; width: 100%; max-width: 800px; gap: 10px; }"
        ".band-btn { background-color: #333; color: #FFD700; border: 2px solid #555; border-radius: 5px; cursor: pointer; font-size: 1vw; width: 80px; height: 40px; display: flex; align-items: center; justify-content: center; box-shadow: inset 0 2px 4px rgba(255, 255, 255, 0.3), 0 4px 8px rgba(0, 0, 0, 0.7); transition: background-color 0.2s, box-shadow 0.2s; position: relative; }"
        ".band-btn:hover { background-color: #444; box-shadow: inset 0 2px 4px rgba(255, 255, 255, 0.4), 0 6px 12px rgba(0, 0, 0, 0.9); }"
        ".band-btn::before { content: ''; position: absolute; top: -15px; left: 50%; transform: translateX(-50%); width: 10px; height: 10px; border-radius: 50%; background-color: #555; box-shadow: 0 0 5px #555; }"
        ".band-btn.active::before { background-color: #00ff00; box-shadow: 0 0 10px #00ff00; }"
        ".panel-container { display: flex; justify-content: space-around; padding: 20px; gap: 20px; }"
        ".panel { flex: 1; background-color: #222; padding: 15px; border-radius: 12px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5); }"
        ".panel-content { display: flex; flex-direction: column; gap: 10px; }"
        ".meters { width: 100%; display: flex; flex-direction: column; gap: 10px; }"
        ".meter { background-color: #333; border-radius: 12px; padding: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5); position: relative; }"
        ".meter h3 { margin: 0; font-size: 16px; color: #ffffff; display: inline-block; }"
        ".meter-bar { width: 100%; height: 20px; background-color: #252526; border-radius: 15px; margin-top: 10px; position: relative; }"
        ".progress { height: 100%; border-radius: 15px; transition: width 0.5s ease; }"
        ".scale { font-size: 12px; color: white; position: absolute; top: 0; width: 100%; display: flex; justify-content: space-between; padding: 0 5px; }"
        ".meter-row { display: flex; gap: 10px; }"
        ".half-meter { flex: 1; }"
        "#swr .progress { background-color: #4caf50; }"
        "#comp .progress { background-color: #ffeb3b; }"
        "#idd .progress { background-color: #2196f3; }"
        "#vdd .progress { background-color: #03a9f4; }"
        "#alc .progress { background-color: #ff9800; }"
        "#smm .progress { background-color: #8bc34a; }"
        "#po .progress { background-color: #00bcd4; }"
        ".scale span.yellow { color: #ffeb3b; }"
        ".scale span.red { color: #ff0000; }"
        ".pwr-setting { display: inline-block; float: right; font-size: 16px; color: #ffffff; transition: color 0.3s ease; }"
        ".panel-content button { background-color: #333; color: #ffffff; border: 2px solid #555; border-radius: 5px; padding: 5px 10px; cursor: pointer; font-size: 12px; position: relative; }"
        "@media (max-width: 600px) {"
        "  .freq-btn { width: 24px; height: 24px; font-size: 12px; }"
        "  .band-btn { width: 70px; height: 35px; font-size: 2.5vw; }"
        "  .step-select, #mode-select-A { width: 60px; font-size: 9px; }"
        "}"
        "</style>";
}


// Java script functies
String getScript() {
    return "<script>"
        "let previousPwrSetting = null;"
        "let prevMD = null;"
        "let socket;"

        "function initWebSocket() {"
        "  socket = new WebSocket('ws://' + window.location.hostname + ':81/');"
        "  socket.onopen = function() {"
        "    console.log('WebSocket connection open');"
        "  };"
        "  socket.onmessage = function(event) {"
        "    console.log('Message received:', event.data);"
        "    try {"
        "      const data = JSON.parse(event.data);"
        "      updateMeters(data);"
        "    } catch (error) {"
        "      console.error('Error parsing JSON:', error);"
        "    }"
        "  };"
        "  socket.onerror = function(error) {"
        "    console.error('WebSocket error:', error);"
        "  };"
        "  socket.onclose = function() {"
        "    console.log('WebSocket connection closed');"
        "  };"
        "}"

        "function updateMeters(data) {"
        "  console.log('Updating meters with data:', data);"
        "  document.getElementById('freq-display_A').innerHTML = formatFrequency(data.FreqA);"
        "  document.getElementById('freq-display_A').setAttribute('data-freq', data.FreqA);"
        "  document.getElementById('freq-display_B').innerHTML = formatFrequency(data.FreqB);"
        "  document.getElementById('freq-display_B').setAttribute('data-freq', data.FreqB);"

        "  document.getElementById('swr-progress').style.width = (data.SWR * 100 / 255) + '%';"
        "  document.getElementById('comp-progress').style.width = (data.Comp * 100 / 255) + '%';"
        "  document.getElementById('idd-progress').style.width = (data.IDD * 100 / 255) + '%';"
        "  document.getElementById('vdd-progress').style.width = (data.VDD * 100 / 255) + '%';"
        "  document.getElementById('alc-progress').style.width = (data.ALC * 100 / 255) + '%';"
        "  document.getElementById('smm-progress').style.width = (data.SMM * 100 / 255) + '%';"

        "  let poProgressBar = document.getElementById('po-progress');"
        "  let mappedPO = (data.PO / 150) * 255;"
        "  poProgressBar.style.width = (mappedPO / 255) * 37.3 + '%';"

        "  const pwrSettingElement = document.getElementById('pwr-setting-value');"
        "  const newPwrSetting = data.pwrsetting;"
        "  if (previousPwrSetting !== newPwrSetting) {"
        "    pwrSettingElement.innerText = newPwrSetting + ' W';"
        "    pwrSettingElement.style.color = '#00ff00';"
        "    setTimeout(() => { pwrSettingElement.style.color = '#ffffff'; }, 1000);"
        "    previousPwrSetting = newPwrSetting;"
        "  }"

        "  if (prevMD !== data.MD) {"
        "    updateModeSelect(data.MD);"
        "    prevMD = data.MD;"
        "  }"
        "}"

        "function updateModeSelect(md) {"
        "  const modeSelect = document.getElementById('mode-select-A');"
        "  if (modeSelect) {"
        "    console.log('Attempting to set mode select to:', md);"
        "    modeSelect.value = md;"
        "    console.log('Mode select updated to:', modeSelect.value);"
        "  } else {"
        "    console.error('Mode select element not found');"
        "  }"
        "}"

        "function formatFrequency(freq) {"
        "  if (freq >= 1000000) {"
        "    let freqMHz = (freq / 1000000).toFixed(6);"
        "    let parts = freqMHz.split('.');"
        "    let mainPart = parts[0] + '.' + parts[1].slice(0, 3);"
        "    let smallPart = parts[1].slice(3, 5);"
        "    return mainPart + '<span class=\"small\">' + smallPart + '</span><span class=\"unit\">MHz</span>';"
        "  } else {"
        "    return freq + ' Hz';"
        "  }"
        "}"

        "function changeFrequency(vfo, direction) {"
        "  let stepSelect = document.getElementById('step-select-' + vfo);"
        "  let step = parseInt(stepSelect.value);"
        "  let frequencyChange = direction === 'up' ? step : -step;"

        "  if (vfo === 'A') {"
        "    let currentFreq = parseInt(document.getElementById('freq-display_A').getAttribute('data-freq'));"
        "    let newFreq = currentFreq + frequencyChange;"
        "    let freqString = newFreq.toString().padStart(9, '0');"
        "    socket.send('FA' + freqString);"
        "    document.getElementById('freq-display_A').setAttribute('data-freq', newFreq);"
        "  } else if (vfo === 'B') {"
        "    let currentFreq = parseInt(document.getElementById('freq-display_B').getAttribute('data-freq'));"
        "    let newFreq = currentFreq + frequencyChange;"
        "    let freqString = newFreq.toString().padStart(9, '0');"
        "    socket.send('FB' + freqString);"
        "    document.getElementById('freq-display_B').setAttribute('data-freq', newFreq);"
        "  }"
        "}"

        "window.onload = function() {"
        "  initWebSocket();"
        "  const modeSelect = document.getElementById('mode-select-A');"
        "  if (modeSelect) {"
        "    modeSelect.addEventListener('change', function() {"
        "      const selectedMode = this.value;"
        "      const command = 'MD0' + selectedMode + ';';"
        "      if (socket && socket.readyState === WebSocket.OPEN) {"
        "        socket.send(command);"
        "      } else {"
        "        console.error('WebSocket is not open');"
        "      }"
        "    });"
        "  } else {"
        "    console.error('Mode select element not found on window load');"
        "  }"
        "};"
        "</script>";
}



// Functie bandbuttons
String getBandButtons() {
    String html = "<div class='band-buttons-container'><div class='band-buttons'>";
    String bands[] = { "160m", "80m", "60m", "40m", "30m", "20m", "17m", "15m", "12m", "10m", "6m", "2m", "70cm" };
    for (String band : bands) {
        html += "<button id='button-" + band + "' class='band-btn' onclick=\"sendBandSelectCommand('" + band + "')\">" + band + "</button>";
    }
    html += "</div></div>";
    return html;
}

// Functies voor de vfo displays
String getVfoDisplays() {
    return "<div class='vfo-display'>"
        "<div class='vfo'>"
        "<div class='freq' data-label='VFO-A' data-freq='" + prevfreqA + "' id='freq-display_A'>Loading... <span class='unit'>MHz</span></div>"
        "<div class='vfo-controls'>"
        "<select id='mode-select-A' class='step-select'>"  // Mode selector naast VFO-A
        "<option value='1'>LSB</option>"
        "<option value='2'>USB</option>"
        "<option value='3'>CW-USB</option>"
        "<option value='4'>FM</option>"
        "<option value='5'>AM</option>"
        "<option value='6'>RTTY-LSB</option>"
        "<option value='7'>CW-LSB</option>"
        "<option value='8'>DATA-LSB</option>"
        "<option value='9'>RTTY-USB</option>"
        "<option value='A'>DATA-FM</option>"
        "<option value='B'>FM-N</option>"
        "<option value='C'>DATA-USB</option>"
        "<option value='D'>AM-N</option>"
        "<option value='E'>C4FM</option>"
        "</select>"
        "<select id='step-select-A' class='step-select'>"
        "<option value='100'>100 Hz</option>"
        "<option value='500'>500 Hz</option>"
        "<option value='1000'>1 kHz</option>"
        "<option value='2500'>2.5 kHz</option>"
        "<option value='3000'>3 kHz</option>"
        "<option value='5000'>5 kHz</option>"
        "<option value='6000'>6 kHz</option>"
        "<option value='6250'>6.25 kHz</option>"
        "<option value='8330'>8.33 kHz</option>"
        "<option value='9000'>9 kHz</option>"
        "<option value='10000'>10 kHz</option>"
        "<option value='12000'>12 kHz</option>"
        "<option value='12500'>12.5 kHz</option>"
        "<option value='25000'>25 kHz</option>"
        "<option value='50000'>50 kHz</option>"
        "</select>"
        "<button class='freq-btn up' onclick='changeFrequency(\"A\", \"up\")'>&#9650;</button>"
        "<button class='freq-btn down' onclick='changeFrequency(\"A\", \"down\")'>&#9660;</button>"
        "</div>"
        "</div>"
        "<div class='vfo'>"
        "<div class='freq' data-label='VFO-B' data-freq='" + prevfreqB + "' id='freq-display_B'>Loading... <span class='unit'>MHz</span></div>"
        "<div class='vfo-controls'>"
        "<select id='step-select-B' class='step-select'>"
        "<option value='100'>100 Hz</option>"
        "<option value='500'>500 Hz</option>"
        "<option value='1000'>1 kHz</option>"
        "<option value='2500'>2.5 kHz</option>"
        "<option value='3000'>3 kHz</option>"
        "<option value='5000'>5 kHz</option>"
        "<option value='6000'>6 kHz</option>"
        "<option value='6250'>6.25 kHz</option>"
        "<option value='8330'>8.33 kHz</option>"
        "<option value='9000'>9 kHz</option>"
        "<option value='10000'>10 kHz</option>"
        "<option value='12000'>12 kHz</option>"
        "<option value='12500'>12.5 kHz</option>"
        "<option value='25000'>25 kHz</option>"
        "<option value='50000'>50 kHz</option>"
        "</select>"
        "<button class='freq-btn up' onclick='changeFrequency(\"B\", \"up\")'>&#9650;</button>"
        "<button class='freq-btn down' onclick='changeFrequency(\"B\", \"down\")'>&#9660;</button>"
        "</div>"
        "</div>"
        "</div>";
}



// get functies
String getPanelContainer() {
    return "<div class='panel-container'>"
        "<div class='panel'>" + getMeters() + "</div>"
        "<div class='panel'>" + getControlButtons() + "</div>"
        "</div>";
}

// functies voor de meters
String getMeters() {
    return "<div class='panel-content meters'>"
        "<div class='meter' id='swr'><h3>SWR Meter</h3><div class='meter-bar'><div class='scale'><span>1</span><span>1.5</span><span>2</span><span class='yellow'>3</span><span class='red'>5</span><span class='red'>&#8734;</span></div><div id='swr-progress' class='progress'></div></div></div>"
        "<div class='meter' id='comp'><h3>COMP Meter</h3><div class='meter-bar'><div class='scale'><span>0</span><span>5</span><span class='yellow'>10</span><span class='yellow'>15</span><span class='red'>20</span></div><div id='comp-progress' class='progress'></div></div></div>"
        "<div class='meter-row'>"
        "  <div class='meter half-meter' id='vdd'><h3>VDD Meter</h3><div class='meter-bar'><div class='scale'><span>12</span><span>13</span><span class='circled'>13.8</span><span>V</span></div><div id='vdd-progress' class='progress'></div></div></div>"
        "  <div class='meter half-meter' id='idd'><h3>IDD Meter</h3><div class='meter-bar'><div class='scale'><span>0</span><span>5</span><span>10</span><span>15</span><span>20</span><span>25</span><span>A</span></div><div id='idd-progress' class='progress'></div></div></div>"
        "</div>"
        "<div class='meter' id='alc'><h3>ALC Meter</h3><div class='meter-bar'><div class='scale'><span>0</span><span>70</span><span>100</span></div><div id='alc-progress' class='progress'></div></div></div>"
        // S METER SCHAAL
        "<div class='meter' id='smm'><h3>S Meter (RX)</h3><div class='meter-bar'><div class='scale'>"
        "<span data-value='0'>1</span>"
        "<span data-value='14'>2</span>"
        "<span data-value='28'>3</span>"
        "<span data-value='42'>4</span>"
        "<span data-value='57'>5</span>"
        "<span data-value='71'>6</span>"
        "<span data-value='85'>7</span>"
        "<span data-value='99'>8</span>"
        "<span data-value='113'>9</span>"
        "<span data-value='137' class='red'>+10</span>"
        "<span data-value='160' class='red'>+20</span>"
        "<span data-value='184' class='red'>+30</span>"
        "<span data-value='208' class='red'>+40</span>"
        "<span data-value='231' class='red'>+50</span>"
        "<span data-value='255' class='red'>+60</span>"
        "</div><div id='smm-progress' class='progress'></div></div></div>"
        // po METER SCHAAL
        "<div class='meter' id='po'><h3>PO Meter</h3><div class='pwr-setting'>PWR Setting: <span id='pwr-setting-value'>Loading...</span></div><div class='meter-bar'><div class='scale'>"
        "<span data-value='0'>0</span>"
        "<span data-value='25'>5</span>"
        "<span data-value='44'>10</span>"
        "<span data-value='20'>20</span>"
        "<span data-value='103'>30</span>"
        "<span data-value='125'>40</span>"
        "<span data-value='147'>50</span>"
        "<span data-value='158'>60</span>"
        "<span data-value='169'>70</span>"
        "<span data-value='179'>80</span>"
        "<span data-value='190'>90</span>"
        "<span data-value='201'>100</span>"
        "<span data-value='212'>110</span>"
        "<span data-value='223'>120</span>"
        "<span data-value='233'>130</span>"
        "<span data-value='244'>140</span>"
        "<span data-value='255'>W</span>"
        "</div><div id='po-progress' class='progress'></div></div></div>"
        "</div>";
}

// functies voor de control buttons
String getControlButtons() {
    return "<div class='panel-content'>"
        "<h3>Control Buttons - Under Construction - </h3>"
        "<button>Button 1</button>"
        "<button>Button 2</button>"
        "<button>Button 3</button>"
        "</div>";
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

    // Elke 100 mseconde de waarden verzenden
    static unsigned long lastTime = 0;
    if (millis() - lastTime > 100) {
        lastTime = millis();
        sendValues();
    }


}



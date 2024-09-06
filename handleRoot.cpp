// Functie voor de root (homepage)
void handleRoot() {
	String html = "<html><head>";

	// CSS voor de Matrix Dashboard stijl
	html += "<style>";
	html += "body { background-color: black; color: #00ff00; font-family: 'Courier New', Courier, monospace; margin: 0; padding: 0; }";
	html += "h1 { font-size: 2.5em; margin-top: 20px; text-align: center; color: #00ff00; }";

	// Een grid layout voor een dashboard look
	html += ".container { display: grid; grid-template-columns: 1fr 1fr; grid-gap: 20px; padding: 20px; }";
	html += ".box { background-color: rgba(0, 255, 0, 0.1); border: 1px solid #00ff00; padding: 20px; border-radius: 10px; }";
	html += ".box h2 { font-size: 1.8em; margin-bottom: 10px; }";
	html += ".terminal { height: 250px; background-color: black; color: #00ff00; font-family: 'Courier New', Courier, monospace; border: 1px solid #00ff00; padding: 10px; overflow-y: scroll; }";

	// Animatie voor Matrix effect
	html += "@keyframes flicker { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }";
	html += ".flicker { animation: flicker 1.5s infinite alternate; }";

	html += "input[type='submit'] { background-color: #00ff00; color: black; border: none; padding: 15px; font-size: 1.2em; cursor: pointer; width: 100%; }";
	html += "input[type='submit']:hover { background-color: #00cc00; }";

	html += "</style>";

	html += "<title>SMART DISPLAY - Matrix Dashboard</title>";
	html += "</head><body>";

	// Matrix header
	html += "<h1 class='flicker'>SMART DISPLAY TERMINAL</h1>";

	// Dashboard container
	html += "<div class='container'>";

	// Linker Box: Terminal venster
	html += "<div class='box'>";
	html += "<h2>Live Terminal</h2>";
	html += "<textarea id='terminal' class='terminal' readonly></textarea>";
	html += "</div>";

	// Rechter Box: Knoppen en informatie
	html += "<div class='box'>";
	html += "<h2>Beheer</h2>";
	html += "<p>Gebruik de knop hieronder om WiFi-instellingen te wissen:</p>";
	html += "<form action='/reset' method='POST'>";
	html += "<input type='submit' value='Wis WiFi-instellingen'>";
	html += "</form>";
	html += "</div>";

	html += "</div>"; // Sluit de container

	// JavaScript voor WebSocket terminal output
	html += "<script>";
	html += "let terminal = document.getElementById('terminal');";

	// WebSocket setup
	html += "let socket = new WebSocket('ws://' + window.location.hostname + ':81/');";

	// Wanneer data wordt ontvangen via WebSocket
	html += "socket.onmessage = function(event) {";
	html += "  terminal.value += event.data + '\\n';"; // Voeg data toe aan terminal
	html += "  terminal.scrollTop = terminal.scrollHeight;"; // Scroll naar beneden
	html += "};";
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
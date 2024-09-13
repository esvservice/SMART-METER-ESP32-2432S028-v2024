void setup() {

	//-----------------------------------------------------------------------------------------------------------------------//
	// Start de seriële communicatie met UART0 (voor seriële monitor)
	Serial.begin(38400, SERIAL_8N2);
	//-----------------------------------------------------------------------------------------------------------------------//
	// Ledverlichting Display in of uitschakelen

	pinMode(TFT_LED, OUTPUT);
	digitalWrite(TFT_LED, HIGH); // Schakel de achtergrondverlichting in als dat nodig is

	//-----------------------------------------------------------------------------------------------------------------------//  
	//Linksboven vinden we de seriële interface("Power supply base") met :
	//		=> VIN(ingangsspanning, niet meer dan 3, 3 V)
	//		=> TX(verzenden, is verbonden met de TX aan de andere kant, komt eigenlijk overeen met RX op het bord)
	//		=> RX(ontvangen, is verbonden met de RX aan de andere kant, komt eigenlijk overeen met TX op het bord)
	//		=> GND(aarde)
	//  Hier kunt u een seriële monitor op aansluiten, of apparaten die via een seriële interface worden aangestuurd 
	//  en uitgelezen, zoals een NEO - 6M GPS - ontvangermodule of een SIM800L - module voor het mobiele netwerk .


	//-----------------------------------------------------------------------------------------------------------------------//
	//		ONBOARD CON "P3" of "Extended IO" 
	//		=> GND (aarde)
	//		=> IO35(GPI Pin 35) (ja, 35, niet 33, het lijkt alleen zo vanwege de via daar)
	//		=> IO22(GPIO Pin 22, overlapt met connector CN1)
	//		=> IO21(GPIO - pin 21, overlapt met de TFT - achtergrondverlichtingspin)

	  //pinMode(switchPin, INPUT); // set the Tune button pin as input

	//-----------------------------------------------------------------------------------------------------------------------//
	//		Links daarvan vind je op het bord een aansluiting die CN1 of "Temperatuur- en vochtigheidsinterface"
	//		=> GND (aarde)
	//		=> IO22(GPIO Pin 22, overlapt met connector P3)
	//		=> IO27(GPIO - pen 27)
	//		=> 3, 3V(voedingsspanning 3, 3V, ca. 20 mA)
	//-----------------------------------------------------------------------------------------------------------------------//
	// 
	// Andere aangesloten apparaten:
	//		De fotoresistor aan de voorzijde is van het type GT36516 en is aangesloten op GPIO pin 34.
	//		Het SD - kaartslot is via SPI verbonden met GPIO - pinnen 5 (CS), 23 (MOSI), 18 (CLK)en 19 (MISO).
	//		
	// Het flashgeheugen wordt aangesloten via 
	//		GPIO 11 (SCS / CMD), 6 (SCK / CLK), 8 (SDI / SD1), 7 (SDO / SD0), 9 (SHD / SD2) en 10 (SWP / SD3).
	//		
	// De positiesensor in het touchscreen(XPT2046) is via SPI verbonden met 
	//		GPIO 25 (CLK), 33 (CS), 32 (DIN / MOSI), 39 (OUT / MISO) en 36 (IRQ).
	//		
	// Het TFT - scherm is via SPI verbonden met 
	//		GPIO 14 (SCK), 2 (RS), 15 (CS), 13 (SDI / MOSI), 12 (SDO / MISO).
	//		GPIO 21 bestuurt de achtergrondverlichting - LED.
	//		
	// Rechtsboven bevinden zich ook twee knoppen, één voor reset(RST) en één voor opstarten.
	// 
	//-----------------------------------------------------------------------------------------------------------------------//
	//   
	// ONBOARD CODE: gemarkeerd met "LED1"
	pinMode(RED_LED_PIN, OUTPUT); // rode led
	pinMode(GREEN_LED_PIN, OUTPUT); // rode led
	pinMode(BLUE_LED_PIN, OUTPUT); // rode led

	digitalWrite(RED_LED_PIN, HIGH); // Schakel de led uit opgelet inverted logic HIGH = OFF  LOW = ON
	digitalWrite(GREEN_LED_PIN, HIGH); // Schakel de led uit opgelet inverted logic
	digitalWrite(BLUE_LED_PIN, HIGH); // Schakel de led uit opgelet inverted logic
	//
	//-----------------------------------------------------------------------------------------------------------------------//  
	  // display setup  
	tft.begin();
	//-----------------------------------------------------------------------------------------------------------------------//   
	//touchscreen activeren
	touch.begin();
	touch.setRotation(3);										 // set touchscreen orientation
	//-----------------------------------------------------------------------------------------------------------------------//   
	//opstartscherm intro en versie
	tft.fillScreen(BLACK);									// fill background with black
	tft.setCursor(70, 50);                                    // write the welcome screen
	tft.setTextColor(GOLD);
	tft.setTextSize(2);
	tft.println("SMART DISPLAY ");
	tft.setCursor(16, 80);
	tft.println("    for FT-991(a)");
	tft.setCursor(150, 110);
	tft.println("by");
	tft.setCursor(40, 140);
	tft.setTextColor(SILVER);
	tft.println("ON4JES & ON7LSP ");
	tft.setCursor(230, 220);
	tft.setTextSize(1);
	tft.println("v1234.T");
	delay(2000);                                             // show welcome screen for 2000 mS
	//-----------------------------------------------------------------------------------------------------------------------//  
	
	//Draw screen1 scherm met meters inladen    // now first draw most meter outlines with their scale text and colors       //
	
	wifisetup32();              //WifiManager.ino
	screen1text();				//display.ino
	draw_radio_screen();			//display.ino
	show_status();				//display.ino
	

	//-----------------------------------------------------------------------------------------------------------------------//  
}

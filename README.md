Name:		SMART_METER_ESP32_2432S028_v2024.ino

Created:	2024 and also under construction

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

#pragma once

#define switchPin 35                                      // Tune button is connected to NANO pin 2
#define set_tune_pwr "PC020;"                             // set tune power, you can choose which PWR to use for tuning (005 - 100 W)
#define timeout_delay 10;                                 // delay to flag a data timeout from the radio, now set at 10 mSec, same as in radio
#define RED_LED_PIN 4
#define GREEN_LED_PIN 16
#define BLUE_LED_PIN 17

#define SER_BUF_SIZE 256

bool isVersion3 = 1;  // Zet deze variabele op true of 1 voor versie 3 (ST7789) op false of 0 voor versie 1 (ILI9341)

// Voor ESP32-2432S028 zijn pin-definities voor het display mogelijk als volgt:
// Dit zul je moeten verifiëren in de documentatie van het bord.
//#define TFT_CS   15
//#define TFT_RST  -1
//#define TFT_DC   2
//#define TFT_MOSI 13
//#define TFT_CLK  14
//#define TFT_MISO 12
#define TFT_LED  21 // Voor sommige schermen moet je misschien de achtergrondverlichting aanzetten

int touchswitch;
byte X = 55;                   // starting point of meter bars from left
byte Y = 18;                   // row of complete meter: bar, outline and text
byte H = 6;                    // height of meter bars
byte Z = 15;                   // starting point of the meters text from left
byte SWR;                      // measured value of SWR
byte pSWR = 0;                 // previous measured value of SWR
byte Comp;                     // measured value of Comp
byte pComp = 0;                // previous measured value of Comp
byte IDD;                      // measured value of IDD
byte pIDD = 0;                 // previous measured value of IDD
byte VDD;                      // measured value of VDD
byte pVDD = 0;                 // previous measured value of VDD
byte ALC;                      // measured value of VDD
byte pALC = 0;                 // previous measured value of VDD
byte SMM;                      // measured value of S meter
byte pSMM = 0;                 // previous measured value of S meter
byte PO;                       // measured value of PO meter meter
byte pPO = 0;                  // previous measured value of PO meter
byte delaycounter = 0;         // counter used for debounce of tune button

int val;                        // variable for reading the Tune button status
int val2;                       // variable for reading the delayed/debounced status of Tune button
bool buttonrelease = false;     // flag to send just one set of commands at Tune button release
bool buttonpress = false;       // flag to send just one set of commands at Tune button press
bool tune = true;               // flag, do not tune when false
String prevpwr;                 // this string stores the original power setting
String prevmode;                // this string stores the original mode setting
bool receiving;                 // variable for the receive radio data process
String CAT_buffer;              // string to hold the received CAT data from radio
char rx_char;                   // variable for each character received from the radio
char t;                         // used to clear input buffer
unsigned long timeout, current_millis;  // variables to handle communication timeout errors
bool constatus = false;         // status of connection to radio
bool main_flag1 = false;        // flag to check receiver status
bool main_flag2 = true;         // flag to check receiver status
bool txrx_flag1 = false;        // flag to check if in TX or RX
bool txrx_flag2 = true;         // flag to check if in TX or RX
bool in_tx = false;             // status of tx
String pwrsetting;              // used for displaying power setting
String prevpwrsetting;          // used for displaying power setting

String FreqA;
String freqA;                   // used for displaying Freq VFOA
String prevfreqA;               // used for displaying Freq VFOA

String MyipAddress;             // ipadres when connected with wifi

//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO); // Invoke custom library
bool tuneflag = false;
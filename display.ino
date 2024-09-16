// SHOW OPTIONAL TEXT ON DISPLAY
void screen1text() {
    tft.fillScreen(BLACK);
    tft.setTextSize(1);                                             // small text

    //print the text in the upper part of the display
    tft.setCursor((X + 40), (Y - 10));
    tft.setTextColor(GOLD);
    //tft.println("FT-991(a) SMART DISPLAY");
    tft.println("FT-991/a SMART DISPLAY " + MyipAddress);

    // print the text- connection status & Power set to
    tft.setCursor(12, 11);
    tft.setTextColor(WHITE);
    tft.println("No Cat:");
    tft.setCursor((X + 115), 82);
    tft.setTextColor(WHITE);
    tft.println("Power set to (W):");
    
    // check status & texting display


}

//SHOW SERIAL STATUS ON DISPLAY
void show_status() {                            // this routine shows a red or green connection statusled on the display



    //tft.drawCircle(5, 11, 6, BLACK);         // outline of circle is black
    if (constatus == false) {
        tft.fillCircle(5,11, 3, RED);         // red led if status is false (75 + X, 85, 4, RED);
    }
    else {
        tft.fillCircle(5,11, 3, GREEN);       // green led if status is true
    }
}


// radioscreen 01
void draw_radio_screen() {

    draw_SWR_meter();
    draw_COMP_meter();
    draw_IDD_meter();
    draw_VDD_meter();
    draw_ALC_meter();
    draw_M_S1_meter();
    draw_touchbuttons01();

    Display();
    prevpwrsetting = "0";
    pVDD = 0;
    prevfreqA = "0";
}
// Drawing the meters on display


void draw_M_S1_meter()
{
    tft.drawRoundRect(0, 23, 319, 196, 7, TFT_GOLD);  // Omkadering meters
    tft.drawRoundRect(90, 1, 146, 23, 3, GOLD);
    
    //Draw S meter, the S meter can be substituted by the PO meter
    
    tft.fillRect((X - 44), (Y + 175), 42, 20, BLACK);           // clear the text area
    tft.setCursor(Z, (Y + 178));
    tft.setTextColor(SILVER);                      // set textcolor
    tft.setTextSize(2);
    tft.println("  S");                            // display text of receiver meter bar
    tft.setTextSize(1);                                             // display text of receiver meter bar
    
    tft.fillRect(X, (Y + 173), (X + 200), 21, BLACK);       // background of meter to black
    tft.drawFastVLine(54, 204, 7, SILVER);
    tft.drawFastHLine(55, 204, 135, SILVER);
    tft.drawFastHLine(190, 204, 119, RED);
    tft.setTextColor(GREY);                                 // set textcolor to white
    tft.setCursor(57, 192);
    tft.println("1");                                                 // display S = 1 scale text, 1 S point = 14 pixels
    tft.setCursor((X + 4), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 11), Y + 174);
    tft.println("2");                                                 // display S = 2 scale text white
    tft.setCursor((X + 11), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 28), Y + 174);
    tft.println("3");                                                 // display S = 3 scale text white
    tft.setCursor((X + 28), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 48), Y + 174);
    tft.println("4");                                                 // display S = 4 scale text white
    tft.setCursor((X + 48), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 65), Y + 174);
    tft.println("5");                                                 // display S = 5 scale text white
    tft.setCursor((X + 65), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 80), Y + 174);
    tft.println("6");                                                 // display S = 6 scale text white
    tft.setCursor((X + 80), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 95), Y + 174);
    tft.println("7");                                                 // display S = 7 scale text white
    tft.setCursor((X + 95), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 113), Y + 174);
    tft.println("8");                                                 // display S = 8 scale text white
    tft.setCursor((X + 113), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 131), Y + 175);
    tft.println("9");                                                 // display S = 9 scale text white
    tft.setCursor((X + 131), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setTextColor(RED);                                            // set textcolor to red
    tft.setCursor((X + 165), Y + 174);
    tft.println("+20");                                                // display S = 20 scale text red **************
    tft.setCursor((X + 170), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 202), Y + 174);
    tft.println("+40");                                                // display S = 40 scale text red
    tft.setCursor((X + 207), Y + 179);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 235), Y + 174);
    tft.println("+60");                                                // display S = 60 scale text red
    tft.setCursor((X + 250), Y + 179);
    tft.println(".");                                                 // display S = dot scale
}
void draw_PO_meter() {

    //Draw PO meter, the PO meter can be substituted by the S meter
    
    tft.fillRect((X - 44), (Y + 173), (X + 244), 21, BLACK); // background of meter to black
    tft.setCursor(28, 195);
    tft.drawFastVLine(54, 204, 6, SILVER);
    tft.drawFastHLine(55, 204, 252, SILVER);
    tft.setTextColor(SILVER);                                  // set textcolor to blue
    tft.setTextSize(2);
    tft.println("PO");                 // print the meters text
    tft.setTextSize(1);                                              // print meter text
    tft.setCursor((X + 4), Y + 175);
    tft.println("0");                                                 // display PO = 0 scale text, power 0W
    tft.setCursor((X + 32), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 5W
    tft.setCursor((X + 51), Y + 175);
    tft.println("10");                                                // display PO = 10 scale text
    tft.setCursor((X + 82), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 20W
    tft.setCursor((X + 104), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 30W
    tft.setCursor((X + 128), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 40W
    tft.setCursor((X + 147), Y + 175);
    tft.println("50");                                                 // display PO = 50W scale text
    tft.setCursor((X + 159), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 60W
    tft.setCursor((X + 169), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 70W
    tft.setCursor((X + 183), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 80W
    tft.setCursor((X + 193), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 90W
    tft.setCursor((X + 194), Y + 175);
    tft.println("100");                                               // display PO = 100W scale text
    tft.setTextColor(RED);
    tft.setCursor((X + 200), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 100W RED
    tft.setCursor((X + 54), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 10W
    tft.setCursor((X + 147), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 50W
    tft.setCursor((X + 248), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 150W
    tft.setTextColor(BLUE);
    tft.setCursor((X + 208), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 110W
    tft.setCursor((X + 218), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 120W
    tft.setCursor((X + 228), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 130W
    tft.setCursor((X + 238), Y + 179);
    tft.println(".");                                                 // display PO = dot scale 140W
    tft.setCursor((X + 248), Y + 175);
    tft.println("W");                                                 // display scale text W
}
void draw_SWR_meter() //Draw SWR meter
{
    tft.fillRect((X - 44), (Y + 11), (X + 244), 21, BLACK); // background of meter to black
    tft.setTextColor(SILVER);           // Tekst kleur
    tft.setCursor(Z, Y + 14);
    tft.setTextSize(2);
    tft.println("SWR");                 // print the meters text
    tft.setTextSize(1);
    tft.drawLine(55, 40, 150, 40, SILVER);
    tft.drawLine(150, 40, 240, 40, ORANGE);
    tft.drawLine(240, 40, 305, 40, RED);
    tft.drawFastVLine(54, 40, 6, SILVER);
    tft.setTextColor(WHITE);
    tft.setCursor(55, Y + 11);
    tft.println("1 . . .");                                               // display SWR = 1 scale text white
    tft.setCursor((X + 50), Y + 11);
    tft.println("1.5");                                             // display SWR = 1.5 scale text white
    tft.setCursor((X + 90), Y + 11);
    tft.println("2");                                               // display SWR = 2 scale text white
    tft.setTextColor(ORANGE);                               // text colour to orange
    tft.setCursor((X + 136), Y + 11);
    tft.println("3");                                               // display SWR = 3 scale text orange
    tft.setTextColor(RED);                                  // text colour to red
    tft.setCursor((X + 223), Y + 11);
    tft.println("5");                                               // display SWR = 5 scale text red
    tft.setCursor((X + 238), Y + 11);
    tft.println("o");                                               // display SWR = oo scale text red
    tft.setCursor((X + 242), Y + 11);
    tft.println("o");
}
void draw_COMP_meter() {
    //Draw COMP meter
    //tft.fillRect((X - 44), (Y + 37), (X + 244), 21, BLACK); // background of meter to black
    tft.setTextColor(SILVER);                                // text colour to white
    tft.setCursor((X - 50), Y + 44);    // Begin van de lijn op een hoogte van 44   (Z, (Y + 44));
    tft.setTextSize(2);
    tft.println("COMP");                                            // print the meters text
    tft.setTextSize(1);
    tft.drawLine(54, 69, 305, 69, SILVER); // Horizontale Lijn 
    tft.drawFastVLine(54, 70, 6, SILVER);
    tft.setCursor((X + 2), Y + 41);
    tft.println("0");                                               // display COMP = 0 scale text white
    tft.setCursor((X + 50), Y + 41);
    tft.println("5");                                               // display COMP = 5 scale text white
    //tft.setTextColor(YELLOW);                               // text colour to yellow
    tft.setCursor((X + 101), Y + 41);
    tft.println("10");                                              // display COMP = 10 scale text yellow
    //tft.setTextColor(ORANGE);                               // text colour to orange
    tft.setCursor((X + 152), Y + 41);
    tft.println("15");                                              // display COMP = 15 scale text orange
    //tft.setTextColor(RED);                                  // text colour to red
    tft.setCursor((X + 203), Y + 41);
    tft.println("20");                                              // display COMP = 20 scale text red
    //tft.setTextColor(WHITE);                                // text colour to white
    tft.setCursor((X + 242), Y + 41);
    tft.println("dB");                                         // display dB scale text white
}

void draw_IDD_meter() {
    //Draw IDD meter
    tft.fillRect((X - 44), (Y + 77), (X + 244), 21, BLACK); // background of meter to black
    tft.setTextColor(SILVER);
    tft.setCursor(Z, (Y + 84));
    tft.setTextSize(2);
    tft.println("IDD");                                             // print the meters text
    tft.setTextSize(1);
    tft.drawLine(54, 109, 305, 109, SILVER);
    tft.drawFastVLine(54, 110, 6, SILVER);
    tft.setCursor((X + 2), Y + 81);
    tft.println("0 . . .");                                               // display IDD = 0 scale text, all scale text in white
    tft.setCursor((X + 50), Y + 81);
    tft.println("5 . . .");                                               // display IDD = 5 scale text
    tft.setCursor((X + 101), Y + 81);
    tft.println("10");                                              // display IDD = 10 scale text
    tft.setCursor((X + 152), Y + 81);
    tft.println("15");                                              // display IDD = 15 scale text
    tft.setCursor((X + 203), Y + 81);
    tft.println("20");                                              // display IDD = 20 scale text
    tft.setCursor((X + 235), Y + 81);
    tft.println("25");                                              // display IDD = 25 scale text
    tft.setCursor((X + 248), Y + 81);
    tft.println("A");                                             // display A scale text
}
void draw_VDD_meter() {
    
    //Draw VDD meter
    tft.setCursor(Z, (Y + 114));
    tft.setTextSize(2);
    tft.println("VDD");                                             // print the meters text
    tft.setTextSize(1);                                              // print the meters text
    tft.drawRect((X - 1), (Y + 106), 257, 23, BLACK);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 106), 44, 23, BLACK);        // draw rectangular around the text
    tft.fillRect(X, (Y + 116), 244, 3, GREEN);
    tft.drawLine(54, 139, 305, 139, SILVER);
    tft.drawFastVLine(54, 140, 6, SILVER);

    tft.fillRect((X + 178), (Y + 107), (X - 25), 12, GREEN); // bar indicating allowed tolerance of VDD in greenyellow
    tft.setTextColor(BLACK);                                   // textcolor set to RED
    tft.setCursor((X + 180), Y + 109);
    tft.println("13.8");                                             // display VDD = 13.8 scale text in red
    tft.setTextColor(WHITE);                                 // make text  = V white
    tft.setCursor((X + 248), Y + 109);
    tft.println("V");                                         // display V scale text
}
void draw_ALC_meter() {

    //Draw ALC meter - ALC meter has different dimensions
    tft.fillRect((X - 44), (Y + 147), (X + 244), 14, BLACK);   // background of meter to black
    tft.drawLine(55, 170, 305, 170, TFT_NAVY);
    tft.drawLine(183, 170, 305, 170, TFT_RED);
    tft.drawLine(54, 174, 305, 174, SILVER);                  // Onderlijn meter
    tft.drawFastVLine(54, 175, 6, SILVER);
    tft.setTextColor(SILVER);
    tft.setTextSize(2);
    tft.setCursor(Z, (Y + 150));
    tft.println("ALC");                                      // print the meters text
    tft.setTextSize(1);
    tft.fillRect(X, (Y + 147), 128, 5, TFT_NAVY);                // white ALC scale

    draw_M_S1_meter();                                      // call routine draw_M_S_meter. S meter differs, it can be substituted by the PO meter

}

// Drawing the touchbuttons on display

void draw_touchbuttons01() {

    // touch buttons
    tft.setTextColor(BLACK);
    tft.drawCircle(290, 230, 10, ILI9341_BACKGROUND);
    tft.fillCircle(290, 230, 10, ILI9341_BACKGROUND);
    tft.setCursor(286, 223);
    tft.setTextSize(2);
    tft.println("T");

    tft.drawCircle(160, 230, 10, ILI9341_BACKGROUND);
    tft.fillCircle(160, 230, 10, ILI9341_BACKGROUND);
    tft.setCursor(156, 223);
    tft.setTextSize(2);
    tft.println("B");

    tft.drawCircle(16, 230, 10, ILI9341_BACKGROUND);
    tft.fillCircle(16, 230, 10, ILI9341_BACKGROUND);
    tft.setCursor(12, 223);
    tft.setTextSize(2);
    tft.println("A");

    tft.setTextColor(WHITE);
    tft.setTextSize(1);
}


// show the different values in the bars on display
void Display()
{
    // Fill the SWR bar
    if (SWR != pSWR) {                                                      // if new SWR is same as previous value, do nothing, no need to draw graphics if they did not change
        if (SWR < pSWR) {
            tft.fillRect((X + SWR), (Y + 24), (255 - SWR), H, BLACK);          // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                     // if new value is higher than previous value, just add to the bar
        if (SWR <= 96) {
            tft.fillRect(X, (Y + 24), SWR, H, GREEN);                          // when SWR is equal or less than 96, make bar green
        }
        if ((SWR > 96) && (SWR <= 128)) {                                     // when SWR between 97 and 128, make first part of bar green and second part yellow
            tft.fillRect(X, (Y + 24), 96, H, GREEN);                         // fill 96 pixels green, start from X, row is Y
            tft.fillRect((96 + X), (Y + 24), (SWR - 96), H, YELLOW);         // fill 96 pixels green, add SWR minus green part as yellow
        }
        if ((SWR > 128) && (SWR <= 192)) {                                    // when SWR between 129 and 192, make this part of bar orange
            tft.fillRect(X, (Y + 24), 96, H, GREEN);                         // first 96 pixels green
            tft.fillRect((96 + X), (Y + 24), 32, H, YELLOW);                 // next 32 pixels yellow
            tft.fillRect((128 + X), (Y + 24), (SWR - 128), H, ORANGE);       // add SWR minus green and yellow part as orange
        }
        if (SWR > 192) {                                                      // when SWR higher than 192, make this part of bar red
            tft.fillRect(X, (Y + 24), 96, H, GREEN);                         // first 96 pixels green
            tft.fillRect((96 + X), (Y + 24), 32, H, YELLOW);                 // next 34 pixels yellow
            tft.fillRect((128 + X), (Y + 24), 64, H, ORANGE);                // add SWR minus green and yellow part as orange
            tft.fillRect((192 + X), (Y + 24), (SWR - 192), H, RED);          // add SWR minus green, yellow and orange part as red
        }
        pSWR = SWR;                                                                // store measured SWR value as previous value
    }



    // Fill the COMP bar

    if (Comp != pComp) {                                                        // if new COMP value is same as previous value, do nothing, no need to draw graphics if they did not change
        if (Comp < pComp) {
            tft.fillRect((X + Comp), (Y + 52), (255 - Comp), H, BLACK);     // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                         // if new value is higher than previous value, just add to the bar
        if (Comp <= 102) {
            tft.fillRect(X, (Y + 52), Comp, H, GREEN);                      // when COMP is equal or less than 102 (10 db), make bar green
        }
        if ((Comp > 102) && (Comp <= 152)) {                                      // when COMP between 102 (10 dB) and 152 (15 dB), make first part of bar green and second part Yellow
            tft.fillRect(X, (Y + 52), 102, H, GREEN);                       // fill 102 pixels green, start from X, row is Y
            tft.fillRect((102 + X), (Y + 52), (Comp - 102), H, YELLOW);     // fill 102 pixels green, add COMP minus green part as yellow
        }
        if ((Comp > 152) && (Comp <= 203)) {                                      // when COMP between 152 (15 dB) and 203 (20 degr), make this part of bar orange
            tft.fillRect(X, (Y + 50), 102, H, GREEN);                       // first 102 pixels green
            tft.fillRect((102 + X), (Y + 52), 50, H, YELLOW);               // next 50 pixels yellow, 0.2 of scale
            tft.fillRect((152 + X), (Y + 52), (Comp - 152), H, ORANGE);     // add COMP minus green and yellow part as orange
        }
        if (Comp > 203) {                                                         // when COMP is higher than 203, (25 dB) make this part of bar red
            tft.fillRect(X, (Y + 50), 102, H, GREEN);                       // first 102  pixels green
            tft.fillRect((102 + X), (Y + 52), 50, H, YELLOW);               // next 50 pixels yellow
            tft.fillRect((152 + X), (Y + 52), 50, H, ORANGE);               // next 50 pixels orange
            tft.fillRect((203 + X), (Y + 52), (Comp - 203), H, RED);        // add COMP minus green, yellow and orange part as red
        }
        pComp = Comp;                                                             // store measured COMP value as previous value
    }



    // Fill the IDD bar
    if (IDD != pIDD) {                                                      // if new IDD is same as previous value, do nothing, no need to draw graphics if they did not change
        if (IDD < pIDD) {
            tft.fillRect((X + IDD), (Y + 92), (255 - IDD), H, BLACK);   // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                     // if new value is higher than previous value, just add to the bar
        tft.fillRect(X, (Y + 92), IDD, H, BLUE);                      // IDD, startpoint,row,value,height of bar
        pIDD = IDD;                                                           // store measured IDD value as previous value
    }



    // Fill the VDD bar
    if (VDD != pVDD) {                                                      // if new VDD is same as previous value, do nothing, no need to draw graphics if they did not change
        if (VDD < pVDD) {
            tft.fillRect((X + VDD), (Y + 122), (255 - VDD), H, BLACK);  // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                     // if new value is higher than previous value, just add to the bar
        tft.fillRect(X, (Y + 122), VDD, H, ILI9341_BACKGROUND);                     // VDD, startpoint,row,value,height of bar
        pVDD = VDD;                                                           // store measured VDD value as previous value
    }



    // Fill the ALC bar                                                        // ALC value according to manual can vary betwee 0 - 255. But measured max value for blue bar is 070, max red = 082.
                                                                               // half scale is 128 pixels, which is CAT output of 70, so correction factor = 128/70 = 1.825 (times output)
    if (ALC != pALC) {                                                         // if new ALC value is same as previous value, do nothing, no need to draw graphics if they did not change
        if (ALC < pALC) {
            tft.fillRect((X + (1.825 * (ALC))), (Y + 157), (255 - (1.825 * (ALC))), H, BLACK); // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                        // if new value is higher than previous value, just add to the bar
        if ((1.825 * (ALC)) <= 128) {
            tft.fillRect(X, (Y + 157), (1.825 * (ALC)), H, TFT_GREENYELLOW);                 // when ALC is equal or less than blue scale, make this part of bar blue
        }
        if ((1.825 * (ALC)) > 128) {
            tft.fillRect((X), (Y + 157), (1.825 * (ALC)), H, TFT_RED);             // if ALC is more than half scale make complete bar orange
        }
        pALC = ALC;                                                                     // store measured ALC value as previous value
    }


    // Fill the S Meter bar
    if (in_tx == false) {                                                      // if we are in RX
        if (SMM != pSMM) {                                                     // if new S meter value is same as previous value, do nothing, no need to draw graphics if they did not change
            if (SMM < pSMM) {
                tft.fillRect((X + SMM), (Y + 187), (255 - SMM), H, BLACK);    // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
            }                                                                       // if new value is higher than previous value, just add to the bar
            tft.fillRect(X, (Y + 187), SMM, H, TFT_GREENYELLOW);
        }

        pSMM = SMM;                                                             // store measured S meter value as previous value
    }
    else {
        if (PO < pPO) {
            tft.fillRect((X + PO), (Y + 187), (255 - PO), H, BLACK);        // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                         // if new value is higher than previous value, just add to the bar
        tft.fillRect(X, (Y + 187), PO, H, BLUE);                          // PO, startpoint,row,value,height of bar
        pPO = PO;                                                                 // store measured PO value as previous value
    }

}


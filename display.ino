// SHOW OPTIONAL TEXT ON DISPLAY
void screen1text() {
    tft.fillScreen(BLACK);
    tft.setTextSize(1);                                             // small text

    //print the text in the upper part of the display
    tft.setCursor((X + 40), (Y - 10));
    tft.setTextColor(GOLD);
    tft.println("FT-991(a) SMART DISPLAY");


    // print the text- connection status & Power set to
    tft.setCursor((X - 45), 82);
    tft.setTextColor(WHITE);
    tft.println("Connection status:");
    tft.setCursor((X + 115), 82);
    tft.setTextColor(WHITE);
    tft.println("Power set to (W):");
    // check status & texting display

}

//SHOW SERIAL STATUS ON DISPLAY
void show_status() {                            // this routine shows a red or green connection statusled on the display



    tft.drawCircle(75 + X, 85, 6, BLACK);         // outline of circle is black
    if (constatus == false) {
        tft.fillCircle(75 + X, 85, 4, RED);         // red led if status is false
    }
    else {
        tft.fillCircle(75 + X, 85, 4, GREEN);       // green led if status is true
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

    //Draw S meter, the S meter can be substituted by the PO meter


    tft.fillRect(X, (Y + 171), (X + 200), 21, BLACK);       // background of meter to black
    tft.drawRect((X - 1), (Y + 170), 257, 23, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 170), 44, 23, WHITE);        // draw rectangular around the text
    tft.setTextColor(WHITE);                                 // set textcolor to white
    tft.setCursor((X + 4), Y + 175);
    tft.println("1");                                                 // display S = 1 scale text, 1 S point = 14 pixels
    tft.setCursor((X + 4), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 11), Y + 175);
    tft.println("2");                                                 // display S = 2 scale text white
    tft.setCursor((X + 11), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 28), Y + 175);
    tft.println("3");                                                 // display S = 3 scale text white
    tft.setCursor((X + 28), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 48), Y + 175);
    tft.println("4");                                                 // display S = 4 scale text white
    tft.setCursor((X + 48), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 65), Y + 175);
    tft.println("5");                                                 // display S = 5 scale text white
    tft.setCursor((X + 65), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 80), Y + 175);
    tft.println("6");                                                 // display S = 6 scale text white
    tft.setCursor((X + 80), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 95), Y + 175);
    tft.println("7");                                                 // display S = 7 scale text white
    tft.setCursor((X + 95), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 113), Y + 175);
    tft.println("8");                                                 // display S = 8 scale text white
    tft.setCursor((X + 113), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 131), Y + 175);
    tft.println("9");                                                 // display S = 9 scale text white
    tft.setCursor((X + 131), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setTextColor(RED);                                    // set textcolor to red
    tft.setCursor((X + 170), Y + 175);
    tft.println("20");                                                // display S = 20 scale text red
    tft.setCursor((X + 170), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 207), Y + 175);
    tft.println("40");                                                // display S = 40 scale text red
    tft.setCursor((X + 207), Y + 185);
    tft.println(".");                                                 // display S = dot scale
    tft.setCursor((X + 240), Y + 175);
    tft.println("60");                                                // display S = 60 scale text red
    tft.setCursor((X + 250), Y + 185);
    tft.println(".");                                                 // display S = dot scale
}
void draw_PO_meter() {

    //Draw PO meter, the PO meter can be substituted by the S meter

    tft.fillRect((X - 44), (Y + 171), (X + 244), 21, BLACK); // background of meter to black
    tft.drawRect((X - 1), (Y + 170), 257, 23, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 170), 44, 23, WHITE);        // draw rectangular around the text
    tft.setCursor(Z, (Y + 178));
    tft.setTextColor(BLUE);                                  // set textcolor to blue
    tft.println("PO");                                               // print meter text
    tft.setCursor((X + 4), Y + 175);
    tft.println("0");                                                 // display PO = 0 scale text, power 0W
    tft.setCursor((X + 32), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 5W
    tft.setCursor((X + 51), Y + 175);
    tft.println("10");                                                // display PO = 10 scale text

    tft.setCursor((X + 82), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 20W
    tft.setCursor((X + 104), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 30W
    tft.setCursor((X + 128), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 40W
    tft.setCursor((X + 147), Y + 175);
    tft.println("50");                                                 // display PO = 50W scale text
    tft.setCursor((X + 159), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 60W
    tft.setCursor((X + 169), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 70W
    tft.setCursor((X + 183), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 80W
    tft.setCursor((X + 193), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 90W
    tft.setCursor((X + 194), Y + 175);
    tft.println("100");                                               // display PO = 100W scale text
    tft.setTextColor(RED);
    tft.setCursor((X + 200), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 100W RED
    tft.setCursor((X + 54), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 10W
    tft.setCursor((X + 147), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 50W
    tft.setCursor((X + 248), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 150W
    tft.setTextColor(BLUE);
    tft.setCursor((X + 208), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 110W
    tft.setCursor((X + 218), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 120W
    tft.setCursor((X + 228), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 130W
    tft.setCursor((X + 238), Y + 185);
    tft.println(".");                                                 // display PO = dot scale 140W
    tft.setCursor((X + 248), Y + 175);
    tft.println("W");                                                 // display scale text W
}
void draw_SWR_meter() //Draw SWR meter
{
    tft.fillRect((X - 44), (Y + 7), (X + 244), 21, BLACK); // background of meter to black
    tft.setTextColor(WHITE);                                // text colour to white
    tft.setCursor(Z, Y + 14);
    tft.println("SWR");                                             // print the meters text
    tft.drawRect((X - 1), (Y + 6), 257, 23, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 6), 44, 23, WHITE);        // draw rectangular around the text
    tft.setCursor((X + 2), Y + 11);
    tft.println("1");                                               // display SWR = 1 scale text white
    tft.setCursor((X + 48), Y + 11);
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
    tft.fillRect((X - 44), (Y + 37), (X + 244), 21, BLACK); // background of meter to black
    tft.setTextColor(WHITE);                                // text colour to white
    tft.setCursor(Z, (Y + 44));
    tft.println("COMP");                                            // print the meters text
    tft.drawRect((X - 1), (Y + 36), 257, 23, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 36), 44, 23, WHITE);        // draw rectangular around the text
    tft.setCursor((X + 2), Y + 41);
    tft.println("0");                                               // display COMP = 0 scale text white
    tft.setCursor((X + 50), Y + 41);
    tft.println("5");                                               // display COMP = 5 scale text white
    tft.setTextColor(YELLOW);                               // text colour to yellow
    tft.setCursor((X + 101), Y + 41);
    tft.println("10");                                              // display COMP = 10 scale text yellow
    tft.setTextColor(ORANGE);                               // text colour to orange
    tft.setCursor((X + 152), Y + 41);
    tft.println("15");                                              // display COMP = 15 scale text orange
    tft.setTextColor(RED);                                  // text colour to red
    tft.setCursor((X + 203), Y + 41);
    tft.println("20");                                              // display COMP = 20 scale text red
    tft.setTextColor(WHITE);                                // text colour to white
    tft.setCursor((X + 242), Y + 41);
    tft.println("dB");                                              // display dB scale text white
}
void draw_IDD_meter() {
    //Draw IDD meter
    tft.fillRect((X - 44), (Y + 77), (X + 244), 21, BLACK); // background of meter to black
    tft.setCursor(Z, (Y + 84));
    tft.println("IDD");                                             // print the meters text
    tft.drawRect((X - 1), (Y + 76), 257, 23, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 76), 44, 23, WHITE);        // draw rectangular around the text
    tft.setCursor((X + 2), Y + 81);
    tft.println("0");                                               // display IDD = 0 scale text, all scale text in white
    tft.setCursor((X + 50), Y + 81);
    tft.println("5");                                               // display IDD = 5 scale text
    tft.setCursor((X + 101), Y + 81);
    tft.println("10");                                              // display IDD = 10 scale text
    tft.setCursor((X + 152), Y + 81);
    tft.println("15");                                              // display IDD = 15 scale text
    tft.setCursor((X + 203), Y + 81);
    tft.println("20");                                              // display IDD = 20 scale text
    tft.setCursor((X + 235), Y + 81);
    tft.println("25");                                              // display IDD = 25 scale text
    tft.setCursor((X + 248), Y + 81);
    tft.println("A");                                               // display A scale text
}
void draw_VDD_meter() {
    //Draw VDD meter
    tft.fillRect((X - 44), (Y + 107), (X + 244), 21, BLACK); // background of meter to black
    tft.setCursor(Z, (Y + 114));
    tft.println("VDD");                                              // print the meters text
    tft.drawRect((X - 1), (Y + 106), 257, 23, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 106), 44, 23, WHITE);        // draw rectangular around the text
    tft.fillRect(X, (Y + 107), 244, 4, WHITE);

    //  tft.fillRect((X + 178), (Y + 107), (X - 25), 12, GREEN,YELLOW);
    tft.fillRect((X + 178), (Y + 107), (X - 25), 12, GREEN);// bar indicating allowed tolerance of VDD in greenyellow
    tft.setTextColor(RED);                                   // textcolor set to red
    tft.setCursor((X + 180), Y + 109);
    tft.println("13.8");                                             // display VDD = 13.8 scale text in red
    tft.setTextColor(WHITE);                                 // make text  = V white
    tft.setCursor((X + 248), Y + 111);
    tft.println("V");                                                // display V scale text
}
void draw_ALC_meter() {

    //Draw ALC meter - ALC meter has different dimensions
    tft.fillRect((X - 44), (Y + 147), (X + 244), 14, BLACK); // background of meter to black
    tft.setCursor(Z, (Y + 150));
    tft.println("ALC");                                      // print the meters text in white
    tft.drawRect((X - 1), (Y + 146), 257, 16, WHITE);        // draw rectangular around the bar
    tft.drawRect((X - 45), (Y + 146), 44, 16, WHITE);        // draw rectangular around the text
    tft.fillRect(X, (Y + 147), 128, 5, WHITE);               // white ALC scale

    draw_M_S1_meter();                                      // call routine draw_M_S_meter. S meter differs, it can be substituted by the PO meter

}

// Drawing the touchbuttons on display

void draw_touchbuttons01() {

    // touch buttons
    tft.setTextColor(BLACK);
    tft.drawCircle(290, 225, 10, ILI9341_BACKGROUND);
    tft.fillCircle(290, 225, 10, ILI9341_BACKGROUND);
    tft.setCursor(286, 218);
    tft.setTextSize(2);
    tft.println("T");

    tft.drawCircle(160, 225, 10, ILI9341_BACKGROUND);
    tft.fillCircle(160, 225, 10, ILI9341_BACKGROUND);
    tft.setCursor(156, 218);
    tft.setTextSize(2);
    tft.println("B");

    tft.drawCircle(16, 225, 10, ILI9341_BACKGROUND);
    tft.fillCircle(16, 225, 10, ILI9341_BACKGROUND);
    tft.setCursor(12, 218);
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
            tft.fillRect((X + SWR), (Y + 20), (255 - SWR), H, BLACK);          // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                     // if new value is higher than previous value, just add to the bar
        if (SWR <= 96) {
            tft.fillRect(X, (Y + 20), SWR, H, GREEN);                          // when SWR is equal or less than 96, make bar green
        }
        if ((SWR > 96) && (SWR <= 128)) {                                     // when SWR between 97 and 128, make first part of bar green and second part yellow
            tft.fillRect(X, (Y + 20), 96, H, GREEN);                         // fill 96 pixels green, start from X, row is Y
            tft.fillRect((96 + X), (Y + 20), (SWR - 96), H, YELLOW);         // fill 96 pixels green, add SWR minus green part as yellow
        }
        if ((SWR > 128) && (SWR <= 192)) {                                    // when SWR between 129 and 192, make this part of bar orange
            tft.fillRect(X, (Y + 20), 96, H, GREEN);                         // first 96 pixels green
            tft.fillRect((96 + X), (Y + 20), 32, H, YELLOW);                 // next 32 pixels yellow
            tft.fillRect((128 + X), (Y + 20), (SWR - 128), H, ORANGE);       // add SWR minus green and yellow part as orange
        }
        if (SWR > 192) {                                                      // when SWR higher than 192, make this part of bar red
            tft.fillRect(X, (Y + 20), 96, H, GREEN);                         // first 96 pixels green
            tft.fillRect((96 + X), (Y + 20), 32, H, YELLOW);                 // next 34 pixels yellow
            tft.fillRect((128 + X), (Y + 20), 64, H, ORANGE);                // add SWR minus green and yellow part as orange
            tft.fillRect((192 + X), (Y + 20), (SWR - 192), H, RED);          // add SWR minus green, yellow and orange part as red
        }
        pSWR = SWR;                                                                // store measured SWR value as previous value
    }



    // Fill the COMP bar

    if (Comp != pComp) {                                                        // if new COMP value is same as previous value, do nothing, no need to draw graphics if they did not change
        if (Comp < pComp) {
            tft.fillRect((X + Comp), (Y + 50), (255 - Comp), H, BLACK);     // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                         // if new value is higher than previous value, just add to the bar
        if (Comp <= 102) {
            tft.fillRect(X, (Y + 50), Comp, H, GREEN);                      // when COMP is equal or less than 102 (10 db), make bar green
        }
        if ((Comp > 102) && (Comp <= 152)) {                                      // when COMP between 102 (10 dB) and 152 (15 dB), make first part of bar green and second part Yellow
            tft.fillRect(X, (Y + 50), 102, H, GREEN);                       // fill 102 pixels green, start from X, row is Y
            tft.fillRect((102 + X), (Y + 50), (Comp - 102), H, YELLOW);     // fill 102 pixels green, add COMP minus green part as yellow
        }
        if ((Comp > 152) && (Comp <= 203)) {                                      // when COMP between 152 (15 dB) and 203 (20 degr), make this part of bar orange
            tft.fillRect(X, (Y + 50), 102, H, GREEN);                       // first 102 pixels green
            tft.fillRect((102 + X), (Y + 50), 50, H, YELLOW);               // next 50 pixels yellow, 0.2 of scale
            tft.fillRect((152 + X), (Y + 50), (Comp - 152), H, ORANGE);     // add COMP minus green and yellow part as orange
        }
        if (Comp > 203) {                                                         // when COMP is higher than 203, (25 dB) make this part of bar red
            tft.fillRect(X, (Y + 50), 102, H, GREEN);                       // first 102  pixels green
            tft.fillRect((102 + X), (Y + 50), 50, H, YELLOW);               // next 50 pixels yellow
            tft.fillRect((152 + X), (Y + 50), 50, H, ORANGE);               // next 50 pixels orange
            tft.fillRect((203 + X), (Y + 50), (Comp - 203), H, RED);        // add COMP minus green, yellow and orange part as red
        }
        pComp = Comp;                                                             // store measured COMP value as previous value
    }



    // Fill the IDD bar
    if (IDD != pIDD) {                                                      // if new IDD is same as previous value, do nothing, no need to draw graphics if they did not change
        if (IDD < pIDD) {
            tft.fillRect((X + IDD), (Y + 90), (255 - IDD), H, BLACK);   // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                     // if new value is higher than previous value, just add to the bar
        tft.fillRect(X, (Y + 90), IDD, H, BLUE);                      // IDD, startpoint,row,value,height of bar
        pIDD = IDD;                                                           // store measured IDD value as previous value
    }



    // Fill the VDD bar
    if (VDD != pVDD) {                                                      // if new VDD is same as previous value, do nothing, no need to draw graphics if they did not change
        if (VDD < pVDD) {
            tft.fillRect((X + VDD), (Y + 120), (255 - VDD), H, BLACK);  // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                     // if new value is higher than previous value, just add to the bar
        tft.fillRect(X, (Y + 120), VDD, H, BLUE);                     // VDD, startpoint,row,value,height of bar
        pVDD = VDD;                                                           // store measured VDD value as previous value
    }



    // Fill the ALC bar                                                        // ALC value according to manual can vary betwee 0 - 255. But measured max value for blue bar is 070, max red = 082.
                                                                               // half scale is 128 pixels, which is CAT output of 70, so correction factor = 128/70 = 1.825 (times output)
    if (ALC != pALC) {                                                         // if new ALC value is same as previous value, do nothing, no need to draw graphics if they did not change
        if (ALC < pALC) {
            tft.fillRect((X + (1.825 * (ALC))), (Y + 153), (255 - (1.825 * (ALC))), H, BLACK); // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                        // if new value is higher than previous value, just add to the bar
        if ((1.825 * (ALC)) <= 128) {
            tft.fillRect(X, (Y + 153), (1.825 * (ALC)), H, BLUE);                 // when ALC is equal or less than blue scale, make this part of bar blue
        }
        if ((1.825 * (ALC)) > 128) {
            tft.fillRect((X), (Y + 153), (1.825 * (ALC)), H, ORANGE);             // if ALC is more than half scale make complete bar orange
        }
        pALC = ALC;                                                                     // store measured ALC value as previous value
    }


    // Fill the S Meter bar
    if (in_tx == false) {                                                      // if we are in RX
        if (SMM != pSMM) {                                                        // if new S meter value is same as previous value, do nothing, no need to draw graphics if they did not change
            if (SMM < pSMM) {
                tft.fillRect((X + SMM), (Y + 184), (255 - SMM), H, BLACK);    // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
            }                                                                       // if new value is higher than previous value, just add to the bar
            tft.fillRect(X, (Y + 184), SMM, H, GREEN);                      // S meter value, startpoint,row,value,height of bar
            pSMM = SMM;                                                             // store measured S meter value as previous value
        }
    }
    else {
        if (PO < pPO) {
            tft.fillRect((X + PO), (Y + 184), (255 - PO), H, BLACK);        // if new value < previous value, clear the bar partly. not whole bar but from value to 255, reduces flicker
        }                                                                         // if new value is higher than previous value, just add to the bar
        tft.fillRect(X, (Y + 184), PO, H, BLUE);                          // PO, startpoint,row,value,height of bar
        pPO = PO;                                                                 // store measured PO value as previous value
    }

}


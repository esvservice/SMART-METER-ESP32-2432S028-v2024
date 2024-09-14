void cat()

{
    // Forward declarations (if needed)
    void convert_CAT_buffer();







    // check if in TX, if yes then draw PO meter, otherwise draw S meter, do this only once to avoid flicker

    Serial.print("TX;");                                              // send CAT command to the radio, ask for TX status
    get_radio_response();                                             // call routine to read from radio
    if ((((CAT_buffer.startsWith("TX2")) || (CAT_buffer.startsWith("TX1"))) && (txrx_flag1 == false))) {  // ask if the transceiver is in TX (PTT or CAT, for the first loop)
        draw_PO_meter();                                                // draw the PO meter outline, can be substituted by S meter
        in_tx = true;                                                   // set flag we are in TX
        txrx_flag1 = true;                                              // flag so that text and clear is only written once, toggle function
        txrx_flag2 = true;                                              // flag so that text and clear is only written once, toggle function
        main_flag1 = false;                                             // after TX reset flag to check main receiver status again, toggle function
        main_flag2 = true;                                              // after TX reset flag to check main receiver status again, toggle function
    }
    else {
        if ((txrx_flag2 == true) && (CAT_buffer == ("TX0"))) {          // we are in RX, check if this is the first RX loop
            in_tx = false;                                                // reset tx flag to false
            draw_M_S1_meter();                                             // call draw S meter outline, can be substituted by PO meter
            txrx_flag1 = false;                                           // flag so that text and clear is only written once, toggle function
            txrx_flag2 = false;                                           // flag so that text and clear is only written once, toggle function
        }
    }
    if (in_tx == false) {                                             // not in TX, now read S meter
        Serial.print("RM1;");                                          // send CAT command to the radio, ask S meter reading
        get_radio_response();                                           // call routine to read from radio
        convert_CAT_buffer();                                           // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
        SMM = CAT_buffer.toInt();                                       // store string as int in SSM to be displayed as S meter bar
        if (main_flag1 == false) {                                      // clear area and write the text S, but only once to reduce flicker
            tft.fillRect((X - 44), (Y + 172), 42, 20, BLACK);     // clear the text area
            tft.setCursor(Z, (Y + 178));
            tft.setTextColor(SILVER);                              // set textcolor to white
            tft.setTextSize(2);
            tft.println("  S");          // display text of receiver meter bar
            tft.setTextSize(1);                                             // display text of receiver meter bar
        }
        main_flag1 = true;                                              // flag so that text and clear is only written once, toggle function
        main_flag2 = true;                                              // flag so that text and clear is only written once, toggle function
    }

    // read Freq VFO A
    Serial.print("FA;");                                     // send CAT command to the radio, ask comp value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    FreqA = CAT_buffer.toInt();                                // store string as int in freqa to be displayed as text

    // read Freq VFO B
    Serial.print("FB;");                                     // send CAT command to the radio, ask comp value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    FreqB = CAT_buffer.toInt();                              // store string as int in FreqB to be displayed as text



    // read comp meter
    Serial.print("RM3;");                                    // send CAT command to the radio, ask comp value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    Comp = CAT_buffer.toInt();                                // store string as int in Comp to be displayed as Comp meter bar

    // read ALC meter
    Serial.print("RM4;");                                    // send CAT command to the radio, ask ALC value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    ALC = CAT_buffer.toInt();                                // store string as int in ALC to be displayed as ALC meter bar

    //read power meter
    Serial.print("RM5;");                                  // send CAT command to the radio, ask PO value
    get_radio_response();                                  // call routine to read from radio
    convert_CAT_buffer();                                  // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    PO = CAT_buffer.toInt();                               // store string as int in PO to be displayed as power meter bar

    // read SWR meter
    Serial.print("RM6;");                                    // send CAT command to the radio, ask SWR value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    SWR = CAT_buffer.toInt();                                // store string as int in ALC to be displayed as SWR meter bar

    // read IDD meter
    Serial.print("RM7;");                                    // send CAT command to the radio, ask IDD value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    IDD = CAT_buffer.toInt();                                // store string as int in IDD to be displayed as IDD meter bar

    // read VDD meter
    Serial.print("RM8;");                                    // send CAT command to the radio, ask VDD value
    get_radio_response();                                    // call routine to read from radio
    convert_CAT_buffer();                                    // CAT_buffer holds received string in format: RMNVVV000; N=meternumber, VVV is wanted value
    VDD = CAT_buffer.toInt();                                // store string as int in VDD to be displayed as VDD meter bar


    // read and display power setting from transceiver, only write to screen when changed
    Serial.print("PC;");                                              // send CAT command to the radio, ask what power has been set
    get_radio_response();                                             // call routine to read from radio
    CAT_buffer.remove(0, 2);                                          // remove characters PC
    if (CAT_buffer.startsWith("0")) {                                 // remove leading zero if present
        CAT_buffer.remove(0, 1);
        if (CAT_buffer.startsWith("0")) {                               // remove leading zero if present
            CAT_buffer.remove(0, 1);
        }
    }
    pwrsetting = CAT_buffer;                                          // present setting to pwrsetting
    if (prevpwrsetting != pwrsetting)                               // if setting has changed since last read
    {
        // pwrsetting = 55; //debug
        tft.setTextColor(YELLOW);
        tft.fillRect((X + 218), 82, (20), 10, BLACK);  // clear previous display
        tft.setCursor((X + 222), 82);
        tft.setTextSize(1);
        tft.println(pwrsetting);                                         // display power setting, it has changed
    }
    prevpwrsetting = pwrsetting;                                      // present setting now becomes previous setting
    tft.setTextSize(1);

    Serial.print("FA;");                                              // send CAT command to the radio, ask what power has been set
    get_radio_response();                                             // call routine to read from radio
    CAT_buffer.remove(0, 2);                                          // remove characters PC
    if (CAT_buffer.startsWith("0")) {                                 // remove leading zero if present
        CAT_buffer.remove(0, 1);
        if (CAT_buffer.startsWith("0")) {                               // remove leading zero if present
            CAT_buffer.remove(0, 1);
        }
    }

    FreqA = CAT_buffer;                                          // present setting to freqasetting

    Serial.print("FB;");                                              // send CAT command to the radio, ask what power has been set
    get_radio_response();                                             // call routine to read from radio
    CAT_buffer.remove(0, 2);                                          // remove characters PC
    if (CAT_buffer.startsWith("0")) {                                 // remove leading zero if present
        CAT_buffer.remove(0, 1);
        if (CAT_buffer.startsWith("0")) {                               // remove leading zero if present
            CAT_buffer.remove(0, 1);
        }
    }

    FreqB = CAT_buffer;                                          // present setting to freqBsetting





    if (prevfreqA != FreqA)                                 // if setting has changed since last read
    {
        tft.setTextColor(WHITE);
        tft.fillRect((X - 44), -10, (60), 30, BLACK); // clear previous display
        tft.setCursor((X - 44), (Y - 10));
        tft.setTextSize(1);
        tft.println(FreqA);                                         // display freqA, it has changed
    }
    prevfreqA = FreqA;                                      // present setting now becomes previous setting
    tft.setTextSize(1);

}




void get_radio_response()                                         // this routine receives the meter values from the radio
{

    // set a timeout value for if we do not get an answer from the radio, time out is non blocking

    current_millis = millis();                                // get the current time
    timeout = current_millis + timeout_delay;                 // calculate the timeout time

    // check for millis() rollover condition - the Arduino millis() counter rolls over about every 47 days
    if (timeout < current_millis)                             // we've calculated the timeout during a millis() rollover event
    {
        timeout = timeout_delay;              // go ahead and calculate as if we've rolled over already (adds a few millis to the timeout delay)
    }

    // start to receive CAT response from the radio
    receiving = true;                                         // start receiving
    CAT_buffer = "";                                          // clear CAT buffer
    do
    {
        if (millis() > timeout)                                 // no data received within timeout delay
        {
            // there is a time out - exit thru break
            receiving = false;                                    // clear receive flag
            tune = false;                                         // do not start tuning, because no info received
            CAT_buffer = "";                                      // clear buffer
            break;                                                // no receive response, exit to loop
        }
        if (Serial.available() && receiving)                    // if there's a character in the rx buffer and we are receiving
        {
            rx_char = Serial.read();                              // get one character at the time from radio
            if (rx_char == ';')                                   // ";" indicates the end of the response from the radio
            {
                receiving = false;                                  // turn off the ok to receive flag, this was last character
                tune = true;                                        // a complete answer is received, now we are allowed to proceed,
            }
            else
            {
                CAT_buffer = CAT_buffer + rx_char;                  // add the received character to the CAT rx string, build the received string
            }
        }
    } while (receiving);                                      // keep looping while ok to receive data from the radio
}

void convert_CAT_buffer()                                   // get the wanted data out of the received string ( wanted data is 0 - 255)
{
    CAT_buffer.remove(0, 3);                                  // delete the text RMN from string (string was RMNVVV000;), now VVV000;
    CAT_buffer.remove(3, 4);                                  // delete the text 000; from string (string was VVV000;), now VVV
}


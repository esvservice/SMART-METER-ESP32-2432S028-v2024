void Tuner() {

    // this routine is called to check if tune button is pressed, if so tune

    val = digitalRead(switchPin);                           // read Tune button and store it in val
    delaycounter = delaycounter + 1;                        // increment delaycounter
    if (delaycounter == 3) {                                // when counter has done 3 loops, keep loop speed, non blocking delay
        delaycounter = 0;                                     // reset counter
        val2 = digitalRead(switchPin);                          // read Tune button again for bounces
        if (val == val2) {                                      // when no bounce
            if (val == HIGH) {                                    // the Tune button is pressed, and no bounce
                if (buttonpress == false) {                         // input and output CAT commands only once when button pressed (it is in loop)
                    Serial.print("PC;");                              // send the request Power Command to radio, call send routine
                    get_radio_response();                             // receive the present (power) setting from radio
                    CAT_buffer = CAT_buffer + ";";                    // add ";" to buffer, terminator is needed for restore command send to radio
                    prevpwr = CAT_buffer;                             // prevpwr holds original power setting to restore later
                    Serial.print("MD0;");                             // send the request Mode Command to radio, call send routine
                    get_radio_response();                             // receive the present (mode) setting from radio
                    CAT_buffer = CAT_buffer + ";";                    // add ";" to buffer, terminator is needed for restore command send to radio
                    prevmode = CAT_buffer;                            // prevmode holds original mode setting to restore later
                    if (tune == true) {                               // valid power/mode info is received from radio, we can set tune mode & power now
                        Serial.print("MD0B;");                          // set mode to FM-N, call send routine
                        Serial.print(set_tune_pwr);                     // send the tune power to the radio
                        delay(35);                                      // wait before starting to transmit
                        Serial.print("MX1;");                           // MOX on, transmitting, now do your tuning
                        tune = false;                                   // tuning flag reset, be ready for next button press.
                    }
                }
                buttonpress = true;                                 // Tune button was pressed and now we
                buttonrelease = true;                               // do Tune button release
            }
            if ((val == LOW) && (buttonrelease == true)) {          // now the Tune button has been released
                Serial.print("MX0;");                                 // MOX off, stop transmitting, stop tune signal
                delay(35);                                            // wait until TX is switched off before sending CAT commands.
                Serial.print(prevmode);                               // restore original mode setting to the radio, mode first then power
                Serial.print(prevpwr);                                // restore original power setting to the radio
                buttonrelease = false;                                // reset flag, Tune button was released
                buttonpress = false;                                  // reset flag, Tune button not pressed anymore
            }

        }
    }

}

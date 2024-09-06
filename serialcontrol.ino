void serialcontrol()
{
    // check rs232 connection
    do
    {
        t = Serial.read();                                              // this is to empty the input buffer when starting reading data from radio,
    } while (Serial.available() > 0);                                 // buffer contains erratic data when the radio is switched on (fix power up sequence)
    Serial.print("TX;");                                              // send CAT command to the radio, ask for TX status
    get_radio_response();                                             // call routine to read from radio
    if (CAT_buffer.startsWith("TX")) {                                // if the answer is correct (it should start with TX)
        constatus = true;                                               // connection status is ok, green light must be turned on
    }
    else {                                                            // we do not get a (correct-) answer
        constatus = false;                                              // red light must be turned on, no connection with radio, bad data/baudrate or radio powered off
    }
}

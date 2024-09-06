void TouchScreen() {
    while (touch.touched())

    {
        rawLocation = touch.getPoint();
        xt = rawLocation.x / 10;
        yt = rawLocation.y / 10;
        if ((xt > 280 && xt < 370) && (yt > 200 && yt < 345)) // waar T staat
        {

            //touchTune();
            tft.drawCircle(160, 120, 120, RED);
            tft.fillCircle(160, 120, 120, RED);
            tft.setCursor(80, 100);
            tft.setTextSize(8);
            tft.setTextColor(YELLOW);
            tft.println("BANG");
            tuneflag = true; // set flag true
            
            
        }

        //xt = 0;
        // yt = 0;
    }

    if (tuneflag == true) {
        if ((xt > 180 && xt < 240) && (yt > 180 && yt < 240)) {
            // code if both statements are true
            wifisetup32(); // zorgt ervoor dat er een wifireset gebeurt => WifiManager32.ino
            screen1text();
            draw_radio_screen();
            show_status();
            Display();
            tuneflag = false; // set flag false
        }
    }






}




void debug() //used to set up buttons
{
    Serial.println("xt");
    Serial.println(xt);
    Serial.println("yt");
    Serial.println(yt);
    //delay(2000);
}

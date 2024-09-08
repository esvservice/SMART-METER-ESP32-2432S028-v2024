#pragma once
//----------------------------------------------------------------------
// https://github.com/lovyan03/LovyanGFX/blob/master/examples/HowToUse/2_user_setting/2_user_setting.ino
//----------------------------------------------------------------------
class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ILI9341 _panel_instance;      // set for ESP32-2432S028 with one USB connector
    //lgfx::Panel_ST7789 _panel_instance;   // set for new ESP32-2432S028 (2 USB connectors)
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;
    lgfx::Touch_XPT2046 _touch_instance;
    //----------------------------------------------------------------------  
public:LGFX(void) {
    {                            // Set up bus control settings
        auto cfg = _bus_instance.config(); // Obtain the structure for bus settings
        // SPI bus settings
        cfg.spi_host = HSPI_HOST;  // Select the SPI to use (VSPI_HOST or HSPI_HOST)
        cfg.spi_mode = 0;          // Set SPI communication mode (0 ~ 3)
        cfg.freq_write = 40000000;   // SPI clock speed for transmission (Max 80MHz, rounded to an integer divisor of 80MHz)
        cfg.freq_read = 16000000;   // SPI clock speed for reception
        cfg.spi_3wire = false;      // Set to true if receiving on the MOSI pin
        cfg.use_lock = true;       // Set to true if using transaction locks
        cfg.dma_channel = 1;         // Set DMA channel to use (1 or 2, 0=disable)
        cfg.pin_sclk = 14;         // Set SPI SCLK pin number
        cfg.pin_mosi = 13;         // Set SPI MOSI pin number
        cfg.pin_miso = 12;         // Set SPI MISO pin number (-1 = disable)
        cfg.pin_dc = 2;         // Set SPI D/C pin number (-1 = disable)
        // If sharing the SPI bus with an SD card, be sure to set MISO and do not omit it.
        _bus_instance.config(cfg);   // Apply the settings to the bus
        _panel_instance.setBus(&_bus_instance); // Set the bus for the panel
    }
    {                            // Set up display panel control settings
        auto cfg = _panel_instance.config(); // Obtain the structure for display panel settings
        cfg.pin_cs = 15; // Pin number where CS is connected (-1 = disable)
        cfg.pin_rst = -1; // Pin number where RST is connected (-1 = disable)
        cfg.pin_busy = -1; // Pin number where BUSY is connected (-1 = disable)
        cfg.memory_width = 240; // Maximum width supported by the driver IC
        cfg.memory_height = 320; // Maximum height supported by the driver IC
        cfg.panel_width = 240; // Actual displayable width
        cfg.panel_height = 320; // Actual displayable height
        cfg.offset_x = 0; // X-axis offset of the panel
        cfg.offset_y = 0; // Y-axis offset of the panel
        cfg.offset_rotation = 0; // Offset value for rotation direction 0~7 (4~7 for upside-down)
        cfg.dummy_read_pixel = 8; // Number of dummy reads before pixel read
        cfg.dummy_read_bits = 1; // Number of dummy reads before data read
        cfg.readable = true; // Set to true if data reading is possible
        cfg.invert = false; // Set to true if the brightness of the panel is inverted
        cfg.rgb_order = false; // Set to true if red and blue are swapped on the panel
        cfg.dlen_16bit = false; // Set to true if the panel transmits data in 16-bit units
        cfg.bus_shared = false; // Set to true if sharing the bus with an SD card
        _panel_instance.config(cfg);
    }
    { // Set up backlight control settings (delete if not needed)
        auto cfg = _light_instance.config(); // Obtain the structure for backlight settings
        cfg.pin_bl = 21;             // Pin number where the backlight is connected
        cfg.invert = false;          // Set to true if the backlight brightness should be inverted
        cfg.freq = 44100;          // PWM frequency for the backlight
        cfg.pwm_channel = 7;         // PWM channel number to use
        _light_instance.config(cfg);
        _panel_instance.setLight(&_light_instance); // Set the backlight for the panel
    }
    { // Set up touchscreen control settings (delete if not needed)
        auto cfg = _touch_instance.config();
        cfg.x_min = 300;    // Minimum X value obtained from the touchscreen (raw value)
        cfg.x_max = 3900;   // Maximum X value obtained from the touchscreen (raw value)
        cfg.y_min = 200;    // Minimum Y value obtained from the touchscreen (raw value)
        cfg.y_max = 3700;   // Maximum Y value obtained from the touchscreen (raw value)
        cfg.pin_int = 36;     // Pin number where INT is connected
        cfg.bus_shared = false;  // Set to true if sharing the bus with the screen
        cfg.offset_rotation = 6; // Adjustment for touchscreen and display orientation mismatch (set with a value from 0 to 7)
        // For SPI connection
        cfg.spi_host = VSPI_HOST;// Select the SPI to use (HSPI_HOST or VSPI_HOST)
        cfg.freq = 1000000;      // Set SPI clock speed
        cfg.pin_sclk = 25;       // Pin number where SCLK is connected
        cfg.pin_mosi = 32;       // Pin number where MOSI is connected
        cfg.pin_miso = 39;       // Pin number where MISO is connected
        cfg.pin_cs = 33;       // Pin number where CS is connected
        _touch_instance.config(cfg);
        _panel_instance.setTouch(&_touch_instance);  // Set the touchscreen for the panel
    }
    setPanel(&_panel_instance); // Set the panel to use
}
};
LGFX tft; // Create an instance of the prepared class

//=====================================================================

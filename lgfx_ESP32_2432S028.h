#define LGFX_USE_V1
#include <LovyanGFX.hpp>  // Vergeet niet de juiste library te includen
#include "variables.h"


//----------------------------------------------------------------------
// LGFX class met ondersteuning voor meerdere displays via autodetectie
//----------------------------------------------------------------------
class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ILI9341 _panel_ili9341;  // Display driver voor ILI9341
    lgfx::Panel_ST7789 _panel_st7789;    // Display driver voor ST7789
    lgfx::Bus_SPI _bus_instance;         // SPI-bus
    lgfx::Light_PWM _light_instance;     // Backlight
    lgfx::Touch_XPT2046 _touch_instance; // Touchscreen instance voor beide versies

public:
    // Constructor met versie detectie
    LGFX(bool isVersion3) {
        if (isVersion3) {
            // Versie 3 (ST7789)
            auto cfg = _panel_st7789.config();  // Haal ST7789 configuratie op
            cfg.pin_cs = 15;
            cfg.pin_rst = -1;
            cfg.pin_busy = -1;
            cfg.memory_width = 240;
            cfg.memory_height = 320;
            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 3; // Stel de rotatie in (0, 1, 2, 3 of meer voor spiegelingen)

            // Voeg kleurdiepte toe
            //cfg.dlen_16bit = true;  // Stel in op true voor 16-bit kleurdiepte (565 RGB)
            cfg.dlen_16bit = false;  // Voor 8-bit kleurdiepte

            _panel_st7789.config(cfg);  // Pas de instellingen toe
            _panel_st7789.setBus(&_bus_instance);  // Koppel de SPI-bus aan het ST7789 paneel
            setPanel(&_panel_st7789);  // Zet het ST7789 paneel actief
        }
        else {
            // Versie 1 (ILI9341)
            auto cfg = _panel_ili9341.config();  // Haal ILI9341 configuratie op
            cfg.pin_cs = 15;
            cfg.pin_rst = -1;
            cfg.pin_busy = -1;
            cfg.memory_width = 240;
            cfg.memory_height = 320;
            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 1; // Stel de rotatie in (0, 1, 2, 3 of meer voor spiegelingen)

            // Voeg kleurdiepte toe
            //cfg.dlen_16bit = true;  // Stel in op true voor 16-bit kleurdiepte (565 RGB)
            cfg.dlen_16bit = false;  // Voor 8-bit kleurdiepte

            _panel_ili9341.config(cfg);  // Pas de instellingen toe
            _panel_ili9341.setBus(&_bus_instance);  // Koppel de SPI-bus aan het ILI9341 paneel
            setPanel(&_panel_ili9341);  // Zet het ILI9341 paneel actief
        }

        // SPI-configuratie die hetzelfde is voor beide versies
        auto bus_cfg = _bus_instance.config();
        bus_cfg.spi_host = HSPI_HOST;
        bus_cfg.spi_mode = 0;
        bus_cfg.freq_write = 40000000;
        bus_cfg.freq_read = 16000000;
        bus_cfg.spi_3wire = false;
        bus_cfg.use_lock = true;
        bus_cfg.dma_channel = 1;
        bus_cfg.pin_sclk = 14;
        bus_cfg.pin_mosi = 13;
        bus_cfg.pin_miso = 12;
        bus_cfg.pin_dc = 2;
        _bus_instance.config(bus_cfg);

        // Backlight-instellingen die hetzelfde zijn voor beide versies
        auto light_cfg = _light_instance.config();
        light_cfg.pin_bl = 21;
        light_cfg.invert = false;
        light_cfg.freq = 44100;
        light_cfg.pwm_channel = 7;
        _light_instance.config(light_cfg);
        getPanel()->setLight(&_light_instance);  // Stel backlight in

        // Touchscreen configuratie toepassen
        setupTouch();
    }

    // Functie om het touchscreen in te stellen (voor beide versies hetzelfde)
    void setupTouch() {
        auto cfg = _touch_instance.config();    // Haal touchscreen configuratie op
        cfg.x_min = 300;                        // Minimum X-waarde
        cfg.x_max = 3900;                       // Maximum X-waarde
        cfg.y_min = 200;                        // Minimum Y-waarde
        cfg.y_max = 3700;                       // Maximum Y-waarde
        cfg.pin_int = 36;                       // Interrupt pin voor touch
        cfg.bus_shared = false;                 // Bus niet gedeeld met scherm
        cfg.offset_rotation = 5;                // Correctie voor oriëntatie
        cfg.spi_host = VSPI_HOST;               // Gebruik VSPI voor touch
        cfg.freq = 1000000;                     // SPI frequentie voor touch
        cfg.pin_sclk = 25;                      // SCLK pin voor touch
        cfg.pin_mosi = 32;                      // MOSI pin voor touch
        cfg.pin_miso = 39;                      // MISO pin voor touch
        cfg.pin_cs = 33;                        // Chip select pin voor touch
        _touch_instance.config(cfg);            // Pas configuratie toe
        getPanel()->setTouch(&_touch_instance); // Koppel touch aan het paneel
    }
};

//bool isVersion3 = true;  // Zet deze variabele op true voor versie 3 (ST7789) of false voor versie 1 (ILI9341)

LGFX tft(isVersion3);  // Maak een instance van LGFX aan en geef de hardwareversie door
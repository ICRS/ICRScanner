#define USER_SETUP_INFO "User_Setup"
#define ST7789_DRIVER      // Full configuration option, define additional parameters below for this display
#define TFT_WIDTH  135
#define TFT_HEIGHT 240 // ST7789 240 x 240
#define TFT_BL   4            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)
#define TFT_MISO  PIN_D6  // Automatically assigned with ESP8266 if not defined
#define TFT_MOSI  19  // Automatically assigned with ESP8266 if not defined
#define TFT_SCLK  18  // Automatically assigned with ESP8266 if not defined

#define TFT_CS    5  // Chip select control pin D8
#define TFT_DC    16  // Data Command control pin
#define TFT_RST   23  // Reset pin (could connect to NodeMCU RST, see next line)

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

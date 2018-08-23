//
// Created by 13567 on 8/22/2018.
//

#ifndef F030_CPP_DEMO_SSD1306_H
#define F030_CPP_DEMO_SSD1306_H
#include <spi.h>
#include <font.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_spi.h>

const std::uint8_t oled_height = 32;
const std::uint8_t oled_width = 128;
class SpiDev_ssd1306_4wire: public Spi
{
public:

    // Enumeration for screen colors
    typedef enum {
        Black = 0x0, // Black color, no pixel
        White = 0x1  // Pixel is set. Color depends on OLED
    }Color_t;
    // Struct to store transformations
    typedef struct {
        std::uint16_t current_x;
        std::uint16_t current_y;
        std::uint8_t inverted;
        std::uint8_t initialized;
    } SSD1306_t;
    SpiDev_ssd1306_4wire(
        SPI_TypeDef *spi,
        Gpio& sck_,
        Gpio& mosi_,
        Gpio& miso_,
        Gpio& dc_,
        Gpio& rst_,
        Spi_config_t& spi_config_)
        :   Spi(spi, sck_, mosi_, miso_, spi_config_),
            dc(dc_), rst(rst_)
    {

    }
    // Procedure definitions
    void init_ssd1306(bool is_cs_enabled);
    void fill(Color_t color);
    void update();
    void draw_pixel(std::uint8_t x, std::uint8_t y, Color_t color);
    char put_char(char ch, FontDef Font, Color_t color);
    char put_string(char* str, FontDef Font, Color_t color);
    void set_cursor(std::uint8_t x, std::uint8_t y);
    // Low-level procedures
    void reset();
    void write_cmd(std::uint8_t byte);
    void write_data(std::uint8_t* buffer, std::size_t buff_size);
    void test_border();
    void test_fps();
private:
    Gpio& dc;
    Gpio& rst;
    std::uint8_t SSD1306_Buffer[oled_width * oled_height / 8];
    SSD1306_t SSD1306;
};
#endif //F030_CPP_DEMO_SSD1306_H

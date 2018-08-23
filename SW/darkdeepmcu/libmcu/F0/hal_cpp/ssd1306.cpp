//
// Created by 13567 on 8/22/2018.
//
#include <ssd1306.h>
#include <cstring>
void SpiDev_ssd1306_4wire::reset() {
	// CS = High (not selected)
    // cs.set();

	// Reset the OLED
	rst.reset();
	HAL_Delay(10);
	rst.set();
	HAL_Delay(10);
}

// Send a byte to the command register
void SpiDev_ssd1306_4wire::write_cmd(std::uint8_t byte)
{
    // cs.reset();
    dc.reset();// cmd
    write(&byte, 1);
    // cs.set();
}
// Send data
void SpiDev_ssd1306_4wire::write_data(std::uint8_t *buffer, std::size_t buff_size)
{
    // cs.reset();
    dc.set();// data
    write(buffer, buff_size);
    // cs.set();
}

// Initialize the oled screen
void SpiDev_ssd1306_4wire::init_ssd1306(bool is_cs_enabled)
{
    // spi init
    init_spibus(is_cs_enabled);//disable cs pin
    // dc rst pin init
    dc.init(
        Gpio::mode_opp,
        Gpio::pull_nopull,
        Gpio::speed_high
    );
    rst.init(
        Gpio::mode_opp,
        Gpio::pull_nopull,
        Gpio::speed_high
    );
    // Reset OLED
    reset();
    // Wait for the screen to boot
    HAL_Delay(100);
    // Init OLED
    write_cmd(0xAE); //display off
    write_cmd(0x20); //Set Memory Addressing Mode
    write_cmd(0x10); // 00,Horizontal Addressing Mode; 01,Vertical Addressing Mode;
    // 10,Page Addressing Mode (RESET); 11,Invalid
    write_cmd(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
#ifdef SSD1306_MIRROR_VERT
    write_cmd(0xC0); // Mirror vertically
#else
    write_cmd(0xC8); //Set COM Output Scan Direction
#endif
    write_cmd(0x00); //---set low column address
    write_cmd(0x10); //---set high column address
    write_cmd(0x40); //--set start line address - CHECK
//    write_cmd(0x20); //--set start line address - CHECK

    write_cmd(0x81); //--set contrast control register - CHECK
    write_cmd(0x8F);

#ifdef SSD1306_MIRROR_HORIZ
    write_cmd(0xA0); // Mirror horizontally
#else
    write_cmd(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
    write_cmd(0xA7); //--set inverse color
#else
    write_cmd(0xA6); //--set normal color
#endif

    write_cmd(0xA8); //--set multiplex ratio(1 to 64) - CHECK
    write_cmd(static_cast<std::uint8_t>(oled_height - 1));


    write_cmd(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    write_cmd(0xD3); //-set display offset - CHECK
    write_cmd(0x00); //-not offset

    write_cmd(0xD5); //--set display clock divide ratio/oscillator frequency
    write_cmd(0xF0); //--set divide ratio

    write_cmd(0xD9); //--set pre-charge period
    write_cmd(0x22); //

    write_cmd(0xDA); //--set com pins hardware configuration - CHECK
//    write_cmd(0x12);//12864
    write_cmd(0x02);//12832

    write_cmd(0xDB); //--set vcomh
    write_cmd(0x20); //0x20,0.77xVcc

    write_cmd(0x8D); //--set DC-DC enable
    write_cmd(0x14); //
    write_cmd(0xAF); //--turn on SSD1306 panel

/*
    // For SH1106
    // see http://we.easyelectronics.ru/lcd_gfx/oled-sh1106.html
    write_cmd(0xAE); //display off

    write_cmd(0xA8); // Set Multiplex Ratio (0x3F) - CHECK
    write_cmd(0x3F);

    write_cmd(0x81); // Set Contrast Control (0x7f) - CHECK
    write_cmd(0x7F);

    write_cmd(0xA1); // Set Segment Re-map - CHECK
    write_cmd(0xC0); // Set Common Output Scan Direction (i.e. reverset)

    write_cmd(0xDA); // Common Pads Hardware Configuration (Alternative Mode Set) - CHECK
    write_cmd(0x12);

    write_cmd(0xD3); // Set Display Offset (0x00) - CHECK
    write_cmd(0x00);

    write_cmd(0x40); // Set Display Start Line 0 - CHECK

    write_cmd(0xAF); //--turn on SSD1306 panel
*/
    // Clear screen
    fill(Black);

    // Flush buffer to screen
    update();

    // Set default values for screen object
    SSD1306.current_x = 0;
    SSD1306.current_y = 0;

    SSD1306.initialized = 1;
}

// Fill the whole screen with the given color
void SpiDev_ssd1306_4wire::fill(Color_t color) {
    /* Set memory */
    uint32_t i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = static_cast<uint8_t>((color == Black) ? 0x00 : 0xFF);
    }
}

// Write the screenbuffer with changed to the screen
void SpiDev_ssd1306_4wire::update()
{
    uint8_t i;
    for(i = 0; i < 8; i++) {
        write_cmd(0xB0 + i);
        write_cmd(0x00);
        write_cmd(0x10);
        write_data(&SSD1306_Buffer[oled_width*i], oled_width);
    }
}

//    Draw one pixel in the screenbuffer
//    X => X Coordinate
//    Y => Y Coordinate
//    color => Pixel color
void SpiDev_ssd1306_4wire::draw_pixel(uint8_t x, uint8_t y, Color_t color) {
    if(x >= oled_width || y >= oled_height) {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if(SSD1306.inverted) {
        color = (Color_t)!color;
    }

    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * oled_width] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * oled_width] &= ~(1 << (y % 8));
    }
}

// Draw 1 char to the screen buffer
// ch         => char om weg te schrijven
// Font     => Font waarmee we gaan schrijven
// color     => Black or White
char SpiDev_ssd1306_4wire::put_char(char ch, FontDef Font, Color_t color) {
    std::uint32_t i, b, j;

    // Check remaining space on current line
    if (oled_width <= (SSD1306.current_x + Font.FontWidth) ||
        oled_height <= (SSD1306.current_y + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Use the font to write
    for(i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for(j = 0; j < Font.FontWidth; j++) {
            if((b << j) & 0x8000)  {
                draw_pixel(
                    static_cast<std::uint8_t>(SSD1306.current_x + j),
                    static_cast<std::uint8_t>(SSD1306.current_y + i),
                    (Color_t) color);
            } else {
                draw_pixel(
                    static_cast<std::uint8_t>(SSD1306.current_x + j),
                    static_cast<std::uint8_t>(SSD1306.current_y + i),
                    (Color_t) !color);
            }
        }
    }

    // The current space is now taken
    SSD1306.current_x += Font.FontWidth;

    // Return written char for validation
    return ch;
}

// Write full string to screenbuffer
char SpiDev_ssd1306_4wire::put_string(char* str, FontDef Font, Color_t color) {
    // Write until null-byte
    while (*str) {
        if (put_char(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

// Position the cursor
void SpiDev_ssd1306_4wire::set_cursor(std::uint8_t x, std::uint8_t y) {
    SSD1306.current_x = x;
    SSD1306.current_y = y;
}
void SpiDev_ssd1306_4wire::test_border()
{
    fill(Black);
    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t x_ = 8;
    uint8_t y_ = 8;
    do {
        for(uint8_t i = 0; i < x_+1; ++i)
            for(uint8_t j = 0; j < y_+1; ++j)
                draw_pixel(x+i, y+j, Black);
        if((y == y_) && (x < oled_width-x_))
            x++;
        else if((x == oled_width-x_) && (y < (uint8_t)(oled_height - 1)-y_))
            y++;
        else if((y == (uint8_t)(oled_height - 1)-y_) && (x > x_))
            x--;
        else
            y--;
        for(uint8_t i = 0; i < x_+1; ++i)
            for(uint8_t j = 0; j < y_+1; ++j)
                draw_pixel(x+i, y+j, White);
        update();
        HAL_Delay(5);
        end = HAL_GetTick();
    } while((end - start) < 8000);

    HAL_Delay(1000);
}
void SpiDev_ssd1306_4wire::test_fps() {
    fill(Black);

    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    int fps = 0;
    char message[] = "ABCDEFGHIJK";

    set_cursor(2, 1);
    put_string("Testing...", Font_7x10, White);

    do {
        set_cursor(2, 13);
        put_string(message, Font_7x10, White);
        update();

        char ch = message[0];
        std::memmove(message, message+1, sizeof(message)-2);
        message[sizeof(message)-2] = ch;

        fps++;
        end = HAL_GetTick();
    } while((end - start) < 5000);

    HAL_Delay(500);

    char buff[64];
    fps = (float)fps / ((end - start) / 1000.0);
    snprintf(buff, sizeof(buff), "~ %d FPS", fps);

    fill(Black);
    set_cursor(2, 13);
    put_string(buff, Font_7x10, White);
    update();
    HAL_Delay(500);
}
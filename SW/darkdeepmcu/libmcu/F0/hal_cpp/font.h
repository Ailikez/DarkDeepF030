//
// Created by 13567 on 8/22/2018.
//

#ifndef F030_CPP_DEMO_FONT_H
#define F030_CPP_DEMO_FONT_H
#include <cstdint>
typedef struct {
    const std::uint8_t FontWidth;    /*!< Font width in pixels */
    std::uint8_t FontHeight;   /*!< Font height in pixels */
    const std::uint16_t *data; /*!< Pointer to data font data array */
} FontDef;


extern FontDef Font_7x10;
#endif //F030_CPP_DEMO_FONT_H

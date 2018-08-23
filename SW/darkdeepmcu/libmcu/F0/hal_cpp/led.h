//
// Created by 13567 on 8/18/2018.
//

#ifndef F030_CPP_DEMO_LED_H
#define F030_CPP_DEMO_LED_H
#ifdef __cplusplus
extern "C" {
#endif
// #include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal.h"
#include <gpio.h>
#include <cstdint>
class Led_Gpio: public Gpio
{
public:
    Led_Gpio(port_type port_, pin_id_type pin_)
        :Gpio(port_, pin_)
    {}
};
#ifdef __cplusplus
}
#endif
#endif //F030_CPP_DEMO_LED_H

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
#include <cstdint>
class Led_Base
{
public:
    virtual ~Led_Base()
    {}// Virtual dtor.
    virtual void toggle() = 0;// Pure abstract.
    // Interface for querying the LED state
    bool get_state()
    { return state; }
protected:
    bool state;
    // A protected default ctor.
    Led_Base()
        : state(false)
    {}
private:
    // Private non-implemented copy ctor.
    Led_Base(const Led_Base &) = delete;
    // Private non-implemented copy assignment operator.
    const Led_Base &operator=(const Led_Base &) = delete;
};

class Led_Gpio: public Led_Base
{
public:
    typedef GPIO_TypeDef *port_type;
    typedef std::uint16_t pin_type;
    Led_Gpio(const port_type port_, const pin_type pin_)
        : port(port_), pin(pin_)
    {
        // Do Initialization for the Led port.
        GPIO_InitTypeDef GPIO_InitStruct;
        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        /*Configure GPIO pins : PBPin PBPin */
        GPIO_InitStruct.Pin = pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(port, &GPIO_InitStruct);
    }
    virtual ~Led_Gpio()
    {}
    virtual void toggle()
    {
        // Toggle the Led.
        // reinterpret_cast<port_type>(port)->ODR ^= pin;
        port->ODR ^= pin;
        // Toggle the indication flag too.
        state = !state;
    }
private:
    const port_type port;
    const pin_type pin;
};
#ifdef __cplusplus
}
#endif
#endif //F030_CPP_DEMO_LED_H

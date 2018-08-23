//
// Created by 13567 on 8/18/2018.
//

#ifndef F030_CPP_DEMO_GPIO_H
#define F030_CPP_DEMO_GPIO_H
#include <stm32f0xx_hal.h>
#include <cstdint>
/*
 * 2 methods to initialize an Gpio object:
 *      Gpio pa3(GPIOF, 3);
 *      Gpio pa4('B', 4);
 * */
class Gpio
{
public:
    typedef enum
    {
        mode_input = (0x00000000U),   /*!< Input Floating Mode                   */
        mode_opp = (0x00000001U),   /*!< Output Push Pull Mode                 */
        mode_ood = (0x00000011U),   /*!< Output Open Drain Mode                */
        mode_af_pp = (0x00000002U),   /*!< Alternate Function Push Pull Mode     */
        mode_af_od = (0x00000012U),   /*!< Alternate Function Open Drain Mode    */
        mode_analog = (0x00000003U),   /*!< Analog Mode  */
        mode_it_rising = (0x10110000U),   /*!< External Interrupt Mode with Rising edge trigger detection          */
        mode_it_falling = (0x10210000U),   /*!< External Interrupt Mode with Falling edge trigger detection         */
        mode_it_rising_falling =
        (0x10310000U),   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */
        mode_evt_rising = (0x10120000U),   /*!< External Event Mode with Rising edge trigger detection               */
        mode_evt_falling = (0x10220000U),   /*!< External Event Mode with Falling edge trigger detection              */
        mode_evt_rising_falling =
        (0x10320000U),   /*!< External Event Mode with Rising/Falling edge trigger detection       */
        /**
          * @}
          */
    } Gpio_mode_t;
    typedef enum
    {
        speed_low = (0x00000000U),  /*!< range up to 2 MHz, please refer to the product datasheet */
        speed_mid = (0x00000001U),  /*!< range  4 MHz to 10 MHz, please refer to the product datasheet */
        speed_high = (0x00000003U),  /*!< range 10 MHz to 50 MHz, please refer to the product datasheet */

    } Gpio_speed_t;
    typedef enum
    {
        pull_nopull = (0x00000000U),   /*!< No Pull-up or Pull-down activation  */
        pull_pullup = (0x00000001U),   /*!< Pull-up activation                  */
        pull_pulldown = (0x00000002U),   /*!< Pull-down activation                */
    } Gpio_pull_t;
    typedef enum
    {
        level_low = GPIO_PIN_RESET,
        level_high = GPIO_PIN_SET,
        level_none = 2,
    } Gpio_outputlevel_t;
    typedef enum
    {
        // more details in stm32f0xx_hal_gpio_ex.h
        af0 = 0,
        af1,
        af2,
        af3,
        af4,
        af5,
        af6 = 6,
    } Gpio_af_t;
    typedef char port_name_type;
    typedef GPIO_TypeDef *port_type;
    typedef std::uint16_t pin_type;
    typedef std::uint8_t pin_id_type;

    Gpio(port_name_type port_, pin_id_type pin_)
        : port(
        reinterpret_cast<GPIO_TypeDef *>(
            (port_ >= 'A' && port_ <= 'F' ? port_ - 'A' : 0x0U) * 0x400U + AHB2PERIPH_BASE
        )
    ),
          pin(static_cast<pin_type>(0x1U) << pin_),
          is_initialized(false), pin_id(pin_), port_name(port_)
    {

    }
    Gpio(port_type port_, pin_id_type pin_)
        : port(port_),
          pin(static_cast<pin_type>(0x1U) << pin_),
          is_initialized(false), pin_id(pin_),
          port_name(static_cast<char>(reinterpret_cast<std::uint32_t>(port_)/0x400U)+'A')
    {

    }
    Gpio()
        : is_initialized(false), pin_id(0)
    {}
    ~Gpio()
    {
        HAL_GPIO_DeInit(port, pin);
    };
    bool init(Gpio_mode_t mode,
              Gpio_pull_t pull,
              Gpio_speed_t speed = speed_low,
              Gpio_outputlevel_t level = level_none,
              Gpio_af_t af = af0)
    {
        // // get pin id
        // while ((pin >> pin_id) != 0) {pin_id++;}
        if (port == GPIOA)
            __HAL_RCC_GPIOA_CLK_ENABLE();
        else if (port == GPIOB)
            __HAL_RCC_GPIOB_CLK_ENABLE();
        else if (port == GPIOC)
            __HAL_RCC_GPIOC_CLK_ENABLE();
        else if (port == GPIOF)
            __HAL_RCC_GPIOF_CLK_ENABLE();
        else
            return false; // error
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = pin;
        GPIO_InitStruct.Mode = mode;
        switch (mode) {
            case mode_af_od:// no pulldown mode, no level config
            case mode_af_pp:// no level config
                GPIO_InitStruct.Alternate = af;
            case mode_opp:
            case mode_ood:// no pulldown mode
            {
                // output level config
                if (level == level_high)
                    port->BRR = pin;
                else if (level == level_low)
                    port->BSRR = pin;
                else;// no level config
                GPIO_InitStruct.Pull = pull;
                GPIO_InitStruct.Speed = speed;
                break;
            }
            case mode_analog:// no pull
            case mode_input:
            case mode_it_falling:
            case mode_it_rising_falling:
            case mode_it_rising: {
                GPIO_InitStruct.Pull = pull;
                break;
            }
            default:break;
        }
        HAL_GPIO_Init(port, &GPIO_InitStruct);
        is_initialized = true;
        return true; // ok
    }
    void set()
    {
        port->BSRR = pin;
    }
    void reset()
    {
        port->BRR = pin;
    }
    void write(std::uint8_t val)
    {
        if (val)
            port->BSRR = pin;
        else
            port->BRR = pin;
    }
    void read(std::uint8_t *val)
    {
        *val = static_cast<std::uint8_t>(port->IDR & pin);
    }
    std::uint8_t read()
    {
        return static_cast<std::uint8_t>(port->IDR & pin);
    }
    void toggle()
    {
        port->ODR ^= pin;
    }
    std::uint8_t operator=(Gpio &gpio)
    {
        return read();
    }
    operator int()
    {
        return read();
    }
    operator std::uint8_t()
    {
        return read();
    }
    Gpio &operator=(std::uint8_t val)
    {
        write(val);
        return *this;
    }
private:
    port_type port;
    pin_type pin;

    port_name_type port_name;
    std::uint8_t pin_id;

    bool is_initialized;

};


#endif //F030_CPP_DEMO_GPIO_H

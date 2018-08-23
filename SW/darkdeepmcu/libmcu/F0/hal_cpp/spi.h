//
// Created by 13567 on 8/22/2018.
//

#ifndef F030_CPP_DEMO_SPI_H
#define F030_CPP_DEMO_SPI_H
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_spi.h>
#include <gpio.h>

class Spi
{
public:
    typedef enum
    {
        mode_0 = 0,
        mode_1,
        mode_2,
        mode_3,
    }Spi_mode_id_t;
    typedef enum
    {
        prescaler_2 = 0,
        prescaler_4,
        prescaler_8,
        prescaler_16,
        prescaler_32,
        prescaler_64,
        prescaler_128,
        prescaler_256,
    }Spi_prescaler_id_t;
    typedef enum
    {
        msb = 0,
        lsb,
    }Spi_bit_order_t;
    typedef enum
    {
        data_size_4bit = 0,
        data_size_5bit,
        data_size_6bit,
        data_size_7bit,
        data_size_8bit,
        data_size_9bit,
        data_size_10bit,
        data_size_11bit,
        data_size_12bit,
        data_size_13bit,
        data_size_14bit,
        data_size_15bit,
        data_size_16bit,

    }Spi_data_size_t;
    typedef struct
    {
        Spi_mode_id_t mode_id;
        Spi_prescaler_id_t baud_rate_prescaler_id;
        Spi_bit_order_t bit_order_id;
        Spi_data_size_t data_size_id;
        std::uint8_t device_id;
        Gpio& cs;
    }Spi_config_t;
    Spi(
        SPI_TypeDef *spi,
        Gpio& sck_,
        Gpio& mosi_,
        Gpio& miso_,
        Spi_config_t& spi_config_
        ) :
        sck(sck_),
        mosi(mosi_),
        miso(miso_),
        spi_config(spi_config_),
        cs(spi_config_.cs),
        is_occupied(true)
    {
        spi_handle.Instance = spi;
    }
    
    void init_spibus(bool is_cs_enabled)
    {
        // user configs
        switch (spi_config.data_size_id)
        {
            case data_size_4bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_4BIT;
                break;
            case data_size_5bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_5BIT;
                break;
            case data_size_6bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_6BIT;
                break;
            case data_size_7bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_7BIT;
                break;
            case data_size_8bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
                break;
            case data_size_9bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_9BIT;
                break;
            case data_size_10bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_10BIT;
                break;
            case data_size_11bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_11BIT;
                break;
            case data_size_12bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_12BIT;
                break;
            case data_size_13bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_13BIT;
                break;
            case data_size_14bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_14BIT;
                break;
            case data_size_15bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_15BIT;
                break;
            case data_size_16bit:
                spi_handle.Init.DataSize = SPI_DATASIZE_16BIT;
                break;
        }
        switch (spi_config.mode_id)
        {
            case mode_0:
                spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
                spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
                break;
            case mode_1:
                spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
                spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
                break;
            case mode_2:
                spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
                spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
                break;
            case mode_3:
                spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
                spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
                break;
        }
        switch(spi_config.baud_rate_prescaler_id)
        {
            case prescaler_2:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
                break;
            case prescaler_4:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
                break;
            case prescaler_8:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
                break;
            case prescaler_16:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
                break;
            case prescaler_32:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
                break;
            case prescaler_64:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
                break;
            case prescaler_128:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
                break;
            case prescaler_256:
                spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
                break;
        }
        switch (spi_config.bit_order_id)
        {
            case lsb:
                spi_handle.Init.FirstBit = SPI_FIRSTBIT_LSB;
                break;
            case msb:
                spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
                break;
        }
        // default configs
        spi_handle.Init.Mode = SPI_MODE_MASTER;
        spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
        spi_handle.Init.NSS = SPI_NSS_SOFT;
        spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
        spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        spi_handle.Init.CRCPolynomial = 7;
        spi_handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        spi_handle.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
        if(spi_handle.Instance == SPI1)
            __HAL_RCC_SPI1_CLK_ENABLE();
#ifdef SPI2
        else if(spi == SPI2)
            __HAL_RCC_SPI2_CLK_ENABLE();
#endif
#ifdef SPI3
        else if(spi == SPI3)
            __HAL_RCC_SPI3_CLK_ENABLE();
#endif
        sck.init(
            Gpio::mode_af_pp,
            Gpio::pull_nopull,
            Gpio::speed_high);
        mosi.init(
            Gpio::mode_af_pp,
            Gpio::pull_nopull,
            Gpio::speed_high);
        miso.init(
            Gpio::mode_af_pp,
            Gpio::pull_nopull,
            Gpio::speed_high);
        if(is_cs_enabled)
            spi_config.cs.init(
                Gpio::mode_opp,
                Gpio::pull_pullup,
                Gpio::speed_high
                );
        
        while(HAL_SPI_Init(&spi_handle) != HAL_OK);// wait until init success
        is_occupied = false;
    }
    void write(std::uint8_t val)
    {
        HAL_SPI_Transmit(&spi_handle, &val, 0x1U, HAL_MAX_DELAY);
    }
    void write(std::uint8_t* val, std::size_t size)
    {
        HAL_SPI_Transmit(&spi_handle, val, size, HAL_MAX_DELAY);
    }

protected:
    Gpio& sck;
    Gpio& mosi;
    Gpio& miso;
    SPI_HandleTypeDef spi_handle;
    Spi_config_t spi_config;
    Gpio& cs;
    bool is_occupied;
};


#endif //F030_CPP_DEMO_SPI_H

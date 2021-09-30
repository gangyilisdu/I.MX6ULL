#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "imx6ul.h"

typedef enum gpio_interrupt_mode
{
    kGPIO_Nointmode=0U,
    kGPIO_IntLowLevel=1U,
    kGPIO_IntHighLevel=2U,
    kGPIO_IntRisingEdge=3U,
    kGPIO_IntFallingEdge=4U,
    kGPIO_IntRisingOrFallingEdge=5U,
}gpio_interrupt_mode_t;


typedef enum _gpio_pin_direction 
{
    kGPIO_DigitialInput=0U,
    kGPIO_DigitialOutput=1U,
}gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    uint8_t outputLogic; 
    gpio_interrupt_mode_t interruptMode;
}gpio_pin_config_t; 

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);
void gpio_disenableint(GPIO_Type *base, unsigned int pin);
void gpio_clearintflags(GPIO_Type *base, unsigned int pin);
void gpio_enableint(GPIO_Type *base, unsigned int pin);
void gpio_intconfig(GPIO_Type *base, unsigned int pin, gpio_interrupt_mode_t pin_int_mode);

#endif









#include "bsp_exit.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_beep.h"

void exit_init(void)
{
    gpio_pin_config_t key_config;

    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

    key_config.direction = kGPIO_DigitialInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    key_config.outputLogic = 1;
    gpio_init(GPIO1, 18, &key_config);
    
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);

    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio_io18_handler,NULL);

    gpio_enableint(GPIO1,18);
}

void gpio_io18_handler(unsigned int gicciar, void *param)
{
    static unsigned char state = 0;

    if(gpio_pinread(GPIO1,18) == 0)
        {
            state = !state;
            beep_switch(state);
        }
    gpio_clearintflags(GPIO1,18);
}









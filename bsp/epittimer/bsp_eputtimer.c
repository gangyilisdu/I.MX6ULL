#include "bsp_epittimer.h"
#include "bsp_int.h"
#include "bsp_led.h"

void epittimer_init(unsigned int frac, unsigned int value)
{
    if(frac > 0xfff)
        frac = 0xfff;
    EPIT1->CR = 0;
    EPIT1->CR = (1<<24 | frac<<4 | 1<<3 | 1<<2 | 1<<1);
    EPIT1->LR = value;
    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL);	

    EPIT1->CR |= 1<<0;
}

void epit1_irqhandler(void)
{
    static unsigned char state = 0;
    state = !state ;
    if(EPIT1->SR & (1<<0))
    {
        led_switch(LED0, state);
    }
    EPIT1->SR |= 1<<0;
}







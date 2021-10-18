#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "imx6ul.h"

void delay_gpt_init(void);
void delay_us(unsigned int usdelay);
void delay_ms(unsigned int msdelay);
void delay_short(volatile unsigned int n);
void gpt1_irqhandler(unsigned int gicciar, void *Param ); 

#endif
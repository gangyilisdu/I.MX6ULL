#include "bsp_delay.h"
#include "bsp_led.h"


void delay_gpt_init(void)
{
	GPT1->CR = 0;
	GPT1->CR = 1<<15;
	while((GPT1->CR >> 15) & 0x01);
	GPT1->CR |= (1<<1) | (1<<6);

	GPT1->OCR[0] = 0xffffffff;


/*gpt_delay application*/
#if 0
	GPT1->PR = 65;

	GPT1->OCR[0] = 1000000/2; /*500ms*/

	GPT1->IR = 1<<0;

	GIC_EnableIRQ(GPT1_IRQn);

	system_register_irqhandler(GPT1_IRQn, (system_irq_handler_t)gpt1_irqhandler, NULL);

#endif

	GPT1->CR = 1<<0; 	
}

#if 0

void gpt1_irqhandler(unsigned int gicciar, void *Param ) 
{
	static unsigned char state = OFF; 
	state= !state;
	if(GPT1->SR & (1<<0))
	{
		led_switch(LED0, state);
	}

	GPT1-> SR |= (1<<0);
}

#endif


void delay_us(unsigned int usdelay)
{
	unsigned long oldcnt, newcnt;
	unsigned long tcntvalue = 0;

	oldcnt = GPT1->CNT;
	while(1)
	{
		newcnt = GPT1->CNT;
		if(newcnt != oldcnt)
		{	
			if(newcnt > oldcnt)
				tcntvalue += newcnt - oldcnt;
			else
				tcntvalue += 0xffffffff - oldcnt + newcnt;
			oldcnt = newcnt;
			if(tcntvalue >= usdelay)
			break;
		}
	}
}

void delay_ms(unsigned int msdelay)
{
	int i=0;
	for(i=0;i<msdelay;i++)
	{
		delay_us(1000);
	}
}

void delay_short(volatile unsigned int n)
{
	while(n--);
}






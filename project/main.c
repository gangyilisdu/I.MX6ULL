#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"
#include "bsp_i2c.h"
#include "bsp_ap3216c.h"
#include "stdio.h"

int main(void)
{
	unsigned short ir, als, ps;
	unsigned char state = OFF;
	int_Init();
	imx6u_clkinit();
	clk_enable();		
	led_init();		
	delay_gpt_init();	
	uart_init();
	beep_init();
	key_init();
	filterkey_init();
	ap3216c_init();

	while(1)			
	{	
		ap3216c_readdata(&ir, &ps, &als); 
		printf("ir=%d, ps=%d, als=%d\r\n", ir, ps, als);
		
		state = !state;
		led_switch(LED0,state);
		delay_ms(500);
	}

	return 0;
}

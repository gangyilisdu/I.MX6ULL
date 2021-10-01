#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"
#include "stdio.h"

int main(void)
{
	int a,b;
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

	while(1)			
	{	
		printf("insert two integers with space: ");
		scanf("%d %d",&a, &b);
		printf("\r\n data %d + %d = %d \r\n", a,b,a+b);
		
		state = !state;
		led_switch(LED0,state);
		delay_ms(500);
	}

	return 0;
}

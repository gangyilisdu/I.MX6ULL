#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "bsp_i2c.h"
#include "bsp_ap3216c.h"
#include "bsp_spi.h"
#include "bsp_icm20608.h"
#include "stdio.h"

unsigned int backcolor[10] = {LCD_BLUE, LCD_GREEN, LCD_RED, LCD_CYAN, LCD_YELLOW, 
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE};

int main(void)
{
	unsigned char index = 0;
	unsigned char state = OFF;
	int_Init();
	imx6u_clkinit();
	clk_enable();		
	led_init();		
	delay_gpt_init();	
	uart_init();
	icm20608_init();
	beep_init();
	key_init();
	filterkey_init();
	ap3216c_init();

	tftlcd_dev.forecolor = LCD_RED; 
	while(1)			
	{	
		lcd_clear(backcolor[index]); 	  
		lcd_show_string(10, 40, 260, 32, 32,(char*)"ALPHA IMX6U"); 	
		lcd_show_string(10, 80, 240, 24, 24,(char*)"RGBLCD TEST");
		lcd_show_string(10, 110, 240, 16, 16,(char*)"ATOM@ALIENTEK");      					 
		lcd_show_string(10, 130, 240, 12, 12,(char*)"2019/8/14");	      					 
	    index++;
		if(index == 10){
			index = 0;
		}
		icm20608_getdata();
		
		state = !state;
		led_switch(LED0,state);
		delay_ms(500);
	}

	return 0;
}


#include "bsp_spi.h"
#include "bsp_gpio.h"
#include "stdio.h"


void spi_init(ECSPI_Type *base)
{

	base->CONREG = 0; 
	base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20); 


	base->CONFIGREG = 0; 		
	

	base->PERIODREG = 0X2000;	


	base->CONREG &= ~((0XF << 12) | (0XF << 8));	
	base->CONREG |= (0X9 << 12);					
}


unsigned char spich0_readwrite_byte(ECSPI_Type *base, unsigned char txdata)
{ 
	uint32_t  spirxdata = 0;
	uint32_t  spitxdata = txdata;


	base->CONREG &= ~(3 << 18);
	base->CONREG |= (0 << 18);

  	while((base->STATREG & (1 << 0)) == 0){} 
		base->TXDATA = spitxdata;
	
	while((base->STATREG & (1 << 3)) == 0){} 
		spirxdata = base->RXDATA;
	return spirxdata;
}


#ifndef __BSP_EXIT_H
#define __BSP_EXIT_H

#include "imx6ul.h"

void exit_init(void);
void gpio_io18_handler(unsigned int gicciar, void *param);

#endif
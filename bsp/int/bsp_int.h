#ifndef __BSP_INT_H
#define __BSP_INT_H

#include "imx6ul.h"

typedef void (*system_irq_handler_t)(unsigned int gicciar, void *param);

typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler;
    void *userParam;
}sys_irq_handle_t;

void int_Init(void);
void system_irqtable_init(void);
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);
void system_irqhandler(unsigned int gicciar);
void default_irqhandler(unsigned int gicciar, void *param);

#endif
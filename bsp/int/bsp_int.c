#include "bsp_int.h"

static unsigned int irqNesting;

static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

void int_Init(void)
{
    GIC_Init();
    system_irqtable_init();
    __set_VBAR(0x87800000);
}

void system_irqtable_init(void)
{
    unsigned int i=0;
    irqNesting = 0;
    for(i=0;i<NUMBER_OF_INT_VECTORS;i++)
    {
        irqTable[i].irqHandler = default_irqhandler;
        irqTable[i].userParam = NULL;
    }
}

void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

void system_irqhandler(unsigned int gicciar)
{
    uint32_t intNUM = gicciar & 0x3ff;
    if(intNUM >= NUMBER_OF_INT_VECTORS)
    {
        return;
    }

    irqNesting++;
    irqTable[intNUM].irqHandler(intNUM,irqTable[intNUM].userParam);
    irqNesting--;
}

void default_irqhandler(unsigned int gicciar, void *param)
{
    while(1);
}
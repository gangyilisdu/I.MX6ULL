#ifndef __BSP_FT5426_H
#define __BSP_FT5426_H

#include "imx6ul.h"
#include "bsp_gpio.h"

/*FT5426 device address*/
#define FT5426_ADDR                      0X38

#define FT5426_DEVICE_MODE              0X00
#define FT5426_IDGLIB_VERSION           0XA1
#define FT5426_IDG_MODE                 0XA4
#define FT5426_TD_STATUS                0X02
#define FT5426_TOUCH1_XH                0X03

#define FT5426_XYCOORDREG_NUM           30
#define FT5426_INIT_FINISHED            1
#define FT5426_INIT_NOTFINISHED         0

#define FT5426_TOUCH_EVENT_DOWN         0X00
#define FT5426_TOUCH_EVENT_UP           0X01
#define FT5426_TOUCH_EVENT_ON           0X02
#define FT5426_TOUCH_EVENT_RESERVED     0X03

struct ft5426_dev_struc
{
    unsigned char initflag;
    unsigned char intflag;
    unsigned char point_num; 
    unsigned short x[5];
    unsigned short y[5];
};

extern struct ft5426_dev_struc ft5426_dev;

void ft5426_init(void);
void gpio1_io9_irqhandler(void);
unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data);
unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg);
void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf);
void ft5426_read_tpcoord(void);

#endif
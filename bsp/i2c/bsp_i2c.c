#include "bsp_i2c.h"
#include "stdio.h"

void i2c_init(I2C_Type *base)
{
    base->I2CR &= ~(1<<7);
    base->IFDR = 0x15;    
    base->I2CR |= (1<<7);
}

unsigned char i2c_master_repeated_start(I2C_Type *base, 
                                unsigned char address,
                                enum i2c_direction direction)
{
    /*I2C busy and jump*/
    if(base->I2SR & (1<<5) && ((base->I2CR) & (1<<5)) == 0)
        return 1;
    /*set I2CR bit4:sent bit2: restart*/
    base->I2CR |= (1<<4) | (1<<2);
    /*address*/
    base->I2DR = ((unsigned int)address << 1) | 
                    ((direction == kI2C_Read) ? 1 : 0);
    return 0; 
}

unsigned char i2c_master_start(I2C_Type *base, 
                                unsigned char address,
                                enum i2c_direction direction)
{
    if(base->I2SR & (1<<5))
        return 1;
    /*set i2cr bit4: sent bit5: master mode */
    base->I2CR |= (1<<5) | (1<<4);

    base->I2DR = (((unsigned int)address)<<1) | 
                        ((direction == kI2C_Read) ? 1 : 0);
    return 0;
}

unsigned char i2c_check_and_clear_error(I2C_Type *base, 
                                        unsigned int status)
{
    if(status & (1<<4))
    {
        base->I2SR &= ~(1<<4);
        base->I2CR &= ~(1<<7);
        base->I2CR |= (1<<7);
        return I2C_STATUS_ARBITRATIONLOST;
    }
    else if(status & (1<<0))
    {
        return I2C_STATUS_NAK;
    }
    return I2C_STATUS_OK;
}

unsigned char i2c_master_stop(I2C_Type *base)
{
    unsigned short timeout = 0xffff;

    base->I2CR &= ~((1<<5) | (1<<4) | (1<<3));
    while(base->I2SR & (1<<5))
    {
        timeout--;
        if(timeout == 0)
            return I2C_STATUS_TIMEOUT;
    }
    return I2C_STATUS_OK;
}

void i2c_master_write(I2C_Type *base, 
                    unsigned char *buff, unsigned int size) 
{
    while(!(base->I2SR & (1<<7)))
    base->I2SR &= ~(1<<1);
    base->I2CR |= (1<<4);
    while(size--)
    {
        base->I2DR = *buff++;
        while(!(base->I2SR & (1<<1)));
        base->I2SR &= ~(1<<1);
        
        if(i2c_check_and_clear_error(base, base->I2SR))
            break;
    }
    base-> I2SR &= ~(1 << 1);
    i2c_master_stop(base);
}

void i2c_master_read(I2C_Type *base, 
                    unsigned char *buff, unsigned int size)
{
    volatile uint8_t dummy = 0;

    dummy++;
    while(!(base->I2SR & (1<<7)));
    base->I2CR &= (1<<4) | (1<<3);
    if(size == 1)
        base->I2CR |= (1<<3);
    dummy = base->I2DR;
    while(size--)
    {
        while(!(base->I2SR & (1<<1)));
        base->I2SR &= ~(1<<1);

        if(size == 0)
            i2c_master_stop(base);
        if(size == 1)
            base->I2CR |= (1<<3);
        *buff = base->I2DR;
    }
}

unsigned char i2c_master_transfer(I2C_Type *base, 
                                struct i2c_transfer *xfer)
{
    unsigned char ret = 0;
    enum i2c_direction direction = xfer->direction;

    base->I2SR &= ~((1<<1) | (1<<4));
    while(!((base->I2SR >> 7) & 0x1));
    if((xfer->subaddressSize > 0) && (xfer->direction == kI2C_Read)) 
        direction = kI2C_Write;
    ret = i2c_master_start(base, xfer->slaveaddress, direction);
    if(ret) 
        return ret; 
    while(!(base->I2SR & (1<<1)));
    ret = i2c_check_and_clear_error(base, base->I2SR);
    if(ret)
    {
        i2c_master_stop(base);
        return ret;
    }   

    if(xfer->subaddressSize)
    {
        do
        {
            base->I2SR &= ~(1<<1);
            xfer->subaddressSize--;
            base->I2DR = ((xfer->subaddress) >> (8*xfer->subaddressSize));
            while(!(base->I2SR &(1<<1)));
        
            ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
                i2c_master_stop(base);
                return ret;
            }
        } while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));
        
        if(xfer->direction == kI2C_Read)
        {
            base->I2SR &= (1<<1);
            i2c_master_repeated_start(base, xfer->slaveaddress, kI2C_Read);
            while(!(base->I2SR & (1<<1)));

            ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
                ret = I2C_STATUS_ADDRNAK;
                i2c_master_stop(base);
                return ret;
            }
        }     
    }

    if((xfer->direction = kI2C_Write) && (xfer-> dataSize >0))
        i2c_master_write(base, xfer->data, xfer->dataSize);
    if((xfer->direction = kI2C_Read) && (xfer->dataSize>0))
        i2c_master_read(base, xfer->data, xfer->dataSize);
    return 0;
}









// i2c.c
//
// 16-07-2019
//


#include "i2c.h"


//-----------------------------------------------------------------------------
void i2c_init(void)
    {
    SDA_PULL;
    SCL_PULL;
    }


//-----------------------------------------------------------------------------
void i2c_start(void)
    {
    SDA_PULL;
    I2C_DELAY;
    SCL_PULL;
    I2C_DELAY;

    SDA_NULL;
    I2C_DELAY;
    SCL_NULL;
    I2C_DELAY;
    }


//-----------------------------------------------------------------------------
void i2c_stop(void)
    {
    SCL_NULL;
    I2C_DELAY;
    SDA_NULL;
    I2C_DELAY;

    SCL_PULL;
    I2C_DELAY;
    SDA_PULL;
    I2C_DELAY;
    }


//-----------------------------------------------------------------------------
void i2c_send_byte(uint8_t data)
    {
    for(uint8_t mask=0x80; mask; mask>>=1)
        {
        if(data & mask) { SDA_PULL; }
        else { SDA_NULL; }

        I2C_DELAY;
        SCL_PULL;
        I2C_DELAY;
        SCL_NULL;
        }

    SDA_PULL;
    I2C_DELAY;
    SCL_PULL;
    I2C_DELAY;
    SCL_NULL;
    }


//-----------------------------------------------------------------------------
uint8_t i2c_read_byte(uint8_t ack)
    {
    uint8_t data=0;

    SDA_PULL;

    for(uint8_t i=0; i<8; i++)
        {
        data<<=1;
        SCL_PULL;
        I2C_DELAY;
        if(SDA_IS_PULL) data |= 0x01;
        SCL_NULL;
        I2C_DELAY;
        }

    if(ack==I2C_ACK) { SDA_NULL; }
    else { SDA_PULL; }

    I2C_DELAY;
    SCL_PULL;
    I2C_DELAY;
    SCL_NULL;
    I2C_DELAY;
    SDA_PULL;

    return data;
    }




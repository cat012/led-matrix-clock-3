// i2c.h
//
// Software I2C
//
// Compiler: XC8 V1.33
//
// 16-07-2019
//


#ifndef I2C_H_
#define I2C_H_


#include <stdint.h>

#include "main.h"


#define I2C_DELAY  __delay_us(5)


///////////////////////////////////////////////////////////////////////////////

#define I2C_DT_PIN   PORTCbits.RC7
#define I2C_DT_TRIS  TRISCbits.RC7

#define I2C_CK_PIN   PORTDbits.RD4
#define I2C_CK_TRIS  TRISDbits.RD4

///////////////////////////////////////////////////////////////////////////////


#define I2C_DT_CLR  I2C_DT_PIN=0
#define I2C_DT_SET  I2C_DT_PIN=1
#define I2C_DT_OUT  I2C_DT_TRIS=0
#define I2C_DT_INP  I2C_DT_TRIS=1

#define I2C_CK_CLR  I2C_CK_PIN=0
#define I2C_CK_SET  I2C_CK_PIN=1
#define I2C_CK_OUT  I2C_CK_TRIS=0
#define I2C_CK_INP  I2C_CK_TRIS=1


#define SDA_PULL  I2C_DT_INP
#define SDA_NULL  I2C_DT_OUT; I2C_DT_CLR

#define SDA_IS_PULL  I2C_DT_PIN==1

#define SCL_PULL  I2C_CK_INP
#define SCL_NULL  I2C_CK_OUT; I2C_CK_CLR


#define I2C_ACK   0
#define I2C_NACK  1



void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_send_byte(uint8_t data);
uint8_t i2c_read_byte(uint8_t ack);



#endif  //end of I2C_H_




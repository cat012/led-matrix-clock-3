// bitm.h
//
// 23-05-2019
//


#ifndef BITM_H_
#define BITM_H_


// bit manipulation

#define REG_BIT_SET(reg, bit) ((reg) |= (1<<(bit)))
#define REG_BIT_CLR(reg, bit) ((reg) &= ~(1<<(bit)))

#define REG_BIT_IS_SET(reg, bit) ((reg)>>(bit)&1)
#define REG_BIT_IS_CLR(reg, bit) (!((reg)>>(bit)&1))


#endif




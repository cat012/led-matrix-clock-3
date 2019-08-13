
// 4x 8x8 LED matrix + IC MAX7219
//
// file: max7219.h
// codepage: win-1251
// compiler: XC8 1.33
//
//-----------------
//   Y
// X 0-31
//   |
//   7
//-----------------
//
// 06-08-2019



#ifndef MAX7219_H_
#define MAX7219_H_



#include <stdint.h>
#include <xc.h>

#include "bitm.h"

//6-data
//5-cs
//4-clock
/////////////////////////////////////////////////


#define DT_PIN   PORTCbits.RC6
#define DT_TRIS  TRISCbits.RC6

#define CS_PIN   PORTCbits.RC5  
#define CS_TRIS  TRISCbits.RC5

#define CK_PIN   PORTCbits.RC4
#define CK_TRIS  TRISCbits.RC4

#define DT_CLR  DT_PIN=0
#define DT_SET  DT_PIN=1
#define DT_OUT  DT_TRIS=0

#define CS_CLR  CS_PIN=0
#define CS_SET  CS_PIN=1
#define CS_OUT  CS_TRIS=0

#define CK_CLR  CK_PIN=0
#define CK_SET  CK_PIN=1
#define CK_OUT  CK_TRIS=0

/////////////////////////////////////////////////


#define MAX7219_SERIAL_INIT  CS_CLR; CS_OUT; CK_CLR; CK_OUT; DT_CLR; DT_OUT


extern const uint8_t font5x8[];
extern const uint8_t font3x7[];


void maxtrix_init(void);

void matrix_command(uint8_t addr, uint8_t data);

void matrix_clear(void);
void matrix_fill(void);

void matrix_update(void);
/*
void matrix_char(uint8_t xpos, uint8_t code);
void matrix_print(uint8_t xpos, const char *str);
void matrix_print_compact(uint8_t xpos, const char *str);
 */
void matrix_char_small(uint8_t xpos, uint8_t code);
void matrix_print_small(uint8_t xpos, const char *str);

void matrix_clear_shift(void);
void matrix_copy_shift(uint16_t cpos);
void matrix_pixel_shift(uint16_t xpos, uint8_t ypos, uint8_t on);
void matrix_char_shift(uint16_t xpos, uint8_t code);
void matrix_print_shift(uint16_t xpos, const char *str);


#define MAX7219_NUM  4U

#define MATRIX_BUFF_SIZE  (8U*MAX7219_NUM)

#define MATRIX_SHIFT_BUFF_SIZE  (10U*MATRIX_BUFF_SIZE)


#define MAX7219_MAP_INTENSITY   0x0a
#define MAX7219_MAP_SHUTDOWN    0x0c
#define MAX7219_MAP_DECODEMODE  0x09
#define MAX7219_MAP_SCANLIMIT   0x0b
#define MAX7219_MAP_DISPTEST    0x0f

#define MAX7219_INTENSITY_MASK  0b00001111


#define MATRIX_BRIGHTNESS(v)  matrix_command(MAX7219_MAP_INTENSITY, ((v)&MAX7219_INTENSITY_MASK))


extern uint8_t matrixbuff[];

#define MATRIX_BUFF_PIXEL_ON(x,y)   (REG_BIT_SET(matrixbuff[(((x)%(MAX7219_NUM*8))+(((y)%8)*(MAX7219_NUM*8)))/8],7-((x)%8)))
#define MATRIX_BUFF_PIXEL_OFF(x,y)  (REG_BIT_CLR(matrixbuff[(((x)%(MAX7219_NUM*8))+(((y)%8)*(MAX7219_NUM*8)))/8],7-((x)%8)))


#endif /* MAX7219_H_ */



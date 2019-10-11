// file: main.c
// codepage: win-1251
// 11-10-2019

// Program 6660
// Data 509


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <pic18f4520.h>

#include "main.h"

#include "at24c32.h"
#include "max7219.h"
#include "ds3231.h"



char strbuff[16];  //string buffers
char strbuffsum[64];

uint8_t rtcdata[RTC_DATA_SIZE];  //ds3231 rtc data

uint8_t strbufflen=0;

uint16_t scrpos=0xffff;

uint8_t scrbright=0;
uint8_t scrmode=0;

volatile uint8_t scrcnt=0;
volatile uint8_t btncnt=0;
volatile uint8_t hldcnt=0;

const uint8_t buttonadc = 0xff;


const char DAY_1_EN[]="MONDAY";
const char DAY_2_EN[]="TUESDAY";
const char DAY_3_EN[]="WEDNESDAY";
const char DAY_4_EN[]="THURSDAY";
const char DAY_5_EN[]="FRIDAY";
const char DAY_6_EN[]="SATURDAY";
const char DAY_7_EN[]="SUNDAY";

const char MONTH_1_EN[]="JANUARY";
const char MONTH_2_EN[]="FEBRUARY";
const char MONTH_3_EN[]="MARCH";
const char MONTH_4_EN[]="APRIL";
const char MONTH_5_EN[]="MAY";
const char MONTH_6_EN[]="JUNE";
const char MONTH_7_EN[]="JULY";
const char MONTH_8_EN[]="AUGUST";
const char MONTH_9_EN[]="SEPTEMBER";
const char MONTH_10_EN[]="OCTOBER";
const char MONTH_11_EN[]="NOVEMBER";
const char MONTH_12_EN[]="DECEMBER";

#pragma warning disable 228

const char DAY_1_RU[]="ПОНЕДЕЛЬНИК";
const char DAY_2_RU[]="ВТОРНИК";
const char DAY_3_RU[]="СРЕДА";
const char DAY_4_RU[]="ЧЕТВЕРГ";
const char DAY_5_RU[]="ПЯТНИЦА";
const char DAY_6_RU[]="СУББОТА";
const char DAY_7_RU[]="ВОСКРЕСЕНЬЕ";

const char MONTH_1_RU[]="ЯНВАРЬ";
const char MONTH_2_RU[]="ФЕВРАЛЬ";
const char MONTH_3_RU[]="МАРТ";
const char MONTH_4_RU[]="АПРЕЛЬ";
const char MONTH_5_RU[]="МАЙ";
const char MONTH_6_RU[]="ИЮНЬ";
const char MONTH_7_RU[]="ИЮЛЬ";
const char MONTH_8_RU[]="АВГУСТ";
const char MONTH_9_RU[]="СЕНТЯБРЬ";
const char MONTH_10_RU[]="ОКТЯБРЬ";
const char MONTH_11_RU[]="НОЯБРЬ";
const char MONTH_12_RU[]="ДЕКАБРЬ";

#pragma warning enable 228



//-----------------------------------------------------------------------------
const char * month_name(uint8_t lang, uint8_t num)
    {
    if(lang==0)
        {
        switch(num)
            {
            case 1: return MONTH_1_EN;
            case 2: return MONTH_2_EN;
            case 3: return MONTH_3_EN;
            case 4: return MONTH_4_EN;
            case 5: return MONTH_5_EN;
            case 6: return MONTH_6_EN;
            case 7: return MONTH_7_EN;
            case 8: return MONTH_8_EN;
            case 9: return MONTH_9_EN;
            case 10: return MONTH_10_EN;
            case 11: return MONTH_11_EN;
            case 12: return MONTH_12_EN;
            default: break;
            }
        }

    if(lang==1)
        {
        switch(num)
            {
            case 1: return MONTH_1_RU;
            case 2: return MONTH_2_RU;
            case 3: return MONTH_3_RU;
            case 4: return MONTH_4_RU;
            case 5: return MONTH_5_RU;
            case 6: return MONTH_6_RU;
            case 7: return MONTH_7_RU;
            case 8: return MONTH_8_RU;
            case 9: return MONTH_9_RU;
            case 10: return MONTH_10_RU;
            case 11: return MONTH_11_RU;
            case 12: return MONTH_12_RU;
            default: break;
            }
        }

    return '\0';
    }


//-----------------------------------------------------------------------------
const char * day_name(uint8_t lang, uint8_t num)
    {
    if(lang==0)
        {
        switch(num)
            {
            case 1: return DAY_1_EN;
            case 2: return DAY_2_EN;
            case 3: return DAY_3_EN;
            case 4: return DAY_4_EN;
            case 5: return DAY_5_EN;
            case 6: return DAY_6_EN;
            case 7: return DAY_7_EN;
            default: break;
            }
        }

    if(lang==1)
        {
        switch(num)
            {
            case 1: return DAY_1_RU;
            case 2: return DAY_2_RU;
            case 3: return DAY_3_RU;
            case 4: return DAY_4_RU;
            case 5: return DAY_5_RU;
            case 6: return DAY_6_RU;
            case 7: return DAY_7_RU;
            default: break;
            }
        }

    return '\0';
    }


//-----------------------------------------------------------------------------
void delay_ms(volatile uint16_t val)
    {
    while(val-->0) __delay_us(1000);
    }


//=============================================================================
void interrupt handler(void)
    {
    if(TMR0IF && TMR0IE)  //timer0 overflow
        {
        TMR0IF=0;
        TMR0=TMR0_OVF_PRELOAD;

        if(scrcnt) scrcnt--;
        if(btncnt) btncnt--;
        if(hldcnt) hldcnt--;
        }
    }


//-----------------------------------------------------------------------------
uint16_t get_adc(uint8_t channel)
    {
    uint16_t adc=0;

    ADCON0=((channel<<2)&0b00111100);

    ADON=1;

    GODONE=1;
    while(GODONE);

    GODONE=1;
    while(GODONE);
    adc=ADRES;

    ADON=0;

    return adc;
    }


//-----------------------------------------------------------------------------
uint8_t button_state(void)
    {
    if(get_adc(0)<buttonadc) return 1;

    return 0;
    }


//-----------------------------------------------------------------------------
uint8_t button_check(void)
    {
    uint8_t k=0;

    static uint8_t stage=0;

    switch(stage)
        {
        case 0:
            if(btncnt==0)
                {
                btncnt=EVENT_PERIOD(25);

                if(button_state())
                    {
                    stage=1;
                    }
                }
            break;

        case 1:
            if(btncnt==0)
                {
                if(button_state())
                    {
                    stage=2;
                    hldcnt=255;
                    }
                else stage=0;
                }
            break;

        case 2:
            if(hldcnt<(255-(uint8_t)EVENT_PERIOD(500)))
                {
                k=2;
                stage=4;
                }
            else if(btncnt==0)
                {
                btncnt=EVENT_PERIOD(25);

                if(button_state()==0)
                    {
                    stage=3;
                    btncnt=EVENT_PERIOD(25);
                    }
                }
            break;

        case 3:
            if(btncnt==0)
                {
                if(button_state()==0)
                    {
                    stage=0;
                    k=1;
                    }
                }
            break;

        case 4:
            if(btncnt==0)
                {
                btncnt=EVENT_PERIOD(25);

                if(!button_state()==0)
                    {
                    stage=5;
                    btncnt=EVENT_PERIOD(25);
                    }
                }
            break;

        case 5:
            if(btncnt==0)
                {
                if(button_state()==0)
                    {
                    stage=0;
                    }
                }
            break;
        }

    return k;
    }


//-----------------------------------------------------------------------------
void clock_normal_mode(void)
    {
    scrcnt=EVENT_PERIOD(500);

    rtc_read(rtcdata);

    matrix_clear_shift();


    sprintf(strbuff, "%2u", rtcdata[HOURS_REG]);
    matrix_print_shift(0,strbuff);

    matrix_char_shift(14, ':');

    sprintf(strbuff, "%02u",rtcdata[MINUTES_REG]);
    matrix_print_shift(20,strbuff);

    matrix_copy_shift(0);
    }


//-----------------------------------------------------------------------------
void clock_shift_mode(uint8_t lang)
    {
    scrcnt=EVENT_PERIOD(25);

    if(scrpos>(((uint16_t)strbufflen*7U)+MATRIX_BUFF_SIZE))
        {
        scrpos=0;

        rtc_read(rtcdata);

        int8_t tempmsb=ds3231_read_reg(0x11);
        //uint8_t templsb=ds3231_read_reg(0x12);

        strbuffsum[0]=0; //"clear" the buffer

        sprintf(strbuff, "%u:%02u ", rtcdata[HOURS_REG] ,rtcdata[MINUTES_REG]);
        strcat(strbuffsum, strbuff);

        strcat(strbuffsum, day_name(lang, rtcdata[DAY_REG]));

        sprintf(strbuff, " %u ", rtcdata[DATE_REG]);
        strcat(strbuffsum, strbuff);

        strcat(strbuffsum, month_name(lang, rtcdata[MONTH_REG]));

        sprintf(strbuff, " 20%02u", rtcdata[YEAR_REG]);
        strcat(strbuffsum, strbuff);

        //sprintf(strbuff, " %+d.%u", tempmsb,(templsb>>6)*25);
        sprintf(strbuff, " %+d", tempmsb);
        strcat(strbuffsum, strbuff);

        strbufflen=strlen(strbuffsum);

        matrix_clear_shift();
        matrix_print_shift(32,strbuffsum);
        }

    matrix_copy_shift(scrpos++);
    }


//-----------------------------------------------------------------------------
void clock_compact_mode(void)
    {
    scrcnt=EVENT_PERIOD(200);

    rtc_read(rtcdata);

    sprintf(strbuff, "%02u:%02u:%02u", rtcdata[HOURS_REG], rtcdata[MINUTES_REG], rtcdata[SECONDS_REG]);
    matrix_print_small(0,strbuff);
    }


//-----------------------------------------------------------------------------
void clock_settings_mode(uint8_t mode)
    {
    if(mode==5) scrcnt=EVENT_PERIOD(200);
    else scrcnt=EVENT_PERIOD(500);

    rtc_read(rtcdata);

    matrix_clear_shift();

    uint8_t t=0;

    if(mode==1) { matrix_print_shift_compact(0,"MODE"); t=scrmode+1; }
    if(mode==2) { matrix_print_shift_compact(0,"BRIG"); t=scrbright+1; }
    if(mode==3) { matrix_print_shift_compact(0,"HOU"); t=rtcdata[HOURS_REG]; }
    if(mode==4) { matrix_print_shift_compact(0,"MIN"); t=rtcdata[MINUTES_REG]; }
    if(mode==5) { matrix_print_shift_compact(0,"SEC"); t=rtcdata[SECONDS_REG]; }
    if(mode==6) { matrix_print_shift_compact(0,"DAY"); t=rtcdata[DAY_REG]; }
    if(mode==7) { matrix_print_shift_compact(0,"DAT"); t=rtcdata[DATE_REG]; }
    if(mode==8) { matrix_print_shift_compact(0,"MON"); t=rtcdata[MONTH_REG]; }
    if(mode==9) { matrix_print_shift_compact(0,"YEA"); t=rtcdata[YEAR_REG]; }

    if(mode==1 || mode==2)
        {
        sprintf(strbuff, "%1u", t);
        matrix_print_shift(32-5,strbuff);
        }
    else
        {
        sprintf(strbuff, "%02u", t);
        matrix_print_shift((32-(5+5+2)),strbuff);
        }

    matrix_copy_shift(0);
    }


//-----------------------------------------------------------------------------
inline void sys_init(void)
    {
    IRCF2=1; //111=8M 110=4M 101=2M 100=1M 011=500k 010=250k 001=125k 000=31k
    IRCF1=1;
    IRCF0=1;

    //INTSRC PLLEN - TUN4 TUN3 TUN2 TUN1 TUN0
    OSCTUNE=0b00000010;

    SBOREN=1; //1=enable brown out reset
    SWDTEN=0; //0=disable watchdog timer
    RBPU=1;   //0=enable pull-up

    PEIE=1;
    GIE=1;

    //TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0
    T0CON=0b00000000|TMR0_PRESCALER;  //prescaler 000=1:2 001=1:4 010=1:8 101=1:64 110=1:128
    TMR0=TMR0_OVF_PRELOAD;
    TMR0ON=1;
    TMR0IF=0;
    TMR0IE=1;

    //RD16 T1RUN T1CKPS1 T1CKPS0 T1OSCEN T1SYNC TMR1CS TMR1ON
    T1CON=0b10000000;  //11 = 1:8  //10 = 1:4  //00 = 1:1
    TMR1=0;
    TMR1ON=0;
    TMR1IF=0;
    TMR1IE=0;

    //- TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
    T2CON=0b00000000;  //01-1:4  //1x-1:16
    TMR2=0;
    TMR2ON=0;
    TMR2IF=0;
    TMR2IE=0;

    ////RD16 T3CCP2 T3CKPS1 T3CKPS0 T3CCP1 T3SYNC TMR3CS TMR3ON
    T3CON=0b00000000;  //11 = 1:8 //10 = 1:4 //01 = 1:2 //00 = 1:1 Prescale value
    TMR3=0;
    TMR3ON=0;
    TMR3IF=0;
    TMR3IE=0;

    //- - DCxB1 DCxB0 CCPxM3 CCPxM2 CCPxM1 CCPxM0
    CCP2CON=0b00000000;

    CMCON=0b000111; //Comparators Off

    ADCON1 = 0b001111; //5-Vref=Vss //4+Vref=Vdd //Port AN0
    ADCON0 = 0b000000; //Channel 0  //A/D converter module is disabled
    //ADFM — ACQT2 ACQT1 ACQT0 ADCS2 ADCS1 ADCS0
    ADCON2 = 0b10010101; //TACQ 010=4TAD //TAD 101=FOSC/16 Fosc/16=0.5M=2us  //111=FRC
    //ADON=1; //Turn on A/D module
    }


//-----------------------------------------------------------------------------
void main(void)
    {
    sys_init();
    i2c_init();
    ds3231_init();
    delay_ms(100);  //delay for matrix power up
    matrix_init();

    scrbright=ee_read(BRIGHTNESS_EE_ADDR)&0b00000111;
    uint8_t oldbright=scrbright;

    MATRIX_BRIGHTNESS(scrbright);

    scrmode=ee_read(SCREENMODE_EE_ADDR);
    uint8_t oldscrmode=scrmode;

    uint8_t setm=0;

    for(;;)
        {
        if(scrcnt==0)
            {
            if(setm==0)
                {
                if(scrmode==0) clock_normal_mode();
                if(scrmode==1) clock_shift_mode(0);
                if(scrmode==2) clock_shift_mode(1);
                if(scrmode==3) clock_compact_mode();
                }
            else clock_settings_mode(setm);

            matrix_update();
            }

        switch(button_check())
            {
            case 2:
                scrcnt=0;
                scrpos=0xffff;

                if(setm==1) { if(scrmode!=oldscrmode) { oldscrmode=scrmode; ee_write(SCREENMODE_EE_ADDR,scrmode); setm=0; break; } }
                if(setm==2) { if(scrbright!=oldbright) { oldbright=scrbright; ee_write(BRIGHTNESS_EE_ADDR,scrbright); setm=0; break; } }
                if(++setm>9) setm=0;
                break;

            case 1:
                if(setm==0) { break; }

                scrcnt=0;
                scrpos=0xffff;

                if(setm==1) { uint8_t tmp=scrmode; if(++tmp>3) tmp=0; scrmode=tmp; }
                if(setm==2) { uint8_t tmp=scrbright; if(++tmp>7) tmp=0; MATRIX_BRIGHTNESS(tmp); scrbright=tmp; }
                if(setm==3) { uint8_t tmp=rtcdata[HOURS_REG]; if(++tmp>23) tmp=0; rtc_set_hrs(tmp); }
                if(setm==4) { uint8_t tmp=rtcdata[MINUTES_REG]; if(++tmp>59) tmp=0; rtc_set_min(tmp); }
                if(setm==5) { rtc_set_sec(0); }
                if(setm==6) { uint8_t tmp=rtcdata[DAY_REG]; if(++tmp>7) tmp=1; rtc_set_day(tmp); }
                if(setm==7) { uint8_t tmp=rtcdata[DATE_REG]; if(++tmp>31) tmp=1; rtc_set_dat(tmp); }
                if(setm==8) { uint8_t tmp=rtcdata[MONTH_REG]; if(++tmp>12) tmp=1; rtc_set_mon(tmp); }
                if(setm==9) { uint8_t tmp=rtcdata[YEAR_REG]; if(++tmp>99) tmp=0; rtc_set_year(tmp); }
                break;

            case 0: break;
            default: break;
            }
        }
    }


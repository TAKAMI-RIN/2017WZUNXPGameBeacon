#ifndef __TIMECONTROL_H__
#define __TIMECONTROL_H__
#include "pit.h"
#include "CMOS.h"
#define MinSpeed 0
/*½á¹¹Ìå*/
typedef struct{
    uint8_t Arrival_1ms;                          
    uint8_t Arrival_2ms;                              
    uint8_t Arrival_3ms; 
    uint8_t Arrival_4ms;
    uint8_t Arrival_5ms;  	
    uint8_t Arrival_6ms;                          
    uint8_t Arrival_7ms;                              
    uint8_t Arrival_8ms; 
    uint8_t Arrival_9ms;
    uint8_t Arrival_10ms;
	  uint8_t Arrival_50ms;
	  uint8_t Arrival_100ms;
	  uint8_t Arrival_500ms;
	  uint8_t Arrival_1000ms;
}TimeFlagTypedef;

extern TimeFlagTypedef TimeFlag;

void TimeCrtlInit(void);
void TimeCrtl_ISR(void);
	
#endif


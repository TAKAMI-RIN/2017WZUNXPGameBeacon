#include "led.h"
#include "gpio.h"
#include "common.h"


void led_Init()
{
  GPIO_QuickInit(HW_GPIOB, 20, kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOB, 21, kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOB, 22, kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOB, 23, kGPIO_Mode_OPP);
}

void led0_on()
{
   GPIO_ResetBit(HW_GPIOB, 20);
}
void led1_on()
{
   GPIO_ResetBit(HW_GPIOB, 21);
}
void led2_on()
{
   GPIO_ResetBit(HW_GPIOB, 22);
}
void led3_on()
{
   GPIO_ResetBit(HW_GPIOB, 23);
}
void led0_off()
{
   GPIO_SetBit(HW_GPIOB, 20);
}
void led1_off()
{
   GPIO_SetBit(HW_GPIOB, 21);
}
void led2_off()
{
   GPIO_SetBit(HW_GPIOB, 22);
}
void led3_off()
{
   GPIO_SetBit(HW_GPIOB, 23);
}
#include "mode.h"
#include "gpio.h"

void mode_init()
{
	
  GPIO_QuickInit(HW_GPIOD, 10, kGPIO_Mode_IPU);

}

uint8_t getmode_State()
{
	uint8_t a;
     a = GPIO_ReadBit(HW_GPIOD, 10);
	
	return a;
}


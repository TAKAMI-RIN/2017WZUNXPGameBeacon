#include "TimeControl.h"

TimeFlagTypedef TimeFlag;

void TimeCrtlInit(void)
{
    /* 初始化PIT模块 */
    PIT_InitTypeDef PIT_InitStruct1;  //申请结构体变量
    PIT_InitStruct1.chl = HW_PIT_CH1; /* 使用0号定时器 */
    PIT_InitStruct1.timeInUs = 1000; /* 定时周期1mS */
    PIT_Init(&PIT_InitStruct1); //pit模块初始化
    /* 注册PIT 中断回调函数 */
    PIT_CallbackInstall(HW_PIT_CH1, TimeCrtl_ISR); //1号定时器的中断处理
    /* 开启PIT0定时器中断 */
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
}

void TimeCrtl_ISR(void)
{
    static uint32_t TimeSlice = 0;
		TimeSlice++;
	  
	  TimeFlag.Arrival_1ms = 1;
    if(TimeSlice % 50 == 0)
    {
		    TimeFlag.Arrival_50ms = 1;
		}

	  if(TimeSlice % 100 == 0)
    {
		    TimeFlag.Arrival_100ms = 1;
		}
		if(TimeSlice % 500 == 0)
    {
		    TimeFlag.Arrival_500ms = 1;
		}
		if(TimeSlice % 1000 == 0)
    {
        TimeFlag.Arrival_1000ms = 1;
	
		}		
		if(TimeSlice == 4000000000) 
		{
		    TimeSlice = 0;
		}   
}


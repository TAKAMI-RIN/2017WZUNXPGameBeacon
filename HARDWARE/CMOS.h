#ifndef __CMOS_H__
#define __CMOS_H__
#define SCL HW_GPIOA 
#define SDA PTE9_INT 

#include "gpio.h"
#include "dma.h"
#include "math.h"
#include "common.h"

#define CMOS_W 120
#define CMOS_H 31 //34
#define AllPixel 4800
#define PowAllPixel 23040000
#define CMP_NUM 15


/* 引脚定义 */
#define CMOS_VSYNC_PORT     HW_GPIOC
#define CMOS_VSYNC_PIN      (17) //摄像头场中断
#define CMOS_HREF_PORT      HW_GPIOC 
#define CMOS_HREF_PIN       (16) //摄像头行中断
#define CMOS_DATA_PORT      HW_GPIOD//数据口设置
#define CMOS_DATA_OFFSET    (0) /* 摄像头数据引脚PTA8-PTA15 只能为 0 8 16 24 */

/*枚举体*/
typedef enum {
	  Init,              //初始状态
    FinishSampling,    //完成采样
	  FinishProcess,     //完成处理
}ImageDataFlagTypedef; 

typedef enum {
    usefull,
	  useless
}RowFlagTypedef; 

typedef enum {
    Nothing,
	  AtLeft,
	  AtRight
}ObstacleFlagTypedef; 


/*结构体*/
typedef struct{
    uint16_t PictureCount;                          //场计数器
    uint16_t RowCount;                              //行计数器
    uint16_t CollectRowCount;                       //采样行计数器	
}CountTypedef;
typedef struct{
    ImageDataFlagTypedef Image;    //图像数据状态标志
    RowFlagTypedef LeftFlag[CMOS_H];//行是否有用标志
	  RowFlagTypedef RighttFlag[CMOS_H];//行是否有用标志
	  ObstacleFlagTypedef ObstacleFlag;  
}FlagTypedef;
typedef struct{
    uint8_t White;                          //场计数器
    uint8_t Black;                              //行计数器
    uint8_t Dif;                       //采样行计数器	
}BasicGreyTypedef;


/*数据声明*/
extern uint8_t ImageBuf[CMOS_H][CMOS_W+1];    //摄像头数据缓存区
extern uint8_t BinaryImage[CMOS_H][CMOS_W];    //摄像头数据缓存区
//extern int16_t LeftLine[CMOS_H];
//extern int16_t RightLine[CMOS_H];
//extern int16_t MidcourtLine[CMOS_H];
extern CountTypedef Count;
extern FlagTypedef CMOS_Flag;
//extern uint16_t  UselessRowNum;  //无效行数
//extern uint16_t  UsefullRowNum;
//extern uint8_t k1;
//extern float k2;
extern float JudgeNum;
extern BasicGreyTypedef BasicGrey;
extern uint8_t AO_WhiteCMP;
	  

void CMOS_Init(void);
void CMOS_ISR(uint32_t pinArray);
void DMA_Ch0_ISR(void);


void delay(void);
void Binaryzation(uint8_t line,uint8_t CMP);

#endif


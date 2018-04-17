#include "CMOS.h"

/**
  * @file    CMOS.c
  * @version V1.1
  * @date    2016.6.15
  * @note    CMOS driver  新增有效行判断
  */
	
/**
 * @brief  捕食者初始化函数
 * @param  
 * @return 
 */
 
uint8_t ImageBuf[CMOS_H][CMOS_W+1] = { 0 };    //摄像头数据缓存区
uint8_t BinaryImage[CMOS_H][CMOS_W] = { 0 };    //摄像头数据缓存区
CountTypedef Count;
FlagTypedef CMOS_Flag;
float JudgeNum = 0;
unsigned int const NeedCollectRow[70]={

/*114,        118,        122,        126,        131,        136,        141,     146,
151,*/         156,        161,        166,        171,         176,       181,    186,
192,        198,        204,        210,        216,        222,        228,     234,
240,        246,        252,        258,        264,        270,        276,     282,
288,        296,        304,        312,        320,        328,        336,     342,
350,
	
};

uint8_t HisTogram[256];                  //原始灰度统计直方图
void CMOS_Init()
{
	  /**********DMA结构定义**********/
	  DMA_InitTypeDef DMA_InitStruct; 
	  /**********数据定义************/
	  uint32_t i;
	  /**********GPIO配置****************/
    GPIO_QuickInit(HW_GPIOC,17,kGPIO_Mode_IPU);    //场中断
	  GPIO_QuickInit(HW_GPIOC,16,kGPIO_Mode_IPU);    //行中断
	  
	  GPIO_CallbackInstall(CMOS_VSYNC_PORT,CMOS_ISR); 
	  
	  GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,ENABLE);
	  GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,ENABLE);

    for(i=0;i<8;i++)/* 初始化数据端口 */
    {
        GPIO_QuickInit(CMOS_DATA_PORT, CMOS_DATA_OFFSET+i, kGPIO_Mode_IPD); //设置数据口为浮空输入
    }
		//DMA_CallbackInstall(HW_DMA_CH0,DMA_Ch0_ISR);
    //DMA_ITConfig(HW_DMA_CH0,kDMA_IT_Major,ENABLE);
  
	  /***********DMA配置***************/
    DMA_InitStruct.chl = HW_DMA_CH0;                             ///< DMA通道号0    
    DMA_InitStruct.chlTriggerSource = MUX0_DMAREQ;                   ///< 没有触发源 
    DMA_InitStruct.minorLoopByteCnt = 1;                    ///< 内循环N次
    DMA_InitStruct.majorLoopCnt = CMOS_W+1;                       ///< 外循环1次
    DMA_InitStruct.triggerSourceMode = kDMA_TriggerSource_Normal;                  ///< 触发模式选择
    /* 源地址配置 */
    DMA_InitStruct.sAddrOffset = 0;                ///< DMA每次读取sAddr后 sAddr的偏移量  可正可负 
    DMA_InitStruct.sAddr = (uint32_t)&PTD->PDIR;                      ///< 数据源地址
    DMA_InitStruct.sDataWidth = kDMA_DataWidthBit_8;                 ///< 一次传输的数据大小 8 16 32 
    DMA_InitStruct.sLastAddrAdj = 0;               ///< 当内外循环结束后 sAddr的偏移量 
    DMA_InitStruct.sMod = kDMA_ModuloDisable;                       ///< Modulo 见 AN2898 
    /* 目的地址配置 */
    DMA_InitStruct.dAddrOffset = 1;                     //搬完一个元素之后 目标地址自动加一
    DMA_InitStruct.dAddr = (uint32_t)ImageBuf[0];       //               
    DMA_InitStruct.dDataWidth = kDMA_DataWidthBit_8;    //                 
    DMA_InitStruct.dLastAddrAdj = 0;               
    DMA_InitStruct.dMod = kDMA_ModuloDisable;  
	  
	  DMA_Init(&DMA_InitStruct);  
}


/**
 * @brief  捕食者场中断\行中断函数
 * @param  pinArray: 
 * @return 
 */  
void CMOS_ISR(uint32_t pinArray)
{
    if(pinArray & (1 << CMOS_VSYNC_PIN))    //场中断
		{
		    /*****一场开始前的初始化*****/
			  if(CMOS_Flag.Image == FinishProcess)
        {
					  CMOS_Flag.Image = Init;
						Count.RowCount = 0;
						Count.CollectRowCount = 0;
						Count.PictureCount ++;
				}
			  
			  
		}
		if(pinArray & (1 << CMOS_HREF_PIN ))    //行中断   
		{
			  Count.RowCount ++;
			  /*if(NeedCollectRow[Count.CollectRowCount] == Count.RowCount &&
					 Count.CollectRowCount <= CMOS_H)    //是需要的行数
        { */
			  DelayUs(1);
			  delay();

       if(NeedCollectRow[Count.CollectRowCount] == Count.RowCount &&
					 Count.CollectRowCount <= CMOS_H) 
        {
			        if(Count.CollectRowCount == CMOS_H)
              {
								  
							    CMOS_Flag.Image = FinishSampling;
							} 
              else
              {
				       DMA_SetDestAddress(HW_DMA_CH0, (uint32_t)ImageBuf[CMOS_H-Count.CollectRowCount-1]);//设定新的DMA目标地址
					     DMA_EnableRequest(HW_DMA_CH0);       //开启DMA数据传输 							
							}							
						  Count.CollectRowCount ++;	
							
				}
				else
        {
 
				}
		}
}

/**
 * @brief  DMA_CH0中断服务函数
 * @param 
 * @return 
 */
/*void DMA_Ch0_ISR()
{
    RowFlag[Count.RowCount].ImageData = FinishSampling;//标记上行数据采样完成
    Count.RowCount ++;
}*/
void delay(void)
{
    uint32_t i;
	  for(i=0;i<=0;i++);
}

/**
 * @brief  二值化函数
 * @param  line:需要二值化的行数 CMP:阈值
 * @return 
 */
void Binaryzation(uint8_t line,uint8_t CMP)   //从下往上，从左到右二值化
{
	  uint8_t  i = 0,j=0;
	
		for(j=0;j<line;j++)
		{  
		 for(i=0;i<CMOS_W;i++)
			{
					if(ImageBuf[j][i] < CMP)
					{
							BinaryImage[j][i] = 0;
					}
					else
					{
							BinaryImage[j][i] = 1;
					}
			}  
		}
}


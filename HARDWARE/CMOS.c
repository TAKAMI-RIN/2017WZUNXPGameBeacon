#include "CMOS.h"

/**
  * @file    CMOS.c
  * @version V1.1
  * @date    2016.6.15
  * @note    CMOS driver  ������Ч���ж�
  */
	
/**
 * @brief  ��ʳ�߳�ʼ������
 * @param  
 * @return 
 */
 
uint8_t ImageBuf[CMOS_H][CMOS_W+1] = { 0 };    //����ͷ���ݻ�����
uint8_t BinaryImage[CMOS_H][CMOS_W] = { 0 };    //����ͷ���ݻ�����
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

uint8_t HisTogram[256];                  //ԭʼ�Ҷ�ͳ��ֱ��ͼ
void CMOS_Init()
{
	  /**********DMA�ṹ����**********/
	  DMA_InitTypeDef DMA_InitStruct; 
	  /**********���ݶ���************/
	  uint32_t i;
	  /**********GPIO����****************/
    GPIO_QuickInit(HW_GPIOC,17,kGPIO_Mode_IPU);    //���ж�
	  GPIO_QuickInit(HW_GPIOC,16,kGPIO_Mode_IPU);    //���ж�
	  
	  GPIO_CallbackInstall(CMOS_VSYNC_PORT,CMOS_ISR); 
	  
	  GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,ENABLE);
	  GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,ENABLE);

    for(i=0;i<8;i++)/* ��ʼ�����ݶ˿� */
    {
        GPIO_QuickInit(CMOS_DATA_PORT, CMOS_DATA_OFFSET+i, kGPIO_Mode_IPD); //�������ݿ�Ϊ��������
    }
		//DMA_CallbackInstall(HW_DMA_CH0,DMA_Ch0_ISR);
    //DMA_ITConfig(HW_DMA_CH0,kDMA_IT_Major,ENABLE);
  
	  /***********DMA����***************/
    DMA_InitStruct.chl = HW_DMA_CH0;                             ///< DMAͨ����0    
    DMA_InitStruct.chlTriggerSource = MUX0_DMAREQ;                   ///< û�д���Դ 
    DMA_InitStruct.minorLoopByteCnt = 1;                    ///< ��ѭ��N��
    DMA_InitStruct.majorLoopCnt = CMOS_W+1;                       ///< ��ѭ��1��
    DMA_InitStruct.triggerSourceMode = kDMA_TriggerSource_Normal;                  ///< ����ģʽѡ��
    /* Դ��ַ���� */
    DMA_InitStruct.sAddrOffset = 0;                ///< DMAÿ�ζ�ȡsAddr�� sAddr��ƫ����  �����ɸ� 
    DMA_InitStruct.sAddr = (uint32_t)&PTD->PDIR;                      ///< ����Դ��ַ
    DMA_InitStruct.sDataWidth = kDMA_DataWidthBit_8;                 ///< һ�δ�������ݴ�С 8 16 32 
    DMA_InitStruct.sLastAddrAdj = 0;               ///< ������ѭ�������� sAddr��ƫ���� 
    DMA_InitStruct.sMod = kDMA_ModuloDisable;                       ///< Modulo �� AN2898 
    /* Ŀ�ĵ�ַ���� */
    DMA_InitStruct.dAddrOffset = 1;                     //����һ��Ԫ��֮�� Ŀ���ַ�Զ���һ
    DMA_InitStruct.dAddr = (uint32_t)ImageBuf[0];       //               
    DMA_InitStruct.dDataWidth = kDMA_DataWidthBit_8;    //                 
    DMA_InitStruct.dLastAddrAdj = 0;               
    DMA_InitStruct.dMod = kDMA_ModuloDisable;  
	  
	  DMA_Init(&DMA_InitStruct);  
}


/**
 * @brief  ��ʳ�߳��ж�\���жϺ���
 * @param  pinArray: 
 * @return 
 */  
void CMOS_ISR(uint32_t pinArray)
{
    if(pinArray & (1 << CMOS_VSYNC_PIN))    //���ж�
		{
		    /*****һ����ʼǰ�ĳ�ʼ��*****/
			  if(CMOS_Flag.Image == FinishProcess)
        {
					  CMOS_Flag.Image = Init;
						Count.RowCount = 0;
						Count.CollectRowCount = 0;
						Count.PictureCount ++;
				}
			  
			  
		}
		if(pinArray & (1 << CMOS_HREF_PIN ))    //���ж�   
		{
			  Count.RowCount ++;
			  /*if(NeedCollectRow[Count.CollectRowCount] == Count.RowCount &&
					 Count.CollectRowCount <= CMOS_H)    //����Ҫ������
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
				       DMA_SetDestAddress(HW_DMA_CH0, (uint32_t)ImageBuf[CMOS_H-Count.CollectRowCount-1]);//�趨�µ�DMAĿ���ַ
					     DMA_EnableRequest(HW_DMA_CH0);       //����DMA���ݴ��� 							
							}							
						  Count.CollectRowCount ++;	
							
				}
				else
        {
 
				}
		}
}

/**
 * @brief  DMA_CH0�жϷ�����
 * @param 
 * @return 
 */
/*void DMA_Ch0_ISR()
{
    RowFlag[Count.RowCount].ImageData = FinishSampling;//����������ݲ������
    Count.RowCount ++;
}*/
void delay(void)
{
    uint32_t i;
	  for(i=0;i<=0;i++);
}

/**
 * @brief  ��ֵ������
 * @param  line:��Ҫ��ֵ�������� CMP:��ֵ
 * @return 
 */
void Binaryzation(uint8_t line,uint8_t CMP)   //�������ϣ������Ҷ�ֵ��
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


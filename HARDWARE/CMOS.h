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


/* ���Ŷ��� */
#define CMOS_VSYNC_PORT     HW_GPIOC
#define CMOS_VSYNC_PIN      (17) //����ͷ���ж�
#define CMOS_HREF_PORT      HW_GPIOC 
#define CMOS_HREF_PIN       (16) //����ͷ���ж�
#define CMOS_DATA_PORT      HW_GPIOD//���ݿ�����
#define CMOS_DATA_OFFSET    (0) /* ����ͷ��������PTA8-PTA15 ֻ��Ϊ 0 8 16 24 */

/*ö����*/
typedef enum {
	  Init,              //��ʼ״̬
    FinishSampling,    //��ɲ���
	  FinishProcess,     //��ɴ���
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


/*�ṹ��*/
typedef struct{
    uint16_t PictureCount;                          //��������
    uint16_t RowCount;                              //�м�����
    uint16_t CollectRowCount;                       //�����м�����	
}CountTypedef;
typedef struct{
    ImageDataFlagTypedef Image;    //ͼ������״̬��־
    RowFlagTypedef LeftFlag[CMOS_H];//���Ƿ����ñ�־
	  RowFlagTypedef RighttFlag[CMOS_H];//���Ƿ����ñ�־
	  ObstacleFlagTypedef ObstacleFlag;  
}FlagTypedef;
typedef struct{
    uint8_t White;                          //��������
    uint8_t Black;                              //�м�����
    uint8_t Dif;                       //�����м�����	
}BasicGreyTypedef;


/*��������*/
extern uint8_t ImageBuf[CMOS_H][CMOS_W+1];    //����ͷ���ݻ�����
extern uint8_t BinaryImage[CMOS_H][CMOS_W];    //����ͷ���ݻ�����
//extern int16_t LeftLine[CMOS_H];
//extern int16_t RightLine[CMOS_H];
//extern int16_t MidcourtLine[CMOS_H];
extern CountTypedef Count;
extern FlagTypedef CMOS_Flag;
//extern uint16_t  UselessRowNum;  //��Ч����
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


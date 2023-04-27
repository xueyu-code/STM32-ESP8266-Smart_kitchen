#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "usart2.h"
#include "timer.h"
#include "gizwits_product.h"
#include "math.h"
#include "adc.h"
#include "stm32f10x_adc.h"
#include "BEEP.h"

//声明
int flag=0;
int MQ2_flag,Heat_flag,MQ5_flag,MQ9_flag;
int MQ2_threshold,MQ5_threshold,MQ9_threshold,fire_threshold;
int beep_flag=0;
uint16_t Heat,MQ2,MQ5,MQ9;/*Heat：火焰传感器，MQ2:烟雾检测，MQ5:煤气液化气检测，MQ9:可燃气体检测*/
int main(void)
{	
	int i;
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
// 	Init_LEDpin();			     //LED端口初始化
	adc_Init();
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 
	TIM3_Int_Init(71,999);				//一毫秒定时
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	usart3_init((u32)9600);//WIFI初始化
	gizwitsInit();//缓冲区初始化
//	gizwitsSetMode(WIFI_RESET_MODE);
	delay_ms(500);
//	gizwitsSetMode(WIFI_AIRLINK_MODE);
	gizwitsSetMode(WIFI_SOFTAP_MODE);
	BEEP_Init();

	OLED_ShowString(0,0,"Heat:",16);
	OLED_ShowString(0,2,"MQ2:",16);
	OLED_ShowString(0,4,"MQ5:",16);
	OLED_ShowString(0,6,"MQ9:",16);

	while(1)
	{

		
		Heat = AD_GetValue(ADC_Channel_0);/*读取传感器的值*/
		MQ2 = AD_GetValue(ADC_Channel_1);
		MQ5 = AD_GetValue(ADC_Channel_2);
		MQ9 = AD_GetValue(ADC_Channel_3);
		
		
		userHandle();
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
		
		//进行相关报警的阈值检测
		if(MQ2>MQ2_threshold&&beep_flag==1)//烟雾检测报警
		{
			MQ2_flag=1;
			BEEP_ON();
		}
		else
		{
			MQ2_flag=0;
		}
		///////////////////////
		if(Heat<fire_threshold&&beep_flag==1)//火焰检测报警
		{
			Heat_flag=1;
			BEEP_ON();
		}
		else
		{
			Heat_flag=0;
		}
		/////////////////////
		if(MQ5>MQ5_threshold&&beep_flag==1)//煤气检测报警
		{
			MQ5_flag=1;
			BEEP_ON();
		}
		else
		{
			MQ5_flag=0;
		}
		///////////////////////
		if(MQ9>MQ9_threshold&&beep_flag==1)//一氧化碳检测报警
		{
			MQ9_flag=1;
			BEEP_ON();
		}
		else
		{
			MQ9_flag=0;
		}
		OLED_ShowNum(70,0,Heat,3,16);
		OLED_ShowNum(70,2,MQ2,3,16);
		OLED_ShowNum(70,4,MQ5,3,16);
		OLED_ShowNum(70,6,MQ9,3,16);
}
}










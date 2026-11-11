#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"
#include "Buzzer.h"
#include "hx711.h"
#include "led.h"
#include "usart.h"

DHT11_Data_TypeDef DHT11_Data;

int value;
float weight;
int32_t reset;
u8 buff[30];

float Weights=100.0;  //100g
int32_t Weights_100=8493860;  //100g


int main(void)
{
	OLED_Init();
	Buzzer_Init();
	DHT11_GPIO_Config();
	
	
	SystemInit();//配置系统时钟为72M	
	delay_init(72);
	LED_Init();
	LED_On();
	HX711_Init();
	USART1_Config();//串口初始化
	
	OLED_Init();
	printf("Start \n");
	Delay_ms(1000);
	
	reset = HX711_GetData();
	
	
		
	
	OLED_ShowString(1, 1, "temp:");
	OLED_ShowString(2, 1, "humidity:");
	OLED_ShowString(3, 1, "weight:");
	
	while (1)
	{
		
		if(Read_DHT11(&DHT11_Data) == SUCCESS)
		{
			OLED_ShowNum(2, 10, DHT11_Data.humi_int, 2);
			OLED_ShowString(2, 12, ".");
			OLED_ShowNum(2, 13, DHT11_Data.humi_deci, 2);
			
			OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);
			OLED_ShowString(1, 8, ".");
			OLED_ShowNum(1, 9, DHT11_Data.temp_deci, 1);
			if (DHT11_Data.humi_int>45)		//如果当前光敏输出1
		{
			Buzzer_ON();				//蜂鸣器开启
		}
		else							//否则
		{
			Buzzer_OFF();				//蜂鸣器关闭
		}
			
		}
		
		
		LED_Toggle();
		value = HX711_GetData();
		
		weight=(float)(value-reset)*Weights/(float)(Weights_100-reset);
		
		sprintf((char*)buff,"%.1f   ",weight);
		
		printf("重量: %f\r\n",weight);
		
		//OLED_ShowString(50,30,".");

		Delay_ms(500);
	}
}

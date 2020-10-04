#include "gpio.h"

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void gpio_init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOC, &GPIO_InitStructure);
	
 GPIO_ResetBits(GPIOC,GPIO_Pin_13);						 

}

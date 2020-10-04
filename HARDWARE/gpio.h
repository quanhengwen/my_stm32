#ifndef __GPIO_H_
#define __GPIO_H_
#include "main.h"


#define  AIR302_RSR_H 	GPIO_SetBits(GPIOC,GPIO_Pin_13)	
#define  AIR302_RSR_L 	GPIO_ResetBits(GPIOC,GPIO_Pin_13)

void gpio_init(void);

#endif

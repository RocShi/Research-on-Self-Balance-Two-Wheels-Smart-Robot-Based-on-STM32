#ifndef __LED_H  //条件编译
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	

void LED_Init(void);//初始化

		 				    
#endif

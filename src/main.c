#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"

void gpioConfig(){

	GPIO_InitTypeDef	GPIOInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	// for leds
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIOInitStructure.GPIO_OType=GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIOInitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);

	// button
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIOInitStructure.GPIO_OType=GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIOInitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC,&GPIOInitStructure);

}

void extiConfig(){
	EXTI_InitTypeDef	EXTIInitStructure;
	NVIC_InitTypeDef	NVICInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,GPIO_PinSource6);
	//external interrupt path
	EXTIInitStructure.EXTI_Line=EXTI_Line6;
	EXTIInitStructure.EXTI_LineCmd=ENABLE;
	EXTIInitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;

	EXTI_Init(&EXTIInitStructure);
	//NVÝC Part
	NVICInitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;

	NVIC_Init(&NVICInitStructure);


}

void delay(uint32_t time){

	while(time--){}
}

void EXTI9_5_IRQHandler(){

	if(EXTI_GetITStatus(EXTI_Line6) != RESET){

		for(int i=0;i<4;i++){
			GPIO_SetBits(GPIOB,GPIO_Pin_3);
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			delay(3600000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			delay(3600000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}



int main(void){
	gpioConfig();
	extiConfig();

	int LedArray[3]={GPIO_Pin_0 , GPIO_Pin_1,  GPIO_Pin_2};
  while (1){
	  for(int i=0;i<3;i++){
		  GPIO_SetBits(GPIOB,LedArray[i]);
		  delay(3600000);
		  GPIO_ResetBits(GPIOB,LedArray[i]);
		  delay(3600000);

	  }
	  for(int i=1;i>0;i--){
		  GPIO_SetBits(GPIOB,LedArray[i]);
		  delay(3600000);
		  GPIO_ResetBits(GPIOB,LedArray[i]);
		  delay(3600000);


	  }
  }
}

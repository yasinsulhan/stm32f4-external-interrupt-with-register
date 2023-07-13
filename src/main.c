#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void CLK_Config()
{
	RCC->CR |= 0x00010000;	// HSEON enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;		// CSS Flag clear
}

void GPIO_Config()
{
	RCC->AHB1ENR = 0x00000009; 		// GPIOA and GPIOD active

	GPIOD->MODER = 0x55000000;		// 12-15 output
	GPIOD->OTYPER = 0x00000000;		// push-pull
	GPIOD->OSPEEDR = 0xFF000000;	// High Speed
	GPIOD->PUPDR = 0x00000000;		// no pull-up, pull-down
}

void EXTI_Config()
{
	RCC->APB2ENR = 0x00004000; //SYSCFG Active (external ýnterrupt)

	// Enable Ports interrupt
	SYSCFG->EXTICR[0] = 0x00000000;
	SYSCFG->EXTICR[1] = 0x00000000;
	SYSCFG->EXTICR[2] = 0x00000000;

	// Enable NVIC Interrupts
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);

	// Setting priority of interrupts
	NVIC_SetPriority(EXTI0_IRQn, 0);	// first priority
	NVIC_SetPriority(EXTI1_IRQn, 1);	// second priority
	NVIC_SetPriority(EXTI2_IRQn, 2);	// third priority

	EXTI->IMR = 0x00000007; 	// PA0-1-2 bits are interrupt without masking
	EXTI->RTSR = 0x00000007;	// PA0-1-2 bits are rising edge

}

void delay(uint32_t time)
{
	while(time--);
}

//External 1 intterupt process
void EXTIO_IRQHandler()
{
	if(EXTI->PR & 0x00000001)		// if interrupt flag is raised for interrupt 0
	{
		GPIOD->ODR = 0x00001000; 	// Light up the led on PD12
		delay(33600000);

		EXTI->PR = 0x00000001;		//flag is set to be cleared the interrupt
	}
}

//External 2 interrupt process
void EXTI1_IRQHandler()
{
	if(EXTI->PR & 0x00000002)		// if interrupt flag is raised for interrupt 1
	{
		GPIOD->ODR = 0x00002000; 	// Light up the led on PD13
		delay(33600000);

		EXTI->PR = 0x00000002; 		// flag is set to be cleared the interrupt
	}
}

//External3 icin intterupt
void EXTI2_IRQHandler()
{
	if(EXTI->PR & 0x00000004)		// if interrupt flag is raised for interrupt 2
	{
		GPIOD->ODR = 0x00004000; 	// Light up the led on PD14
		delay(33600000);

		EXTI->PR = 0x00000004; 		// flag is set to be cleared the interrupt
	}
}

int main(void)
{
  CLK_Config();
  GPIO_Config();
  EXTI_Config();

  while (1)
  {
	  GPIOD->ODR = 0x0000F000; // Light all led if the interrupt does not happen.
  }
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}

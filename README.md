# INTERRUPT with REGISTER
The interrupt is a signal emitted by hardware or software when a process or an event needs immediate attention. It alerts the processor to a high-priority process requiring interruption of the current working process. In I/O devices one of the bus control lines is dedicated for this purpose and is called the Interrupt Service Routine (ISR).

In this project, PA0, PA1, PA2 pins belonging stm32f407 device are assigned as external interrupt. Each pin represents **EXTI0**, **EXTI1**, **EXTI2** respectively and these external interrupt have priority each other according to vector table below. Order of priority is inversely proportional to the magnitude of the priority. Also the push buttons are connected PA1 and PA2 pins. When the push buttons are pushed, The leds on D port ligh up according to priority of external interrupts (PA0, PA1, PA2). 

![priority](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/5690cf68-c240-4b61-ae34-8d4fb6d339dc)

### GPIO Configuration
Fistly, **Port A and D point AHB1 clock** in stm32f407 block diagram. So, AHB1 clock must be enabled for Port A and D by using RCC register.

![ahb1enr](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/6e03c392-707d-49a9-9bdc-a2c5340a5f15)

To be enabled these ports, `RCC->AHB1ENR = 0x00000009` must be writed.

As a default option, the ports are input mode if these are not changed. In our project, **A port is input** because it is connected to buttons. **D port is output** because **PD12, PD13, PD14, PD15** are connected to LED's. So, these pins must be output in **RCC_MODER** register. In hexadecimal system this is adjusted `GPIOD->MODER = 0x55000000` as shown below.

![gpıo_moder](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/14d78739-83fa-42bd-9865-f0cd552bfbb9)

Also, these pins are adjusted **as push-pull**, **high speed**, **no pull-up, pull-down**. These are `GPIOD->OTYPER = 0x00000000`, `GPIOD->OSPEEDR = 0xFF000000`, `GPIOD->PUPDR = 0x00000000` respectively.

### External Interrupt Configuration
The referance manual explains that ***Configure the mask bits of the 23 interrupt lines (EXTI_IMR)***. External interrupt1 (EXTI0), External interrupt2 (EXTI1), External interrupt3 (EXTI2) are not masked to be initialized. Thus, the register is adjusted as `EXTI->IMR = 0x00000007` and these interrupts are adjusted rising edge corresponding `EXTI->RTSR = 0x00000007`. Here is the registers adjusted below.

![extı_ımr](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/83872a43-5215-460b-93fe-09d551db743f)

![extı_rtrs](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/506b45c1-b165-4512-b512-b312849a9eb6)

The other step for setting up external is **interrups GPIO mapping**. The mapping shows that how to set needed interrupt pins matching **SYSCFG** register contains all these pins as shown below.

![event_interrupt_mapping](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/04f0ae2c-0e04-444f-92e9-3977f0857c11)

![syscfg_extıcr](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/d01af9b1-84e9-479c-93fa-b0288991b30b)

Thus, `SYSCFG->EXTICR[0]`, `SYSCFG->EXTICR[1]`, `SYSCFG->EXTICR[2]` make PA0, PA1, PA2 activate for interrupt.

The last step that activates external interrupts is **NVIC_EnableIRQ** having one parameter EXTIX_IRQn and **NVIC_SetPriority** indicating priority. The following NVIC functions used for activating and priority are below

`NVIC_EnableIRQ(EXTI0_IRQn)`, `NVIC_EnableIRQ(EXTI1_IRQn)`, `NVIC_EnableIRQ(EXTI2_IRQn)`

`NVIC_SetPriority(EXTI0_IRQn, 0)`, `NVIC_SetPriority(EXTI1_IRQn, 1)`, `NVIC_SetPriority(EXTI2_IRQn, 2)`

To be used external interrupts in functions, the interrupt flag must be raised. It lets external interrupt function having duty process. After the process, the flag is set which indicates the process is finished. The interrupt flag is created from **EXTI_PR** register. The needed interrupt port is set '1' and checked in condition. In this project PA0, PA1, PA2 pins are responsible of external interrupt so, PR0, PR1, PR2 is set when the interrupt process is needed as shown below.

![pr](https://github.com/yasinsulhan/stm32f4-external-interrupt-with-register/assets/109728194/e5d28dcd-c3b8-471e-b1db-c5fcb2229ffa)

Fox example, the condition can be checked on raised interrupt `if(EXTI->PR & 0x00000001)` for PA0 pin.




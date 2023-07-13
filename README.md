# INTERRUPT with REGISTER
The interrupt is a signal emitted by hardware or software when a process or an event needs immediate attention. It alerts the processor to a high-priority process requiring interruption of the current working process. In I/O devices one of the bus control lines is dedicated for this purpose and is called the Interrupt Service Routine (ISR).

In this project, PA0, PA1, PA2 pins belonging stm32f407 device are assigned as external interrupt. Each pin represents **EXTI0**, **EXTI1**, **EXTI2** respectively and these external interrupt have priority each other according to vector table below. Order of priority is inversely proportional to the magnitude of the priority. Also the push buttons are connected PA1 and PA2 pins. When the push buttons are pushed, The leds on D port ligh up according to priority of external interrupts (PA0, PA1, PA2). 

![priority](https://github.com/yasinsulhan/readme/assets/109728194/f1af3c35-46b9-48cb-b7a0-4987998cba4a)

### GPIO Configuration
Fistly, **Port A and D point AHB1 clock** in stm32f407 block diagram. So, AHB1 clock must be enabled for Port A and D by using RCC register.

![ahb1enr](https://github.com/yasinsulhan/readme/assets/109728194/548bd06c-9219-4853-80bd-b4937181607f)

To be enabled these ports, `RCC->AHB1ENR = 0x00000009` must be writed.

As a default option, the ports are input mode if these are not changed. In our project, **A port is input** because it is connected to buttons. **D port is output** because **PD12, PD13, PD14, PD15** are connected to LED's. So, these pins must be output in **RCC_MODER** register. In hexadecimal system this is adjusted `GPIOD->MODER = 0x55000000` as shown below.

![gpıo_moder](https://github.com/yasinsulhan/readme/assets/109728194/6ef294c3-855f-4c85-87c9-88d78573c098)

Also, these pins are adjusted **as push-pull**, **high speed**, **no pull-up, pull-down**. These are `GPIOD->OTYPER = 0x00000000`, `GPIOD->OSPEEDR = 0xFF000000`, `GPIOD->PUPDR = 0x00000000` respectively.

### External Interrupt Configuration
The referance manual explains that ***Configure the mask bits of the 23 interrupt lines (EXTI_IMR)***. External interrupt1 (EXTI0), External interrupt2 (EXTI1), External interrupt3 (EXTI2) are not masked to be initialized. Thus, the register is adjusted as `EXTI->IMR = 0x00000007` and these interrupts are adjusted rising edge corresponding `EXTI->RTSR = 0x00000007`. Here is the registers adjusted below.

![extı_ımr](https://github.com/yasinsulhan/readme/assets/109728194/327d498e-dd24-4d74-9d2f-993c8b1a4ccb)

![extı_rtrs](https://github.com/yasinsulhan/readme/assets/109728194/d3eeaee4-dce2-4c1b-ae49-6e703a432302)

The other step for setting up external is **interrups GPIO mapping**. The mapping shows that how to set needed interrupt pins matching **SYSCFG** register contains all these pins as shown below.

![event_interrupt_mapping](https://github.com/yasinsulhan/readme/assets/109728194/a91b4f1a-f7a6-4f61-8852-0f490cb650c3)

![syscfg_extıcr](https://github.com/yasinsulhan/readme/assets/109728194/a3734994-424c-4463-9e4e-ace2308839f2)

Thus, `SYSCFG->EXTICR[0]`, `SYSCFG->EXTICR[1]`, `SYSCFG->EXTICR[2]` make PA0, PA1, PA2 activate for interrupt.

The last step that activates external interrupts is **NVIC_EnableIRQ** having one parameter EXTIX_IRQn and **NVIC_SetPriority** indicating priority. The following NVIC functions used for activating and priority are below

`NVIC_EnableIRQ(EXTI0_IRQn)`, `NVIC_EnableIRQ(EXTI1_IRQn)`, `NVIC_EnableIRQ(EXTI2_IRQn)`

`NVIC_SetPriority(EXTI0_IRQn, 0)`, `NVIC_SetPriority(EXTI1_IRQn, 1)`, `NVIC_SetPriority(EXTI2_IRQn, 2)`

To be used external interrupts in functions, the interrupt flag must be raised. It lets external interrupt function having duty process. After the process, the flag is set which indicates the process is finished. The interrupt flag is created from **EXTI_PR** register. The needed interrupt port is set '1' and checked in condition. In this project PA0, PA1, PA2 pins are responsible of external interrupt so, PR0, PR1, PR2 is set when the interrupt process is needed as shown below.

![pr](https://github.com/yasinsulhan/readme/assets/109728194/0a407084-c727-4b2e-a6e5-e4865c8d4118)

Fox example, the condition can be checked on raised interrupt `if(EXTI->PR & 0x00000001)` for PA0 pin.




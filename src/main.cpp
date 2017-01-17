//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include <LPC11xx.h>
#include <system_LPC11xx.h>
#include <RCsemi_typedef.hpp>
#include <motor.hpp>
#include <PFMmotor.hpp>
#include <PWMmotor.hpp>
#include <GPIO_struct.h>
const uint32_t PFM_MASK = (0x1 << 3);
int duty;

int
main (int argc, char* argv[])
{
	SystemCoreClockUpdate();
	LPC_GPIO0->DIR |= PFM_MASK | (0x1 << 2);
	LPC_GPIO0->DATA &= ~(0x1 << 2);
/*
	const RCsemi::gpioMask_t p1(0,3);
	const RCsemi::gpioMask_t p2(0,2);
*/
	//RCsemi::PFMmotor motor0(p1,p2);
	RCsemi::PFMmotor motor0(RCsemi::GPIOMask_t(0,3),RCsemi::GPIOMask_t(0,2));

	RCsemi::TickingEntry::InitTimer();
	duty = 20;
	/*
	p1.port = 0;
	p2.port = 0;
	p1.mask = (0x1 << 3);
	p2.mask = (0x1 << 2);
	*/



	//RCsemi::PFMmotor_globalValues::PFMmotorList[0] = &motor0;

//	RCsemi::TickingEntry::Append(&motor0);
	while(1)
	{
		for(int i=-100;i<100;i++)
		{
			motor0.drive(i);
			for(volatile int j=0;j<10000;j++);
		}
		for(int i=100;i>-100;i--)
		{
			motor0.drive(i);
			for(volatile int j=0;j<10000;j++);
		}
	}
}

extern "C"
{
void TIMER16_0_IRQHandler()
{
	if(LPC_TMR16B0->IR & 0x1)
	{
		LPC_TMR16B0->IR |= 0x1;

		RCsemi::TickingEntry::Tick();

	}
}
/*
void TIMER16_0_IRQHandler()
{
	static uint32_t toggleTimer=0;
	if(LPC_TMR16B0->IR & 0x1)
	{
		LPC_GPIO0->DATA ^= (0x1 << 2);
		LPC_TMR16B0->IR |= 0x1;
		if(toggleTimer) toggleTimer--;

		else
		{
			if(duty)
			{
				if(LPC_GPIO0->DATA & PFM_MASK)
				{
					LPC_GPIO0->DATA &= ~(PFM_MASK);
					toggleTimer = 10000/duty-100;
				}
				else
				{
					toggleTimer = 100;
					LPC_GPIO0->DATA |= PFM_MASK;
				}
			}
			else
			{
				toggleTimer=100;
				LPC_GPIO0->DATA &= ~(PFM_MASK);
			}
		}
	}
}
*/
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

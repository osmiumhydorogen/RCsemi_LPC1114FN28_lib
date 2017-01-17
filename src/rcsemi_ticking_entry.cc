/*
 * motor_list_4_tick.cc
 *
 *  Created on: 2016/10/13
 *      Author: osmium
 */
#include <LPC11xx.h>
#include <rcsemi_ticking_entry.hpp>

namespace RCsemi
{
/*
	Motor::Motor()
	{
		need_ticking_ = false;
	}
*/
	TickingObject::TickingObject()
	{
		TickingEntry::Append(this);
	}

	TickingEntry::TickingEntry(TickingObject* obj)
	{
		object_ = obj;
		next_  = 0;
	}

	void TickingEntry::InitTimer()
	{
		LPC_SYSCON->SYSAHBCLKCTRL |= (0x01 << 7);

		LPC_TMR16B0->MCR |= 0x3;
		LPC_TMR16B0->PR   = 19;
		LPC_TMR16B0->MR0  = 23;

		LPC_TMR16B0->TCR |= 0x2;
		LPC_TMR16B0->TCR &= ~0x2;
		LPC_TMR16B0->TCR |= 0x1;

		NVIC_EnableIRQ(TIMER_16_0_IRQn);
	}
	void TickingEntry::Append(TickingObject* obj)
	{
		if(last_)
		{
			last_->next_ = new TickingEntry(obj);
			last_ = last_->next_;
		}
		else
		{
			last_ = origin_ = new TickingEntry(obj);
		}
	}

	void TickingEntry::Tick()
	{
		TickingEntry* current;
		if(origin_)
		{
			for(current = origin_; current; current = current->next())
			{
				current->object()->_tick();
			}
		}
	}
	TickingEntry* TickingEntry::last_ = 0;
	TickingEntry* TickingEntry::origin_ = 0;
}

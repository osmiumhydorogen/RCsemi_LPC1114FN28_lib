/*
 * PWMmotor.cc
 *
 *  Created on: 2016/10/13
 *      Author: osmium
 */

#include <LPC11xx.h>
#include <PWMmotor.hpp>

namespace RCsemi
{
	PWMmotor::PWMmotor(gpioMask_t p1,gpioMask_t p2)
	{
		pin1_ = p1;
		pin2_ = p2;

		port1 = ((p1.port == 0) ? (LPC_GPIO0) : (LPC_GPIO1));
		port2 = ((p2.port == 0) ? (LPC_GPIO0) : (LPC_GPIO1));

		port1->DIR  |= p1.mask;
		port2->DIR  |= p2.mask;

		port1->DATA &= (p1.mask);
		port2->DATA &= (p2.mask);

		current_duty_ = 0;
		is_brake_ = false;
		motor_tick_count_ = 0;
		update_timer_     = 0;
	}

	int PWMmotor::drive(int Duty)
	{
		is_brake_ = false;
		current_duty_ = Duty;
		return Duty;
	}

	int PWMmotor::brake()
	{
		is_brake_ = true;
		return 0;
	}

	int PWMmotor::getCurrentDuty()
	{
		return current_duty_;
	}

	void PWMmotor::_tick()
	{
		using namespace PWMmotor_conf;
		if(!update_timer_)
		{
			update_timer_ = period_per_cent_tick;
			if(motor_tick_count_ < 100)
			{
				motor_tick_count_++;
				if(is_brake_)
				{
					if((port1->DATA & pin1_.mask) || (port2->DATA & pin2_.mask))
					{
						port1->DATA &= ~(pin1_.mask);
						port2->DATA &= ~(pin2_.mask);
					}
					else
					{
						port1->DATA |= (pin1_.mask);
						port2->DATA |= (pin2_.mask);
					}
				}
				else if(current_duty_ < 0)
				{
					if(motor_tick_count_ > -current_duty_)
					{
						port1->DATA &= ~(pin1_.mask);
						port2->DATA &= ~(pin2_.mask);
					}
				}
				else if(current_duty_ > 0)
				{
					if(motor_tick_count_ > current_duty_)
					{
						port2->DATA &= ~(pin2_.mask);
						port1->DATA &= ~(pin1_.mask);
					}
				}
			}
			else
			{
				motor_tick_count_ = 0;
				if(current_duty_ < 0)
				{
					if(port1->DATA & pin1_.mask)
					{
						update_timer_ = PWMDelayTick;
						port2->DATA &= ~(pin2_.mask);
						port1->DATA &= ~(pin1_.mask);
					}
					else
					{
						port1->DATA &= ~(pin1_.mask);
						port2->DATA |= (pin2_.mask);
					}
				}
				else if(current_duty_ > 0)
				{
					if(port2->DATA & pin2_.mask)
					{
						update_timer_ = PWMDelayTick;
						port2->DATA &= ~(pin2_.mask);
						port1->DATA &= ~(pin1_.mask);
					}
					else
					{
						port2->DATA &= ~(pin2_.mask);
						port1->DATA |= (pin1_.mask);
					}
				}
				else
				{
						port2->DATA &= ~(pin2_.mask);
						port1->DATA &= ~(pin1_.mask);
				}

			}
		}
		else
		{
			update_timer_--;
		}
	}
}

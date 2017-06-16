/*
 * PWMmotor.cc
 *
 *  Created on: 2016/10/13
 *      Author: osmium
 */

#include "../rcsemi_lib/inc/PWMmotor.hpp"

#include <LPC11xx.h>

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
		to_brake_ = false;
		motor_tick_count_ = 0;
		update_timer_     = 0;
	}

	int PWMmotor::drive(int Duty)
	{
		to_brake_ = false;
		current_duty_ = Duty;
		return Duty;
	}

	bool PWMmotor::brake()
	{
		current_duty_ = 0;
		to_brake_ = true;
		return true;
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
			if(motor_tick_count_ != 100)
			{
				motor_tick_count_++;
				if((motor_tick_count_ == current_duty_) || (motor_tick_count_ == -current_duty_))
				{
					port1->DATA &= ~(pin1_.mask);
					port2->DATA &= ~(pin2_.mask);
					is_brake_ = false;
				}
				else if(to_brake_ && !is_brake_)
				{
					if((port1->DATA & pin1_.mask) || (port2->DATA & pin2_.mask))
					{
						update_timer_ = PWMDelayTick;
						port1->DATA &= ~(pin1_.mask);
						port2->DATA &= ~(pin2_.mask);
					}
					else
					{
						port1->DATA |= (pin1_.mask);
						port2->DATA |= (pin2_.mask);
						is_brake_ = true;
					}
				}
			}
			//if(motor_tick_count_ != 100)
			else
			{
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
						motor_tick_count_ = 0;
					}
					is_brake_ = false;
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
						motor_tick_count_ = 0;
					}
					is_brake_ = false;
				}
				else if(!is_brake_)
				{
						port2->DATA &= ~(pin2_.mask);
						port1->DATA &= ~(pin1_.mask);
				}
				else	motor_tick_count_ = 1;
			}
		}
		else
		{
			update_timer_--;
		}
	}
}

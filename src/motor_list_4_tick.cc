/*
 * motor_list_4_tick.cc
 *
 *  Created on: 2016/10/13
 *      Author: osmium
 */

#include <motor.hpp>
namespace RCsemi
{

/*
	MotorList4Tick::MotorList4Tick(Motor* M)
	{
		origin_ = this;
		motor_ = M;
		next_  = 0;
	}

	void MotorList4Tick::Append(Motor* motor)
	{
		if(motor->need_ticking())
		{
			if(last_)
			{
				last_->next_ = new MotorList4Tick(motor);
				last_ = last_->next_;
			}
			else
			{
				last_ = origin_ = new MotorList4Tick(motor);
			}
		}
	}

	void MotorList4Tick::Tick()
	{
		MotorList4Tick* current;
		if(origin_)
		{
			for(current = origin_; current; current = current->next())
			{
				current->motor()->_tick();
			}
		}
	}
	MotorList4Tick* MotorList4Tick::last_ = 0;
	MotorList4Tick* MotorList4Tick::origin_ = 0;
	*/
}

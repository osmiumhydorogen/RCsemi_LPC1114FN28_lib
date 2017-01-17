/*
 * PFMmotor.cpp
 *
 *  Created on: 2016/09/27
 *      Author: osmium
 */

#include <GPIO_struct.h>
#include <PFMmotor.hpp>
#include <RCsemi_typedef.hpp>
#include <../RCsemiCommon/inc/motor.hpp>


namespace RCsemi
{
	namespace PFMmotor_globalValues
	{
		PFMmotor* PFMmotorList[20];
	}

	PFMmotor::PFMmotor(gpioMask_t p1,gpioMask_t p2)
	{
		isBrake = false;
		toggleTimer=0;
		currentDuty=0;
		Pin1=p1;
		Pin2=p2;
		pioPrt1 = ((p1.port==0) ? (LPC_GPIO0) : (LPC_GPIO1));
		pioPrt2 = ((p2.port==0) ? (LPC_GPIO0) : (LPC_GPIO1));
		pioPrt1->DIR  |= Pin1.mask;
		pioPrt2->DIR  |= Pin2.mask;
		pioPrt1->DATA &= ~(Pin1.mask);
		pioPrt2->DATA &= ~(Pin2.mask);
	}

	int PFMmotor::drive(int duty)
	{
		isBrake = false;
		currentDuty = duty;
		return currentDuty;
	}

	int PFMmotor::getCurrentDuty()
	{
		return currentDuty;
	}

	int PFMmotor::brake()
	{
		isBrake = true;
		return 0;
	}

	void PFMmotor::_tick()
	{
		using namespace PFMmotor_conf;
		int tmpDuty;
		if(toggleTimer) toggleTimer--;
		else
		{

			if(isBrake)
			{
				if((pioPrt1->DATA & Pin1.mask) || (pioPrt2->DATA & Pin2.mask))
				{
					pioPrt1->DATA &= ~(Pin1.mask);
					pioPrt2->DATA &= ~(Pin2.mask);
					toggleTimer = PFMDelayTime;
				}
				else
				{
					pioPrt1->DATA |= Pin1.mask;
					pioPrt2->DATA |= Pin2.mask;
				}

			}
			else if(currentDuty < 0)
			{
				tmpDuty = -currentDuty;
				if(pioPrt1->DATA & Pin1.mask)
				{
					pioPrt1->DATA &= ~(Pin1.mask);
					pioPrt2->DATA &= ~(Pin2.mask);
					toggleTimer   = PFMDelayTime;
				}
				else if(pioPrt2->DATA & Pin2.mask)
				{
					pioPrt2->DATA &= ~(Pin2.mask);
					toggleTimer = ontime*100/tmpDuty - ontime;
				}
				else
				{
					pioPrt2->DATA |= Pin2.mask;
					toggleTimer = ontime;
				}
			}
			else if(currentDuty > 0)
			{
				if(pioPrt2->DATA & Pin2.mask)
				{
					pioPrt2->DATA &= ~(Pin2.mask);
					pioPrt1->DATA &= ~(Pin1.mask);
					toggleTimer   = PFMDelayTime;
				}
				else if(pioPrt1->DATA & Pin1.mask)
				{
					pioPrt1->DATA &= ~(Pin1.mask);
					toggleTimer = ontime*100/currentDuty - ontime;
				}
				else
				{
					pioPrt1->DATA |= Pin1.mask;
					toggleTimer = ontime;
				}
			}
			else
			{
				pioPrt1->DATA &= ~(Pin1.mask);
				pioPrt2->DATA &= ~(Pin2.mask);
			}

		}
	}
/*
	namespace PFMmotor_globalValues
	{
		PFMMotorList::PFMMotorList(Motor* M)
		{
			origin_ = this;
			motor_ = M;
			next_  = 0;
		}

		void PFMMotorList::Append(Motor* motor)
		{
			if(last_)
			{
				last_->next_ = new PFMMotorList(motor);
				last_ = last_->next_;
			}
			else
			{
				last_ = origin_ = new PFMMotorList(motor);
			}
		}

		void PFMMotorList::Tick()
		{
			PFMMotorList* current;
			if(origin_)
			{
				for(current = origin_; current; current = current->next())
				{
					current->motor()->_tick();
				}
			}
		}
		PFMMotorList* PFMMotorList::last_ = 0;
		PFMMotorList* PFMMotorList::origin_ = 0;

	}
	*/
}



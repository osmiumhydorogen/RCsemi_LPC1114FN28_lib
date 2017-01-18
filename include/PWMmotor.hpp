/*
 * PWMmotor.hpp
 *
 *  Created on: 2016/10/13
 *      Author: osmium
 */

#ifndef INCLUDE_PWMMOTOR_HPP_
#define INCLUDE_PWMMOTOR_HPP_

#include <../RCsemiCommon/inc/motor.hpp>
#include <RCsemi_typedef.hpp>
#include <rcsemi_ticking_entry.hpp>
#include <GPIO_struct.h>

namespace RCsemi
{
	namespace PWMmotor_conf
	{
		static const tickVal_t period_per_cent_tick = 1;
		static const tickVal_t PWMDelayTick = 200;
	}

	class PWMmotor : public Motor,public TickingObject
	{
	private:
		tickVal_t update_timer_;
		int current_duty_;
		bool is_brake_;
		gpioMask_t pin1_;
		gpioMask_t pin2_;

		tickVal_t motor_tick_count_;
		LPC_GPIO_TypeDef* port1;
		LPC_GPIO_TypeDef* port2;
	public:
		PWMmotor(gpioMask_t p1,gpioMask_t p2);
		int drive(int Duty);
		bool brake();
		int getCurrentDuty();
		void _tick();
	};
}

#endif /* INCLUDE_PWMMOTOR_HPP_ */

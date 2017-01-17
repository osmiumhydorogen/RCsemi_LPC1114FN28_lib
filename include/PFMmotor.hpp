/*
 * PFMmotor.hpp
 *
 *  Created on: 2016/09/27
 *      Author: osmium
 */
#ifndef INCLUDE_PFMMOTOR_HPP_
#define INCLUDE_PFMMOTOR_HPP_

#include <LPC11xx.h>
#include <RCsemi_typedef.hpp>
#include <../RCsemiCommon/inc/motor.hpp>
#include <rcsemi_ticking_entry.hpp>
#include <GPIO_struct.h>
#include <vector>


namespace RCsemi
{
	namespace PFMmotor_conf
	{
		static const tickVal_t offtime_ratio=0;
		static const tickVal_t ontime = 50;
		static const tickVal_t PFMDelayTime = 200;
	}

	class PFMmotor : public Motor,public TickingObject
	{
	private:
		gpioMask_t	Pin1,Pin2;
		LPC_GPIO_TypeDef *pioPrt1,*pioPrt2;

		int currentDuty;
		bool isBrake;

		tickVal_t toggleTimer;
	public:
		PFMmotor(gpioMask_t p1,gpioMask_t p2);
		int drive(int duty);
		int getCurrentDuty();
		int brake();

		void _tick();
	};

	/*
	namespace PFMmotor_globalValues
	{
		//extern PFMmotor* PFMmotorList[20];

		class PFMMotorList
		{
		private:
			static PFMMotorList* origin_;
			PFMMotorList* next_;
			static PFMMotorList* last_;
			Motor*        motor_;
		public:
			PFMMotorList();
			PFMMotorList(Motor* M);


			inline void origin(PFMMotorList* new_origin)
			{
				origin_ = new_origin;
			}

			inline PFMMotorList* origin() const
			{
				return origin_;
			}

			inline PFMMotorList* next() const
			{
				return next_;
			}

			inline Motor* motor()
			{
				return motor_;
			}
			static void Append(Motor* motor);

			static void Tick();
		};
	}
	*/
}
#endif /* INCLUDE_PFMMOTOR_HPP_ */

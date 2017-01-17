/*
 * motor.hpp
 *
 *  Created on: 2016/09/27
 *      Author: osmium
 */

#ifndef INCLUDE_MOTOR_HPP_
#define INCLUDE_MOTOR_HPP_

namespace RCsemi
{
	class Motor
	{
	public:
		virtual int drive(int Duty)  = 0;
		virtual int brake()          = 0;
		virtual int getCurrentDuty() = 0;
		//virtual int _tick()          = 0;
		virtual ~Motor()             {};
	};
/*
	class MotorList4Tick
	{
	private:
		static MotorList4Tick* origin_;
		MotorList4Tick* next_;
		static MotorList4Tick* last_;
		Motor*        motor_;
	public:
		MotorList4Tick();
		MotorList4Tick(Motor* M);


		inline void origin(MotorList4Tick* new_origin)
		{
			origin_ = new_origin;
		}

		inline MotorList4Tick* origin() const
		{
			return origin_;
		}

		inline MotorList4Tick* next() const
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
*/
}







#endif /* INCLUDE_MOTOR_HPP_ */

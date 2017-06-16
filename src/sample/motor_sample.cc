//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//
//modified by osmiumhydorogen
// ----------------------------------------------------------------------------

#define BUILD_4_SAMPLE
#ifdef BUILD_4_SAMPLE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "sample_conf.hpp"

#ifdef   MOTOR_SAMPLE

#define PWM_MOTOR
//#define PFM_MOTOR

#include <LPC11xx.h>
#include <system_LPC11xx.h>
#include "../../rcsemi_lib/inc/RCsemi_typedef.hpp"
#include <../RCsemiCommon/inc/motor.hpp>
#include "../../rcsemi_lib/inc/PFMmotor.hpp"
#include "../../rcsemi_lib/inc/PWMmotor.hpp"
#include "../../rcsemi_lib/inc/GPIO_struct.h"
const uint32_t MOTOR_MASK1 = (0x1 << 3);
const uint32_t MOTOR_MASK2 = (0x1 << 2);

const uint32_t MOTOR_MASK3 = (0x1 << 4);
const uint32_t MOTOR_MASK4 = (0x1 << 5);
int duty;


int
main (int argc, char* argv[])
{
	//初期化前にやっておきたい奴
	SystemCoreClockUpdate();

	//モーターの出力ピンの設定
	LPC_GPIO0->DIR |= MOTOR_MASK1 | MOTOR_MASK2;
	LPC_GPIO0->DIR |= MOTOR_MASK3 | MOTOR_MASK4;
	LPC_GPIO0->DATA &= ~(MOTOR_MASK1 | MOTOR_MASK2);
	LPC_GPIO0->DATA &= ~(MOTOR_MASK3 | MOTOR_MASK4);
	//モーターを宣言
	RCsemi::Motor*   motor0;
	RCsemi::Motor*   motor1;
	RCsemi::Motor*   motor2;

#ifdef PWM_MOTOR
	//A出力をport0 pin 3,B出力を同pin2とするPWMのモーターを宣言
	motor0 = new RCsemi::PWMmotor(RCsemi::GPIOMask_t(0,3),RCsemi::GPIOMask_t(0,2));
	motor1 = new RCsemi::PWMmotor(RCsemi::GPIOMask_t(0,4),RCsemi::GPIOMask_t(0,5));
	motor2 = new RCsemi::PWMmotor(RCsemi::GPIOMask_t(1,0),RCsemi::GPIOMask_t(1,1));
#endif

#ifdef PFM_MOTOR
	//A出力をport0 pin 3,B出力を同pin2とするPFMのモーターを宣言
	motor0 = new RCsemi::PFMmotor(RCsemi::GPIOMask_t(0,3),RCsemi::GPIOMask_t(0,2));
#endif

	//タイマーを初期化
	RCsemi::TickingEntry::InitTimer();

	while(1)
	{
		for(int i=-100;i<100;i++)
		{
			motor0->drive(i);
			motor1->drive(i);
			motor2->drive(i);
			for(volatile int j=0;j<100;j++);//簡易的な待ち
		}
		for(int i=100;i>-100;i--)
		{
			motor0->drive(i);
			motor1->drive(i);
			motor2->drive(i);
			for(volatile int j=0;j<100;j++);//簡易的な待ち
		}
		for(int i=100;i>-100;i--)
		{
			motor0->brake();
			motor1->brake();
			motor2->brake();
			for(volatile int j=0;j<100;j++);//簡易的な待ち
		}
	}
}

#endif
#pragma GCC diagnostic pop
#endif

// ----------------------------------------------------------------------------

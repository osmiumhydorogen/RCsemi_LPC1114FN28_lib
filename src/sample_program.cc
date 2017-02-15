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

#include <LPC11xx.h>
#include <system_LPC11xx.h>
#include <RCsemi_typedef.hpp>
#include <../RCsemiCommon/inc/motor.hpp>
#include <PFMmotor.hpp>
#include <PWMmotor.hpp>
#include <GPIO_struct.h>
const uint32_t MOTOR_MASK1 = (0x1 << 3);
const uint32_t MOTOR_MASK2 = (0x1 << 2);
int duty;

//#define BUILD_4_SAMPLE
#define PFM_MOTOR_SAMPLE
//#define PWM_MOTOR_SAMPLE
//#define PSPAD_SAMPLE
//#define UART_PORT_SAMPLE

int
main (int argc, char* argv[])
{
	//初期化前にやっておきたい奴
	SystemCoreClockUpdate();

#ifdef PFM_MOTOR_SAMPLE
	//モーターの出力ピンの設定
	LPC_GPIO0->DIR |= MOTOR_MASK1 | MOTOR_MASK2;
	LPC_GPIO0->DATA &= ~(MOTOR_MASK1 | MOTOR_MASK2);

	//A出力をport0 pin 3,B出力を同pin2とするPFMのモーターを宣言
	RCsemi::Motor*   motor0;
	motor0 = new RCsemi::PFMmotor(RCsemi::GPIOMask_t(0,3),RCsemi::GPIOMask_t(0,2));

	//タイマーを初期化
	RCsemi::TickingEntry::InitTimer();

	while(1)
	{
		for(int i=-100;i<100;i++)
		{
			motor0->drive(i);
			for(volatile int j=0;j<10000;j++);//簡易的な待ち
		}
		for(int i=100;i>-100;i--)
		{
			motor0->drive(i);
			for(volatile int j=0;j<10000;j++);//簡易的な待ち
		}
	}
#endif

#ifdef PWM_MOTOR_SAMPLE
	//モーターの出力ピンの設定
	LPC_GPIO0->DIR |= MOTOR_MASK1 | MOTOR_MASK2;
	LPC_GPIO0->DATA &= ~(MOTOR_MASK1 | MOTOR_MASK2);

	//A出力をport0 pin 3,B出力を同pin2とするPFMのモーターを宣言
	RCsemi::Motor*   motor0;
	motor0 = new RCsemi::PWMmotor(RCsemi::GPIOMask_t(0,3),RCsemi::GPIOMask_t(0,2));

	//タイマーを初期化
	RCsemi::TickingEntry::InitTimer();

	while(1)
	{
		for(int i=-100;i<100;i++)
		{
			motor0->drive(i);
			for(volatile int j=0;j<10000;j++);//簡易的な待ち
		}
		for(int i=100;i>-100;i--)
		{
			motor0->drive(i);
			for(volatile int j=0;j<10000;j++);//簡易的な待ち
		}
	}
#endif
}

#pragma GCC diagnostic pop
#endif

// ----------------------------------------------------------------------------

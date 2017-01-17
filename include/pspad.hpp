/*
 * pspad.hpp
 *
 *  Created on: 2016/11/11
 *      Author: osmium
 */

#ifndef INCLUDE_PSPAD_HPP_
#define INCLUDE_PSPAD_HPP_

namespace RCsemi
{

typedef union
{
	uint8_t bytes[13];

	struct
	{
        uint8_t node       : 8; //送信モジュールのノード番号

        unsigned  id       : 16; //送信モジュールの固有ID

        uint8_t rssi       : 8; //受信強度


		uint8_t Select     : 1;
        uint8_t LStick     : 1;
        uint8_t RStick     : 1;
        uint8_t Start      : 1;
        uint8_t UpArrow    : 1;
        uint8_t RightArrow : 1;
        uint8_t DownArrow  : 1;
        uint8_t LeftArrow  : 1;

        uint8_t L1         : 1;
        uint8_t R1         : 1;
        uint8_t L2         : 1;
        uint8_t R2         : 1;
        uint8_t Triangle   : 1;
        uint8_t Circle     : 1;
        uint8_t Cross      : 1;
        uint8_t Square     : 1;

        uint8_t RStick_X   : 8;

        uint8_t RStick_Y   : 8;

        uint8_t LStick_X   : 8;

        uint8_t LStick_Y   : 8;

        uint8_t chkSum     : 8;

        uint8_t is_error    : 8;

	}name;
}psdata_format_t;


class PspadData
{
private:
	typedef enum
	{
		Range_plus_minus_2,
		Range_plus_minus_4,
		Range_plus_minus_8,
		Range_plus_minus_16,
	}AnalogpadRange_t;



	int* analog_stick_convert_table_;

	int (*convertStickValue)(uint8_t raw_stick);

	int convertStickValue_4Bits(uint8_t raw_stick)
	{
		return analog_stick_convert_table_[raw_stick >> 4];
	}
	int convertStickValue_8Bits(uint8_t raw_stick)
	{
		return analog_stick_convert_table_[raw_stick];
	}
public:
	psdata_format_t data;

	PspadData(AnalogpadRange_t range);

	inline int RStick_X   ()
	{
		return convertStickValue(data.name.LStick_Y);
	}

	inline int RStick_Y   ()
	{
		return convertStickValue(data.name.LStick_Y);
	}

	inline int LStick_X   ()
	{
		return convertStickValue(data.name.LStick_Y);
	}

	inline int LStick_Y   ()
	{
		return convertStickValue(data.name.LStick_Y);
	}


	/*データがエラーかどうか*/
	inline uint8_t IsError()
	{
		return data.name.is_error;
	}

	/*Timeout時の処理.オーバーライドしなければ値の初期化*/
	virtual void TimeoutHandler();


	/*初期値に戻す*/
	inline void InitValue()
	{
		for(int i=0;i<12;i++)
		{
			data.bytes[i] = 0;
		}

		data.name.LStick_X = 128;
		data.name.LStick_Y = 128;
		data.name.RStick_X = 128;
		data.name.RStick_Y = 128;
	}

	virtual ~PspadData();

	/*以下ボタン類のゲッター*/
	inline uint8_t B_Select     ()
	{
		return data.name.Select;
	}
	inline uint8_t B_LStick     ()
	{
		return data.name.LStick;
	}
	inline uint8_t B_RStick     ()
	{
		return data.name.Select;
	}
	inline uint8_t B_Start      ()
	{
		return data.name.Start;
	}
	inline uint8_t B_UpArrow    ()
	{
		return data.name.UpArrow;
	}
	inline uint8_t B_RightArrow ()
	{
		return data.name.RightArrow;
	}
	inline uint8_t B_DownArrow  ()
	{
		return data.name.DownArrow;
	}
	inline uint8_t B_LeftArrow  ()
	{
		return data.name.LeftArrow;
	}
	inline uint8_t B_L1         ()
	{
		return data.name.L1;
	}
	inline uint8_t B_R1         ()
	{
		return data.name.R1;
	}
	inline uint8_t B_L2         ()
	{
		return data.name.L2;
	}
	inline uint8_t B_R2         ()
	{
		return data.name.R2;
	}
	inline uint8_t B_Triangle   ()
	{
		return data.name.Triangle;
	}
	inline uint8_t B_Circle     ()
	{
		return data.name.Circle;
	}
	inline uint8_t B_Cross      ()
	{
		return data.name.Cross;
	}
	inline uint8_t B_Square     ()
	{
		return data.name.Square;
	}

};

}

#endif /* INCLUDE_PSPAD_HPP_ */

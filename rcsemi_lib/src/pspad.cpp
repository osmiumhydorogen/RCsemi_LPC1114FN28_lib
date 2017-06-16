/*
 * pspad.cpp
 *
 *  Created on: 2016/12/09
 *      Author: osmium
 */
#include <pspad.hpp>
namespace RCsemi
{

	PspadData::PspadData(AnalogpadRange_t range)
	{
		int i;
		float stick_maxvalue;
		switch(range)
		{
		case    Range_plus_minus_2:
			analog_stick_convert_table_ = new int[16];
			stick_maxvalue = 2;
			for(i=0;i<8;i++)
			{
				analog_stick_convert_table_[8+i] = (int)((stick_maxvalue+1) * (i/16) * (i/16));
				analog_stick_convert_table_[7-i] = (int)((stick_maxvalue+1) * (i/16) * (i/16));
			}
			convertStickValue = convertStickValue_4Bits;
			break;
		case 	Range_plus_minus_4:
			analog_stick_convert_table_ = new int[16];
			stick_maxvalue = 4;
			for(i=0;i<8;i++)
			{
				analog_stick_convert_table_[8+i] = (int)((stick_maxvalue+1) * (i/16) * (i/16));
				analog_stick_convert_table_[7-i] = (int)((stick_maxvalue+1) * (i/16) * (i/16));
			}
			convertStickValue = convertStickValue_4Bits;
			break;
		case 	Range_plus_minus_8:
			analog_stick_convert_table_ = new int[256];
			stick_maxvalue = 4;
			for(i=0;i<128;i++)
			{
				analog_stick_convert_table_[128+i] = (int)((stick_maxvalue+1) * (i/128) * (i/128));
				analog_stick_convert_table_[127-i] = (int)((stick_maxvalue+1) * (i/128) * (i/128));
			}
			convertStickValue = convertStickValue_8Bits;

			break;
		case 	Range_plus_minus_16:
			analog_stick_convert_table_ = new int[256];
			analog_stick_convert_table_ = new int[256];
			stick_maxvalue = 16;
			for(i=0;i<128;i++)
			{
				analog_stick_convert_table_[128+i] = (int)((stick_maxvalue+1) * (i/128) * (i/128));
				analog_stick_convert_table_[127-i] = (int)((stick_maxvalue+1) * (i/128) * (i/128));
			}
			convertStickValue = convertStickValue_8Bits;
			break;
		}

		InitValue();
	}

}

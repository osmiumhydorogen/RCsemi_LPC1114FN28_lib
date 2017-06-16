/*
 * im315xx_lpc1114.cc
 *
 *  Created on: 2017/02/14
 *      Author: osmium
 */
#include "../rcsemi_lib/inc/im315xx_lpc1114.hpp"
namespace
{
void str2Uint8Arr(const char* input,uint8_t* output,const int length_of_input)
{
	int i=0;
	RCsemi::Uint8AsciiConverter& u8_ascii_converter = *RCsemi::Uint8AsciiConverter::getInstance();
	uint8_t* p_output = output;
	const char*    p_input  = input;
	while(i<length_of_input)
	{
		if(*p_input == ':' || *p_input == ',')
		{
			++p_input;
			++i;
		}
		u8_ascii_converter(p_input,p_output++);
		p_input += 2;
		i+=2;
	}
}
char* Uint8Arr2str_withComma(const uint8_t* input,char* output,int num_of_data)
{
	RCsemi::Uint8AsciiConverter& u8_ascii_converter = *RCsemi::Uint8AsciiConverter::getInstance();
	int i=0;
	char*    p_output = output;

	while(i < num_of_data)
	{
		u8_ascii_converter(input[i++],p_output);
		p_output += 2;
		*p_output = ',';
		++p_output;
	}
	return output;
}
}

namespace RCsemi
{
IM315xx::IM315xx() : u8_ascii_conv(*Uint8AsciiConverter::getInstance()),
					 port_(*UARTPort::getObject(UARTPort::UART0))
{
	timer_ = SystemTimer::getObject();
	port_.setTimeout(IM315xx_conf::timeout_ms * 100);
	port_.SetBaud(48000000,115200);
}

IM315xx* IM315xx::getObject()
{
	if(!object_) object_ = new IM315xx();

	return object_;
}

void IM315xx::destroyObject()
{
	delete object_;
}

int IM315xx::recievePacket(IM315xxPacket_t* packet)
{
	char raw_string[64];
	int length_of_raw_string;
	length_of_raw_string = port_.RecieveLine((void*)raw_string,36);
	if(raw_string[10] != ':' || length_of_raw_string > 34) return 3;
	else if(length_of_raw_string < 34) return 2;
	str2Uint8Arr(raw_string,packet->byte,34);
	packet->by_name.id = (packet->byte[1] << 8) | packet->byte[2];
	return 0;
}

int IM315xx::sendPacket(const IM315xxPacket_t& packet)
{
	char dummy[32];
	const char *str_txdt = "TXDT ",*str_cr_lf = "\r\n";
	while(port_.Send((void*)str_txdt,5));
	while(port_.Send((void*)Uint8Arr2str_withComma(packet.by_name.data,dummy,8),23));
	while(port_.Send((void*)str_cr_lf,2));
	return 0;
}

IM315xx_device* GetIM315xx(int port_num)
{
	switch(port_num)
	{
	case 0:
		return IM315xx::getObject();
	default:
		return 0;
	}
}

IM315xx* IM315xx::object_ = 0;
}

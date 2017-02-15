/*
 * im315xx_lpc1114.cc
 *
 *  Created on: 2017/02/14
 *      Author: osmium
 */
#include <im315xx_lpc1114.hpp>

namespace RCsemi
{
IM315xx::IM315xx() : u8_ascii_conv(&Uint8AsciiConverter::getInstance()),
					 port(UARTPort::UART0)
{
	object_ = *this;
	timer_ = SystemTimer::getObject();
}

IM315xx* IM315xx::getObject()
{
	return object_;
}

int IM315xx::recievePacket(IM315xxPacket_t packet)
{

}

IM315xx_device* GetIM315xx()
{
	return IM315xx::getObject();
}
}

/*
 * im315xx_lpc1114.hpp
 *
 *  Created on: 2017/02/14
 *      Author: osmium
 */

#ifndef INCLUDE_IM315XX_LPC1114_HPP_
#define INCLUDE_IM315XX_LPC1114_HPP_

#include <im315xx.hpp>
#include <rcsemi_system_timer.hpp>
#include <uint8_ascii_converter.hpp>

namespace RCsemi
{

	namespace IM315xx_conf
	{
		const int timeout_ms = 2;
	}

class IM315xx : public IM315xx_device
{
private:
	IM315xx();
	Uint8AsciiConverter& u8_ascii_conv;
	UARTPort port;
	IM315xx* object_;
	SystemTimer* timer_;
public:
	int sendPacket(const IM315xxPacket_t& packet);
	int recievePacket(IM315xxPacket_t* packet);
	static IM315xx* getObject();
};
}

#endif /* INCLUDE_IM315XX_LPC1114_HPP_ */

/*
 * GPIO_struct.cpp
 *
 *  Created on: 2016/10/13
 *      Author: osmium
 */

#include <GPIO_struct.h>

#ifdef __cplusplus
namespace RCsemi
{
	GPIOMask_t::GPIOMask_t()
	{
		this->port = 3;
		this->mask = 0x8000;
	}
	GPIOMask_t::GPIOMask_t(uint32_t port,uint32_t pin)
	{
		this->port = port;
		this->mask = (0x1 << pin);
	}
}
#endif

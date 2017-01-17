/*
 * GPIO_struct.hpp
 *
 *  Created on: 2016/09/27
 *      Author: osmium
 */

#ifndef INCLUDE_GPIO_STRUCT_HPP_
#define INCLUDE_GPIO_STRUCT_HPP_
#include <LPC11xx.h>

#ifdef __cplusplus
namespace RCsemi
{
#endif

typedef struct GPIOMask_t
{
	uint32_t port;
	uint32_t mask;
#ifdef __cplusplus
	GPIOMask_t();
	GPIOMask_t(uint32_t port,uint32_t pin);
} gpioMask_t;
#endif

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_GPIO_STRUCT_HPP_ */

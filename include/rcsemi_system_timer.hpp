/*
 * rcsemi_system_timer.hpp
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */

#ifndef INCLUDE_RCSEMI_SYSTEM_TIMER_HPP_
#define INCLUDE_RCSEMI_SYSTEM_TIMER_HPP_

#include <LPC11xx.h>
#include <rcsemi_ticking_entry.hpp>
namespace RCsemi
{

class SystemTimer : public TickingObject
{
private:
	SystemTimer();
	static unsigned int counter_;
	static SystemTimer* object_;
public:
	static SystemTimer* getObject();

	static inline int counter()
	{
		return counter_;
	}

	void _tick();
};

}

#endif /* INCLUDE_RCSEMI_SYSTEM_TIMER_HPP_ */

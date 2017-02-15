/*
 * rcsemi_system_timer.cc
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */

#include <rcsemi_system_timer.hpp>
namespace RCsemi
{

SystemTimer::SystemTimer()
{
}

void SystemTimer::_tick()
{
	++counter_;
}

SystemTimer* SystemTimer::getObject()
{
	if(!object_)
	{
		object_ = new SystemTimer;
	}
	return object_;
}
unsigned int SystemTimer::counter_ = 0;
}

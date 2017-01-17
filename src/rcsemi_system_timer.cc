/*
 * rcsemi_system_timer.cc
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */

#include <rcsemi_system_timer.hpp>
namespace RCsemi
{

void SystemTimer::_tick()
{
	++counter_;
}

void SystemTimer::initialize()
{
	if(!object_)
	{
		object_ = new SystemTimer;
	}
}

}

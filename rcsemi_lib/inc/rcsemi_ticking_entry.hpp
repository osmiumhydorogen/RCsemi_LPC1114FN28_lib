/*
 * rcsemi_ticking_entry.hpp
 *
 *  Created on: 2016/09/27
 *      Author: osmium
 */

#ifndef INCLUDE_RCSEMI_TICKING_ENTRY_HPP_
#define INCLUDE_RCSEMI_TICKING_ENTRY_HPP_

namespace RCsemi
{
/*
	class Motor
	{
	protected:
		bool need_ticking_;
	public:
		Motor();

		inline bool need_ticking()
		{
			return need_ticking_;
		}

		virtual int drive(int Duty)  = 0;
		virtual int brake()          = 0;
		virtual int getCurrentDuty() = 0;
		virtual int _tick()          = 0;
		virtual ~Motor()             {};
	};
*/
	class TickingObject
	{
	public:
		TickingObject();
		virtual void _tick() = 0;
		virtual ~TickingObject(){};
	};

	class TickingEntry
	{
	private:
		static TickingEntry* origin_;
		TickingEntry* next_;
		static TickingEntry* last_;
		TickingObject*    object_;
	public:
		TickingEntry();
		TickingEntry(TickingObject* M);

		static void InitTimer();

		inline void origin(TickingEntry* new_origin)
		{
			origin_ = new_origin;
		}

		inline TickingEntry* origin() const
		{
			return origin_;
		}

		inline TickingEntry* next() const
		{
			return next_;
		}

		inline TickingObject* object()
		{
			return object_;
		}
		static void Append(TickingObject* obj);

		static void Tick();
	};

}







#endif /*INCLUDE_RCSEMI_TICKING_ENTRY_HPP_*/

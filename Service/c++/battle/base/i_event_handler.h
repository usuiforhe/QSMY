#ifndef __battle__i_event_handler__
#define __battle__i_event_handler__

#include "battle_prefix.h"
#include "base_event_args.h"
namespace battle{
	template<class T>
	class i_event_handler{
	public:
		virtual ~i_event_handler(){};

		virtual void handle_event(T sender, base_event_args & args) = 0;
    };
}

#endif // __battle__i_event_handler__

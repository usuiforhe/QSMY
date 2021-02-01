#ifndef __battle__event__
#define __battle__event__

#include "battle_prefix.h"
#include "i_event_handler.h"
namespace battle{
	template<class T>
	class event{
	public:
		event(){}
		virtual ~event(){
        
        }

		virtual void bind(i_event_handler<T>* handler ){
			if(std::find(handlers_.begin(), handlers_.end(), handler) == handlers_.end()){
				handlers_.push_back(handler);
			}
		}

		virtual void unbind(i_event_handler<T>* handler){
			BOOST_AUTO(pos,std::find(handlers_.begin(), handlers_.end(), handler));
			if(pos != handlers_.end()){
				handlers_.erase(pos);
			}			
		}

		virtual void operator()(T sender, base_event_args & args){
			for(BOOST_AUTO(pos, handlers_.begin()); pos != handlers_.end(); ++pos){
				i_event_handler<T> * handler = *pos;
				if(handler){
					handler->handle_event(sender, args);
				}
			}
		}
	protected:
		std::vector<i_event_handler<T>* > handlers_;
	};
}

#endif // __battle__event__

#ifndef __battle__report__
#define __battle__report__

#include "battle_prefix.h"
#include "base/base_behavior.h"
#include "base/base_behavior_result.h"
#include "net/protobuf_codec.h"

namespace battle{
    class force;
    class scene;
    class unit;
	class report{
        friend class client_report_builder;
	public:
		explicit report(battle::scene * scene):
            scene_(scene),
            grade_(0),
            winner_(NULL),
            first_dier_(NULL){
            
        }
		virtual ~report();

		virtual void push_behavior(base_behavior * behavior);
        
		virtual void add_behavior_result(base_behavior_result * behavior_result);

        virtual void pop_behavior();
        
        virtual boost::shared_ptr<Json::Value> to_json();
        
        virtual boost::shared_ptr<google::protobuf::Message> to_message();
        
        inline SETTER(uint8_t, grade);
        
        virtual void unit_die(unit * dier);
        
        inline GETTER(unit *, first_dier);
        
        inline SETTER(force *, winner);
        inline GETTER(force *, winner);
        
        virtual bool is_reversal();
        virtual bool is_nose_out();

	protected:
		std::list<base_behavior*> behaviors_;
		std::stack<base_behavior*> behavior_stack_;
        
        scene * scene_;
        uint8_t grade_;
        force * winner_;
        unit * first_dier_;
        
        void append_force(Json::Value & json, battle::force * force);
	};
    
    class report_hp : public report
    {
    public:
		explicit report_hp(battle::scene * scene)
        : report(scene)
        {
            
        }
        
		virtual ~report_hp()
        {
            
        }
        
        virtual boost::shared_ptr<google::protobuf::Message> to_message();
        
    protected:
        
    };
}

#endif // __battle__report__

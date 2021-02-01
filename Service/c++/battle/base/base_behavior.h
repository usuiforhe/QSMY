#ifndef __battle__base_behavior__
#define __battle__base_behavior__

#include "battle_prefix.h"
#include "base_behavior_result.h"

namespace battle{
	class base_behavior{
	protected:
		uint16_t id_;
        base_unit * actor_;
        std::vector<base_behavior_result*> results_;
        
        virtual void append_results(Json::Value & root){
            Json::Value results(Json::arrayValue);
            BOOST_FOREACH(base_behavior_result* result, results_){
                results.append(result->to_json());
            }
            
            root["results"] = results;
        }
        
        virtual void append_results(std::vector<char> & bin){
            //结果数量
            bin.push_back(static_cast<char>(results_.size() & 0x000000ff));
            //结果列表
            BOOST_FOREACH(base_behavior_result* result, results_){
                result->append_to_bin(bin);
            }
        }
        
	public:
		base_behavior(base_unit * actor):actor_(actor){

		}

		virtual ~base_behavior(){
            for(BOOST_AUTO(pos, results_.begin()); pos != results_.end(); ++pos){
                delete *pos;
            }
		}
        
        inline SETTER(uint16_t, id);
        
        inline GETTER(uint16_t, id);
        
        inline GETTER(base_unit*, actor);
        
        inline void add_result(base_behavior_result * result){
            results_.push_back(result);
        }
        
        virtual Json::Value to_json() = 0;
        
        virtual void append_to_bin(std::vector<char> & bin) = 0;
	};
}

#endif // __battle__base_behavior__

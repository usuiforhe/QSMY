#include "attack_behavior.h"
using namespace battle;

Json::Value attack_behavior::to_json(){
    Json::Value json;
    json["type"] = BEHAVIOR_ATTACK;
    json["actor"] = actor_->get_id();
    
    append_results(json);
    
    return json;
}

void attack_behavior::append_to_bin(std::vector<char> &bin){
    bin.push_back(static_cast<char>(BEHAVIOR_ATTACK));
    bin.push_back(static_cast<char>(actor_->get_id()));
    
    append_results(bin);
}

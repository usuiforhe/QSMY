#include "report.h"
#include "util.h"
#include "scene.h"
#include "log/log.h"
#include "util.h"
#include "force/force.h"
#include "unit/unit.h"
#include "builder/client_report_builder.h"
#include "meta/report_meta_data.pb.h"

using namespace battle;

report::~report(){
    for(BOOST_AUTO(pos, behaviors_.begin()); pos != behaviors_.end(); ++pos){
        delete *pos;
    }
}

void report::push_behavior(base_behavior * behavior){
    behavior->set_id(behaviors_.size() + 1);
	behaviors_.push_back(behavior);
    behavior_stack_.push(behavior);
}

void report::add_behavior_result(base_behavior_result * behavior_result){
    base_behavior * behavior = behavior_stack_.top();
    behavior->add_result(behavior_result);
}

void report::pop_behavior(){
    base_behavior * behavior = behavior_stack_.top();
    behavior_stack_.pop();
    Json::Value json = behavior->to_json();

#if DEBUG
    std::stringstream & ss = latte::fast_stringstream::ss();
    
    Json::FastWriter writer;
    ss << "behavior end:" << writer.write(json);
    s_log.out_debug(ss.str());
    
#endif
}

boost::shared_ptr<Json::Value> report::to_json(){
    boost::shared_ptr<Json::Value> root(new Json::Value);
    (*root)["scene"] = scene_->get_sid();
    (*root)["map"] = scene_->get_map_id();
    (*root)["grade"] = grade_;

    force * left = scene_->get_left_force();
    force * right = scene_->get_right_force();
    if(left->get_is_winner()){
        (*root)["winner"] = left->get_id();
    }
    else{
        (*root)["winner"] = right->get_id();
    }
    
    Json::Value forces(Json::arrayValue);
    append_force(forces, left);
    append_force(forces, right);
    (*root)["forces"] = forces;
    
    Json::Value actions(Json::arrayValue);
    
    BOOST_FOREACH(base_behavior * val, behaviors_){
        actions.append(val->to_json());
    }
    
    (*root)["actions"] = actions;
    return root;
}

void report::append_force(Json::Value & json, battle::force * force){
    Json::Value force_json;
    force_json["id"] = force->get_id();
    force_json["name"] = force->get_name();
    force_json["speed"] = force->get_total_speed();
    battle::user * user = force->get_users().front();
    
    force_json["user_id"] = user->get_user_id();
    force_json["is_mob"] = user->get_is_mob();
    force_json["level"] = user->get_level();
    
    Json::Value units(Json::arrayValue);
    
    for(BOOST_AUTO(pos, force->get_unit_map().begin()); pos != force->get_unit_map().end(); ++pos){
        Json::Value unit;
        unit["id"] = pos->second->get_id();
        unit["sid"] = pos->second->get_sid();
        unit["position"] = pos->second->get_position()->get_id();
        unit["size"] = pos->second->size;
        unit["level"] = pos->second->level;
        unit["hp"] = pos->second->init_hp;
        unit["max_hp"] = pos->second->max_hp;
        unit["fight_factor"] = static_cast<uint32_t>(pos->second->get_fight_factor() * 10000);
        unit["is_boss"] = pos->second->is_boss;
        unit["dress_id"] = pos->second->dress_id;
        unit["is_show"] = pos->second->is_show;
        
        units.append(unit);
    }
    force_json["units"] = units;
    json.append(force_json);
}

boost::shared_ptr<google::protobuf::Message> report::to_message(){
    boost::shared_ptr<report_meta_data> data(new report_meta_data);
    force * force = scene_->get_left_force();
    user * user = force->get_users().front();
    data->set_scene(scene_->get_sid());
    data->set_user_id(user->get_user_id());
    data->set_win(force->get_is_winner());
    data->set_grade(static_cast<uint32_t>(grade_));
    data->set_fight_percent((float)(force->get_fight() / force->get_max_fight()));
    data->set_content(S_CLIENT_REPORT_BUILDER.build(scene_, this));

    return data;
}

void report::unit_die(unit * dier)
{
    if(first_dier_ == NULL)
    {
        first_dier_ = dier;
    }
}

bool report::is_reversal()
{
    if(first_dier_ == NULL) return false;

    return winner_ == first_dier_->get_force();
}

bool report::is_nose_out()
{
    return winner_->get_alive_num() == 1;
}

boost::shared_ptr<google::protobuf::Message> report_hp::to_message(){
    boost::shared_ptr<hp_report_meta_data> data(new hp_report_meta_data);
    battle::force * force = scene_->get_left_force();
    user * user = force->get_users().front();
    data->set_scene(scene_->get_sid());
    data->set_user_id(user->get_user_id());
    data->set_win(force->get_is_winner());
    data->set_grade(static_cast<uint32_t>(grade_));
    data->set_fight_percent((float)(force->get_fight() / force->get_max_fight()));
    
    battle::force * winner = force->get_is_winner() ? force: scene_->get_right_force();
    
    BOOST_FOREACH(battle::force::unit_map_t::value_type val, winner->get_unit_map()){
        hp_unit_meta_data * hp_unit = data->add_winner_units();
        hp_unit->set_position(val.second->get_position_id());
        hp_unit->set_hp(val.second->hp);
    }
    
    data->set_content(S_CLIENT_REPORT_BUILDER.build(scene_, this));
    
    return data;
}

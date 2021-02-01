#include "scene.h"
#include "position.h"
#include "log/log.h"
#include "util.h"
#include "util/script_util.h"

using namespace battle;

scene::scene(uint32_t sid, scene_type type):
    sid_(sid),
    type_(type),
    map_id_(0),
    is_end_(false),
    current_unit_id_(0),
    left_force_(NULL),
    right_force_(NULL),
    scheduler_(this),
    end_checker_(NULL),
    report_(NULL)
{
    lua_state_ = lua_open();
    luabind::open(lua_state_);
    luaL_openlibs(lua_state_);
    luabind::set_pcall_callback(&battle::lua_call_error);
}

scene::~scene(){    
    if(left_force_ != NULL){
        delete left_force_;
    }
    if(right_force_ != NULL){
        delete right_force_;
    }
    
    if (lua_state_ != NULL)
    {
        lua_close(lua_state_);
    }
    
    delete report_;
}

void scene::add_unit(battle::unit *unit){
    if(unit_map_.count(unit->get_id()) == 0){        
        unit_map_.insert(std::make_pair(unit->get_id(), unit));
    }
}

unit * scene::get_unit(uint32_t id){
    BOOST_AUTO(pos, unit_map_.find(id));
    if(pos != unit_map_.end()){
        return pos->second;
    }
    else{
        return NULL;
    }
}

void scene::execute(){
    scheduler_.init();
    left_force_->init();    
    right_force_->init();
    scheduler_.execute();
}

void scene::unit_die(unit * dier)
{
    report_->unit_die(dier);
}

void scene::over(uint8_t grade){
    is_end_ = true;
    report_->set_grade(grade);
    if(left_force_->get_is_winner())
    {
        report_->set_winner(left_force_);
    }
    else
    {
        report_->set_winner(right_force_);
    }
}


uint32_t scene::get_frames(){
    return scheduler_.get_frames();
}

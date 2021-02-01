//
//  player.cpp
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "player.h"
#include "equip/equip_pkg.h"
#include "item/item_pkg.h"
#include "soul/soul_pkg.h"
#include "destiny/destiny_pkg.h"
#include "warrior/warrior_pkg.h"
#include "dress/dress_pkg.h"
#include "common/updater.h"

using namespace cache;

player_ptr player::create_player(uint32_t user_id, bool & got){
    player_ptr player(new cache::player(user_id));
    got = true;
    return player;
}

void player::initialize(){
    if(initialized_)
    {
        return;
    }
    
    boost::mutex::scoped_lock lock(self_mutex_);
    
    if(initialized_)
    {
        return;
    }
    
    equip_pkg_ = equip_pkg_ptr(new cache::equip_pkg(user_id_));
    equip_pkg_->load();
    item_pkg_ = item_pkg_ptr(new cache::item_pkg(user_id_));
    item_pkg_->load();
    soul_pkg_ = soul_pkg_ptr(new cache::soul_pkg(user_id_));
    soul_pkg_->load();
    destiny_pkg_ = destiny_pkg_ptr(new cache::destiny_pkg(user_id_));
    destiny_pkg_->load();
    warrior_pkg_ = warrior_pkg_ptr(new cache::warrior_pkg(user_id_));
    warrior_pkg_->load();
    dress_pkg_ = dress_pkg_ptr(new cache::dress_pkg(user_id_));
    dress_pkg_->load();
    updater_ = updater_ptr(new cache::updater(user_id_));
    initialized_ = true;
    
#if DEBUG
    std::stringstream & ss = latte::fast_stringstream::ss();
    ss << "player loaded from db. user id:" << user_id_ << std::endl;
    s_log.out_debug(ss.str());
#endif
}

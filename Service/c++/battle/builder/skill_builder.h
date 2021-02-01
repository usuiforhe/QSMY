//
//  skill_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__skill_builder__
#define __battle__skill_builder__

#include "battle_prefix.h"
#include "singleton.h"
#include "meta/scene_meta_data.pb.h"
#include "skill/skill.h"

namespace battle {
    class unit;
    class skill_builder{
    public:
        skill_builder(){}
        
        ~skill_builder(){}
        
        boost::shared_ptr<skill> build(unit * unit, const battle::skill_meta_data & data);
        
        boost::shared_ptr<skill> build(unit * unit, const lua_object & obj);
        
        boost::shared_ptr<skill> build(unit * unit, const Json::Value & json);
        
        boost::shared_ptr<skill> build(unit * unit, uint32_t sid, uint32_t level);
    };
}

#define S_SKILL_BUILDER latte::singleton<skill_builder>::instance()

#endif /* defined(__battle__skill_builder__) */

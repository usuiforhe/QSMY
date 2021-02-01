//
//  scene_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-6.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__scene_builder__
#define __battle__scene_builder__

#include "battle_prefix.h"
#include "meta/scene_meta_data.pb.h"
#include "singleton.h"

namespace battle{
    class scene;
    class scene_builder{
    public:
        scene_builder(){}
        
        ~scene_builder(){}
        /**
         * 通过protobuf构建场景
         */
        scene * build(battle::scene_meta_data & data);
        
        /**
         * 通过json构建场景
         */
        scene * build(const Json::Value & json);
        /**
         * 构建LUA对象
         */
        void bind_class(scene * scene);
        
    private:
        void build_pve(scene * scene);
        void build_cup(scene * scene);
        void build_challenge(scene * scene);
        void build_tower(scene * scene);
        void build_hunt(scene * scene);
        void build_guild_boss(scene * scene);
        void add_unit_property(scene *scene, uint8_t forceId);
    };
}

#define S_SCENE_BUILDER latte::singleton<scene_builder>::instance()

#endif /* defined(__battle__scene_builder__) */

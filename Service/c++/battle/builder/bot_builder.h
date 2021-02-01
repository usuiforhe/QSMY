//
//  bot_builder.h
//  battle
//
//  Created by 汪燕军 on 13-4-15.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__bot_builder__
#define __battle__bot_builder__

#include "battle_prefix.h"
#include "singleton.h"
#include "meta/scene_meta_data.pb.h"
#include "util/script_loader.h"
#include "cup/bot_name.h"

namespace battle {
    class bot_builder{
    public:
        bot_builder(){}
        
        ~bot_builder(){}
        
        force_meta_data build(uint32_t bot_id, lua_State * L);
        
        void load_bot_name(lua_State * L, std::vector<boost::shared_ptr<bot_name> > & list);
        
    private:
        
    };

#define S_BOT_BUILDER latte::singleton<bot_builder>::instance()
}

#endif /* defined(__battle__bot_builder__) */

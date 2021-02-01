//
//  cup.h
//  battle
//
//  Created by 汪燕军 on 13-4-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__cup__
#define __battle__cup__

#include "battle_prefix.h"
#include "meta/scene_meta_data.pb.h"
#include "cup_player.h"
#include "bot_name.h"

namespace battle{
    class cup{
    public:
        cup(time_t date)
        :date_(date),
        cur_round_(0),
        lua_state_(NULL)
        {
            
        }
        
        ~cup();
        
        void init();
        
        void run();
        
    private:
        time_t date_;
        uint8_t cur_round_;
        
        std::vector<boost::shared_ptr<cup_player> > player_list_;
        std::vector<boost::shared_ptr<force_meta_data> > bot_list_;
        std::vector<boost::shared_ptr<bot_name> > name_list_;
        
        lua_State * lua_state_;
        
        void match(uint16_t final_num, uint16_t match_id, boost::shared_ptr<cup_player> user1, boost::shared_ptr<cup_player> user2 = boost::shared_ptr<cup_player>());
        boost::shared_ptr<cup_player>  get_bot();
        
        void load_bot();
    };
}

#endif /* defined(__battle__cup__) */

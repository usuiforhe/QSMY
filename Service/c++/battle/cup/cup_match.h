//
//  cup_match.h
//  battle
//
//  Created by 汪燕军 on 13-4-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__cup_match__
#define __battle__cup_match__

#include "battle_prefix.h"
#include "game/scene.h"
#include "game/report.h"

namespace battle{
    class cup_player;
    class cup_match{
    public:
        cup_match(uint16_t final_num, uint16_t match_id, boost::shared_ptr<cup_player>  left, boost::shared_ptr<cup_player> right);
        
        ~cup_match();
        
        inline GETTER(boost::shared_ptr<cup_player> , winner);
        
        inline GETTER(uint16_t, final_num);
        
        void match();
        
    private:
        uint16_t final_num_;
        uint16_t match_id_;
        boost::shared_ptr<cup_player>  left_;
        boost::shared_ptr<cup_player>  right_;
        boost::shared_ptr<cup_player>  winner_;
        boost::shared_ptr<cup_player>  loser_;
                
        scene * scene_;
        
        void save();
    };
}

#endif /* defined(__battle__cup_match__) */

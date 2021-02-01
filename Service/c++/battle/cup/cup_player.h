//
//  cup_player.h
//  battle
//
//  Created by 汪燕军 on 13-4-10.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__cup_player__
#define __battle__cup_player__

#include "battle_prefix.h"
#include "meta/scene_meta_data.pb.h"

namespace battle {
    class cup_match;
    class cup_player{
    public:
        cup_player(uint32_t user_id,std::auto_ptr<std::istream> formation_stream, const std::string & disp, std::string & disp_detail, uint32_t win_count, uint16_t best_grade);
        explicit cup_player(force_meta_data & force);
        
        ~cup_player();
        
        inline GETTER(uint32_t, user_id);
        
        inline GETTER(const std::string &, disp);
        
        inline GETTER(const std::string &, disp_detail);
        
        inline GETTER(force_meta_data &, force);
        
        inline GETTER(uint32_t, win_count);
        
        inline GETTER(uint16_t, best_grade);
        
        void match_over(cup_match * match);
    private:
        uint32_t user_id_;
        force_meta_data force_;
        std::string disp_;
        std::string disp_detail_;
        uint32_t win_count_;
        uint16_t best_grade_;
    };
}

#endif /* defined(__battle__cup_player__) */

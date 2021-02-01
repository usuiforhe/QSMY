//
//  client_report_builder.h
//  battle
//
//  Created by wangyanjun on 13-3-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__client_report_builder__
#define __battle__client_report_builder__

#include "battle_prefix.h"
#include "game/scene.h"
#include "force/force.h"


namespace battle{
    class report;
    class client_report_builder{
    public:
        std::string build(scene * scene, report * report);
        
    private:
        void build_scene(scene * scene, std::vector<char> & bin);
        void build_force(force * force, std::vector<char> & bin);
        void build_behaviors(report * report, std::vector<char> & bin);
    };
#define S_CLIENT_REPORT_BUILDER latte::singleton<client_report_builder>::instance()
}

#endif /* defined(__battle__client_report_builder__) */

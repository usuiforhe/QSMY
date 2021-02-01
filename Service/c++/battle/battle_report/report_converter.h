//
//  report_converter.h
//  battle
//
//  Created by 汪燕军 on 13-10-12.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __battle__report_converter__
#define __battle__report_converter__

#include "battle_prefix.h"
#include "cup/cup_match.h"

namespace battle_report
{
    struct match_info
    {
    public:
        match_info()
        : final_num(0)
        , match_id(0)
        , report(new std::vector<char>())
        , new_report(new std::vector<char>())
        {
            
        }
        
        virtual ~match_info()
        {
            
        }
        
        uint16_t final_num;
        uint16_t match_id;
        boost::shared_ptr<std::vector<char> > report;
        boost::shared_ptr<std::vector<char> > new_report;
    };
    
    class report_converter
    {
    public:
        report_converter(){}
        ~report_converter(){}
        void convert_cup();
    
    private:
        void convert_one(boost::shared_ptr<match_info> match);
        
        void load_cup_matches(std::vector<boost::shared_ptr<match_info> > & list);
        
        void update_cup_match_report(boost::shared_ptr<match_info> match);
        
        void parse_force(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter);
        void parse_unit(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter);
        void parse_action(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter);
        void parse_result(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter);
        void parse_coactors(boost::shared_ptr<match_info> match, std::vector<char>::iterator & iter);
    };
}


#endif /* defined(__battle__report_converter__) */

//
//  command_center.h
//  battle
//
//  Created by 汪燕军 on 13-5-28.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef battle_command_center_h
#define battle_command_center_h

#include "battle_prefix.h"
#include "config.h"
#include "cup/cup.h"
namespace battle {
    class command_center{
    public:
        std::string update_script(){
            std::string command = "svn up " + S_CONFIG.get_script_path();
            std::system(command.c_str());
            return "ok";
        }
        
        std::string run_cup(){
            try{
                std::tm today = boost::gregorian::to_tm(boost::gregorian::day_clock::local_day());
                cup c(std::mktime(&today));
                c.init();
                c.run();
                return "ok";
            }
            catch (boost::exception& e)
            {
                s_log.out_error("command_center::run_cup throw exception. what:" + boost::diagnostic_information(e));
                return boost::diagnostic_information(e);
            }
            catch (std::exception& e)
            {
                s_log.out_error("command_center::run_cup throw exception. what:" + std::string(e.what()));
                
                return e.what();
            }
            catch(...){
                s_log.out_error("command_center::run_cup throw exception.");
                return "run cup failed";
            }
        }
    };
#define S_COMMAND_CENTER latte::singleton<command_center>::instance()
}


#endif

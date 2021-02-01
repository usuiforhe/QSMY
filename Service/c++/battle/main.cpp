
#include "log/log.h"
#include "battle_prefix.h"
#include "game/scene.h"
#include "config.h"
#include "gateway/battle_service.h"
#include "cup/cup_scheduler.h"
#include "server.hpp"
#include "command_center.h"

using namespace battle;


bool is_start = false;

void sig_abort_handler(int signal);
void before_quit();
void open_monitor();

void sig_abort_handler(int signal_code)
{
	s_log.out_string("程序准备退出 ... ");
	before_quit();
	exit(-1);
}

void before_quit()
{
    try{
        S_BATTLE_SERVICE.stop();
        S_CUP_SCHEDULER.stop();
        TIME_NOW.stop();
    }
    catch(...)
    {
        
    }
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
#if !DEBUG
	try
	{
#endif
        TIME_NOW.start();
		s_log.out_string("pili:battle started.");
        S_CUP_SCHEDULER.start();
        S_BATTLE_SERVICE.start();
		is_start = true;
        
        http::server::server s("0.0.0.0", boost::lexical_cast<std::string>(S_CONFIG.get_http_port()));
        s_log.out_string("Http Listen:" + boost::lexical_cast<std::string>(S_CONFIG.get_http_port()));
#if DEBUG
        s.add_handler("update_script", boost::bind(&command_center::update_script, S_COMMAND_CENTER));
#endif
        s.add_handler("run_cup", boost::bind(&command_center::run_cup, S_COMMAND_CENTER));
        
        s.run();
        
        before_quit();
		s_log.out_string("service shutdown...");
        
		return 0;
		
#if !DEBUG
	}
    catch (boost::exception& e)
    {
        s_log.out_error("pili:battle:main throw exception. what:" + boost::diagnostic_information(e));
        
    }
	catch (std::exception& e)
	{
		s_log.out_error("pili:battle:main throw exception. what:" + std::string(e.what()));
	}
#endif
	
	before_quit();
	return 1;
}

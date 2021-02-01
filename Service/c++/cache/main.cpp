#include "cache_prefix.h"
#include "config.h"
#include "cache_service.h"
#include "server.hpp"
#include "command_center.h"

using namespace cache;


bool is_start = false;

void before_quit()
{
    try{
        SINGLETON(cache_service).stop();
        TIME_NOW.stop();
    }
    catch(...)
    {
        
    }
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
#ifndef DEBUG
	try
	{
#endif                
        TIME_NOW.start();
        SINGLETON(cache_service).start();
		s_log.out_string("pili:cache started.");
		is_start = true;
        
        http::server::server s("0.0.0.0", boost::lexical_cast<std::string>(SINGLETON(config).http_port));
        s_log.out_string("Http Listen:" + boost::lexical_cast<std::string>(SINGLETON(config).http_port));

        s.add_handler("qt", boost::bind(&command_center::get_opq_count, SINGLETON(command_center)));
        
        s.run();
        
        before_quit();
		s_log.out_string("service shutdown...");
        
		return 0;
		
#ifndef DEBUG
	}
    catch (boost::exception& e)
    {
        s_log.out_error("pili:cache:main throw exception. what:" + boost::diagnostic_information(e));
        
    }
	catch (std::exception& e)
	{
		s_log.out_error("pili:cache:main throw exception. what:" + std::string(e.what()));
	}
    catch(...)
    {
		s_log.out_error("pili:cache:main throw exception");
    }
#endif
	
	before_quit();
	return 1;
}

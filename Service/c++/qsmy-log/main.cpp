#include "config.h"
#include "log_prefix.h"
#include "log_service.h"

using namespace qsmy_log;


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
        SINGLETON(log_service).stop();
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
        SINGLETON(log_service).start();
		s_log.out_string("pili-log started.");
		is_start = true;
        
        while (true)
		{
			std::string line;
			std::getline(std::cin, line);
			
			if (line == "exit" || line == "quit" || line == "bye" || line == "q")
			{
				break;
			}
            
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
        
        before_quit();
		s_log.out_string("service shutdown...");
        
		return 0;
		
#ifndef DEBUG
	}
    catch (boost::exception& e)
    {
        s_log.out_error("pili-log:main throw exception. what:" + boost::diagnostic_information(e));
        
    }
	catch (std::exception& e)
	{
		s_log.out_error("pili-log:main throw exception. what:" + std::string(e.what()));
	}
#endif
	
	before_quit();
	return 1;
}

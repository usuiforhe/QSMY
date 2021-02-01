AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = qsmy_log
qsmy_log_SOURCES = \
	../../qsmy-log/archive_task.cpp \
	../../qsmy-log/config.cpp \
	../../qsmy-log/log_model.cpp \
	../../qsmy-log/log_service.cpp \
	../../qsmy-log/sys_utility_model.cpp \
	../../qsmy-log/main.cpp \
	../../qsmy-log/scheduler.cpp \
	../../qsmy-log/log_queue.cpp
		
CPPFLAGS = -I../../../../../../SDK/server/c++/release/include/latte -I../../qsmy-log -DDEBUG=1
CXXFLAGS = -O3 -g3 -Wall 
LDADD = ../../../../../../SDK/server/c++/release/lib/centos/liblatte.a
LDFLAGS =  -L/usr/local/lib -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lpthread -lprotobuf -ldl -rdynamic -lstdc++ -lmysqlclient -lmysqlcppconn

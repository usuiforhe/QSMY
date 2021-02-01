AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = qsmy_cache
qsmy_cache_SOURCES = \
	../../cache/cache_service.cpp \
	../../cache/config.cpp \
	../../cache/base/async_update_object.cpp \
	../../cache/common/updater.cpp \
	../../cache/destiny/destiny.cpp \
	../../cache/destiny/destiny_pkg.cpp \
	../../cache/dress/dress.cpp \
	../../cache/dress/dress_pkg.cpp \
	../../cache/equip/equip.cpp \
	../../cache/equip/equip_pkg.cpp \
	../../cache/handler/common_handler.cpp \
	../../cache/handler/destiny_handler.cpp \
	../../cache/handler/dress_handler.cpp \
	../../cache/handler/equip_handler.cpp \
	../../cache/handler/item_handler.cpp \
	../../cache/handler/soul_handler.cpp \
	../../cache/handler/warrior_handler.cpp \
	../../cache/item/item.cpp \
	../../cache/item/item_pkg.cpp \
	../../cache/main.cpp \
	../../cache/player.cpp \
	../../cache/player_manager.cpp \
	../../cache/soul/soul.cpp \
	../../cache/soul/soul_pkg.cpp \
	../../cache/warrior/warrior.cpp \
	../../cache/warrior/warrior_pkg.cpp \
	../../cache/protocol/common.pb.cc \
	../../cache/protocol/destiny.pb.cc \
	../../cache/protocol/dress.pb.cc \
	../../cache/protocol/equip.pb.cc \
	../../cache/protocol/item.pb.cc \
	../../cache/protocol/soul.pb.cc \
	../../cache/protocol/warrior.pb.cc
		
		
CPPFLAGS = -DTIME_STATIC=0 -I../../../../../../SDK/server/c++/release/include/latte -I../../../../../../SDK/server/c++/release/include/http -I../../cache
CXXFLAGS = -O3 -g3 -Wall 
LDADD = ../../../../../../SDK/server/c++/release/lib/centos/liblatte.a ../../../../../../SDK/server/c++/release/lib/centos/libhttp.a
LDFLAGS =  -L/usr/local/lib -lboost_date_time -lboost_system -lboost_chrono -lboost_thread -lboost_filesystem -lpthread -lprotobuf -ldl -rdynamic -lstdc++ -lmysqlclient -lmysqlcppconn

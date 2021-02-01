AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = qsmy_battle
qsmy_battle_SOURCES = \
	../../battle/buff/action_buff.cpp \
	../../battle/buff/unit_buff.cpp \
	../../battle/buff/unit_frames_buff_expire_strategy.cpp \
	../../battle/builder/bot_builder.cpp \
	../../battle/builder/buff_builder.cpp \
	../../battle/builder/client_report_builder.cpp \
	../../battle/builder/force_builder.cpp \
	../../battle/builder/scene_builder.cpp \
	../../battle/builder/skill_builder.cpp \
	../../battle/builder/unit_builder.cpp \
	../../battle/builder/user_builder.cpp \
	../../battle/config.cpp \
	../../battle/cup/cup.cpp \
	../../battle/cup/cup_match.cpp \
	../../battle/cup/cup_player.cpp \
	../../battle/cup/cup_scheduler.cpp \
	../../battle/event/handler/unit_event_handler.cpp \
	../../battle/force/force.cpp \
	../../battle/force/user.cpp \
	../../battle/game/behavior/attack_behavior.cpp \
	../../battle/game/behavior/buff_behavior_result.cpp \
	../../battle/game/behavior/buff_effect_behavior.cpp \
	../../battle/game/behavior/damage_behavior_result.cpp \
	../../battle/game/behavior/dialog_behavior_result.cpp \
	../../battle/game/behavior/display_behavior_result.cpp \
	../../battle/game/behavior/heal_behavior_result.cpp \
	../../battle/game/behavior/skill_behavior.cpp \
	../../battle/game/end_checker/normal_end_checker.cpp \
	../../battle/game/game_context.cpp \
	../../battle/game/report.cpp \
	../../battle/game/scene.cpp \
	../../battle/game/scheduler.cpp \
	../../battle/gateway/battle_service.cpp \
	../../battle/gateway/create_battle_handler.cpp \
	../../battle/gateway/default_handler.cpp \
	../../battle/main.cpp \
	../../battle/rule/target_rule.cpp \
	../../battle/skill/skill.cpp \
	../../battle/unit/unit.cpp \
	../../battle/unit/unit_buff_partitial.cpp \
	../../battle/unit/unit_state.cpp \
	../../battle/unit/unit_task.cpp \
	../../battle/util/script_loader.cpp \
	../../battle/meta/report_meta_data.pb.cc \
	../../battle/meta/scene_meta_data.pb.cc
	
CPPFLAGS = -I../../../../../../SDK/server/c++/sdk/luabind -I../../../../../../SDK/server/c++/release/include/latte -I../../../../../../SDK/server/c++/release/include/http -I../../battle  -DDEBUG=0 -DTIME_STATIC=0
CXXFLAGS = -O3 -g3 -Wall 
LDADD = ../../../../../../SDK/server/c++/release/lib/centos/liblatte.a ../../../../../../SDK/server/c++/release/lib/centos/libhttp.a /usr/local/lib/liblua.a /usr/local/lib/libluabind.a
LDFLAGS =  -L/usr/local/lib -lboost_date_time -lboost_chrono -lboost_system -lboost_thread -lboost_filesystem -lpthread -lprotobuf -ldl -rdynamic -lstdc++ -lmysqlclient -lmysqlcppconn

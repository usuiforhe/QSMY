//
//  defines.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef cache_defines_h
#define cache_defines_h

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

namespace cache{
    class player;
    class equip;
    class equip_pkg;
    
    class proRes;
    
    class proEquip;
    class proEquipRes;
    class proEquipsRes;
    
    class item;
    class item_pkg;
    class proItem;
    class proItemRes;
    class proItemsRes;
    
    class soul;
    class soul_pkg;
    class proSoul;
    class proSoulRes;
    class proSoulsRes;
    
    class destiny;
    class destiny_pkg;
    class proDestiny;
    class proDestinyRes;
    class proDestiniesRes;
    
    class warrior;
    class warrior_pkg;
    class proWarrior;
    class proWarriorRes;
    class proWarriorsRes;
    
    class dress;
    class dress_pkg;
    class proDress;
    class proDressRes;
    class proDressesRes;
    
    class updater;
    class proAsyncUpdate;
    
    typedef boost::shared_lock<boost::shared_mutex> read_lock;
    typedef boost::unique_lock<boost::shared_mutex> write_lock;
    
    typedef boost::shared_ptr<player> player_ptr;
    typedef boost::weak_ptr<player> weak_player_ptr;
    typedef boost::shared_ptr<equip> equip_ptr;
    typedef boost::shared_ptr<equip_pkg> equip_pkg_ptr;
    
    typedef boost::shared_ptr<proRes> pro_res_ptr;
    
    typedef boost::shared_ptr<proEquip> pro_equip_ptr;    
    typedef boost::shared_ptr<proEquipRes> pro_equip_res_ptr;
    typedef boost::shared_ptr<proEquipsRes> pro_equips_res_ptr;
    
    typedef boost::shared_ptr<item> item_ptr;
    typedef boost::shared_ptr<item_pkg> item_pkg_ptr;
    
    typedef boost::shared_ptr<proItem> pro_item_ptr;
    typedef boost::shared_ptr<proItemRes> pro_item_res_ptr;
    typedef boost::shared_ptr<proItemsRes> pro_items_res_ptr;
    
    typedef boost::shared_ptr<soul> soul_ptr;
    typedef boost::shared_ptr<soul_pkg> soul_pkg_ptr;
    
    typedef boost::shared_ptr<proSoul> pro_soul_ptr;
    typedef boost::shared_ptr<proSoulRes> pro_soul_res_ptr;
    typedef boost::shared_ptr<proSoulsRes> pro_souls_res_ptr;
    
    typedef boost::shared_ptr<destiny> destiny_ptr;
    typedef boost::shared_ptr<destiny_pkg> destiny_pkg_ptr;
    
    typedef boost::shared_ptr<proDestiny> pro_destiny_ptr;
    typedef boost::shared_ptr<proDestinyRes> pro_destiny_res_ptr;
    typedef boost::shared_ptr<proDestiniesRes> pro_destinies_res_ptr;
        
    typedef boost::shared_ptr<warrior> warrior_ptr;
    typedef boost::shared_ptr<warrior_pkg> warrior_pkg_ptr;
    
    typedef boost::shared_ptr<proWarrior> pro_warrior_ptr;
    typedef boost::shared_ptr<proWarriorRes> pro_warrior_res_ptr;
    typedef boost::shared_ptr<proWarriorsRes> pro_warriors_res_ptr;
    
    typedef boost::shared_ptr<dress> dress_ptr;
    typedef boost::shared_ptr<dress_pkg> dress_pkg_ptr;
    
    typedef boost::shared_ptr<proDress> pro_dress_ptr;
    typedef boost::shared_ptr<proDressRes> pro_dress_res_ptr;
    typedef boost::shared_ptr<proDressesRes> pro_dresses_res_ptr;
    
    typedef boost::shared_ptr<proAsyncUpdate> pro_async_update_ptr;
    typedef boost::shared_ptr<updater> updater_ptr;
    
    
#define LOCK_TIMEOUT 10
#define READ_LOCK(var, mutex) read_lock var(mutex, boost::chrono::seconds(LOCK_TIMEOUT));\
    if(!var.owns_lock()){\
        BOOST_THROW_EXCEPTION(boost::lock_error());\
    }
#define WRITE_LOCK(var, mutex) write_lock var(mutex, boost::chrono::seconds(LOCK_TIMEOUT));\
    if(!var.owns_lock()){\
        BOOST_THROW_EXCEPTION(boost::lock_error());\
    }
    
#define SCOPED_LOCK(var, m) boost::unique_lock<boost::timed_mutex> var(m, boost::chrono::seconds(LOCK_TIMEOUT));\
    if(!var.owns_lock()){\
    BOOST_THROW_EXCEPTION(boost::lock_error());\
}
    
#define GETTER(type, name)  type name(){ return name##_; }
#define CONST_GETTER(type, name)  type name() const { return name##_; }
#define SETTER(type, name)  void name(type name){ name##_ = name; }

#define SINGLETON(T) latte::singleton<T>::instance()
    
#define SERVICE_HANDLER_TRY try{
    
#define SERVICE_HANDLER_CATCH(response) \
}\
catch(const std::exception & e){\
    response->set_code(SYS_ERROR);\
    std::stringstream &ss = latte::fast_stringstream::ss();\
    ss << name_ << " throws exceptions. what:" + boost::diagnostic_information(e);\
    s_log.out_error(ss.str());\
}\
catch(const boost::exception & e){\
    response->set_code(SYS_ERROR);\
    std::stringstream &ss = latte::fast_stringstream::ss();\
    ss << name_ << " throws exceptions. what:" + boost::diagnostic_information(e);\
    s_log.out_error(ss.str());\
}

}


#endif

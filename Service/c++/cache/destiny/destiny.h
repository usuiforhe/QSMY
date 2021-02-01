//
//  destiny.h
//  cache
//
//  Created by 汪燕军 on 13-5-11.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__destiny__
#define __cache__destiny__

#include "cache_prefix.h"
#include "protocol/destiny.pb.h"
#include "base/async_update_object.h"

namespace cache{
    class destiny:public async_update_object{
    public:
        destiny(
                uint64_t id,
                uint32_t destiny_id,
                uint8_t type,
                uint16_t level = 0,
                uint32_t reward_ts = 0,
                uint16_t num1 = 0,
                uint16_t num2 = 0,
                uint16_t num3 = 0,
                uint16_t num4 = 0,
                uint16_t num5 = 0,
                uint16_t num6 = 0):
        async_update_object(),
        id_(id),
        destiny_id_(destiny_id),
        type_(type),
        level_(level),
        reward_ts_(reward_ts),
        num1_(num1),
        num2_(num2),
        num3_(num3),
        num4_(num4),
        num5_(num5),
        num6_(num6){
            
        }
        
        explicit destiny(const proDestiny & proto):
        async_update_object(),
        id_(0),
        destiny_id_(proto.destiny_id()),
        type_(static_cast<uint8_t>(proto.type())),
        level_(static_cast<uint16_t>(proto.level())),
        reward_ts_(proto.reward_ts()),
        num1_(static_cast<uint16_t>(proto.num1())),
        num2_(static_cast<uint16_t>(proto.num2())),
        num3_(static_cast<uint16_t>(proto.num3())),
        num4_(static_cast<uint16_t>(proto.num4())),
        num5_(static_cast<uint16_t>(proto.num5())),
        num6_(static_cast<uint16_t>(proto.num6())){
            
        }
        
        inline GETTER(uint64_t, id);
        inline SETTER(uint64_t, id);
        inline GETTER(uint32_t, destiny_id);
        inline GETTER(uint8_t, type);
        inline GETTER(uint16_t, level);
        inline GETTER(uint32_t, reward_ts);
        inline GETTER(uint16_t, num1);
        inline GETTER(uint16_t, num2);
        inline GETTER(uint16_t, num3);
        inline GETTER(uint16_t, num4);
        inline GETTER(uint16_t, num5);
        inline GETTER(uint16_t, num6);
        
        inline GETTER(boost::shared_mutex &, mutex);
        
        virtual ~destiny(){}
        
        inline std::auto_ptr<proDestiny> to_proto(){
            std::auto_ptr<proDestiny> proto(new proDestiny);
            READ_LOCK(lock, mutex_);
            
            proto->set_destiny_id(destiny_id_);
            proto->set_type(type_);
            proto->set_level(level_);
            proto->set_reward_ts(reward_ts_);
            proto->set_num1(num1_);
            proto->set_num2(num2_);
            proto->set_num3(num3_);
            proto->set_num4(num4_);
            proto->set_num5(num5_);
            proto->set_num6(num6_);
            
            return proto;
        }
        
        inline void copy_from(const destiny & source){
            level_ = source.level_;
            reward_ts_ = source.reward_ts_;
            num1_ = source.num1_;
            num2_ = source.num2_;
            num3_ = source.num3_;
            num4_ = source.num4_;
            num5_ = source.num5_;
            num6_ = source.num6_;
        }
        
        inline void copy_from(const proDestiny & source){
            level_ = static_cast<uint16_t>(source.level());
            reward_ts_ = source.reward_ts();
            num1_ = static_cast<uint16_t>(source.num1());
            num2_ = static_cast<uint16_t>(source.num2());
            num3_ = static_cast<uint16_t>(source.num3());
            num4_ = static_cast<uint16_t>(source.num4());
            num5_ = static_cast<uint16_t>(source.num5());
            num6_ = static_cast<uint16_t>(source.num6());
        }
        
        inline void copy_to_proto(proDestiny * proto){
            READ_LOCK(lock, mutex_);
            
            proto->set_destiny_id(destiny_id_);
            proto->set_type(type_);
            proto->set_level(level_);
            proto->set_reward_ts(reward_ts_);
            proto->set_num1(num1_);
            proto->set_num2(num2_);
            proto->set_num3(num3_);
            proto->set_num4(num4_);
            proto->set_num5(num5_);
            proto->set_num6(num6_);
        }
        
        inline void copy_to_destiny_res(pro_destiny_res_ptr res){
            READ_LOCK(lock, mutex_);
            
            proDestiny * proto = new proDestiny;
            res->set_allocated_destiny(proto);
            proto->set_destiny_id(destiny_id_);
            proto->set_type(type_);
            proto->set_level(level_);
            proto->set_reward_ts(reward_ts_);
            proto->set_num1(num1_);
            proto->set_num2(num2_);
            proto->set_num3(num3_);
            proto->set_num4(num4_);
            proto->set_num5(num5_);
            proto->set_num6(num6_);
        }
        
    private:
        uint64_t id_;
        uint32_t destiny_id_;
        uint8_t type_;
        uint16_t level_;
        uint32_t reward_ts_;
        uint16_t num1_;
        uint16_t num2_;
        uint16_t num3_;
        uint16_t num4_;
        uint16_t num5_;
        uint16_t num6_;
        
        boost::shared_mutex mutex_;
    };
}

#endif /* defined(__cache__destiny__) */

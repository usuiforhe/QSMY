//
//  soul.h
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__soul__
#define __cache__soul__

#include "cache_prefix.h"
#include "protocol/soul.pb.h"
#include "base/async_update_object.h"
namespace cache {
    class soul:public async_update_object{
    public:
        explicit soul(const proSoul & proto):
        async_update_object(),
        id_(0),
        soul_id_(proto.soul_id()),
        num_(proto.num()),
        protect_ts_(proto.protect_ts()){
            
        }
        
        soul(uint64_t id, uint32_t soul_id, uint32_t num, uint32_t protect_ts = 0):
        async_update_object(),
        id_(id),
        soul_id_(soul_id),
        num_(num),
        protect_ts_(protect_ts){
            
        }
        
        virtual ~soul(){}
        
        inline GETTER(uint64_t, id);
        inline SETTER(uint64_t, id);
        inline GETTER(uint32_t, soul_id);
        inline GETTER(uint32_t, num);
        inline GETTER(uint32_t, protect_ts);
        inline SETTER(uint32_t, protect_ts);
        inline GETTER(boost::shared_mutex &, mutex);
        
        inline void add(uint32_t num){
            WRITE_LOCK(lock, mutex_);
            num_ += num;
        }
        
        inline bool deduct(soul_ptr soul){
            WRITE_LOCK(lock, mutex_);
            if(num_ < soul->num_){
                return false;
            }
            else{
                num_ -= soul->num_;
                if(soul->protect_ts_ > protect_ts_)
                {
                    protect_ts_ = soul->protect_ts_;
                }
                return true;
            }
        }
        
        inline bool is_enough(uint32_t num){
            READ_LOCK(lock, mutex_);
            return num_ > 0 && num_ >= num;
        }
        
        inline std::auto_ptr<proSoul> to_proto(){
            std::auto_ptr<proSoul> proto(new proSoul);
            READ_LOCK(lock, mutex_);
            proto->set_soul_id(soul_id_);
            proto->set_num(num_);
            proto->set_protect_ts(protect_ts_);
            
            return proto;
        }
        
        inline void copy_to_proto(proSoul * proto){
            READ_LOCK(lock, mutex_);
            proto->set_soul_id(soul_id_);
            proto->set_num(num_);
            proto->set_protect_ts(protect_ts_);
        }
        
        inline void copy_to_soul_res(pro_soul_res_ptr res){
            READ_LOCK(lock, mutex_);
            proSoul * proto = new proSoul;
            res->set_allocated_soul(proto);
            proto->set_soul_id(soul_id_);
            proto->set_num(num_);
            proto->set_protect_ts(protect_ts_);
        }
        
    private:
        uint64_t id_;
        uint32_t soul_id_;
        uint32_t num_;
        uint32_t protect_ts_;
        
        boost::shared_mutex mutex_;
    };
}

#endif /* defined(__cache__soul__) */

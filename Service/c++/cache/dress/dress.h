//
//  dress.h
//  cache
//
//  Created by 汪燕军 on 14-3-4.
//  Copyright (c) 2014年 汪燕军. All rights reserved.
//

#ifndef __cache__dress__
#define __cache__dress__


#include "cache_prefix.h"
#include "protocol/dress.pb.h"
#include "base/async_update_object.h"

namespace cache {
    class dress:public async_update_object{
    public:
        dress(uint64_t id,
              uint32_t dress_id,
              uint32_t warrior_id,
              uint16_t level
              ):
        async_update_object(),
        id_(id),
        dress_id_(dress_id),
        warrior_id_(warrior_id),
        level_(level)        {
            
        }
        
        explicit dress(const proDress & dress):
        async_update_object(),
        id_(0),
        dress_id_(dress.dress_id()),
        warrior_id_(dress.warrior_id()),
        level_(dress.level())
        {
            
        }
        
        inline GETTER(uint64_t, id);
        inline SETTER(uint64_t, id);
        inline GETTER(uint32_t, dress_id);
        inline GETTER(uint32_t, warrior_id);
        inline GETTER(uint16_t, level);
        
        inline GETTER(boost::shared_mutex &, mutex);
        
        virtual ~dress(){}
        
        inline std::auto_ptr<proDress> to_proto(){
            std::auto_ptr<proDress> proto(new proDress);
            READ_LOCK(lock, mutex_);
            
            proto->set_dress_id(dress_id_);
            proto->set_warrior_id(warrior_id_);
            proto->set_level(level_);
            
            return proto;
        }
        
        inline void copy_from(const dress & source){
            dress_id_ = source.dress_id_;
            warrior_id_ = source.warrior_id_;
            level_ = source.level_;
        }
        
        inline void copy_from(const proDress & source){
            dress_id_ = source.dress_id();
            warrior_id_ = source.warrior_id();
            level_ = static_cast<uint16_t>(source.level());
        }
        
        inline void copy_to_proto(proDress * proto){
            READ_LOCK(lock, mutex_);
            
            proto->set_dress_id(dress_id_);
            proto->set_warrior_id(warrior_id_);
            proto->set_level(level_);
        }
        
        inline void copy_to_dress_res(pro_dress_res_ptr res){
            READ_LOCK(lock, mutex_);
            
            proDress * proto = new proDress;
            res->set_allocated_dress(proto);
            
            proto->set_dress_id(dress_id_);
            proto->set_warrior_id(warrior_id_);
            proto->set_level(level_);
        }
        
    private:
        uint64_t id_;
        uint32_t dress_id_;
        uint32_t warrior_id_;
        uint16_t level_;
        
        boost::shared_mutex mutex_;
    };
}


#endif /* defined(__cache__dress__) */

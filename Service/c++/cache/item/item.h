//
//  item.h
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__item__
#define __cache__item__

#include "cache_prefix.h"
#include "protocol/item.pb.h"
#include "base/async_update_object.h"
namespace cache {
    class item:public async_update_object{
    public:
        explicit item(const proItem & proto):
        async_update_object(),
        id_(0),
        item_id_(proto.item_id()),
        num_(proto.num()){
            
        }
        
        item(uint64_t id, uint32_t item_id, uint32_t num):
        async_update_object(),
        id_(id),
        item_id_(item_id),
        num_(num){
            
        }
        
        virtual ~item(){}
        
        inline GETTER(uint64_t, id);
        inline SETTER(uint64_t, id);
        inline GETTER(uint32_t, item_id);
        inline GETTER(uint32_t, num);
        inline GETTER(boost::shared_mutex &, mutex);
        
        inline void add(uint32_t num){
            WRITE_LOCK(lock, mutex_);
            num_ += num;
        }
        
        inline bool deduct(uint32_t num){
            WRITE_LOCK(lock, mutex_);
            if(num_ < num){
                return false;
            }
            else{
                num_ -= num;
                return true;
            }
        }
        
        inline bool is_enough(uint32_t num){
            READ_LOCK(lock, mutex_);
            return num_ > 0 && num_ >= num;
        }
        
        inline std::auto_ptr<proItem> to_proto(){
            std::auto_ptr<proItem> proto(new proItem);
            READ_LOCK(lock, mutex_);
            proto->set_item_id(item_id_);
            proto->set_num(num_);
            
            return proto;
        }
        
        inline void copy_to_proto(proItem * proto){
            READ_LOCK(lock, mutex_);
            proto->set_item_id(item_id_);
            proto->set_num(num_);
        }
        
        inline void copy_to_item_res(pro_item_res_ptr res){
            READ_LOCK(lock, mutex_);
            proItem * proto = new proItem;
            res->set_allocated_item(proto);
            proto->set_item_id(item_id_);
            proto->set_num(num_);
        }        
        
    private:
        uint64_t id_;
        uint32_t item_id_;
        uint32_t num_;
        
        boost::shared_mutex mutex_;
    };
}

#endif /* defined(__cache__item__) */

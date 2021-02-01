//
//  equip.h
//  cache
//
//  Created by 汪燕军 on 13-5-9.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__equip__
#define __cache__equip__

#include "cache_prefix.h"
#include "protocol/equip.pb.h"
#include "base/async_update_object.h"

namespace cache {
    class equip:public async_update_object{
    public:
        equip(uint64_t id,
              uint32_t sid,
              uint32_t exp,
              uint16_t level,
              uint32_t gem,
              uint32_t pos,
              uint8_t type,
              uint8_t color,
              uint16_t refining,
              uint32_t refining_exp):
        async_update_object(),
        id_(id),
        sid_(sid),
        exp_(exp),
        level_(level),
        gem_(gem),
        pos_(pos),
        type_(type),
        color_(color),
        refining_(refining),
		refining_exp_(refining_exp){
            
        }
        
        explicit equip(const proEquip & proto):
        async_update_object(),
        id_(proto.id()),
        sid_(proto.sid()),
        exp_(proto.exp()),
        level_(static_cast<uint16_t>(proto.level())),
        gem_(proto.gem()),
        pos_(proto.pos()),
        type_(static_cast<uint8_t>(proto.type())),
        color_(static_cast<uint8_t>(proto.color())),
        refining_(static_cast<uint16_t>(proto.refining())),
        refining_exp_(static_cast<uint32_t>(proto.refining_exp())){

        }
        
        virtual ~equip(){
            
        }
        
        inline GETTER(uint64_t, id);
        inline SETTER(uint64_t, id);
        inline GETTER(uint32_t, sid);
        inline GETTER(uint32_t, exp);
        inline SETTER(uint32_t, exp);
        inline GETTER(uint16_t, level);
        inline SETTER(uint16_t, level);
        inline GETTER(uint32_t, gem);
        inline SETTER(uint32_t, gem);
        inline GETTER(uint32_t, pos);
        inline SETTER(uint32_t, pos);
        inline GETTER(uint8_t, type);
        inline SETTER(uint8_t, type);
        inline GETTER(uint8_t, color);
        inline SETTER(uint8_t, color);
        inline GETTER(uint16_t, refining);
        inline SETTER(uint16_t, refining);
		inline GETTER(uint32_t, refining_exp);
        inline SETTER(uint32_t, refining_exp);
        inline GETTER(boost::shared_mutex &, mutex);
        
        inline void copy_from(const equip & e){
            id_ = e.id_;
            sid_ = e.sid_;
            exp_ = e.exp_;
            level_ = e.level_;
            gem_ = e.gem_;
            pos_ = e.pos_;
            type_ = e.type_;
            color_ = e.color_;
            refining_ = e.refining_;
			refining_exp_ = e.refining_exp_;
        }
        
        inline void copy_from(const proEquip & proto){
            id_ = proto.id();
            sid_ = proto.sid();
            exp_ = proto.exp();
            level_ = proto.level();
            gem_ = proto.gem();
            pos_ = proto.pos();
            type_ = static_cast<uint8_t>(proto.type());
            color_ = static_cast<uint8_t>(proto.color());
            refining_ = static_cast<uint16_t>(proto.refining());
			refining_exp_ = static_cast<uint32_t>(proto.refining_exp());
        }
        
        inline std::auto_ptr<proEquip> to_proto(){
            std::auto_ptr<proEquip> proto(new proEquip);
            READ_LOCK(lock, mutex_);
            proto->set_id(id_);
            proto->set_sid(sid_);
            proto->set_exp(exp_);
            proto->set_level(level_);
            proto->set_gem(gem_);
            proto->set_pos(pos_);
            proto->set_type(type_);
            proto->set_color(color_);
            proto->set_refining(refining_);
            proto->set_refining_exp(refining_exp_);
            return proto;
        }
        
        inline void copy_to_proto(proEquip * proto){
            READ_LOCK(lock, mutex_);
            proto->set_id(id_);
            proto->set_sid(sid_);
            proto->set_exp(exp_);
            proto->set_level(level_);
            proto->set_gem(gem_);
            proto->set_pos(pos_);
            proto->set_type(type_);
            proto->set_color(color_);
            proto->set_refining(refining_);
			proto->set_refining_exp(refining_exp_);
        }
        
        inline void copy_to_equip_res(pro_equip_res_ptr res){
            proEquip * proto = new proEquip;
            READ_LOCK(lock, mutex_);
            res->set_allocated_equip(proto);
            proto->set_id(id_);
            proto->set_sid(sid_);
            proto->set_exp(exp_);
            proto->set_level(level_);
            proto->set_gem(gem_);
            proto->set_pos(pos_);
            proto->set_type(type_);
            proto->set_color(color_);
            proto->set_refining(refining_);
			proto->set_refining_exp(refining_exp_);
        }
        
    private:
        uint64_t id_;
        uint32_t sid_;
        uint32_t exp_;
        uint16_t level_;
        uint32_t gem_;
        uint32_t pos_;
        uint8_t type_;
        uint8_t color_;
        uint16_t refining_;
		uint32_t refining_exp_;
        
        boost::shared_mutex mutex_;
    };
}

#endif /* defined(__cache__equip__) */

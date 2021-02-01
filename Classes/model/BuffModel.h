//
//  BuffModel.h
//  buff信息
//
//  Created by wanghejun on 13-5-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BuffModel__
#define __QSMY__BuffModel__

#include "Model.h"
#include "../AppMacros.h"

#define DEFAULT_BUFF    0   //默认找不到的资源编号

class BuffModel;


namespace buff
{
    
    enum BuffType
    {
        KBuffTypeVertigo = 1025
    };
    
    class Buff:public ValueObjcet
    {
        //CC_SYNTHESIZE_READONLY(uint32_t, sid, Sid);
        CC_SYNTHESIZE(float, m_nDuration, Duration);
    public:
        
        uint32_t sid;
        const uint32_t getSid()
        {
            return sid;
        }
        
        Buff()
        :skeletonName("")
        ,sid(0)
        ,m_nDuration(0.f)
        {
            
        }
        
        CREATE_FUNC(Buff);
        D_INIT_DEFAULT(ValueObjcet);
        
        virtual ~Buff()
        {
        }
        
        friend class ::BuffModel;
        
        
        const char* getSkeletonName()
        {
            if(skeletonName.compare("")==0) skeletonName = cocos2d::CCString::createWithFormat("BUFF_%d",sid)->getCString();
            return skeletonName.c_str();
        }
    private:
        std::string skeletonName;
    };
}


class BuffModel : public Model
{
    typedef cocos2d::CCDictionary BuffMap;
    
public:
    
    BuffModel();
    virtual ~BuffModel();
    virtual bool init();
    void loadData();
    void parseInfo(const CSVDecoder::CSV &val);
    
    
    buff::Buff* getBuff(uint32_t sid)
    {
        return (buff::Buff*)m_Buffs->objectForKey(sid);
    }
    
private:
    BuffMap *m_Buffs;
};


#endif /* defined(__QSMY__BuffModel__) */

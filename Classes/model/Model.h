//
//  Model.h
//  QSMY
//
//  Created by wanghejun on 13-3-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Model__
#define __QSMY__Model__

#include "json/json.h"
#include "AppInclude.h"
#include "../utils/Notification.h"
#include "../AppEventType.h"
#include "../utils/CSVDecoder.h"
#include "../AppMacros.h"

class Model:public cocos2d::CCObject,public Notification
{
    CC_SYNTHESIZE_READONLY(bool, m_bInited, Inited);
public:
    virtual bool init();
    //virtual bool parse(const Json::Value &val)=0;
    Model();
    virtual ~Model(){}
    bool isSynced;                 //是否跟后台同步
    
    virtual void reset();
};


class ValueObjcet:public cocos2d::CCObject,public Notification
{
    
public:
    bool isSynced;                 //是否跟后台同步

    ValueObjcet():isSynced(false){};
    virtual ~ValueObjcet(){};
    D_INIT_DEFAULT(Notification);
    virtual void reset();
};



#endif /* defined(__QSMY__Model__) */

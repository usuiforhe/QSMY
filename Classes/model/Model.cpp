//
//  Model.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Model.h"


Model::Model()
:m_bInited(false)
,isSynced(false)
{
    
};

bool Model::init()
{
    if(!Notification::init()) return false;
    m_bInited = true;
    postNotification(D_EVENT_INIT_MODEL);
    return true;
}

void Model::reset()
{
    isSynced = false;
}

void ValueObjcet::reset()
{
    isSynced = false;
}
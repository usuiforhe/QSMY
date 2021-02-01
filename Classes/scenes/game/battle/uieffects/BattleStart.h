//
//  BattleStart.h
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BattleStart__
#define __QSMY__BattleStart__

#include "AppInclude.h"
#include "components/components.h"
#include "DragonBone.h"

class BattleStart : public DEffectNode
{
public:
    CREATE_FUNC(BattleStart);
    virtual bool init();
    
    void animateEnd(sp::Armature * target, const char* event,const char* data);

private:
    void menuCallBack(cocos2d::CCObject* obj);
    void close();
};

#endif /* defined(__QSMY__BattleStart__) */

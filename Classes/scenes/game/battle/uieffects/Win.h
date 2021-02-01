//
//  Win.h
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Win__
#define __QSMY__Win__

#include "AppInclude.h"
#include "components/components.h"
#include "DragonBone.h"

class Win : public DEffectNode
{
public:
    static Win* create(uint8_t score);
    virtual bool init(uint8_t score);
    
    void animateEnd(sp::Armature * target, const char* event,const char* data);

private:
    void menuCallBack(cocos2d::CCObject* obj);
    void close();
};


#endif /* defined(__QSMY__Win__) */

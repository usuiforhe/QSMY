//
//  DropWonderEffect.h
//  QSMY
//
//  Created by wanghejun on 13-8-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DropWonderEffect__
#define __QSMY__DropWonderEffect__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SettlementModel.h"
#include "DragonBone.h"

class DropWonderEffect : public DEffectNode
{
public:
    static DropWonderEffect* create(settlement::WonderInfo*);
    virtual bool init(settlement::WonderInfo*);
    
    void animateEnd(sp::Armature * target, const char* event,const char* data);
};



#endif /* defined(__QSMY__DropWonderEffect__) */

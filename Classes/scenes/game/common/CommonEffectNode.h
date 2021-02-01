//
//  CommonEffectNode.h
//  QSMY
//
//  Created by wanghejun on 13-11-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__CommonEffectNode__
#define __QSMY__CommonEffectNode__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SettlementModel.h"
#include "DragonBone.h"

class CommonEffectNode : public DEffectNode
{
public:
    static CommonEffectNode* create(sp::Armature*);
    virtual bool init(sp::Armature*);
private:
    void animateEnd(sp::Armature * target, const char* event,const char* data);
};

#endif /* defined(__QSMY__CommonEffectNode__) */

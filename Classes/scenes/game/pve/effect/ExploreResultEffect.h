//
//  ExploreResultEffect.h
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ExploreResultEffect__
#define __QSMY__ExploreResultEffect__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SettlementModel.h"
#include "DragonBone.h"

class ExploreResultEffect : public DEffectNode
{
public:
    static ExploreResultEffect* create(settlement::SettlementInfoPve*);
    virtual bool init(settlement::SettlementInfoPve*);
    void animateEnd(sp::Armature * target, const char* event,const char* data);
};


#endif /* defined(__QSMY__ExploreResultEffect__) */

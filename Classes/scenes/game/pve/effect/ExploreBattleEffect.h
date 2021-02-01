//
//  ExploreBattleEffect.h
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ExploreBattleEffect__
#define __QSMY__ExploreBattleEffect__

#include "AppInclude.h"
#include "components/components.h"
#include "model/PveModel.h"
#include "DragonBone.h"

class ExploreBattleEffect : public DEffectNode
{
public:
    static ExploreBattleEffect* create(pve::MapInfo*);
    virtual bool init(pve::MapInfo*);
    
    void animateEnd(sp::Armature * target, const char* event,const char* data);
};


#endif /* defined(__QSMY__ExploreBattleEffect__) */

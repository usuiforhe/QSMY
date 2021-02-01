//
//  ExploreEffect.h
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ExploreEffect__
#define __QSMY__ExploreEffect__

#include "AppInclude.h"
#include "components/components.h"
#include "model/PveModel.h"
#include "DragonBone.h"

class ExploreEffect : public DEffectNode
{
public:
    ExploreEffect();
    virtual ~ExploreEffect();
    
    static ExploreEffect* create(pve::MapInfo*);
    virtual bool init(pve::MapInfo*);
    void animateEnd(sp::Armature * target, const char* event,const char* data);
    
    void setOffset(const CCPoint &offset,float scale);
    
private:
    CCSprite *m_pNode;
};


#endif /* defined(__QSMY__ExploreEffect__) */

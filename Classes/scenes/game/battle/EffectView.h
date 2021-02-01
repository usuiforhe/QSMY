//
//  EffectView.h
//  QSMY
//  单位普攻打中效果
//
//  Created by wanghejun on 13-5-25.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EffectView__
#define __QSMY__EffectView__


#include "AppInclude.h"
#include "../../../model/BattleModel.h"
#include "DragonBone.h"
#include "../../../model/SkillModel.h"
#include "../../../managers/GameManager.h"


/**
 *	@brief	技能动画
 */
class UnitEffect:public cocos2d::CCNode/*, public sigslot::has_slots<>*/
{
    
public:
    
    UnitEffect();
    virtual ~UnitEffect();
    
    static UnitEffect* create(battle::ResultDamage* pResultDamage,float delay);
    
    virtual float getDuration();
    
    /**
     * Sets the Z order which stands for the drawing order, and reorder this node in its parent's children array.
     *
     * The Z order of node is relative to its "brothers": children of the same parent.
     * It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in cocos2d.
     * The larger number it is, the later this node will be drawn in each message loop.
     * Please refer to setVertexZ(float) for the difference.
     *
     * @param nZOrder   Z order of this node.
     */
    virtual void setZOrder(int zOrder);
    /**
     * Gets the Z order of this node.
     *
     * @see setZOrder(int)
     *
     * @return The Z order.
     */
    virtual int getZOrder();
    
protected:
    void startAnimation();
    virtual bool init(battle::ResultDamage* pResultDamage,float delay);
    void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    
protected:
    sp::Armature * m_pArmature;
    float m_fDelay;
	std::string m_sEffectName;
	uint32_t m_nWarriorID;
};


#endif /* defined(__QSMY__EffectView__) */

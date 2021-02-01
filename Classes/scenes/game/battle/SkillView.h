//
//  SkillView.h
//  QSMY
//
//  Created by wanghejun on 13-3-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SkillView__
#define __QSMY__SkillView__

#include "AppInclude.h"
#include "../../../model/BattleModel.h"
#include "DragonBone.h"
#include "../../../model/SkillModel.h"
#include "../../../managers/GameManager.h"
#include "../../../components/components.h"


#pragma mark SkillView
/**
 *	@brief	技能动画
 */
class SkillView:public cocos2d::CCNode//, public sigslot::has_slots<>
{
    
public:
    SkillView();
    virtual ~SkillView();
    
    static SkillView* create(uint32_t skill_id,float delay);
    
    virtual float getDuration();
    
protected:
    void startAnimation();
    virtual bool init(uint32_t skill_id,float delay);
    void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    
protected:
    sp::Armature * m_pArmature;
    float m_fDelay;
	uint32_t m_nSkillID;
};

#pragma mark SkillEffect
/**
 *	@brief	技能动画
 */
class SkillEffect:public cocos2d::CCNode//, public sigslot::has_slots<>
{
    
public:
    SkillEffect();
    virtual ~SkillEffect();
    
    static SkillEffect* create(uint32_t skill_id,float delay);    
    virtual float getDuration();
    
protected:
    void startAnimation();
    virtual bool init(uint32_t skill_id,float delay);
    void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    
protected:
    sp::Armature * m_pArmature;
	float m_fDelay;
	uint32_t m_nSkillID;
};


#pragma mark SkillUnitEffect
/**
 *	@brief	技能动画
 */
class SkillUnitEffect:public cocos2d::CCNode//, public sigslot::has_slots<>
{
    
public:
    SkillUnitEffect();
    virtual ~SkillUnitEffect();
    
    static SkillUnitEffect* create(battle::Skill *pSkill,float delay);
    
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
    virtual bool init(battle::Skill *pSkill,float delay);
    void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    
protected:
    sp::Armature * m_pArmature;
    float m_fDelay;
    uint8_t unit_effect_id;
	uint32_t m_nSkillID;
};


#pragma mark SkillCover

class SkillCover:public cocos2d::CCNode//, public sigslot::has_slots<>
{
    
public:
    SkillCover();
    virtual ~SkillCover();
    
    static SkillCover* create(battle::ActionSkill *pAction,float delay);    //创建遮罩
    virtual float getDuration()=0;  //持续时间
    
    virtual void show();
    virtual void finish();
    virtual void hideCover();
protected:
    virtual bool init(battle::ActionSkill *pAction,float delay);
    virtual void showSkillName();
    
protected:
    cocos2d::CCLayerColor *m_pLayer;
    float m_fDelay;
    std::string skillName;
    skill::SkillColorConfig *mSkillColor;
    sp::Armature * m_pArmature;
};

class SkillCoverOne:public SkillCover
{
    
public:
    SkillCoverOne(){}
    virtual ~SkillCoverOne(){}
protected:
    virtual bool init(battle::ActionSkill *pAction,float delay);
    virtual float getDuration();
    
};

class SkillCoverTwo:public SkillCover
{
    
public:
    SkillCoverTwo(){}
    virtual ~SkillCoverTwo(){}
protected:
    virtual bool init(battle::ActionSkill *pAction,float delay);
    virtual float getDuration();
    
};


class SkillCoverThree:public SkillCover
{
    
public:
    SkillCoverThree(){}
    virtual ~SkillCoverThree(){}
protected:
    virtual bool init(battle::ActionSkill *pAction,float delay);
    virtual float getDuration();
    
};


class SkillCoverFour:public SkillCover
{
    
public:
    SkillCoverFour(){}
    virtual ~SkillCoverFour(){}
protected:
    virtual bool init(battle::ActionSkill *pAction,float delay);
    virtual float getDuration();
    
};


class SkillCoverUnit:public SkillCover
{

public:
    SkillCoverUnit()
    :m_bFlipX(false)
    {
    }
    
    virtual ~SkillCoverUnit(){}
    static SkillCoverUnit* create(battle::ActionSkill *pAction,float delay,bool flipX);    //创建遮罩
protected:
    virtual bool init(battle::ActionSkill *pAction,float delay,bool flipX);
    virtual float getDuration();
    virtual void showSkillName();
private:
    bool    m_bFlipX;
};

#endif /* defined(__QSMY__SkillView__) */

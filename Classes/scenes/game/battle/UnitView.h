//
//  UnitView.h
//  QSMY
//
//  Created by wanghejun on 13-3-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UnitView__
#define __QSMY__UnitView__

#include "AppInclude.h"
#include "model/BattleModel.h"
#include "components/components.h"
#include "DragonBone.h"

class BattleView;


class UnitView : public cocos2d::CCNode, /*public sigslot::has_slots<>,*/public Notification
{
    
public:
    
    static const char *ATTACK;          //普通攻击
    static const char *STAND;           //站立
    static const char *HARD_STRAIGHT;   //被攻击
    static const char *DEATH;           //死亡
    static const char *SKILL;           //小技能攻击
    static const char *PARRY;           //格挡
    static const char *DODGE;           //闪避
    static const char *VERTIGO;         //眩晕
    
    static const char *CONJURE_BEGIN;   //开始施法
    static const char *CONJURE;         //施法
    static const char *CONJURE_END;     //结束施法
    
public:
    
    
    static UnitView* create(battle::BattleUnit *pUnit);
   
    UnitView();
    virtual ~UnitView();
    
    
    /**
     *	@brief	播放一个动作
     *
     *	@param 	name 	动作名称
     *	@param 	delay
     *  @param  forceChange 是否强制切换动作 如果传入动作和当前动作一致是否打断当前动作
     *  @param 	fixeRate  两个动作之间的融合率 这个系数会乘以该动作实际需要时间
     *
     *	@return	返回该动作的持续时间和延迟时间的和
     */
    float playAction(const char* name,float delay = 0.0f,bool forceChange = false,float fixeRate = 1.0f);      // 返回该动作的持续时间
    
    float getActionDuration(const char* name); //获取该单位某个动作的持续时间
    
    float playAddBuff(uint32_t id,float delay);    // 播放增加buff动画
    float playBuffAction(uint32_t id,float delay); //播放buff动作
	float playFateAddBuff(uint32_t id,float delay);    // 播放缘分增加buff动画
    
    float playSkillCover(battle::ActionSkill *pAction,float delay = 0.0f);  //播放技能描述开场
    
    /**
     *	@brief	跳数字动画
     *
     *	@param 	pResult  伤害信息
     *	@param 	delay    延迟时间
     */
    void playNum(battle::Result *pResult,float delay);
    
    
    /**
     *	@brief	播放单位技能击中效果
     *
     *	@param 	pSkill  技能信息
     *	@param 	delay 	延迟时间
     *
     *	@return	技能效果持续时间和延迟时间的和
     */
    float playUnitEffect(battle::Skill *pSkill,float delay);
    
    
    /**
     *	@brief	播放单位普攻击中效果
     *
     *	@param 	pSkill  技能信息
     *	@param 	delay 	延迟时间
     *
     *	@return	技能效果持续时间和延迟时间的和
     */
    float playUnitEffect(battle::ResultDamage* pResultDamage,float delay);
    
    
    /**
     *	@brief	一个单位退场
     *
     *	@param 	delay 	延迟时间
     *
     *	@return	退场需要时间和延迟时间的和
     */
    float playHide(float delay);
    
    
    
    /**
     *	@brief	一个单位上场
     *
     *	@param 	delay 	延迟时间
     *
     *	@return	上场需要时间和延迟时间的和
     */
    float playShow(float delay);
    
    
    /**
     * 获取显示坐标
     */
    static cocos2d::CCPoint positionToPoint(int pos,int count);
    
    int getHp();
    void setHp(int value,float delay);
    
    
    battle::BattleUnit * getData()
    {
        return m_pUnit;
    }
    
	void setDead()
	{
		m_bDead = true;
	}
    
    
    bool isDead()
    {
        return m_bDead;
    }
    
	void removeBuffEffect();    
	void hideUnit();
	void showUnit();
private:
    bool init(battle::BattleUnit *pUnit);
    void initView();
    
    void setHp(CCObject*);
    
    void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    void onNumEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    
    void playAction(cocos2d::CCObject *name);
    
    void playNum(cocos2d::CCObject *pResult);
    void showName();
    //void doPlayEffect(uint32_t skill_id);
    
    void playHide();
    void playShow();
    void onHideEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    void onShowEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
    void onComplete(sp::Armature *_armature, const char *_eventType, const char *_movementID);

    
    bool isActiveAction(const char*); //是否是主动动作 攻击 技能等 需要单位主动出击 说明该单位没有在（或结束）晕眩状态
    void playArmature(cocos2d::CCObject*);
private:
    battle::BattleUnit *m_pUnit; //weak;
    sp::Armature *m_ani;
    sp::Armature *m_pName;
    DProgressBar *m_pHp;
    sp::Armature *m_pBossMark;
    bool m_bDead;
    //int32_t zOrder;
    int m_iHp;
    bool isVertigo;//是否晕眩状态

	cocos2d::CCArray  *m_pBuffArray;
    
};

#endif /* defined(__QSMY__UnitView__) */

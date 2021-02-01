//
//  Battle.h
//  战斗模块
//
//  Created by wanghejun on 13-3-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Battle__
#define __QSMY__Battle__

#include "AppInclude.h"
#include "components/components.h"
#include "model/BattleModel.h"
#include "BattleView.h"
#include "model/BuffModel.h"
#include "./uieffects/BattleStart.h"
#include "./uieffects/Lose.h"
#include "./uieffects/Win.h"
#include "../common/LoadingLayer.h"

class SettlementView;

class SettlementDelegate
{
public:
    virtual ~SettlementDelegate(){}
    virtual void quitBattle()=0;		//退出战斗
	virtual void replayBattle()=0;		//重播
	virtual void showOtherFormation(unsigned int tid = 0){};/*默认0是机器人*/
};


class Battle
:public DLayer
,public BattleDelegate
,public SettlementDelegate
,public LoadingLayerDelegate
{
    CC_SYNTHESIZE_READONLY(battle::SkipBattleType, m_kSkipType, SkipType);
public:
    Battle();
    ~Battle();
    
    virtual void skipBattle();		//跳过战斗
    virtual void endBattle();		//结束战斗
    virtual void quitBattle();		//退出战斗
    virtual void replayBattle();	//重播战斗
	virtual void beforeStartBattle();   //开始战斗之前
	//查看对手阵容
	virtual void showOtherFormation(unsigned int tid);
	virtual void setSkipType(battle::SkipBattleType type){
		m_kPrevSkipType = type;
		m_kSkipType = type;
	}

    virtual bool init();
    CREATE_FUNC(Battle);
    
#pragma mark - battle -
private:
    
    bool canSkipBattle();
    
    void startBattle(CCObject*);
    void startBattle();             //开始战斗
    void endBattle(CCObject*);      //结束战斗
    void playNextAction();          //播放下一条战报
    void playNextAction(float delay);   //播放下一条战报
    void playAction(battle::Action *pAction);    //播放战报
    void playSkill(battle::ActionSkill *pAction);       //播放技能
    void playAttack(battle::ActionAttack * pAction);   //播放普攻
    void playBuff(battle::ActionBuff * pAction);         //播放buff
    
    void resetSkillAction(battle::ActionSkill *pAction,float delay);                 //重置技能动作
    void resetSkillAction(CCObject* obj);                            //重置技能动作
    /**
     *	@brief	播放技能结果
     *
     *	@param 	pResult 	结果消息信息
     *	@param 	delay 	延迟时间
     *
     *	@return	返回（输入延迟时间） 和 (结果本身持续时间）的和
     */
    float playSkillResult(battle::Skill *pSkill,battle::Result *pResult,float delay);
	/**
	*	@brief	播放攻击结果
	*
	*	@param 	pResult 	结果消息信息
	*	@param 	delay 	延迟时间
	*
	*	@return	返回（输入延迟时间） 和 (结果本身持续时间）的和
	*/
	float playAttackResult(battle::Result *pResult,float delay);
    
    /**
     *	@brief	播放伤害
     *
     *	@param 	pResult 	伤害消息信息
     *	@param 	delay 	延迟时间
     *
     *	@return	返回（输入延迟时间） 和 (伤害本身持续时间）的和
     */
    float playSkillDamage(battle::Skill *pSkill,battle::ResultDamage *pResult,float delay);
    
    
    float playSkillShowUnit(battle::Skill *pSkill,battle::ResultShow*pResult,float delay);

    float playSkillHideUnit(battle::Skill *pSkill,battle::ResultHide*pResult,float delay);

    float playSkillDialog(battle::Skill *pSkill,battle::ResultDialog*pResult,float delay);

    
    /**
     *	@brief	播放伤害
     *
     *	@param 	pResult 	伤害消息信息
     *	@param 	delay 	延迟时间
     *
     *	@return	返回（输入延迟时间） 和 (伤害本身持续时间）的和
     */
    float playDamage(battle::ResultDamage *pResult,float delay);
    
    
    
    float playSkillHeal(battle::Skill *pSkill,battle::ResultHeal *pResult,float delay);
    
        /**
     *	@brief	播放治疗
     *
     *	@param 	pResult 	治疗消息信息
     *	@param 	delay 	延迟时间
     *
     *	@return	返回（输入延迟时间） 和 (治疗本身持续时间）的和
     */
    float playHeal(battle::ResultHeal *pResult,float delay);

	void playSound(cocos2d::CCObject*);
    
    /**
     *	@brief	播放增加buff
     *
     *	@param 	pResult 	<#pResult description#>
     *	@param 	delay 	<#delay description#>
     *
     *	@return	<#return value description#>
     */
    float playAddBuff(battle::Skill *pSkill,battle::ResultAddBuff *pResult,float delay);

	/**
     *	@brief	播放增加buff
     *
     *	@param 	pResult 	<#pResult description#>
     *	@param 	delay 	<#delay description#>
     *
     *	@return	<#return value description#>
     */
    float playAddBuff(battle::ResultAddBuff *pResult,float delay);

    /**
     *	@brief	播放去掉buff
     *
     *	@param 	result 	<#result description#>
     *	@param 	delay 	<#delay description#>
     *
     *	@return	<#return value description#>
     */
    float playRemoveBuff(battle::ResultRemoveBuff *result,float delay);

    
    void initBattleResource();  //初始化战场资源
    void loadBattleResource();
    void releaseResource();
    
    void loadingCallBack();
    void showLoading();
    void hideLoading();
    
    void playNextDialog();
    void playUnitDialog();
    void playDialog(battle::BattleDialogInfo* pInfo);

	void startFateEffect();             //开始播放缘分加成动画
    
private:
    BattleModel         *m_pBattleModel;    //weak
    WarriorModel        *m_pUnitModel;    //weak
    BuffModel           *m_pBuffModel;   //weak
    BattleView          *m_pView;
    uint32_t            currentActionIndex;
    uint32_t            currentDialogIndex;
    int                 m_nNumberOfLoadedSprites;
    int                 m_nNumberOfSprites;
    LoadingLayer        *m_pLoading;
    
    cocos2d::CCArray    *m_pDialogs;

#pragma mark - settlement -
private:
    void showSettlement();  //显示战后结算
	void showBusyLayer();  

	SettlementView* m_pSettle;

	void keyBackClicked();

public:
	virtual void loadingLayerTouched(LoadingLayer *pLayer);
private:
	void touchToBegin();
};

#endif /* defined(__QSMY__Battle__) */

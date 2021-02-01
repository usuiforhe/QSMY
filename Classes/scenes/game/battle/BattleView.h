//
//  BattleView.h
//  QSMY
//
//  Created by wanghejun on 13-3-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BattleView__
#define __QSMY__BattleView__

#include "AppInclude.h"
#include "../common/common.h"
#include "model/BattleModel.h"
#include "model/PveModel.h"
#include "model/WarriorModel.h"


#define BATTLE_BLOOD_STAR 3
class UnitView;

class BattleDelegate
{
	CC_SYNTHESIZE_READONLY(battle::SkipBattleType, m_kPrevSkipType, PrevSkipType);
public:
    virtual ~BattleDelegate(){}
    virtual void skipBattle()=0; //跳过战斗
    virtual void endBattle()=0;  //结束战斗    
    virtual void playNextDialog() = 0;  //播放下一条对话
	virtual void beforeStartBattle() = 0;   //开始战斗之前
};


class BattleView
: public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE(BattleDelegate *, m_pDelegate, Delegate);
public:
    BattleView();
    virtual ~BattleView();
    CREATE_FUNC(BattleView);
    virtual  bool init();
    
    void reset();
    
    UnitView* getUnit(uint8_t id); //获取一个单位
    
    float playSkillCover(battle::ActionSkill *pAction,float delay = 0.0f);  //播放技能描述开场
    float playSkill(battle::ActionSkill *pAction,float delay = 0.0f);  //播放技能动画
    float playSkillEffect(battle::ActionSkill *pAction,float delay = 0.0f);  //播放技能效果动画
    
    void updatePower();
    
    void playEffect(DEffectNode* pEffect);
    
    void createSkipButton(bool canSkip);
    
    void playDialog(battle::BattleDialogInfo* pInfo);
    void hideDialog();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void aniEnd(sp::Armature * target, const char* event,const char* data);
	void removeStar(uint8_t grade=0);
	void playFateEffect();
	void playSideFateEffect(cocos2d::CCObject*);
	void showUnits(bool bShow);
	void onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID);
	void onComplete(sp::Armature *_armature, const char *_eventType, const char *_movementID);
	void playTitle();
	void playFateNum(cocos2d::CCObject*);
	void startBattle();
private:
    BattleModel                 *m_pModel;
    PveModel                    *m_pPveModel;
    WarriorModel                *m_pWarriorModel;
    cocos2d::CCDictionary       *units;
    cocos2d::CCLayer            *m_pUnitLayer;
    cocos2d::CCLayer            *m_pUnitUILayer;
    DEffectLayer                *m_pEffectLayer;    //播放动画效果层
    
    DProgressBar                *m_PowerLeft;    //左边的战斗力
    DProgressBar                *m_PowerRight;   //右边的战斗力
    
    cocos2d::CCNode             *m_pPowerAniLeft;
    cocos2d::CCNode             *m_pPowerAniRight;
    
    cocos2d::CCNode             *m_EffectNodeLeft;
    cocos2d::CCNode             *m_EffectNodeRight;
    
        
    cocos2d::CCLabelTTF         *m_pNameLeft;
    cocos2d::CCLabelTTF         *m_pNameRight;
    
	cocos2d::CCLabelBMFont         *m_pSceneName;
	cocos2d::CCRichLabelTTF     *m_pRichSceneName;
    cocos2d::CCNode             *m_pSceneNode;
    
	cocos2d::CCMenuItem        *m_pSpeedUp;
    cocos2d::CCNode         *m_p1x;
    cocos2d::CCNode         *m_p2x;

	cocos2d::CCMenuItem *m_pSkipMenu;
    
    
    
    uint64_t  m_dPowerLeftTotal;  //左边总战斗力
    uint64_t  m_dPowerRightTotal; //右边总战斗力

	float  m_dLastPercentLeft;  //左边上次剩余战斗力百分比
	float  m_dLastPercentRight; //右边上次剩余战斗力百分比

	cocos2d::CCPoint m_pBattleGridCenter;
	cocos2d::CCNode* m_pGridCenter;
    
    UIAnimate* m_pBloodStar[BATTLE_BLOOD_STAR];
	UIAnimate* m_pRightBloodStar[BATTLE_BLOOD_STAR];

	cocos2d::CCNode         *m_pFateNode;

	bool m_bUnitInited;
	cocos2d::CCArray* m_pArmatureArr1;//底动画
	cocos2d::CCArray* m_pArmatureArr2;//人动画
	cocos2d::CCArray* m_pArmatureArr3;//攻防血动画

	cocos2d::CCArray* m_pLeftUnits;//左边战斗单位
	cocos2d::CCArray* m_pRightUnits;//右边战斗单位
#pragma mark - dialog -
    StoryTalk             *m_pDialog;
    
private:
    void toggleSpeed(cocos2d::CCObject* sender);
    /**
     *	@brief	是否加速播放
     *
     *	@param 	val 	是否加速播放
     */
    void updateSpeedUp();

    
    void skipBattle(cocos2d::CCObject *sender);
    void testPause(cocos2d::CCObject *sender);
    
    void nextDialog(cocos2d::CCObject *sender);
    
    void initBattleInfo();  //初始化战场信息
    void initUnits();           //初始化战斗单位
    //void initSkipButton();         //初始化跳过按钮
    void playArmature(cocos2d::CCObject*);
};

#endif /* defined(__QSMY__BattleView__) */

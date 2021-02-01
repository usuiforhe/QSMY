//
//  TowerDifficulty.h
//  千机楼-困难选择
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TowerDifficulty__
#define __QSMY__TowerDifficulty__
#include "AppInclude.h"
#include "../../../components/components.h"
#include "Tower.h"
#include "json/json.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define POINT_COUNT 5

class TowerModel;
class TowerDifficulty : public DLayer, public cocos2d::extension::CCBMemberVariableAssigner, public cocos2d::extension::CCBSelectorResolver, public TowerChildDelegate
{      
public:
	TowerDifficulty();
	~TowerDifficulty();
	virtual bool init();
	CREATE_FUNC(TowerDifficulty);

	virtual void onEnter();
	virtual void onExit();

	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
	virtual void UpdateInfo();
public:
	void NotificationUpdate();
private:
	void onSelectFight(CCObject*);
private:
	CCLabelBMFont*			m_MissionName;		// 关卡num
	CCMenuItemImage*	m_btnHardFight;		//困难攻击 onHardFight
	CCLabelBMFont*		m_HardReward;		//奖励数值
	CCScale9Sprite*		m_hardBackground;	//奖励道具的背景 visible
	CCNode*				m_HardItem;			//奖励道具图标
	CCLabelBMFont*		m_HardInteral;		//奖励积分

	CCMenuItemImage*	m_btnNormalFight;	//普通攻击 onNormalFight
	CCLabelBMFont*		m_NormalReward;		//奖励数值
	CCScale9Sprite*		m_NormalBackground;	//奖励道具的背景 visible
	CCNode*				m_NormalItem;		//奖励道具图标
	CCLabelBMFont*		m_NormalInteral;	//奖励积分

	CCMenuItemImage*	m_btnEasyFight;		//简单攻击 onEasyFight
	CCLabelBMFont*		m_EasyReward;		//奖励数值
	CCScale9Sprite*     m_EasyBackground;	//奖励道具的背景 visible
	CCNode*				m_EasyItem;			//奖励道具图标
	CCLabelBMFont*		m_EasyInteral;		//奖励数值

	CCMenuItemImage*			m_point[POINT_COUNT];
	CCNode*			m_tips_[POINT_COUNT];

	CCLabelTTF*			m_pass;				//本关可以跳过战斗
	CCSprite*			m_unPass;			//不可以跳过战斗

	CCLabelBMFont*		m_attackAdd;		//当前攻击buff
	CCLabelBMFont*		m_defenseAdd;		//当前防御buff
	CCLabelBMFont*		m_hpAdd;			//当前血量buff

	CCLabelBMFont*		m_HavePower;		//已有能量值
	WarriorGradeBg*     m_NormalItemBg;
	WarriorGradeBg*     m_EasyItemBg;
	WarriorGradeBg*     m_HardItemBg;
private:
	TowerModel*			m_pTowerModel;
	TowerDelegate*		m_pDelegate;
public:
	virtual void SetDelegate(TowerDelegate* pDelegate){m_pDelegate = pDelegate;}
};
#endif /* defined(__QSMY__TowerDifficulty__) */

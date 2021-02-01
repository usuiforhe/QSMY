//
//  TowerReward.h
//  千机楼-奖励
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TowerReward__
#define __QSMY__TowerReward__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "Tower.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TowerModel;
class TowerReward : public DLayer, public cocos2d::extension::CCBMemberVariableAssigner, public cocos2d::extension::CCBSelectorResolver, public TowerChildDelegate
{      
public:
	TowerReward();
	~TowerReward();
	virtual bool init();
	CREATE_FUNC(TowerReward);

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
	void onTouchBack(CCObject*);
private:
	CCNode*				m_WinAni;
	CCNode*				m_item1Icon;
	CCNode*				m_item2Icon;
	CCLabelTTF*			m_itemName1;
	CCLabelTTF*			m_itemName2;
	CCLabelBMFont*		m_GetMoney;
	CCMenuItemImage*	m_btnBack;
private:
	TowerModel*			m_pTowerModel;
	TowerDelegate*		m_pDelegate;
public:
	virtual void SetDelegate(TowerDelegate* pDelegate){m_pDelegate = pDelegate;}
};

#endif /* defined(__QSMY__TowerReward__) */

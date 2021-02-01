//
//  TowerHome.h
//  千机楼-首页
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TowerHome__
#define __QSMY__TowerHome__

#include "AppInclude.h"
#include "components/components.h"
#include "Tower.h"
#include "json/json.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TowerModel;
class TowerHome : public DLayer, public cocos2d::extension::CCBMemberVariableAssigner, public cocos2d::extension::CCBSelectorResolver, public TowerChildDelegate
{      
public:
	TowerHome();
	~TowerHome();
	virtual bool init();
	CREATE_FUNC(TowerHome);

	virtual void onEnter();
	virtual void onExit();

	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
	virtual void UpdateInfo();
public:
	void NotificationUpdate();
	void StartResult(const Json::Value &requestData, const Json::Value &responseData);
private:
	void onSelectFight(CCObject*);
private:
	CCMenuItemImage*	m_StartFight;
	CCLabelBMFont*		m_todayNum;				//今日可挑战
	CCLabelBMFont*		m_WarriorTodayValue;	//上阵弟子全属性
	CCLabelBMFont*		m_todayValue;			//今日最佳成绩
	CCLabelBMFont*		m_WarriorTomValue;		//次日上阵弟子全属性
private:
	TowerModel*			m_pTowerModel;
	TowerDelegate*		m_pDelegate;
public:
	virtual void SetDelegate(TowerDelegate* pDelegate){m_pDelegate = pDelegate;}
};

#endif /* defined(__QSMY__TowerHome__) */

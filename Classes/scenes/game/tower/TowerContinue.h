//
//  TowerContinue.h
//  千机楼-失败继续
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#ifndef __QSMY__TowerContinue__
#define __QSMY__TowerContinue__
#include "AppInclude.h"
#include "../../../components/components.h"
#include "Tower.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define POINT_COUNT 5
class TowerModel;
class TowerContinue : public DLayer, public cocos2d::extension::CCBMemberVariableAssigner, public cocos2d::extension::CCBSelectorResolver,
	public TowerChildDelegate
{      
public:
	TowerContinue();
	~TowerContinue();
	virtual bool init();
	CREATE_FUNC(TowerContinue);

	virtual void onEnter();
	virtual void onExit();

	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
	virtual void UpdateInfo();
public:
	void NotificationUpdate();
	void ContinueResult(const Json::Value &requestData, const Json::Value &responseData);
private:
	void onContinue(CCObject*);
private:
	CCLabelBMFont*			m_MissionName;

	CCNode*				m_Icon;
	CCNode*				m_HaveIcon;
	CCLabelBMFont*		m_Money;
	CCLabelBMFont*		m_Buff;
	CCLabelBMFont*		m_RebirthCounts;
	CCLabelBMFont*		m_CanRebirthCounts;
	CCLabelBMFont*		m_VipLev;
	DButton*	        m_btnContinueCancel;
	DButton*	        m_btnContinueOk;

	CCLabelBMFont*		m_HavePower;

	CCNode*				m_pPopupLayer;


	CCMenuItem*			m_point[POINT_COUNT];
	CCNode*			m_tips_[POINT_COUNT];

	CCLabelBMFont*		m_attackAdd;		//当前攻击buff
	CCLabelBMFont*		m_defenseAdd;		//当前防御buff
	CCLabelBMFont*		m_hpAdd;			//当前血量buff

	DButton			    *m_btnReplay;
	CCNode				*m_pNodeReplay;
private:
	TowerModel*			m_pTowerModel;
	TowerDelegate*		m_pDelegate;
public:
	virtual void SetDelegate(TowerDelegate* pDelegate){m_pDelegate = pDelegate;}

	void replayCallBack(CCObject *sender);
};
#endif /* defined(__QSMY__TowerContinue__) */

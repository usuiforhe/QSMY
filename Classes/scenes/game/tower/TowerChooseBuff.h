//
//  TowerChooseBuff.h
//  千机楼-选择buff
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TowerChooseBuff__
#define __QSMY__TowerChooseBuff__
#include "AppInclude.h"
#include "../../../components/components.h"
#include "Tower.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TowerModel;
class TowerChooseBuff : public DLayer, public cocos2d::extension::CCBMemberVariableAssigner, public cocos2d::extension::CCBSelectorResolver, public TowerChildDelegate
{      
public:
	TowerChooseBuff();
	~TowerChooseBuff();
	virtual bool init();
	CREATE_FUNC(TowerChooseBuff);

	virtual void onEnter();
	virtual void onExit();

	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
	virtual void UpdateInfo();
public:
	void NotificationUpdate();
	void ChooseResult(const Json::Value &requestData, const Json::Value &responseData);
private:
	void onChoose(CCObject*);
private:
	CCLabelBMFont*			m_MissionName;		// 关卡num

	CCNode*				m_BuffType[3];		//攻防血
	CCLabelBMFont*		m_UseValue[3];		//消耗数值
	CCLabelBMFont*		m_BuffValue[3];		//攻防血百分比
	CCMenuItemImage*	m_btnBuffChoose[3];	//选择
	
	CCLabelBMFont*		m_HavePower;		//已有能量值

	CCLabelBMFont*		m_attackAdd;		//当前攻击buff
	CCLabelBMFont*		m_defenseAdd;		//当前防御buff
	CCLabelBMFont*		m_hpAdd;			//当前血量buff


private:
	TowerModel*			m_pTowerModel;
	TowerDelegate*		m_pDelegate;
	int					m_pUseValue;//扣除能量
public:
	virtual void SetDelegate(TowerDelegate* pDelegate){m_pDelegate = pDelegate;}
};

#endif /* defined(__QSMY__TowerChooseBuff__) */

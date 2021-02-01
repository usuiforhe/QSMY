//
//  Tower.h
//  千机楼
//
//  Created by ldr123 on 13-07-01.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Tower__
#define __QSMY__Tower__

#include "AppInclude.h"
#include "components/components.h"
#include "../common/OtherFormationView.h"
#include "model/TowerModel.h"
#include "../Game.h"


class TowerModel;
class TowerDelegate
{
public:
	virtual void NavigateTo(tower::NavigationType navType) = 0;
	virtual void ClosePopup() = 0;
    virtual void showOtherFormation(otherFormation::FormationInfo* pInfo) = 0;
};

class TowerChildDelegate
{
public:
	virtual void SetDelegate(TowerDelegate* pDelegate) = 0;
};

class Tower
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public TowerDelegate
,public DPopupDelegate
{       
public:
	Tower();
	~Tower();
	virtual bool init();
	CREATE_FUNC(Tower);

	virtual void onEnter();
	virtual void onExit();

    virtual void showOtherFormation(otherFormation::FormationInfo* pInfo);
    virtual void onPopupClose(DPopup*);
    
    void theRoundEnd(cocos2d::CCObject*obj);
    
	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
public:
	virtual void NavigateTo(tower::NavigationType navType);
	virtual void ClosePopup();

public:
	void NotificationUpdate();
private:
    
    void forceUpdate(float dt);
	void onTouchTowerRank(CCObject *sender);
	void onTouchTowerInteral(CCObject *sender);
private:
	tower::NavigationType	m_currentShow;
	tower::NavigationType	m_currentPop;
	CCNode*					m_pContainer;
	CCLayerColor*			m_pPopLayer;
	TowerModel*				m_pTowerModel;
	TowerChildDelegate*		m_pLastLayer;//weak
	CCLabelBMFont*			m_pInteral;
public:
	virtual void onBackKey();
    
};

#endif /* defined(__QSMY__Tower__) */

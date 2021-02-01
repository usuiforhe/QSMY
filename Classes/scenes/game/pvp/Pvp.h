//
//  Pvp.h
//  论剑模块
//
//  Created by cll on 13-4-20.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Pvp__
#define __QSMY__Pvp__

#include "AppInclude.h"
#include "model/PvpModel.h"
#include "components/components.h"
#include "../Game.h"
#include "../common/OtherFormationView.h"
#include "../inventory/InventoryDelegate.h"

class NavigationDelegate;

class OtherFormationView;
class ItemInfoView;
class PvpInfoView;

struct PvpDelegate
:public InventoryDelegate
{
public:
    virtual void showOtherFormation(otherFormation::FormationInfo* ) =0;
	virtual void willChallenge(pvp::ChTargetInfo * m_pTargetInfo, bool isBoss = false, bool isRobot = false) = 0;
	virtual void showTargetInfo(pvp::ChTargetInfo * pInfo) = 0;
	virtual void slideBackGround(float offset){	};
	virtual void showAwardInfo(pvp::ChMallItem * pAwardData)=0;
};

class Pvp:
public DLayer,
public PvpDelegate,
public cocos2d::extension::CCBMemberVariableAssigner
,public DPopupDelegate
{
public:
	Pvp();
	~Pvp();

	static Pvp* create(NavigationDelegate* pDelegate);
	virtual bool init(NavigationDelegate* pDelegate);

    virtual void onPopupClose(DPopup*);
    
	virtual void onEnter();
	virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	void showOtherFormation(unsigned int target_id);
	void showOtherFormation(otherFormation::FormationInfo* );
	
    virtual void willChallenge(pvp::ChTargetInfo * m_pTargetInfo, bool isBoss = false, bool isRobot = false);
	virtual void showTargetInfo(pvp::ChTargetInfo *pInfo);
	virtual void slideBackGround(float offset);
    
    void updateView();

	void menuCallback(cocos2d::CCObject * sender);
private:
    void createView();
	//virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
    //virtual void closeInfoView(InventoryType type);
	//显示奖励详情
	virtual void showAwardInfo(pvp::ChMallItem * pAwardData);

private:
    CCNode  *m_pContainer;
    //ItemInfoView * m_pItemInfoView;

	NavigationDelegate  *m_pDelegate;
    
//     cocos2d::CCLabelBMFont * m_pPoint;
//     cocos2d::CCLabelBMFont * m_pRemainChTimes;
//     cocos2d::CCLabelBMFont * m_pPointGrow;

	cocos2d::CCMenuItem	*m_pBtnRank;
	cocos2d::CCMenuItem *m_pBtnExchange;

	void callBackTab(cocos2d::CCObject *pObj);

	void showRankView();
	void showExchangeView();

	CCNode *m_pPvpView;
	CCNode *m_pExchangeView;
	CCNode *m_pNodeBg;

	CCLabelBMFont *m_pBMFCoins;
	CCLabelBMFont *m_pBMFPoints;
	CCLabelBMFont *m_pBMFFight;

	DProgressBar *m_pPBEndu;
	cocos2d::CCLabelBMFont *m_pBMFEndu;

	bool bFirstEnter;
};

#endif /* defined(__QSMY__Pvp__) */

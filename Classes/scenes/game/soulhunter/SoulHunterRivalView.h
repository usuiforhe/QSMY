//
// SoulHunterRivalView.h
//  狩魂选择对手
//
//  Created by liuxiaogang on 13-10-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterRivalView__
#define __QSMY__SoulHunterRivalView__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/ResourceManager.h"
#include "model/SoulHunterModel.h"
#include "model/WarriorModel.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "SoulHunter.h"

class SoulHunterWarriorView;

#pragma mark -SoulHunterRivalDelegate-
class SoulHunterRivalDelegate
{
public:
	virtual void battle() = 0;

};

#pragma mark -SoulHunterRivalView-
class SoulHunterRivalView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
	CC_SYNTHESIZE(SoulHunterDelegate*, m_pDelegate, Delegate);
public:
	SoulHunterRivalView();
	~SoulHunterRivalView();
	virtual bool init();
	static SoulHunterRivalView*  create();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell){};
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	void setCurWid(uint32_t wid);
	void updateRivals();
	void refreshRivals();
	void setData(warrior::WarriorCurInfo * pWarrior);
	void RefreshRivalsCallBack(cocos2d::CCObject* sender);
	void closeCallBack(cocos2d::CCObject* sender);
	void onSnatchClick(cocos2d::CCObject *sender);
private:
	SoulHunterWarriorView				*m_pTargetWarrior;
	DTableView							*m_pListView;
	cocos2d::extension::CCData			*m_pData;
	cocos2d::CCSprite					*m_pNoRivalSprite;
	cocos2d::CCNode						*m_pRivalListContainer;				//对手列表容器
	cocos2d::CCMenuItem					*m_pRefreshRivalMenuItem;
	cocos2d::CCMenuItem					*m_pCloseRivalMenuItem;
	SoulHunterModel						*m_pModel;
	uint32_t							m_pcurWid;
};




#pragma mark -----------SoulHunterRivalCell------------
class SoulHunterRivalCell
: public cocos2d::extension::CCTableViewCell
, public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pSnatchButton, ButtonSnatch);			//抢夺按钮
public:
	SoulHunterRivalCell();
    
	~SoulHunterRivalCell();
    
    static SoulHunterRivalCell * create(cocos2d::extension::CCData* pData);
    
	virtual bool init(cocos2d::extension::CCData* pData);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setData(soulhunter::SoulHunterRivalInfo *rival);
    
protected:
   
	cocos2d::CCNode			* m_pContainer;		//对手队长头像
	cocos2d::CCLabelTTF		* m_pName;			//对手名字
	cocos2d::CCLabelBMFont	* m_pLevel;			//对手等级
	//cocos2d::CCMenuItem		*m_pSnatchButton;	//抢夺按钮
	CC_SYNTHESIZE_READONLY(soulhunter::SoulHunterRivalInfo*, m_pRivalInfo,RivalInfo);
};


#endif /* defined(__QSMY__SoulHunterRivalView__) */

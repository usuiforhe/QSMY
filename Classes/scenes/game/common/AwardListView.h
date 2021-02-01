//
//  AwardListView.h
//  多个奖品列表
//
//  Created by cll on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AwardListView__
#define __QSMY__AwardListView__

#include "AppInclude.h"
#include "common.h"


class AwardItemSmall;
class AwardListViewCell;

struct AwardViewFactory
{
	static DPopup * create(cocos2d::CCArray	* pAwardList);
};


class BaseAwardListView
:public DPopup
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
public:
	BaseAwardListView();
	virtual ~BaseAwardListView();
    
	virtual bool init(cocos2d::CCArray * pAwardList);
    
    virtual bool init();
    
	virtual void onEnter();
	virtual void onExit();
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);     

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};

	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};

protected:
    virtual void addChildrenBeforeList() = 0;
	virtual void updateAwardListView();
protected:
	cocos2d::CCArray			*m_pAwardList;
    DTableView                  *m_pListView;
    cocos2d::extension::CCData  *m_pCCData;
	cocos2d::CCNode             *m_pContainer;
};

class AwardListView
:public BaseAwardListView
{
    
public:
	AwardListView();
	
	virtual ~AwardListView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

    virtual bool init(cocos2d::CCArray * pAwardList);

protected:
    
    virtual void addChildrenBeforeList();
	void closeCallBack(cocos2d::CCObject*obj);
	void shareCallBack(cocos2d::CCObject*obj);

	DButton				*m_pConfirmButton;
	DButton				*m_pCloseButton;
    cocos2d::CCNode     *m_pEffectNode;

	cocos2d::CCSprite		*m_pTitleCommon;
	cocos2d::CCSprite		*m_pTitleSoul;
};




class AwardListViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
    
	AwardListViewCell();
    ~AwardListViewCell();

	static AwardListViewCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);
	void setAwardItems(cocos2d::CCArray * pAwardItems);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
  

private:
	AwardItemSmall		*m_pItem1Node;
	AwardItemSmall		*m_pItem2Node;
	AwardItemSmall		*m_pItem3Node;
	AwardItemSmall		*m_pItem4Node;
	AwardItemSmall		*m_pItem5Node;

	cocos2d::CCArray    *m_pAwardItems;

};



#endif /* defined(__QSMY__AwardListView__) */

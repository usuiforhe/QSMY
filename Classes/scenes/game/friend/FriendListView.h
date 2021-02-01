//
//  FriendListView.h
//  商品列表
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FRIENDLISTVIEW__
#define __QSMY__FRIENDLISTVIEW__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"

class FriendListDelegate
{
public:
	virtual void showMenu(unsigned int fid) = 0;
	virtual void toggleView(bool val) = 0;
};

class FriendListView
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{

public:
	FriendListView();
	~FriendListView();

	static FriendListView* create(FriendListDelegate* pDelegate , const cocos2d::CCSize& size , const cocos2d::CCPoint& pos);

	virtual bool init(FriendListDelegate* pDelegate , const cocos2d::CCSize& size , const cocos2d::CCPoint& pos);

	virtual void onEnter();
	virtual void onExit();

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	void setDelegate(FriendListDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

private:
	
	void updateFriendListView();    
	/*void buyMenuCallback(CCObject * obj);
	void buyCallback(const Json::Value & requestData,const Json::Value &responseData);*/



private:

	cocos2d::extension::CCData  *m_pData;
	DTableView                  *m_pListView;
	cocos2d::CCArray			m_pDataList;

private:
	FriendModel						*m_pModel;
	FriendListDelegate			*m_pDelegate;
};




class FriendListCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:

	FriendListCell();
	virtual ~FriendListCell();

	static FriendListCell* create(cocos2d::extension::CCData* pData);

	virtual bool init(cocos2d::extension::CCData* pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	
	void setFriendInfo(partner::FriendInfo*);

	void updateCell();

	void setDelegate(FriendListDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

private:
	void onMenu(cocos2d::CCObject*);
	void sendGift(cocos2d::CCObject*);
	
private:
	unsigned int m_uFriendID;
	FriendListDelegate *m_pDelegate;
	partner::FriendInfo		*m_pFriendInfo;

private:
	cocos2d::CCMenuItemImage	*m_pMenu;
	cocos2d::CCMenuItemImage	*m_pGift;
	cocos2d::CCLabelTTF		*m_pName;
	cocos2d::CCRichLabelTTF		*m_pTime;
	cocos2d::CCLabelBMFont	*m_pFight;
	cocos2d::CCLabelBMFont	*m_pLevel;
	cocos2d::CCNode		*m_pCanSend;
	cocos2d::CCNode		*m_pCantSend;
};



#endif /* defined(__QSMY__FRIENDLISTVIEW__) */

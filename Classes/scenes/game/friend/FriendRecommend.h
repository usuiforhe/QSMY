//
//  FriendRecommend.h
//  商品列表
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FRIENDRECOMMEND__
#define __QSMY__FRIENDRECOMMEND__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"


class FriendRecommend
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCEditBoxDelegate
{

public:
	FriendRecommend();
	~FriendRecommend();

	static FriendRecommend* create(const cocos2d::CCSize& size);

	virtual bool init(const cocos2d::CCSize& size);

	virtual void onEnter();
	virtual void onExit();

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox){};

	void doMore(CCObject*);
	void doIgnoreAll(CCObject*);
	void doSearch(CCObject*);
private:
	void updateView();
	void initEditBox();
	void onSearch(const Json::Value& requestData,const Json::Value& responseData);
	/*void buyMenuCallback(CCObject * obj);
	void buyCallback(const Json::Value & requestData,const Json::Value &responseData);*/

private:
	cocos2d::extension::CCData	*m_pData;
	FriendModel							*m_pModel;
	cocos2d::CCArray					m_pDataList;

private:
	DTableView								*m_pListView;
	cocos2d::extension::CCEditBox	*m_pEditBox;

private:
	DButton		*m_pMenuItemMore;
	DButton		*m_pMenuItemIgnore;
	DButton										*m_pBtnSearch;
	cocos2d::CCNode						*m_pPageContainer;
	cocos2d::CCNode						*m_pBoxContainer;
	cocos2d::CCLabelTTF					*m_pMyID;
};


enum FriendCellType
{
	kFriendCellRecommend = 0,
	kFriendCellGift = 1,
};

class FriendRecommendCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	FriendRecommendCell();
	virtual ~FriendRecommendCell();
	static FriendRecommendCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);	
	void setFriendBasic(partner::FriendBasic*);
	//申请好友
	void doApply(cocos2d::CCObject*);
	//接受请求
	void doAgree(cocos2d::CCObject*);
	//拒绝请求
	void doRefuse(cocos2d::CCObject*);
	//领取耐力
	void doAward(cocos2d::CCObject*);

	FriendCellType m_type;

private:

	void updateCell();

	unsigned int m_nFriendID;

	cocos2d::CCNode				*m_pNodeApply;
	DButton						*m_pBtnApply;
	cocos2d::CCNode				*m_pNodeAccept;
	DButton						*m_pBtnAccept;
	cocos2d::CCNode				*m_pNodeAward;
	DButton						*m_pBtnAward;
	DButton						*m_pBtnIgnore;

	cocos2d::CCLabelTTF		*m_pName;
	cocos2d::CCRichLabelTTF		*m_pTime;
	cocos2d::CCLabelBMFont	*m_pFight;
	cocos2d::CCLabelBMFont	*m_pLevel;

	partner::FriendBasic		*m_pBasic;
};



#endif /* defined(__QSMY__FRIENDRECOMMEND__) */

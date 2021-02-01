//
//  UserDetail.h
//  用户详情
//
//  Created by Eci on 13-12-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UserDetail__
#define __QSMY__UserDetail__

#include "AppInclude.h"
#include "../Game.h"
#include "components/components.h"
#include "model/UserModel.h"
#include "../account/VipView.h"


class UserDetail;

class UserDetailDelegate
{
public:
	virtual void willShowVipView(UserDetail *pDetailView) = 0;
};

class UserDetail
	: public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{      
public:
	UserDetail();
	virtual ~UserDetail();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void countDownUpdate(float dt);

	CREATE_FUNC(UserDetail);
    virtual void closePopup();
    
    //D_POPUP_PRIORITY(DPOPUP_USER_LEVELUP_PRIORITY);

	virtual bool									 onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) {};
	virtual void scrollViewDidScroll(CCScrollView* view) {};
	virtual void scrollViewDidZoom(CCScrollView* view) {};
	//void userPrivileges(const Json::Value &requestData,const Json::Value &responseData);
public:
	void updateInfo();
	void updatePrivileges();
	void updatePrivileges(float dt);

protected:
	CC_SYNTHESIZE(UserDetailDelegate *, m_pDelegate, Delegate);
private:
	cocos2d::CCLabelTTF *m_pLabelUserName;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelPoints;
	cocos2d::CCLabelBMFont *m_pLabelCoins;
	cocos2d::CCLabelBMFont *m_pLabelPower;
	cocos2d::CCLabelBMFont *m_pLabelVip;
	cocos2d::CCLabelBMFont *m_pLabelPowerFull;
	cocos2d::CCLabelBMFont *m_pLabelPowerRecover;

	cocos2d::CCLabelBMFont *m_pLabelEndu;
	cocos2d::CCLabelBMFont *m_pLabelEnduFull;
	cocos2d::CCLabelBMFont *m_pLabelEnduRecover;
	DButton							*m_pCloseButton;
	cocos2d::CCMenuItem *m_pButtonRename;
	cocos2d::CCMenuItem *m_pButtonVip;
	cocos2d::CCNode		*m_pContainer;
	cocos2d::CCArray *m_pPrivileges;
	cocos2d::CCNode *m_pNodePrivileges;
	DTableView *m_pTableView;

	void callBackClose(CCObject *sender);
	void callBackRename(CCObject *sender);
	void callBackVip(CCObject *sender);

	user::UserInfo *m_pUserInfo;
};

class UserDetailPrivilegeCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	UserDetailPrivilegeCell();
	virtual ~UserDetailPrivilegeCell();
	CREATE_FUNC(UserDetailPrivilegeCell);
	virtual bool init();
	void setData(user::Privilege *pData);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
protected:
	CC_SYNTHESIZE_READONLY(user::Privilege *, m_pData, Data);
private:
	cocos2d::CCLabelBMFont *m_pEndTs;
	cocos2d::CCLabelTTF *m_pContents;
};

#endif /* defined(__QSMY__UserDetail__) */

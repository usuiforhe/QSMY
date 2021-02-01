//
//  Signin.h
//  成就页面
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__Signin__
#define __QSMY__Signin__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "SigninCell.h"

class Signin
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public SigninDelegate
{
public:
    
    Signin();
    virtual ~Signin();
	CREATE_FUNC(Signin);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) {};
	virtual void scrollViewDidScroll(CCScrollView* view) {};
	virtual void scrollViewDidZoom(CCScrollView* view) {};

    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
	//delegate
	virtual void doSignin(SigninCell *pCell);
	virtual void signinCallBack(){
		closePopup();
	};

	void signinCallBack(const Json::Value &request, const Json::Value &response);

	virtual void onEnter();
	virtual void onExit();

private:
	void closeCallBack(CCObject* obj);
	void updateView();

private:
	CCArray *m_pArr;
	SigninModel *m_pModel;
	DTableView *m_pTableView;
	unsigned int m_uTodayIdx;

private:
    DButton	*m_pCloseButton;
	CCNode	*m_pCell;
	CCNode	*m_pTableContainer;
	ItemBox	*m_pSignItem0;
	ItemBox	*m_pSignItem1;
	CCNode	*m_pThree;
	CCNode	*m_pSeven;
};

#endif /* defined(__QSMY__Signin__) */

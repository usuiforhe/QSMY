//
//  SigninCell.h
//  成就页面
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__SigninCell__
#define __QSMY__SigninCell__

#define MAX_DAY	7
#define ITEM_COUNT	2

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../common/gui/ItemBox.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"

class SigninCell;

class SigninDelegate
{
public:
	virtual void doSignin(SigninCell *pCell) = 0;
	virtual void signinCallBack() = 0;
};

class SigninCell
:public CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(SigninDelegate *, m_pDelegate, Delegate);
public:
    
    SigninCell();
    virtual ~SigninCell();
	CREATE_FUNC(SigninCell);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void setData(signin::SigninAward *pData);
	void setSigned(bool isSigned);
	void setTomorrow(bool tomorrow);
	void setTodaySigned(bool todaySigned);
	void setClear();
	//播放签到动画
	void play();
	void playEnd();
private:
	cocos2d::CCNode *m_pNodeDay[MAX_DAY];
	cocos2d::CCNode *m_pNodeDouble;
	cocos2d::CCNode *m_pNodeSignin;
	cocos2d::CCNode *m_pNodeSigned;
	cocos2d::CCNode *m_pNodeTomorrow;
	cocos2d::CCNode *m_pNodeTodaySigned;
	ItemBox *m_pItem[ITEM_COUNT];
	cocos2d::CCNode *m_pLight;

	cocos2d::CCNode *m_pTodayText;
	cocos2d::CCNode *m_pTodayImg;

	void callBackSignin(cocos2d::CCObject *pObj);
};

#endif /* defined(__QSMY__SigninCell__) */

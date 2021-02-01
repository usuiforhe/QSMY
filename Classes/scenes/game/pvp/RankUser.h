//
//  RankUser.h
//  排行用户
//
//  Created by Eci on 14-7-7.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__RankUser__
#define __QSMY__RankUser__

#include "AppInclude.h"
#include "components/DProgressBar.h"
#include "managers/DataManager.h"

struct PvpDelegate;

class RankUser
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(PvpDelegate *, m_pDelegate, Delegate);
public:
    CREATE_FUNC(RankUser);
    RankUser();
    virtual ~RankUser();
    D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {return NULL;};

	void setTargetInfo(pvp::ChTargetInfo * pInfo);

protected:
	pvp::ChTargetInfo * m_pInfo;

private:
	cocos2d::CCLabelBMFont *m_pBMFRank;
	cocos2d::CCLabelTTF *m_pTTFName;
	cocos2d::CCLabelTTF *m_pTTFPoint;
	cocos2d::CCNode *m_pNodeHead;

	cocos2d::CCNode *m_pNodeTop;
	cocos2d::CCNode *m_pNodeBottom;

	cocos2d::CCNode *m_pMyStone;
	cocos2d::CCNode *m_pOthersStone;

	void updateView();

	void callBackTouch(CCObject *pObj);
};


class RankUserLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(RankUserLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(RankUser);
};

#endif /* defined(__QSMY__RankUser__) */

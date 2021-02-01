//
//  AchievementCell.h
//  成就页面
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__AchievementCell__
#define __QSMY__AchievementCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class AchievementDelegate {
public:
	virtual void onView(unsigned int id , drop::DropType type) = 0;
};

class AchievementCell
:public CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
public:
    
    AchievementCell();
    virtual ~AchievementCell();
	CREATE_FUNC(AchievementCell);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void setData(achievement::TaskInfo *pInfo);
	void getAward(cocos2d::CCObject*);
	void onView(cocos2d::CCObject*);
	void setDelegate(AchievementDelegate* pDelegate){
		m_pDelegate = pDelegate;
	};
private:
	cocos2d::CCLabelBMFont *m_pBMFCount;
	cocos2d::CCLabelTTF	*m_pTTFDes;
	cocos2d::CCLabelTTF *m_pTTFName;
	cocos2d::CCNode *m_pIconContainer;
	cocos2d::CCLabelBMFont *m_pBMFAchPoint;
	cocos2d::CCNode *m_pNodeInvalid;
	cocos2d::CCNode *m_pNodeValid;

	uint32_t m_taskID;

	unsigned int m_uItemID;
	AchievementDelegate* m_pDelegate;
};

#endif /* defined(__QSMY__AchievementCell__) */

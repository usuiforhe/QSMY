//
//  PveReward.h
//  pve奖励
//
//  Created by ldr123 on 13-7-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PveReward__
#define __QSMY__PveReward__

#include "AppInclude.h"
#include "managers/GameManager.h"
#include "model/PveModel.h"
#include "model/UserModel.h"
#include "RewardItemBox.h"
#include "Pve.h"
#include "../common/common.h"

#define PRES_AWARD_COUNT 3

class PveReward :
			public cocos2d::CCLayer,
			public cocos2d::extension::CCBMemberVariableAssigner, 
			public cocos2d::extension::CCBSelectorResolver
{
public:
    PveReward();
    ~PveReward();
    virtual bool init();
    CREATE_FUNC(PveReward);

	virtual bool										onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler					onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler	onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName) {return NULL;}
    
	void UpdateInfo(pve::ChapterInfo *pChapterInfo, PveRewardDelegate* pDelegate);
private:
    void updateInfo();
	void onTouchGet(cocos2d::CCObject *pObj);
	void onTouchBack(cocos2d::CCObject *pObj);    
private:
	cocos2d::CCLabelTTF*		m_ChapterName;
	cocos2d::CCLabelTTF*		m_pLblPlan;
    RewardItemBox*              m_itemIcon[PRES_AWARD_COUNT];
    DButton*                    m_btnGet[PRES_AWARD_COUNT];
	cocos2d::CCNode				*m_pTxtGet[PRES_AWARD_COUNT];
	cocos2d::CCNode				*m_pTxtGot[PRES_AWARD_COUNT];
private:
	PveRewardDelegate*			m_pDelegate;
    pve::ChapterInfo            *m_pChapterInfo;
    
};

#endif /* defined(__QSMY__PveReward__) */

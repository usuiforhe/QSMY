//
//  OnlineAward.h
//  在线奖励
//
//  Created by fengkerong on 14-10-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ONLINEAWARD__
#define __QSMY__ONLINEAWARD__

#include "AppInclude.h"
#include "../common/common.h"


#define AWARD_COUNT 4

class EquipInfoView;
class ItemInfoView;
class OnlineAward
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	OnlineAward();
	virtual ~OnlineAward();
	CREATE_FUNC(OnlineAward);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void onExit();
protected:
	//按钮回调
	void confirmCallBack(cocos2d::CCObject * obj);
	void closeCallBack(cocos2d::CCObject *obj);
	//请求回调
	void onOnlineAward(const Json::Value &requestData,const Json::Value &responseData);
	//countdown
	void refreshOnlineAward(float dt);
private:

	cocos2d::CCNode		*m_pHeroContainer;
	cocos2d::CCNode		*m_pGradeContainer;
	cocos2d::CCNode     *m_pPopupLayer;
	cocos2d::CCLabelTTF	*m_pTime;
	cocos2d::CCNode		*m_pTimeUp;
	ItemInfoView            *m_pItemInfoView;

	warrior::WarriorBaseInfo * pWarriorBase;

	AwardItemSmall		*m_pAward[AWARD_COUNT];

	DButton		*m_pCloseButton;
	DButton		*m_pConfirmButton;
};

#endif /* defined(__QSMY__ONLINEAWARD__) */

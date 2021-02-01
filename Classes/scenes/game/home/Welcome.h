//
//  LoginReward.h
//  登陆奖励
//
//  Created by ldr123 on 13-07-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WELCOME__
#define __QSMY__WELCOME__

#include "AppInclude.h"
#include "../../../components/components.h"

class Welcome : 
						public DPopup, 
						public cocos2d::extension::CCBMemberVariableAssigner
{      
public:
	Welcome();
	virtual ~Welcome();
	virtual bool init();
	CREATE_FUNC(Welcome);

	virtual bool									 onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
public:
	void onTouchBack(cocos2d::CCObject* p);
	void onBackKey();
private:
	cocos2d::CCMenuItem*			m_pMenuItem;
	cocos2d::CCNode*					m_pIconContainer;
	cocos2d::CCRichLabelTTF*	m_pContent;
};
#endif /* defined(__QSMY__WELCOME__) */

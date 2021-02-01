//
//  CustomerService.h
//  兑换
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__CustomerService__
#define __QSMY__CustomerService__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"

class CustomerService
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    CustomerService();
    virtual ~CustomerService();
    CREATE_FUNC(CustomerService);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
private:
	cocos2d::CCNode *m_pContainer;
	DButton *m_pButtonSubmit;
	cocos2d::extension::CCEditBox *m_pEditBox;
	void callBackSubmit(CCObject *sender);
	void submitCallBack(const Json::Value &requestData, const Json::Value &responseData);
};

#endif /* defined(__QSMY__CustomerService__) */

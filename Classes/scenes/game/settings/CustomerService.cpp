//
//  CustomerService.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "CustomerService.h"

CustomerService::CustomerService()
	:m_pContainer(NULL)
	,m_pButtonSubmit(NULL)
	,m_pEditBox(NULL)
{
    
}

CustomerService::~CustomerService()
{
   CC_SAFE_RELEASE(m_pContainer);
   CC_SAFE_RELEASE(m_pButtonSubmit);
   CC_SAFE_RELEASE(m_pEditBox);
}

bool CustomerService::init()
{
    if(!DLayer::init()) return false;
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shezhi/pl_shezhi_duihuan");
	m_pButtonSubmit->getButton()->setTarget(this, menu_selector(CustomerService::callBackSubmit));
	m_pEditBox = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
	CC_SAFE_RETAIN(m_pEditBox);
	m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_14);
	m_pEditBox->setFontColor(ccWHITE);
	m_pEditBox->setPlaceHolder(D_LOCAL_STRING("ConvertCodeTips").c_str());
	m_pEditBox->setPlaceholderFontColor(ccWHITE);
	m_pEditBox->setMaxLength(50);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setAnchorPoint(CCPointZero);
	m_pContainer->addChild(m_pEditBox);
    return true;
}

bool CustomerService::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonSubmit", DButton *, this->m_pButtonSubmit);
	return false;
}

void CustomerService::callBackSubmit(CCObject *sender)
{
	if(strlen(m_pEditBox->getText())==0)
	{
		Tips_Alert(D_LOCAL_STRING("ConvertCodeError").c_str());
		return;
	}
	HTTP_CLIENT->exchangeCode(m_pEditBox->getText(),this,callfuncJson_selector(CustomerService::submitCallBack));
	m_pEditBox->setText("");
}

void CustomerService::submitCallBack(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CLOSE_SETTING, (CCObject*)NULL);
	
}

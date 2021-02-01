//
//  StrategyExchange.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "StrategyExchange.h"


USING_NS_CC;
USING_NS_CC_EXT;

StrategyExchange::StrategyExchange()
:m_pSubmitButton(NULL)
,m_pContainer(NULL)
,m_pEditBox(NULL)
{
    
}

StrategyExchange::~StrategyExchange()
{
    CC_SAFE_RELEASE(m_pSubmitButton);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pEditBox);
}

bool StrategyExchange::init()
{
    if(!DLayer::init()) return false;
    CCB_READER_AND_ADDCHILD("ui/ccb/strategy/strategyInput");
    m_pSubmitButton->getButton()->setTarget(this, menu_selector(StrategyExchange::submitCallBack));
    
    m_pEditBox = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
    CC_SAFE_RETAIN(m_pEditBox);
    m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_14);
    m_pEditBox->setFontColor(ccBLACK);
    m_pEditBox->setPlaceHolder(D_LOCAL_STRING("ConvertCodeTips").c_str());
    m_pEditBox->setPlaceholderFontColor(ccBLACK);
    m_pEditBox->setMaxLength(50);
    m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBox->setAnchorPoint(CCPointZero);
    
    m_pContainer->addChild(m_pEditBox);
    return true;
}

void StrategyExchange::submitCallBack(cocos2d::CCObject* obj)
{
    if(strlen(m_pEditBox->getText())==0)
    {
        Tips_Alert(D_LOCAL_STRING("ConvertCodeError").c_str());
        return;
    }
    HTTP_CLIENT->exchangeCode(m_pEditBox->getText(),this,callfuncJson_selector(StrategyExchange::submitCallBack));
    m_pEditBox->setText("");
}

void StrategyExchange::submitCallBack(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
}

bool StrategyExchange::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton", DButton *, this->m_pSubmitButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    return false;
}

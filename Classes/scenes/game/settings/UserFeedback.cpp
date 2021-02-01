//
//  UserFeedback.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UserFeedback.h"
#include "managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define MAX_CHARS 102


static int _calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
MyTextFieldTTF * MyTextFieldTTF::textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize)
{
    MyTextFieldTTF *pRet = new MyTextFieldTTF();
    //CCSize size(dimensions.width+30,dimensions.height+30);
    if(pRet && pRet->initWithSizeAndBackgroundSprite(dimensions,CCScale9Sprite::create()))
    {
        pRet->setPlaceHolder(placeholder);
        pRet->setFont(fontName, fontSize);
        pRet->setMaxLength(MAX_CHARS);
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void MyTextFieldTTF::setString(const char *label)
{
	EditBox::setText(label);
    //DEditBox::setText(label);
}

const char * MyTextFieldTTF::getString()
{
	return EditBox::getText();
    //return DEditBox::getText();
}

bool MyTextFieldTTF::attachWithIME()
{
    touchDownAction(NULL,0);
    return true;
}


// #else
// 
// 
// const char * MyTextFieldTTF::getString()
// {
// 	return CCEditBox::getText();
// }
// 
// MyTextFieldTTF * MyTextFieldTTF::textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize)
// {
//     MyTextFieldTTF *pRet = new MyTextFieldTTF();
// 	//if(pRet && pRet->initWithPlaceHolder("", dimensions, alignment, fontName, fontSize))
// 	if(pRet && pRet->initWithSizeAndBackgroundSprite(dimensions,CCScale9Sprite::create()))
//     {
//         if (placeholder)
//         {
//             pRet->setPlaceHolder(placeholder);
//         }
//         pRet->autorelease();
//         return pRet;
//     }
//     CC_SAFE_DELETE(pRet);
//     return NULL;
// }
// 
// void MyTextFieldTTF::setString(const char *label)
// {
//     CCEditBox::setText(label);
// //     CC_SAFE_DELETE(m_pInputText);
// //     
// //     if (label)
// //     {
// //         m_pInputText = new std::string(label);
// //     }
// //     else
// //     {
// //         m_pInputText = new std::string;
// //     }
// //     
// //     m_nCharCount = _calcCharCount(m_pInputText->c_str());
// //     
// //     if (m_nCharCount>MAX_CHARS) {
// //         deleteBackward();
// //         return;
// //     }
// //     
// //     CCTextFieldTTF::setString(label);
// }
// 
// void MyTextFieldTTF::setFontColor(const cocos2d::ccColor3B&color3)
// {
//     CCEditBox::setFontColor(color3);
// }
// 
// #endif

#pragma mark -

UserFeedback::UserFeedback()
:m_pSend(NULL)
,m_pContainer(NULL)
,m_pTextField(NULL)
,m_pReset(NULL)
,m_pNodeService(NULL)
{
    
}

UserFeedback::~UserFeedback()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pSend);
    CC_SAFE_RELEASE(m_pTextField);
    CC_SAFE_RELEASE(m_pReset);
	CC_SAFE_RELEASE(m_pNodeService);
}

bool UserFeedback::init()
{
    if(!DLayer::init()) return false;
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shezhi/pl_shezhi_kefu");
    m_pSend->getButton()->setTarget(this, menu_selector(UserFeedback::sendCallBack));
    m_pReset->getButton()->setTarget(this,menu_selector(UserFeedback::resetCallBack));
    
    m_pTextField = MyTextFieldTTF::textFieldWithPlaceHolder(D_LOCAL_STRING("write").c_str(),
                                                            m_pContainer->getContentSize(),
                                                            kCCTextAlignmentLeft,
                                                            D_FONT_DEFAULT,
                                                            D_FONT_SIZE_11);
    m_pTextField->setFontColor(ccWHITE);
    m_pTextField->setAnchorPoint(CCPointZero);
	m_pContainer->addChild(m_pTextField);
	m_pTextField->setDEditBoxContainer(m_pContainer);
    //m_pTextField->setDelegate(this);
    CC_SAFE_RETAIN(m_pTextField);
    
	m_pNodeService->setVisible(!DM_GET_PLATFORM_MODEL->getConfig().getIsHideService());

    return true;
}

bool UserFeedback::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSend", DButton *, this->m_pSend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReset", DButton *, this->m_pReset);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeService", CCNode *, this->m_pNodeService);
    return false;
}

SEL_MenuHandler UserFeedback::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "inputCallBack", UserFeedback::inputCallBack);
    return NULL;
}

void UserFeedback::onExit()
{
    DLayer::onExit();
    reset();
}

void UserFeedback::resetCallBack(cocos2d::CCObject*)
{
    reset();
}


void UserFeedback::reset()
{
    m_pTextField->setString("");
}


void UserFeedback::sendCallBack(cocos2d::CCObject*)
{
    std::string content = m_pTextField->getString();
    if(_calcCharCount(content.c_str()) < 10)
    {
        Tips_Alert(D_LOCAL_STRING("shortWrite").c_str());
        inputCallBack(NULL);
        return;
    }
    HTTP_CLIENT->addFeedBack(content.c_str(),this,callfuncJson_selector(UserFeedback::sendCallBack));
}

void UserFeedback::sendCallBack(const Json::Value &,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    Tips_Alert(D_LOCAL_STRING("send").c_str());
    reset();
}

void UserFeedback::inputCallBack(cocos2d::CCObject*)
{
    m_pTextField->attachWithIME();
}
//
//  InputName.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "InputName.h"

#include "InputName.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define MAX_CHARS 10

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

InputName::InputName()
:m_pSubmitButton(NULL)
,m_pContainer(NULL)
{

}

InputName::~InputName()
{
    CC_SAFE_RELEASE(m_pSubmitButton);
    CC_SAFE_RELEASE(m_pContainer);
}

void InputName::finishStep()
{
    if(strlen(m_pName->getText())<=0)
    {
        Tips_Error(D_LOCAL_STRING("UsernameCantBlank").c_str());
        return;
    }
    HTTP_CLIENT->finishRookie(m_pStepInfo->getID(),m_pName->getText());
}


void InputName::loadUI()
{
    //CCB_READER_AND_ADDCHILD("ui/ccb/tutorial/tutorialNamed");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_xinshou/pl_xinshou_nicheng");
    
    m_pName = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
    m_pName->setFont(D_FONT_DEFAULT,D_FONT_SIZE_14);
    m_pName->setFontColor(ccWHITE);
    m_pName->setPlaceHolder(D_LOCAL_STRING("Username").c_str());
    m_pName->setPlaceholderFontColor(ccWHITE);
    m_pName->setMaxLength(MAX_CHARS*5);
    m_pName->setReturnType(kKeyboardReturnTypeDone);
    m_pName->setAnchorPoint(CCPointZero);
    m_pName->setDelegate(this);
    
    m_pContainer->addChild(m_pName);
        
    m_pSubmitButton->getButton()->setTarget(this, menu_selector(InputName::submitCallBack));
    
    randNameCallBack(NULL);
    m_sContentId = CCString::createWithFormat("%d",m_pStepInfo->getID() * 10 + m_nCurrentSubStep)->getCString();
}


bool InputName::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton", DButton*, this->m_pSubmitButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
    return StepEnForce::onAssignCCBMemberVariable(pTarget, pMemberVariableName , pNode);
}

SEL_MenuHandler InputName::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "randNameCallBack", InputName::randNameCallBack);
    return NULL;
}

void InputName::randNameCallBack(const Json::Value&,const Json::Value& responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(m_pName) m_pName->setText(responseData["data"]["name"].asString().c_str());
}


void InputName::randNameCallBack(cocos2d::CCObject* obj)
{
    HTTP_CLIENT->randUserName(this,callfuncJson_selector(InputName::randNameCallBack));
}

void InputName::submitCallBack(cocos2d::CCObject* obj)
{
    finishStep();
}


void InputName::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    std::string m_pInputText = editBox->getText();
    while (true)
    {
        int nStrLen = _calcCharCount(m_pInputText.c_str());
        if (nStrLen <= MAX_CHARS)
        {
            break;
        }
        // get the delete byte number
        int nDeleteLen = 1;    // default, erase 1 byte
        
        while(0x80 == (0xC0 & m_pInputText.at(m_pInputText.length() - nDeleteLen)))
        {
            ++nDeleteLen;
        }
        m_pInputText.erase(m_pInputText.length()-nDeleteLen,nDeleteLen);
        
    }
    editBox->setText(m_pInputText.c_str());
    
//    // if all text deleted, show placeholder string
//    if (nStrLen <= nDeleteLen)
//    {
//        CC_SAFE_DELETE(m_pInputText);
//        m_pInputText = new std::string;
//        m_nCharCount = 0;
//        CCLabelTTF::setString(m_pPlaceHolder->c_str());
//        return;
//    }
//    
//    // set new input text
//    std::string sText(m_pInputText->c_str(), nStrLen - nDeleteLen);
//    setString(sText.c_str());
}


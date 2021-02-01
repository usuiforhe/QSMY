//
//  ChangeName.cpp
//  QSMY
//
//  Created by Eci on 13-11-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ChangeName.h"
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

ChangeName::ChangeName()
:m_pSubmitButton(NULL)
,m_pContainer(NULL)
,m_pCancel(NULL)
{

}

ChangeName::~ChangeName()
{
    CC_SAFE_RELEASE(m_pSubmitButton);
    CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pCancel);
}

void ChangeName::finishStep()
{
    if(strlen(m_pName->getText())<=0)
    {
        Tips_Error(D_LOCAL_STRING("UsernameCantBlank").c_str());
        return;
    }
    HTTP_CLIENT->useItem(ITEM_CHANGE_NAME_ID,m_pName->getText(), this, callfuncJson_selector(ChangeName::finishStep));
}


void ChangeName::finishStep(const Json::Value&,const Json::Value& responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	this->removeFromParent();
}


bool ChangeName::init()
{
	if (!DLayer::init())
	{
		return false;
	}
	loadUI();
	return true;
}

void ChangeName::loadUI()
{
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_wanjiaxinxi/gaiming");
    
    m_pName = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
    m_pName->setFont(D_FONT_DEFAULT,D_FONT_SIZE_14);
    m_pName->setFontColor(ccWHITE);
    m_pName->setPlaceHolder(D_LOCAL_STRING("Username").c_str());
    m_pName->setPlaceholderFontColor(ccBLACK);
    m_pName->setMaxLength(MAX_CHARS*5);
    m_pName->setReturnType(kKeyboardReturnTypeDone);
    m_pName->setAnchorPoint(CCPointZero);
    m_pName->setDelegate(this);
    
    m_pContainer->addChild(m_pName);
        
    m_pSubmitButton->getButton()->setTarget(this, menu_selector(ChangeName::submitCallBack));
	m_pCancel->getButton()->setTarget(this, menu_selector(ChangeName::cancelCallBack));

    randNameCallBack(NULL);
}


bool ChangeName::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton", DButton*, this->m_pSubmitButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_cancel", DButton*, this->m_pCancel);
    return false;
}

SEL_MenuHandler ChangeName::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "randNameCallBack", ChangeName::randNameCallBack);
    return NULL;
}

void ChangeName::randNameCallBack(const Json::Value&,const Json::Value& responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(m_pName) m_pName->setText(responseData["data"]["name"].asString().c_str());
}


void ChangeName::randNameCallBack(cocos2d::CCObject* obj)
{
    HTTP_CLIENT->randUserName(this,callfuncJson_selector(ChangeName::randNameCallBack));
}

void ChangeName::submitCallBack(cocos2d::CCObject* obj)
{
    finishStep();
}


void ChangeName::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
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

void ChangeName::cancelCallBack(cocos2d::CCObject *obj)
{
	this->removeFromParent();
}
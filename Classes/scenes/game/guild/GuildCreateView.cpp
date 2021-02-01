//
//  GuildCreateView.h
//  公会创建
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildCreateView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

#define GUILD_NAME_MAX_LENGTH 10
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

GuildCreateView::GuildCreateView()
	: m_pButtonClose(NULL)
	, m_pButtonCreate(NULL)
	, m_pButtonCreate2(NULL)
	, m_pContainer(NULL)
	, m_pDelegate(NULL)
	, m_pEditBox(NULL)
{
}

GuildCreateView::~GuildCreateView()
{
	CC_SAFE_RELEASE(m_pButtonClose);
	CC_SAFE_RELEASE(m_pButtonCreate);
	CC_SAFE_RELEASE(m_pButtonCreate2);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pEditBox);
}

GuildCreateView* GuildCreateView::create()
{
	GuildCreateView *pRet = new GuildCreateView();
	if (pRet && pRet->init())
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool GuildCreateView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildCreate");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_chuangjian");
	m_pEditBox = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
	CC_SAFE_RETAIN(m_pEditBox);
	m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
	m_pEditBox->setFontColor(ccWHITE);
	m_pEditBox->setPlaceholderFontColor(ccWHITE);
	m_pEditBox->setMaxLength(GUILD_NAME_MAX_LENGTH * 5);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setText("");
	m_pEditBox->setAnchorPoint(CCPointZero);
    m_pEditBox->setDelegate(this);
	m_pContainer->addChild(m_pEditBox);

	m_pButtonClose->getButton()->setTarget(this, menu_selector(GuildCreateView::callBackClose));
	m_pButtonCreate->getButton()->setTarget(this, menu_selector(GuildCreateView::callBackCreate));
	m_pButtonCreate2->setVisible(false);
	return true;
}

void GuildCreateView::onEnter()
{
	DLayer::onEnter();
	//	BINDING_ONLY(m_pModel, this, GuildView::updateNoticeView, D_EVENT_GUILD_GET_NOTICE);
}

void GuildCreateView::onExit()
{
	DLayer::onExit();
	//	UNBINDING_ALL(m_pModel, this);
}

bool GuildCreateView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pButtonClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_create", DButton *, this->m_pButtonCreate);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_create2", DButton *, this->m_pButtonCreate2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	return false;
}

void GuildCreateView::callBackCreate(CCObject *sender)
{
	if(strlen(m_pEditBox->getText())==0)
	{
		Tips_Alert(D_LOCAL_STRING("GuildNameError").c_str());
		return;
	}
	m_pDelegate->createGuildWithName(m_pEditBox->getText());
	m_pEditBox->setText("");
}

void GuildCreateView::callBackClose(CCObject *sender)
{
	m_pDelegate->closeGuildCreateView();
}

void GuildCreateView::editBoxReturn(CCEditBox *editBox)
{
    /*std::string text = editBox->getText();
	if (text.length() > GUILD_NAME_MAX_LENGTH) {
        std::string subText = text.substr(0, GUILD_NAME_MAX_LENGTH);
        if ((subText[GUILD_NAME_MAX_LENGTH - 1] & 0xe0) == 0xe0) {
            subText[GUILD_NAME_MAX_LENGTH - 1] = 0;
        }
        else if ((subText[GUILD_NAME_MAX_LENGTH - 2] & 0xe0) == 0xe0)
        {
            subText[GUILD_NAME_MAX_LENGTH - 2] = 0;
        }
        editBox->setText(subText.c_str());
    }*/

	std::string m_pInputText = editBox->getText();
	while (true)
	{
		int nStrLen = _calcCharCount(m_pInputText.c_str());
		if (nStrLen <= GUILD_NAME_MAX_LENGTH)
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
}
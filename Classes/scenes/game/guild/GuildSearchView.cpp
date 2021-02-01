//
//  GuildSearchView.h
//  公会搜索
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "GuildSearchView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

#define GUILD_NAME_MAX_LENGTH 27

GuildSearchView::GuildSearchView()
	: m_pButtonClose(NULL)
	, m_pButtonSearch(NULL)
	, m_pButtonCancel(NULL)
	, m_pContainer(NULL)
	, m_pDelegate(NULL)
	, m_pEditBox(NULL)
{
}

GuildSearchView::~GuildSearchView()
{
	CC_SAFE_RELEASE(m_pButtonClose);
	CC_SAFE_RELEASE(m_pButtonSearch);
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pEditBox);
}

GuildSearchView* GuildSearchView::create()
{
	GuildSearchView *pRet = new GuildSearchView();
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

bool GuildSearchView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildSearch");
	m_pEditBox = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
	CC_SAFE_RETAIN(m_pEditBox);
	m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
	m_pEditBox->setFontColor(ccRED);
	m_pEditBox->setPlaceholderFontColor(ccWHITE);
	m_pEditBox->setMaxLength(GUILD_NAME_MAX_LENGTH);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setText("");
	m_pEditBox->setAnchorPoint(CCPointZero);
	m_pEditBox->setDelegate(this);
	m_pContainer->addChild(m_pEditBox);

	m_pButtonClose->setTarget(this, menu_selector(GuildSearchView::callBackClose));
	m_pButtonCancel->getButton()->setTarget(this, menu_selector(GuildSearchView::callBackClose));
	m_pButtonSearch->getButton()->setTarget(this, menu_selector(GuildSearchView::callBackSearch));
	return true;
}

void GuildSearchView::onEnter()
{
	DLayer::onEnter();
	//	BINDING_ONLY(m_pModel, this, GuildView::updateNoticeView, D_EVENT_GUILD_GET_NOTICE);
}

void GuildSearchView::onExit()
{
	DLayer::onExit();
	//	UNBINDING_ALL(m_pModel, this);
}

bool GuildSearchView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_close", CCMenuItem *, this->m_pButtonClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_enter", DButton *, this->m_pButtonSearch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", DButton *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	return false;
}

void GuildSearchView::callBackSearch(CCObject *sender)
{
	if(strlen(m_pEditBox->getText())==0)
	{
		Tips_Alert(D_LOCAL_STRING("GuildNameError").c_str());
		return;
	}

	if (NULL != m_pDelegate)
	{
		m_pDelegate->searchGuildWithName(m_pEditBox->getText());
	}
	m_pEditBox->setText("");
}

void GuildSearchView::callBackClose(CCObject *sender)
{
	if (NULL != m_pDelegate)
	{
		m_pDelegate->closeGuildSearchView();
	}
}

void GuildSearchView::editBoxReturn(CCEditBox *editBox)
{
	std::string text = editBox->getText();
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
	}
}
//
//  GuildMessageView.h
//  公会留言
//
//  Created by Eci on 13-10-26.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildMessageView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

#define NOTICE_MAX_LENGTH 90

GuildMessageView::GuildMessageView() :
m_pEnter(NULL),
	m_pCancel(NULL),
	m_pClose(NULL),
	m_pContainer(NULL),
	m_pTextField(NULL)
	,m_pDelegate(NULL)
	,m_pNodeToHide(NULL)
{
}

GuildMessageView::~GuildMessageView()
{
	CC_SAFE_RELEASE(m_pEnter);
	CC_SAFE_RELEASE(m_pCancel);
	CC_SAFE_RELEASE(m_pClose);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pTextField);
	CC_SAFE_RELEASE(m_pNodeToHide);
}

GuildMessageView *GuildMessageView::create()
{
	GuildMessageView *pRet = new GuildMessageView();
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

bool GuildMessageView::init()
{
	if(!DPopup::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildNotice");
	m_pTextField = MyTextFieldTTF::textFieldWithPlaceHolder("",
		m_pContainer->getContentSize(),
		kCCTextAlignmentLeft,
		D_FONT_DEFAULT,
		D_FONT_SIZE_12);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_pTextField->setMaxLength(NOTICE_MAX_LENGTH);
	m_pTextField->setDelegate(this);
#endif
	//m_pTextField->setString(guildInfo->getAnnouncement().c_str());
	m_pTextField->setFontColor(ccBLACK);
	m_pTextField->setAnchorPoint(CCPointZero);
	m_pTextField->setPlaceHolder(D_LOCAL_STRING("FriendMsgCount").c_str());
	m_pContainer->addChild(m_pTextField);
	m_pTextField->setDEditBoxContainer(m_pContainer);
	CC_SAFE_RETAIN(m_pTextField);

	m_pEnter->getButton()->setTarget(this, menu_selector(GuildMessageView::callBackEnter));
	m_pCancel->getButton()->setTarget(this, menu_selector(GuildMessageView::callBackClose));
	m_pClose->setTarget(this, menu_selector(GuildMessageView::callBackClose));
	m_pNodeToHide->setVisible(false);

	return true;
}

void GuildMessageView::onEnter()
{
	DPopup::onEnter();
}

void GuildMessageView::onExit()
{
	DPopup::onExit();
}

bool GuildMessageView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_enter", DButton *, this->m_pEnter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_cancel", DButton *, this->m_pCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_close", CCMenuItem *, this->m_pClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_announcement", CCNode *, this->m_pNodeToHide);


	return false;
}


void GuildMessageView::callBackEnter(CCObject *sender)
{
	std::string content = m_pTextField->getString();
	content = DM_GET_FILTER_MODEL->filter(content.c_str()).c_str();
	if (0 == content.length())
	{
		Tips_Alert(D_LOCAL_STRING("MessageError").c_str());
		return;
	}
	bool allSpace = true;
	for (std::string::iterator iter = content.begin(); iter != content.end(); iter++)
	{
		if (*iter != ' ')
		{
			allSpace = false;
			break;
		}
	}
	if (allSpace)
	{
		Tips_Alert(D_LOCAL_STRING("MessageError").c_str());
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->willSendMessage(content.c_str());
	}
}

void GuildMessageView::callBackClose(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willCloseMessage();
	}
}
void GuildMessageView::sendCallBack(const Json::Value &,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	//Tips_Alert(D_LOCAL_STRING("send").c_str());

	//guild::GuildInfo *guildInfo = DM_GET_GUILD_MODEL->getGuildInfo();
	//guildInfo->setAnnouncement(m_pTextField->getString());
	this->removeFromParent();
}

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GuildMessageView::editBoxReturn(EditBox* editBox)
{
    std::string text = editBox->getText();
	if (text.length() > NOTICE_MAX_LENGTH) {
        std::string subText = text.substr(0, NOTICE_MAX_LENGTH);
        if ((subText[NOTICE_MAX_LENGTH - 1] & 0xe0) == 0xe0) {
            subText[NOTICE_MAX_LENGTH - 1] = 0;
        }
        else if ((subText[NOTICE_MAX_LENGTH - 2] & 0xe0) == 0xe0)
        {
            subText[NOTICE_MAX_LENGTH - 2] = 0;
        }
		text = subText;
    }
	
	editBox->setText(DM_GET_FILTER_MODEL->filter(text.c_str()).c_str());
}
//#endif
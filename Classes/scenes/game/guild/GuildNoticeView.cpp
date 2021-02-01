//
//  GuildNoticeView.h
//  公会公告
//
//  Created by Eci on 13-10-22.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildNoticeView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

#define NOTICE_MAX_LENGTH 60

GuildNoticeView::GuildNoticeView() :
m_pEnter(NULL),
	m_pCancel(NULL),
	m_pClose(NULL),
	m_pContainer(NULL),
	m_pTextField(NULL)
	,m_pNodeToHide(NULL)
{
}

GuildNoticeView::~GuildNoticeView()
{
	CC_SAFE_RELEASE(m_pEnter);
	CC_SAFE_RELEASE(m_pCancel);
	CC_SAFE_RELEASE(m_pClose);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pTextField);
	CC_SAFE_RELEASE(m_pNodeToHide);
}

GuildNoticeView* GuildNoticeView::create()
{
	GuildNoticeView *pRet = new GuildNoticeView();
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

bool GuildNoticeView::init()
{
	if(!DPopup::init())
	{
		return false;
	}
	guild::GuildInfo *guildInfo = DM_GET_GUILD_MODEL->getGuildInfo();
	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildNotice");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_xuanyan");
	m_pTextField = MyTextFieldTTF::textFieldWithPlaceHolder("",
		m_pContainer->getContentSize(),
		kCCTextAlignmentLeft,
		D_FONT_DEFAULT,
		D_FONT_SIZE_12);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_pTextField->setMaxLength(NOTICE_MAX_LENGTH);
	m_pTextField->setDelegate(this);
#endif
	m_pTextField->setString(guildInfo->getAnnouncement().c_str());
	m_pTextField->setFontColor(ccWHITE);
	m_pTextField->setAnchorPoint(CCPointZero);
	m_pTextField->setDEditBoxContainer(m_pContainer);
	m_pContainer->addChild(m_pTextField);
	CC_SAFE_RETAIN(m_pTextField);

	m_pEnter->getButton()->setTarget(this, menu_selector(GuildNoticeView::callBackEnter));
	m_pCancel->getButton()->setTarget(this, menu_selector(GuildNoticeView::callBackClose));
	m_pClose->getButton()->setTarget(this, menu_selector(GuildNoticeView::callBackClose));
	//m_pNodeToHide->setVisible(false);

	return true;
}

void GuildNoticeView::onEnter()
{
	DPopup::onEnter();

	GuildModel *m_pModel = DM_GET_GUILD_MODEL;

	if (m_pModel->getSelfInfo()->getPosition() == guild::GuildPositionAssistant || m_pModel->getSelfInfo()->getPosition() == guild::GuildPositionPresident)
	{
		m_pEnter->getButton()->setEnabled(true);
	}
	else
	{
		m_pEnter->getButton()->setEnabled(false);
	}
}

bool GuildNoticeView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_enter", DButton *, this->m_pEnter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_cancel", DButton *, this->m_pCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_close", DButton *, this->m_pClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_message", CCNode *, this->m_pNodeToHide);

	return false;
}


void GuildNoticeView::callBackEnter(CCObject *sender)
{
	std::string content = m_pTextField->getString();
	if (0 == content.length())
	{
		Tips_Alert(D_LOCAL_STRING("GuildAnnouncementError").c_str());
		return;
	}
	//HTTP_CLIENT->addFeedBack(content.c_str(),this,callfuncJson_selector(UserFeedback::sendCallBack));
	HTTP_CLIENT->guildUpdate(content.c_str(), this, callfuncJson_selector(GuildNoticeView::sendCallBack));
}

void GuildNoticeView::callBackClose(CCObject *sender)
{
	closePopup();
}
void GuildNoticeView::sendCallBack(const Json::Value &,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	//Tips_Alert(D_LOCAL_STRING("send").c_str());

	//guild::GuildInfo *guildInfo = DM_GET_GUILD_MODEL->getGuildInfo();
	//guildInfo->setAnnouncement(m_pTextField->getString());
	closePopup();
}

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GuildNoticeView::editBoxReturn(EditBox* editBox)
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
        editBox->setText(subText.c_str());
    }
}
//#endif
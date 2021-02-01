//
//  FriendsSearchView.h
//  好友搜索
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FriendsSearchView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "Friends.h"


#define GUILD_NAME_MAX_LENGTH 27

FriendsSearchView::FriendsSearchView()
	: m_pButtonClose(NULL)
	, m_pButtonSearch(NULL)
	, m_pButtonCancel(NULL)
	, m_pContainer(NULL)
	, m_pDelegate(NULL)
	, m_pEditBox(NULL)
{
}

FriendsSearchView::~FriendsSearchView()
{
	CC_SAFE_RELEASE(m_pButtonClose);
	CC_SAFE_RELEASE(m_pButtonSearch);
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pEditBox);
}

FriendsSearchView* FriendsSearchView::create()
{
	FriendsSearchView *pRet = new FriendsSearchView();
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

bool FriendsSearchView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	//m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("ui/ccb/friends/friendsSearch");
	m_pEditBox = CCEditBox::create(m_pContainer->getContentSize(), CCScale9Sprite::create());
	CC_SAFE_RETAIN(m_pEditBox);
	m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
	m_pEditBox->setFontColor(ccRED);
	m_pEditBox->setPlaceholderFontColor(ccWHITE);
	m_pEditBox->setPlaceHolder(D_LOCAL_STRING("FriendInputID").c_str());
	m_pEditBox->setMaxLength(GUILD_NAME_MAX_LENGTH);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setText("");
	m_pEditBox->setAnchorPoint(CCPointZero);
	m_pEditBox->setDelegate(this);
	m_pContainer->addChild(m_pEditBox);

	m_pButtonClose->setTarget(this, menu_selector(FriendsSearchView::callBackClose));
	m_pButtonCancel->getButton()->setTarget(this, menu_selector(FriendsSearchView::callBackClose));
	m_pButtonSearch->getButton()->setTarget(this, menu_selector(FriendsSearchView::callBackSearch));
	return true;
}

void FriendsSearchView::onEnter()
{
	DLayer::onEnter();
	//	BINDING_ONLY(m_pModel, this, GuildView::updateNoticeView, D_EVENT_GUILD_GET_NOTICE);
}

void FriendsSearchView::onExit()
{
	DLayer::onExit();
	//	UNBINDING_ALL(m_pModel, this);
}

bool FriendsSearchView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnClose", CCMenuItem *, this->m_pButtonClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnEnter", DButton *, this->m_pButtonSearch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnCancel", DButton *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	return false;
}

void FriendsSearchView::callBackSearch(CCObject *sender)
{
	if(strlen(m_pEditBox->getText())==0)
	{
		//Tips_Alert(D_LOCAL_STRING("GuildNameError").c_str());
		return;
	}
	uint32_t friendID = atoi(m_pEditBox->getText());
	if (friendID == DM_GET_USER_MODEL->getUserInfo()->getUserId())
	{
		Tips_Alert(D_LOCAL_STRING("FriendSearchSelf").c_str());
		return;
	}
	if (DM_GET_FRIENDS_MODEL->isFriend(friendID))
	{
		Tips_Alert(D_LOCAL_STRING("FriendSearchAdded").c_str());
		return;
	}
	HTTP_CLIENT->searchFriend(friendID, this, callfuncJson_selector(FriendsSearchView::callBackSearch));
	m_pEditBox->setText("");
}

void FriendsSearchView::callBackClose(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavHome);
	}
}

void FriendsSearchView::editBoxReturn(CCEditBox *editBox)
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

void FriendsSearchView::callBackSearch(const Json::Value &request, const Json::Value &response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavFound);
	}
}
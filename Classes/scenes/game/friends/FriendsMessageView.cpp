//
//  FriendsMessageView.h
//  编辑留言
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "FriendsMessageView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "Friends.h"

#define MESSAGE_MAX_LENGTH 90

FriendsMessageView::FriendsMessageView() 
	: m_pContainer(NULL)
	, m_pTTFName(NULL)
	, m_pTextField(NULL)
	, m_pDelegate(NULL)
	, m_nUserID(0)
	, m_eType(mail::kMailTypeFromFriend)
{
}

FriendsMessageView::~FriendsMessageView()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pTextField);
}

FriendsMessageView* FriendsMessageView::create()
{
	FriendsMessageView *pRet = new FriendsMessageView();
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

bool FriendsMessageView::init()
{
	if(!DPopup::init())
	{
		return false;
	}
	CCB_READER_AND_ADDCHILD("ui/ccb/friends/friendsSendMail");
	m_pTextField = MyTextFieldTTF::textFieldWithPlaceHolder("",
		m_pContainer->getContentSize(),
		kCCTextAlignmentLeft,
		D_FONT_DEFAULT,
		D_FONT_SIZE_12);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_pTextField->setMaxLength(MESSAGE_MAX_LENGTH);
	m_pTextField->setDelegate(this);
#endif
	m_pTextField->setFontColor(ccBLACK);
	m_pTextField->setAnchorPoint(CCPointZero);
	m_pTextField->setDEditBoxContainer(m_pContainer);
	m_pTextField->setPlaceHolder(D_LOCAL_STRING("FriendMsgCount").c_str());
	m_pContainer->addChild(m_pTextField);
	CC_SAFE_RETAIN(m_pTextField);

	return true;
}

void FriendsMessageView::onEnter()
{
	DPopup::onEnter();
}

void FriendsMessageView::onExit()
{
	DPopup::onExit();
}

bool FriendsMessageView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, this->m_pTTFName);

	return false;
}

SEL_MenuHandler FriendsMessageView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackClose", FriendsMessageView::callBackClose);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackSend", FriendsMessageView::callBackEnter);
	return NULL;
}

void FriendsMessageView::callBackEnter(CCObject *sender)
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
	if (m_nUserID)
	{
		if (m_eType == mail::kMailTypeFromFriend)
		{
			uint32_t cd = DM_GET_FRIENDS_MODEL->getMsgCD();
			if (cd > 0)
			{
				Tips_Alert(D_LOCAL_STRING("FriendMessageCD%u", cd).c_str());
				return;
			}
			HTTP_CLIENT->friendSendMail(m_nUserID, content.c_str(), this, callfuncJson_selector(FriendsMessageView::callBackSend));
		}
		else if (m_eType == mail::kMailTypeFromUser)
		{
			HTTP_CLIENT->guildMsg(m_nUserID, content.c_str(), this, callfuncJson_selector(FriendsMessageView::callBackSend));
		}
	}
}

void FriendsMessageView::callBackClose(CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavNoPop);
	}
	else
	{
		closePopup();
	}
}

void FriendsMessageView::editBoxReturn(EditBox* editBox)
{
    std::string text = editBox->getText();
	if (text.length() > MESSAGE_MAX_LENGTH) {
        std::string subText = text.substr(0, MESSAGE_MAX_LENGTH);
        if ((subText[MESSAGE_MAX_LENGTH - 1] & 0xe0) == 0xe0) {
            subText[MESSAGE_MAX_LENGTH - 1] = 0;
        }
        else if ((subText[MESSAGE_MAX_LENGTH - 2] & 0xe0) == 0xe0)
        {
            subText[MESSAGE_MAX_LENGTH - 2] = 0;
        }
		text = subText;
	}

	editBox->setText(DM_GET_FILTER_MODEL->filter(text.c_str()).c_str());
}

void FriendsMessageView::setName(std::string sName)
{
	m_pTTFName->setString(sName.c_str());
}

void FriendsMessageView::callBackSend(const Json::Value &request, const Json::Value &response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	if (m_pDelegate)
	{
		m_pDelegate->FriendsNavigateTo(kFriendsNavNoPop);
	}
	else
	{
		closePopup();
	}
}
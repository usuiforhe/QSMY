//
//  Chat.cpp
//  聊天
//  Created by mzp on 14-04-22.
//	Modified 
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "Chat.h"
#include "../../../model/ChatModel.h"
#include "../../../model/GuildModel.h"
#include "ChatView.h"
#include "managers/GameManager.h"
#include "../Game.h"



#define CHAT_MAX_LENGTH 150

USING_NS_CC;



Chat::Chat()
	: m_pContainer(NULL)
	, m_pBtnSend(NULL)
	, m_pBtnClose(NULL)
	, m_pMsgContainer(NULL)
	, m_pTextField(NULL)
	, m_pDTabView(NULL)
	, m_pChatItem(NULL)
	, m_pChatItemNum(NULL)
	, m_bRequestGuild(true)
	, m_nSelectIndex(0)
	, m_pTabContainer(NULL)
{

}

Chat::~Chat()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pBtnSend);
	CC_SAFE_RELEASE(m_pBtnClose);
	CC_SAFE_RELEASE(m_pMsgContainer);
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pTextField);
	CC_SAFE_RELEASE(m_pChatItem);
	CC_SAFE_RELEASE(m_pChatItemNum);
	CC_SAFE_RELEASE(m_pTabContainer);
}

bool Chat::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_liaotian/pl_liaotian");

	m_pBtnSend->getButton()->setTarget(this, menu_selector(Chat::menuCallback));
	m_pBtnClose->getButton()->setTarget(this, menu_selector(Chat::menuCallback));

 	// 初始化编辑框
	m_pTextField = MyTextFieldTTF::textFieldWithPlaceHolder("",
		m_pMsgContainer->getContentSize(),
		kCCTextAlignmentLeft,
		D_FONT_DEFAULT,
		D_FONT_SIZE_12);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_pTextField->setMaxLength(CHAT_MAX_LENGTH);
	m_pTextField->setDelegate(this);
#endif
	m_pTextField->setPlaceHolder(D_LOCAL_STRING("ChatPlaceHolder").c_str());
	m_pTextField->setFontColor(ccWHITE);
	m_pTextField->setPlaceholderFontColor(ccGRAY);
	m_pTextField->setAnchorPoint(CCPointZero);
	m_pTextField->setDEditBoxContainer(m_pMsgContainer);
	m_pMsgContainer->addChild(m_pTextField);
	CC_SAFE_RETAIN(m_pTextField);

	// 初始化聊天频道

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height + 20, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),12);

	//m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, m_pContainer->getPosition().y, 
		//m_pContainer->getContentSize().width, m_pContainer->getContentSize().height), 15);
	m_pDTabView->retain();
	m_pDTabView->setTopMenuisLeftAlignment(true);
	m_pDTabView->setDelegate(this);

	std::string tabName[chat::kChatChannelCount] = {"liaotian_wenzi_shijieliaotian", "liaotian_wenzi_bangpailiaotian"};
	std::string selectImg[2] = {"all_btn_fenye_lv","all_btn_fenye_lan"};
	std::string normalImg[2] = {"all_btn_fenye_lv_up","all_btn_fenye_lan_up"};
	for (unsigned int i = 0; i < chat::kChatChannelCount; ++i)
	{
		//ChatView* chatView = ChatView::create((chat::ChatChannel)i, m_pContainer->getContentSize());
		ChatView* chatView = ChatView::create((chat::ChatChannel)i, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
		//chatView->setDelegate(this);
		m_pDTabView->addTab(chatView, "", CCString::createWithFormat("PL_ui/all/%s.png",normalImg[i].c_str())->getCString(), 
			CCString::createWithFormat("PL_ui/all/%s.png",selectImg[i].c_str())->getCString(), 
			"PL_ui/all/all_btn_fenye_hui.png", CCString::createWithFormat("PL_ui/liaotian/%s.png", tabName[i].c_str())->getCString());
	}
	//m_pDTabView->setTabEnable(false,1);
	m_pTabContainer->addChild(m_pDTabView);

	return true;
}

void Chat::onEnter()
{
	DPopup::onEnter();

	BINDING_ONLY(DM_GET_CHAT_MODEL, this, Chat::guildGetSuc, D_EVENT_CHAT_GUILD_GET_SUC);
	BINDING_ONLY(DM_GET_CHAT_MODEL, this, Chat::guildSendSuc, D_EVENT_CHAT_GUILD_SEND_SUC);
	BINDING_ONLY(DM_GET_CHAT_MODEL, this, Chat::worldGetSuc, D_EVENT_CHAT_WORLD_GET_SUC);
	BINDING_ONLY(DM_GET_CHAT_MODEL, this, Chat::worldSendSuc, D_EVENT_CHAT_WORLD_SEND_SUC);

	item::ItemInfo* pChatItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(ITEM_CHAT_ID);
	BINDING_EXEC(pChatItemInfo, this, Chat::updataChatItem, D_EVENT_ITEMINFO_UPDATE);
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfunc_selector(Chat::buySuccess),D_EVENT_BUYITEM_SUCCESS);
	// 关闭检测新消息
	DM_GET_CHAT_MODEL->closeCheckNew();
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CHAT_CHECK_NEW, CCInteger::create(0));
	// 同步所有频道消息
	DM_GET_CHAT_MODEL->syncWorld(false);
	// 无提示
	m_bRequestGuild = true;
	BINDING_ONLY(DM_GET_GUILD_MODEL, this, Chat::updateGuildInfo, D_EVENT_GUILD_GET);
	DM_GET_GUILD_MODEL->syncFromServer();

	// 切换到检测到的消息页
	chat::ChatCheckType chatType = DM_GET_CHAT_MODEL->getChatCheckType();
	switch (chatType)
	{
	case chat::kChatCheckTypeNone:
	case chat::kChatCheckTypeWorld:
		m_pDTabView->selectedTab((int)chat::kChatChannelWorld);
		break;
	case chat::kChatCheckTypeGuild:
		m_pDTabView->selectedTab((int)chat::kChatChannelGuild);
		break;
	case chat::kChatCheckTypeAll:
		m_pDTabView->selectedTab((int)chat::kChatChannelWorld);
		break;
	}
	
}

void Chat::onExit()
{
	UNBINDING_ALL(DM_GET_CHAT_MODEL, this);
	UNBINDING_ALL(DM_GET_GUILD_MODEL, this);
	item::ItemInfo* pChatItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(ITEM_CHAT_ID);
	UNBINDING_ALL(pChatItemInfo, this);
	NotificationCenter::sharedNotificationCenter()->removeObserver(this,D_EVENT_BUYITEM_SUCCESS);
	unschedule(schedule_selector(Chat::requestGuildMsg));
	unschedule(schedule_selector(Chat::requestWorldMsg));

	// 开启检测新消息
	DM_GET_CHAT_MODEL->openCheckNew();

	DPopup::onExit();
}

bool Chat::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainer", CCNode*, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDevelopButton", DButton*, this->m_pBtnSend);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, this->m_pBtnClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMsgContainer", CCNode*, this->m_pMsgContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChatItem", CCNode*, this->m_pChatItem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont*, this->m_pChatItemNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTabContainer" ,CCNode*,m_pTabContainer);

	return false;
}

void Chat::editBoxReturn(EditBox* editBox)
{
	std::string text = editBox->getText();
	if (text.length() > CHAT_MAX_LENGTH) {
		std::string subText = text.substr(0, CHAT_MAX_LENGTH);
		if ((subText[CHAT_MAX_LENGTH - 1] & 0xe0) == 0xe0) {
			subText[CHAT_MAX_LENGTH - 1] = 0;
		}
		else if ((subText[CHAT_MAX_LENGTH - 2] & 0xe0) == 0xe0)
		{
			subText[CHAT_MAX_LENGTH - 2] = 0;
		}
		editBox->setText(subText.c_str());
	}
}

void Chat::editBoxTextChanged(CCEditBox* editBox, const std::string& texts)
{
	std::string text = editBox->getText();
	if (text.length() > CHAT_MAX_LENGTH) {
		std::string subText = text.substr(0, CHAT_MAX_LENGTH);
		if ((subText[CHAT_MAX_LENGTH - 1] & 0xe0) == 0xe0) {
			subText[CHAT_MAX_LENGTH - 1] = 0;
		}
		else if ((subText[CHAT_MAX_LENGTH - 2] & 0xe0) == 0xe0)
		{
			subText[CHAT_MAX_LENGTH - 2] = 0;
		}
		editBox->setText(subText.c_str());
	}
}

void Chat::SelectTabAtIndex(DTabView* tabView, uint32_t idx)
{
	m_nSelectIndex = idx;
	switch (idx)
	{
	case chat::kChatChannelWorld:
		{
			m_pChatItem->setVisible(true);
			
			break;
		}
	case chat::kChatChannelGuild:
		{
 			// 先同步工会信息
			m_bRequestGuild = false;
 			//DM_GET_GUILD_MODEL->syncFromServer();

			m_pChatItem->setVisible(false);

			break;
		}
	}
}

void Chat::menuCallback(cocos2d::CCObject* obj)
{
	if (obj == m_pBtnClose->getButton())
	{
		closePopup();
	}
	else if (obj == m_pBtnSend->getButton())
	{
		switch (m_nSelectIndex)
		{
		case chat::kChatChannelWorld:
			{
				if(DM_GET_ITEM_MODEL->getItemNum(ITEM_CHAT_ID) == 0)
				{
					closePopup();
				}
				DM_GET_CHAT_MODEL->sendWorldMsg(m_pTextField->getString());
				break;
			}
		case chat::kChatChannelGuild:
			{
				DM_GET_CHAT_MODEL->sendGuildMsg(m_pTextField->getString());
				break;
			}
		}
	}
}

void Chat::updataChatItem()
{
	m_pChatItemNum->setString(cocos2d::CCString::createWithFormat("%d", DM_GET_ITEM_MODEL->getItemNum(ITEM_CHAT_ID))->getCString());
}

void Chat::buySuccess()
{
	item::ItemInfo* pChatItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(ITEM_CHAT_ID);
	UNBINDING_ALL(pChatItemInfo, this);
	BINDING_EXEC(pChatItemInfo, this, Chat::updataChatItem, D_EVENT_ITEMINFO_UPDATE);
}

void Chat::requestGuildMsg(float dt)
{
	DM_GET_CHAT_MODEL->syncGuild(true);
}

void Chat::requestWorldMsg(float dt)
{
	DM_GET_CHAT_MODEL->syncWorld(true);
}

void Chat::guildGetSuc()
{
	// 同步信息之后添加刷新心跳
	unschedule(schedule_selector(Chat::requestGuildMsg));
	scheduleOnce(schedule_selector(Chat::requestGuildMsg), D_CONFIG_FLOAT_FOR_KEY(CHAT_PULL_INTERVAL_TIME));
}

void Chat::guildSendSuc()
{
	// 发送成功之后清空输入框
	m_pTextField->setString("");
}

void Chat::worldGetSuc()
{
	// 同步信息之后添加刷新心跳
	unschedule(schedule_selector(Chat::requestWorldMsg));
	scheduleOnce(schedule_selector(Chat::requestWorldMsg), D_CONFIG_FLOAT_FOR_KEY(CHAT_PULL_INTERVAL_TIME));
}

void Chat::worldSendSuc()
{
	// 发送成功之后清空输入框
	m_pTextField->setString("");
}

void Chat::updateGuildInfo()
{
	guild::GuildUser* userInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	if (!userInfo)
	{
		return;
	}

	uint32_t gid = userInfo->getGuildID();
	if (gid == 0)
	{
		// 不需要请求消息时，只提示
		if (!m_bRequestGuild)
		{
			Tips_Alert(D_LOCAL_STRING("GuildErrorNoGuild"));
		}
		
		return;
	}

	if (m_bRequestGuild)
	{
		DM_GET_CHAT_MODEL->syncGuild(false);
	}
}

//
//  ChatView.cpp
//  聊天
//  Created by mzp on 14-04-22.
//	Modified 
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "ChatView.h"
#include "managers/GameManager.h"
#include "../../../model/GuildModel.h"
#include "../../../model/UserModel.h"


#define CHAT_CELL_MAX_WIDTH	590
#define CHAT_CELL_DEFAULT_SIZE	CCSizeMake(611, 110)
#define CHAT_GUILD_CELL_TOP_HEIGHT	38.0f
#define CHAT_WORLD_CELL_TOP_HEIGHT	38.0f
#define CHAT_CELL_TEXT_OFFSET	30.0f


ChatView::ChatView()
	: m_pListView(NULL)
	, m_pData(NULL)
	, m_pChatModel(NULL)
	, m_kChatChannel(chat::kChatChannelGuild)
{

}

ChatView::~ChatView()
{
	CC_SAFE_RELEASE(m_pData);
}

ChatView* ChatView::create(chat::ChatChannel type, const cocos2d::CCSize& size)
{
	ChatView *pRet = new ChatView();
	if (pRet && pRet->init(type, size))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool ChatView::init(chat::ChatChannel type, const cocos2d::CCSize& size)
{
	if (!DLayer::init())
	{
		return false;
	}

	m_pChatModel = DM_GET_CHAT_MODEL;
	m_kChatChannel = type;

	m_pData = readCCData("PL_ui/ccb/pl_liaotian/pl_liaotian_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pListView = DTableView::create(this, size);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_pListView);

	return true;
}

void ChatView::onEnter()
{
	DLayer::onEnter();

	if (m_kChatChannel == chat::kChatChannelWorld)
	{
		BINDING_EXEC(m_pChatModel, this, ChatView::updateMsg, D_EVENT_CHAT_WORLD_UPDATE);
	}
	else if (m_kChatChannel == chat::kChatChannelGuild)
	{
		BINDING_EXEC(m_pChatModel, this, ChatView::updateMsg, D_EVENT_CHAT_GUILD_UPDATE);
	}
}

void ChatView::onExit()
{
	UNBINDING_ALL(m_pChatModel, this);
	DLayer::onExit();
}

cocos2d::CCSize ChatView::tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	cocos2d::CCArray* pChannelInfo = m_pChatModel->getChatInfoByChannel(m_kChatChannel);
	if (!pChannelInfo)
	{
		return CHAT_CELL_DEFAULT_SIZE;
	}

	chat::ChatInfo* chatInfo = (chat::ChatInfo*)pChannelInfo->objectAtIndex(idx);
	if (!chatInfo)
	{
		return CHAT_CELL_DEFAULT_SIZE;
	}

	if (m_sizeMap.find(chatInfo->getId()) == m_sizeMap.end())
	{
		ChatViewCell* pCell = ChatViewCell::create(m_pData);
		pCell->setChatInfo(m_kChatChannel, chatInfo);
		m_sizeMap[chatInfo->getId()] = CCSizeMake(CHAT_CELL_DEFAULT_SIZE.width, pCell->getHeight());
	}
	
	return m_sizeMap[chatInfo->getId()];
}

void ChatView::tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell)
{

}

cocos2d::extension::CCTableViewCell* ChatView::tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	cocos2d::CCArray* pChannelInfo = m_pChatModel->getChatInfoByChannel(m_kChatChannel);
	if (!pChannelInfo)
	{
		return ChatViewCell::create(m_pData);
	}

	chat::ChatInfo* chatInfo = (chat::ChatInfo*)pChannelInfo->objectAtIndex(idx);
	if (!chatInfo)
	{
		return ChatViewCell::create(m_pData);
	}

	ChatViewCell *cell = (ChatViewCell*)table->dequeueCell();
	if (!cell)
	{
		cell = ChatViewCell::create(m_pData);
	}

	cell->setChatInfo(m_kChatChannel, chatInfo);

	return cell;
}

unsigned int ChatView::numberOfCellsInTableView(cocos2d::extension::CCTableView* table)
{
	cocos2d::CCArray* pChannelInfo = m_pChatModel->getChatInfoByChannel(m_kChatChannel);
	if (!pChannelInfo)
	{
		return 0;
	}
	
	return pChannelInfo->count();
}

void ChatView::updateMsg()
{
	m_pListView->reloadData();
	m_pListView->setContentOffset(m_pListView->maxContainerOffset());
	m_pListView->updateContentOffset();
}









ChatViewCell::ChatViewCell()
	: m_pNode(NULL)
	, m_pOtherBj(NULL)
	, m_pSelfBj(NULL)
	//, m_pGuildName(NULL)
	, m_pName(NULL)
	, m_pText(NULL)
	//, m_pGuild(NULL)
	//, m_pVip(NULL)
	, m_fHeight(0.0f)
	,m_pVipTxt(NULL)
	,m_pArrow(NULL)
	,m_pArrowSelf(NULL)
	,m_pArrowSystem(NULL)
	,m_pTitleNormalBg(NULL)
	,m_pTitleSystemBg(NULL)
	,m_pSystemBg(NULL)
{

}

ChatViewCell::~ChatViewCell()
{
	CC_SAFE_RELEASE(m_pOtherBj);
	CC_SAFE_RELEASE(m_pSelfBj);
	//CC_SAFE_RELEASE(m_pGuildName);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pText);
	//CC_SAFE_RELEASE(m_pGuild);
	//CC_SAFE_RELEASE(m_pVip);
	CC_SAFE_RELEASE(m_pVipTxt);
	CC_SAFE_RELEASE(m_pArrow);
	CC_SAFE_RELEASE(m_pArrowSelf);
	CC_SAFE_RELEASE(m_pArrowSystem);
	CC_SAFE_RELEASE(m_pTitleSystemBg);
	CC_SAFE_RELEASE(m_pTitleNormalBg);
	CC_SAFE_RELEASE(m_pSystemBg);
}

ChatViewCell* ChatViewCell::create(cocos2d::extension::CCData* pData)
{
	ChatViewCell* pRet = new ChatViewCell();
	if (pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool ChatViewCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA(pData, m_pNode);
	if (m_pNode)
	{
		addChild(m_pNode);
	}
	
    m_pText->setFontFillColor(m_pText->getColor());
    //m_pText->setColor(ccWHITE);
    
    m_pName->setFontFillColor(m_pName->getColor());
    m_pName->setColor(ccWHITE);
    
	m_oOriginSize = m_pSelfBj->getContentSize();
	m_oOriginPoint = m_pSelfBj->getPosition();
	m_pVipTxt->setVisible(false);

	return true;
}

bool ChatViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOtherBj", CCNode*, this->m_pOtherBj);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelfBj", CCNode*, this->m_pSelfBj);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildName", CCLabelTTF*, this->m_pGuildName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCRichLabelTTF*, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pText", CCRichLabelTTF*, this->m_pText);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuild", CCNode*, this->m_pGuild);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVip", CCNode*, this->m_pVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipTxt", CCLabelBMFont*,m_pVipTxt);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrow" , CCNode*,m_pArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrowSelf" , CCNode*,m_pArrowSelf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleNormalBg" , CCNode*,m_pTitleNormalBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleSystemBg" , CCNode*,m_pTitleSystemBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrowSystem" , CCNode*,m_pArrowSystem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSystemBg" , CCNode*, m_pSystemBg);

	return false;
}

void ChatViewCell::setChatInfo(chat::ChatChannel channel, chat::ChatInfo* pInfo)
{
	if (!pInfo)
	{
		return;
	}

	/*if (channel == chat::kChatChannelWorld)
	{
	m_pGuild->setVisible(false);
	m_pVip->setVisible(false);
	}
	else if (channel == chat::kChatChannelGuild)
	{
	m_pGuild->setVisible(true);
	m_pVip->setVisible(false);
	}
	else
	{
	m_pGuild->setVisible(false);
	m_pVip->setVisible(false);
	}*/

	// 工会职位
	/*const guild::GuildUserBasics* pGuildUserBasics = DM_GET_GUILD_MODEL->getGuildUserBasicsByUserId(pInfo->getUid());
	if (pGuildUserBasics)
	{
		m_pGuildName->setString(pGuildUserBasics->getKnighthoodName().c_str());
	}
	else
	{
		m_pGuildName->setString("");
	}*/

	// 玩家名字，时间
	std::string strNameTime = pInfo->getName() + "  " + TimeUtil::formatTime(pInfo->getTs());
	m_pName->setString(strNameTime.c_str());
	// 聊天内容
	m_pText->setDimensions(CCSizeZero);
	m_pText->setString(DM_GET_FILTER_MODEL->filter(pInfo->getMsg().c_str()).c_str());

	const CCSize& cSize = m_pText->getContentSize();
	if (cSize.width > CHAT_CELL_MAX_WIDTH)
	{
		m_pText->setDimensions(CCSizeMake(CHAT_CELL_MAX_WIDTH, 0.0f));
	}

	// 聊天背景
	CCSize size(cSize.width + CHAT_CELL_TEXT_OFFSET, cSize.height + CHAT_CELL_TEXT_OFFSET);
	size.width = size.width<80 ? 80: size.width;
	size.height = size.height +10;
	float fPosY = m_oOriginPoint.y - (size.height - m_oOriginSize.height);

	user::UserInfo* pUserInfo = DM_GET_USER_MODEL->getUserInfo();
	m_pSystemBg->setVisible(false);
	m_pTitleSystemBg->setVisible(false);
	m_pTitleNormalBg->setVisible(false);
	m_pArrowSystem->setVisible(false);
	m_pSelfBj->setVisible(false);
	m_pOtherBj->setVisible(false);
	m_pArrow->setVisible(false);
	m_pArrowSelf->setVisible(false);

	if(pInfo->getUid()== 0)
	{
		m_pTitleSystemBg->setVisible(true);
		m_pSystemBg->setVisible(true);
		m_pSystemBg->setContentSize(size);
		m_pSystemBg->setPosition(m_oOriginPoint.x, fPosY);
		m_pArrowSystem->setVisible(true);
	}
	else if (pUserInfo && pUserInfo->getUserId() == pInfo->getUid())
	{
		m_pTitleNormalBg->setVisible(true);
		m_pSelfBj->setVisible(true);
		m_pSelfBj->setContentSize(size);
		m_pSelfBj->setPosition(m_oOriginPoint.x, fPosY);
		m_pArrowSelf->setVisible(true);
	}
	else
	{
		m_pTitleNormalBg->setVisible(true);
		m_pOtherBj->setVisible(true);
		m_pOtherBj->setContentSize(size);
		m_pOtherBj->setPosition(m_oOriginPoint.x, fPosY);
		m_pArrow->setVisible(true);
	}

	if (channel == chat::kChatChannelWorld)
	{
		m_fHeight = size.height + CHAT_WORLD_CELL_TOP_HEIGHT;
	}
	else if (channel == chat::kChatChannelGuild)
	{
		m_fHeight = size.height + CHAT_GUILD_CELL_TOP_HEIGHT;
	}
	else
	{
		m_fHeight = size.height + CHAT_GUILD_CELL_TOP_HEIGHT;
	}
	
	m_pNode->setPositionY(-fPosY);
}

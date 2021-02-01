//
//  GuildDetailsView.h
//  公会详情
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildDetailsView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildDetailsCell.h"


#define UNSELECT -1

GuildDetailsView::GuildDetailsView()
	:m_pModel(NULL)
	,m_pButtonCancel(NULL)
	,m_pContainer(NULL)
	,m_pDelegate(NULL)
	,m_pListView(NULL)
	,m_pLabelAnnouncement(NULL)
	,m_pLabelLevel(NULL)
	,m_pLabelName(NULL)
	,m_pLabelCount(NULL)
	,m_pLabelRank(NULL)
	,m_iGuildID(0)
	,m_pGuildInfo(NULL)
	,m_pGuildUsers(NULL)
	,m_pMarqueeContainer(NULL)
	,m_pMarqueeAnnouncement(NULL)
{
}

GuildDetailsView::~GuildDetailsView()
{
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pLabelAnnouncement);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelCount);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelRank);
	CC_SAFE_RELEASE(m_pMarqueeContainer);
}

GuildDetailsView* GuildDetailsView::create()
{
	GuildDetailsView *pRet = new GuildDetailsView();
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

bool GuildDetailsView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_paihang_xinxi");
	m_pListView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pListView->retain();
	m_pContainer->addChild(m_pListView);

	m_pButtonCancel->setTarget(this, menu_selector(GuildDetailsView::callBackCancel));
	
	m_pLabelAnnouncement->setString("");
	m_pMarqueeAnnouncement = DMarqueeView::create(m_pMarqueeContainer->getContentSize().width, ccc3(0x00, 0x00, 0x00), D_FONT_DEFAULT, NOTICE_FONT_SIZE);
	//m_pMarqueeAnnouncement->setPosition(m_pLabelAnnouncement->getPosition());
	//m_pMarqueeAnnouncement->setAnchorPoint(m_pLabelAnnouncement->getAnchorPoint());
	m_pMarqueeContainer->addChild(m_pMarqueeAnnouncement);

	return true;
}

void GuildDetailsView::onEnter()
{
	DLayer::onEnter();


	BINDING_ONLY(m_pModel, this, GuildDetailsView::updateView, D_EVENT_GUILD_DETAILS);
}

void GuildDetailsView::updateView()
{
	m_pGuildInfo = m_pModel->getOtherGuild(m_iGuildID);
	m_pGuildUsers = m_pModel->getOtherGuildUsers(m_iGuildID);
	m_pLabelLevel->setString(CCString::createWithFormat("%d", m_pGuildInfo->getLevel())->getCString());
	m_pLabelName->setString(m_pGuildInfo->getName().c_str());
	m_pMarqueeAnnouncement->setStack(0);				//remove all text
	m_pMarqueeAnnouncement->setFirst(true);
	m_pMarqueeAnnouncement->addString(m_pGuildInfo->getAnnouncement().c_str());
	m_pLabelCount->setString(CCString::createWithFormat("%u/%u", m_pGuildInfo->getUserCount(), D_CONFIG_UINT_FOR_KEY(GUILD_MEMBER_MAX))->getCString());
	m_pListView->reloadData();
}

void GuildDetailsView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildDetailsView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", CCMenuItemImage *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank", CCLabelBMFont *, this->m_pLabelRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_announcement", CCRichLabelTTF *, this->m_pLabelAnnouncement);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_count", CCLabelBMFont *, this->m_pLabelCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMarqueeContainer", CCNode *, this->m_pMarqueeContainer);

	return false;
}

void GuildDetailsView::callBackCancel(CCObject *sender)
{
	m_pDelegate->closeGuildDetailsView();
}


CCTableViewCell* GuildDetailsView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildDetailsCell *cell = (GuildDetailsCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildDetailsCell::create();
	}
	guild::GuildUserBasics *pData = (guild::GuildUserBasics *)m_pGuildUsers->objectAtIndex(idx);
	cell->setData(pData);
	return cell;
}

uint32_t GuildDetailsView::numberOfCellsInTableView(CCTableView *table)
{
	if (NULL == m_pGuildUsers)
	{
		return 0;
	}
	else
	{
		return m_pGuildUsers->count();
	}
}


CCSize GuildDetailsView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return DETAILS_CELL_SIZE;
}

void GuildDetailsView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
/*	m_iSelectIndex = m_iSelectIndex == cell->getIdx() ?UNSELECT :cell->getIdx();
	DTableView * DTable = dynamic_cast<DTableView *>(table);
	DTable->reloadData();
*/}


void GuildDetailsView::setRank(uint32_t iRank)
{
	m_pLabelRank->setString(CCString::createWithFormat("%u", iRank)->getCString());
}
//
//  GuildRankView.h
//  公会排名
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildRankView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildRankCell.h"
#define UNSELECT -1
#define  EQUIP_TAB_RECT		CCRectMake(0, 212, 640, 552)
#include "GuildRankTable.h"
GuildRankView::GuildRankView()
	:m_pModel(NULL)
	,m_pContainer(NULL)
	,m_pButtonCancel(NULL)
	,m_pTableNode(NULL)
//	,m_pListView(NULL)
	,m_pDelegate(NULL)
	,m_iSelectIndex(UNSELECT)
//	,m_iType(0)
	,m_pDTabView(NULL)
	,m_pDetailsView(NULL)
	,m_pGuildRank(NULL)
{
}

GuildRankView::~GuildRankView()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pTableNode);
//	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pDetailsView);
	CC_SAFE_RELEASE(m_pGuildRank);
}

GuildRankView* GuildRankView::create()
{
	GuildRankView *pRet = new GuildRankView();
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

bool GuildRankView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildRank");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_paihang");
/*	m_pListView = DTableView::create(this, m_pContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->retain();
	m_pContainer->addChild(m_pListView);*/

	//m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2), m_pContainer->getContentSize().width, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height+9, -PL_MAX_HEIGHT_DIFF)),50);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, m_pContainer->getPosition().y, m_pContainer->getContentSize().width, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height+9, -PL_MAX_HEIGHT_DIFF)),50);
	//m_pDTabView = DTabView::create(EQUIP_TAB_RECT,360);
	m_pDTabView->retain();
	
	GuildRankTable *guildTable = GuildRankTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF) , 0);
	GuildRankTable *userTable = GuildRankTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF) , 1);
	guildTable->setType(0);
	userTable->setType(1);
	m_pDTabView->addTab(guildTable,"","PL_ui/all/all_btn_fenye_lan_up.png","PL_ui/all/all_btn_fenye_lan.png",NULL,"PL_ui/banghui/banghui_btn_txt_banghui.png");
	m_pDTabView->addTab(userTable,"","PL_ui/all/all_btn_fenye_lv_up.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/banghui/banghui_btn_txt_gongxian.png"); 
	//m_pDTabView->addTabWithFontImg(guildTable,"ui/common/font_sheet_guild.png");
	//m_pDTabView->addTabWithFontImg(userTable,"ui/common/font_sheet_contribution.png");
	m_pTableNode->addChild(m_pDTabView);
	m_pButtonCancel->setTarget(this, menu_selector(GuildRankView::callBackCancel));
	guildTable->setDelegate(this);

	return true;
}

void GuildRankView::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pModel, this, GuildRankView::updateView, D_EVENT_GUILD_GET_LIST);
//	HTTP_CLIENT->guildGetList(0);
}

void GuildRankView::updateView()
{
	//m_pListView->reloadData();

	//我自己的排名
	CCArray* pList = m_pModel->getGuildList();
	CCObject* pObj = NULL;
	unsigned int rank = 0;
	CCARRAY_FOREACH(pList , pObj)
	{
		rank++;
		guild::GuildBasics *guildData = dynamic_cast<guild::GuildBasics *>(pObj);
		if(guildData->getGuildID() == m_pModel->getSelfInfo()->getGuildID())
		{
			m_pGuildRank->setString(D_CSTRING_FROM_UINT(rank));
			break;
		}
	}
}

void GuildRankView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildRankView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", CCMenuItemImage *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildRank", CCLabelBMFont *, this->m_pGuildRank);
	return false;
}


void GuildRankView::callBackCancel(CCObject *sender)
{
	//m_pListView->reloadData();
	m_pDelegate->willCloseRankView();
}


void GuildRankView::willShowGuildDetails(uint32_t guildID, uint32_t rank)
{
	if (NULL == m_pDetailsView)
	{
		m_pDetailsView = GuildDetailsView::create();
		m_pDetailsView->retain();
		m_pDetailsView->setDelegate(this);
	}
	if (m_pDelegate)
	{
		m_pDelegate->openGuildDetail(m_pDetailsView);
	}
	m_pDetailsView->setGuildID(guildID);
	m_pDetailsView->setRank(rank);
	m_pModel->syncGuildByID(guildID);
}


void GuildRankView::closeGuildDetailsView()
{
	m_pDetailsView->removeFromParent();
}
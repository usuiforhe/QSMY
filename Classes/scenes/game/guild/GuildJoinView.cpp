//
//  GuildJoinView.h
//  加入公会
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildJoinView.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "GuildJoinCell.h"
#include "DLoadingTabelView.h"


#define UNSELECT -1
#define GUILD_JOIN_PAGE_SIZE 20


GuildJoinView::GuildJoinView()
	:m_pModel(NULL)
	,m_pButtonCancel(NULL)
	,m_pButtonSearch(NULL)
	,m_pBoxContainer(NULL)
	,m_pMenuList(NULL)
	,m_pContainer(NULL)
	,m_pListView(NULL)
	,m_pDelegate(NULL)
	,m_iSelectIndex(UNSELECT)
	,m_pSearchView(NULL)
	,m_pDetailsView(NULL)
	,m_iLastCount(0)
    ,m_bScrollToTop(false)
	,m_pEditBox(NULL)
{
}

GuildJoinView::~GuildJoinView()
{
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pButtonSearch);
	CC_SAFE_RELEASE(m_pBoxContainer);
	CC_SAFE_RELEASE(m_pMenuList);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pSearchView);
	CC_SAFE_RELEASE(m_pDetailsView);
}

GuildJoinView* GuildJoinView::create()
{
	GuildJoinView *pRet = new GuildJoinView();
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

bool GuildJoinView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guildJoin");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_liebiao");

	m_pListView = DLoadingTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->retain();
	m_pListView->setLoadingDelegate(this);
	m_pContainer->addChild(m_pListView);


	m_pButtonSearch->getButton()->setTarget(this, menu_selector(GuildJoinView::callBackSearch));
	//m_pButtonCancel->getButton()->setTarget(this, menu_selector(GuildJoinView::callBackCancel));
	m_pMenuList->setTarget(this, menu_selector(GuildJoinView::getGuildList));
	initEditBox();

	return true;
}

void GuildJoinView::onEnter()
{
	DLayer::onEnter();


	BINDING_ONLY(m_pModel, this, GuildJoinView::updateView, D_EVENT_GUILD_GET_LIST);
	HTTP_CLIENT->guildGetList(0);
	//if (m_pModel->getGuildList() == NULL || m_pModel->getGuildList()->count() == 0)
	//{
	//	HTTP_CLIENT->guildGetList(0);
	//}
	//else
	//{
	//	this->updateView();
	//}
}

void GuildJoinView::getGuildList(cocos2d::CCObject*)
{
	HTTP_CLIENT->guildGetList(0);
}

void GuildJoinView::updateView()
{
	m_pListView->reloadData();
    if (m_bScrollToTop) {
        m_bScrollToTop = false;
        m_pListView->setContentOffset(ccp(0, -m_pListView->getContentSize().height + m_pContainer->getContentSize().height));
	}
	if (m_pModel->getGuildList()->count() - m_iLastCount < GUILD_JOIN_PAGE_SIZE)
	{
		m_pListView->setReachTheEnd(true);
	}
	else
	{
		m_pListView->setReachTheEnd(false);
	}
}

void GuildJoinView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
}

bool GuildJoinView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxContainer", CCNode *, this->m_pBoxContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", DButton *, this->m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_search", DButton *, this->m_pButtonSearch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuList", CCMenuItemImage *, this->m_pMenuList);
	return false;
}

void GuildJoinView::initEditBox()
{

	m_pEditBox = CCEditBox::create(m_pBoxContainer->getContentSize(), CCScale9Sprite::create());
	m_pEditBox->setFont(D_FONT_DEFAULT,D_FONT_SIZE_14);
	m_pEditBox->setFontColor(ccWHITE);
	m_pEditBox->setPlaceHolder(D_LOCAL_STRING(GUILD_TXT_SEARCH_PLACEHOLDER).c_str());
	m_pEditBox->setPlaceholderFontColor(ccc3(117, 117, 117));
	m_pEditBox->setMaxLength(15);
	m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_pEditBox->setDelegate(this);
	m_pEditBox->setAnchorPoint(CCPointZero);

	m_pBoxContainer->addChild(m_pEditBox);
}

void GuildJoinView::callBackSearch(CCObject *sender)
{
	if(strlen(m_pEditBox->getText())==0)
	{
		Tips_Alert(D_LOCAL_STRING("GuildNameError").c_str());
	}
	else
	{
		searchGuildWithName(m_pEditBox->getText());
		m_pEditBox->setText("");
	}
}

void GuildJoinView::callBackCancel(CCObject *sender)
{
    if (!m_pListView->getLoadingEnabled()) {
        m_bScrollToTop = true;
        m_pListView->setLoadingEnabled(true);
        HTTP_CLIENT->guildGetList(0);
        return;
    }
	m_pDelegate->closeGuildSearchView();
}


CCTableViewCell* GuildJoinView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildJoinCell *cell = (GuildJoinCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildJoinCell::create();
		//cell->getSelectButton()->setTarget(this, menu_selector(GuildJoinView::onTouch));
		cell->setDelegate(this);
	}
	cell->setExpanded(idx == m_iSelectIndex);
	guild::GuildBasics *pData = (guild::GuildBasics *)m_pModel->getGuildList()->objectAtIndex(idx);
	cell->setData(pData);
	cell->setRank(idx + 1);
	//cell->getSelectButton()->setUserData(cell);
	return cell;
}

uint32_t GuildJoinView::numberOfCellsInTableView(CCTableView *table)
{
	CCArray * guildList = m_pModel->getGuildList();
	if (NULL == guildList)
	{
		return 0;
	}
	else
	{
		return guildList->count();
	}
	
}


CCSize GuildJoinView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	/*if (m_iSelectIndex == idx)
	{
		return JOIN_CELL_SIZE_EXPANDED;
	}
	else
	{
		return JOIN_CELL_SIZE;
	}*/
	return JOIN_CELL_SIZE;
}

void GuildJoinView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
/*	m_iSelectIndex = m_iSelectIndex == cell->getIdx() ?UNSELECT :cell->getIdx();
	DTableView * DTable = dynamic_cast<DTableView *>(table);
	DTable->reloadData();
*/}


void GuildJoinView::onTouch(cocos2d::CCObject* obj)
{
	cocos2d::CCMenuItem* pItem = static_cast<cocos2d::CCMenuItem*>(obj);

	GuildJoinCell* pCell = static_cast<GuildJoinCell*>(pItem->getUserData());
	if(!pCell) return;
	//CCLOG("equip cell touched at index: %i,%i", idx,pCell->getIdx());
	if(m_iSelectIndex == pCell->getIdx())
	{
		m_iSelectIndex = - 1;
		m_pListView->updateCellPositions(pCell);
		m_pListView->reloadData();
	}
	else
	{
		m_iSelectIndex = pCell->getIdx();
		m_pListView->updateCellPositions(pCell);
		m_pListView->reloadData();
	}
}


void GuildJoinView::willApplyForTheGuild(uint32_t guildID, int action)
{
	if (action == 1)
	{
		if (m_pModel->getSelfApplication()->count() >= D_CONFIG_UINT_FOR_KEY(GUILD_APPLICATION_MAX))
		{
			Tips_Alert(D_LOCAL_STRING("GuildApplyError").c_str());
			return;
		}
	}
	HTTP_CLIENT->guildApply(guildID, action, this, callfuncJson_selector(GuildJoinView::willApplyForTheGuild));
}

void GuildJoinView::willApplyForTheGuild(const Json::Value &,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	m_pListView->reloadData();
}

void GuildJoinView::willShowGuildDetails(uint32_t guildID, uint32_t rank)
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

void GuildJoinView::searchGuildWithName(const char * pGuildName)
{
	HTTP_CLIENT->guildSearch(pGuildName, this, callfuncJson_selector(GuildJoinView::searchGuildWithName));
	if(m_pSearchView) m_pSearchView->removeFromParent();
}

void GuildJoinView::searchGuildWithName(const Json::Value &,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) 
	{
		//Tips_Alert(D_LOCAL_STRING("GuildSearchError").c_str());
	}
    else
    {
        m_pListView->setLoadingEnabled(false);
    }
}

void GuildJoinView::closeGuildSearchView()
{
	m_pSearchView->removeFromParent();
}

void GuildJoinView::closeGuildDetailsView()
{
	m_pDetailsView->removeFromParent();
}

void GuildJoinView::startLoading()
{
	m_iLastCount = m_pModel->getGuildList()->count();
	HTTP_CLIENT->guildGetList(m_pModel->getGuildList()->count(), this, callfuncJson_selector(GuildJoinView::startLoading));
}

void GuildJoinView::startLoading(const Json::Value &,const Json::Value &responseData)
{
	if (0 != m_iLastCount)
	{
		m_pListView->endLoading();
	}
	if (m_pModel->getGuildList()->count() - m_iLastCount < GUILD_JOIN_PAGE_SIZE)
	{
		m_pListView->setReachTheEnd(true);
	}
}
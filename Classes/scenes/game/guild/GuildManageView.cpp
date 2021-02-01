//
//  GuildManageView.h
//  公会管理
//
//  Created by Eci on 13-10-24.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildManageView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildManageTable.h"
#include "GuildApplyTable.h"
//#include "GuildLogTable.h"
#define  RANK_CELL_SIZE        CCSizeMake(640, 112)
#define UNSELECT -1
#define  EQUIP_TAB_RECT		CCRectMake(0, 200, 640, 534)

GuildManageView::GuildManageView()
	:m_pModel(NULL)
	,m_pContainer(NULL)
	,m_pButtonCancel(NULL)
	,m_pDelegate(NULL)
	,m_iSelectIndex(UNSELECT)
	,m_pTabView(NULL)
	,m_pDemiseView(NULL)
    //,m_pNodeRequestNum(NULL)
{
}

GuildManageView::~GuildManageView()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pTabView);
	//CC_SAFE_RELEASE(m_pDemiseView);
    //CC_SAFE_RELEASE(m_pNodeRequestNum);
}

GuildManageView* GuildManageView::create()
{
	GuildManageView *pRet = new GuildManageView();
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

bool GuildManageView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

    cocos2d::CCNode *pRoot = NULL;
	CCB_READER("PL_ui/ccb/pl_banghui/pl_banghui_guanli",pRoot);
	if(pRoot) addChild(pRoot);    

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, -PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height + 12, -PL_MAX_HEIGHT_DIFF);
	m_pTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),9);
	m_pTabView->retain();

	GuildManageTable *guildTable = GuildManageTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pTabView->addTab(guildTable,"","PL_ui/all/all_btn_fenye_lan_up.png","PL_ui/all/all_btn_fenye_lan.png",NULL,"PL_ui/banghui/banghui_btn_txt_chengyuan.png");
	guildTable->setDelegate(this);
	//font_btn_apply;
	//GuildLogTable *logTable = GuildLogTable::create(m_pContainer->getContentSize());
    //m_pTabView->addTabWithFontImg(logTable, "PL_ui/banghui/banghui_btn_txt_chengyuan.png");

    GuildApplyTable *applyTable = GuildApplyTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pTabView->addTab(applyTable,"","PL_ui/all/all_btn_fenye_lv_up.png","PL_ui/all/all_btn_fenye_lv.png",NULL,"PL_ui/banghui/banghui_btn_txt_shenqing.png"); 
	pRoot->addChild(m_pTabView);
	m_pButtonCancel->setTarget(this, menu_selector(GuildManageView::callBackCancel));

    //m_pNodeRequestNum->setZOrder(1);
    
	return true;
}

void GuildManageView::onEnter()
{
    BINDING_ONLY(m_pModel, this, GuildManageView::updateView, D_EVENT_GUILD_APPLY_LIST);
    m_pModel->syncGuildApplyList();
	DLayer::onEnter();
}

void GuildManageView::updateView()
{
    uint32_t count = m_pModel->getGuildApplication()->count();
	/*m_pNodeRequestNum->setVisible(count > 0 && (m_pModel->getSelfInfo()->getPosition()==guild::GuildPositionAssistant||m_pModel->getSelfInfo()->getPosition()==guild::GuildPositionPresident));*/
}

void GuildManageView::onExit()
{
    UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

bool GuildManageView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", CCMenuItem *, this->m_pButtonCancel);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRequestNum", CCNode *, m_pNodeRequestNum);
	return false;
}


void GuildManageView::callBackCancel(CCObject *sender)
{
	m_pDelegate->willCloseManageView();
}

void GuildManageView::willShowDemiseView()
{
	if (NULL == m_pDemiseView)
	{
		m_pDemiseView = GuildDemiseView::create();
		//m_pDemiseView->retain();
		m_pDemiseView->setDelegate(this);
	}
	if (m_pDelegate)
	{
		//m_pDelegate->openUpperPopup(m_pDemiseView);
		Game::sharedGame()->openPopup(m_pDemiseView);
	}
}

void GuildManageView::willShowFormation(DLayer* pView)
{
	if (m_pDelegate && pView)
	{
		m_pDelegate->openUpperPopup(pView);
	}
}

void GuildManageView::closeGuildDemiseView()
{
	//m_pDemiseView->removeFromParent();
	m_pDemiseView = NULL;
}

void GuildManageView::demiseToUser(uint32_t userID)
{
	HTTP_CLIENT->guildPosition(userID, guild::GuildPositionPresident, this, callfuncJson_selector(GuildManageView::demiseToUser));
}


void GuildManageView::demiseToUser(const Json::Value &,const Json::Value &responseData)
{

	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	m_pDemiseView->removeFromParent();
	m_pDemiseView = NULL;
}

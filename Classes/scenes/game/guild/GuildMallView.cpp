///////////////////////////////////////////////////////////////////////////////////////////////////
//  
// GuildMallView.cpp
// QSMY
// 门派商店
//
// Created by jin.xia on 14-07-04.
// Copyright (c) 2013-2014 thedream. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "utils/TimerUtil.h"

#include "GuildHead.h"
#include "GuildMallCell.h"
#include "effect/GuildExpEffect.h"
#include "GuildMallView.h"

USING_NS_CC ;
USING_NS_CC_EXT ;
using namespace sp ;
using namespace guild ;
using namespace drop ;


GuildMallView::GuildMallView()
	:m_pLabelName(NULL),
	m_pLabelPosition(NULL),
	m_pLabelLevel(NULL),
	m_pLabelExp(NULL),
	m_pExpBar(NULL),
	m_pLabelNextExp(NULL),
	m_pDelegate( NULL )
//,m_pGuildHead( NULL )
,m_pBackBtn( NULL )
,m_pRefreshBtn( NULL )
,m_pContainer( NULL )
,m_pCrtContributeTF( NULL )
,m_pTotalContributeTF( NULL )
,m_pRemainTimeTF( NULL )
,m_pRemainCountTF( NULL )
,m_pPoint(NULL)
,m_isExpShown( false )
{

}

GuildMallView::~GuildMallView()
{
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelPosition);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelExp);
	CC_SAFE_RELEASE(m_pExpBar);
	CC_SAFE_RELEASE(m_pLabelNextExp);

    //CC_SAFE_RELEASE( m_pGuildHead ) ;
    CC_SAFE_RELEASE( m_pBackBtn ) ;
    CC_SAFE_RELEASE( m_pRefreshBtn ) ;
    CC_SAFE_RELEASE( m_pContainer ) ;
    CC_SAFE_RELEASE( m_pCrtContributeTF ) ;
    CC_SAFE_RELEASE( m_pTotalContributeTF ) ;
    CC_SAFE_RELEASE( m_pRemainTimeTF ) ;
    CC_SAFE_RELEASE( m_pRemainCountTF ) ;
	CC_SAFE_RELEASE(m_pPoint);
}

GuildMallView* GuildMallView::create()
{
    GuildMallView *pRet = new GuildMallView() ;
    if ( pRet && pRet->init() )
    { 
        pRet->autorelease() ; 
        return pRet ; 
    } 
    CC_SAFE_DELETE( pRet ) ; 
    return NULL; 
}

bool GuildMallView::init()
{
    if ( !DLayer::init() )
    {
        return false ;
    }
    m_pGuildModel = DM_GET_GUILD_MODEL ;

    CCNodeLoaderLibrary *pNodeLoaderLibrary = 
        CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary() ;
    CUSTOM_CCB_LOADER( pNodeLoaderLibrary ) ;
    pNodeLoaderLibrary->registerCCNodeLoader( "GuildHead", GuildHeadLoader::loader() ) ;
    //CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/guild/guildStore", pNodeLoaderLibrary ) ;
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_shop", pNodeLoaderLibrary ) ;
    
    //m_pBackBtn->setTarget( this, menu_selector( GuildMallView::backHandler ) ) ;
    m_pRefreshBtn->getButton()->setTarget( this, menu_selector( GuildMallView::refreshHandler ) ) ;

    m_pListView = DTableView::create( this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
    m_pListView->setDirection( kCCScrollViewDirectionVertical ) ;
    m_pListView->setVerticalFillOrder( kCCTableViewFillTopDown ) ;
    m_pListView->setDelegate( this ) ;
    m_pContainer->addChild( m_pListView ) ;

    return true;
}

void GuildMallView::onEnter()
{
    DLayer::onEnter() ;
    BINDING_EXEC( m_pGuildModel, this, GuildMallView::updateView, D_EVENT_GUILD_MALL_REFRESH ) ;
    TimerUtil::sharedTimerUtil()->scheduleSelector( 
        schedule_selector( GuildMallView::update ), this, 1, false ) ;
}

void GuildMallView::onExit()
{
    DLayer::onExit() ;
    UNBINDING_ALL( m_pGuildModel, this ) ;
    TimerUtil::sharedTimerUtil()->unscheduleSelector( 
        schedule_selector( GuildMallView::update ), this ) ;
}

void GuildMallView::updateView()
{
    GuildInfo *guildInfo = m_pGuildModel->getGuildInfo() ;
    GuildUser *selfInfo = m_pGuildModel->getSelfInfo() ;
	if(guildInfo && selfInfo) GUILD_HEAD_UPDATE(guildInfo , selfInfo);
 //   // 更新帮会信息
 //   m_pGuildHead->setLevel( guildInfo->getLevel() ) ;
 //   m_pGuildHead->setName( guildInfo->getName().c_str() ) ;
 //   m_pGuildHead->setUserExp( selfInfo->getNextExp() ) ;
	//m_pGuildHead->setKnighthood( selfInfo->getKnighthoodName().c_str() ) ;
	//if ( m_isExpShown )
	//{
	//	m_pGuildHead->setGuildExp(guildInfo->getContributionLevel(), 
	//		guildInfo->getContributionNext(), .2f);
	//}
	//else
	//{
	//	m_pGuildHead->setGuildExp(guildInfo->getContributionLevel(), 
	//		guildInfo->getContributionNext(), .0f);
	//	m_isExpShown = true;
	//}

    // 更新个人帮会信息
    m_pTotalContributeTF->setString( CCString::createWithFormat( 
        "%d", selfInfo->getContribution() )->getCString() ) ;
    m_pCrtContributeTF->setString( CCString::createWithFormat( 
        "%d", selfInfo->getRemainContribution() )->getCString() ) ;
    m_pRemainCountTF->setString( CCString::createWithFormat( 
        "%d", m_pGuildModel->getGuildMall()->getRefreshCount() )->getCString() ) ;

	m_pPoint->setString(D_CSTRING_FROM_UINT(D_CONFIG_UINT_FOR_KEY(GUILD_MALL_REFRESH_COST)));
    m_pListView->reloadData() ;
	update( 0 ) ;
}

void GuildMallView::update( float dt )
{
    time_t now = GameManager::sharedGameManager()->getServerTime() ;
    time_t remainTime = TimeUtil::getTimeDiffToTomorrow( now ) ;
    m_pRemainTimeTF->setString( TimeUtil::getSeconds2String( remainTime, "%H:%M:%S" ) ) ;
}

bool GuildMallView::onAssignCCBMemberVariable( CCObject* pTarget, 
        const char *pMemberVariableName, CCNode *pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_position", CCLabelTTF *, this->m_pLabelPosition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp", CCLabelBMFont *, this->m_pLabelExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp_bar", DProgressBar *, this->m_pExpBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_next_exp", CCLabelBMFont *, this->m_pLabelNextExp);

    //CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "guild_head", 
    //    GuildHead *, this->m_pGuildHead ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "guild_cancel", 
        CCMenuItem *, this->m_pBackBtn ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pRefreshBtn", 
        DButton *, this->m_pRefreshBtn ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pContainer", 
        CCNode *, this->m_pContainer ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pCrtContribute", 
        CCLabelBMFont *, this->m_pCrtContributeTF ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pTotalContribute", 
        CCLabelBMFont *, this->m_pTotalContributeTF ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pRemainTime", 
        CCLabelBMFont *, this->m_pRemainTimeTF ) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pRemainCount", 
        CCLabelBMFont *, this->m_pRemainCountTF ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pPoint", CCLabelBMFont *, this->m_pPoint ) ;	
    return false ;
}

void GuildMallView::backHandler( CCObject *sender )
{
    if ( m_pDelegate )
    {
        m_pDelegate->willCloseStoreView() ;
    }
}

void GuildMallView::refreshHandler( CCObject *sender )
{
    int count = m_pGuildModel->getGuildMall()->getRefreshCount() ;
    if ( count <= 0 )
    {
        Tips_Alert( D_LOCAL_STRING( "guildMallRefreshNoCount" ).c_str() ) ;
        return ;
    }
    if ( m_pGuildModel->getSelfInfo()->getRemainContribution() 
		< D_CONFIG_UINT_FOR_KEY(GUILD_MALL_REFRESH_COST) )
    {
        Tips_Alert( D_LOCAL_STRING( "GuildMallRefreshNotEnough" ).c_str() ) ;
        return ;
    }
    HTTP_CLIENT->guildMallRefresh() ;
}

///////////////////////////// CCTableViewDataSource /////////////////////////////
CCTableViewCell *GuildMallView::tableCellAtIndex( CCTableView *table, uint32_t idx )
{
    GuildMallCell *cell = ( GuildMallCell * )table->dequeueCell() ;
    if ( !cell )
    {
        cell = GuildMallCell::create() ;
    }
    guild::GuildMall *m_pGuildMall = m_pGuildModel->getGuildMall() ;
    CCArray *goodIDArr = m_pGuildMall->getGoodIdArr() ;
    if ( goodIDArr && idx < goodIDArr->count() )
    {
        CCString *str = (CCString *)(goodIDArr->objectAtIndex( idx ) ) ;
        cell->setData( m_pGuildModel->getMallGoodsById( str->intValue() ), idx ) ;
        cell->setDelegate( m_pDelegate ) ;
    }
    else
    {
        cell->setData( NULL, idx ) ;
        cell->setDelegate( NULL ) ;
    }
    return cell ;
}

uint32_t GuildMallView::numberOfCellsInTableView( CCTableView *table )
{
    GuildInfo *guildInfo = m_pGuildModel->getGuildInfo() ;
    //m_pGuildHead->setLevel( guildInfo->getLevel() ) ;
    // 门派商店内初始只有一种可兑换物品，每10级增加一样可兑换物品
    // 显示两个未开放的格子
    int num = ( guildInfo->getLevel() ) / D_CONFIG_INT_FOR_KEY(GUILD_MALL_CELL_STEP)
		+ D_CONFIG_INT_FOR_KEY(GUILD_MALL_CELL_INIT) 
		+ D_CONFIG_INT_FOR_KEY(GUILD_MALL_CELL_LOCK) ;
    if ( num > D_CONFIG_INT_FOR_KEY(GUILD_MALL_CELL_MAX) )
    {
        num = D_CONFIG_INT_FOR_KEY(GUILD_MALL_CELL_MAX) ;
    }
    return num ;
}

CCSize GuildMallView::tableCellSizeForIndex( CCTableView *table, unsigned int idx )
{
    return GUILD_STORE_TREE_CELL_SIZE ;
}

///////////////////////////// CCTableViewDelegate /////////////////////////////
void GuildMallView::tableCellTouched( CCTableView *table, CCTableViewCell *cell )
{
}

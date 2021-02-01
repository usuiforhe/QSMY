///////////////////////////////////////////////////////////////////////////////////////////////////
//  
// GuildMallCell.cpp
// QSMY
// 门派商店列表格
//
// Created by jin.xia on 14-07-04.
// Copyright (c) 2013-2014 thedream. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "GuildMallCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

USING_NS_CC ;
USING_NS_CC_EXT ;
using namespace Json ;
using namespace drop ;
using namespace item ;
using namespace warrior ;
using namespace guild ;


GuildMallCell::GuildMallCell()
:m_pGuildModel( NULL )
,m_pMallGoods( NULL )
,m_pDrop( NULL )
,m_iCellId( 0 )
,m_iIndex( 0 )
,m_pLockNode( NULL )
,m_pUnlockLevelTF( NULL )
,m_pUnlockNode( NULL )
,m_pContainer( NULL )
,m_pHeroContainer(NULL)
,m_pNameTF( NULL )
,m_pNumTF( NULL )
,m_pContributeTF( NULL )
,m_pDressMark( NULL )
,m_pSoulMark( NULL )
,m_pExchangeSp( NULL )
,m_pExchangedSp( NULL )
,m_pExchangeBtn( NULL )
,m_pViewBtn( NULL )
,m_pPieceIcon(NULL)
,m_pSoulIcon(NULL)
{
}


GuildMallCell::~GuildMallCell()
{
	CC_SAFE_RELEASE_NULL( m_pDrop ) ;
	CC_SAFE_RELEASE( m_pMallGoods ) ;

	CC_SAFE_RELEASE( m_pLockNode ) ;
	CC_SAFE_RELEASE( m_pUnlockLevelTF ) ;
	CC_SAFE_RELEASE( m_pUnlockNode ) ;
	CC_SAFE_RELEASE( m_pContainer ) ;
	CC_SAFE_RELEASE(m_pHeroContainer);
	CC_SAFE_RELEASE( m_pNameTF ) ;
	CC_SAFE_RELEASE( m_pNumTF ) ;
	CC_SAFE_RELEASE( m_pContributeTF ) ;
	CC_SAFE_RELEASE( m_pDressMark ) ;
	CC_SAFE_RELEASE( m_pSoulMark ) ;
	CC_SAFE_RELEASE( m_pExchangeSp ) ;
	CC_SAFE_RELEASE( m_pExchangedSp ) ;
	CC_SAFE_RELEASE( m_pExchangeBtn ) ;
	CC_SAFE_RELEASE( m_pViewBtn ) ;
	CC_SAFE_RELEASE(m_pPieceIcon);
	CC_SAFE_RELEASE(m_pSoulIcon);
}

GuildMallCell* GuildMallCell::create()
{
	GuildMallCell *pRet = new GuildMallCell() ;
	if ( pRet && pRet->init() )
	{ 
		pRet->autorelease() ; 
		return pRet ; 
	} 
	CC_SAFE_DELETE( pRet ) ; 
	return NULL ; 
}

bool GuildMallCell::init()
{
	if ( !CCTableViewCell::init() )
	{
		return false ;
	}

	m_pGuildModel = DM_GET_GUILD_MODEL ;
	
	//CCB_READER_AND_ADDCHILD( "ui/ccb/guild/guildStoreCell" ) ;
	CCB_READER_AND_ADDCHILD( "PL_ui/ccb/pl_banghui/pl_banghui_shop_cell" ) ;
	m_pViewBtn->setTarget( this, menu_selector( GuildMallCell::showInfoHandler ) ) ;
	m_pExchangeBtn->getButton()->setTarget( this, menu_selector( GuildMallCell::requestExchange ) ) ;

	return true ;
}

void GuildMallCell::onEnter()
{
	CCTableViewCell::onEnter() ;
}

void GuildMallCell::onExit()
{
	CCTableViewCell::onExit() ;
}

bool GuildMallCell::onAssignCCBMemberVariable( CCObject* pTarget, 
        const char *pMemberVariableName, CCNode *pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pLock", 
		CCNode *, m_pLockNode ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pUnlockLevel", 
		CCLabelBMFont *, m_pUnlockLevelTF ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pUnlock", 
		CCNode *, m_pUnlockNode ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pContainer", 
		CCNode *, m_pContainer ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pHeroContainer", 
		CCNode *, m_pHeroContainer ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pName", 
		CCLabelTTF *, m_pNameTF ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pCount", 
		CCLabelBMFont *, m_pNumTF ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pContribute", 
		CCLabelBMFont *, m_pContributeTF ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pDressMark", 
		CCSprite *, m_pDressMark ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pSoulMark", 
		CCSprite *, m_pSoulMark ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pExchange", 
		CCSprite *, m_pExchangeSp ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pExchanged", 
		CCSprite *, m_pExchangedSp ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pExchangedBtn", 
		DButton *, m_pExchangeBtn ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pViewBtn", 
		CCMenuItem *, m_pViewBtn ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pPieceIcon", 
		CCNode *, m_pPieceIcon ) ;
	CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_pSoulIcon", 
		CCNode *, m_pSoulIcon ) ;
	return false ;
}

void GuildMallCell::setData( GuildMallGoods *pData, int idx )
{
	CC_SAFE_RELEASE_NULL( m_pDrop ) ;
	CC_SAFE_RELEASE( m_pMallGoods ) ;

	m_iIndex = idx ;
	m_iCellId = idx + 1 ;
	m_pMallGoods = pData ;
	CC_SAFE_RETAIN( m_pMallGoods ) ;
	bool isOpen = ( m_pMallGoods != NULL ) ;
	m_pLockNode->setVisible( !isOpen ) ;
	m_pUnlockNode->setVisible( isOpen ) ;
	//默认隐藏
	m_pPieceIcon->setVisible(false);
	m_pSoulIcon->setVisible(false);
	if ( isOpen )
	{
		//m_pDressMark->setVisible( false ) ;
		//m_pSoulMark->setVisible( false ) ;
		m_pNumTF->setString( CCString::createWithFormat( 
			"x%d", m_pMallGoods->num )->getCString() ) ;
		m_pContributeTF->setString( CCString::createWithFormat( 
			"%d", m_pMallGoods->cost )->getCString() ) ;
	    m_pContainer->removeAllChildrenWithCleanup( true ) ;
	    m_pHeroContainer->removeAllChildrenWithCleanup( true ) ;

		CCSprite *icon = NULL ;
		switch ( m_pMallGoods->type )
		{
			case drop::kDropTypeItem :
			{
				ItemInfo *pItemInfo = ItemInfo::create( m_pMallGoods->goodsID, 
					m_pMallGoods->num ) ;

	            m_pDrop = drop::Drop::create( pItemInfo ) ;
				CC_SAFE_RETAIN( m_pDrop ) ;
				m_pNameTF->setString( pItemInfo->getBaseInfo()->getItemName().c_str() ) ;
			    icon = ResourceManager::sharedResourceManager()
				    ->getItemIcon50PForList( m_pMallGoods->goodsID ) ;

				//装备碎片标志
				if(pItemInfo->getBaseInfo()->getItemType() == item::kItemTypeFragment)
				{
					m_pPieceIcon->setVisible(true);
					icon = ResourceManager::sharedResourceManager()->getEquipIcon40P(m_pDrop->getItem()->getBaseInfo()->getResID());
				}

				m_pContainer->addChild( icon ) ;
				break ;
			}

	        case drop::kDropTypeSoul :
			{
				m_pSoulIcon->setVisible(true);
				//m_pSoulMark->setVisible( true ) ;
				warrior::SoulInfo *pSoulInfo = SoulInfo::create( m_pMallGoods->goodsID, 
					m_pMallGoods->num ) ;
	            m_pDrop = drop::Drop::create( pSoulInfo ) ;
				CC_SAFE_RETAIN( m_pDrop ) ;
	            m_pNameTF->setString( pSoulInfo->getBaseInfo()->getWarriorName().c_str() ) ;
			    //icon = ResourceManager::sharedResourceManager()
				   // ->getWarriorIcon50P( m_pMallGoods->goodsID ) ;
			    //icon->setScale( 0.6f ) ;

				m_pHeroContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pMallGoods->goodsID));
	            break;
			} 

			case drop::kDropTypeEquip :
			{
				equip::EquipCurInfo* pInfo = equip::EquipCurInfo::create(m_pMallGoods->goodsID);
				m_pDrop = drop::Drop::create(pInfo);
				CC_SAFE_RETAIN( m_pDrop ) ;
				m_pNameTF->setString( pInfo->getBaseInfo()->getEquipName().c_str() ) ;
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon40P(m_pMallGoods->goodsID));
				break;
			} 

			case drop::kDropTypeCoin :
			{
				item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID);
				m_pDrop = drop::Drop::createCoin(m_pMallGoods->num);
				CC_SAFE_RETAIN( m_pDrop ) ;
				m_pNameTF->setString( pItemBase->getItemName().c_str() ) ;
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(COIN_ID));
				break;
			} 

			case drop::kDropTypePoint :
			{
				item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID);
				m_pDrop = drop::Drop::createCoin(m_pMallGoods->num);
				CC_SAFE_RETAIN( m_pDrop ) ;
				m_pNameTF->setString( pItemBase->getItemName().c_str() ) ;
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(POINT_ID));
				break;
			} 
		}
	    //if ( icon )
	    //{
	    //	m_pContainer->addChild( icon ) ;
	    //}

	    // 设置已经兑换
	    bool isExchanged = false ;
	    CCArray *pIDArr = m_pGuildModel->getGuildMall()->getExchangedIdArr() ;
	    for ( unsigned int i = 0; i < pIDArr->count(); ++i )
	    {
			CCString *str = (CCString *)( pIDArr->objectAtIndex( i ) ) ;
			if ( str->intValue() == m_iCellId )
	    	{
	    		isExchanged = true ;
	    		break ;
	    	}
	    }
		setExchanged( isExchanged ) ;
	}
	else
	{
	    // 门派商店内初始只有一种可兑换物品，每10级增加一样可兑换物品
		m_pUnlockLevelTF->setString( CCString::createWithFormat( "%d", 
			m_iIndex * D_CONFIG_UINT_FOR_KEY(GUILD_MALL_CELL_STEP) )->getCString() ) ;
	}
}

void GuildMallCell::setExchanged( bool isExchanged )
{
	//m_pExchangeSp->setVisible( !isExchanged ) ;
	//m_pExchangedSp->setVisible( isExchanged ) ;
	m_pExchangeBtn->getButton()->setEnabled( !isExchanged ) ;
}

void GuildMallCell::showInfoHandler( CCObject *sender )
{
    if ( m_pDelegate && m_pDrop )
    {
        switch ( m_pDrop->getType()  )
        {
            case kDropTypeItem :
                m_pDelegate->showItemInfoView( m_pDrop->getItem() ) ;
                break ;

            case kDropTypeSoul :
                m_pDelegate->showWarriorInfoView( m_pDrop->getSoul()->getBaseInfo() ) ;
                break ;
        }
    }
}

void GuildMallCell::requestExchange( CCObject *sender )
{
    int count = m_pGuildModel->getGuildMall()->getRefreshCount() ;
	if ( m_pGuildModel->getSelfInfo()->getRemainContribution() < m_pMallGoods->cost )
	{
        Tips_Alert( D_LOCAL_STRING( "GuildMallExchangeNotEnough" ).c_str() ) ;
        return ;
	}
	if ( m_pMallGoods && m_pMallGoods->id > 0 )
	{
		HTTP_CLIENT->guildMallExchange( m_iCellId, this, 
			callfuncJson_selector( GuildMallCell::responseExchange ) ) ;
	}
}

void GuildMallCell::responseExchange( const Value & requestData, const Value &responseData )
{
	if( responseData["code"].asString().compare( SUC_CODE ) == 0 )
	{
		NotificationCenter::sharedNotificationCenter()->postNotification( D_EVENT_DROPS_SHOW,
			cocos2d::CCInteger::create( requestData[UUID].asUInt() ) ) ;
	}
}

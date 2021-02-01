//
//  Guild.h
//  公会视图
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "Guild.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildView.h"
#include "GuildNoneView.h"
#include "GuildNoticeView.h"

Guild::Guild()
	: m_pModel(NULL)
	, m_pGuildView(NULL)
	, m_pNoneView(NULL)
	, m_pCreateView(NULL)
	, m_pJoinView(NULL)
	, m_pRankView(NULL)
	, m_pManageView(NULL)
	, m_pDelegate(NULL)
	, m_pBackground(NULL)
	, m_pCommonLayer(NULL)
	, m_pPopupLayer(NULL)
	, m_pPopupLayerUpper(NULL)
	, m_nGuildID(0)
	, m_pItemInfoView( NULL )
	, m_pSoulInfoView( NULL )
	, m_CreatePageType(kGuildPageNone)
	, m_CurrentPageType(kGuildPageNone)
	, m_pTombView(NULL)
	, m_pBossView(NULL)
{
}

Guild::~Guild()
{
	CC_SAFE_RELEASE(m_pGuildView);
	CC_SAFE_RELEASE(m_pNoneView);
	CC_SAFE_RELEASE(m_pCreateView);
	CC_SAFE_RELEASE(m_pJoinView);
	CC_SAFE_RELEASE(m_pRankView);
	CC_SAFE_RELEASE(m_pManageView);
	CC_SAFE_RELEASE(m_pTombView);
	CC_SAFE_RELEASE(m_pBossView);
	//CC_SAFE_RELEASE(m_pBackground);
	//CC_SAFE_RELEASE(m_pPopupLayer);
}

Guild* Guild::create(GuildPageType val)
{
	Guild *pRet = new Guild();
	if (pRet && pRet->init(val))
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

bool Guild::init(GuildPageType val)
{
	if(!DLayer::init())
	{
		return false;
	}
	m_CreatePageType = val;
	//m_pBackground = CCSprite::create("ui/common/bg_index_smooth.jpg");
	//m_pBackground->setScale(2.0);
	//m_pBackground->setAnchorPoint(CCPointZero);
	//m_pBackground->setPosition(CCPointZero);
	//this->addChild(m_pBackground);
	//m_pBackground->retain();

	m_pCommonLayer = CCNode::create();
	addChild(m_pCommonLayer, 0);

	m_pPopupLayer = CCNode::create();
	addChild(m_pPopupLayer, 1);
	//CC_SAFE_RETAIN(m_pPopupLayer);

	CCNode* pFooter = NULL;
	CCB_READER("PL_ui/ccb/pl_banghui/pl_banghui_xia", pFooter);
	addChild(pFooter , 2);

	m_pPopupLayerUpper = CCNode::create();
	addChild(m_pPopupLayerUpper, 3);
	//自适应
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pCommonLayer);
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pPopupLayer);
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pPopupLayerUpper);
	PL_MOVE_NODE_BELOW_CENTER_Y(pFooter);

	m_pModel = DM_GET_GUILD_MODEL;

	return true;
}

void Guild::openTopPopup(cocos2d::CCLayer* pLayer)
{
	m_pPopupLayerUpper->removeAllChildren();
	m_pPopupLayerUpper->addChild(pLayer);
}

SEL_MenuHandler Guild::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackFooter", Guild::footerCallBack);
	return NULL;
}

void Guild::footerCallBack(cocos2d::CCObject* obj)
{

	//按钮从左到右01234
	int tag = (dynamic_cast<CCNode*>(obj))->getTag();

	//是否有帮会
	bool bGuild = (m_nGuildID != 0);

	//先不上
	//if((GuildPageType)tag == kGuildPageTomb)
	//{
	//	Tips_Alert(D_LOCAL_STRING("$GuildTianxian").c_str());
	//	return;
	//}

	//特殊处理苍龙现
	//新创建的公会不能参加，不用弹筐显示
	bool isCreatedToday = DM_GET_GUILD_MODEL->getGuildInfo()->getIsCreatedToday();
	if (bGuild && isCreatedToday && (GuildPageType)tag == kGuildPageBoss)
	{
		Tips_Alert(D_LOCAL_STRING("GuildErrorNewGuild").c_str());
		return;
	}


	
	
	//清理上层
	m_pPopupLayer->removeAllChildren();
	m_pPopupLayerUpper->removeAllChildren();

	if(m_CurrentPageType == (GuildPageType)tag && m_CurrentPageType != kGuildPageNone)
	{
		/*已经在当前页了*/
		if(!bGuild)
		{
			GuildPageType pagetype = (GuildPageType)tag;
			if(pagetype == kGuildPageBoss || pagetype == kGuildPageTomb) showNoGuildError();
		}
	}
	else
	{
		//换页
		m_CurrentPageType = (GuildPageType)tag;
		//清理基层
		if(bGuild)
		{
			m_pCommonLayer->removeAllChildren();
		}
		
		//换页操作
		switch(tag)
		{
		case 0:
			{
				//清理界面回到主页
				removeFromParent();
				Game::sharedGame()->NavigateTo(navigation::kNavHome);
				break;
			}
		case kGuildPageGuild:
			{
				//回到帮会主界面
				if(bGuild)
				{
					if (NULL == m_pGuildView)
					{
						m_pGuildView = GuildView::create();
						m_pGuildView->retain();
						m_pGuildView->setDelegate(this);
					}
					m_pCommonLayer->addChild(m_pGuildView);
					m_pGuildView->updateViewWithDurOfPB(.0f);
				}
				else
				{
					updateView();
				}
					
				break;
			}
		case kGuildPageBoss:
			{
				//苍龙现
				if(bGuild)
				{
					if (NULL == m_pBossView)
					{
						m_pBossView = GuildBoss::create();
						m_pBossView->retain();
					}
					m_pCommonLayer->addChild(m_pBossView);
				}
				else
				{
					showNoGuildError();
				}					
				break;
			}
		case kGuildPageTomb:
			{
				//魔界探险
				if(bGuild)
				{
					if (NULL == m_pTombView)
					{
						m_pTombView = KingTomb::create();
						m_pTombView->setDelegate(this);
						m_pTombView->retain();
						//m_pGuildView->setDelegate(this);
					}
					m_pCommonLayer->addChild(m_pTombView);
				}
				else
				{
					showNoGuildError();
				}
				break;
			}
		case 4:
			{
				//去好友界面
				removeFromParent();
				Game::sharedGame()->NavigateTo(navigation::kNavFriends);
				break;
			}
		}
	}
}

void Guild::onEnter()
{
	DLayer::onEnter();
	BINDING_ONLY(m_pModel, this, Guild::updateView, D_EVENT_GUILD_GET);
	BINDING_ONLY(m_pModel, this, Guild::updateViewAfterQuiting, D_EVENT_GUILD_QUIT);
	BINDING_ONLY(m_pModel, this, Guild::showNoGuildError, D_EVENT_GUILDBOSS_ERROR_NO_GUILD);
	m_pModel->syncFromServer();
	//隐藏底部
	Game::sharedGame()->setNavigatorVisable(false);
}

void Guild::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	//m_pModel->isSynced = false;

	//恢复底部显示
	Game::sharedGame()->setNavigatorVisable(true);
}


void Guild::updateView()
{
	guild::GuildUser *selfInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	m_nGuildID = selfInfo->getGuildID();
	if (0 != m_nGuildID)
	{
		//有帮会了 借用底部导行的方法
		bool bFirst = false;
		switch(m_CreatePageType)
		{
		case kGuildPageGuild:bFirst = (NULL == m_pGuildView);break;
		case kGuildPageBoss:bFirst = (NULL == m_pBossView);break;
		case kGuildPageTomb:bFirst = (NULL == m_pTombView);break;
		}
		//处理首次进入加载和 平时更新的情况
		if(bFirst)
		{
			CCNode* pNode = CCNode::create();
			pNode->setTag((int)m_CreatePageType);
			footerCallBack(pNode);
		}
		else if(m_pGuildView && m_pGuildView->getParent())
		{
			m_pGuildView->updateViewWithDurOfPB(.0f);
		}
		

		//if (NULL == m_pGuildView)
		//{
		//	m_pGuildView = GuildView::create();
		//	m_pGuildView->retain();
		//	m_pGuildView->setDelegate(this);
		//}
		if (m_pNoneView)
		{
			m_pNoneView->removeFromParent();
		}
		//if (NULL == m_pGuildView->getParent())
		//{
		//	m_pCommonLayer->addChild(m_pGuildView);
		//}
	}
	else
	{
		//根本就没有帮会
		//强行修改创建类型！
		m_CreatePageType = kGuildPageGuild;
		if (NULL == m_pNoneView)
		{
			m_pNoneView = GuildNoneView::create();
			m_pNoneView->retain();
			m_pNoneView->setDelegate(this);
		}
		//其他的层清理干净
		m_pPopupLayer->removeAllChildren();
		m_pPopupLayerUpper->removeAllChildren();
		m_pCommonLayer->removeAllChildren();
		//加入显示
		if (NULL == m_pNoneView->getParent())
		{
			m_pCommonLayer->addChild(m_pNoneView);
		}

		//处理刚刚退帮造成的 显示逻辑错误
		if(m_pGuildView) CC_SAFE_RELEASE_NULL(m_pGuildView);
		m_CurrentPageType = kGuildPageNone;
	}
	//if (m_nGuildID != selfInfo->getGuildID())
	//{
	//	//记录帮会ID
	//	m_nGuildID = selfInfo->getGuildID();
	//	if (m_pPopupLayerUpper && m_pPopupLayerUpper->getChildrenCount())
	//	{
	//		m_pPopupLayerUpper->removeAllChildren();
	//	}
	//	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	//	{
	//		m_pPopupLayer->removeAllChildren();
	//		if (m_pGuildView && m_pGuildView->getParent())
	//		{
	//			m_pGuildView->updateViewWithDurOfPB(.0f);
	//		}
	//	}
	//}
}

void Guild::updateViewAfterQuiting()
{
	if (m_pManageView)
	{
		m_pManageView->removeFromParent();
	}
	m_pGuildView->removeFromParent();
}

void Guild::willJoinGuild()
{
	if (NULL == m_pJoinView)
	{
		m_pJoinView = GuildJoinView::create();
		m_pJoinView->retain();
		m_pJoinView->setDelegate(this);
	}
	m_pPopupLayer->addChild(m_pJoinView);
}

void Guild::willCreateGuild()
{
	if (NULL == m_pCreateView)
	{
		m_pCreateView = GuildCreateView::create();
		m_pCreateView->retain();
		m_pCreateView->setDelegate(this);
	}
	m_pPopupLayer->addChild(m_pCreateView);
}

void Guild::willClose()
{
	removeFromParent();
	Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Guild::createGuildWithName(const char * pGuildName)
{
	if (DM_GET_USER_MODEL->getUserInfo()->getVipLevel() < D_CONFIG_UINT_FOR_KEY(GUILD_CREATE_CONDITION_VIPLV))
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
		return;
	}
    
    if (DM_GET_USER_MODEL->getUserInfo()->getPoints() < D_CONFIG_UINT_FOR_KEY(GUILD_CREATE_CONDITION_POINT))
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
        return;
    }
    
	HTTP_CLIENT->guildCreate(pGuildName, this, callfuncJson_selector(Guild::createGuildWithName));
}

void Guild::createGuildWithName(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	m_pNoneView->removeFromParent();
	m_pCreateView->removeFromParent();
}

void Guild::closeGuildCreateView()
{
	m_pCreateView->removeFromParent();
}

void Guild::closeGuildSearchView()
{
	m_pJoinView->removeFromParent();
}

void Guild::willShowRankView()
{
	if (NULL == m_pRankView)
	{
		m_pRankView = GuildRankView::create();
		m_pRankView->retain();
		m_pRankView->setDelegate(this);
	}
	m_pPopupLayer->addChild(m_pRankView);
}

void Guild::willCloseRankView()
{
	m_pRankView->removeFromParent();
}

void Guild::willShowManageView()
{
	if (NULL == m_pManageView)
	{
		m_pManageView = GuildManageView::create();
		m_pManageView->retain();
		m_pManageView->setDelegate(this);
	}
	m_pPopupLayer->addChild(m_pManageView);
}

void Guild::willCloseManageView()
{
	m_pManageView->removeFromParent();
}

void Guild::willShowTreeView()
{
	GuildTreeView *pView = GuildTreeView::create();
	pView->setDelegate(this);
	m_pPopupLayer->addChild(pView);
}

void Guild::willShowMallView()
{
	GuildMallView *pView = GuildMallView::create();
	pView->setDelegate(this);
	m_pPopupLayer->addChild(pView);
}

void Guild::willShowQuestionView()
{
	GuildQuestionView *pView = GuildQuestionView::create();
	pView->setDelegate(this);
	m_pPopupLayer->addChild(pView);
}

void Guild::willShowNoticeView()
{
	GuildNoticeView *pView = GuildNoticeView::create();
	Game::sharedGame()->openPopup(pView);
	//m_pPopupLayer->addChild(pView);
}

void Guild::willCloseTreeView()
{
	m_pPopupLayer->removeAllChildren();
	m_pGuildView->updateViewWithDurOfPB(.0f);
}

void Guild::willCloseStoreView()
{
	m_pPopupLayer->removeAllChildren();
	m_pGuildView->updateViewWithDurOfPB(.0f);
}

void Guild::willCloseQuestionView(uint32_t dropID /* = 0 */)
{
	if (dropID)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(dropID));
	}
	m_pPopupLayer->removeAllChildren();
	m_pGuildView->updateViewWithDurOfPB(.0f);
}

void Guild::showItemInfoView( item::ItemInfo *pItemInfo )
{
    m_pPopupLayerUpper->removeAllChildren();
	if( m_pItemInfoView == NULL )
	{
		m_pItemInfoView = ItemInfoView::create() ;
        m_pItemInfoView->setDelegate( this ) ;
		m_pItemInfoView->retain() ;
	}
	m_pItemInfoView->setItemInfo( pItemInfo ) ;
	m_pPopupLayerUpper->addChild( m_pItemInfoView ) ;
}


void Guild::closeInfoView( InventoryType type )
{
    if( m_pItemInfoView != NULL ) 
	{
		m_pItemInfoView->removeFromParent() ;
	}
}


void Guild::showWarriorInfoView( warrior::WarriorBaseInfo *pWarrior )
{
    m_pPopupLayerUpper->removeAllChildren();
	if( m_pSoulInfoView == NULL )
	{
		m_pSoulInfoView = WarriorInfoView::create( WarriorInfoView::kTypeCloseOnly ) ;
        m_pSoulInfoView->retain() ;
	}
	m_pSoulInfoView->setWarrior( pWarrior ) ;
	m_pPopupLayerUpper->addChild( m_pSoulInfoView ) ;
}

void Guild::openGuildDetail(CCNode *popUp)
{
	//m_pPopupLayer->removeAllChildren();
	m_pPopupLayer->addChild(popUp);
}

void Guild::openUpperPopup(CCNode *popUp)
{
	m_pPopupLayerUpper->removeAllChildren();
	m_pPopupLayerUpper->addChild(popUp);
}

void Guild::onBackKey()
{
	if (m_pPopupLayerUpper && m_pPopupLayerUpper->getChildrenCount())
	{
		m_pPopupLayerUpper->removeAllChildren();
		return;
	}
	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	{
		m_pPopupLayer->removeAllChildren();
		if (m_pGuildView && m_pGuildView->getParent())
		{
			m_pGuildView->updateViewWithDurOfPB(.0f);
		}
		return;
	}
	BackKeyInterface::onBackKey();
}

void Guild::showNoGuildError()
{
	Tips_Alert(D_LOCAL_STRING("GuildErrorNoGuild").c_str());
}
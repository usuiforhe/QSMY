//
//  ServerListView.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ServerListView.h"
#include "model/PlatformModel.h"
#include "managers/GameManager.h"
#include "../Login.h"

USING_NS_CC;
USING_NS_CC_EXT;


ServerListView::ServerListView()
:m_pMyServerList(NULL)
,m_pServerList(NULL)
,m_pDelegate(NULL)
,m_pCellData(NULL)
,m_pContainer(NULL)
,m_pContainer2(NULL)
{
    
}

ServerListView::~ServerListView()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pContainer2);
    CC_SAFE_RELEASE(m_pCellData);
    CC_SAFE_RELEASE(m_pMyServerList);
    CC_SAFE_RELEASE(m_pServerList);
}

bool ServerListView::init()
{
    if(!DLayer::init()) return false;
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("ServerCell",ServerCellLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("ServerStatus",ServerStatusLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_denglu/PL_serverlist", pNodeLoaderLibrary);
    
    
    m_pCellData = readCCData("PL_ui/ccb/pl_denglu/PL_denglu_xuanfu");
    CC_SAFE_RETAIN(m_pCellData);
    
    m_pServerList = DTableView::create(this, m_pContainer->getContentSize());
    m_pServerList->setDirection(kCCScrollViewDirectionVertical);
	m_pServerList->setDelegate(this);
	m_pServerList->setVerticalFillOrder(kCCTableViewFillTopDown);
	//m_pServerList->setContentOffset(ccp(10, 0));
    m_pContainer->addChild(m_pServerList);
    CC_SAFE_RETAIN(m_pServerList);
    
    
    m_pMyServerList = DTableView::create(this, m_pContainer2->getContentSize());
    m_pMyServerList->setDirection(kCCScrollViewDirectionVertical);
	m_pMyServerList->setDelegate(this);
	m_pMyServerList->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pContainer2->addChild(m_pMyServerList);
    CC_SAFE_RETAIN(m_pMyServerList);
    
    return true;
}

void ServerListView::onEnter()
{
    DLayer::onEnter();
    m_pServerList->reloadData();
    BINDING_EXEC(DM_GET_PLATFORM_MODEL, this, ServerListView::updateView, D_EVENT_PLATFORM_ON_GET_MY_SERVER_LIST);
    DM_GET_PLATFORM_MODEL->syncMyServerList();
}

void ServerListView::onExit()
{
    UNBINDING(DM_GET_PLATFORM_MODEL, this, D_EVENT_PLATFORM_ON_GET_MY_SERVER_LIST);
    DLayer::onExit();
}

void ServerListView::updateView()
{
    m_pMyServerList->reloadData();
}

bool ServerListView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pContainer2",CCNode*,m_pContainer2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pContainer",CCNode*,m_pContainer);
    return false;
}

SEL_MenuHandler ServerListView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", ServerListView::closeCallBack);
    return NULL;
}

cocos2d::CCSize ServerListView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return CCSizeMake(400, 60);
}

void ServerListView::tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell)
{
    
}

cocos2d::extension::CCTableViewCell* ServerListView::tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx)
{
    ServerListCell* pCell =  static_cast<ServerListCell*>(table->dequeueCell());
    if(!pCell)
    {
        pCell = ServerListCell::create(m_pCellData);
        pCell->getServerCell(0)->getTouchMenu()->setTarget(this, menu_selector(ServerListView::selectCallBack));
		pCell->getServerCell(1)->getTouchMenu()->setTarget(this, menu_selector(ServerListView::selectCallBack));
    }
    
    platform::ServerInfo* pInfo1 = NULL;
	platform::ServerInfo* pInfo2 = NULL;
    CCArray* aServerList = NULL;
	uint32_t iIndex = idx * 2;
    if(table == m_pMyServerList)
    {
		aServerList = DM_GET_PLATFORM_MODEL->getMyServerList();
        pInfo1 = static_cast<platform::ServerInfo*>(aServerList->objectAtIndex(iIndex));
		if(aServerList->data->num > iIndex + 1) 
		{
			pInfo2 = static_cast<platform::ServerInfo*>(aServerList->objectAtIndex(iIndex + 1));
		}
    }else
    {
		aServerList = DM_GET_PLATFORM_MODEL->getServerList();
        pInfo1 = static_cast<platform::ServerInfo*>(aServerList->objectAtIndex(iIndex));
		if(aServerList->data->num > iIndex + 1) 
		{
			pInfo2 = static_cast<platform::ServerInfo*>(aServerList->objectAtIndex(iIndex + 1));
		}
    }
    
    pCell->getServerCell(0)->setServerInfo(pInfo1);
    pCell->getServerCell(0)->getTouchMenu()->setUserData(pInfo1);
	if(pInfo2)
	{
        pCell->getServerCell(1)->setVisible(true);
		pCell->getServerCell(1)->setServerInfo(pInfo2);
		pCell->getServerCell(1)->getTouchMenu()->setUserData(pInfo2);
	} else 
	{
		pCell->getServerCell(1)->setVisible(false);
	}
    return pCell;
}


uint32_t ServerListView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	uint32_t iCount = 0;
    if(table ==m_pMyServerList)
    {
        iCount = DM_GET_PLATFORM_MODEL->getMyServerList()->count();
    } else 
	{
		iCount = DM_GET_PLATFORM_MODEL->getServerList()->count();
	}
    return iCount / 2 + iCount % 2;
}


void ServerListView::closeCallBack(cocos2d::CCObject*)
{
    removeFromParent();
}

void ServerListView::selectCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* pItem = static_cast<CCMenuItem*>(obj);
    platform::ServerInfo* pInfo = static_cast<platform::ServerInfo*>(pItem->getUserData());
	if(pInfo->getStatus()==platform::kServerStatusMaintain)
	{
		Tips_Alert(D_LOCAL_STRING("$weihutips").c_str());
		return;
	}
    if(m_pDelegate) m_pDelegate->selectServer(pInfo);
    closeCallBack(NULL);
}


#pragma mark -

ServerListCell::ServerListCell()
{
	D_RESET_C_ARRAY(m_pServerCell, SERVER_CELL_COUNT)
}

ServerListCell::~ServerListCell()
{
    //CC_SAFE_RELEASE(m_pServerInfo);
	D_SAFE_RELEASE_C_ARRAY(m_pServerCell, SERVER_CELL_COUNT);
}

ServerListCell* ServerListCell::create(cocos2d::extension::CCData* pData)
{
    ServerListCell* pRet = new ServerListCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool ServerListCell::init(cocos2d::extension::CCData* pData)
{
    if(!CCTableViewCell::init()) return false;
    
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("ServerCell",ServerCellLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("ServerStatus",ServerStatusLoader::loader());
    /*DCCBReader * pReader = new DCCBReader(pNodeLoaderLibrary);
    cocos2d::CCNode *pNode= pReader->readNodeGraphFromData(pData, this,CCDirector::sharedDirector()->getWinSize());
    pReader->release();
    if(pNode)
    {
        m_pServerInfo = static_cast<ServerCell*>(pNode);
        CC_SAFE_RETAIN(pNode);
        addChild(pNode);
    }*/
	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);
    
    return true;
}


bool ServerListCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    D_CCB_ASSIGN_ARRAY("m_pServerCell", ServerCell*, m_pServerCell, SERVER_CELL_COUNT);
    return false;
}

ServerCell* ServerListCell::getServerCell(uint8_t i)
{
	return m_pServerCell[i];
}
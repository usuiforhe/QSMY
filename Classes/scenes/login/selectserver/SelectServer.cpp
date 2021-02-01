//
//  SelectServer.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SelectServer.h"
#include "managers/PlatformManager.h"
#include "ServerListView.h"

USING_NS_CC;
USING_NS_CC_EXT;

SelectServer::SelectServer()
:m_pDelegate(NULL)
,m_pServerListView(NULL)
,m_pUserCenter(NULL)
,m_pUserCenterText(NULL)
,m_pUserName(NULL)
,m_pUsernameMenuItem(NULL)
,m_pContainer(NULL)
,m_pMyServerContainer(NULL)
,m_pServerId(NULL)
,m_pStatus(NULL)
,m_pServerName(NULL)
,m_pUserNameNode(NULL)
,m_pUserCenterNode(NULL)
{
    
}

SelectServer::~SelectServer()
{
    CC_SAFE_RELEASE(m_pUserName);
    CC_SAFE_RELEASE(m_pUsernameMenuItem);
    CC_SAFE_RELEASE(m_pContainer);
    
    CC_SAFE_RELEASE(m_pServerListView);
    CC_SAFE_RELEASE(m_pUserCenter);
    CC_SAFE_RELEASE(m_pUserCenterText);
    CC_SAFE_RELEASE(m_pMyServerContainer);
    UNBINDING(m_pPlatformModel, this, D_EVENT_PLATFORM_SERVER_CHANGED);
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CC_SAFE_RELEASE(m_pPlatformModel);
	CC_SAFE_RELEASE(m_pServerId);
	CC_SAFE_RELEASE(m_pStatus);
	CC_SAFE_RELEASE(m_pServerName);
	CC_SAFE_RELEASE(m_pUserNameNode);
	CC_SAFE_RELEASE(m_pUserCenterNode);
}

bool SelectServer::init()
{
    if(!DLayer::init()) return false;
    m_pPlatformModel = DM_GET_PLATFORM_MODEL;
    CC_SAFE_RETAIN(m_pPlatformModel);
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    //pNodeLoaderLibrary->registerCCNodeLoader("ServerCell",ServerCellLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("ServerStatus",ServerStatusLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_denglu/PL_selectServer", pNodeLoaderLibrary);
    
    m_pUserCenter->setTarget(this, menu_selector(SelectServer::showUserCenter));
    if (!PlatformManager::sharedPlatformManager()->isInternal()) {
       m_pUserCenterNode->setVisible(true);
    }
    else
    {
       m_pUserCenterNode->setVisible(false);
    }
    
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(SelectServer::updateMyServer), D_EVENT_LOGIN_SUC);
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(SelectServer::updateMyServer), D_EVENT_LOGOUT_SUC);
    if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
	{
		m_pUserCenterNode->setVisible(false);
		m_pUserNameNode->setVisible(false);
	}
	
  
    return true;
}

void SelectServer::onEnter()
{
    DLayer::onEnter();
    BINDING_EXEC(m_pPlatformModel, this, SelectServer::updateView, D_EVENT_PLATFORM_ON_GET_SERVER_LIST);
    m_pPlatformModel->syncServerList(); //login
	std::string pUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
    
    if (PlatformManager::sharedPlatformManager()->isInternal()) {
        if(pUserName.empty())
        {
            m_pUserName->setString(D_LOCAL_STRING("$logintips8").c_str());
        } else
        {
            m_pUserName->setString(pUserName.c_str());
        }
    }
    else
    {
        m_pUserName->setVisible(false);
        m_pUsernameMenuItem->setVisible(false);
    }
    m_pDelegate->getAnnounce();
}

void SelectServer::onExit()
{
    DLayer::onExit();
    UNBINDING(m_pPlatformModel, this, D_EVENT_PLATFORM_ON_GET_SERVER_LIST);
}

void SelectServer::updateView()
{
    if(!m_pPlatformModel->isSynced) return;
  //  if(PlatformManager::sharedPlatformManager()->isInternal())
  //  {
  //      createInternalLogin();
		//HTTP_CLIENT->setURL(m_pPlatformModel->getServerURL());
		//HTTP_CLIENT->checkMaint();
  //  }else
  //  {
  //      //showMyServer();
		//
  //  }
	

	BINDING_EXEC(m_pPlatformModel, this, SelectServer::showMyServer, D_EVENT_PLATFORM_SERVER_CHANGED);
}

void SelectServer::showUserCenter(cocos2d::CCObject*obj)
{
    PlatformManager::sharedPlatformManager()->showUserCenter();
}

void SelectServer::showMyServer()
{
    //m_pUserName->setVisible(false);
    //m_pMyServer->setVisible(true);
    //BINDING_EXEC(m_pPlatformModel, this, SelectServer::updateMyServer, D_EVENT_PLATFORM_SERVER_CHANGED);
    //m_pMyServer->getTouchMenu()->setTarget(this, menu_selector(SelectServer::changeServerCallBack));
	platform::ServerInfo* pCurrentServer = m_pPlatformModel->getCurrentServer();
	if(pCurrentServer==NULL)
	{
		//error
		return;
	}

	//m_pMyServer->setServerInfo();
	m_pServerId->setString(D_LOCAL_STRING("Server%d",pCurrentServer->getSID()).c_str());
	m_pServerName->setString(pCurrentServer->getName().c_str());
	m_pStatus->setStatus(pCurrentServer->getStatus());

	HTTP_CLIENT->setURL(m_pPlatformModel->getServerURL());
	HTTP_CLIENT->checkMaint();
}

void SelectServer::updateMyServer()
{
    platform::ServerInfo* pCurrentServer = m_pPlatformModel->getCurrentServer();
    if(pCurrentServer==NULL)
    {
        //error
        return;
    }
    
    //m_pMyServer->setServerInfo();
	m_pServerId->setString(D_LOCAL_STRING("Server%d",pCurrentServer->getSID()).c_str());
	m_pServerName->setString(pCurrentServer->getName().c_str());
	m_pStatus->setStatus(pCurrentServer->getStatus());
    //m_pMyServer->setVisible(true);
    if(PlatformManager::sharedPlatformManager()->isLogined())
    {
        //m_pMyServerContainer->setVisible(true);
        if(!PlatformManager::sharedPlatformManager()->isGuess() && !PlatformManager::sharedPlatformManager()->isInternal())
        {
            m_pUserCenter->setVisible(true);
            m_pUserCenterText->setVisible(true);
        }
        HTTP_CLIENT->setURL(m_pPlatformModel->getServerURL());
        HTTP_CLIENT->checkMaint();
    }else
    {
        PlatformManager::sharedPlatformManager()->login();
        m_pUserCenter->setVisible(false);
        m_pUserCenterText->setVisible(false);
        m_pMyServerContainer->setVisible(false);
    }
    
    //如果是游客登入则用户中心不显示
   
}

bool SelectServer::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyServer", ServerCell*, this->m_pMyServer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserCenter", CCMenuItem*, this->m_pUserCenter);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserCenterText", CCSprite*, this->m_pUserCenterText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserName", CCLabelTTF*, this->m_pUserName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUsernameMenuItem", CCMenuItemImage*, this->m_pUsernameMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMyServerContainer", CCNode*, this->m_pMyServerContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pServerId",  CCLabelTTF*, this->m_pServerId);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStatus",  ServerStatus*, this->m_pStatus);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pServerName",  CCLabelTTF*, this->m_pServerName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserNameNode", CCNode*, this->m_pUserNameNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserCenterNode", CCNode*, this->m_pUserCenterNode);
   
    return false;
}

SEL_MenuHandler SelectServer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "loginServerCallBack", SelectServer::loginServerCallBack);
    return NULL;
}


void SelectServer::changeServerCallBack(cocos2d::CCObject* obj)
{
    if(!PlatformManager::sharedPlatformManager()->isLogined())
    {
        PlatformManager::sharedPlatformManager()->login();
        return;
    }
    
    if(m_pServerListView==NULL)
    {
        m_pServerListView = ServerListView::create();
        m_pServerListView->setDelegate(m_pDelegate);
        CC_SAFE_RETAIN(m_pServerListView);
    }
    addChild(m_pServerListView);
}



void SelectServer::enterGameCallBack(cocos2d::CCObject* obj)
{
    if(PlatformManager::sharedPlatformManager()->isLogined())
    {
        m_pDelegate->loginGame();
    }else
    {
        PlatformManager::sharedPlatformManager()->login();
    }
}

void SelectServer::loginServerCallBack(cocos2d::CCObject* obj)
{
	CCMenu* pTar = static_cast<CCMenu*>(obj);
	switch(pTar->getTag())
	{
	case kUserRegisterTag:
		{
			if(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD, "").empty())
			{
				m_pDelegate->openPopup(kLoginView);
			} else 
			{
			    m_pDelegate->openPopup(kLoginPWView);
			}
		}
		break;
	case kSelectServerListTag:
		changeServerCallBack(obj);
		break;
	case kEnterGameTag:
		enterGameCallBack(obj);break;
	}
}



void SelectServer::createInternalLogin()
{
    //m_pMyServer->setVisible(false);
    //m_pUserName->setVisible(true);
    //登陆输入框
    
	//m_pUserName->setString(m_pLoginUsernameEditBox->getText());
	//m_pContainer->setVisible(false);
	//m_pUserName->setVisible(true);
}




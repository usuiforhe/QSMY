//
//  SelectServer.h
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SelectServer__
#define __QSMY__SelectServer__

#include "AppInclude.h"
#include "components/components.h"
#include "ServerCell.h"
#include "ServerStatus.h"
#include "managers/GameManager.h"
#include "../LoginInclude.h"

class ServerListView;

class SelectServer
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pServerId, ServerId);
	CC_SYNTHESIZE_READONLY(ServerStatus*, m_pStatus, Status);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pServerName, ServerName);
public:
    SelectServer();
    virtual ~SelectServer();
    CREATE_FUNC(SelectServer);
        
private:
    
    void loginGame();
    
    void enterGameCallBack(cocos2d::CCObject* obj);
	void loginServerCallBack(cocos2d::CCObject* obj);
    void changeServerCallBack(cocos2d::CCObject* obj);
    void showUserCenter(cocos2d::CCObject*obj);
    
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    void createInternalLogin();
    
    void updateView();
    
    virtual void onEnter();
    virtual void onExit();
    
    void showMyServer();
    void updateMyServer();
    
    
private:
    cocos2d::CCMenuItem *m_pUserCenter;
    cocos2d::CCSprite   *m_pUserCenterText;
    //ServerCell          *m_pMyServer;
    ServerListView      *m_pServerListView;
    PlatformModel       *m_pPlatformModel;
    cocos2d::CCLabelTTF     *m_pUserName;
    cocos2d::CCMenuItemImage *m_pUsernameMenuItem;
    cocos2d::CCNode     *m_pContainer;
    cocos2d::CCNode     *m_pMyServerContainer;
	cocos2d::CCNode     *m_pUserNameNode;
	cocos2d::CCNode     *m_pUserCenterNode;
    

};

#endif /* defined(__QSMY__SelectServer__) */

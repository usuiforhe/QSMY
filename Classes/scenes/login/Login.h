//
//  Login.h
//  登陆
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Login__
#define __QSMY__Login__

#include "AppInclude.h"
#include "json/json.h"
#include "../BaseScenes.h"
#include "./selectserver/SelectServer.h"
#include "./update/UpdateGame.h"
#include "model/PlatformModel.h"
#include "components/components.h"
#include "LoginInclude.h"

#define LOGIN_ANI_SECOND_NUM 6

class BusyLayer;

#pragma mark - Login
class Login
:public BaseScenes
,public cocos2d::extension::CCBMemberVariableAssigner
,public LoginDelegate

{
#pragma mark - - init etc.
public:
    Login();
    ~Login(void);
    
    CREATE_FUNC(Login);
    void showFacebookBusyLayer(CCObject*);
    void hideFacebookBusyLayer(CCObject*);
    void openPopup(PopupView);
	void loginGame(const char* userName, const char* password);
	void registerUser(const char* userName = NULL, const char* password = NULL, const char* confirmPassword = NULL, const char* email = NULL);
	void bindAccount(const char* userName = NULL, const char* password = NULL, const char* confirmPassword = NULL, const char* email = NULL);
	void resetPass(const char* userName = NULL, const char* oldPass = NULL, const char* newPass = NULL, const char* confirmPass = NULL);
	void animateLogin();
	void animateLoginStart();
	void showSelectServer();
	void resetPassCallback(const Json::Value & ,const Json::Value &);
	void getServerList();
	void showSelectLoginType();
    
    void registered(const Json::Value &request,const Json::Value &respones);
	void binded(const Json::Value &request,const Json::Value &respones);
private:
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	void getAnnounce();
	void getAnnouceCallBack(const Json::Value &requestData, const Json::Value &responseData);

    void showInfo(LoginPage, PageFrom from = kFromNone);
    void selectServer(platform::ServerInfo*);
    void registerLoginSucHandler();
    void addNotifyHandler();
    void removeNotifyHandler();
    void systemMaintain(CCObject*);
    void requestFailed(CCObject*);
    void requestFailedCallBack(MessageType,DMessageBox*);
    void LoginPlatform();
    void getServerListCallBack();
    void LoginPlatformCallBack(const Json::Value&,const Json::Value&);
    void animationComplete(sp::Armature * target, const char* event,const char* data);
	void loginFailed(CCObject*);
	void logoAniComplete(sp::Armature * target, const char* event,const char* data);
	void delayPlayLoginAni();
private:
    
    enum
    {
        kZorder                 = 0,
        kMainLayerZorder        = 10,
		
//        kNoticeLayerZoder       = 30,
        kPopupZorder            = 40,
//        kActivityPopupZorder    = 44,
        kMessageBoxZorder       = 45,
//        kBattleLayerZorder      = 50,
//        kEffectLayerZorder      = 60,
//        kTutorialLayerZorder    = 65,        //新手引导层
        kTipsLayerZorder        = 70,        //提示层
        kBusyLayerZorder        = 80,
//        kTouchLayerZorder       = 90,
//        kBoderZoder             = 100,       //最外层遮罩
    };
    
    DLayer                      *m_pMainLayer;
    
private:
    DLayer                      *m_pMessageBoxLayer;        //确认信息提示层
    cocos2d::CCNode             *m_pContainer;
    cocos2d::CCLabelBMFont      *m_pVersion;
	DPopupLayer                 *m_pPopupLayer;  //弹出层
	cocos2d::CCSprite			*m_pBg;
	bool						m_isShowedAnnounce;
    BusyLayer                   *m_pFacebookBusyLayer;
	sp::Armature				*m_pArmature[LOGIN_ANI_SECOND_NUM];
	cocos2d::CCNode             *m_pAniNode;
};

#endif /* defined(__QSMY__Login__) */

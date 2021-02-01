//
//  Login.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Login.h"
#include "managers/GameManager.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "managers/TipsManager.h"
#include "managers/PlatformManager.h"
#include "./systemmaintain/SystemMaintain.h"
#include "utils/SystemUtil.h"
#include "managers/SoundManager.h"
#include "./selectserver/LoginView.h"
#include "utils/URLUtil.h"
#include "./selectserver/SelectLoginType.h"
#include "./selectserver/SelectUser.h"
#include "../game/announce/Announce.h"
#include "json/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;

static const int LOGIN_ANIMATION_Z_ORDER = 110;

#pragma mark - Login
Login::Login()
:m_pContainer(NULL)
,m_pVersion(NULL)
,m_pMainLayer(NULL)
,m_pMessageBoxLayer(NULL)
,m_pPopupLayer(NULL)
,m_pBg(NULL)
,m_isShowedAnnounce(false)
,m_pFacebookBusyLayer(NULL)
,m_pAniNode(NULL)
{
    D_RESET_C_ARRAY(m_pArmature, LOGIN_ANI_SECOND_NUM);
}

Login::~Login()
{
    removeNotifyHandler();
	CC_SAFE_RELEASE(m_pMessageBoxLayer);
	CC_SAFE_RELEASE(m_pPopupLayer);
    CC_SAFE_RELEASE(m_pFacebookBusyLayer);
	D_SAFE_RELEASE_C_ARRAY(m_pArmature, LOGIN_ANI_SECOND_NUM);
	CC_SAFE_RELEASE(m_pAniNode);
	//m_pArmature->getAnimation()->removeObserver(this, COMPLETE);
}
bool Login::init()
{
    if ( !BaseScenes::init() )
    {
        return false;
    }
    
    DM_GET_USER_MODEL->setLogin(false);
    
    m_pMainLayer = DLayer::create();
    m_pMainLayer->setPosition(D_APP_CENTER_POINT);
    addChild(m_pMainLayer,kMainLayerZorder);
    
    DLayer* tips =DLayer::create();
    tips ->setContentSize(DESIGN_SIZE);
    tips->setPosition(D_APP_CENTER_POINT);
    addChild(tips,kTipsLayerZorder);
    TIPSMANAGER->setParent(tips);
    
    CCNode* pBusyLayer =  CCNode::create();
    pBusyLayer->setPosition(D_APP_CENTER_POINT);
    HTTP_CLIENT->getBusyLayer()->removeFromParent();
    pBusyLayer->addChild(HTTP_CLIENT->getBusyLayer());
    addChild(pBusyLayer,kBusyLayerZorder);
    
    m_pFacebookBusyLayer = BusyLayer::create();
    this->addChild(m_pFacebookBusyLayer, kBusyLayerZorder);
    CC_SAFE_RETAIN(m_pFacebookBusyLayer);
    
    
    m_pMessageBoxLayer = DLayer::create();
    m_pMessageBoxLayer->retain();
    m_pMessageBoxLayer->setPosition(D_APP_CENTER_POINT);
    addChild(m_pMessageBoxLayer,kMessageBoxZorder);
    DMessageBox::setContainer(m_pMessageBoxLayer);

	m_pPopupLayer = DPopupLayer::create();
	m_pPopupLayer->retain();
	m_pPopupLayer->setPosition(D_APP_CENTER_POINT);
	addChild(m_pPopupLayer, kPopupZorder);

    cocos2d::CCNode *pNode = NULL;
    CCB_READER("PL_ui/ccb/pl_denglu/PL_denglu_moren", pNode);
    if(pNode) m_pMainLayer->addChild(pNode);

    m_pVersion->setString(SystemUtil::getVersion().c_str());
    
    addNotifyHandler();
    
	SoundManager::sharedSoundManager()->playLoginSound();

	showInfo(kLoginPageUpdate);
    
    CCNode *pBorderNode = NULL;
    CCB_READER("PL_ui/ccb/pl_kuozhankuang/PL_kuozhankuang",pBorderNode);
    pBorderNode->setPosition(D_APP_CENTER_POINT);
    if(pBorderNode) addChild(pBorderNode, LOGIN_ANIMATION_Z_ORDER + 1);
    return true;
}

void Login::getServerList()
{
	PlatformModel* pPlatformModel = DM_GET_PLATFORM_MODEL;
	BINDING_ONLY(pPlatformModel, this, Login::getServerListCallBack, D_EVENT_PLATFORM_ON_GET_SERVER_LIST);
	pPlatformModel->syncServerList();
}

void Login::getServerListCallBack()
{
    PlatformModel* pPlatformModel = DM_GET_PLATFORM_MODEL;
    if(!pPlatformModel->isSynced) return;
    UNBINDING(pPlatformModel, this, D_EVENT_PLATFORM_ON_GET_SERVER_LIST);
	NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(Login::registerLoginSucHandler), D_EVENT_LOGOUT_SUC);
	registerLoginSucHandler();
    if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0
       && !PlatformManager::sharedPlatformManager()->isInternal())
    {
		LoginPlatform();
	}
}

void Login::registerLoginSucHandler()
{
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(Login::LoginPlatform), D_EVENT_LOGIN_SUC);
}

void Login::removeNotifyHandler()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


void Login::addNotifyHandler()
{
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Login::systemMaintain),D_EVENT_SYSTEM_MAINTAIN);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Login::requestFailed),D_EVENT_REQUEST_FAILD);
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Login::loginFailed),D_EVENT_LOGIN_FAILD);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Login::showFacebookBusyLayer),D_EVENT_FETCHING_FACEBOOK_EMAIL);
    NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Login::hideFacebookBusyLayer),D_EVENT_FETCH_FACEBOOK_EMAIL_DONE);
}

void Login::LoginPlatform()
{
    if(PlatformManager::sharedPlatformManager()->isGuess()) return;
    if(PlatformManager::sharedPlatformManager()->isInternal() && strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) != 0) return;
    if(PlatformManager::sharedPlatformManager()->isLogined())
    {
        NotificationCenter::sharedNotificationCenter()->removeObserver(this, D_EVENT_LOGIN_SUC);
		if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
        {   
			showInfo(kLoginPageSelectServer);
		} else 
		{
			HTTP_CLIENT->loginSetTokken(PlatformManager::sharedPlatformManager()->getLoginCBParam().c_str(),this,callfuncJson_selector(Login::LoginPlatformCallBack));
		}
		
    }
}

void Login::LoginPlatformCallBack(const Json::Value& request,const Json::Value &responseData)
{
    
    if(responseData["code"].asString().compare(SUC_CODE)==0)
    {
        PlatformManager::sharedPlatformManager()->setLoginCallBack(responseData["data"]);
		GameManager::sharedGameManager()->logined(request, responseData);
    }
    else
    {
        registerLoginSucHandler();
        if(responseData["code"].asString().compare(INVAILD_TOKKEN)==0)
        {
            PlatformManager::sharedPlatformManager()->login();
        }
    }
    
}

void Login::requestFailedCallBack(MessageType type,DMessageBox*pBox)
{
    switch (type)
    {
        case kMessageTypeOk:
            HTTP_CLIENT->retryFailedRequest();
            break;
        case kMessageTypeCancel:
            //userLogout();
            HTTP_CLIENT->retryFailedRequest();
            break;
        default:
            break;
    }
    
}
void Login::requestFailed(CCObject*)
{
    //Tips_Error(D_LOCAL_STRING("checkNetwork").c_str());
    //HTTP_CLIENT->reset();
    DMessageBox::showMessageBox(D_LOCAL_STRING("checkNetworkRetry").c_str(), this, messageBox_callfunc_selector(Login::requestFailedCallBack));
}

void Login::systemMaintain(CCObject*)
{
    //if(SystemMaintain::sharedSystemMaintain()->getParent()) return;
    //m_pContainer->addChild(SystemMaintain::sharedSystemMaintain());
	 DM_GET_ERRORTIP_MODEL->showErrorTips(SYSTEM_MAINTAIN);
}

void Login::showInfo(LoginPage page, PageFrom from /*= kFromNone*/)
{
	if(from == kFromUpdate)
	{
		getServerList();
	}
    m_pContainer->removeAllChildren();
	CCLOG("login showinfo: %d", page);
    switch (page)
    {
        case kLoginPageUpdate:
        {
            UpdateGame *pNode = UpdateGame::create();
            pNode->setDelegate(this);
            m_pContainer->addChild(pNode);
            break;
        }
        case KLoginTypeSelect:
        {
			CCActionInterval *delay = CCDelayTime::create(0.1f);
			
			switch(from)
			{
			case kFromUpdate:
				this->runAction(CCSequence::create(delay,
					CCCallFunc::create(this, callfunc_selector(Login::animateLoginStart)),
					CCDelayTime::create(2.5f),
					CCCallFunc::create(this, callfunc_selector(Login::animateLogin)),
					CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(Login::showSelectLoginType)), NULL));break;
			default:
				this->runAction(CCSequence::create(
					CCCallFunc::create(this, callfunc_selector(Login::animateLogin)),
					CCCallFunc::create(this, callfunc_selector(Login::showSelectLoginType)), NULL));break;
			}
			
            break;
        }
		case KUserSelect:
		{

			break;
		}
		case kLoginPageSelectServer:
		{
			if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
			{
				this->runAction(CCSequence::create(
					CCCallFunc::create(this, callfunc_selector(Login::animateLogin)), 
					CCCallFunc::create(this, callfunc_selector(Login::showSelectServer)), NULL));
			} else 
			{
				CCActionInterval *delay = CCDelayTime::create(0.1f);

				switch(from)
				{
				case kFromUpdate:
					this->runAction(CCSequence::create(delay,
						CCCallFunc::create(this, callfunc_selector(Login::animateLoginStart)),
						CCDelayTime::create(2.5f),
						CCCallFunc::create(this, callfunc_selector(Login::animateLogin)),
						CCDelayTime::create(0.5f),CCCallFunc::create(this, callfunc_selector(Login::showSelectServer)), NULL));break;
				default:
					this->runAction(CCSequence::create(
						CCCallFunc::create(this, callfunc_selector(Login::animateLogin)), delay,
						CCCallFunc::create(this, callfunc_selector(Login::showSelectServer)), NULL));break;
				}
			}
			
			break;
		}
        default:
            break;
    }
}

void Login::showSelectLoginType()
{
	m_pBg->setVisible(true);
	
	std::string platId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
	//pDeUserName = "";
	LoginType iLoginType = (LoginType)CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, 1);
    
	//如果没有账户
	if(!DM_GET_PLATFORM_MODEL->getLoginedUserByName(platId.c_str(), iLoginType))
	{
		SelectLoginType* pNode = SelectLoginType::create();
		pNode->setDelegate(this);
		m_pContainer->addChild(pNode, 20);
	} else //如果有账户
	{
		SelectUser* pNode = SelectUser::create(iLoginType);
		pNode->setDelegate(this);
		m_pContainer->addChild(pNode, 20);
	}
}

void Login::showSelectServer()
{
	//getServerList();
	m_pBg->setVisible(true);
	SelectServer* pNode = SelectServer::create();
	pNode->setDelegate(this);
	m_pContainer->addChild(pNode, 20);
	//getAnnounce();
}

void Login::animateLogin()
{
	/*CCNode* pAniNode = CCNode::create();
	pAniNode->setPosition(D_DESIGN_POINT_CENTER);
	m_pContainer->addChild(pAniNode, 0);

	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("loginsecond");
	pAniNode->addChild(m_pArmature);*/
	CC_SAFE_RELEASE(m_pAniNode);
	m_pAniNode = CCNode::create();
	CC_SAFE_RETAIN(m_pAniNode);
	m_pAniNode->setPosition(D_DESIGN_POINT_CENTER);
	m_pContainer->addChild(m_pAniNode, 0);

	m_pBg->setVisible(true);
	float delayTime = 0.5f;
	for(int i=0; i<LOGIN_ANI_SECOND_NUM; i++)
	{
		if(!m_pArmature[i])
		{
			m_pArmature[i] = ResourceManager::sharedResourceManager()->getUIAnimateOnly(CCString::createWithFormat("loginsecond%d", i+1)->getCString());
			CC_SAFE_RETAIN(m_pArmature[i]);
			//delayTime += 0.1f;
		}
		m_pAniNode->addChild(m_pArmature[i], 100 - i*10);
	}
	
	this->runAction(CCSequence::create(CCDelayTime::create(delayTime), CCCallFunc::create(this, callfunc_selector(Login::delayPlayLoginAni)), NULL));
	//CC_SAFE_RETAIN(m_pArmature);
	/*m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Login::animationComplete), COMPLETE);
	m_pArmature->getAnimation()->playByIndex(0);*/
}

void Login::delayPlayLoginAni()
{
	for(int i=LOGIN_ANI_SECOND_NUM-1; i>0; i--)
	{
		m_pArmature[i]->getAnimation()->playByIndex(0, -1, 1, true);
	}
	//一次播放
	sp::Armature* pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly("loginlogostart");
	m_pAniNode->addChild(pArmature, LOGIN_ANIMATION_Z_ORDER);
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Login::logoAniComplete), COMPLETE);
	pArmature->getAnimation()->playByIndex(0);
	m_pArmature[0]->setVisible(false);
}

void Login::animationComplete(sp::Armature * target, const char* event,const char* data)
{
	if(target){
		target->getAnimation()->removeObserver(this, COMPLETE);
		target->removeFromParent();
	}
}

void Login::logoAniComplete(sp::Armature * target, const char* event,const char* data)
{
	m_pArmature[0]->setVisible(true);
	m_pArmature[0]->getAnimation()->playByIndex(0, -1, 1, true);
	if(target){
		target->getAnimation()->removeObserver(this, COMPLETE);
		target->removeFromParent();
	}
}

void Login::animateLoginStart()
{
	m_pBg->setVisible(false);
	CCNode* pAniNode = CCNode::create();
	pAniNode->setPosition(D_DESIGN_POINT_CENTER);
	m_pContainer->addChild(pAniNode, 10);
	//一次播放
	sp::Armature* pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly("loginstart");
	pAniNode->addChild(pArmature, 10);
	pArmature->getAnimation()->playByIndex(0);
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Login::animationComplete), COMPLETE);

	//一次播放之后，循环播放第二个标签
	/*m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly("loginsecond");
	pAniNode->addChild(m_pArmature);
	m_pArmature->getAnimation()->playByIndex(0);*/
	////CC_SAFE_RETAIN(m_pArmature);
	//m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Login::animationComplete), COMPLETE);
	//m_pArmature->getAnimation()->playByIndex(0);
}

void Login::selectServer(platform::ServerInfo* pInfo)
{
    DM_GET_PLATFORM_MODEL->setCurrentServer(pInfo);
}

bool Login::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pVersion", CCLabelBMFont *, this->m_pVersion);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pBg", CCSprite *, this->m_pBg);
    return false;
}

void Login::showFacebookBusyLayer(CCObject*)
{
    if (m_pFacebookBusyLayer) {
        m_pFacebookBusyLayer->showBusy();
    }
}

void Login::hideFacebookBusyLayer(CCObject*)
{
    if (m_pFacebookBusyLayer && m_pFacebookBusyLayer->isShowing()) {
        m_pFacebookBusyLayer->hideBusy();
    }
}

void Login::openPopup(PopupView pView)
{
	switch(pView)
	{
	case kLoginView:
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kLogin);
			CCUserDefault::sharedUserDefault()->flush();
			LoginView *pPopView = LoginView::create();
			pPopView->setDelegate(this);
			m_pPopupLayer->openPopup(pPopView);
			break;
		}
	case kLoginPWView:
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kLogin);
			CCUserDefault::sharedUserDefault()->flush();
			LoginViewPW *pPopView = LoginViewPW::create();
			pPopView->setDelegate(this);
			m_pPopupLayer->openPopup(pPopView);
			break;
		}
	case kChangePWView:
		{
			ChangePWView *pPopView = ChangePWView::create();
			pPopView->setDelegate(this);
			m_pPopupLayer->openPopup(pPopView);
			break;
		}
	case KRegisterView:
		{
			RegisterView *pPopView = RegisterView::create();
			pPopView->setDelegate(this);
			m_pPopupLayer->openPopup(pPopView);
			break;
		}
	case KBindingView:
		{
			BindingView *pPopView = BindingView::create();
			pPopView->setDelegate(this);
			m_pPopupLayer->openPopup(pPopView);
			break;
		}
	case kAnnounceView:
		{
			m_pPopupLayer->openPopup(Announce::create());
			break;
		}
	case kRemoveUserTipsView:
		{
			RemoveUserTipsView *pPopView = RemoveUserTipsView::create();
			pPopView->setDelegate(this);
			m_pPopupLayer->openPopup(pPopView);
			break;
		}
	}
}

void Login::loginGame(const char* userName, const char* password)
{
	int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	if(iLoginType == kFacebookLogin || iLoginType == kGoogelPlusLogin){
		HTTP_CLIENT->loginSetTokken(PlatformManager::sharedPlatformManager()->getLoginCBParam().c_str(),this,callfuncJson_selector(Login::LoginPlatformCallBack));
	} else {
		if(!userName || !password || strcmp(userName,"") == 0 || strcmp(password,"") == 0)
		{
			if(!PlatformManager::sharedPlatformManager()->isLogined()) {
				std::string pDeUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
				std::string pDePassword = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD,"");
				if(pDeUserName.empty() || pDePassword.empty())
				{
					Tips_Alert(D_LOCAL_STRING("$logintips9").c_str());
					showInfo(KLoginTypeSelect);
					return;
				}
			}
			//pUserName = pDeUserName.c_str();
			//pPassword = pDePassword.c_str();
		} else
		{
			CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, userName);
			CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, password);
			CCUserDefault::sharedUserDefault()->flush();
		}
		GameManager::sharedGameManager()->login();
	}
    DM_GET_PLATFORM_MODEL->setCurrentServer(DM_GET_PLATFORM_MODEL->getCurrentServer());
    return;
}

void Login::loginFailed(CCObject*)
{
	this->showInfo(KLoginTypeSelect);
	int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	if(iLoginType == kLogin) 
	{
		openPopup(kLoginView);
	}
}

void Login::registerUser(const char* userName, const char* password, const char* confirmPassword, const char* email)
{
	CCLOG("registerUser: %s, %s", userName, password);
	if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
	{
		json_serializer a;
		a<<"un"<<userName<<"p"<<password<<"rp"<<confirmPassword<<"e"<<email<<"c"<<SystemUtil::getUDID().c_str()<<"et"<<UrlEncode(SystemUtil::getDeviceInfo()).c_str();
		HTTP_CLIENT->registerUserTW(this,callfuncJson_selector(Login::registered), a);
	} else 
	{
		const char* pParam = CCString::createWithFormat("u=%s&p=%s&rp=%s&e=%s&c=%s&et=%s&un=%s",userName, password, confirmPassword, email,
			SystemUtil::getUDID().c_str(), UrlEncode(SystemUtil::getDeviceInfo()).c_str(), CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME).c_str())->getCString();
		GameManager::sharedGameManager()->registerUser(pParam);
	}
	
	DM_GET_PLATFORM_MODEL->setCurrentServer(DM_GET_PLATFORM_MODEL->getCurrentServer());
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, userName);
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, password);
	CCUserDefault::sharedUserDefault()->flush();
}

void Login::bindAccount(const char* userName, const char* password, const char* confirmPassword, const char* email)
{
	CCLOG("bindAccount: %s, %s", userName, password);
	int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	if(iLoginType != kQuickLogin) return;
	
	json_serializer a;
	std::string pDeUserPlatId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
	a<<"guessId"<<pDeUserPlatId.c_str()<<"platName"<<DM_GET_PLATFORM_MODEL->getConfig().getPlatform().c_str();
	a<<"un"<<userName<<"p"<<password<<"rp"<<confirmPassword<<"c"<<SystemUtil::getUDID().c_str()<<"et"<<UrlEncode(SystemUtil::getDeviceInfo()).c_str();
	HTTP_CLIENT->bindAccount(this,callfuncJson_selector(Login::binded), a);

	DM_GET_PLATFORM_MODEL->setCurrentServer(DM_GET_PLATFORM_MODEL->getCurrentServer());
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_GUESS_USER_NAME, pDeUserPlatId);
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, userName);
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, password);
	CCUserDefault::sharedUserDefault()->flush();
}

void Login::binded(const Json::Value &request,const Json::Value &respones)
{
	std::string pDeUserPlatId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_GUESS_USER_NAME,"");
	if(respones["code"].asString().compare(SUC_CODE)!=0)
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, pDeUserPlatId);
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, "");
		CCUserDefault::sharedUserDefault()->flush();
		return;
	}

	DM_GET_PLATFORM_MODEL->removeLoginedUser(pDeUserPlatId.c_str(), kQuickLogin, false);

	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	/*std::string pDeUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
	std::string pDePassword = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD,"");
	DM_GET_PLATFORM_MODEL->saveLoginedUser(pDeUserName.c_str(), pDePassword.c_str(), kLogin, NULL);
	CCLOG("login binded: %s", pDeUserName.c_str());*/

	
	this->showInfo(kLoginPageSelectServer);
}

void Login::registered(const Json::Value &request,const Json::Value &respones)
{
    if(respones["code"].asString().compare(SUC_CODE)!=0) return;
    CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE, kLogin);
    /*std::string pDeUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME,"");
	std::string pDePassword = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD,"");
	DM_GET_PLATFORM_MODEL->saveLoginedUser(pDeUserName.c_str(), pDePassword.c_str(), kLogin, NULL);*/
    
	this->showInfo(kLoginPageSelectServer);
}

void Login::resetPass(const char* userName, const char* oldPass, const char* newPass, const char* confirmPass)
{
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, newPass);
	CCUserDefault::sharedUserDefault()->flush();
	if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
	{
		json_serializer a;
		a<<"un"<<userName<<"op"<<oldPass<<"p"<<newPass<<"rp"<<confirmPass<<"c"<<SystemUtil::getUDID().c_str()<<"et"<<UrlEncode(SystemUtil::getDeviceInfo()).c_str();
		HTTP_CLIENT->resetPassTW(this, callfuncJson_selector(Login::resetPassCallback), a);
	} else 
	{
		const char* pParam = CCString::createWithFormat("u=%s&op=%s&p=%s&rp=%s&c=%s&et=%s",userName, oldPass, newPass, confirmPass,
			SystemUtil::getUDID().c_str(), UrlEncode(SystemUtil::getDeviceInfo()).c_str())->getCString();
		HTTP_CLIENT->resetPass(pParam, this, callfuncJson_selector(Login::resetPassCallback));
	}
}

void Login::resetPassCallback(const Json::Value & request,const Json::Value & response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	Tips_Alert(D_LOCAL_STRING("$logintips14").c_str());
}

void Login::getAnnounce()
{
	if(m_isShowedAnnounce)
	{
		return;
	}

	HTTP_CLIENT->getAnnounce(this, callfuncJson_selector(Login::getAnnouceCallBack));
}

void Login::getAnnouceCallBack(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	DM_GET_ANNOUNCE_MODEL->parseAnnounces(responseData["data"]["announces"]);
	m_isShowedAnnounce = true;
	openPopup(kAnnounceView);
}




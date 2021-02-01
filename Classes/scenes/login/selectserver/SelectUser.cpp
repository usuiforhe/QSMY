#include "SelectUser.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace platform;


#define NAMECELL_SIZE CCSizeMake(265, 41)

SelectUser::SelectUser()
	:m_pCloseBtn(NULL),
	m_pNameContainer(NULL),
	m_pTab1(NULL),
	m_pTab2(NULL),
	m_pRegister(NULL),
	m_pQuickLoginNode(NULL),
	m_pLoginNode(NULL),
	m_pMenuUp(NULL),
	m_pMenuDown(NULL),
	m_pNameCell(NULL),
	m_pDropBox(NULL),
	m_pDropContainer(NULL),
	m_pData(NULL),
	m_pRegisterBtn(NULL),
	m_pLoginOthers(NULL)
{

}

SelectUser::~SelectUser()
{
	CC_SAFE_RELEASE(m_pCloseBtn);
	CC_SAFE_RELEASE(m_pNameContainer);
	CC_SAFE_RELEASE(m_pTab1);
	CC_SAFE_RELEASE(m_pTab2);
	CC_SAFE_RELEASE(m_pRegister);
	CC_SAFE_RELEASE(m_pQuickLoginNode);
	CC_SAFE_RELEASE(m_pLoginNode);
	CC_SAFE_RELEASE(m_pMenuUp);
	CC_SAFE_RELEASE(m_pMenuDown);
	CC_SAFE_RELEASE(m_pNameCell);
	CC_SAFE_RELEASE(m_pDropBox);
	CC_SAFE_RELEASE(m_pDropContainer);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pRegisterBtn);
	CC_SAFE_RELEASE(m_pLoginOthers);
}


SelectUser* SelectUser::create(LoginType type/*=kQuickLogin*/)
{
	SelectUser* pRet = new SelectUser();
	if(pRet && pRet->init(type))
	{
		pRet->autorelease();
		return pRet;
	} else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool SelectUser::init(LoginType type)
{
	if(!DLayer::init()) return false;
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("NameCell",NameCellLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_denglu/PL_denglu_gengming", pNodeLoaderLibrary);

	m_pCloseBtn->getButton()->setTarget(this, menu_selector(SelectUser::closePopup));

	m_pData = readCCData("PL_ui/ccb/pl_denglu/PL_denglu_gengming_xia_view");
	CC_SAFE_RETAIN(m_pData);

	m_pListView = DTableView::create(this, m_pDropContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pDropContainer->addChild(m_pListView);
	CC_SAFE_RETAIN(m_pListView);

	m_pLoginType = type;


	m_pDropBox->setVisible(false);
	m_pMenuUp->setVisible(false);
	m_pMenuDown->setVisible(true);

	
	return true;
}

void SelectUser::onExit()
{
	UNBINDING_ALL(DM_GET_PLATFORM_MODEL, this);
	DLayer::onExit();
}

void SelectUser::onEnter()
{
    m_pLoginOthers->setTarget(this, menu_selector(SelectUser::loginOthersCallback));
	BINDING_EXEC(DM_GET_PLATFORM_MODEL, this, SelectUser::updateView, D_EVENT_LOGINED_USER_CHANGED);
	DLayer::onEnter();
}

void SelectUser::updateView()
{
	if(DM_GET_PLATFORM_MODEL->getLoginedUsers()->count() == 0)
	{
		m_pDelegate->showInfo(KLoginTypeSelect);
	} else 
	{

		reset();
		switch(m_pLoginType)
		{
		case kQuickLogin:
			m_pTab2->setVisible(true);
			m_pQuickLoginNode->setVisible(true);
			break;
		case kLogin:
			m_pTab2->setVisible(true);
			m_pLoginNode->setVisible(true);
			break;
		case kFacebookLogin:
			m_pTab1->setVisible(true);
			m_pRegister->setVisible(true);
			break;
		case kGoogelPlusLogin:
			m_pTab1->setVisible(true);
			m_pRegister->setVisible(true);
			break;
		}
		std::string platId = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID,"");
		int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
		m_pNameCell->setUserLoginInfo((UserLoginInfo*)DM_GET_PLATFORM_MODEL->getLoginedUserByName(platId.c_str(), iLoginType));
		m_pListView->reloadData();
	}
}

void SelectUser::reset()
{
	m_pTab1->setVisible(false);
	m_pTab2->setVisible(false);
	m_pRegister->setVisible(false);
	m_pQuickLoginNode->setVisible(false);
	m_pLoginNode->setVisible(false);
}

void SelectUser::closePopup(cocos2d::CCObject*)
{
	if(m_pLoginType == kQuickLogin)  
	{
	     m_pDelegate->showInfo(KLoginTypeSelect);
	}
}


bool SelectUser::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTab1", CCNode*, m_pTab1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTab2", CCNode*, m_pTab2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRegister", CCSprite*, m_pRegister);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pQuickLoginNode", CCNode*, m_pQuickLoginNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLoginNode", CCNode*, m_pLoginNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pNameContainer",CCNode*,m_pNameContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuUp", CCMenu*, m_pMenuUp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuDown", CCMenu*, m_pMenuDown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameCell", NameCell*, m_pNameCell);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDropBox", CCNode*, m_pDropBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDropContainer", CCNode*, m_pDropContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRegisterBtn", CCMenuItem*, m_pRegisterBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLoginOthers", CCMenuItem*, m_pLoginOthers);
	return true;
}

SEL_MenuHandler SelectUser::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallback", SelectUser::menuCallback);
	return NULL;
}

void SelectUser::loginOthersCallback(cocos2d::CCObject* obj)
{
	m_pDelegate->openPopup(kLoginView);
}

void SelectUser::menuCallback(cocos2d::CCObject* obj)
{
	int tag = ((CCMenuItem*)obj)->getTag();
	switch(tag)
	{
	case kLoginButton:
		{
			m_pDelegate->showInfo(kLoginPageSelectServer);
		}
		break;
	case kRegisterOrBindingButton:
		{
			m_pDelegate->openPopup(KRegisterView);
		}
		break;
	case kRegisterButton:
		m_pDelegate->openPopup(KRegisterView);
		break;		
	case kChangePWButton:
		m_pDelegate->openPopup(kChangePWView);
		break;
	case kBindingForQuickLoginBtn:
		m_pDelegate->openPopup(KBindingView);
		break;
	case kFacebookLoginButton:
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kFacebookLogin);
		CCUserDefault::sharedUserDefault()->flush();
		PlatformManager::sharedPlatformManager()->loginByType(kFacebookLogin);
		break;
	case kGoogelPlusLoginButton:
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kGoogelPlusLogin);
		CCUserDefault::sharedUserDefault()->flush();

		break;
	case kMenuUp:
		m_pDropBox->setVisible(false);
		m_pMenuUp->setVisible(false);
		m_pMenuDown->setVisible(true);
		break;
	case kMenuDown:
		m_pDropBox->setVisible(true);
		m_pMenuUp->setVisible(true);
		m_pMenuDown->setVisible(false);
		break;
	default: break;
	}

}

cocos2d::CCSize SelectUser::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return NAMECELL_SIZE;
}

void SelectUser::tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell)
{
	NameCellView* pNameView = (NameCellView*)cell;
	UserLoginInfo* pUser = pNameView->getUserLoginInfo();
	m_pNameCell->setUserLoginInfo(pUser);

	m_pDropBox->setVisible(false);
	m_pMenuUp->setVisible(false);
	m_pMenuDown->setVisible(true);

	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, pUser->getLoginName());
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_PLATID, pUser->getPlatId());
	CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, pUser->getPassword());
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,pUser->getLogType());
	CCUserDefault::sharedUserDefault()->flush();

	m_pLoginType = (LoginType)pUser->getLogType();
	updateView();
}

cocos2d::extension::CCTableViewCell* SelectUser::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	NameCellView *pCell = (NameCellView*)table->dequeueCell();
	if(!pCell)
	{
		pCell = NameCellView::create(m_pData);
		
	}
	pCell->setUserLoginInfo((UserLoginInfo*)DM_GET_PLATFORM_MODEL->getLoginedUsers()->objectAtIndex(idx));
	pCell->setDelegate(m_pDelegate);
	return pCell;
}

unsigned int SelectUser::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return DM_GET_PLATFORM_MODEL->getLoginedUsers()->count();
}

#pragma mark -- NameCell 

NameCell::NameCell()
	:m_pGoogleIcon(NULL),
	m_pFacebookIcon(NULL),
	m_pGuessIcon(NULL),
	m_pUserIcon(NULL),
	m_pName(NULL)
{

}

NameCell::~NameCell()
{
	CC_SAFE_RELEASE(m_pGoogleIcon);
	CC_SAFE_RELEASE(m_pFacebookIcon);
	CC_SAFE_RELEASE(m_pGuessIcon);
	CC_SAFE_RELEASE(m_pUserIcon);
	CC_SAFE_RELEASE(m_pName);
	
}

bool NameCell::init()
{
	if(!CCNode::init())
	{
		return false;
	}


	return true;
}

bool NameCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGoogleIcon", CCSprite*, m_pGoogleIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFacebookIcon", CCSprite*, m_pFacebookIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuessIcon", CCSprite*, m_pGuessIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUserIcon", CCSprite*, m_pUserIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, m_pName);
	return true;
}

void NameCell::setUserLoginInfo(UserLoginInfo* pUserInfo)
{
	m_pUserIcon->setVisible(false);
	m_pGuessIcon->setVisible(false);
	m_pFacebookIcon->setVisible(false);
	m_pGoogleIcon->setVisible(false);
	
	if(!pUserInfo)
	{
		m_pName->setString(D_LOCAL_STRING("$logintips8").c_str());
	} else 
	{
		CCLOG("logined user in name cell:%s, %s, %d, %s(fbname)",pUserInfo->getLoginName().c_str(),pUserInfo->getPassword().c_str(), pUserInfo->getLogType(),pUserInfo->getFacebookName().c_str());
		std::string pLangId("");
		switch(pUserInfo->getLogType())
		{
		case kLogin:
			pLangId = "";
			m_pUserIcon->setVisible(true);
			break;
		case kQuickLogin:
			m_pGuessIcon->setVisible(true);
			pLangId = "$loginyouke";
			break;
		case kFacebookLogin:
			m_pFacebookIcon->setVisible(true);
			pLangId = "$loginFB";
			break;
		case kGoogelPlusLogin:
			m_pGoogleIcon->setVisible(true);
			pLangId = "$loginGP";
			break;
		default: break;
		}
		
        if (pUserInfo->getLogType() == kFacebookLogin) {
            m_pName->setString(CCString::createWithFormat(D_LOCAL_STRING(pLangId).c_str(), pUserInfo->getFacebookName().c_str())->getCString());
		} else if (pUserInfo->getLogType() == kQuickLogin) {
			m_pName->setString(CCString::createWithFormat(D_LOCAL_STRING(pLangId).c_str(), pUserInfo->getPlatId().c_str())->getCString());
		} else if (pLangId.empty()) {
            m_pName->setString(pUserInfo->getLoginName().c_str());
        } else
        {
            m_pName->setString(CCString::createWithFormat(D_LOCAL_STRING(pLangId).c_str(), pUserInfo->getLoginName().c_str())->getCString());
        }
		//m_pName->setString(CCString::createWithFormat(D_LOCAL_STRING(pLangId).c_str(), pUserInfo->getLoginName().c_str())->getCString());
        //m_pName->setString(pUserInfo->getLoginName().c_str());

	}
	
}


#pragma mark -- NameCellView 

NameCellView::NameCellView()
	:m_pNameCell(NULL),
	m_pClose(NULL),
	m_pUserInfo(NULL)
{

}

NameCellView::~NameCellView()
{
	CC_SAFE_RELEASE(m_pNameCell);
	CC_SAFE_RELEASE(m_pClose);
	CC_SAFE_RELEASE(m_pUserInfo);
}

NameCellView* NameCellView::create(cocos2d::extension::CCData* pData)
{
	NameCellView* pRet = new NameCellView();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	} else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool NameCellView::init(cocos2d::extension::CCData* pData)
{
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("NameCell",NameCellLoader::loader());
	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);
	m_pClose->setTarget(this, menu_selector(NameCellView::menuCallback));
	return true;
}

bool NameCellView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameCell", NameCell*, m_pNameCell);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClose", CCMenuItem*, m_pClose);
	return true;
}

SEL_MenuHandler NameCellView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallback", NameCellView::menuCallback);
	return NULL;
}

void NameCellView::menuCallback(cocos2d::CCObject* obj)
{
	int iLoginType = m_pUserInfo->getLogType();
	if(iLoginType == kQuickLogin)
	{
		m_pDelegate->openPopup(kRemoveUserTipsView);
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_GUESS_USER_NAME, m_pUserInfo->getLoginName());
		CCUserDefault::sharedUserDefault()->flush();
	} else 
	{
		DM_GET_PLATFORM_MODEL->removeLoginedUser(m_pUserInfo);
	}
}

void NameCellView::setUserLoginInfo(UserLoginInfo* pUserInfo)
{
	if(m_pUserInfo==pUserInfo) return;
	CC_SAFE_RETAIN(pUserInfo);
	m_pUserInfo = pUserInfo;
	m_pNameCell->setUserLoginInfo(pUserInfo);
}



#pragma mark -- RemoveUserTipsView 


RemoveUserTipsView::RemoveUserTipsView()
	:m_pDeleteBtn(NULL)
	,m_pBindBtn(NULL)
	,m_pCloseBtn(NULL)
{
}

RemoveUserTipsView::~RemoveUserTipsView()
{
	CC_SAFE_RELEASE(m_pDeleteBtn);
	CC_SAFE_RELEASE(m_pBindBtn);
	CC_SAFE_RELEASE(m_pCloseBtn);
	
}

bool RemoveUserTipsView::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_denglu_tishi");
	return true;
}

void RemoveUserTipsView::onEnter()
{
	m_pDeleteBtn->getButton()->setTarget(this, menu_selector(RemoveUserTipsView::deleteCallback));
	m_pBindBtn->getButton()->setTarget(this, menu_selector(RemoveUserTipsView::bindCallback));
	m_pCloseBtn->getButton()->setTarget(this, menu_selector(RemoveUserTipsView::closePopup));

	DPopup::onEnter();
}

void RemoveUserTipsView::deleteCallback(cocos2d::CCObject*)
{
	std::string pDeGuessUserName = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_GUESS_USER_NAME,"");
	DM_GET_PLATFORM_MODEL->removeLoginedUser(pDeGuessUserName.c_str(), kQuickLogin);
	DPopup::closePopup();
}
void RemoveUserTipsView::bindCallback(cocos2d::CCObject*)
{
	DPopup::closePopup();
	m_pDelegate->openPopup(KBindingView);
}

void RemoveUserTipsView::closePopup(cocos2d::CCObject*)
{
	DPopup::closePopup();
}


bool RemoveUserTipsView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pDeleteBtn",DButton*,m_pDeleteBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pBindBtn",DButton*,m_pBindBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	return false;
}










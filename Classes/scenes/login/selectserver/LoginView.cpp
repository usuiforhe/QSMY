#include "LoginView.h"
#include "managers/PlatformManager.h"
USING_NS_CC;

LoginView::LoginView()
	:m_pLogin(NULL)
	,m_pRegister(NULL)
	,m_pCloseBtn(NULL)
	,m_pTips(NULL)
{
	D_RESET_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_2);
	D_RESET_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_2);
}

LoginView::~LoginView()
{
	CC_SAFE_RELEASE(m_pLogin);
	CC_SAFE_RELEASE(m_pRegister);
	CC_SAFE_RELEASE(m_pCloseBtn);
	CC_SAFE_RELEASE(m_pTips);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_2);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_2);
}

bool LoginView::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_loginview");

	const char* const pPlaceHodlerKeyArr[EDIT_BOX_COUNT_2] = {"$logintips1", "$logintips2"};

	CCEditBox *pEditBoxTemp = NULL;
	for(int i=0; i<EDIT_BOX_COUNT_2; i++){
		pEditBoxTemp = CCEditBox::create(LOGIN_EDIT_BOX_SIZE, CCScale9Sprite::create());
		pEditBoxTemp->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
		pEditBoxTemp->setFontColor(ccWHITE);
		pEditBoxTemp->setPlaceHolder(D_LOCAL_STRING(pPlaceHodlerKeyArr[i]).c_str());
		pEditBoxTemp->setPlaceholderFontColor(PLACE_HOLDER_COLOR);
		pEditBoxTemp->setMaxLength(EDIT_BOX_MAX_LEN);
		pEditBoxTemp->setReturnType(kKeyboardReturnTypeDone);
		pEditBoxTemp->setDelegate(this);
		pEditBoxTemp->setAnchorPoint(CCPointZero);
		CC_SAFE_RETAIN(pEditBoxTemp);
		m_pEditBoxArr[i] = pEditBoxTemp;
		m_pEditBoxContainer[i]->addChild(pEditBoxTemp);
	}
	m_pEditBoxArr[1]->setInputFlag(kEditBoxInputFlagPassword);
	int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
	if(iLoginType == kLogin) 
	{
		m_pEditBoxArr[0]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME, "").c_str());
		m_pEditBoxArr[1]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD, "").c_str());
	}

	return true;
}

void LoginView::onEnter()
{
	m_pLogin->getButton()->setTarget(this, menu_selector(LoginView::loginCallBack));
	m_pRegister->getButton()->setTarget(this, menu_selector(LoginView::registerCallBack));
	m_pCloseBtn->getButton()->setTarget(this, menu_selector(LoginView::closePopup));

	DPopup::onEnter();
}


void LoginView::menucallback(cocos2d::CCObject* sender)
{
	CCMenuItem *menu = static_cast<CCMenuItem*>(sender);
	switch(menu->getTag())
	{
	case kMenuTag0:
		m_pEditBoxArr[kMenuTag0]->setText("");
		break;
	case kMenuTag1:
		m_pEditBoxArr[kMenuTag1]->setText("");
		break;
	}
}

void LoginView::loginCallBack(cocos2d::CCObject* sender)
{
	DPopup::closePopup();
	if(strcmp(m_pEditBoxArr[0]->getText(), "") == 0)
		return;
	if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, m_pEditBoxArr[0]->getText());
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, m_pEditBoxArr[1]->getText());
		CCUserDefault::sharedUserDefault()->flush();
		m_pDelegate->showInfo(kLoginPageSelectServer);
	} else
	{
		m_pDelegate->loginGame(m_pEditBoxArr[0]->getText(), m_pEditBoxArr[1]->getText());
	}
	
	
}
void LoginView::registerCallBack(cocos2d::CCObject*)
{
	DPopup::closePopup();
	m_pDelegate->openPopup(KRegisterView);
}


void LoginView::closePopup(cocos2d::CCObject*)
{
	/*const char* pUserName = m_pEditBoxArr[0]->getText();
	if(strcmp(pUserName, "") != 0)
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, pUserName);
		const char* pPassworkd = m_pEditBoxArr[1]->getText();
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, pPassworkd);
	}*/
	
	DPopup::closePopup();
}


bool LoginView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	for(int i=0; i<EDIT_BOX_COUNT_2; i++)
	{
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,CCString::createWithFormat("m_pEditBoxContainer%d",i)->getCString(),CCNode*,m_pEditBoxContainer[i]);
	}
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pLogin",DButton*,m_pLogin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pRegister",DButton*,m_pRegister);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	return false;
}

SEL_MenuHandler LoginView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menucallback", LoginView::menucallback);
	return NULL;
}


#pragma mark --------------------LoginViewPW--------------------
LoginViewPW::LoginViewPW()
	:m_pChangePWBtn(NULL)
	,m_pLogin(NULL)
	,m_pRegister(NULL)
	,m_pCloseBtn(NULL)
	,m_pTips(NULL)
{
	D_RESET_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_2);
	D_RESET_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_2);
}

LoginViewPW::~LoginViewPW()
{
	CC_SAFE_RELEASE(m_pLogin);
	CC_SAFE_RELEASE(m_pRegister);
	CC_SAFE_RELEASE(m_pCloseBtn);
	CC_SAFE_RELEASE(m_pTips);
	CC_SAFE_RELEASE(m_pChangePWBtn);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_2);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_2);
}

bool LoginViewPW::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_loginview_pw");

	const char* const pPlaceHodlerKeyArr[EDIT_BOX_COUNT_2] = {"$logintips1", "$logintips2"};

	CCEditBox *pEditBoxTemp = NULL;
	for(int i=0; i<EDIT_BOX_COUNT_2; i++){
		pEditBoxTemp = CCEditBox::create(LOGIN_EDIT_BOX_SIZE, CCScale9Sprite::create());
		pEditBoxTemp->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
		pEditBoxTemp->setFontColor(ccWHITE);
		pEditBoxTemp->setPlaceHolder(D_LOCAL_STRING(pPlaceHodlerKeyArr[i]).c_str());
		pEditBoxTemp->setPlaceholderFontColor(PLACE_HOLDER_COLOR);
		pEditBoxTemp->setMaxLength(EDIT_BOX_MAX_LEN);
		pEditBoxTemp->setReturnType(kKeyboardReturnTypeDone);
		pEditBoxTemp->setDelegate(this);
		pEditBoxTemp->setAnchorPoint(CCPointZero);
		CC_SAFE_RETAIN(pEditBoxTemp);
		m_pEditBoxArr[i] = pEditBoxTemp;
		m_pEditBoxContainer[i]->addChild(pEditBoxTemp);
	}
	m_pEditBoxArr[1]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[0]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME, "").c_str());
	m_pEditBoxArr[1]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD, "").c_str());

	return true;
}

void LoginViewPW::onEnter()
{
	m_pLogin->getButton()->setTarget(this, menu_selector(LoginViewPW::loginCallBack));
	m_pRegister->getButton()->setTarget(this, menu_selector(LoginViewPW::registerCallBack));
	m_pCloseBtn->getButton()->setTarget(this, menu_selector(LoginViewPW::closePopup));

	m_pChangePWBtn->getButton()->setTarget(this, menu_selector(LoginViewPW::changePWCallback));
	DPopup::onEnter();
}

void LoginViewPW::changePWCallback(cocos2d::CCObject*)
{
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, m_pEditBoxArr[0]->getText());
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, m_pEditBoxArr[1]->getText());
	DPopup::closePopup();
	m_pDelegate->openPopup(kChangePWView);
}

void LoginViewPW::loginCallBack(cocos2d::CCObject* sender)
{
	DPopup::closePopup();
	m_pDelegate->loginGame(m_pEditBoxArr[0]->getText(), m_pEditBoxArr[1]->getText());

}
void LoginViewPW::registerCallBack(cocos2d::CCObject*)
{
	DPopup::closePopup();
	m_pDelegate->openPopup(KRegisterView);
}
void LoginViewPW::closePopup(cocos2d::CCObject*)
{
	/*const char* pUserName = m_pEditBoxArr[0]->getText();
	if(strcmp(pUserName, "") != 0)
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, pUserName);
		const char* pPassworkd = m_pEditBoxArr[1]->getText();
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, pPassworkd);
	}*/
	DPopup::closePopup();
	//m_pDelegate->showInfo(kLoginPageSelectServer);
}



void LoginViewPW::menucallback(cocos2d::CCObject* sender)
{
	CCMenuItem *menu = static_cast<CCMenuItem*>(sender);
	switch(menu->getTag())
	{
	case kMenuTag0:
		m_pEditBoxArr[kMenuTag0]->setText("");
		break;
	case kMenuTag1:
		m_pEditBoxArr[kMenuTag1]->setText("");
		break;
	}
}

bool LoginViewPW::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	for(int i=0; i<EDIT_BOX_COUNT_2; i++)
	{
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,CCString::createWithFormat("m_pEditBoxContainer%d",i)->getCString(),CCNode*,m_pEditBoxContainer[i]);
	}
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pLogin",DButton*,m_pLogin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pRegister",DButton*,m_pRegister);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pChangePWBtn",DButton*,m_pChangePWBtn);
	return false;
}

SEL_MenuHandler LoginViewPW::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menucallback", LoginViewPW::menucallback);
	return NULL;
}


#pragma mark --------------------ChangePWView--------------------
ChangePWView::ChangePWView()
	:m_pChangePWBtn(NULL)
	,m_pCloseBtn(NULL)
	,m_pTips(NULL)
{
	D_RESET_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_4);
	D_RESET_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_4);
}

ChangePWView::~ChangePWView()
{
	CC_SAFE_RELEASE(m_pChangePWBtn);
	CC_SAFE_RELEASE(m_pCloseBtn);
	CC_SAFE_RELEASE(m_pTips);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_4);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_4);
}

bool ChangePWView::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_changePW");

	const char* const pPlaceHodlerKeyArr[EDIT_BOX_COUNT_4] = {"$logintips1", "$logintips5", "$logintips6", "$logintips7"};

	CCEditBox *pEditBoxTemp = NULL;
	for(int i=0; i<EDIT_BOX_COUNT_4; i++){
		pEditBoxTemp = CCEditBox::create(LOGIN_EDIT_BOX_SIZE, CCScale9Sprite::create());
		pEditBoxTemp->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
		pEditBoxTemp->setFontColor(ccWHITE);
		pEditBoxTemp->setPlaceHolder(D_LOCAL_STRING(pPlaceHodlerKeyArr[i]).c_str());
		pEditBoxTemp->setPlaceholderFontColor(PLACE_HOLDER_COLOR);
		pEditBoxTemp->setMaxLength(EDIT_BOX_MAX_LEN);
		pEditBoxTemp->setReturnType(kKeyboardReturnTypeDone);
		pEditBoxTemp->setDelegate(this);
		pEditBoxTemp->setAnchorPoint(CCPointZero);
		CC_SAFE_RETAIN(pEditBoxTemp);
		m_pEditBoxArr[i] = pEditBoxTemp;
		m_pEditBoxContainer[i]->addChild(pEditBoxTemp);
	}
	m_pEditBoxArr[1]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[2]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[3]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[0]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME, "").c_str());
	//m_pEditBoxArr[1]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD, "").c_str());

	return true;
}

void ChangePWView::onEnter()
{
	m_pCloseBtn->getButton()->setTarget(this, menu_selector(ChangePWView::closePopup));
	m_pChangePWBtn->getButton()->setTarget(this, menu_selector(ChangePWView::changePWCallback));
	DPopup::onEnter();
}

void ChangePWView::changePWCallback(cocos2d::CCObject*)
{
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, m_pEditBoxArr[0]->getText());
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, m_pEditBoxArr[1]->getText());
	/************************************************************************/
	/* 1：旧密码是否少于6个字符（客户端判断）不超过15位
	2：新密码是否少于6个字符（客户端判断）不超过15位
	3：两次输入的密码是否一致（客户端判断）
	4：旧密码是否正确（服务端验证）
                                                                     */
	/************************************************************************/

	const char* pUserName = m_pEditBoxArr[0]->getText();
	const char* pOldPass = m_pEditBoxArr[1]->getText();
	const char* pNewPass = m_pEditBoxArr[2]->getText();
	const char* pConfirmPass = m_pEditBoxArr[3]->getText();
	if(!pUserName || (strcmp(pUserName, "") == 0)) 
	{	
		Tips_Alert(D_LOCAL_STRING("$logintips11").c_str());
		return;
	}
	uint32_t pPassLen = strlen(pNewPass);
	if(pPassLen < 6 || pPassLen > 15)
	{
		Tips_Alert(D_LOCAL_STRING("$logintips12").c_str());
		return;
	}
	if(strcmp(pNewPass, pConfirmPass) != 0)
	{
		m_pEditBoxArr[2]->setText("");
		m_pEditBoxArr[3]->setText("");
		Tips_Alert(D_LOCAL_STRING("$logintips13").c_str());
		return;
	}

	DPopup::closePopup();
	
	//调用服务端api修改密码
	m_pDelegate->resetPass(pUserName, pOldPass, pNewPass, pConfirmPass);
}

void ChangePWView::closePopup(cocos2d::CCObject*)
{
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, m_pEditBoxArr[0]->getText());
	//CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, m_pEditBoxArr[1]->getText());
	DPopup::closePopup();
	//m_pDelegate->showInfo(kLoginPageSelectServer);
	//m_pDelegate->openPopup(kLoginPWView);
}


void ChangePWView::menucallback(cocos2d::CCObject* sender)
{
	CCMenuItem *menu = static_cast<CCMenuItem*>(sender);
	switch(menu->getTag())
	{
	case kMenuTag0:
		m_pEditBoxArr[kMenuTag0]->setText("");
		break;
	case kMenuTag1:
		m_pEditBoxArr[kMenuTag1]->setText("");
		break;
	case kMenuTag2:
		m_pEditBoxArr[kMenuTag2]->setText("");
		break;
	case kMenuTag3:
		m_pEditBoxArr[kMenuTag3]->setText("");
		break;
	}
}

bool ChangePWView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	for(int i=0; i<EDIT_BOX_COUNT_4; i++)
	{
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,CCString::createWithFormat("m_pEditBoxContainer%d",i)->getCString(),CCNode*,m_pEditBoxContainer[i]);
	}
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pChangePWBtn",DButton*,m_pChangePWBtn);
	return false;
}

SEL_MenuHandler ChangePWView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menucallback", ChangePWView::menucallback);
	return NULL;
}





#pragma mark --------------------RegisterView--------------------
RegisterView::RegisterView()
	:m_pLogin(NULL)
	,m_pCloseBtn(NULL)
	,m_pTips(NULL)
	,m_bBinding(false)
	,m_bClauseAgreed(true)
	,m_pClauseAgreeBtn(NULL)
{
	D_RESET_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_4);
	D_RESET_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_4);
}

RegisterView::~RegisterView()
{
	CC_SAFE_RELEASE(m_pLogin);
	CC_SAFE_RELEASE(m_pCloseBtn);
	CC_SAFE_RELEASE(m_pTips);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_4);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_4);
	CC_SAFE_RELEASE(m_pClauseAgreeBtn);
}

bool RegisterView::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_registerview");

	const char* const pPlaceHodlerKeyArr[EDIT_BOX_COUNT_4] = {"$logintips1", "$logintips2", "$logintips3", "$logintips4"};

	CCEditBox *pEditBoxTemp = NULL;
	for(int i=0; i<EDIT_BOX_COUNT_4; i++){
		pEditBoxTemp = CCEditBox::create(LOGIN_EDIT_BOX_SIZE, CCScale9Sprite::create());
		pEditBoxTemp->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
		pEditBoxTemp->setFontColor(ccWHITE);
		pEditBoxTemp->setPlaceHolder(D_LOCAL_STRING(pPlaceHodlerKeyArr[i]).c_str());
		pEditBoxTemp->setPlaceholderFontColor(PLACE_HOLDER_COLOR);
		pEditBoxTemp->setMaxLength(EDIT_BOX_MAX_LEN);
		pEditBoxTemp->setReturnType(kKeyboardReturnTypeDone);
		pEditBoxTemp->setDelegate(this);
		pEditBoxTemp->setAnchorPoint(CCPointZero);
		CC_SAFE_RETAIN(pEditBoxTemp);
		m_pEditBoxArr[i] = pEditBoxTemp;
		m_pEditBoxContainer[i]->addChild(pEditBoxTemp);
	}
	m_pEditBoxArr[1]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[2]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[3]->setMaxLength(50);
	m_pEditBoxArr[3]->setInputMode(kEditBoxInputModeEmailAddr);

	return true;
}

void RegisterView::onEnter()
{
	m_pLogin->getButton()->setTarget(this, menu_selector(RegisterView::loginCallBack));
	m_pCloseBtn->getButton()->setTarget(this, menu_selector(RegisterView::closePopup));
	m_pClauseAgreeBtn->getButton()->setTarget(this, menu_selector(RegisterView::agreeClause));
	if(m_bClauseAgreed)
	{
		m_pClauseAgreeBtn->getButton()->selected();
	} else
	{
		m_pClauseAgreeBtn->getButton()->unselected();
	}
	DPopup::onEnter();
}

void RegisterView::loginCallBack(cocos2d::CCObject* sender)
{
	if(!m_bClauseAgreed)
	{
		Tips_Alert(D_LOCAL_STRING("$logintips15").c_str());
		return;
	}
	//检测账号密码是否符合限制 
	/************************************************************************/
	/* 1：账号是否为空（客户端判断）不超过15位			
	2：密码是否少于6个字符（客户端判断）不超过15位			
	3：两次输入的密码是否一致（客户端判断）			
	4：用户是否存在（服务端验证）			
                                                                     */
	/************************************************************************/
	const char* pUserName = m_pEditBoxArr[0]->getText();
	const char* pPassword = m_pEditBoxArr[1]->getText();
	const char* pConfirmPass = m_pEditBoxArr[2]->getText();
	const char* pEmail = m_pEditBoxArr[3]->getText();
	if(!pUserName || (strcmp(pUserName, "") == 0))
	{	
		Tips_Alert(D_LOCAL_STRING("$logintips11").c_str());
		return;
	}
	uint32_t pPassLen = strlen(pPassword);
	if(pPassLen < 6 || pPassLen > 15)
	{
		Tips_Alert(D_LOCAL_STRING("$logintips12").c_str());
		return;
	}
	if(strcmp(pPassword, pConfirmPass) != 0)
	{
		m_pEditBoxArr[1]->setText("");
		m_pEditBoxArr[2]->setText("");
		Tips_Alert(D_LOCAL_STRING("$logintips13").c_str());
		return;
	}
	

	DPopup::closePopup();
	
	m_pDelegate->registerUser(pUserName, pPassword, pConfirmPass, pEmail);
	
}
void RegisterView::closePopup(cocos2d::CCObject*)
{
	DPopup::closePopup();
	//m_pDelegate->showInfo(kLoginPageSelectServer);
}

void RegisterView::agreeClause(cocos2d::CCObject*)
{
	m_bClauseAgreed = !m_bClauseAgreed;
	if(m_bClauseAgreed)
	{
		m_pClauseAgreeBtn->getButton()->selected();
	} else
	{
		m_pClauseAgreeBtn->getButton()->unselected();
	}
}

void RegisterView::menucallback(cocos2d::CCObject* sender)
{
	CCMenuItem *menu = static_cast<CCMenuItem*>(sender);
	switch(menu->getTag())
	{
	case kMenuTag0:
		m_pEditBoxArr[kMenuTag0]->setText("");
		break;
	case kMenuTag1:
		m_pEditBoxArr[kMenuTag1]->setText("");
		break;
	case kMenuTag2:
		m_pEditBoxArr[kMenuTag2]->setText("");
		break;
	case kMenuTag3:
		m_pEditBoxArr[kMenuTag3]->setText("");
		break;
	case kMenuTag4:
		SystemUtil::openURL(D_LOCAL_STRING("$clauseURL").c_str());
		break;
	}
}

bool RegisterView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	for(int i=0; i<EDIT_BOX_COUNT_4; i++)
	{
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,CCString::createWithFormat("m_pEditBoxContainer%d",i)->getCString(),CCNode*,m_pEditBoxContainer[i]);
	}
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pClauseAgreeBtn",DButton*,m_pClauseAgreeBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pLogin",DButton*,m_pLogin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	return false;
}

SEL_MenuHandler RegisterView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menucallback", RegisterView::menucallback);
	return NULL;
}


#pragma mark - - BindingView--
BindingView::BindingView()
	:m_pLogin(NULL)
	,m_pCloseBtn(NULL)
	,m_pTips(NULL)
	,m_pGuessUser(NULL)
	,m_pClauseAgreeBtn(NULL)
	,m_bClauseAgreed(true)
{
	D_RESET_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_3);
	D_RESET_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_3);
}

BindingView::~BindingView()
{
	CC_SAFE_RELEASE(m_pLogin);
	CC_SAFE_RELEASE(m_pCloseBtn);
	CC_SAFE_RELEASE(m_pTips);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxArr, EDIT_BOX_COUNT_3);
	D_SAFE_RELEASE_C_ARRAY(m_pEditBoxContainer, EDIT_BOX_COUNT_3);
	CC_SAFE_RELEASE(m_pGuessUser);
	CC_SAFE_RELEASE(m_pClauseAgreeBtn);
}

bool BindingView::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_loginview2");

	const char* const pPlaceHodlerKeyArr[EDIT_BOX_COUNT_3] = {"$logintips1", "$logintips2", "$logintips3"};

	CCEditBox *pEditBoxTemp = NULL;
	for(int i=0; i<EDIT_BOX_COUNT_3; i++){
		pEditBoxTemp = CCEditBox::create(LOGIN_EDIT_BOX_SIZE, CCScale9Sprite::create());
		pEditBoxTemp->setFont(D_FONT_DEFAULT,D_FONT_SIZE_12);
		pEditBoxTemp->setFontColor(ccWHITE);
		pEditBoxTemp->setPlaceHolder(D_LOCAL_STRING(pPlaceHodlerKeyArr[i]).c_str());
		pEditBoxTemp->setPlaceholderFontColor(PLACE_HOLDER_COLOR);
		pEditBoxTemp->setMaxLength(EDIT_BOX_MAX_LEN);
		pEditBoxTemp->setReturnType(kKeyboardReturnTypeDone);
		pEditBoxTemp->setDelegate(this);
		pEditBoxTemp->setAnchorPoint(CCPointZero);
		CC_SAFE_RETAIN(pEditBoxTemp);
		m_pEditBoxArr[i] = pEditBoxTemp;
		m_pEditBoxContainer[i]->addChild(pEditBoxTemp);
	}
	m_pEditBoxArr[1]->setInputFlag(kEditBoxInputFlagPassword);
	m_pEditBoxArr[2]->setInputFlag(kEditBoxInputFlagPassword);
	//m_pEditBoxArr[0]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_NAME, "").c_str());
	//m_pEditBoxArr[1]->setText(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PASSWORD, "").c_str());
	m_pGuessUser->setString(D_LOCAL_STRING("$loginyouke", CCUserDefault::sharedUserDefault()->getStringForKey(KEY_USER_PLATID, "").c_str()).c_str());
	return true;
}

void BindingView::onEnter()
{
	m_pLogin->setTarget(this, menu_selector(BindingView::loginCallBack));
	
	m_pCloseBtn->getButton()->setTarget(this, menu_selector(BindingView::closePopup));
	m_pClauseAgreeBtn->getButton()->setTarget(this, menu_selector(BindingView::agreeClause));
	if(m_bClauseAgreed)
	{
		m_pClauseAgreeBtn->getButton()->selected();
	} else
	{
		m_pClauseAgreeBtn->getButton()->unselected();
	}
	DPopup::onEnter();
}

void BindingView::agreeClause(cocos2d::CCObject*)
{
	m_bClauseAgreed = !m_bClauseAgreed;
	if(m_bClauseAgreed)
	{
		m_pClauseAgreeBtn->getButton()->selected();
	} else
	{
		m_pClauseAgreeBtn->getButton()->unselected();
	}
}

void BindingView::menucallback(cocos2d::CCObject* sender)
{
	CCMenuItem *menu = static_cast<CCMenuItem*>(sender);
	switch(menu->getTag())
	{
	case kMenuTag0:
		m_pEditBoxArr[kMenuTag0]->setText("");
		break;
	case kMenuTag1:
		m_pEditBoxArr[kMenuTag1]->setText("");
		break;
	case kMenuTag2:
		m_pEditBoxArr[kMenuTag2]->setText("");
		break;
	case kMenuTag3:
		SystemUtil::openURL(D_LOCAL_STRING("$clauseURL").c_str());
		break;
	}
}

void BindingView::loginCallBack(cocos2d::CCObject* sender)
{
	if(!m_bClauseAgreed)
	{
		Tips_Alert(D_LOCAL_STRING("$logintips15").c_str());
		return;
	}
	const char* pUserName = m_pEditBoxArr[0]->getText();

	if(!pUserName || (strcmp(pUserName, "") == 0))
	{	
		Tips_Alert(D_LOCAL_STRING("$logintips11").c_str());
		return;
	}
	const char* pPassword = m_pEditBoxArr[1]->getText();
	const char* pConfirmPass = m_pEditBoxArr[2]->getText();
	uint32_t pPassLen = strlen(pPassword);
	if(pPassLen < 6 || pPassLen > 15)
	{
		Tips_Alert(D_LOCAL_STRING("$logintips12").c_str());
		return;
	}
	if(strcmp(pPassword, pConfirmPass) != 0)
	{
		m_pEditBoxArr[1]->setText("");
		m_pEditBoxArr[2]->setText("");
		Tips_Alert(D_LOCAL_STRING("$logintips13").c_str());
		return;
	}
	DPopup::closePopup();
	m_pDelegate->bindAccount(m_pEditBoxArr[0]->getText(), m_pEditBoxArr[1]->getText(), m_pEditBoxArr[2]->getText());
	
}
void BindingView::registerCallBack(cocos2d::CCObject*)
{
	DPopup::closePopup();
	m_pDelegate->openPopup(KRegisterView);
}


void BindingView::closePopup(cocos2d::CCObject*)
{
	/*const char* pUserName = m_pEditBoxArr[0]->getText();
	if(strcmp(pUserName, "") != 0)
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, pUserName);
		const char* pPassworkd = m_pEditBoxArr[1]->getText();
		CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, pPassworkd);
	}*/
	
	DPopup::closePopup();
}


bool BindingView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	for(int i=0; i<EDIT_BOX_COUNT_3; i++)
	{
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,CCString::createWithFormat("m_pEditBoxContainer%d",i)->getCString(),CCNode*,m_pEditBoxContainer[i]);
	}
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pLogin",CCMenuItem*,m_pLogin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pCloseBtn",DButton*,m_pCloseBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pGuessUser",CCLabelTTF*,m_pGuessUser);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pClauseAgreeBtn",DButton*,m_pClauseAgreeBtn);
	
	return false;
}

SEL_MenuHandler BindingView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menucallback", BindingView::menucallback);
	return NULL;
}

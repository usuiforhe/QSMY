
#include "SelectLoginType.h"
#include "managers/PlatformManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

SelectLoginType::SelectLoginType()
{

}

SelectLoginType::~SelectLoginType()
{

}


bool SelectLoginType::init()
{
	if(!DLayer::init()) return false;
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_denglu_kuaisu");
	return true;
}

bool SelectLoginType::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	return true;
}

SEL_MenuHandler SelectLoginType::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallback", SelectLoginType::menuCallback);
	return NULL;
}

void SelectLoginType::menuCallback(cocos2d::CCObject* obj)
{
	int tag = ((CCMenuItem*)obj)->getTag();
	switch(tag)
	{
	case 1://quick login
		{
			int iLoginType = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_LOGIN_TYPE, kLogin);
			if(iLoginType != kQuickLogin)
			{
				CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kQuickLogin);
				CCUserDefault::sharedUserDefault()->setStringForKey(KEY_USER_NAME, "");
				CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PASSWORD, "");
				CCUserDefault::sharedUserDefault()->flush();
			}
			//guess login
			m_pDelegate->showInfo(kLoginPageSelectServer);
			break;
		}
	case 2://login
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kLogin);
		m_pDelegate->openPopup(kLoginView);
		break;
	case 3://register
		
		m_pDelegate->openPopup(KRegisterView);
		break;
	case 4://facebook
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kFacebookLogin);
			//m_pDelegate->showInfo(kLoginPageSelectServer);
			PlatformManager::sharedPlatformManager()->loginByType(kFacebookLogin);
			break;
		}
	case 5://google+
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_LOGIN_TYPE,kGoogelPlusLogin);
			//m_pDelegate->showInfo(kLoginPageSelectServer);
			PlatformManager::sharedPlatformManager()->loginByType(kGoogelPlusLogin);
			break;
		}
	}
	CCUserDefault::sharedUserDefault()->flush();
}
//
//  Setting.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Setting.h"
#include "model/NavigationModel.h"
#include "managers/PlatformManager.h"
#include "../announce/Announce.h"
#include "../announce/GonggaolanView.h"

USING_NS_CC;
USING_NS_CC_EXT;

Setting::Setting()
:m_pSelected(NULL)
,m_pNavs(NULL)
,m_pDelegate(NULL)
,m_pMenu(NULL)
,m_pContainer(NULL)
,m_pCloseButton(NULL)
,m_pCurrentTitle(NULL)
,m_pCdkeyButton(NULL)
,m_pCdKeyText(NULL)
,m_pAccountButton(NULL)
,m_pAccountText(NULL)
{
    for (unsigned int i = kGuide; i < kSettingCount; i++)
    {
		m_titleMap[(SettingNav)i] = NULL;
    }
}

Setting::~Setting()
{
    CC_SAFE_RELEASE(m_pMenu);
    CC_SAFE_RELEASE(m_pNavs);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pCdKeyText);
    CC_SAFE_RELEASE(m_pCdkeyButton);
    CC_SAFE_RELEASE(m_pAccountButton);
    CC_SAFE_RELEASE(m_pAccountText);

	for (unsigned int i = kGuide; i < kSettingCount; i++)
	{
		CC_SAFE_RELEASE(m_titleMap[(SettingNav)i]);
	}

	NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

Setting* Setting::create(NavigationDelegate* pDelegate)
{
	Setting *pRet = new Setting();
    if (pRet && pRet->init(pDelegate))
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

bool Setting::init(NavigationDelegate* pDelegate)
{
    if(!DPopup::init()) return false;
	m_pDelegate = pDelegate;
    
	m_pNavs = CCDictionary::create();
	CC_SAFE_RETAIN(m_pNavs);
    
	cocos2d::CCNode *pNode = NULL;
	//CCB_READER("ui/ccb/settings/setting",pNode);
	CCB_READER("PL_ui/ccb/pl_shezhi/pl_shezhi",pNode);
	if(pNode)
	{
		addChild(pNode);
	}
	else
	{
		CCLog("ccb  read failed in Setting init.");
		return false;
	}
    
    uint32_t config = D_CONFIG_UINT_FOR_KEY(DISABLE_SETTING);

	if (m_pMenu == NULL)
	{
		CCLog("menu = null in Setting init.");
		return false;
	}
	
	//todo 6个界面都到位后再修改
	/*for (int i = 0; i < kSettingCount; i++)
	{
	CCMenuItem* pMenuItem = (CCMenuItem*)m_pMenu->getChildByTag(i);
	if (pMenuItem)
	{
	pMenuItem->setVisible(!(1 << i & config) && DM_GET_PLATFORM_MODEL->getSettingVisibleByIndex(i));
	CCNode* pWords = (CCNode*)pNode->getChildByTag(i);
	if (pWords)
	{
	pWords->setVisible(pMenuItem->isVisible());
	}
	}
	}

	CCPoint pt[kSettingCount];
	CCPoint ptWords[kSettingCount];
	for (int i = 0; i < kSettingCount; i++)
	{
	CCNode* pMenuItem = (CCNode*)m_pMenu->getChildByTag(i);
	if (pMenuItem)
	{
	pt[i] = pMenuItem->getPosition();
	CCNode* pWords = (CCNode*)pNode->getChildByTag(i);
	if (pWords)
	{
	ptWords[i] = pWords->getPosition();
	}
	}
	}
	int vIdx = 0;
	int vIdxWords = 0;
	for (int i = 0; i < kSettingCount; i++)
	{
	CCNode* pMenuItem = (CCNode*)m_pMenu->getChildByTag(i);
	if (pMenuItem == NULL)
	{
	continue;
	}

	if (pMenuItem->isVisible())
	{
	pMenuItem->setPosition(pt[vIdx++]);
	}
	CCNode* pWords = (CCNode*)pNode->getChildByTag(i);
	if (pWords && pWords->isVisible())
	{
	pWords->setPosition(ptWords[vIdxWords++]);
	}

	}*/
    m_pCdkeyButton->setVisible(DM_GET_SETTING_MODEL->getEnableCDKey());
    m_pCdKeyText->setVisible(DM_GET_SETTING_MODEL->getEnableCDKey());
    
    m_pAccountButton->setVisible(!PlatformManager::sharedPlatformManager()->isInternal());
    m_pAccountText->setVisible(!PlatformManager::sharedPlatformManager()->isInternal());

	for(unsigned int i = kGuide; i < kSettingCount; i++)
	{
		if (m_titleMap[(SettingNav)i])
		{
			m_titleMap[(SettingNav)i]->setVisible(false);
		}
	}

	if (m_pCloseButton)
	{
		m_pCloseButton->getButton()->setTarget(this, menu_selector(Setting::closeCallBack));
	}

	 NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(Setting::closeCallBack),D_EVENT_CLOSE_SETTING);
	NavTo(kSettingSound);

    return true;
}

bool Setting::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenu", CCMenu *, this->m_pMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCdkeyButton", CCMenuItemImage*, this->m_pCdkeyButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCdKeyText", CCSprite*, this->m_pCdKeyText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAccountButton", CCMenuItemImage*, this->m_pAccountButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAccountText", CCSprite*, this->m_pAccountText);
	
	for (unsigned int i = kGuide; i < kSettingCount; i ++)
	{
		 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, CCString::createWithFormat("m_pTitle%d", i)->getCString(), CCSprite *, this->m_titleMap[(SettingNav)i]);
	}
   
	return false;
}

SEL_MenuHandler Setting::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", Setting::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "navCallBack", Setting::navCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", Setting::viewCallBack);
    return NULL;
}

void Setting::viewNav(SettingNav nav)
{
    switch (nav)
    {
        case kUserCenter:
            //如果是游客登入显示绑定用户
            if(PlatformManager::sharedPlatformManager()->isGuess())
                PlatformManager::sharedPlatformManager()->showBindAccount();
            else
                PlatformManager::sharedPlatformManager()->showUserCenter();
            break;
        case kFourm:
            PlatformManager::sharedPlatformManager()->showFourm();
            break;
        default:
            break;
    }
}

void Setting::NavTo(SettingNav nav)
{
	if(nav == kAnnounce)
	{
		//Game::sharedGame()->openPopup(Announce::create());
		Game::sharedGame()->openPopup(GonggaolanView::create());
	    closePopup();
		return;
	}
    
    if (nav == kUserCenter) {
        PlatformManager::sharedPlatformManager()->showUserCenter();
        return;
    }
    
    CCMenuItem* pMenuItem = (CCMenuItem*)m_pMenu->getChildByTag(nav);
	if (pMenuItem)
	{
		CCNode* pNode = getView(nav);
		CCLog("w:%f, h%f", pNode->getContentSize().width, pNode->getContentSize().height);
		if (pNode)
		{
			m_pContainer->removeAllChildren();
			m_pContainer->addChild(pNode);
			pMenuItem->setEnabled(false);
			pMenuItem->selected();

			if(m_pSelected) m_pSelected->setEnabled(true);
			m_pSelected = pMenuItem;

			if (m_pCurrentTitle) m_pCurrentTitle->setVisible(false);
			if (m_titleMap[nav] != NULL)
			{
				m_pCurrentTitle = m_titleMap[nav];
				m_pCurrentTitle->setVisible(true);
			}
		}
	}
}

CCNode* Setting::getView(SettingNav nav)
{
    if(m_pNavs->objectForKey(nav)) return (CCNode*)m_pNavs->objectForKey(nav);
    CCNode* pNode = NULL;
    switch (nav)
    {
        case kGuide:
            pNode = Guide::create();
            break;
        case kUserCenter:
            //pNode = Raiders::create();
            break;
        case kFourm:
            //pNode = Raiders::create();
            break;
        case kCustomerService:
            pNode = CustomerService::create();
            break;
        case kUserFeedback:
            pNode = UserFeedback::create();
            break;
        case kSettingSound:
            pNode = SoundSetting::create();
            break;
        default:
            break;
    }
    m_pNavs->setObject(pNode, nav);
    return pNode;
}

void Setting::viewCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* pItem = (CCMenuItem*)obj;
    int tag = pItem->getTag();
    if(tag!=-1)viewNav((SettingNav)tag);
    else CCLOG("error nav");
}

void Setting::navCallBack(cocos2d::CCObject* obj)
{
    CCMenuItemImage* pItem = (CCMenuItemImage*)obj;
    int tag = pItem->getTag();
    if(tag!=-1)NavTo((SettingNav)tag);
    else CCLOG("error nav");
}

void Setting::closeCallBack(CCObject* obj)
{
   closePopup();
}

void Setting::layout()
{
	
}
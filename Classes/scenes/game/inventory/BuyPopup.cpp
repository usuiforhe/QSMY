//
//  BuyPopup.h
//  用户升级弹出框
//
//  Created by fengkerong on 14-07-4.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "BuyPopup.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

BuyPopup::BuyPopup()
	:m_award1Num(NULL)
	,m_award2Num(NULL)
	,m_maxPower(NULL)
	,m_unlockWarriorNum(NULL)
	,m_pCloseButton(NULL)
	,m_pEffectNode(NULL)
	,m_pNavButton(NULL)
	,m_pLightEffectNode(NULL)
	,m_pLevel(NULL)

	,m_pConfirmButton(NULL)
	,m_pCancelButton(NULL)
	,m_pJiahao(NULL)
	,m_pJiajiahao(NULL)
	,m_pJianhao(NULL)
	,m_pItemName(NULL)
	,m_pItemInfo(NULL)
	,m_pCount(NULL)
	,m_pTotalPrice(NULL)
	,m_pContainer(NULL)
	,m_pProductInfo(NULL)
{

}

BuyPopup::~BuyPopup()
{
	CC_SAFE_RELEASE(m_award1Num);               //奖励元宝数	-- 对应用户iPoints
	CC_SAFE_RELEASE(m_award2Num);               //奖励银币数	-- 对应用户iCoins
	CC_SAFE_RELEASE(m_maxPower);				//最大体力
	CC_SAFE_RELEASE(m_unlockWarriorNum);		//最大上阵弟子
	CC_SAFE_RELEASE(m_pNavButton);              //确定按钮
	CC_SAFE_RELEASE(m_pEffectNode);             //效果
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pLightEffectNode);        //光效果
	CC_SAFE_RELEASE(m_pLevel);

	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pCancelButton);
	CC_SAFE_RELEASE(m_pJiahao);
	CC_SAFE_RELEASE(m_pJiajiahao);
	CC_SAFE_RELEASE(m_pJianhao);
	CC_SAFE_RELEASE(m_pItemName);
	CC_SAFE_RELEASE(m_pItemInfo);
	CC_SAFE_RELEASE(m_pCount);
	CC_SAFE_RELEASE(m_pTotalPrice);
	CC_SAFE_RELEASE(m_pContainer);
}

bool BuyPopup::init()
{
	if (!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_goumai");

	m_pCloseButton->getButton()->setTarget(this, menu_selector(BuyPopup::closeCallBack));
	m_pCancelButton->getButton()->setTarget(this, menu_selector(BuyPopup::closeCallBack));
	m_pJianhao->getButton()->setTarget(this, menu_selector(BuyPopup::changeCallBackReduce));
	m_pJiahao->getButton()->setTarget(this, menu_selector(BuyPopup::changeCallBackAdd));
	m_pJiajiahao->getButton()->setTarget(this, menu_selector(BuyPopup::changeCallBackAdd10));
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(BuyPopup::doBuy));
	

	return true;
}

bool BuyPopup::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_award1Num",			CCLabelBMFont *,			m_award1Num);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_award2Num",			CCLabelBMFont *,			m_award2Num);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_maxPower",				CCLabelBMFont *,			m_maxPower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_unlockWarriorNum",		CCLabelBMFont *,		m_unlockWarriorNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel",               CCLabelBMFont *,		m_pLevel);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton",			DButton*,                m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNavButton",			CCNode*,                m_pNavButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode",			CCNode *,                   m_pEffectNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLightEffectNode",		CCNode *,           m_pLightEffectNode);


	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton",			DButton *,			m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCancelButton",			DButton *,			m_pCancelButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pJiahao",					DButton *,			m_pJiahao);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pJiajiahao",				DButton *,			m_pJiajiahao);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pJianhao",					DButton *,			m_pJianhao);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName",				CCLabelTTF*,        m_pItemName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemInfo",				CCLabelTTF*,        m_pItemInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount",					CCLabelBMFont *,    m_pCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTotalPrice",				CCLabelBMFont *,    m_pTotalPrice);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",				CCNode *,           m_pContainer);

	return false;
}

SEL_MenuHandler BuyPopup::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void BuyPopup::onEnter()
{
	
	updateInfo();
	//CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, DM_GET_CONFIG_MODEL->getConfigByKey(LEVEL_UP_SOUND_KEY));
	DPopup::onEnter();
}

void BuyPopup::updateInfo()
{

	MallModel::ProductInfoVec   m_products;
	DM_GET_MALL_MODEL->getProductsInMallByType(mall::kProductTypeItem,m_products);

	uint32_t pID = DM_GET_MALL_MODEL->getUnenoughItemID();
	if (pID)
	{
		int idx = 0;
		mall::ProductInfo *pInfo = NULL;
		for (int i = 0 ; i < m_products.count(); i++)
		{
			//pInfo = (mall::ProductInfo *)m_products.objectAtIndex(i);
			if (((mall::ProductInfo *)m_products.objectAtIndex(i))->getBaseInfo()->getItemId() == pID)
			{
				pInfo = (mall::ProductInfo *)m_products.objectAtIndex(i);
				idx = i;
				//CCLog("found-------------------------------------");
			}
		}
		if(pInfo){
			m_pContainer->removeAllChildren();
			m_pContainer->addChild(ResourceManager::sharedResourceManager()->getProductIconForList(pInfo->getPId(),pInfo->getBaseInfo()->getType()));

			m_pItemName->setString(pInfo->getBaseInfo()->getProductName().c_str());
			m_pItemInfo->setString(D_LOCAL_STRING("FastPay%s" , pInfo->getBaseInfo()->getProductName().c_str()).c_str());

			int count = atoi(m_pCount->getString());
			m_pTotalPrice->setString(CCString::createWithFormat("%d" , count * pInfo->getPrice())->getCString());

			m_pProductInfo = (mall::ProductInfo *)m_products.objectAtIndex(idx);
			//break;
		}
		

	}else
	{
		//m_pListView->updateContentOffset();
	}

	DM_GET_MALL_MODEL->setUnenoughItemID(0);
}
void BuyPopup::doBuy(CCObject * obj)
{
	int totalPrice = atoi(m_pTotalPrice->getString());
	int num = atoi(m_pCount->getString());
	mall::ProductInfo* pProductInfo = m_pProductInfo;
	if(pProductInfo)
	{
		if(!pProductInfo->isCanBuy())
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
			closePopup();
			return;
		}

		if( totalPrice > DM_GET_USER_MODEL->getUserInfo()->getPoints())
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
			closePopup();
			return;
		}
		HTTP_CLIENT->buyProduct(pProductInfo->getPId(),num,this,callfuncJson_selector(BuyPopup::buyCallback));
	}
}
void BuyPopup::buyCallback(const Json::Value & requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	Tips_Alert(D_LOCAL_STRING("$mall_purchase"));
	//购买后的掉落事件，应该已经发过update消息了
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_BUYITEM_SUCCESS);
	this->closePopup();
}
void BuyPopup::changeCallBackReduce(cocos2d::CCObject * obj)
{
	addNum(-1);
}
void BuyPopup::changeCallBackAdd(cocos2d::CCObject * obj)
{
	addNum(1);
}
void BuyPopup::changeCallBackAdd10(cocos2d::CCObject * obj)
{
	addNum(10);
}
void BuyPopup::addNum(int num)
{
	int addNum = num;
	int count = atoi(m_pCount->getString());
	count += addNum;
	if(count < 1)return;
	m_pCount->setString(CCString::createWithFormat("%d",count)->getCString());
	m_pTotalPrice->setString(CCString::createWithFormat("%d" , count * m_pProductInfo->getPrice())->getCString());
}
void BuyPopup::closePopup()
{
	DPopup::closePopup();
	//NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CLOSE_LEVELUP_POPUP);
}

void BuyPopup::onBackKey()
{
	closePopup();
}
//
//  VipView.cpp
//  VIP列表
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "VipView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "Account.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define VIPBUTTON_HEIGHT 85


/************************ vip select class begin*******************/

VipView::VipView()
:m_selectIndex(-1)
,m_pVipInfos(NULL)
,m_pContainer(NULL)
,m_pDelegate(NULL)
,m_pTitle(NULL)
,m_pContent(NULL)
,m_pBuy(NULL)
,m_pCanBuy(NULL)
,m_pHave(NULL)
,m_pPriceNow(NULL)
,m_pPrice(NULL)
,m_pRechargeButton(NULL)
,m_pCloseButton(NULL)
,m_pVipNow(NULL)
,m_pVipSelected(NULL)
,m_pExpInfoNode(NULL)
,m_pExpInfo(NULL)
,m_pExpCoin(NULL)
,m_pExpLevel(NULL)
,m_pExp(NULL)
,m_pProgress(NULL)
,m_pScrollView(NULL)
,m_pNoTouchLayer(NULL)
,m_pFixNode(NULL)
,m_pScrollNode(NULL)
,m_pButtonsNode(NULL)
,m_firstEnter(true)
{
	D_RESET_C_ARRAY(m_pVipAll, VIP_BUTTON_COUNT);
	D_RESET_C_ARRAY(m_pVip, VIP_LV_FIX + 1);
}


VipView::~VipView()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTitle);
    CC_SAFE_RELEASE(m_pContent);
    CC_SAFE_RELEASE(m_pBuy);
    CC_SAFE_RELEASE(m_pCanBuy);
    CC_SAFE_RELEASE(m_pHave);
	D_SAFE_RELEASE_C_ARRAY(m_pVipAll, VIP_BUTTON_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pVip, VIP_LV_FIX + 1);
    CC_SAFE_RELEASE(m_pUserInfo);
    CC_SAFE_RELEASE(m_pVipInfos);
    CC_SAFE_RELEASE(m_pPriceNow);
    CC_SAFE_RELEASE(m_pPrice);
	CC_SAFE_RELEASE(m_pRechargeButton);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pVipNow);
	CC_SAFE_RELEASE(m_pVipSelected);
	CC_SAFE_RELEASE(m_pExpInfoNode);
	CC_SAFE_RELEASE(m_pExpInfo);
	CC_SAFE_RELEASE(m_pExpCoin);
	CC_SAFE_RELEASE(m_pExpLevel);
	CC_SAFE_RELEASE(m_pExp);
	CC_SAFE_RELEASE(m_pProgress);
	CC_SAFE_RELEASE(m_pScrollView);
	CC_SAFE_RELEASE(m_pNoTouchLayer);
	CC_SAFE_RELEASE(m_pFixNode);
	CC_SAFE_RELEASE(m_pScrollNode);
	CC_SAFE_RELEASE(m_pButtonsNode);
}


bool VipView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pUserInfo = DM_GET_USER_MODEL->getUserInfo();
    m_pVipInfos = DM_GET_USER_MODEL->getVipInfos();
    
    CC_SAFE_RETAIN(m_pUserInfo);
    CC_SAFE_RETAIN(m_pVipInfos);
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/account/VipView");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/vip/pl_vip");

	m_pBuy->getButton()->setTarget(this, menu_selector(VipView::buyVipBag));
	m_pRechargeButton->getButton()->setTarget(this, menu_selector(VipView::backCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(VipView::closeCallBack));

	CCSize orginalsize = m_pScrollView->getViewSize();
	CCSize newsize = PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(orginalsize, -PL_MAX_HEIGHT_DIFF);
	m_pScrollView->setViewSize(newsize);
    
	return true;
    
}

bool VipView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitle", CCLabelTTF *, this->m_pTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContent", CCLabelTTF *, this->m_pContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBuy", DButton *, this->m_pBuy);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRechargeButton", DButton *, this->m_pRechargeButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanBuy", CCNode *, this->m_pCanBuy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHave", CCNode *, this->m_pHave);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont *, this->m_pPrice);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceNow", CCLabelBMFont *, this->m_pPriceNow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipNow", CCLabelBMFont *, this->m_pVipNow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipSelected", CCLabelBMFont *, this->m_pVipSelected);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpInfoNode", CCNode *, this->m_pExpInfoNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpInfo", CCLabelTTF *, this->m_pExpInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpCoin", CCLabelTTF *, this->m_pExpCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpLevel", CCLabelTTF *, this->m_pExpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelTTF *, this->m_pExp);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgress", DProgressBar *, this->m_pProgress);	

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pScrollView", CCScrollView *, this->m_pScrollView);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoTouchLayer", CCLayer *, this->m_pNoTouchLayer);	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFixNode", CCNode *, this->m_pFixNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pScrollNode", CCNode *, this->m_pScrollNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonsNode", CCNode *, this->m_pButtonsNode);
	
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pVipAll",CCMenuItemImage*,this->m_pVipAll,VIP_BUTTON_COUNT);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pVip",CCMenuItemImage*,this->m_pVip,VIP_LV_FIX + 1);
	
    return false;
}


SEL_MenuHandler VipView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", VipView::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "backCallBack", VipView::backCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", VipView::viewCallBack);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "levelCallBack", VipView::onVipSelectCallBack);
    return NULL;
}

void VipView::onEnter()
{
    DLayer::onEnter();
    updateView();
}

void VipView::updateView()
{

	if(m_pUserInfo->getVipLevel()>VIP_LV_FIX){
		//scroll
		m_pFixNode->setVisible(false);
		m_pScrollNode->setVisible(true);

		for (uint8_t i(0); i<VIP_BUTTON_COUNT; ++i)
		{
			if(i+1==m_pUserInfo->getVipLevel()){
				m_pVipAll[i]->selected();
			}
			else if( i > m_pUserInfo->getVipLevel() ){
				m_pVipAll[i]->getParent()->getParent()->setVisible(false);
			}
			else{
				m_pVipAll[i]->unselected();
			}
		}


		int offset = VIP_BUTTON_COUNT - m_pUserInfo->getVipLevel() - 1;
		if(offset < 0){
			offset=0;
		}
		//m_pScrollView->setContentOffset(ccp(0,- VIPBUTTON_HEIGHT * offset));

		if(m_firstEnter){
			//do it only once
			m_firstEnter = false;
			CCSize contentSize = m_pScrollView->getContentSize();//>getContentSize();
			m_pScrollView->setContentSize(CCSize(contentSize.width , contentSize.height - VIPBUTTON_HEIGHT * offset));
			float y = m_pButtonsNode->getPositionY();
			m_pButtonsNode->setPositionY(y - VIPBUTTON_HEIGHT * offset);
		}
	}
	else
	{
		//fix
		m_pFixNode->setVisible(true);
		m_pScrollNode->setVisible(false);

		for (uint8_t i(0); i<VIP_LV_FIX + 1; ++i)
		{
			if(i+1==m_pUserInfo->getVipLevel()){
				m_pVip[i]->selected();
			}
			else{
				m_pVip[i]->unselected();
			}
		}
	}

	m_selectIndex = m_pUserInfo->getVipLevel();
	if(m_selectIndex==0) m_selectIndex =1;
	updateInfo();
}

void VipView::updateInfo()
{
	uint8_t userVipLevel = m_pUserInfo->getVipLevel();
	
	m_pVipNow->setString(CCString::createWithFormat("%d",userVipLevel)->getCString());
	m_pVipSelected->setString(CCString::createWithFormat("%d",m_selectIndex)->getCString());

    user::VipInfo* pVip = getSelVipInfo();
    if(pVip==NULL) return;

	//title
	//uint8_t nextVipLv = (m_pUserInfo->getVipLevel()==0)?m_pUserInfo->getVipLevel()+2:m_pUserInfo->getVipLevel()+1;
	uint8_t nextVipLv = m_pUserInfo->getVipLevel()+1;//對0，1級不再特殊處理
	m_pTitle->setString(D_LOCAL_STRING("Vip%dTitle%d",pVip->getPoint(),pVip->getVipLv()  ).c_str());	
	
	//exp
	if(m_pUserInfo->getVipLevel() < DM_GET_USER_MODEL->getMaxVipLevel())
	{
		int pointNeed = DM_GET_USER_MODEL->getToVipLevelCost(nextVipLv);
		m_pExpCoin->setString(CCString::createWithFormat("%d" , pointNeed)->getCString());
		m_pExpLevel->setString(CCString::createWithFormat("%d" , nextVipLv)->getCString());

		user::VipInfo* nextVipInfo = static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(nextVipLv-1));
		user::VipInfo* nowVipInfo = NULL;
		if(m_pUserInfo->getVipLevel() != 0)
		{
			nowVipInfo = static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(nextVipLv-2));
		}

		//顯示本機別以上的進度
		int pointDetla = (m_pUserInfo->getVipLevel()==0) ? nextVipInfo->getPoint() : nextVipInfo->getPoint() - nowVipInfo->getPoint();//兩級相差多少元寶
		int pointHadDetla = (m_pUserInfo->getVipLevel()==0) ? m_pUserInfo->getAmount() : m_pUserInfo->getAmount() - nowVipInfo->getPoint();//在這個級別上沖了多少元寶
		if(pointHadDetla < 0) pointHadDetla = 0;
		m_pExp->setString(CCString::createWithFormat("%d/%d" , pointHadDetla , pointDetla)->getCString());	
		m_pProgress->setPercent(0.0f);
		m_pProgress->setPercent((float)pointHadDetla/pointDetla,0.5);
	}
	else{
		//max vip
		m_pExpInfoNode->setVisible(false);
		m_pExpInfo->setVisible(true);
		m_pExpInfo->setString(D_LOCAL_STRING("VipMaxTip" ).c_str());
	}
    m_pContent->setString(pVip->getVipDesc().c_str());   
    
    
	/*****************
	禮包購買
	*****************/

    m_pHave->setVisible(m_pUserInfo->hasBoughtVipBag(m_selectIndex));
    m_pCanBuy->setVisible(!m_pHave->isVisible());
    m_pBuy->getButton()->setEnabled(m_pCanBuy->isVisible());// && pProduct->isCanBuy());
	//m_pBuy->getButton()->setEnabled(true);
    m_pContainer->removeAllChildren();

	mall::ProductInfo* pProduct = DM_GET_MALL_MODEL->getProductById(pVip->getVipBoxId());
    m_pPriceNow->setString(CCString::createWithFormat("%d",pProduct->getPrice())->getCString());
    m_pPrice->setString(CCString::createWithFormat("%d",pProduct->getBaseInfo()->getOldPrice())->getCString());
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon30P(pProduct->getBaseInfo()->getItemId()));
}


void VipView::onVipSelectCallBack(cocos2d::CCObject * obj)
{
    CCMenuItem* item = static_cast<CCMenuItem*>(obj);
    m_selectIndex = item->getTag();

	if(m_pUserInfo->getVipLevel()>VIP_LV_FIX){
		for (uint8_t i(0); i<VIP_BUTTON_COUNT; ++i)
		{
			if(i+1==item->getTag()){
				m_pVipAll[i]->selected();
			}
			else{
				m_pVipAll[i]->unselected();
			}
		}
	}
	else{
		for (uint8_t i(0); i<VIP_LV_FIX +1; ++i)
		{
			if(i+1==item->getTag()){
				m_pVip[i]->selected();
			}
			else{
				m_pVip[i]->unselected();
			}

		}
	}

    updateInfo();
}

void VipView::buyVipBag(cocos2d::CCObject* obj)
{
    user::VipInfo* pVip = getSelVipInfo();
    if(pVip==NULL) return;
	/************************************************************************/
	/* 
	这里将1(00000001)左移level-1位 再与vip_bag与预算，取得vip_bag中第level位的值
	(vip_bag&(1<<(vipLevel-1)))>0;
	*/
	/************************************************************************/
    if(m_pUserInfo->hasBoughtVipBag(pVip->getVipLv())) return;

    mall::ProductInfo* pProductInfo = static_cast<mall::ProductInfo*>(DM_GET_MALL_MODEL->getProductById(pVip->getVipBoxId()));
    
    if(!pProductInfo || !pProductInfo->isCanBuy())
    {
        Tips_Error(D_LOCAL_STRING("VIPLVNotEnough").c_str());
        return;
    }
    HTTP_CLIENT->buyProduct(pProductInfo->getPId(),1,this,callfuncJson_selector(VipView::buyCallback));
}

void VipView::buyCallback(const Json::Value & requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	Tips_Alert(D_LOCAL_STRING("$mall_purchase"));
    updateInfo();
}

user::VipInfo* VipView::getSelVipInfo()
{
    return static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(m_selectIndex-1));
}

void VipView::viewCallBack(cocos2d::CCObject * obj)
{
    user::VipInfo* pVip = getSelVipInfo();
    mall::ProductBase* pProduct = DM_GET_MALL_MODEL->getProductBaseByID(pVip->getVipBoxId());
    
    if (pProduct->getType() == mall::kProductTypeItem || pProduct->getType() == mall::kProductTypeVipPk)
    {
        m_pDelegate->showInfoView(kItemInfo, item::ItemInfo::create(pProduct->getItemId(), 0));
    }
}

void VipView::backCallBack(cocos2d::CCObject * obj)
{
	if(m_pDelegate) m_pDelegate->showInfo(kAccountTypeRecharge);
	else removeFromParent();
}

void VipView::closeCallBack(cocos2d::CCObject * obj)
{
	if(m_pDelegate) m_pDelegate->closeInfo();
	else removeFromParent();
}

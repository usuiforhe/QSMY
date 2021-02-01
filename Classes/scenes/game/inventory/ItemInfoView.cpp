//  ItemInfoView.cpp
//  道具描述
//  Created by cll on 13-4-15
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "ItemInfoView.h"
#include "Inventory.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;

ItemInfoView::ItemInfoView()
:m_pDelegate(NULL)
,m_pveDelegate(NULL)
,m_pItemModel(NULL)
,m_pContainer(NULL)
,m_pUseMenuItem(NULL)
,m_pDesc(NULL)
,m_pName(NULL)
,m_pReturn(NULL)
,m_pUse(NULL)
,m_pItemInfo(NULL)
,m_pItemInfoView(NULL)
,m_pVipPkgInfoView(NULL)
,m_bUseAble(false)
,m_pAmount(NULL)
,m_pAmountNode(NULL)
,m_pCloseButton(NULL)
,m_pGrade(NULL)
,m_pCountFontNode(NULL)
,m_pDressFontNode(NULL)
{
    
}


ItemInfoView::~ItemInfoView()
{
    
    CC_SAFE_RELEASE(m_pUseMenuItem);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pReturn);
    CC_SAFE_RELEASE(m_pUse);
    CC_SAFE_RELEASE(m_pAmount);
    CC_SAFE_RELEASE(m_pAmountNode);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pCountFontNode);
	CC_SAFE_RELEASE(m_pDressFontNode);
    SAFE_RELEASE_UNBINDING_ALL(m_pItemInfo, this);
}


bool ItemInfoView::init()
{

	if(!DLayer::init())
	{
		return false;
	}

	m_pItemModel = DM_GET_ITEM_MODEL;
    
    //CCB_READER("ui/ccb/inventory/ItemInfoView", m_pItemInfoView);
	CCB_READER("PL_ui/ccb/pl_daojuxinxi/PL_daojuxinxi", m_pItemInfoView);
    addChild(m_pItemInfoView);
	
	//Vertical Text
	//m_pDesc->setVerticalTextStyle(kCCVerticalTextStyleRightToLeft);
    //Close Button
	m_pCloseButton->getButton()->setTarget(this, menu_selector(ItemInfoView::closeCallBack));

    m_pVipPkgInfoView = PkgInfoView::create();
    addChild(m_pVipPkgInfoView);
    
	return true;
}


bool ItemInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUseMenuItem", DButton *, this->m_pUseMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReturn", CCSprite *, this->m_pReturn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUse", CCSprite *, this->m_pUse);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont *, this->m_pAmount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmountNode", CCNode *, this->m_pAmountNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", WarriorGradeBg *, this->m_pGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountFontNode", CCNode *, this->m_pCountFontNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDressFontNode", CCNode *, this->m_pDressFontNode);
    return false;
}

SEL_MenuHandler ItemInfoView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", ItemInfoView::closeCallBack);
    return NULL;
}

void ItemInfoView::binding()
{
    BINDING_ONLY(m_pItemInfo, this, ItemInfoView::updateView, D_EVENT_ITEMINFO_UPDATE);
}

void ItemInfoView::onEnter()
{
    binding();
    DLayer::onEnter();
}

void ItemInfoView::onExit()
{
    UNBINDING_ALL(m_pItemInfo, this);
    DLayer::onExit();
}

void ItemInfoView::updateView()
{
    m_pContainer->removeAllChildren();
    
    if(m_pItemInfo==NULL) return;
    if(m_bUseAble)
    {
        if(m_pItemModel->getItemNum(m_pItemInfo->getItemId())==0)
        {
            closeCallBack(NULL);
        }
    }
    
    if (m_pItemInfo->getBaseInfo()->getItemType() == item::kItemTypeVipPk)
    {
        
        m_pItemInfoView->setVisible(false);
        m_pVipPkgInfoView->setVisible(true);
        
        m_pVipPkgInfoView->setItemInfo(m_pItemInfo->getBaseInfo());
        m_pVipPkgInfoView->getCloseButton()->getButton()->setTarget(this, menu_selector(ItemInfoView::closeCallBack));
		m_pVipPkgInfoView->getConfirmButton()->getButton()->setTarget(this, menu_selector(ItemInfoView::closeCallBack));
        /*if (m_bUseAble)
        {
            m_pVipPkgInfoView->getButton()->getButton()->setTarget(this, menu_selector(ItemInfoView::useCallback));
            m_pVipPkgInfoView->getUse()->setVisible(true);
            m_pVipPkgInfoView->getOK()->setVisible(false);
            m_pVipPkgInfoView->getAmount()->setVisible(true);
            m_pVipPkgInfoView->getAmount()->setString(CCString::createWithFormat("X%d",m_pItemInfo->getNum())->getCString());
        }else
        {
            m_pVipPkgInfoView->getAmount()->setVisible(false);
            m_pVipPkgInfoView->getButton()->getButton()->setTarget(this, menu_selector(ItemInfoView::closeCallBack));
            m_pVipPkgInfoView->getUse()->setVisible(false);
            m_pVipPkgInfoView->getOK()->setVisible(true);
        }*/   
        return;
    }
    
    m_pItemInfoView->setVisible(true);
    m_pVipPkgInfoView->setVisible(false);

	if(m_pItemInfo->getBaseInfo()->getItemType() == item::kItemTypeFragment)
	{
		//处理碎片显示
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon(m_pItemInfo->getBaseInfo()->getResID()));
	}
	else{
		//默认显示
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon(m_pItemInfo->getItemId()));
	}

    m_pName->setString(m_pItemInfo->getBaseInfo()->getItemName().c_str());
    m_pDesc->setString(m_pItemInfo->getBaseInfo()->getItemDesc().c_str());
    if(m_pItemInfo->getNum()>0)
    {
        m_pAmount->setString(CCString::createWithFormat("X%d",m_pItemInfo->getNum())->getCString());
        m_pAmountNode->setVisible(true);
    }else
    {
        m_pAmountNode->setVisible(false);//自己有的物品不可能是0个
    }
    
	if (m_bUseAble)
	{
        m_pUse->setVisible(true);
        m_pReturn->setVisible(false);
        m_pUseMenuItem->getButton()->setTarget(this, menu_selector(ItemInfoView::useCallback));
        
	}else
    {
        m_pUse->setVisible(false);
        m_pReturn->setVisible(true);
        m_pUseMenuItem->getButton()->setTarget(this, menu_selector(ItemInfoView::closeCallBack));
    }

	int iGrade = (int)m_pItemInfo->getBaseInfo()->getItemGrade();
	warrior::WarriorGrade wGrade = (warrior::WarriorGrade)iGrade;
	m_pGrade->setWarriorGrade(wGrade);

}

void ItemInfoView::setAvatarInfo(avatar::AvatarBaseInfo* pInfo)
{
	m_pItemInfoView->setVisible(true);
	m_pVipPkgInfoView->setVisible(false);
	m_pCountFontNode->setVisible(false);
	m_pDressFontNode->setVisible(true);

	m_bUseAble = false;
	m_pUse->setVisible(false);
	m_pReturn->setVisible(true);
	m_pUseMenuItem->getButton()->setTarget(this, menu_selector(ItemInfoView::closeCallBack));

	m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pInfo->getResId()));
    m_pName->setString(pInfo->getName().c_str());
    m_pDesc->setString(pInfo->getDesc().c_str());
}

void ItemInfoView::setItemInfo(item::ItemInfo* pItemInfo)
{
    CC_SAFE_RETAIN(pItemInfo);
    CC_SAFE_RELEASE(m_pItemInfo);
    m_pItemInfo = pItemInfo;
    binding();
    m_bUseAble = pItemInfo!=NULL && DM_GET_ITEM_MODEL->isMyItem(pItemInfo) && pItemInfo->getBaseInfo()->getCanBeUsed() && m_pItemModel->getItemNum(pItemInfo->getItemId())>0;
    updateView();

	m_pCountFontNode->setVisible(true);
	m_pDressFontNode->setVisible(false);
}

void ItemInfoView::setItemInfo(item::ItemBase * pItemInfo)
{
    setItemInfo(item::ItemInfo::create(pItemInfo));
}

void ItemInfoView::closeCallBack(CCObject*obj)
{
    if(m_pDelegate) 
	{
		m_pDelegate->closeInfoView(kItemInfo);
		return;
	}
	if(m_pveDelegate)
	{
		m_pveDelegate->closePopup();
		return;
	}
	removeFromParent();
}

void ItemInfoView::useCallback(CCObject * obj)
{
	if(m_bUseAble)
	{
		if (m_pDelegate)
		{
			m_pDelegate->useItem(m_pItemInfo);
		}
	}
}

//MARK:  PkgInfoView

PkgInfoView::PkgInfoView()
:m_pName(NULL)
,m_pButton(NULL)
,m_pCloseButton(NULL)
,m_pConfirmButton(NULL)
,m_pUse(NULL)
,m_pOK(NULL)
,m_pAmount(NULL)
,m_pTitleCommon(NULL)
,m_pTitlePackage(NULL)
{
    
}

PkgInfoView::~PkgInfoView()
{
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pButton);
    CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pConfirmButton);
    CC_SAFE_RELEASE(m_pUse);
    CC_SAFE_RELEASE(m_pOK);
    CC_SAFE_RELEASE(m_pAmount);
	CC_SAFE_RELEASE(m_pTitleCommon);
	CC_SAFE_RELEASE(m_pTitlePackage);
}

void PkgInfoView::setItemInfo(item::ItemBase * pItemInfo)
{
    CCArray* pList =  DM_GET_ITEM_MODEL->getVipPkgInfo(pItemInfo->getItemId());
    CC_SAFE_RETAIN(pList);
    CC_SAFE_RELEASE(m_pAwardList);
    m_pAwardList = pList;
    updateAwardListView();
    //m_pName->setString(pItemInfo->getItemName().c_str());
}
    
bool PkgInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", cocos2d::CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUse", cocos2d::CCNode *, this->m_pUse);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOK", cocos2d::CCNode *, this->m_pOK);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);	
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont *, this->m_pAmount);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleCommon", CCNode *, this->m_pTitleCommon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitlePackage", CCNode *, this->m_pTitlePackage);

    return BaseAwardListView::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

void PkgInfoView::addChildrenBeforeList()
{
    //CCB_READER_AND_ADDCHILD("ui/ccb/inventory/giftView");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_tankuang_shiliankai");

	m_pTitleCommon->setVisible(false);
	m_pTitlePackage->setVisible(true);
	
}


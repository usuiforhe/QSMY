//
//  MallView.cpp
//  商城列表
//
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MallView.h"
#include "Mall.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  Mall_CELL_SIZE		CCSizeMake(640, 175)



/************************ Mall view class begin*******************/

MallView::MallView()
:m_pListView(NULL)
,m_pMallModel(NULL)
,m_pUserInfo(NULL)
,m_pDelegate(NULL)
,m_pData(NULL)
,m_bNoFly(false)
{
    
}


MallView::~MallView()
{
    CC_SAFE_RELEASE(m_pData);
}


MallView* MallView::create(mall::ProductType pType,const cocos2d::CCSize& size)
{
	MallView *pRet = new MallView();
	if (pRet && pRet->init(pType,size))
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

bool MallView::init(mall::ProductType pType,const cocos2d::CCSize& size)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pType = pType;
    
	m_pMallModel    = DM_GET_MALL_MODEL;
    
    MallModel::ProductInfoVec products;
    m_pMallModel->getProductsInMallByType(m_pType,products);
    if(products.count()==0) return false;
    
	m_pUserInfo = DM_GET_USER_MODEL->getUserInfo();
    
	m_pListView = DTableView::create(this, size);
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pListView->setPositionX(10);
	this->addChild(m_pListView);
    
    //m_pData = readCCData("ui/ccb/mall/mallCell");
	m_pData = readCCData("PL_ui/ccb/pl_shop/PL_shop_cell");
    CC_SAFE_RETAIN(m_pData);
    
    
	return true;
}

void MallView::onEnter()
{
	BINDING_ONLY(m_pMallModel,this,MallView::updateMallView,D_EVENT_MALLLIST_UPDATE);
    
	if (m_pType == mall::kProductTypeVipPk)
	{
		BINDING_ONLY(m_pUserInfo,this,MallView::updateMallView,D_EVENT_USERINFO_UPDATE);
	}
	updateMallView();
    DLayer::onEnter();
}

void MallView::onExit()
{
	UNBINDING_ALL(m_pMallModel,this);
	
	if (m_pType == mall::kProductTypeVipPk)
	{
		UNBINDING_ALL(m_pUserInfo,this);
	}
    
	DLayer::onExit();
}



void MallView::updateMallView()
{
	//if(!m_pMallModel->isSynced) return;
    
	m_pMallModel->getProductsInMallByType(m_pType,m_products);
    
	if(m_bNoFly == true){
		m_bNoFly = false;
		m_pListView->reloadData();
	}
	else{
		m_pListView->reloadData();
	}	
    
	uint32_t pID = DM_GET_MALL_MODEL->getUnenoughItemID();
	if (pID)
	{
		int idx = 0;
		for (int i = 0; i < m_products.count(); i++)
		{
			mall::ProductInfo *pInfo = (mall::ProductInfo *)m_products.objectAtIndex(i);
			if (pInfo->getBaseInfo()->getItemId() == pID)
			{
				idx = i;
				break;
			}
		}
		if (idx)
		{
			int height = Mall_CELL_SIZE.height * (idx + 3);
			height -= m_pListView->getContentSize().height;
			m_pListView->setContentOffset(ccp(0, height));
			m_pListView->updateContentOffset();
		}
		
	}else
    {
        m_pListView->updateContentOffset();
    }
    
	DM_GET_MALL_MODEL->setUnenoughItemID(0);
}


void MallView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
	CCLOG("Mall cell touched at index: %i", cell->getIdx());
    
}

CCSize MallView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return Mall_CELL_SIZE;
}

CCTableViewCell* MallView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	MallViewCell *cell = (MallViewCell*)table->dequeueCell();
	if (!cell)
	{
		cell = MallViewCell::create(m_pData);
        cell->setDelegate(m_pDelegate);
        //cell->getBuyMenuItem()->setTarget(this,menu_selector(MallView::buyMenuCallback));
		cell->getBuyButton()->getButton()->setTarget(this,menu_selector(MallView::buyMenuCallback));
	}
    
    mall::ProductInfo * pProductInfo = (mall::ProductInfo *)m_products.objectAtIndex(idx);
    cell->setProductInfo(pProductInfo);
	return cell;
    
}

uint32_t MallView::numberOfCellsInTableView(CCTableView *table)
{
	return m_products.count();
}

void MallView::buyMenuCallback(CCObject * obj)
{
    cocos2d::CCMenuItem* pItem = (cocos2d::CCMenuItem*)obj;
    mall::ProductInfo* pProductInfo = (mall::ProductInfo*)pItem->getUserObject();
	if(pProductInfo)
	{
        if(!pProductInfo->isCanBuy())
        {
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
            return;
        }
        
        if(pProductInfo->getPrice() > m_pUserInfo->getPoints())
        {
            NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
            return;
        }
        HTTP_CLIENT->buyProduct(pProductInfo->getPId(),1,this,callfuncJson_selector(MallView::buyCallback));
	}
}

void MallView::buyCallback(const Json::Value & requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	Tips_Alert(D_LOCAL_STRING("$mall_purchase"));
	m_bNoFly = true;
    updateMallView();
}

/************************ Mall view class end*******************/










/************************ Mall cell class begin**********************/


MallViewCell::MallViewCell()
:m_pProductInfo(NULL)
,m_pContainer(NULL)
,m_pName(NULL)
,m_pAmount(NULL)
,m_pDesc(NULL)
,m_pBuyMenuItem(NULL)
,m_pNoSale(NULL)
,m_pPriceSingle(NULL)
,m_pOnSale(NULL)
,m_pPrice(NULL)
,m_pPriceNow(NULL)
,m_pNum(NULL)
,m_pDelegate(NULL)
,m_pIfTime(NULL)
,m_pPriceBeforeNode(NULL)
,m_pBuy(NULL)
,m_pGradeBlue(NULL)
,m_pGradePurple(NULL)
,m_pGradeOrange(NULL)
,m_pLimiteBuy(NULL)
{
    
}

MallViewCell::~MallViewCell()
{
    CC_SAFE_RELEASE(m_pProductInfo);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pAmount);
    CC_SAFE_RELEASE(m_pDesc);
    CC_SAFE_RELEASE(m_pBuyMenuItem);
    CC_SAFE_RELEASE(m_pNoSale);
    CC_SAFE_RELEASE(m_pPriceSingle);
    CC_SAFE_RELEASE(m_pOnSale);
    CC_SAFE_RELEASE(m_pPrice);
    CC_SAFE_RELEASE(m_pPriceNow);
    CC_SAFE_RELEASE(m_pNum);
    CC_SAFE_RELEASE(m_pIfTime);
	CC_SAFE_RELEASE(m_pPriceBeforeNode);
	CC_SAFE_RELEASE(m_pBuy);
    CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradePurple);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pLimiteBuy);
}

bool MallViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemInfo", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount", CCLabelBMFont *, this->m_pAmount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceBefore", CCLabelBMFont *, this->m_pPrice);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceNow", CCLabelBMFont *, this->m_pPriceNow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceBeforeNode", CCNode *, this->m_pPriceBeforeNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBuy", DButton *, this->m_pBuy);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBuyMenuItem", CCMenuItem *, this->m_pBuyMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoSale", CCNode *, this->m_pNoSale);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceSingle", CCLabelBMFont *, this->m_pPriceSingle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnSale", CCNode *, this->m_pOnSale);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNum", CCLabelBMFont *, this->m_pNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIfTime", CCNode *, this->m_pIfTime);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pGradePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pGradeOrange);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLimiteBuy" ,UIAnimate*,m_pLimiteBuy)
	
    return false;
}

SEL_MenuHandler MallViewCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", MallViewCell::viewCallBack);
    return NULL;
}


MallViewCell* MallViewCell::create(cocos2d::extension::CCData* pData)
{
    MallViewCell* pRet = new MallViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool MallViewCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
	return true;
}

void MallViewCell::setProductInfo(mall::ProductInfo *pProductInfo)
{
    //这里不能判断相等 数量会变
    CC_SAFE_RETAIN(pProductInfo);
    CC_SAFE_RELEASE(m_pProductInfo);
    m_pProductInfo = pProductInfo;
    updateProductInfo();
}

void MallViewCell::updateProductInfo()
{
	//if(!m_pProductInfo) return;
	//m_pName->setString(m_pProductInfo->getBaseInfo()->getProductName().c_str());
 //   
 //   m_pContainer->removeAllChildren();
 //   m_pContainer->addChild(ResourceManager::sharedResourceManager()->getProductIconForList(m_pProductInfo->getPId(),m_pProductInfo->getBaseInfo()->getType()));
	//
 //   m_pNoSale->setVisible(m_pProductInfo->getBaseInfo()->getOldPrice()==0);
 //   m_pOnSale->setVisible(!m_pNoSale->isVisible());
 //   m_pIfTime->setVisible(m_pProductInfo->getBaseInfo()->getIfTime());
 //   m_pPriceSingle->setString(CCString::createWithFormat("%d",m_pProductInfo->getPrice())->getCString());
 //   m_pPriceNow->setString(CCString::createWithFormat("%d",m_pProductInfo->getPrice())->getCString());
 //   m_pPrice->setString(CCString::createWithFormat("%d",m_pProductInfo->getBaseInfo()->getOldPrice())->getCString());
 //   m_pNum->setString(CCString::createWithFormat("x%d",m_pProductInfo->getBaseInfo()->getNum())->getCString());
 //   m_pBuyMenuItem->setUserObject(m_pProductInfo);
 //   m_pBuyMenuItem->setEnabled(m_pProductInfo->isCanBuy() || m_pProductInfo->getBaseInfo()->getReqDesc()==BUY_CONDITION_VIP); //vip礼包仍然可买 给出充值提示
 //   
 //   m_pAmount->setString(CCString::createWithFormat("%i",m_pProductInfo->getNum())->getCString());
 //   m_pDesc->setString(m_pProductInfo->getDesc().c_str());

	if(!m_pProductInfo) return;
	m_pName->setString(m_pProductInfo->getBaseInfo()->getProductName().c_str());

	m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getProductIconForList(m_pProductInfo->getPId(),m_pProductInfo->getBaseInfo()->getType()));

	m_pPriceBeforeNode->setVisible(m_pProductInfo->getBaseInfo()->getOldPrice()!=0);
	m_pPriceNow->setString(CCString::createWithFormat("%d",m_pProductInfo->getPrice())->getCString());
	m_pPrice->setString(CCString::createWithFormat("%d",m_pProductInfo->getBaseInfo()->getOldPrice())->getCString());
	m_pBuy->getButton()->setUserObject(m_pProductInfo);
	//m_pBuy->getButton()->setEnabled(m_pProductInfo->isCanBuy() || m_pProductInfo->getBaseInfo()->getReqDesc()==BUY_CONDITION_VIP);

	m_pAmount->setString(CCString::createWithFormat("%i",m_pProductInfo->getNum())->getCString());
	if(m_pProductInfo->getBaseInfo()->getType() == mall::kProductTypeAvatar)
	{
		m_pBuy->getButton()->setEnabled(m_pProductInfo->getNum() == 0);
	}

	m_pDesc->setString(m_pProductInfo->getDesc().c_str());
	
	item::ItemBase* itemBase = DM_GET_ITEM_MODEL->getItemBaseByID(m_pProductInfo->getBaseInfo()->getItemId());
	if(itemBase)
	{
		item::ItemGrade grade = itemBase->getItemGrade();
		m_pGradeBlue->setVisible(grade == item::kItemGradeBlue);
		m_pGradePurple->setVisible(grade == item::kItemGradePurple);
		m_pGradeOrange->setVisible(grade == item::kItemGradeRed);
	}
	
	//是否限时
	if(m_pProductInfo->getBaseInfo()->getIfTime())
	{
		m_pLimiteBuy->setVisible(true);
		m_pLimiteBuy->playAtIndex(0);
	}
	else
	{
		m_pLimiteBuy->setVisible(false);
		m_pLimiteBuy->stop();
	}
}


void MallViewCell::viewCallBack(cocos2d::CCObject* obj)
{
    if(!m_pDelegate) return;
    if (m_pProductInfo->getBaseInfo()->getType() == mall::kProductTypeItem || m_pProductInfo->getBaseInfo()->getType() == mall::kProductTypeVipPk)
    {
        m_pDelegate->showInfoView(kItemInfo, item::ItemInfo::create(m_pProductInfo->getBaseInfo()->getItemId(), 0));   
    }
	else if(m_pProductInfo->getBaseInfo()->getType() == mall::kProductTypeAvatar)
	{
		m_pDelegate->showAvatar(m_pProductInfo);   
	}
}


/************************ Mall view cell class end**********************/





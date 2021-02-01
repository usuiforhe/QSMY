//
//  DressChange.cpp
//  时装模块
//  Created by mzp on 14-5-15.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "DressChange.h"
#include "../../../model/AvatarModel.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../warrior/WarriorDelegate.h"


USING_NS_CC;


DressChange::DressChange()
	: m_pWarriorInfo(NULL)
	, m_pDressList(NULL)
	, m_pPropertyList(NULL)
	, m_pDelegate(NULL)
	, m_pConfirmButton(NULL)
	, m_pBtnOk(NULL)
	, m_pSelectButton(NULL)
	, m_pPrev(NULL)
	, m_pNext(NULL)
	, m_pAvatarAni(NULL)
	, m_pAvatarContainer(NULL)
	, m_pTableContainer(NULL)
	, m_pPropertyTable(NULL)
	, m_pListView(NULL)
	, m_pPropertyView(NULL)
	, m_pData(NULL)
	, m_pPropertyData(NULL)
    , m_pShowDress(NULL)
	, m_pWarriorGradeBg(NULL)
	, m_pMenuTouch(NULL)
	, m_pCount(NULL)
{

}

DressChange::~DressChange()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RELEASE(m_pDressList);
	CC_SAFE_RELEASE(m_pPropertyList);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pBtnOk);
	CC_SAFE_RELEASE(m_pSelectButton);
	CC_SAFE_RELEASE(m_pPrev);
	CC_SAFE_RELEASE(m_pNext);
	CC_SAFE_RELEASE(m_pAvatarContainer);
	CC_SAFE_RELEASE(m_pTableContainer);
	CC_SAFE_RELEASE(m_pPropertyTable);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pPropertyView);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pPropertyData);
    CC_SAFE_RELEASE(m_pShowDress);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
	CC_SAFE_RELEASE(m_pMenuTouch);
	CC_SAFE_RELEASE(m_pCount);
}

bool DressChange::init()
{
	if (!DLayer::init())
	{
		return false;
	}

	CCNodeLoaderLibrary *pNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary() ;
	CUSTOM_CCB_LOADER( pNodeLoaderLibrary ) ;
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorGradeBg", WarriorGradeBgLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shizhuang/PL_shizhuang", pNodeLoaderLibrary ) ;

	m_pData = readCCData("PL_ui/ccb/pl_shizhuang/PL_shizhuang_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pPropertyData = readCCData("PL_ui/ccb/pl_shizhuang/PropertyCell");
	CC_SAFE_RETAIN(m_pPropertyData);

	m_pConfirmButton->getButton()->setTarget(this, menu_selector(DressChange::closeCallback));
	m_pSelectButton->getButton()->setTarget(this, menu_selector(DressChange::onSelectClick));
	m_pMenuTouch->setTarget(this, menu_selector(DressChange::onTouchFlash));
	//m_pBtnOk->getButton()->setTarget(this, menu_selector(DressChange::closeCallback));

	m_pListView = DTableView::create(this, m_pTableContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pListView->setDelegate(this);
	m_pTableContainer->addChild(m_pListView);
	CC_SAFE_RETAIN(m_pListView);

	m_pPropertyView = DTableView::create(this, m_pPropertyTable->getContentSize());
	m_pPropertyView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pPropertyView->setDelegate(this);
	m_pPropertyTable->addChild(m_pPropertyView);
	CC_SAFE_RETAIN(m_pPropertyView);

	return true;
}

bool DressChange::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, this->m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnOk", DButton *, this->m_pBtnOk);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton", DButton *, this->m_pSelectButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrev", cocos2d::CCMenuItem *, this->m_pPrev);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNext", cocos2d::CCMenuItem *, this->m_pNext);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatarContainer", cocos2d::CCNode *, this->m_pAvatarContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableContainer", cocos2d::CCNode *, this->m_pTableContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyTable", cocos2d::CCNode *, this->m_pPropertyTable);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pShowDress", cocos2d::CCNode *, this->m_pShowDress);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg *, this->m_pWarriorGradeBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuTouch", cocos2d::CCMenuItem *, this->m_pMenuTouch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount", cocos2d::CCLabelTTF *, this->m_pCount);
	return false;
}

cocos2d::SEL_MenuHandler DressChange::onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectClick", DressChange::onSelectClick);

	return NULL;
}

void DressChange::onTouchFlash(cocos2d::CCObject* obj)
{
	if(m_pAvatarAni)
	{
		int count = m_pAvatarAni->getAnimation()->getMovementCount();
		int idx = count * CCRANDOM_0_1();
		m_pAvatarAni->getAnimation()->playByIndex(idx, -1, -1, true);
	}	
}

void DressChange::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{
	if(table != m_pListView)return;

	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo() || !m_pDressList)
	{
		return;
	}

	unsigned int index = cell->getIdx();
	m_pAvatar = (avatar::AvatarBaseInfo*)m_pDressList->objectAtIndex(index);
	if (!m_pAvatar)
	{
		return;
	}

	if (m_pWarriorInfo->getDressId() != m_pAvatar->getId())
	{

        if(NULL!=DM_GET_AVATAR_MODEL->getAvatarInfoById(m_pAvatar->getId())){
            DM_GET_WARRIOR_MODEL->dressUp(m_pAvatar, true);
        }else{
			mall::ProductInfo *  productInfo = DM_GET_MALL_MODEL->getProductByItemId(m_pAvatar->getId());
			if(productInfo->getBaseInfo()->getOldPrice()==0){
			DMessageBox::showMessageBox(D_LOCAL_STRING("buy%dADress",productInfo->getPrice()).c_str(), this, messageBox_callfunc_selector(DressChange::BuyDress));
			}else if(productInfo->getBaseInfo()->getOldPrice()>productInfo->getPrice())
			DMessageBox::showMessageBox(D_LOCAL_STRING("buy%dADress",productInfo->getPrice()).c_str(), this, messageBox_callfunc_selector(DressChange::BuyDress));
			else
			DMessageBox::showMessageBox(D_LOCAL_STRING("buy%dADress",productInfo->getBaseInfo()->getOldPrice()).c_str(), this, messageBox_callfunc_selector(DressChange::BuyDress));
		}
	}
}

void DressChange::BuyDress(MessageType type,DMessageBox* target){
    if (type == kMessageTypeCancel) return;
    mall::ProductInfo *  productInfo = DM_GET_MALL_MODEL->getProductByItemId(m_pAvatar->getId());
    HTTP_CLIENT->buyProduct(productInfo->getPId(),1,this,callfuncJson_selector(DressChange::buyCallback));
}

void DressChange::buyCallback(const Json::Value & requestData,const Json::Value &responseData){
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    DM_GET_WARRIOR_MODEL->dressUp(m_pAvatar, true);
}



cocos2d::CCSize DressChange::tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	if(table == m_pListView)
		return CCSizeMake(250, 300);
	else
		return CCSizeMake(200,50);
}

cocos2d::extension::CCTableViewCell* DressChange::tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	if(table == m_pListView)
	{
		DressViewCell* cell = (DressViewCell*)table->dequeueCell();
		if (!cell)
		{
			cell = DressViewCell::create(m_pData);
		}

		if (m_pDressList)
		{
			cell->setAvatarInfo((avatar::AvatarBaseInfo*)m_pDressList->objectAtIndex(idx));
		}
		else
		{
			cell->setAvatarInfo(NULL);
		}

		return cell;
	}
	else
	{
		DressPropertyCell* cell = (DressPropertyCell*)table->dequeueCell();
		if (!cell)
		{
			cell = DressPropertyCell::create(m_pPropertyData);
		}

		if (m_pPropertyList)
		{
			cell->setPropertyInfo((avatar::AvatarPropertyInfo*)m_pPropertyList->objectAtIndex(idx));
		}
		else
		{
			cell->setPropertyInfo(NULL);
		}

		return cell;
	}
}

unsigned int DressChange::numberOfCellsInTableView(cocos2d::extension::CCTableView* table)
{
	if(table == m_pListView)
	{
		return (m_pDressList == NULL ? 0 :m_pDressList->count());
	}	
	else
	{
		return (m_pPropertyList == NULL ? 0 :m_pPropertyList->count());
	}	
}

void DressChange::setWarriorInfo(warrior::WarriorCurInfo* pWarriorCurInfo)
{
	if (m_pWarriorInfo == pWarriorCurInfo)
	{
		return;
	}
	
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RETAIN(pWarriorCurInfo);
	m_pWarriorInfo = pWarriorCurInfo;
	BINDING_EXEC(m_pWarriorInfo, this, DressChange::updateView, D_EVENT_WARRIORINFO_UPDATE);
}

void DressChange::closeCallback(cocos2d::CCObject* obj)
{
	if (m_pDelegate)
	{
		m_pDelegate->closeInfoView(kDressView);
	}
}

void DressChange::onSelectClick(cocos2d::CCObject* sender)
{
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo() || !m_pDressList)
	{
		return;
	}

	if (m_pWarriorInfo->getDressId() != 0)
	{
		m_pWarriorInfo->unDress();
	}
	else
	{
		if (m_pDressList->count() > 0)
		{
			avatar::AvatarBaseInfo* pAvatar = (avatar::AvatarBaseInfo*)m_pDressList->objectAtIndex(0);
			DM_GET_WARRIOR_MODEL->dressUp(pAvatar, true);
		}
	}
}

void DressChange::updateView()
{
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		return;
	}

	m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
    
	//todo
	//防止崩溃
	m_pAvatarContainer->removeAllChildren();
	m_pAvatarAni = NULL;
	m_pAvatarAni = ResourceManager::sharedResourceManager()->getUnitAnimate2X(m_pWarriorInfo->getAvatarId());
	m_pAvatarAni->setScale(1.8f);
	m_pAvatarContainer->addChild(m_pAvatarAni);

	if (m_pWarriorInfo->getDressId() != 0)
	{
		m_pSelectButton->getButton()->selected();
	}
	else
	{
		m_pSelectButton->getButton()->unselected();
	}

	CC_SAFE_RELEASE(m_pDressList);
    m_pDressList = DM_GET_AVATAR_MODEL->findAvatarInfoForWarrior(m_pWarriorInfo->getWid());
	CC_SAFE_RETAIN(m_pDressList);

	//计算属性加成表
	CC_SAFE_RELEASE(m_pPropertyList);
	m_pPropertyList = DM_GET_AVATAR_MODEL->getPropertyList(m_pWarriorInfo->getWid());
	CC_SAFE_RETAIN(m_pPropertyList);
	

	m_pListView->reloadData();
	m_pPropertyView->reloadData();
    //m_pShowDress->setVisible(false);
    CCObject* obj1 = NULL;
    avatar::AvatarBaseInfo*  pNode = NULL;
	unsigned int dressCount = (m_pDressList == NULL ? 0 : m_pDressList->count());
	unsigned int dressOwnCount = 0;
    CCARRAY_FOREACH(m_pDressList, obj1)
    {
        pNode = static_cast<avatar::AvatarBaseInfo*>(obj1);
        avatar::AvatarInfo* info = DM_GET_AVATAR_MODEL->getAvatarInfoById(pNode->getId());
        if(info){
			dressOwnCount++;
            //m_pShowDress->setVisible(true);
            //break;
        }
	}
    
    //显示拥有比例
	m_pCount->setString(CCString::createWithFormat("(%d/%d)" , dressOwnCount , dressCount)->getCString());
    

	//if (m_pListView->getViewSize().width >= m_pListView->getContentSize().width)
	//{
	//	m_pPrev->setEnabled(false);
	//	m_pNext->setEnabled(false);
	//}
	//else
	//{
	//	m_pPrev->setEnabled(true);
	//	m_pNext->setEnabled(true);
	//}
}




DressViewCell::DressViewCell()
	: m_pContainer(NULL)
	, m_pBoard(NULL)
	, m_pShow(NULL)
	, m_pMark(NULL)
	, m_pValue(NULL)
    , m_pWillBuy(NULL)
	, m_pName(NULL)
	, m_pShining(NULL)
	,m_pPriceNode(NULL)
    ,m_pPriceNow(NULL)
    ,m_pPriceBmTf(NULL)
    ,m_pPrice1(NULL)
    ,m_pOnSale(NULL)
    ,m_pPrice(NULL)
    ,m_pPriceBmTf1(NULL)
    ,m_pHave(NULL)
{
	D_RESET_C_ARRAY(m_pSpecialDressMark, SpecialDressMark);
}

DressViewCell::~DressViewCell()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pBoard);
	CC_SAFE_RELEASE(m_pShow);
	CC_SAFE_RELEASE(m_pMark);
	CC_SAFE_RELEASE(m_pValue);
	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pWillBuy);
    CC_SAFE_RELEASE(m_pPriceNow);
	CC_SAFE_RELEASE(m_pPriceNode);
	CC_SAFE_RELEASE(m_pShining);
    
    CC_SAFE_RELEASE(m_pPriceBmTf);
    CC_SAFE_RELEASE(m_pPriceBmTf1);
    CC_SAFE_RELEASE(m_pHave);
    CC_SAFE_RELEASE(m_pPrice1);
    CC_SAFE_RELEASE(m_pOnSale);
    CC_SAFE_RELEASE(m_pPrice);
	D_SAFE_RELEASE_C_ARRAY(m_pSpecialDressMark, SpecialDressMark);
}

DressViewCell* DressViewCell::create(cocos2d::extension::CCData* pData)
{
	DressViewCell* pRet = new DressViewCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool DressViewCell::init(cocos2d::extension::CCData* pData)
{
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());
	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);

	return true;
}

bool DressViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHave", CCNode*, this->m_pHave);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoard", CCNode*, this->m_pBoard);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pShow", CCNode*, this->m_pShow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWillBuy", CCNode*, this->m_pWillBuy);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMark", EquipTypeMark*, this->m_pMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pValue", CCLabelBMFont*, this->m_pValue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pShining", CCNode*, this->m_pShining);

	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyGroup", CCNode*, this->m_pPropertyGroup);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyGroup1", CCNode*, this->m_pPropertyGroup1);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType1", EquipTypeMark*, this->m_pPropertyType1);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty1", CCLabelBMFont*, this->m_pProperty1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, this->m_pName);
		
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceNode", CCNode*, this->m_pPriceNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont*, this->m_pPriceNow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceBmTf", CCLabelBMFont*, this->m_pPriceBmTf);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPriceBmTf1", CCLabelBMFont*, this->m_pPriceBmTf1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice1", CCNode*, this->m_pPrice1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOnSale", CCNode*, this->m_pOnSale);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCNode*, this->m_pPrice);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pMarkType",CCNode*,this->m_pSpecialDressMark,SpecialDressMark);
	return false;
}

void DressViewCell::setAvatarInfo(avatar::AvatarBaseInfo* pAvatarInfo)
{
    
	if (pAvatarInfo)
	{
		m_pContainer->removeAllChildren();
		cocos2d::CCNode* obj = ResourceManager::sharedResourceManager()->getWarriorIcon(pAvatarInfo->getResId() , 0.5f);
		m_pContainer->addChild(obj);
        
        avatar::AvatarInfo* info = DM_GET_AVATAR_MODEL->getAvatarInfoById(pAvatarInfo->getId());


		// 判断是否是当前装备的时装
		warrior::WarriorCurInfo* pWarriorCurInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pAvatarInfo->getWarriorId());
		if (pWarriorCurInfo && pWarriorCurInfo->getDressId() == pAvatarInfo->getId())
		{
			m_pBoard->setVisible(true);
			m_pShining->setVisible(true);
			m_pShow->setVisible(true);
            m_pHave->setVisible(false);
		}
		else
		{
			m_pBoard->setVisible(false);
			m_pShining->setVisible(false);
			m_pShow->setVisible(false);
            m_pHave->setVisible(NULL != info);
		}       
        m_pWillBuy->setVisible(NULL == info);

		m_pPriceNode->setVisible(!pAvatarInfo->getIsOwn());
		mall::ProductInfo *  productInfo = DM_GET_MALL_MODEL->getProductByItemId(pAvatarInfo->getId());
		if(productInfo)m_pPriceNow->setString(CCString::createWithFormat("%d",productInfo->getPrice())->getCString());

        //mall::ProductInfo *  productInfo = DM_GET_MALL_MODEL->getProductByItemId(pAvatarInfo->getId());
        //if(productInfo){
        //    m_pPriceNow->setString(CCString::createWithFormat("%d",productInfo->getPrice())->getCString());
        //
        //    if(productInfo->getBaseInfo()->getOldPrice()==0){
        //        m_pPriceBmTf->setString(CCString::createWithFormat("%d",productInfo->getPrice())->getCString());
        //        m_pPriceBmTf1->setString(CCString::createWithFormat("%d",productInfo->getPrice())->getCString());
        //    }else{
        //        m_pPriceBmTf->setString(CCString::createWithFormat("%d",productInfo->getBaseInfo()->getOldPrice())->getCString());
        //        m_pPriceBmTf1->setString(CCString::createWithFormat("%d",productInfo->getBaseInfo()->getOldPrice())->getCString());
        //    }
        //}
        //if(info ||NULL==productInfo){
        //    m_pPrice->setVisible(false);//原价
        //    m_pOnSale->setVisible(false);//特价
        //    m_pPrice1->setVisible(false);//价格
        //}else{
        //    m_pPrice1->setVisible(productInfo->getBaseInfo()->getOldPrice()==0);//价格
        //    m_pPrice->setVisible(!m_pPrice1->isVisible());//原价
        //    m_pOnSale->setVisible(!m_pPrice1->isVisible());//特价
        //}
        

        

		unsigned int group = 0;

		equip::TreasurePropType dresstype = pAvatarInfo->getPropType();
		m_pMark->setEquipType(dresstype);
		std::string str = "";
		if((int)dresstype >= 4)str = "%";
		m_pValue->setString(CCString::createWithFormat("+%d%s" , pAvatarInfo->getDressValue() , str.c_str())->getCString());

		//m_pPropertyGroup->setVisible(group > 0);
		//m_pPropertyGroup1->setVisible(group > 1);

		// 时装名字
		m_pName->setString(pAvatarInfo->getName().c_str());

		//特殊
		for(int i = 0 ; i < SpecialDressMark ; i++)
		{
			if(m_pSpecialDressMark[i])
			{
				m_pSpecialDressMark[i]->setVisible(i+1 == pAvatarInfo->getDressMark());
			}
		}	
	}
	else
	{
		m_pContainer->removeAllChildren();
		m_pBoard->setVisible(false);
		m_pShining->setVisible(false);
		m_pShow->setVisible(false);
		//m_pPropertyGroup->setVisible(false);
		//m_pPropertyGroup1->setVisible(false);
		m_pName->setString("");
	}
}



DressPropertyCell::DressPropertyCell()
	: m_pValue(NULL)
{
	D_RESET_C_ARRAY(m_pMarkFont, DressMarkCount);
}

DressPropertyCell::~DressPropertyCell()
{
	CC_SAFE_RELEASE(m_pValue);
	D_SAFE_RELEASE_C_ARRAY(m_pMarkFont, DressMarkCount);
}

DressPropertyCell* DressPropertyCell::create(cocos2d::extension::CCData* pData)
{
	DressPropertyCell* pRet = new DressPropertyCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool DressPropertyCell::init(cocos2d::extension::CCData* pData)
{
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("EquipTypeMark", EquipTypeMarkLoader::loader());
	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pNodeLoaderLibrary);

	return true;
}

bool DressPropertyCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pValue", CCLabelBMFont*, this->m_pValue);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType",CCNode*,this->m_pMarkFont,DressMarkCount);

	return false;
}

void DressPropertyCell::setPropertyInfo(avatar::AvatarPropertyInfo* pInfo)
{
	setVisible(pInfo != NULL);
	if (pInfo)
	{
		
		//equip::TreasurePropType dresstype = pAvatarInfo->getPropType();
		//m_pMark->setEquipType(dresstype);

		unsigned int dresstype = pInfo->getPropertyType();
		//11,12,13百分比攻防血
		int marktype = (dresstype > 10 ? dresstype%10 : dresstype);
		for(int i = 0 ; i < DressMarkCount ; i++)
		{
			if(m_pMarkFont[i])
			{
				m_pMarkFont[i]->setVisible(i+1 == marktype);
			}
		}

		std::string str = "";
		if((int)dresstype >= 4)str = "%";
		m_pValue->setString(CCString::createWithFormat("+%d%s" , pInfo->getPropertyValue() , str.c_str())->getCString());
	}
}
//
//  EquipMultiSelect.cpp
//  装备多选列表
//  Created by mzp on 14-04-11.
//  Copyright (c) 2013, thedream. All rights reserved.
//
#include "../Game.h"
#include "EquipMultiSelect.h"
#include "../../../model/EquipModel.h"
#include "EquipView.h"
#include "managers/GameManager.h"
#include <vector>
#include <algorithm>

USING_NS_CC;

EquipMultiSelect::EquipMultiSelect()
	: m_pDTabView(NULL)
	, m_pEquipModel(NULL)
	, m_pEquipMultiSelecteDelegate(NULL)
	, m_pEquipSelectVec(NULL)
	, m_pEquipList(NULL)
	, m_pContainer(NULL)
	, m_pConfirmButton(NULL)
	, m_pCancelButton(NULL)
	, m_pImageTips(NULL)
	, m_pImageTipsGreen(NULL)
	, m_pImageTipsBlue(NULL)
	, m_pImageTipsPurple(NULL)
	, m_pImageTipsOrange(NULL)
	, m_pCloseButton(NULL)
	, m_pView(NULL)
	, m_pSellButton(NULL)
	, m_pGradeSellButton(NULL)
	, m_pSelectCount(NULL)
	, m_pTotalAmount(NULL)
	, m_uTotalPrice(0)
{
	//grade map
	m_pGradeMap[equip::kEquipGradeGreen] = false;
	m_pGradeMap[equip::kEquipGradeBlue] = false;
	m_pGradeMap[equip::kEquipGradePurple] = false;
	m_pGradeMap[equip::kEquipGradeRed] = false;
}

EquipMultiSelect::~EquipMultiSelect()
{
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pEquipSelectVec);
	CC_SAFE_RELEASE(m_pEquipList);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pCancelButton);
	CC_SAFE_RELEASE(m_pImageTips);
	CC_SAFE_RELEASE(m_pImageTipsGreen);
	CC_SAFE_RELEASE(m_pImageTipsBlue);
	CC_SAFE_RELEASE(m_pImageTipsPurple);
	CC_SAFE_RELEASE(m_pImageTipsOrange);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pView);
	CC_SAFE_RELEASE(m_pSellButton);
	CC_SAFE_RELEASE(m_pGradeSellButton);
	CC_SAFE_RELEASE(m_pSelectCount);
	CC_SAFE_RELEASE(m_pTotalAmount);
}

bool EquipMultiSelect::init()
{
	if (!DLayer::init())
	{
		return false;
	}

	m_pEquipModel = DM_GET_EQUIP_MODEL;

	m_pEquipSelectVec = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pEquipSelectVec);

	m_pEquipList = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pEquipList);

	//CCB_READER_AND_ADDCHILD("ui/ccb/equip/equipDecomposeListSelect");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_chushou");

	m_pCloseButton->getButton()->setTarget(this, menu_selector(EquipMultiSelect::closeCallback));
	m_pSellButton->setTarget(this, menu_selector(EquipMultiSelect::confirmCallback));
	m_pGradeSellButton->setTarget(this, menu_selector(EquipMultiSelect::showPopup));

	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, m_pContainer->getPosition().y, m_pContainer->getContentSize().width, m_pContainer->getContentSize().height), 15);
	m_pDTabView->setTopMenuisLeftAlignment(false);
	m_pDTabView->retain();
	m_pDTabView->setDelegate(this);

	std::string tabName[4] = {"font_sheet_green","font_sheet_blue","font_sheet_purple","font_sheet_orange"};
	for (uint32_t i = 0; i < 4; i++)
	{
	EquipMultiSelectView* equipView = EquipMultiSelectView::create(equip::EquipGrade(i + 1), PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));//TODO
	PL_MOVE_WITH_FLEXIBLE_HEIGHT(equipView,80);
	equipView->setEquipMultiSelect(this);
	//m_pView = equipView;
	m_pDTabView->addTab(equipView,"","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,"PL_ui/beibao/pl_bb_btn_xiaofenye_up.png");
	//m_pDTabView->addTab(equipView,"",NULL,NULL,NULL,NULL);
	}
	addChild(m_pDTabView);
	m_pDTabView->setTabSize(CCSizeMake(1,1));
	//addChild(m_pView);
	return true;
}

void EquipMultiSelect::onEnter()
{
	DLayer::onEnter();

	updateEquipList();

	m_pDTabView->selectedTab(0);

	BINDING_EXEC(m_pEquipModel, this, EquipMultiSelect::equipListUpdate, D_EVENT_EQUIPLIST_UPDATE);

	Game::sharedGame()->setNavigatorVisable(false);
}

void EquipMultiSelect::onExit()
{
	UNBINDING_ALL(m_pEquipModel, this);

	Game::sharedGame()->setNavigatorVisable(true);
	DLayer::onExit();
}

void EquipMultiSelect::onGradeSelect(std::vector<equip::EquipGrade> v)
{
	if(v.empty()){
		//select nothing
		return;
	}
	else{
		CCObject *pObj = NULL;
		equip::EquipCurInfo* pEquipCurInfo = NULL;
		uint32_t totalprice = 0;
		std::vector<equip::EquipGrade>::iterator iter;

		m_pEquipSelectVec->removeAllObjects();

		CCARRAY_FOREACH(m_pEquipList, pObj)
		{
			pEquipCurInfo = (equip::EquipCurInfo*)pObj;
			equip::EquipGrade equipGrade = pEquipCurInfo->getBaseInfo()->getEquipGrade();

			iter = find(v.begin() , v.end() , equipGrade);
			if(iter!=v.end()){
				m_pEquipSelectVec->addObject(pEquipCurInfo);
				//更新 钱
				totalprice += DM_GET_EQUIP_MODEL->getPriceForSell(pEquipCurInfo);
			}
		}


		EquipMultiSelectView* selectView = (EquipMultiSelectView*)m_pDTabView->getSelectedTab();
		if (!selectView)
		{
			return;
		}
		//update checkbox
		selectView->updateEquipView(false);
		//update count
		m_pSelectCount->setString(CCString::createWithFormat("%d",m_pEquipSelectVec->count())->getCString());
		//update total amount
		m_uTotalPrice = totalprice;
		//m_pTotalAmount->setString(CCString::createWithFormat("%d" , m_uTotalPrice)->getCString());		
		m_uTotalPrice < MAX_SHOW_COIN ? m_pTotalAmount->setString(D_CSTRING_FROM_UINT(m_uTotalPrice)) : m_pTotalAmount->setString(D_CSTRING_WITH10K_FROM_UINT(m_uTotalPrice));
	}
}

bool EquipMultiSelect::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellButton", CCMenuItemImage *, this->m_pSellButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeSellButton", CCMenuItemImage *, this->m_pGradeSellButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectCount", CCLabelBMFont *, this->m_pSelectCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTotalAmount", CCLabelBMFont *, this->m_pTotalAmount);

	return true;
}

void EquipMultiSelect::SelectTabAtIndex(DTabView* tabView, uint32_t idx)
{
	m_pEquipSelectVec->removeAllObjects();
	updateTips();

	EquipMultiSelectView* selectView = (EquipMultiSelectView*)tabView->getSelectedTab();
	if (!selectView)
	{
		return;
	}
	
	equip::EquipGrade grade = selectView->getEquipGrade();
	if (!m_pEquipList)
	{
		return;
	}
	//select no-one at beginning
	//m_pEquipSelectVec->addObjectsFromArray(pEquipList);
	selectView->updateEquipView();
}

void EquipMultiSelect::confirmCallback(cocos2d::CCObject* sender)
{
	if (m_pEquipMultiSelecteDelegate)
	{
		m_pEquipMultiSelecteDelegate->selectedEquips(m_pEquipSelectVec);
	}
}

void EquipMultiSelect::cancelCallback(cocos2d::CCObject* sender)
{
	if (m_pEquipMultiSelecteDelegate)
	{
		m_pEquipMultiSelecteDelegate->selectedEquips(NULL);
	}
}

void EquipMultiSelect::closeCallback(cocos2d::CCObject* sender)
{
	if (m_pEquipMultiSelecteDelegate)
	{
		m_pEquipMultiSelecteDelegate->selectedEquips(NULL);
	}
}

void EquipMultiSelect::showPopup(cocos2d::CCObject* sender)
{
	//show sell popup
	if (m_pEquipMultiSelecteDelegate)
	{
		//reset
		m_pGradeMap[equip::kEquipGradeGreen] = false;
		m_pGradeMap[equip::kEquipGradeBlue] = false;
		m_pGradeMap[equip::kEquipGradePurple] = false;
		m_pGradeMap[equip::kEquipGradeRed] = false;


		CCObject *pObj = NULL;
		equip::EquipCurInfo* pEquipCurInfo = NULL;
		
		CCARRAY_FOREACH(m_pEquipList, pObj)
		{
			pEquipCurInfo = (equip::EquipCurInfo*)pObj;
			equip::EquipGrade equipGrade = pEquipCurInfo->getBaseInfo()->getEquipGrade();
			m_pGradeMap[equipGrade] = true;			
		}

		m_pEquipMultiSelecteDelegate->showInfoView(kEquipDeletePopup , NULL);		
	}

}

void EquipMultiSelect::selectEquip(equip::EquipCurInfo* pEquipCurInfo)
{
	if (!pEquipCurInfo)
	{
		return;
	}

	if (isEquipSelected(pEquipCurInfo))
	{
		return;
	}
	//m_pEquipSelectVec->removeAllObjects();
	m_pEquipSelectVec->addObject(pEquipCurInfo);
	m_pSelectCount->setString(CCString::createWithFormat("%d",m_pEquipSelectVec->count())->getCString());

	//uint32_t totalprice = atoi(m_pTotalAmount->getString()) + DM_GET_EQUIP_MODEL->getPriceForSell(pEquipCurInfo);	
	//m_pTotalAmount->setString(CCString::createWithFormat("%d" , totalprice)->getCString());

	m_uTotalPrice += DM_GET_EQUIP_MODEL->getPriceForSell(pEquipCurInfo);	
	//m_pTotalAmount->setString(CCString::createWithFormat("%d" , m_uTotalPrice)->getCString());
	m_uTotalPrice < MAX_SHOW_COIN ? m_pTotalAmount->setString(D_CSTRING_FROM_UINT(m_uTotalPrice)) : m_pTotalAmount->setString(D_CSTRING_WITH10K_FROM_UINT(m_uTotalPrice));
}

void EquipMultiSelect::unselectEquip(equip::EquipCurInfo* pEquipCurInfo)
{
	if (!pEquipCurInfo)
	{
		return;
	}

	m_pEquipSelectVec->removeObject(pEquipCurInfo);
	m_pSelectCount->setString(CCString::createWithFormat("%d",m_pEquipSelectVec->count())->getCString());
	//uint32_t totalprice = atoi(m_pTotalAmount->getString()) - DM_GET_EQUIP_MODEL->getPriceForSell(pEquipCurInfo);	
	//m_pTotalAmount->setString(CCString::createWithFormat("%d" , totalprice)->getCString());
	
	m_uTotalPrice -= DM_GET_EQUIP_MODEL->getPriceForSell(pEquipCurInfo);	
	//m_pTotalAmount->setString(CCString::createWithFormat("%d" , m_uTotalPrice)->getCString());
	m_uTotalPrice < MAX_SHOW_COIN ? m_pTotalAmount->setString(D_CSTRING_FROM_UINT(m_uTotalPrice)) : m_pTotalAmount->setString(D_CSTRING_WITH10K_FROM_UINT(m_uTotalPrice));
}

bool EquipMultiSelect::isEquipSelected(equip::EquipCurInfo* pEquipCurInfo)
{
	if (!pEquipCurInfo)
	{
		return false;
	}

	return m_pEquipSelectVec->containsObject(pEquipCurInfo);
}

cocos2d::CCArray* EquipMultiSelect::getEquipListByGrade(equip::EquipGrade grade)
{
	return m_pEquipList;
}

void EquipMultiSelect::equipListUpdate()
{
	updateEquipList();
	updateTips();

	m_pEquipSelectVec->removeAllObjects();

	EquipMultiSelectView* selectView = (EquipMultiSelectView*)m_pDTabView->getSelectedTab();
	if (!selectView)
	{
		return;
	}

	selectView->updateEquipView();

	//把结算面板也更新了
	m_uTotalPrice = 0;
	m_pSelectCount->setString(CCString::createWithFormat("%d",0)->getCString());
	m_pTotalAmount->setString(CCString::createWithFormat("%d",0)->getCString());
}

void EquipMultiSelect::updateEquipList()
{
	if (!m_pEquipModel->isSynced) 
	{
		return;
	}

	m_pEquipModel->getEquipListByTypeIgnoreID(equip::kEquipTypeALL, 0, *m_pEquipList);
	siftEquiped(m_pEquipList);

	//for (uint32_t i = 0; i < 4; i++)
	//{
	//	cocos2d::CCArray* pCCArray = (cocos2d::CCArray*)m_pEquipMap->objectForKey(i + 1);
	//	if (pCCArray)
	//	{
	//		//m_pEquipModel->getEquipListByGradeIgnoreID(equip::EquipGrade(i + 1), 0, *pCCArray);
	//		m_pEquipModel->getEquipListByTypeIgnoreID(equip::kEquipTypeALL, 0, *pCCArray);
	//		siftEquiped(pCCArray);
	//	}
	//}
}

void EquipMultiSelect::updateTips()
{
	
}

void EquipMultiSelect::siftEquiped(cocos2d::CCArray* pEquipVec)
{
	CCArray* pRemove = CCArray::create();

	CCObject* obj = NULL;
	equip::EquipCurInfo* pEquipCurInfo = NULL;

	CCARRAY_FOREACH(pEquipVec, obj)
	{
		pEquipCurInfo = (equip::EquipCurInfo*)obj;
		equip::EquipType equipType = pEquipCurInfo->getBaseInfo()->getEquipType();
		if (pEquipCurInfo->isEquiped() || (equipType != equip::kEquipTypeWeapon && equipType != equip::kEquipTypeArmor && equipType != equip::kEquipTypeOrnament))
		{
			pRemove->addObject(obj);
		}
	}

	CCARRAY_FOREACH(pRemove, obj)
	{
		pEquipVec->removeObject(obj);
	}
}

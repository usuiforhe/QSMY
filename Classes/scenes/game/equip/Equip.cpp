//
//  Equip.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-12.
//	Modified by cll on 13-4-1
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Equip.h"
#include "../user/UserInfo.h"
#include "EquipView.h"
#include "EquipInfoView.h"
#include "EquipEnhance.h"
#include "EquipRefine.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "EquipMultiSelect.h"
#include "EquipDeletePopup.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define  EQUIP_TAB_RECT		CCRectMake(0, 130, 640, 623)


/************************ equip class begin*****************************/

Equip::Equip()
:m_pContainer(NULL)
,m_pEnhanceNode(NULL)
,m_pDTabView(NULL)
,m_pDecompose(NULL)
,m_pContainerPos(NULL)
,m_pEquipRefine(NULL)
,m_pEquipMultiSelect(NULL)
,m_pEquipBurn(NULL)
,m_pTabViewArray(NULL)
,m_pSellButton(NULL)
,m_pEquipDeletePopup(NULL)
,m_pTableNode(NULL)
,m_pBottom(NULL)
{
    
}

Equip::~Equip()
{
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pEnhanceNode);
	CC_SAFE_RELEASE(m_pDecompose);
	CC_SAFE_RELEASE(m_pContainerPos);
	CC_SAFE_RELEASE(m_pEquipRefine);
	CC_SAFE_RELEASE(m_pEquipMultiSelect);
	CC_SAFE_RELEASE(m_pEquipBurn);
	CC_SAFE_RELEASE(m_pTabViewArray);
	CC_SAFE_RELEASE(m_pSellButton);	
	CC_SAFE_RELEASE(m_pEquipDeletePopup);	
	CC_SAFE_RELEASE(m_pTableNode);
	CC_SAFE_RELEASE(m_pBottom);
}

bool Equip::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	//CCB_READER_AND_ADDCHILD("ui/ccb/equip/equip");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao");

	//m_pDecompose->getButton()->setTarget(this, menu_selector(Equip::decomposeCallBack));
	//m_pSellButton->setTarget(this, menu_selector(Equip::sellCallBack));
	m_pSellButton->setTarget(this, menu_selector(Equip::burnCallBack));

	//user info , on page top
    UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
    addChild(UserInfoView::sharedUserInfoView(),1);
    
    
	m_pContainer = CCNode::create();
	m_pContainer->retain();
	addChild(m_pContainer,2);

	m_pEnhanceNode = CCNode::create();
	m_pEnhanceNode->retain();
	addChild(m_pEnhanceNode,2);
	PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEnhanceNode);

	m_pTabViewArray = CCArray::create();
	m_pTabViewArray->retain();
    
	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getContentSize().height + 9, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainerPos->getPosition().x, posY, m_pContainerPos->getContentSize().width, height),10);
	m_pDTabView->setPadding(2.0);
	m_pDTabView->retain();
    
	std::string tabName[equip::kEquipTypeCount] = {"pl_bb_btn_wenzi_quanbu","pl_bb_btn_wenzi_wuqi","pl_bb_btn_wenzi_kuijia","pl_bb_btn_wenzi_shoushi","pl_bb_btn_wenzi_miji"};
    
	for(uint32_t i = 0; i < equip::kEquipTypeCount; i++)
	{
		EquipView * equipView = EquipView::create((equip::EquipType)i, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainerPos->getContentSize(), -PL_MAX_HEIGHT_DIFF));
		
		equipView->setDelegate(this);
		m_pTabViewArray->addObject(equipView);

		m_pDTabView->addTab(equipView,"","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,CCString::createWithFormat("PL_ui/beibao/%s.png",tabName[i].c_str())->getCString());
	}
    
	showEquipList();

	return true;
}

void Equip::closeInfoView(EquipViewType type)
{
    showEquipList();
}

void Equip::showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param)
{
    switch (type) {
        case kEquipEnhance:
            showEquipEnhance(pEquip);
            break;
        case kEquipInfo:
            showEquipInfo(pEquip);
            break;
		case kEquipRefine:
			showEquipRefine(pEquip);
			break;
		case kEquipDecompose:
			doDecompose(pEquip);
			break;
		case kEquipDeletePopup:
			showDeletePopup();
			break;
        default:
            break;
    }
}

bool Equip::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRefineButton", DButton *, this->m_pDecompose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainerPos);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellButton", CCMenuItemImage *, this->m_pSellButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom", CCNode *, this->m_pBottom);
	return true;
}

void Equip::showEquipList()
{
    m_pDTabView->setVisible(true);
    m_pContainer->removeAllChildrenWithCleanup(false);
	m_pEnhanceNode->removeAllChildrenWithCleanup(false);
    if(m_pDTabView->getParent()) return;
    m_pTableNode->addChild(m_pDTabView);
	
}

void Equip::showEquipInfo(equip::EquipCurInfo* pEquipCurInfo)
{
	m_pEnhanceNode->removeAllChildrenWithCleanup(false);
    //m_pDTabView->setVisible(false);
	//if(m_pEquipInfo == NULL)
	//{
	//	m_pEquipInfo = EquipInfoView::create(EquipInfoView::kTypeEnhanceOnly);
	//	m_pEquipInfo->setDelegate(this);
	//	m_pEquipInfo->retain();
	//}
	EquipInfoView* m_pEquipInfo = EquipInfoView::create(EquipInfoView::kTypeEnhanceOnly);
	m_pEquipInfo->setDelegate(this);

	m_pEquipInfo->setEquip(pEquipCurInfo);
	m_pEnhanceNode->addChild(m_pEquipInfo);
}
void Equip::onGradeSelect(std::vector<equip::EquipGrade> v){
	m_pEquipMultiSelect->onGradeSelect(v);
}
void Equip::showDeletePopup(){
	//m_pContainer->removeAllChildrenWithCleanup(false);
	//m_pDTabView->setVisible(false);

	if (m_pEquipDeletePopup != NULL)
	{
		m_pEquipDeletePopup->removeFromParentAndCleanup(true);
		m_pEquipDeletePopup = NULL;
		CC_SAFE_RELEASE_NULL(m_pEquipDeletePopup);
	}
	
	if(m_pEquipDeletePopup == NULL)
	{
		m_pEquipDeletePopup = EquipDeletePopup::create(EquipDeletePopup::kTypeEnhanceOnly);
		m_pEquipDeletePopup->setDelegate(this);
		m_pEquipDeletePopup->setEquip(m_pEquipMultiSelect->m_pGradeMap);
		m_pContainer->addChild(m_pEquipDeletePopup);
		m_pEquipDeletePopup->retain();
	}	
}

void Equip::showEquipEnhance(equip::EquipCurInfo* pEquipCurInfo)
{
	m_pEnhanceNode->removeAllChildrenWithCleanup(false);
	m_pContainer->removeAllChildrenWithCleanup(false);
    m_pDTabView->setVisible(false);
    
	//if(m_pEquipEnhance == NULL)
	//{
	//	m_pEquipEnhance = EquipEnhance::create();
	//	m_pEquipEnhance->setDelegate(this);
	//	m_pEquipEnhance->retain();
	//}
	
	EquipEnhance* m_pEquipEnhance = EquipEnhance::create();
	m_pEquipEnhance->setDelegate(this);
	m_pEquipEnhance->setEquip(pEquipCurInfo);
	m_pContainer->addChild(m_pEquipEnhance);
	
	//showEquipList();
}

void Equip::showEquipRefine(equip::EquipCurInfo* pEquipCurInfo)
{
	m_pEnhanceNode->removeAllChildrenWithCleanup(false);
	m_pContainer->removeAllChildrenWithCleanup(false);
	m_pDTabView->setVisible(false);

	if (m_pEquipRefine == NULL)
	{
		m_pEquipRefine = EquipRefine::create();
		m_pEquipRefine->setEquipDelegate(this);
		m_pEquipRefine->retain();
	}
	m_pEquipRefine->setEquip(pEquipCurInfo);
	m_pContainer->addChild(m_pEquipRefine);
}

void Equip::doDecompose(equip::EquipCurInfo* pEquipCurInfo)
{
	
}

void Equip::onBackKey()
{
	if (m_pContainer && m_pContainer->getChildrenCount() || m_pEnhanceNode && m_pEnhanceNode->getChildrenCount())
	{
		showEquipList();
		return;
	}
	
	BackKeyInterface::onBackKey();
}

void Equip::decomposeCallBack(cocos2d::CCObject* obj)
{
	m_pContainer->removeAllChildrenWithCleanup(false);
	m_pDTabView->setVisible(false);

	if (m_pEquipMultiSelect == NULL)
	{
		m_pEquipMultiSelect = EquipMultiSelect::create();
		m_pEquipMultiSelect->setEquipMultiSelecteDelegate(this);
		m_pEquipMultiSelect->retain();
	}

	m_pContainer->addChild(m_pEquipMultiSelect);
}

void Equip::burnCallBack(cocos2d::CCObject* obj)
{
	//m_pContainer->removeAllChildrenWithCleanup(false);
	//m_pDTabView->setVisible(false);

	//if (m_pEquipBurn == NULL)
	//{
	//	m_pEquipBurn = EquipBurn::create();
	//	m_pEquipBurn->setEquipBurnCloseDelegate(this);
	//	m_pEquipBurn->retain();
	//}

	//m_pContainer->addChild(m_pEquipBurn);
	Game::sharedGame()->NavigateTo(navigation::kNavEquipBurn);
}

void Equip::sellCallBack(cocos2d::CCObject* obj)
{
	m_pContainer->removeAllChildrenWithCleanup(false);
	m_pDTabView->setVisible(false);

	if (m_pEquipMultiSelect == NULL)
	{
		m_pEquipMultiSelect = EquipMultiSelect::create();
		m_pEquipMultiSelect->setEquipMultiSelecteDelegate(this);
		m_pEquipMultiSelect->retain();
	}

	m_pContainer->addChild(m_pEquipMultiSelect);
}

void Equip::selectedEquips(cocos2d::CCArray* pArray)
{
	if (!pArray)
	{
		m_pContainer->removeAllChildrenWithCleanup(false);
		m_pDTabView->setVisible(true);
	}

	//DM_GET_EQUIP_MODEL->decomposeEquip(pArray);
	DM_GET_EQUIP_MODEL->sellEquip(pArray);
}

void Equip::onBurnClose()
{
	m_pContainer->removeAllChildrenWithCleanup(false);
	m_pDTabView->setVisible(true);
}



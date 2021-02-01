//
//  TowerInteral.cpp
//  千机楼积分排行
//
//  Created by mzp on 14-8-19.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "TowerInteral.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"


#define TOWER_INTERAL_SCROLL_SIZE CCSizeMake(638.f, 580.f)
#define TOWER_INTERAL_CELL_SIZE	CCSizeMake(618.f, 135.f)


TowerInteral::TowerInteral()
	: m_pTowerModel(NULL)
	, m_pDelegate(NULL)
	, m_pCCData(NULL)
	, m_pRewardList(NULL)
	, m_pView(NULL)
	, m_tabViewLayer(NULL)
	, m_pNum(NULL)
{

}

TowerInteral::~TowerInteral()
{
	CC_SAFE_RELEASE(m_pCCData);
	CC_SAFE_RELEASE(m_pRewardList);
	CC_SAFE_RELEASE(m_tabViewLayer);
	CC_SAFE_RELEASE(m_pNum);
}

bool TowerInteral::init()
{
	if (!DLayer::init())
	{
		return false;
	}

	m_pTowerModel = DM_GET_TOWER_MODEL;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_pointAward");
	m_pCCData = readCCData("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_wupin_cell");
	CC_SAFE_RETAIN(m_pCCData);

	m_pView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_tabViewLayer->getContentSize(), -50));
	m_pView->setDirection(kCCScrollViewDirectionVertical);
	m_pView->setDelegate(this);
	m_pView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_tabViewLayer->addChild(m_pView);

	// 获取积分奖励数据
	m_pRewardList = CCArray::create();
	CC_SAFE_RETAIN(m_pRewardList);
	m_pTowerModel->getExchangeArray(m_pRewardList);

	return true;
}

void TowerInteral::onEnter()
{
	DLayer::onEnter();

	m_pView->reloadData();

	BINDING_EXEC(m_pTowerModel, this, TowerInteral::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerInteral::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);

	DLayer::onExit();
}

bool TowerInteral::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tabViewLayer",	CCNode *,			m_tabViewLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNum",			CCLabelBMFont *,	m_pNum);

	return false;
}

cocos2d::SEL_MenuHandler TowerInteral::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchBack", TowerInteral::onTouchBack);

	return NULL;
}

void TowerInteral::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{

}

cocos2d::CCSize TowerInteral::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return TOWER_INTERAL_CELL_SIZE;
}

cocos2d::extension::CCTableViewCell* TowerInteral::tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx)
{
	TowerInteralCell* cell = (TowerInteralCell*)table->dequeueCell();
	if (!cell)
	{
		cell = TowerInteralCell::create(m_pCCData);
	}
	
	cell->setDelegate(m_pDelegate);
	if (idx < m_pRewardList->count())
	{
		cell->setTowerExchangeInfo((tower::TowerExchangeInfo*)m_pRewardList->objectAtIndex(idx));
	}
	else
	{
		cell->setTowerExchangeInfo(NULL);
	}

	return cell;
}

uint32_t TowerInteral::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	if (!m_pRewardList)
	{
		return 0;
	}
	
	return m_pRewardList->count();
}

void TowerInteral::onTouchBack(CCObject* sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->ClosePopup();
	}
}

void TowerInteral::NotificationUpdate()
{
	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}

	// 积分
	m_pNum->setString(CCString::createWithFormat("%d", pInfo->points)->getCString());
}




TowerInteralCell::TowerInteralCell()
	: m_pDelegate(NULL)
	, m_pTowerExchangeInfo(NULL)
	, m_pContainer(NULL)
	, m_pGradeOrange(NULL)
	, m_pName(NULL)
	, m_pNum(NULL)
	, m_pButton(NULL)
	//, m_pImgDress(NULL)
	, m_pImgFrag(NULL)
	, m_pImgHunpo(NULL)
	, m_pInteral(NULL)
	,m_pGradeGreen(NULL)
	,m_pGradeBlue(NULL)
	,m_pGradePurple(NULL)
{

}

TowerInteralCell::~TowerInteralCell()
{
	CC_SAFE_RELEASE(m_pTowerExchangeInfo);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pNum);
	CC_SAFE_RELEASE(m_pButton);
	//CC_SAFE_RELEASE(m_pImgDress);
	CC_SAFE_RELEASE(m_pImgFrag);
	CC_SAFE_RELEASE(m_pImgHunpo);
	CC_SAFE_RELEASE(m_pInteral);
	CC_SAFE_RELEASE(m_pGradeGreen);
	CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradePurple);
}

TowerInteralCell* TowerInteralCell::create(CCData* pCCData)
{
	TowerInteralCell *pRet = new TowerInteralCell();
	if (pRet && pRet->init(pCCData))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);

	return NULL;
}

bool TowerInteralCell::init(CCData* pCCData)
{
	CCB_READER_DATA_AND_ADDCHILD(pCCData);

	m_pButton->getButton()->setTarget(this, menu_selector(TowerInteralCell::menuCallback));

	return true;
}

bool TowerInteralCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pGradeOrange);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNum", CCLabelBMFont *, this->m_pNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgDress", CCNode *, this->m_pImgDress);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPieceIcon", CCNode *, this->m_pImgFrag);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgHunpo", CCNode *, this->m_pImgHunpo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInteral", CCLabelBMFont *, this->m_pInteral);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", CCNode *, this->m_pGradeGreen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pGradePurple);

	return false;
}

cocos2d::SEL_MenuHandler TowerInteralCell::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", TowerInteralCell::viewCallback);

	return NULL;
}

void TowerInteralCell::setTowerExchangeInfo(tower::TowerExchangeInfo* pTowerExchangeInfo)
{
	CC_SAFE_RETAIN(pTowerExchangeInfo);
	CC_SAFE_RELEASE(m_pTowerExchangeInfo);
	m_pTowerExchangeInfo = pTowerExchangeInfo;
	updateView();
}

void TowerInteralCell::menuCallback(cocos2d::CCObject* sender)
{
	if (!m_pTowerExchangeInfo)
	{
		return;
	}

	tower::TowerInfo* pInfo = DM_GET_TOWER_MODEL->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}

	// 积分不足
	if (pInfo->points < m_pTowerExchangeInfo->getNeedPoint())
	{
		TIPSMANAGER->alert(D_LOCAL_STRING("TOWERPOINTSNOTENOUGH"));
		return;
	}
	
	HTTP_CLIENT->towerReward(m_pTowerExchangeInfo->getID());
}

void TowerInteralCell::viewCallback(cocos2d::CCObject* sender)
{
	if (!m_pTowerExchangeInfo)
	{
		return;
	}

	switch (m_pTowerExchangeInfo->getType())
	{
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		//m_pDelegate->showItemInfo(item::ItemInfo::create(m_pTowerExchangeInfo->getItemID(), 0));
		break;
	case drop::kDropTypeEquip:
		break;
	/*case drop::kDropTypeGundam:
	case drop::kDropTypeGundamFrag:
		m_pDelegate->showInfoView(GundamDelegate::kGundamInfo, gundam::GundamInfo::create(m_pTowerExchangeInfo->getItemID()), GundamInfoView::kTypeCloseOnly);
		break;*/
	}
}

void TowerInteralCell::updateView()
{
	if (!m_pTowerExchangeInfo)
	{
		return;
	}
	
	CCSprite* m_pItemSprite = NULL;
	std::string sName = "";

	//m_pImgDress->setVisible(false);
	m_pImgFrag->setVisible(false);
	m_pImgHunpo->setVisible(false);
	int itemGrade = 0;
	unsigned int itemID = m_pTowerExchangeInfo->getItemID();
	switch (m_pTowerExchangeInfo->getType())
	{
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getItemIcon50PForList(itemID);
		sName = DM_GET_ITEM_MODEL->getItemBaseByID(itemID)->getItemName().c_str();
		itemGrade = (int)DM_GET_ITEM_MODEL->getItemBaseByID(itemID)->getItemGrade();
		break;
	case drop::kDropTypeEquip:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getEquipIcon50PForList(itemID);
		sName = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(itemID)->getEquipName().c_str();
		itemGrade = (int)DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(itemID)->getEquipGrade();
		break;
	case drop::kDropTypeSoul:
		m_pImgHunpo->setVisible(true);
		m_pItemSprite = ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(itemID);
		sName = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(itemID)->getWarriorName().c_str();
		itemGrade = (int)DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(itemID)->getColorGrade();
		break;
	case drop::kDropTypeWarrior:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(itemID);
		sName = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(itemID)->getWarriorName().c_str();
		itemGrade = (int)DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(itemID)->getColorGrade();
		break;
	case drop::kDropTypeCoin:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getItemIcon50PForList(COIN_ID);
		sName = DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID)->getItemName().c_str();
		break;
	case drop::kDropTypePoint:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getItemIcon50PForList(POINT_ID);
		sName = DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID)->getItemName().c_str();
		break;
	case drop::kDropTypeAvatar:
		//m_pImgDress->setVisible(true);
		m_pItemSprite = ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(itemID);
		sName = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(itemID)->getName().c_str();
		break;
	/*case drop::kDropTypeAlchemy:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getAlchemyIcon50P(itemID);
		sName = DM_GET_ALCHEMY_MODEL->getAlchemyBaseInfoByID(itemID)->getAlchemyName().c_str();
		m_pGradeBg->setVisible(true);
		m_pGradeBg->setColor(DM_GET_ALCHEMY_MODEL->getAlchemyColorByGrade(DM_GET_ALCHEMY_MODEL->getAlchemyBaseInfoByID(itemID)->getAlchemyGrade()));
		break;
	case drop::kDropTypeChiNum:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getItemIcon50PForList(CHINUM_ID);
		sName = DM_GET_ITEM_MODEL->getItemBaseByID(CHINUM_ID)->getItemName().c_str();
		break;
	case drop::kDropTypeGundam:
		m_pItemSprite = ResourceManager::sharedResourceManager()->getGundamIcon50PForList(itemID);
		sName = DM_GET_GUNDAM_MODEL->getGundamBaseInfoByConfigId(itemID)->getGName().c_str();
		m_pGradeBg->setVisible(true);
		m_pGradeBg->setColor(DM_GET_GUNDAM_MODEL->getGundamBaseInfoByConfigId(itemID)->getGradeInfo()->getColor());
		break;
	case drop::kDropTypeGundamFrag:
		m_pImgFrag->setVisible(true);
		m_pItemSprite = ResourceManager::sharedResourceManager()->getGundamIcon50PForList(itemID);
		sName = DM_GET_GUNDAM_MODEL->getGundamBaseInfoByConfigId(itemID)->getGName();
		m_pGradeBg->setVisible(true);
		m_pGradeBg->setColor(DM_GET_GUNDAM_MODEL->getGundamBaseInfoByConfigId(itemID)->getGradeInfo()->getColor());
		break;*/
	case drop::kDropTypeNone:
	default:
		break;
	}

	item::ItemGrade grade = static_cast<item::ItemGrade>(itemGrade);
	m_pGradeGreen->setVisible(grade == item::kItemGradeGreen);
	m_pGradeBlue->setVisible(grade == item::kItemGradeBlue);
	m_pGradePurple->setVisible(grade == item::kItemGradePurple);
	m_pGradeOrange->setVisible(grade == item::kItemGradeRed);
	

	m_pContainer->removeAllChildren();
	m_pContainer->addChild(m_pItemSprite);

	m_pName->setString(sName.c_str());
	m_pNum->setString(CCString::createWithFormat("x%d", m_pTowerExchangeInfo->getNum())->getCString());

	m_pInteral->setString(CCString::createWithFormat("x%d", m_pTowerExchangeInfo->getNeedPoint())->getCString());
}

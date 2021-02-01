//
//  PvpExchangeView.cpp
//  兑换奖品列表
//
//  Created by cll on 13-4-20
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PvpExchangeView.h"
#include "Pvp.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace drop;


#define PVP_EXTRA_HEIGHT_UP	90
#define	PVP_EXTRA_HEIGHT_DN	240
#define OFFSET_DEFAULT ccp(0,-260)

/************************ pvp exchange select class begin*******************/



PvpExchangeView::PvpExchangeView()
:m_pListView(NULL)
,m_pDelegate(NULL)
,m_pRealAwards(NULL)
,m_pData(NULL)
,m_pCell(NULL)
,m_pContainer(NULL)
,m_pBMFPvpPoints(NULL)
,m_pDBtnRefresh(NULL)
,m_pBMFCD(NULL)
,m_pBMFItemCount(NULL)
,m_pBMFPvpNeedPoints(NULL)
,m_pLevel(NULL)
,m_pUpBox(NULL)
,m_pMaxLvlImg(NULL)
,m_bTipsShown(false)
{
    
}

PvpExchangeView::~PvpExchangeView()
{
    CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pRealAwards);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pBMFPvpPoints);
	CC_SAFE_RELEASE(m_pDBtnRefresh);
	CC_SAFE_RELEASE(m_pBMFCD);
	CC_SAFE_RELEASE(m_pBMFItemCount);
	CC_SAFE_RELEASE(m_pBMFPvpNeedPoints);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pUpBox);
	CC_SAFE_RELEASE(m_pMaxLvlImg);
}


bool PvpExchangeView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
	m_pRealAwards = PvpModel::AwardArray::create();
	CC_SAFE_RETAIN(m_pRealAwards);
    
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_lunjian/PL_lunjian_duihuan");

    m_pData = readCCData("PL_ui/ccb/pl_lunjian/PL_lunjian_duihuan_cell");
    CC_SAFE_RETAIN(m_pData);

	CCB_READER_DATA(m_pData, m_pCell);
	CC_SAFE_RETAIN(m_pCell);
	
	m_pListView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));//CCSizeMake(PVP_LIST_RECT.size.width,PVP_LIST_RECT.size.height));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	m_pContainer->addChild(m_pListView);
    
	m_pDBtnRefresh->getButton()->setTarget(this, menu_selector(PvpExchangeView::refreshCallBack));

	m_pBMFCD->setString("00:00:00");
	return true;

}

void PvpExchangeView::onEnter()
{
	scheduleUpdate();
	DLayer::onEnter();
	BINDING_EXEC(DM_GET_PVP_MODEL, this, PvpExchangeView::updateView, D_EVENT_PVP_EXMALL_UPDATE);
    BINDING_EXEC(DM_GET_PVP_MODEL, this, PvpExchangeView::updateView, D_EVENT_PVP_UPDATE_AWARDLIST);
}

void PvpExchangeView::onExit()
{
	unscheduleUpdate();
	DLayer::onExit();
    UNBINDING_ALL(DM_GET_PVP_MODEL, this);
    
	m_pRealAwards->removeAllObjects();
//     m_pListView->reloadData();
//     m_pListView->releaseCellsFreed();
}

void PvpExchangeView::updateView()
{
    if(!DM_GET_PVP_MODEL->isSynced)
    {
        return;
    }
    
	m_pRealAwards->removeAllObjects();

    pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();
    CCArray * pAwards = DM_GET_PVP_MODEL->getArrChMall();
    cocos2d::CCObject * pObj(NULL);
    CCARRAY_FOREACH(pAwards, pObj)
    {
		pvp::ChMallItem *pItem = (pvp::ChMallItem *)(pObj);
		if (pItem->getItemNum() > 0)
		{
			if(pItem->getStatus() == false)
			{
				//1兑换过，0未兑换
				m_pRealAwards->addObject(pObj);
			}
		}
		
//         pvp::AwardInfo * pAward = (pvp::AwardInfo *)pObj;
//         if(!info->isAwardDrawn(pAward->getId()))
//         {
//             m_pAwards->addObject(pAward);
//         }
    }
    
	m_pListView->reloadData();
    //m_pListView->setContentOffset(m_pListView->minContainerOffset());
	//m_pListView->setContentOffset(OFFSET_DEFAULT);
	m_pBMFPvpPoints->setString(D_CSTRING_FROM_UINT(info->getPoint()));

	//商店等级
	m_pLevel->setString(D_CSTRING_FROM_UINT(DM_GET_PVP_MODEL->getMallLv()));
	int needPoint = DM_GET_PVP_MODEL->getMallLvInfo(DM_GET_PVP_MODEL->getMallLv());

	if(needPoint<0)
	{
		m_pMaxLvlImg->setVisible(true);
		m_pUpBox->setVisible(false);
	}
	else
	{
		m_pMaxLvlImg->setVisible(false);
		m_pUpBox->setVisible(true);
		int exchangePoint = DM_GET_PVP_MODEL->getExchangePoint();
		m_pBMFPvpNeedPoints->setString(D_CSTRING_FROM_UINT(needPoint-exchangePoint));
	}

	item::ItemInfo * pFreshItem = DM_GET_ITEM_MODEL->getItemInfoByID(D_CONFIG_UINT_FOR_KEY("SHUAXINLING_ITEM"));
	uint32_t num = pFreshItem ? pFreshItem->getNum() : 0;
	m_pBMFItemCount->setString(D_CSTRING_FROM_UINT(num));
}

void PvpExchangeView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	uint32_t idx =  cell->getIdx();

	CCLOG("exchange cell touched at index: %i", idx);
}

CCSize PvpExchangeView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
// 	if (idx == 0)
// 	{
// 		return ccp(m_pCell->getContentSize().width, m_pCell->getContentSize().height + PVP_EXTRA_HEIGHT_UP);
// 	}
// 	else if (idx == m_pAwards->count())
// 	{
// 		return ccp(m_pCell->getContentSize().width, PVP_EXTRA_HEIGHT_DN);
// 	}
// 	else
	
	return m_pCell->getContentSize();
}

CCTableViewCell* PvpExchangeView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{

//	PvpExchangeViewCell *cell = (PvpExchangeViewCell*)table->dequeueCell();
//	if (!cell)
//	{
//		cell = PvpExchangeViewCell::create(m_pData);
//	}
//
//	cell->setVisible(idx != m_pAwards->count());
//	if (idx != m_pAwards->count())
//	{
//		cell->setDelegate(m_pDelegate);
//		cell->setData((pvp::ChMallItem *)m_pAwards->objectAtIndex(idx));
////		cell->setAwardInfo((pvp::AwardInfo *)m_pAwards->objectAtIndex(idx));
//	}
//
//	return cell;

	PvpExchangeViewCell *cell = (PvpExchangeViewCell*)table->dequeueCell();
	if (!cell)
	{
		cell = PvpExchangeViewCell::create(m_pData);
	}

	cell->setDelegate(m_pDelegate);
	//cell->setData((pvp::ChMallItem *)m_pAwards->objectAtIndex(idx));
	cell->setData((pvp::ChMallItem *)m_pRealAwards->objectAtIndex(idx));

	return cell;
}

uint32_t PvpExchangeView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pRealAwards->count();
}

bool PvpExchangeView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPvpPoints", CCLabelBMFont *, m_pBMFPvpPoints);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnRefresh", DButton *, m_pDBtnRefresh);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCD", CCLabelBMFont *, m_pBMFCD);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFItemCount", CCLabelBMFont *, m_pBMFItemCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont*,m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPvpNeedPoints", CCLabelBMFont*,m_pBMFPvpNeedPoints);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMaxLvlImg" , CCNode*,m_pMaxLvlImg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpBox" , CCNode*,m_pUpBox);
	return false;
}

void PvpExchangeView::refreshCallBack(CCObject *pObj)
{
	//判断是否需要消耗元宝
	item::ItemInfo * pFreshItem = DM_GET_ITEM_MODEL->getItemInfoByID(D_CONFIG_UINT_FOR_KEY("SHUAXINLING_ITEM"));
	if(m_bTipsShown == false && pFreshItem == NULL)
	{
		//需要消耗元宝 没有刷新令	
		DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("pvpshoppresstips").c_str(), this, messageBox_callfunc_selector(PvpExchangeView::onMessageBoxComfirm));
	}
	else
	{
		HTTP_CLIENT->challengeRefresh();
	}
}

void PvpExchangeView::onMessageBoxComfirm(MessageType type,DMessageBox*target)
{
	if(type == kMessageTypeOk)
	{
		m_bTipsShown = true;	
		HTTP_CLIENT->challengeRefresh();
	}
}

void PvpExchangeView::update(float dt)
{
	uint32_t ts = DM_GET_PVP_MODEL->getRefreshTs();
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	uint32_t cd = (D_CONFIG_UINT_FOR_KEY("PVP_REFRESH_CD")) * 60;//读表返回的是分钟数，
	if (ts + cd < now)
	{
		m_pBMFCD->setString("00:00:00");
		unscheduleUpdate();
		return;
	}
	
	m_pBMFCD->setString(TimeUtil::getSeconds2String(ts + cd - now));
}

/************************ pvp exchange select class end*******************/

/************************ pvp exchange cell class begin**********************/

PvpExchangeViewCell::~PvpExchangeViewCell()
{
	CC_SAFE_RELEASE(m_pAwardInfo);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pWarriorContainer);
	CC_SAFE_RELEASE(m_pExchangeContainer);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pNum);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pButton);
	CC_SAFE_RELEASE(m_pViewButton);
	CC_SAFE_RELEASE(m_pFontExchange);
	CC_SAFE_RELEASE(m_pFontDraw);
	CC_SAFE_RELEASE(m_pBMFNeedPoints);
	CC_SAFE_RELEASE(m_pData);
	D_SAFE_RELEASE_C_ARRAY(m_pNodeGrade, PvpExGradeMax);
	CC_SAFE_RELEASE(m_pIconSoul);
	CC_SAFE_RELEASE(m_pIconPiece);
	CC_SAFE_RELEASE(m_pTxtRank);
	CC_SAFE_RELEASE(m_pTxtPoint);
	CC_SAFE_RELEASE(m_pAmountNode);
	CC_SAFE_RELEASE(m_pNeed);
	CC_SAFE_RELEASE(m_pHave);
}

PvpExchangeViewCell* PvpExchangeViewCell::create(cocos2d::extension::CCData* pData)
{
    PvpExchangeViewCell* pRet = new PvpExchangeViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool PvpExchangeViewCell::init(cocos2d::extension::CCData* pData)
{    
    CCB_READER_DATA_AND_ADDCHILD(pData);
	m_pButton->getButton()->setTarget(this, menu_selector(PvpExchangeViewCell::menuCallback));
	m_pViewButton->setTarget(this, menu_selector(PvpExchangeViewCell::viewCallback));
	
	return CCTableViewCell::init();
}

bool PvpExchangeViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorContainer", CCNode *, this->m_pWarriorContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExchangeContainer", CCNode *, this->m_pExchangeContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNum", CCLabelBMFont *, this->m_pNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton *, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pViewButton", CCMenuItem *, this->m_pViewButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFontExchange", CCSprite *, this->m_pFontExchange);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFontDraw", CCSprite *, this->m_pFontDraw);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNeedPoints", CCLabelBMFont *, this->m_pBMFNeedPoints);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", CCNode *, this->m_pNodeGrade[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pNodeGrade[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pNodeGrade[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pNodeGrade[3]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconSoul", CCNode *, this->m_pIconSoul);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconPiece", CCNode *, this->m_pIconPiece);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtRank", CCNode *, this->m_pTxtRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtPoint", CCNode *, this->m_pTxtPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNeed", CCLabelBMFont *, this->m_pNeed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHave", CCLabelBMFont *, this->m_pHave);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmountNode",  CCNode*, this->m_pAmountNode);
    return false;
}

cocos2d::SEL_MenuHandler PvpExchangeViewCell::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{    
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", PvpExchangeViewCell::viewCallback);
    return NULL;
}

void PvpExchangeViewCell::setData(pvp::ChMallItem *pData)
{
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RETAIN(pData);
	m_pData = pData;

	updateView();
}

void PvpExchangeViewCell::updateView()
{
	m_pBMFNeedPoints->setString(D_CSTRING_FROM_UINT(m_pData->getCost()));
	m_pNum->setString(D_CSTRING_FROM_UINT(m_pData->getItemNum()));

	/************************************************************************/
	/* 新增逻辑，积分购买or点券购买			                                                */
	/************************************************************************/
	m_pTxtRank->setVisible(m_pData->getBuyType() == pvp::ChMallItem::kBuyTypeRank);
	m_pTxtPoint->setVisible(m_pData->getBuyType() == pvp::ChMallItem::kBuyTypePoint);

	pvp::ChallengeInfo * info = DM_GET_PVP_MODEL->getChallengeInfo();

//	m_pButton->getButton()->setEnabled(info->getPoint() >= m_pData->getCost());
	m_pContainer->removeAllChildrenWithCleanup(true);
	m_pWarriorContainer->removeAllChildrenWithCleanup(true);
	m_pIconSoul->setVisible(false);
	m_pIconPiece->setVisible(false);
	m_pAmountNode->setVisible(false);

	uint32_t grade = 0;

	switch (m_pData->getItemType())
	{
	case kDropTypeItem:
	case kDropTypePkg:
		{
			item::ItemBase *pBase = DM_GET_ITEM_MODEL->getItemBaseByID(m_pData->getItemID());
			if(pBase->getItemType() == item::kItemTypeFragment){
				//碎片
				m_pIconPiece->setVisible(true);
				m_pAmountNode->setVisible(true);
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50PForList(pBase->getResID()));
			
				item::ItemInfo* iteminfo = DM_GET_ITEM_MODEL->getItemInfoByID(m_pData->getItemID());				
				m_pHave->setString(D_CSTRING_FROM_UINT(iteminfo == NULL ? 0 : iteminfo->getNum()));
				m_pNeed->setString(D_CSTRING_FROM_UINT(pBase->getUseItemNum()));
			}
			else{
				m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50PForList(m_pData->getItemID()));
			}
			if (pBase->isNeedShowGradeColor())
			{
				grade = (uint32_t)pBase->getItemGrade();
			}
			
			m_pName->setString(DM_GET_ITEM_MODEL->getItemBaseByID(m_pData->getItemID())->getItemName().c_str());
			m_pDesc->setString(DM_GET_ITEM_MODEL->getItemBaseByID(m_pData->getItemID())->getItemDesc().c_str());
			
			break;
		}
	case kDropTypeWarrior:
		{
			m_pIconSoul->setVisible(false);
			warrior::WarriorBaseInfo *pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pData->getItemID());
			grade = (uint32_t)pBase->getColorGrade();
			m_pName->setString(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pData->getItemID())->getWarriorName().c_str());
			m_pWarriorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pData->getItemID()));
			m_pDesc->setString(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pData->getItemID())->getWarriorDesc().c_str());
			break;
		}
	case kDropTypeSoul:
		{
			m_pIconSoul->setVisible(true);
			m_pAmountNode->setVisible(true);			

			warrior::WarriorBaseInfo *pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pData->getItemID());
			grade = (uint32_t)pBase->getColorGrade();
			m_pWarriorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pData->getItemID()));
			//m_pDesc->setString(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pData->getItemID())->getWarriorDesc().c_str());
			CCString *pName = CCString::createWithFormat("%s%s", 
				DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pData->getItemID())->getWarriorName().c_str(), 
				D_LOCAL_STRING("Soul").c_str());
			m_pName->setString(pName->getCString());
			m_pDesc->setString(pName->getCString());

			warrior::SoulInfo* soulinfo = DM_GET_WARRIOR_MODEL->getSoulInfoByWID(m_pData->getItemID());
			warrior::WarriorGrowInfo * pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(m_pData->getItemID());
			m_pHave->setString(D_CSTRING_FROM_UINT(soulinfo == NULL ? 0 : soulinfo->getNum()));
			m_pNeed->setString(D_CSTRING_FROM_UINT(pGrowInfo->getRctSoul()));

			break;
		}
	case kDropTypeEquip:
		{
			equip::EquipBaseInfo *pBase = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pData->getItemID());
			grade = (uint32_t)pBase->getEquipGrade();
			m_pName->setString(DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pData->getItemID())->getEquipName().c_str());
			m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50PForList(m_pData->getItemID()));
			m_pDesc->setString(DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(m_pData->getItemID())->getEquipDesc().c_str());
			break;
		}
	case kDropTypeAvatar:
		{
			m_pName->setString(DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(m_pData->getItemID())->getName().c_str());
			m_pWarriorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pData->getItemID()));
			m_pDesc->setString(DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(m_pData->getItemID())->getDesc().c_str());
			break;
		}
	case kDropTypeCoin:
		{
			m_pName->setString(D_LOCAL_STRING("Coin").c_str());
			m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(COIN_ID));
			m_pDesc->setString(DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID)->getItemDesc().c_str());
			break;
		}
	case kDropTypePoint:
		{
			m_pName->setString(D_LOCAL_STRING("Point").c_str());
			m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(POINT_ID));
			m_pDesc->setString(DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID)->getItemDesc().c_str());
			break;
		}
	case kDropTypeVip:
		{
			user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(m_pData->getItemID());
			item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());
			m_pName->setString(pItemBase->getItemName().c_str());
			m_pDesc->setString(pItemBase->getItemDesc().c_str());
			m_pContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(pItemBase->getItemId()));
			break;
		}
	case kDropTypeNone:
	default:;
	}
	if (grade == 0)
	{
		grade = 1;
	}
	
	for (int i = 0; i < PvpExGradeMax; i++)
	{
		m_pNodeGrade[i]->setVisible(i + 1 == grade);
	}

	m_pDesc->setVisible(!m_pAmountNode->isVisible());
	
}

void PvpExchangeViewCell::menuCallback(cocos2d::CCObject *sender)
{
	//元宝购买2次确认 积分的直接买
	if(m_pData->getBuyType() == pvp::ChMallItem::kBuyTypePoint)
	{		
		DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("pvpshopbuytips").c_str(), this, messageBox_callfunc_selector(PvpExchangeViewCell::onMessageBoxComfirm));
	}
	else
	{
		uint32_t id = m_pData->getID();
		HTTP_CLIENT->challengeExchange(id);	
	}
}

void PvpExchangeViewCell::onMessageBoxComfirm(MessageType type,DMessageBox*target)
{
	if(type == kMessageTypeOk)
	{
		uint32_t id = m_pData->getID();
		HTTP_CLIENT->challengeExchange(id);
	}
}

void PvpExchangeViewCell::viewCallback(cocos2d::CCObject *sender)
{
	m_pDelegate->showAwardInfo(m_pData);
//    m_pDelegate->showInfoView(kItemInfo, item::ItemInfo::create(m_pAwardInfo->getItemId(), 0));
}

/************************ pvp exchange cell class end**********************/





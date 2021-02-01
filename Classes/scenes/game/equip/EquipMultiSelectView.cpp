//
//  EquipMultiSelectView.cpp
//  装备多选列表
//  Created by mzp on 14-04-12.
//  Copyright (c) 2013, thedream. All rights reserved.
//
#include "EquipMultiSelectView.h"
#include "EquipView.h"
#include "../../../managers/GameManager.h"
#include "EquipMultiSelect.h"
#include "../../../model/EquipModel.h"


#define  EQUIP_CELL_SIZE        CCSizeMake(640, 128)



EquipMultiSelectView* EquipMultiSelectView::create(equip::EquipGrade equipGrade, const cocos2d::CCSize& size)
{
	EquipMultiSelectView* pRet = new EquipMultiSelectView();
	if (pRet && pRet->init(equipGrade, size))
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

EquipMultiSelectView* EquipMultiSelectView::create(equip::EquipType equipType, const cocos2d::CCSize& size)
{
	EquipMultiSelectView* pRet = new EquipMultiSelectView();
	if (pRet && pRet->init(equipType, size))
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

EquipMultiSelectView::EquipMultiSelectView()
	: m_equipGrade(equip::kEquipGradeWhite)
	, m_equipType(equip::kEquipTypeALL)
	, m_pListView(NULL)
	, m_pData(NULL)
	, m_pEquipMultiSelect(NULL)
	, m_pDataList(NULL)
{

}

EquipMultiSelectView::~EquipMultiSelectView()
{
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pDataList);
}

bool EquipMultiSelectView::init(equip::EquipType equipType, const cocos2d::CCSize& size)
{
	m_equipType = equipType;

	m_pDataList = cocos2d::CCArray::create();
	CC_SAFE_RETAIN(m_pDataList);

	return EquipMultiSelectView::init(size);
}

bool EquipMultiSelectView::init(equip::EquipGrade equipGrade, const cocos2d::CCSize& size)
{
	m_equipGrade = equipGrade;
	return EquipMultiSelectView::init(size);
}

bool EquipMultiSelectView::init(const cocos2d::CCSize& size)
{
	if (!DLayer::init())
	{
		return false;
	}

	//m_equipGrade = equipGrade;


	//m_pData = readCCData("ui/ccb/equip/equipListSelectCell");
	m_pData = readCCData("PL_ui/ccb/pl_beibao/PL_beibao_wupin_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pListView = DTableView::create(this, size);
	m_pListView->setFlyInStyle(kCCTableViewFlyInBoth);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	//m_pListView->setPosition(EQUIP_LIST_POS);

	this->addChild(m_pListView);

	return true;
}

void EquipMultiSelectView::onEnter()
{
	DLayer::onEnter();
	updateEquipView();
}

void EquipMultiSelectView::onExit()
{

}

void EquipMultiSelectView::setSelectedCell(EquipSelectViewCell* cell)
{
	CCLOG("equip cell touched at index: %i", cell->getIdx());
	//m_selectIndex = m_selectIndex == cell->getIdx() ?UNSELECT :cell->getIdx();

	EquipSelectViewCell* equipCell = dynamic_cast<EquipSelectViewCell*>(cell);
	if (!equipCell)
	{
		return;
	}

	if(m_equipGrade == equip::kEquipGradeWhite)
	{
		if(DM_GET_BURN_MODEL->isSelected(equipCell->getEquipInfo()->getId() , BurnModel::kBurnTypeEquip))
		{
			DM_GET_BURN_MODEL->removeFromList(equipCell->getEquipInfo()->getId() , BurnModel::kBurnTypeEquip);
		}
		else
		{
			if(DM_GET_BURN_MODEL->getEquipList()->count() >= TOTAL_BURN_NUM)
			{
				Tips_Alert(D_LOCAL_STRING("EquipRefineTip4").c_str());
			}
			else
			{
				DM_GET_BURN_MODEL->addToList(equipCell->getEquipInfo()->getId() , BurnModel::kBurnTypeEquip);
			}			
		}
	}
	else
	{


		if (m_pEquipMultiSelect->isEquipSelected(equipCell->getEquipInfo()))
		{
			m_pEquipMultiSelect->unselectEquip(equipCell->getEquipInfo());
		}
		else
		{
			m_pEquipMultiSelect->selectEquip(equipCell->getEquipInfo());
		}
	}
	
	updateEquipView(false);
}

cocos2d::CCSize EquipMultiSelectView::tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	return EQUIP_CELL_SIZE;
}

void EquipMultiSelectView::tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell)
{

}

cocos2d::extension::CCTableViewCell* EquipMultiSelectView::tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	if(m_equipGrade == equip::kEquipGradeWhite)
	{
		EquipSelectViewCell* cell = (EquipSelectViewCell*)table->dequeueCell();
		if (!cell)
		{
			cell = EquipSelectViewCell::create(m_pData);
		}

		equip::EquipCurInfo* pEquipCurInfo = dynamic_cast<equip::EquipCurInfo*>(m_pDataList->objectAtIndex(idx));
		cell->setEquipInfo(pEquipCurInfo);
		bool selected = DM_GET_BURN_MODEL->isSelected(pEquipCurInfo->getId() , BurnModel::kBurnTypeEquip);
		cell->setSelected(selected);
		cell->setDelegate(this);

		return cell;
	}
	else
	{
		if (!m_pEquipMultiSelect)
		{
			return EquipSelectViewCell::create(m_pData);
		}

		cocos2d::CCArray* pEquipList = m_pEquipMultiSelect->getEquipListByGrade(m_equipGrade);
		if (!pEquipList)
		{
			return EquipSelectViewCell::create(m_pData);
		}

		EquipSelectViewCell* cell = (EquipSelectViewCell*)table->dequeueCell();
		if (!cell)
		{
			cell = EquipSelectViewCell::create(m_pData);
		}

		equip::EquipCurInfo* pEquipCurInfo = dynamic_cast<equip::EquipCurInfo*>(pEquipList->objectAtIndex(idx));
		cell->setEquipInfo(pEquipCurInfo);
		bool selected = m_pEquipMultiSelect->isEquipSelected(pEquipCurInfo);
		cell->setSelected(selected);
		cell->setDelegate(this);

		return cell;
	}
	
}

unsigned int EquipMultiSelectView::numberOfCellsInTableView(cocos2d::extension::CCTableView* table)
{
	unsigned int count = 0;
	if(m_equipGrade == equip::kEquipGradeWhite)
	{
		if (!m_pDataList)
		{
			count = 0;
		}
		count = m_pDataList->count();
	}
	else
	{
		if (!m_pEquipMultiSelect)
		{
			count = 0;
		}
		else
		{
			cocos2d::CCArray* pEquipList = m_pEquipMultiSelect->getEquipListByGrade(m_equipGrade);
			if (!pEquipList)
			{
				count = 0;
			}
			count = pEquipList->count();
		}		
	}
	return count;
}

void EquipMultiSelectView::setEquipGrade(equip::EquipGrade equipGrade)
{
	m_equipGrade = equipGrade;
}

equip::EquipGrade EquipMultiSelectView::getEquipGrade() const
{
	return m_equipGrade;
}

void EquipMultiSelectView::updateEquipView(bool fly)
{
	if(fly) m_pListView->setFlyInStyle(kCCTableViewFlyInBoth);
	else m_pListView->setFlyInStyle(kCCTableViewFlyInDisable);
	if(m_equipGrade == equip::kEquipGradeWhite)
	{
		m_pDataList->removeAllObjects();
		DM_GET_EQUIP_MODEL->getEquipListByTypeIgnoreID(m_equipType, 0, *m_pDataList);
		shiftList(m_pDataList);
	}
	m_pListView->reloadData();
}

void EquipMultiSelectView::setEquipMultiSelect(EquipMultiSelect* pEquipMultiSelect)
{
	m_pEquipMultiSelect = pEquipMultiSelect;
}

void EquipMultiSelectView::shiftList(cocos2d::CCArray* pList)
{
	pList->reverseObjects();
	CCArray* pRemove = CCArray::create();

	CCObject* obj = NULL;
	equip::EquipCurInfo* pEquipCurInfo = NULL;

	CCARRAY_FOREACH(pList, obj)
	{
		pEquipCurInfo = (equip::EquipCurInfo*)obj;
		//equip::EquipType equipType = pEquipCurInfo->getBaseInfo()->getEquipType();
		if (pEquipCurInfo->isEquiped())
		{
			pRemove->addObject(obj);
		}
	}

	CCARRAY_FOREACH(pRemove, obj)
	{
		pList->removeObject(obj);
	}
}




/************************ equip cell class begin**********************/

EquipSelectViewCell::EquipSelectViewCell()
:m_pEquipInfo(NULL)
,m_pEquipListTableDelegate(NULL)
,m_pGrade(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pEquipUpLevel(NULL)
,m_pProperty(NULL)
,m_pSelectButton(NULL)
,m_pUnEquip(NULL)
,m_pEquiped(NULL)
,m_pEquipedOn(NULL)
,m_pGradeBg(NULL)
,m_pContainer(NULL)
,m_pFateMark(NULL)
,m_pSellNode(NULL)
,m_pEnhanceNode(NULL)
,m_pGradeGreen(NULL)
,m_pGradeBlue(NULL)
,m_pGradePurple(NULL)
,m_pGradeOrange(NULL)
,m_pAmount(NULL)
,m_pCoinSprite(NULL)
,m_pZizhiNum(NULL)
,m_pFightNode(NULL)
,m_pBottom(NULL)
{
	for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
	{
		m_pPropertyType[i] = NULL;
	}
}

EquipSelectViewCell::~EquipSelectViewCell()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    
    
    CC_SAFE_RELEASE(m_pContainer);
    
    CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pEquipUpLevel);
    
    CC_SAFE_RELEASE(m_pProperty);
    
    CC_SAFE_RELEASE(m_pSelectButton);
    
    CC_SAFE_RELEASE(m_pUnEquip);
    CC_SAFE_RELEASE(m_pEquiped);
    CC_SAFE_RELEASE(m_pEquipedOn);
    CC_SAFE_RELEASE(m_pGradeBg);
    
    CC_SAFE_RELEASE(m_pFateMark);

	CC_SAFE_RELEASE(m_pSellNode);
	CC_SAFE_RELEASE(m_pEnhanceNode);
	CC_SAFE_RELEASE(m_pGradeGreen);
	CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradePurple);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pAmount);
	CC_SAFE_RELEASE(m_pCoinSprite);
	CC_SAFE_RELEASE(m_pZizhiNum);
	CC_SAFE_RELEASE(m_pFightNode);
	CC_SAFE_RELEASE(m_pBottom);

	for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
	{
		CC_SAFE_RELEASE(m_pPropertyType[i]);
	}
}

EquipSelectViewCell* EquipSelectViewCell::create(cocos2d::extension::CCData* pData)
{
    EquipSelectViewCell* pRet = new EquipSelectViewCell();
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

bool EquipSelectViewCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
    setFate(false);

	m_pSellNode->setVisible(true);
	m_pEnhanceNode->setVisible(false);
	m_pBottom->setVisible(false);
	m_pFightNode->setVisible(false);
	return true;
}

bool EquipSelectViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton", CCMenuItem *, this->m_pSelectButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellNode", CCNode *, this->m_pSellNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceNode", CCNode *, this->m_pEnhanceNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", CCNode *, this->m_pGradeGreen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pGradePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pGradeOrange);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont *, this->m_pAmount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoinSprite", CCNode *, this->m_pCoinSprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIsEquiped", CCNode *, this->m_pEquiped);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNotEquiped", CCNode *, this->m_pUnEquip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pEquipedOn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum",CCLabelBMFont*, m_pZizhiNum);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType",CCSprite*,this->m_pPropertyType,equip::kEquipPropTotal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFightNode", CCNode *, this->m_pFightNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom", CCNode *, this->m_pBottom);
    
    return false;
}

cocos2d::SEL_MenuHandler EquipSelectViewCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectClick", EquipSelectViewCell::onSelectClick);

	return NULL;
}

void EquipSelectViewCell::setTreasureInfo(equip::EquipCurInfo *pEquipInfo)
{
	setEquipInfo(pEquipInfo);
	m_pEquiped->setVisible(false);
	m_pAmount->setVisible(false);
	m_pCoinSprite->setVisible(false);
}

void EquipSelectViewCell::setEquipInfo(equip::EquipCurInfo *pEquipInfo)
{
	if(m_pEquipInfo == pEquipInfo) return;
    
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    
	CC_SAFE_RETAIN(pEquipInfo);
	m_pEquipInfo = pEquipInfo;
	BINDING_EXEC(m_pEquipInfo,this,EquipSelectViewCell::updateCellView,D_EVENT_EQUIPINFO_UPDATE);
}

equip::EquipCurInfo* EquipSelectViewCell::getEquipInfo() const
{
	return m_pEquipInfo;
}

void EquipSelectViewCell::updateCellView()
{
	if(!m_pEquipInfo||!m_pEquipInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("EquipInfoError").c_str());
		return;
	}

    m_pName->setString(m_pEquipInfo->getBaseInfo()->getEquipName().c_str());
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50PForList(m_pEquipInfo->getEquipId()));
    
	m_pLevel->setString(CCString::createWithFormat("%i",m_pEquipInfo->getLevel())->getCString());
	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pEquipInfo->getBaseInfo()->getQuality())->getCString());
	}
	//m_pEquipUpLevel->removeAllChildren();
	//m_pEquipUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(m_pEquipInfo->getRefineLevel()));
	//m_pGrade->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(m_pEquipInfo->getBaseInfo()->getEquipGrade()));
	//m_pGradeBg->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(m_pEquipInfo->getBaseInfo()->getEquipGrade()));
	
	//set cell color
	equip::EquipGrade equipGrade = m_pEquipInfo->getBaseInfo()->getEquipGrade();
	this->m_pGradeGreen->setVisible( equipGrade == equip::kEquipGradeGreen );
	this->m_pGradeBlue->setVisible( equipGrade == equip::kEquipGradeBlue );
	this->m_pGradePurple->setVisible( equipGrade == equip::kEquipGradePurple );
	this->m_pGradeOrange->setVisible( equipGrade == equip::kEquipGradeRed );

	// 典籍类型
	//bool bDianji = m_pEquipInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure;

	//m_pEnhanceButton->getButton()->setEnabled(!bDianji);
    //m_pSellButton->getButton()->setEnabled(!m_pEquipInfo->isEquiped() && !bDianji);
	//m_pRefineButton->getButton()->setEnabled(m_pEquipInfo->isCanRefine());
	
	// equiped?
	m_pEquiped->setVisible(m_pEquipInfo->isEquiped());
	m_pUnEquip->setVisible(!m_pEquiped->isVisible());
	m_pEquipedOn->setVisible(m_pEquipInfo->isEquiped());
	if(m_pEquipInfo->isEquiped())
	{
		formation::PositionInfo* pPosInfo = DM_GET_FORMATION_MODEL->getPositionInfoById(m_pEquipInfo->getPid());
		if (pPosInfo)
		{
			warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pPosInfo->iWarriorId);
			if (pWarrior) m_pEquipedOn->setString(pWarrior->getBaseInfo()->getWarriorName().c_str());
		}else
		{
			m_pEquiped->setVisible(false);
			m_pUnEquip->setVisible(true);
		}
	}

	//售價
	int sellPrice = DM_GET_EQUIP_MODEL->getPriceForSell(m_pEquipInfo);
	sellPrice < MAX_SHOW_COIN ? m_pAmount->setString(D_CSTRING_FROM_UINT(sellPrice)) : m_pAmount->setString(D_CSTRING_WITH10K_FROM_UINT(sellPrice));

    equip::EquipPropType type = equip::kEquipPropAttack;
	uint32_t prop_value = 0;
    std::string pecent;
    
	switch(m_pEquipInfo->getBaseInfo()->getEquipType())
	{
        case equip::kEquipTypeWeapon:	//武器
            type = equip::kEquipPropAttack;
            break;
        case equip::kEquipTypeArmor:	//防具
            type = equip::kEquipPropDefence;
            break;
        case equip::kEquipTypeOrnament:	//饰品
            type = equip::kEquipPropHp;
            break;
        case equip::kEquipTypeTreasure:		//典籍 + 速度/躲闪/暴击/格挡
        {
            if(m_pEquipInfo->getBaseInfo()->getSpeed()>0)
            {
                type = equip::kEquipPropSpeed;
            }
            else if(m_pEquipInfo->getBaseInfo()->getDodgeRate()>0)
            {
                type = equip::kEquipPropDodge;
                pecent = "%";
            }
            else if(m_pEquipInfo->getBaseInfo()->getCriticalRate()>0)
            {
                type = equip::kEquipPropCritical;
                pecent = "%";
            }
            else if(m_pEquipInfo->getBaseInfo()->getBlockRate()>0)
            {
                type = equip::kEquipPropBlock;
                pecent = "%";
            }
			else if(m_pEquipInfo->getBaseInfo()->getHitRate()>0)
			{
				type = equip::kEquipPropHit;
				pecent = "%";
			}
			else
            {
                CCLOG("error prop");
            }

            break;
        }
        default:
            CCLOG("error type");
            break;
	}
    
    prop_value = DM_GET_EQUIP_MODEL->computeEquipInfo(m_pEquipInfo,type);

	for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
	{
		if(m_pPropertyType[i])
		{
			m_pPropertyType[i]->setVisible(i==type);
		}
	}

	/*m_pPropertyType[0]->setVisible(type == equip::kEquipPropAttack);
	m_pPropertyType[1]->setVisible(type == equip::kEquipPropDefence);
	m_pPropertyType[2]->setVisible(type == equip::kEquipPropHp);
	m_pPropertyType[3]->setVisible(type == equip::kEquipPropSpeed);*/

    m_pProperty->setString(CCString::createWithFormat("+%d%s",prop_value,pecent.c_str())->getCString());

}

void EquipSelectViewCell::setSelected(bool val)
{
    val?m_pSelectButton->selected():m_pSelectButton->unselected();
}

void EquipSelectViewCell::setFate(bool val)
{
    //m_pFateMark->setVisible(val);
}

void EquipSelectViewCell::onSelectClick(cocos2d::CCObject* sender)
{
	if (m_pEquipListTableDelegate)
	{
		m_pEquipListTableDelegate->setSelectedCell(this);
	}
}


/************************ equip cell class end**********************/



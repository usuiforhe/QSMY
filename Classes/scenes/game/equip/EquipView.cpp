//
//  EquipView.cpp
//  装备列表
//
//  Created by wanghejun on 13-4-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "EquipView.h"
#include "Equip.h"
#include "managers/ResourceManager.h"
#include "model/FormationModel.h"
#include "model/WarriorModel.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"

//#include "EquipSelectView.h"

USING_NS_CC;
USING_NS_CC_EXT;



#define  EQUIP_CELL_SIZE		CCSizeMake(614, 135)
#define  EQUIP_CELL_SIZE1		CCSizeMake(614, 185)
#define  EQUIP_CELL_EXP_SIZE		CCSizeMake(614, 215)
#define  EQUIP_CELL_EXP_SIZE1		CCSizeMake(614, 265)

/************************ equip select class begin*******************/


EquipView::EquipView()
:m_selectIndex(-1)
,m_equipType(equip::kEquipTypeALL)
,m_pListView(NULL)
,m_pDelegate(NULL)
,m_pEquipModel(NULL)
,m_pData(NULL)
,m_bSelectBox(false)
{
    
};

EquipView::~EquipView()
{
    CC_SAFE_RELEASE(m_pData);
}


EquipView* EquipView::create(equip::EquipType equipType, const cocos2d::CCSize& size , bool bSelectBox )
{
	EquipView *pRet = new EquipView();
	if (pRet && pRet->init(equipType, size , bSelectBox))
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

bool EquipView::init(equip::EquipType equipType, const cocos2d::CCSize& size , bool bSelectBox)
{
	if(!DLayer::init())
	{
		return false;
	}
    m_bSelectBox = bSelectBox;
	m_equipType = equipType;
    
	m_pEquipModel = DM_GET_EQUIP_MODEL;
    
	m_pListView = DTableView::create(this, size);
	m_pListView->setFlyInStyle(kCCTableViewFlyInReload);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	//m_pListView->setPosition(EQUIP_LIST_POS);
    
	this->addChild(m_pListView);
    
    //m_pData = readCCData("ui/ccb/equip/equipListCell");
	m_pData = readCCData("PL_ui/ccb/pl_beibao/PL_beibao_wupin_cell");
    CC_SAFE_RETAIN(m_pData);
    
	return true;
    
}

void EquipView::onEnter()
{
    DLayer::onEnter();
	BINDING_ONLY(m_pEquipModel,this,EquipView::updateEquipView,D_EVENT_EQUIP_SELL_EQUIP/*D_EVENT_EQUIPLIST_UPDATE*/);
    updateEquipView();
}

void EquipView::onExit()
{
	UNBINDING_ALL(m_pEquipModel,this);
	DLayer::onExit();
    m_EquipInfoVec.removeAllObjects();
}


void EquipView::updateEquipView()
{
	if(!m_pEquipModel->isSynced) return;
    //m_selectIndex = - 1;
    updateEquipList();
	m_pListView->reloadData();
}


void EquipView::updateEquipList()
{
	m_pEquipModel->getEquipListByTypeIgnoreID(m_equipType, 0, m_EquipInfoVec);
}



void EquipView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	//CCLOG("equip cell touched at index: %i", cell->getIdx());
}

void EquipView::onTouch(cocos2d::CCObject* obj)
{
    //cocos2d::CCMenuItem* pItem = static_cast<cocos2d::CCMenuItem*>(obj);
    //
    //CCTableViewCell* pCell = static_cast<CCTableViewCell*>(pItem->getUserData());
    //if(!pCell) return;
    ////CCLOG("equip cell touched at index: %i,%i", idx,pCell->getIdx());
    //if(m_selectIndex == pCell->getIdx())
    //{
    //    m_selectIndex = - 1;
    //    m_pListView->updateCellPositions(pCell);
    //    m_pListView->reloadData();
    //}
    //else
    //{
    //    m_selectIndex = pCell->getIdx();
    //    m_pListView->updateCellPositions(pCell);
    //    m_pListView->reloadData();
    //}
}

CCSize EquipView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	//if ((int32_t)idx == m_selectIndex)
	//	return EQUIP_CELL_SIZE1;
	//else
	//	return EQUIP_CELL_SIZE;
    
	if(m_EquipInfoVec.count() == idx + 1)
	{
		//最后一个
		if(idx == m_selectIndex) return EQUIP_CELL_EXP_SIZE1;
		else return EQUIP_CELL_SIZE1;
	}
	//非最后一个
	if(idx == m_selectIndex)return EQUIP_CELL_EXP_SIZE;
	else return EQUIP_CELL_SIZE;
}

CCTableViewCell* EquipView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	EquipViewCell *cell = (EquipViewCell*)table->dequeueCell();
	if (!cell)
	{
		cell = EquipViewCell::create(m_pData , m_bSelectBox);
        cell->setDelegate(m_pDelegate);
		cell->setSelectDelegate(this);
        //cell->getSelectButton()->setTarget(this, menu_selector(EquipView::onTouch));
	}
	cell->setIndex(idx);
	cell->setSelected(idx == m_selectIndex);
	cell->setEquipInfo(dynamic_cast<equip::EquipCurInfo *>(m_EquipInfoVec.objectAtIndex(idx)));

	//if(idx == m_EquipInfoVec.count() - 1)
	//	cell->adjust(EquipViewCell::kSelectedStatus);
	//else
	//	cell->adjust(EquipViewCell::kNormalStatus);

	cell->adjust(idx == m_EquipInfoVec.count() - 1 ? EquipViewCell::kSelectedStatus : EquipViewCell::kNormalStatus);
	return cell;
}

uint32_t EquipView::numberOfCellsInTableView(CCTableView *table)
{
	return m_EquipInfoVec.count();
}

void EquipView::setSelected(int index)
{
	int oldindex = m_selectIndex;
	m_selectIndex = index;
	m_pListView->setFlyInStyle(kCCTableViewFlyInDisable);

	int delta = 0;
	if(oldindex == -1){
		//之前没打开
		if(m_selectIndex != -1)delta = EQUIP_CELL_EXP_SIZE.height - EQUIP_CELL_SIZE.height;
		else CCLOG("wrong");
	}
	else
	{
		//之前打开了 现在关
		if(m_selectIndex == -1)delta = EQUIP_CELL_SIZE.height - EQUIP_CELL_EXP_SIZE.height;
		//之前打开了 现在还是打开 那么0
	}

	//CCLOG("%f" , m_pListView->getContentOffset().y);
	m_pListView->setContentOffset(ccpSub(m_pListView->getContentOffset(),ccp(0,delta)));
	//CCLOG("%f" , m_pListView->getContentOffset().y);
	m_pListView->reloadData();
	m_pListView->setFlyInStyle(kCCTableViewFlyInReload);
}

/************************ equip select class end*******************/





/************************ equip cell class begin**********************/

EquipViewCell::EquipViewCell()
:m_pEquipInfo(NULL)
,m_pDelegate(NULL)
,m_pSelectDelegate(NULL)
,m_pInfoContainer(NULL)
,m_pExtNode(NULL)
,m_pContainer(NULL)
,m_pGrade(NULL)
,m_pName(NULL)
,m_pEquipUpLevel(NULL)
,m_pLevel(NULL)
,m_pProperty(NULL)
,m_pEnhanceButton(NULL)
,m_pSellButton(NULL)
,m_pRefineButton(NULL)
,m_pUnEquip(NULL)
,m_pEquiped(NULL)
,m_pEquipedOn(NULL)
,m_pSellNode(NULL)
,m_pEnhanceNode(NULL)
,m_pGradeGreen(NULL)
,m_pGradeBlue(NULL)
,m_pGradePurple(NULL)
,m_pGradeOrange(NULL)
,m_bSelectBox(false)
,m_pZizhiNum(NULL)
,m_pTxtTunshi(NULL)
,m_pBottom(NULL)
,m_pButtonLeft(NULL)
,m_pButtonRight(NULL)
,m_uIndex(0)
,m_bSelected(false)
,m_pFightNode(NULL)
,m_pFight(NULL)
{
    for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
    {
        m_pPropertyType[i] = NULL;
    }
}

EquipViewCell::~EquipViewCell()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    CC_SAFE_RELEASE(m_pInfoContainer);
    CC_SAFE_RELEASE(m_pExtNode);
    
    CC_SAFE_RELEASE(m_pContainer);
    
    CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pEquipUpLevel);
    
    CC_SAFE_RELEASE(m_pProperty);
    
    CC_SAFE_RELEASE(m_pEnhanceButton);
    CC_SAFE_RELEASE(m_pSellButton);
	CC_SAFE_RELEASE(m_pRefineButton);
    
    CC_SAFE_RELEASE(m_pUnEquip);
    CC_SAFE_RELEASE(m_pEquiped);
    CC_SAFE_RELEASE(m_pEquipedOn);
	CC_SAFE_RELEASE(m_pSellNode);
	CC_SAFE_RELEASE(m_pEnhanceNode);
	CC_SAFE_RELEASE(m_pGradeGreen);
	CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradePurple);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pZizhiNum);
	CC_SAFE_RELEASE(m_pTxtTunshi);
	CC_SAFE_RELEASE(m_pBottom);
	CC_SAFE_RELEASE(m_pButtonLeft);
	CC_SAFE_RELEASE(m_pButtonRight);
	CC_SAFE_RELEASE(m_pFightNode);
	CC_SAFE_RELEASE(m_pFight);
    
    for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
    {
        CC_SAFE_RELEASE(m_pPropertyType[i]);
    }
                    
}

EquipViewCell* EquipViewCell::create(cocos2d::extension::CCData* pData , bool bSelectBox)
{
    EquipViewCell* pRet = new EquipViewCell();
    if(pRet && pRet->init(pData , bSelectBox))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool EquipViewCell::init(cocos2d::extension::CCData* pData , bool bSelectBox)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
	m_bSelectBox = bSelectBox;
    m_pEnhanceButton->getButton()->setTarget(this, menu_selector(EquipViewCell::enhanceCallBack));
	m_pButtonLeft->getButton()->setTarget(this, menu_selector(EquipViewCell::enhanceCallBack));
	m_pButtonRight->getButton()->setTarget(this, menu_selector(EquipViewCell::refineCallBack));
	m_pBottom->setVisible(false);
	return true;
}

bool EquipViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInfoContainer", CCNode *, this->m_pInfoContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellNode", CCNode *, this->m_pSellNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceNode", CCNode *, this->m_pEnhanceNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", CCNode *, this->m_pGradeGreen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pGradePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pGradeOrange);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnhanceButton", DButton *, this->m_pEnhanceButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIsEquiped", CCNode *, this->m_pEquiped);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNotEquiped", CCNode *, this->m_pUnEquip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pEquipedOn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum", CCLabelBMFont*, m_pZizhiNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtTunshi", CCNode *, this->m_pTxtTunshi);

	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType",CCSprite*,this->m_pPropertyType,equip::kEquipPropTotal);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonLeft", DButton *, this->m_pButtonLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonRight", DButton *, this->m_pButtonRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom", CCNode *, this->m_pBottom);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFightNode", CCNode *, this->m_pFightNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	
    return false;
}

SEL_MenuHandler EquipViewCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", EquipViewCell::viewCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "bottomCallBack", EquipViewCell::bottomCallBack);
    return NULL;
}

void EquipViewCell::adjust(CellStatus type)
{

	float dy = EQUIP_CELL_SIZE1.height - EQUIP_CELL_SIZE.height;
    
	if(type == kSelectedStatus)
	{
		//m_pInfoContainer->setPositionY(dy);
		if(m_bSelected)m_pInfoContainer->setPositionY(EQUIP_CELL_EXP_SIZE1.height - EQUIP_CELL_SIZE.height);
		else m_pInfoContainer->setPositionY(dy);
	}
	else if(type == kNormalStatus)
	{
		if(m_bSelected)m_pInfoContainer->setPositionY(EQUIP_CELL_EXP_SIZE.height - EQUIP_CELL_SIZE.height);
		else m_pInfoContainer->setPositionY(0.0);
	}

	m_pBottom->setVisible(m_bSelected);
}

void EquipViewCell::setEquipInfo(equip::EquipCurInfo *pEquipInfo)
{
	//m_pBottom->setVisible(m_bSelected);

	if(m_pEquipInfo == pEquipInfo) return;
    
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    
	CC_SAFE_RETAIN(pEquipInfo);
	m_pEquipInfo = pEquipInfo;
	BINDING_EXEC(m_pEquipInfo,this,EquipViewCell::updateCellView,D_EVENT_EQUIPINFO_UPDATE);
}


void EquipViewCell::updateCellView()
{
    if(!m_pEquipInfo||!m_pEquipInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("EquipInfoError").c_str());
		return;
	}

    m_bSelectBox?m_pSellNode->setVisible(true):m_pSellNode->setVisible(false);
	m_bSelectBox?m_pEnhanceNode->setVisible(false):m_pEnhanceNode->setVisible(true);

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
	bool bDianji = m_pEquipInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure;
	m_pTxtTunshi->setVisible(bDianji);
	m_pEnhanceNode->setVisible(bDianji);
	m_pFightNode->setVisible(!bDianji);
	int iFight = DM_GET_EQUIP_MODEL->computeEquipFight(m_pEquipInfo);
	m_pFight->setString(CCString::createWithFormat("%d",iFight)->getCString());

	//m_pEnhanceButton->getButton()->setEnabled(!bDianji);
    //m_pSellButton->getButton()->setEnabled(!m_pEquipInfo->isEquiped() && !bDianji);
	//m_pRefineButton->getButton()->setEnabled(m_pEquipInfo->isCanRefine());

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
	//m_pPropertyType[0]->setVisible(type == equip::kEquipPropAttack);
	//m_pPropertyType[1]->setVisible(type == equip::kEquipPropDefence);
	//m_pPropertyType[2]->setVisible(type == equip::kEquipPropHp);
	//m_pPropertyType[3]->setVisible(type == equip::kEquipPropSpeed);

    m_pProperty->setString(CCString::createWithFormat("+%d%s",prop_value,pecent.c_str())->getCString());

}

void EquipViewCell::bottomCallBack(cocos2d::CCObject* obj)
{
	bool bDianji = m_pEquipInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure;
	if(!bDianji)m_pSelectDelegate->setSelected(m_bSelected ? -1 : m_uIndex);
}

void EquipViewCell::viewCallBack(cocos2d::CCObject* obj)
{
    if(m_pDelegate) m_pDelegate->showInfoView(kEquipInfo, m_pEquipInfo);
}
void EquipViewCell::enhanceCallBack(cocos2d::CCObject* obj)
{
	if (m_pEquipInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure && m_pEquipInfo->getLevel() >= TREASURE_MAX_LEVEL)
	{
		//宝物已满级提示
		Tips_Alert(D_LOCAL_STRING("TreasureTips3").c_str());
		return;
	}

	if (!m_pEquipInfo->isCanEnhance())
	{
		Tips_Alert(D_LOCAL_STRING("EquipMaxLevel").c_str());
		return;
	}

    if(m_pDelegate)m_pDelegate->showInfoView(kEquipEnhance,m_pEquipInfo);
}
void EquipViewCell::sellCallBack(cocos2d::CCObject* obj)
{
    //DM_GET_EQUIP_MODEL->sellEquip(m_pEquipInfo);
	CCArray* equips = CCArray::create();
	equips->addObject(m_pEquipInfo);
	//DM_GET_EQUIP_MODEL->decomposeEquip(equips);
	DM_GET_EQUIP_MODEL->sellEquip(equips);
}

void EquipViewCell::refineCallBack(cocos2d::CCObject* obj)
{
	if (!DM_GET_EQUIP_MODEL->getEquipRefineIsCanUpLevel(m_pEquipInfo , true))
	{
		return;
	}

	//if (DM_GET_EQUIP_MODEL->isRefineMaxLevel(m_pEquipInfo->getRefineExp(), m_pEquipInfo->getBaseInfo()->getEquipGrade()))
	//{
	//	Tips_Alert(D_LOCAL_STRING("EquipRefineMax").c_str());
	//	return;
	//}

	if(m_pDelegate)m_pDelegate->showInfoView(kEquipRefine,m_pEquipInfo);
}



/************************ equip cell class end**********************/





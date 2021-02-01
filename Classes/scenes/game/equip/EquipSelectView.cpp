//  EquipList.cpp
//	选择单个装备界面

//  Created by cll on 13-5-7
//  Copyright (c) 2013, thedream. All rights reserved.
//
#include <algorithm>
#include "EquipSelectView.h"
#include "../formation/Formation.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/TipsManager.h"
#include "../../../managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  EQUIP_LIST_SIZE		CCSizeMake(640, 888)
#define  EQUIP_LIST_POS			ccp(0, 207)
#define  EQUIP_CELL_SIZE        CCSizeMake(640, 129)

#define UNSELECT -1

/************************ equip class begin*****************************/

EquipSelectView::EquipSelectView()
:m_pEquipSelecteDelegate(NULL)
,m_selectIndex(UNSELECT)
,m_pData(NULL)
,m_pEquipList(NULL)
,m_pListView(NULL)
,m_pFates(NULL)
,m_pChangeEquipLabel(NULL)
,m_pChangeHeroLabel(NULL)
,m_pPageContainer(NULL)
,m_pPageContainerEnd(NULL)
{
    
}

EquipSelectView::~EquipSelectView()
{
    CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RELEASE(m_pEquipList);
    CC_SAFE_RELEASE(m_pFates);
	CC_SAFE_RELEASE(m_pChangeEquipLabel);
	CC_SAFE_RELEASE(m_pChangeHeroLabel);
	CC_SAFE_RELEASE(m_pPageContainer);
	CC_SAFE_RELEASE(m_pPageContainerEnd);
}

EquipSelectView* EquipSelectView::create()
{
	EquipSelectView* pRet = new EquipSelectView();
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	} else 
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool EquipSelectView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pFates = CCArray::create();
    CC_SAFE_RETAIN(m_pFates);
    
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhenrong/PL_zhenrong_lb");
	

	m_pData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_zb_cell");
    CC_SAFE_RETAIN(m_pData);
	
    /*CCLOG(CCString::createWithFormat("point w:%f, h:%f", m_pPageContainer->getPositionX(), m_pPageContainer->getPositionY())->getCString());
	CCLOG(CCString::createWithFormat("point w:%f, h:%f", m_pPageContainerEnd->getPositionX(), m_pPageContainerEnd->getPositionY())->getCString());
	CCLOG(CCString::createWithFormat("size w:%f, h:%f", PL_WITHNAV_GET_CONTEENT_SIZE(m_pPageContainer,m_pPageContainerEnd).width, PL_WITHNAV_GET_CONTEENT_SIZE(m_pPageContainer,m_pPageContainerEnd).height)->getCString());
    */
	m_pListView = DTableView::create(this, PL_WITHNAV_GET_CONTEENT_SIZE(m_pPageContainer,m_pPageContainerEnd));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
    m_pListView->setPosition(ccp(12.0,0.0));
	m_pPageContainerEnd->setContentSize(PL_WITHNAV_GET_CONTEENT_SIZE(m_pPageContainer,m_pPageContainerEnd));
	m_pPageContainerEnd->addChild(m_pListView);
    m_pChangeEquipLabel->setVisible(true);
	m_pChangeHeroLabel->setVisible(false);
	return true;
}


void EquipSelectView::setSelectedCell(FormationEquipSelectViewCell* cell)
{
	CCLOG("equip cell touched at index: %i", cell->getIdx());
	m_selectIndex = m_selectIndex == cell->getIdx() ?UNSELECT :cell->getIdx();
	//DTableView * DTable = dynamic_cast<DTableView *>(table);
	m_pListView->reloadData();
}

bool EquipSelectView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeEquipLabel", CCNode *, this->m_pChangeEquipLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeHeroLabel", CCNode *, this->m_pChangeHeroLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainer", CCNode *, this->m_pPageContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainerEnd", CCNode *, this->m_pPageContainerEnd);
    return false;
}

SEL_MenuHandler EquipSelectView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callbackReturn", EquipSelectView::callbackReturn);
	return NULL;
}

void EquipSelectView::callbackReturn(cocos2d::CCObject *sender)
{
	if(m_pEquipSelecteDelegate)
	{
		m_pEquipSelecteDelegate->selectedEquip(NULL);
	}
}

static bool compareProperty(const CCObject* p1, const CCObject* p2)
{
	equip::EquipCurInfo* pEquip1 = (equip::EquipCurInfo*)p1;
	equip::EquipCurInfo* pEquip2 = (equip::EquipCurInfo*)p2;
	if(pEquip1->getInFate() != pEquip2->getInFate())
	{
		return pEquip1->getInFate() > pEquip2->getInFate();
	}
	equip::EquipGrade pGrade1 = pEquip1->getBaseInfo()->getEquipGrade();
	equip::EquipGrade pGrade2 = pEquip2->getBaseInfo()->getEquipGrade();
	if(pGrade1 != pGrade2)
	{
		return pGrade1 > pGrade2;
	}

	float pProp1 = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquip1);
    float pProp2 = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquip2);
	if(pProp1 != pProp2)
	{
		return pProp1 > pProp2;
	}
	bool pEquiped1 = pEquip1->isEquiped();
	bool pEquiped2 = pEquip2->isEquiped();
	return pEquiped1 > pEquiped2;
}

void EquipSelectView::setEquipList(CCArray*pList,uint32_t wid)
{
    CC_SAFE_RETAIN(pList); 
    CC_SAFE_RELEASE(m_pEquipList); 
    m_pEquipList = pList;
    m_pFates->removeAllObjects();
    
    
    if(wid>0)
    {
        cocos2d::CCArray* pFates = CCArray::create();
        equip::EquipCurInfo* pEquip = NULL;
        CCObject* obj = NULL;
		CCBool* pInFate = NULL;
		CCARRAY_FOREACH(m_pEquipList, obj)
		{
			pInFate = CCBool::create(false);
			pEquip = static_cast<equip::EquipCurInfo*>(obj);
			DM_GET_WARRIOR_MODEL->getWarriorFatesByWIDAndEquipId(wid,pEquip->getEquipId(),pFates);
			pInFate = pFates->count()>0?CCBool::create(true):CCBool::create(false);
			
			pEquip->setInFate(pInFate->getValue());
			CCLOG("eEquip{%d}: fate %i, grade %i", pEquip->getEquipId(), pInFate->getValue(), pEquip->getBaseInfo()->getEquipGrade());
		}
		//排序
		std::sort(m_pEquipList->data->arr, m_pEquipList->data->arr + m_pEquipList->data->num, compareProperty);
		CCARRAY_FOREACH(m_pEquipList, obj)
		{
			pInFate = CCBool::create(false);
			pEquip = static_cast<equip::EquipCurInfo*>(obj);
			DM_GET_WARRIOR_MODEL->getWarriorFatesByWIDAndEquipId(wid,pEquip->getEquipId(),pFates);
			pInFate = pFates->count()>0?CCBool::create(true):CCBool::create(false);
			m_pFates->addObject(pInFate);
		}
    }
    m_pListView->reloadData();    
    m_pListView->setContentOffset(m_pListView->minContainerOffset());
}

void EquipSelectView::onExit()
{
	DLayer::onExit();
    CC_SAFE_RELEASE_NULL(m_pEquipList);
	m_selectIndex = UNSELECT;
}


void EquipSelectView::menuCallback(CCObject * obj)
{
    if(m_pEquipSelecteDelegate)
    {
        equip::EquipCurInfo* pEquip = NULL;
		CCLOG("menu call back at page index: %i", m_selectIndex);
        if(m_selectIndex!=UNSELECT)
        {
             pEquip = (equip::EquipCurInfo *)m_pEquipList->objectAtIndex(m_selectIndex);
        }
        m_pEquipSelecteDelegate->selectedEquip(pEquip);
    }
}


void EquipSelectView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

CCSize EquipSelectView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return EQUIP_CELL_SIZE;
}

CCTableViewCell* EquipSelectView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FormationEquipSelectViewCell *cell = (FormationEquipSelectViewCell*)table->dequeueCell();
	if (!cell)
	{
		cell = FormationEquipSelectViewCell::create(m_pData);
	}
    
    equip::EquipCurInfo* pEquip = static_cast<equip::EquipCurInfo *>(m_pEquipList->objectAtIndex(idx));
    bool fate = idx<m_pFates->count()?static_cast<CCBool*>(m_pFates->objectAtIndex(idx))->getValue():false;
    
    
    //cell->setSelected(m_selectIndex==idx);
	equip::EquipCurInfo* pBest = (equip::EquipCurInfo*)m_pEquipList->objectAtIndex(0);
	cell->setRecommond(pEquip->getId() == pBest->getId());
	
	cell->setDelegate(this);
	cell->setEquipInfo(pEquip);
    cell->setFate(fate);
	return cell;
}

uint32_t EquipSelectView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pEquipList?m_pEquipList->count():0;
}




FormationEquipSelectViewCell::FormationEquipSelectViewCell()
:m_pEquipInfo(NULL)
,m_pEquipListTableDelegate(NULL)
,m_pGrade(NULL)
,m_pEquipName(NULL)
,m_pLevel(NULL)
,m_pEquipUpLevel(NULL)
,m_pProperty(NULL)
,m_pAssignButton(NULL)
,m_pUnEquip(NULL)
,m_pEquiped(NULL)
,m_pEquipedOn(NULL)
,m_pGradeBg(NULL)
,m_pContainer(NULL)
,m_pRecommondMark(NULL)
,m_pFateMark(NULL)
,m_pGradeGreen(NULL)
,m_pGradeBlue(NULL)
,m_pGradePurple(NULL)
,m_pGradeOrange(NULL)
,m_pAddAmount(NULL)
,m_pReduceAmount(NULL)
,m_pPropertyAddMark(NULL)
,m_pPropertyReduceMark(NULL)
,m_pPropNoChange(NULL)
{
	D_RESET_C_ARRAY(m_pPropertyType, equip::kEquipPropTotal);
}

FormationEquipSelectViewCell::~FormationEquipSelectViewCell()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    
    
    CC_SAFE_RELEASE(m_pContainer);
    
    CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pEquipName);
    CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pEquipUpLevel);
    
    CC_SAFE_RELEASE(m_pProperty);
    
    CC_SAFE_RELEASE(m_pAssignButton);
    
    CC_SAFE_RELEASE(m_pUnEquip);
    CC_SAFE_RELEASE(m_pEquiped);
    CC_SAFE_RELEASE(m_pEquipedOn);
    CC_SAFE_RELEASE(m_pGradeBg);

	CC_SAFE_RELEASE(m_pGradeGreen);
	CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradePurple);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pAddAmount);
	CC_SAFE_RELEASE(m_pReduceAmount);
	CC_SAFE_RELEASE(m_pPropertyAddMark);
	CC_SAFE_RELEASE(m_pPropertyReduceMark);
	CC_SAFE_RELEASE(m_pRecommondMark);
	CC_SAFE_RELEASE(m_pFateMark);
	CC_SAFE_RELEASE(m_pPropNoChange);
    D_SAFE_RELEASE_C_ARRAY(m_pPropertyType, equip::kEquipPropTotal);
}

FormationEquipSelectViewCell* FormationEquipSelectViewCell::create(cocos2d::extension::CCData* pData)
{
    FormationEquipSelectViewCell* pRet = new FormationEquipSelectViewCell();
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

bool FormationEquipSelectViewCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);

	m_pAssignButton->getButton()->setTarget(this, menu_selector(FormationEquipSelectViewCell::menuCallback));
    setFate(false);
	setRecommond(false);
	return true;
}

bool FormationEquipSelectViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAssignButton", DButton *, this->m_pAssignButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", CCNode *, this->m_pGradeGreen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", CCNode *, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", CCNode *, this->m_pGradePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", CCNode *, this->m_pGradeOrange);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipName", CCLabelTTF *, this->m_pEquipName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType",CCSprite*,this->m_pPropertyType,equip::kEquipPropTotal);
	/*CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType4", CCSprite *, this->m_pPropertyType[4]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType5", CCSprite *, this->m_pPropertyType[5]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyType6", CCSprite *, this->m_pPropertyType[6]);*/

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddAmount", CCLabelBMFont *, this->m_pAddAmount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReduceAmount", CCLabelBMFont *, this->m_pReduceAmount);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIsEquiped", CCNode *, this->m_pEquiped);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNotEquiped", CCNode *, this->m_pUnEquip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipedOn", CCLabelTTF *, this->m_pEquipedOn);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyAddMark", CCSprite *, this->m_pPropertyAddMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropertyReduceMark", CCSprite *, this->m_pPropertyReduceMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRecommondMark", CCSprite *, this->m_pRecommondMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateMark", CCSprite *, this->m_pFateMark);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPropNoChange", CCLabelBMFont *, this->m_pPropNoChange);

    return false;
}

cocos2d::SEL_MenuHandler FormationEquipSelectViewCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectClick", FormationEquipSelectViewCell::onSelectClick);

	return NULL;
}

void FormationEquipSelectViewCell::menuCallback(cocos2d::CCObject *sender)
{
	this->onSelectClick(sender);
    m_pEquipListTableDelegate->menuCallback(sender);
}

void FormationEquipSelectViewCell::setEquipInfo(equip::EquipCurInfo *pEquipInfo)
{
	if(m_pEquipInfo == pEquipInfo) return;
    
	SAFE_RELEASE_UNBINDING_ALL(m_pEquipInfo, this);
    
	CC_SAFE_RETAIN(pEquipInfo);
	m_pEquipInfo = pEquipInfo;
	BINDING_EXEC(m_pEquipInfo,this,FormationEquipSelectViewCell::updateCellView,D_EVENT_EQUIPINFO_UPDATE);
}

equip::EquipCurInfo* FormationEquipSelectViewCell::getEquipInfo() const
{
	return m_pEquipInfo;
}

void FormationEquipSelectViewCell::updateCellView()
{
	if(!m_pEquipInfo||!m_pEquipInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("EquipInfoError").c_str());
		return;
	}

    m_pEquipName->setString(m_pEquipInfo->getBaseInfo()->getEquipName().c_str());
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon40P(m_pEquipInfo->getEquipId()));
    
	m_pLevel->setString(CCString::createWithFormat("%i",m_pEquipInfo->getLevel())->getCString());
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

	//m_pEnhanceButton->getButton()->setEnabled(!bDianji);
    //m_pSellButton->getButton()->setEnabled(!m_pEquipInfo->isEquiped() && !bDianji);
	//m_pRefineButton->getButton()->setEnabled(m_pEquipInfo->isCanRefine());
	
	// equiped?
	m_pEquiped->setVisible(m_pEquipInfo->isEquiped());
	//m_pUnEquip->setVisible(!m_pEquiped->isVisible());
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
			//m_pUnEquip->setVisible(true);
		}
	}
    std::string pecent;
    equip::EquipPropType type = DM_GET_EQUIP_MODEL->getEquipPropType(m_pEquipInfo, pecent);

	uint32_t prop_value = DM_GET_EQUIP_MODEL->computeEquipInfo(m_pEquipInfo,type);
    
    for (uint8_t i =0; i<equip::kEquipPropTotal; ++i)
    {
        if(m_pPropertyType[i])
        {
            m_pPropertyType[i]->setVisible(i==type);
        }
    }
    m_pProperty->setString(CCString::createWithFormat("%d%s",prop_value,pecent.c_str())->getCString());

	int changeAmount = prop_value - m_pEquipListTableDelegate->getEquipLastProperty();
	
	m_pPropertyAddMark->setVisible(changeAmount > 0);
	m_pAddAmount->setVisible(changeAmount > 0);
	m_pPropertyReduceMark->setVisible(changeAmount < 0);
	m_pReduceAmount->setVisible(changeAmount < 0);
	
	if(changeAmount != 0)
	{
		m_pPropNoChange->setVisible(false);
		changeAmount > 0 ? m_pAddAmount->setString(CCString::createWithFormat("%d",abs(changeAmount))->getCString()) :
		                   m_pReduceAmount->setString(CCString::createWithFormat("%d",abs(changeAmount))->getCString());
	} else {
		//m_pAddAmount->setVisible(false);
		//m_pReduceAmount->setVisible(false);
		m_pPropNoChange->setVisible(true);
		m_pPropNoChange->setString("--");
	}
	
}

void FormationEquipSelectViewCell::setSelected(bool val)
{
	//val?m_pSelectButton->selected():m_pSelectButton->unselected();
}

void FormationEquipSelectViewCell::setFate(bool val)
{
    m_pFateMark->setVisible(val);
}

void FormationEquipSelectViewCell::setRecommond(bool val)
{
	m_pRecommondMark->setVisible(val);
}

void FormationEquipSelectViewCell::onSelectClick(cocos2d::CCObject* sender)
{
	if (m_pEquipListTableDelegate)
	{
		m_pEquipListTableDelegate->setSelectedCell(this);
	}
}


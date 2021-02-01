//
//  SoulListView.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-8-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SoulListView.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/ResourceManager.h"
#include "../common/gui/WarriorTypeMark.h"
#include "Warrior.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  SOUL_CELL_SIZE                      CCSizeMake(640, 130)    //单元大小

#pragma mark ----------------SoulCell--------------------

SoulCell::SoulCell():
m_pSoul(NULL),
m_pName(NULL),
m_pAvatorContainer(NULL),
//m_pRgbaGrade(NULL),
m_pAmount(NULL),
m_pWarriorTypeMark(NULL),
m_pRecruitContainer(NULL),
m_pRecruitButton(NULL),
m_pNotEnough(NULL),
m_pRecruited(NULL),
m_pDelegate(NULL)
{
	m_gradeBgNodes.insert(m_gradeBgNodes.begin(), warrior::kWarGradeRed + 1, (CCNode *)NULL);
}

SoulCell::~SoulCell()
{
    SAFE_RELEASE_UNBINDING_ALL(m_pSoul, this);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pAvatorContainer);
	//CC_SAFE_RELEASE(m_pRgbaGrade);
    CC_SAFE_RELEASE(m_pAmount);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pRecruitContainer);
    CC_SAFE_RELEASE(m_pRecruitButton);
    CC_SAFE_RELEASE(m_pNotEnough);
    CC_SAFE_RELEASE(m_pRecruited);

	for (uint32_t i = 0; i < m_gradeBgNodes.size(); i++)
	{
		CC_SAFE_RELEASE_NULL(m_gradeBgNodes[i]);
	}
}

SoulCell * SoulCell::create(cocos2d::extension::CCData *pData)
{
    SoulCell * pRet = new SoulCell;
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    
    return pRet;
}

bool SoulCell::init(cocos2d::extension::CCData * pData)
{
    if(!CCTableViewCell::init())
    {
        return false;
    }    
    
    CCNodeLoaderLibrary * pCCNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pCCNodeLoaderLibrary);
    pCCNodeLoaderLibrary->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());
    
    CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pCCNodeLoaderLibrary);
    
    return true;
}

void SoulCell::onEnter()
{
    BINDING_EXEC(m_pSoul, this, SoulCell::updateView, D_EVENT_SOULINFO_UPDATE);
    CCTableViewCell::onEnter();
}

void SoulCell::onExit()
{
    UNBINDING_ALL(m_pSoul, this);
    CCTableViewCell::onExit();
}

bool SoulCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer", CCNode *, m_pAvatorContainer);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteGrade", CCNodeRGBA *, m_pRgbaGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeGreen", CCNode *, m_gradeBgNodes[warrior::kWarGradeGreen]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeBlue", CCNode *, m_gradeBgNodes[warrior::kWarGradeBlue]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradePurple", CCNode *, m_gradeBgNodes[warrior::kWarGradePurple]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeOrange", CCNode *, m_gradeBgNodes[warrior::kWarGradeRed]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAmount", CCLabelBMFont *, m_pAmount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRecruitContainer", CCNode *, m_pRecruitContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRecruitButton", DButton *, m_pRecruitButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNotEnough", CCSprite *, m_pNotEnough);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRecruited", CCSprite *, m_pRecruited);
    
    return false;
}

SEL_MenuHandler SoulCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", SoulCell::viewCallBack);
    return NULL;
}

void SoulCell::setSelected(bool selected)
{

}

void SoulCell::setSoul(warrior::SoulInfo *pSoul)
{
    if(m_pSoul == pSoul)
    {
        return;
    }
    
    UNBINDING_ALL(m_pSoul, this);
    CC_SAFE_RELEASE_NULL(m_pSoul);
    m_pSoul = pSoul;
    CC_SAFE_RETAIN(m_pSoul);
    BINDING_EXEC(m_pSoul, this, SoulCell::updateView, D_EVENT_SOULINFO_UPDATE);
    m_pWarriorTypeMark->setWarriorInfo(m_pSoul->getBaseInfo());
}

void SoulCell::updateView()
{
    if(m_pName) m_pName->setString(m_pSoul->getBaseInfo()->getWarriorName().c_str());
    
    if(m_pAvatorContainer)
	{
		m_pAvatorContainer->removeAllChildren();
		m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pSoul->getWid()));
	}
    

	//m_pRgbaGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByColorGrade(m_pSoul->getBaseInfo()->getColorGrade()));
	for (uint32_t i = 0; i < m_gradeBgNodes.size(); i++)
	{
		if (m_gradeBgNodes[i])
		{
			m_gradeBgNodes[i]->setVisible(false);
		}
	}

	if (m_gradeBgNodes[m_pSoul->getBaseInfo()->getColorGrade()])
	{
		m_gradeBgNodes[m_pSoul->getBaseInfo()->getColorGrade()]->setVisible(true);
	}
    
	warrior::WarriorGrowInfo * pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(m_pSoul->getBaseInfo()->getRealGrade());
    if(m_pAmount) m_pAmount->setString(CCString::createWithFormat("%d/%d", m_pSoul->getNum(), pGrowInfo->getRctSoul())->getCString());
    
    if(DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(m_pSoul->getWid()) != NULL)
    {
        if(m_pRecruitContainer) m_pRecruitContainer->setVisible(false);
        if(m_pNotEnough) m_pNotEnough->setVisible(false);
        if(m_pRecruited) m_pRecruited->setVisible(true);
    }
    else
    {
        if(m_pRecruited) m_pRecruited->setVisible(false);
        if(m_pSoul->getNum() >= pGrowInfo->getRctSoul())
        {
            if(m_pRecruitContainer) m_pRecruitContainer->setVisible(true);
            if(m_pNotEnough) m_pNotEnough->setVisible(false);
            if(m_pRecruitButton) m_pRecruitButton->getButton()->setTarget(this, menu_selector(SoulCell::onRecruitClick));
        }
        else
        {            
            if(m_pRecruitContainer) m_pRecruitContainer->setVisible(false);
            if(m_pNotEnough) m_pNotEnough->setVisible(true);
        }
    }
}

void SoulCell::onRecruitClick(cocos2d::CCObject *sender)
{    
    warrior::WarriorGrowInfo * pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(m_pSoul->getBaseInfo()->getRealGrade());
    if (m_pSoul->getNum() > 0 && m_pSoul->getNum() >= pGrowInfo->getRctSoul())
    {
        HTTP_CLIENT->recruitSoul(m_pSoul->getWid());
    }
}


void SoulCell::viewCallBack(cocos2d::CCObject *pSender)
{
    if(m_pDelegate)
    {
        warrior::WarriorCurInfo * pInfo = warrior::WarriorCurInfo::create(m_pSoul->getBaseInfo());
        m_pDelegate->showInfoView(kSoulInfo, pInfo);
    }
}

#pragma mark ----------------SoulSelectCell 魂魄选择列表单元------------------
SoulSelectCell * SoulSelectCell::create(cocos2d::extension::CCData *pData)
{
	SoulSelectCell * pRet = new SoulSelectCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}
	return pRet;
}

SoulSelectCell::SoulSelectCell()
:SoulCell()
,m_pSelect(NULL)
,m_bSelected(false)
{

}

SoulSelectCell::~SoulSelectCell()
{
	CC_SAFE_RELEASE_NULL(m_pSelect);
}

bool SoulSelectCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton", CCMenuItemImage *, m_pSelect);

	return SoulCell::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

SEL_MenuHandler SoulSelectCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectClick", SoulSelectCell::onSelectClick);
	return NULL;
}

void SoulSelectCell::updateView()
{
	SoulCell::updateView();

	if(m_bSelected)
	{
		m_pSelect->selected();
	}
	else
	{
		m_pSelect->unselected();
	}
}

void SoulSelectCell::setSelected(bool selected)
{
	m_bSelected = selected;
	if(m_bSelected)
	{
		m_pSelect->selected();
	}
	else
	{
		if(m_pSelect) m_pSelect->unselected();
	}
}

void SoulSelectCell::onSelectClick(cocos2d::CCObject *sender)
{
	m_bSelected = !m_bSelected;
	if(m_pSelectDelegate != NULL)
	{
		m_pSelectDelegate->setSelectedCell(m_bSelected ? this : NULL,this,m_bSelected);
	}
}

#pragma mark ----------------SoulListTable--------------------

SoulListTable::SoulListTable():
DLayer(),
m_pTableView(NULL),
m_pData(NULL),
m_pSoulList(NULL),
m_pDelegate(NULL),
m_bIsReverse(false),
m_pCell(NULL)
{
    
}

SoulListTable::~SoulListTable()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RELEASE(m_pSoulList);
}

SoulListTable * SoulListTable::create(cocos2d::CCSize size)
{
    SoulListTable * pRet = new SoulListTable;
    if(pRet && pRet->init(size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    
    return pRet;
}

bool SoulListTable::init(cocos2d::CCSize size)
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pSoulList = CCArray::create();
    m_pSoulList->retain();
    
    m_pTableView = DTableView::create(this, size);
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(CCPointZero);
	m_pTableView->setDelegate(this);
	m_pTableView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	addChild(m_pTableView);
    
    m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_hp_cell");
    m_pData->retain();
    
    return true;
}

void SoulListTable::onEnter()
{
	BINDING_EXEC(DM_GET_WARRIOR_MODEL, this, SoulListTable::updateView, D_EVENT_WARRIORLIST_UPDATE);
    BINDING_EXEC(DM_GET_WARRIOR_MODEL, this, SoulListTable::updateView, D_EVENT_SOULLIST_UPDATE);
    BINDING_ONLY_O(DM_GET_WARRIOR_MODEL, this, SoulListTable::onWarriorRecruit, D_EVENT_WARRIOR_FINISH_RECRUIT);
    DLayer::onEnter();
}

void SoulListTable::onExit()
{
	//退出该界面后，清除已有的选择
	CC_SAFE_RELEASE_NULL(m_pCell);
    UNBINDING_ALL(DM_GET_WARRIOR_MODEL,this);
    DLayer::onExit();
}

cocos2d::CCSize SoulListTable::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return SOUL_CELL_SIZE;
}

cocos2d::CCSize SoulListTable::cellSizeForTable(cocos2d::extension::CCTableView *table)
{
    return SOUL_CELL_SIZE;
}

cocos2d::extension::CCTableViewCell* SoulListTable::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    warrior::SoulInfo * pSoul = (warrior::SoulInfo *) m_pSoulList->objectAtIndex(idx);
    
    SoulCell * pCell = (SoulCell *)table->dequeueCell();
    if(pCell == NULL)
    {
        pCell = SoulCell::create(m_pData);
    }
    pCell->setDelegate(m_pDelegate);
    pCell->setSoul(pSoul);
    
    return pCell;
}

unsigned int SoulListTable::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return m_pSoulList->count();
}

void SoulListTable::updateView()
{
    if(!DM_GET_WARRIOR_MODEL->isSynced)
    {
        return;
    }
    
    CCArray * pSouls = DM_GET_WARRIOR_MODEL->getSoulVec();
    m_pSoulList->removeAllObjects();
    m_pSoulList->addObjectsFromArray(pSouls);
    
    if(m_bIsReverse)
    {
        m_pSoulList->reverseObjects();
    }
    
    m_pTableView->reloadData();
}

void SoulListTable::onWarriorRecruit(cocos2d::CCObject * pObj)
{
    updateView();
	if(m_pDelegate)
    {
        warrior::WarriorCurInfo * pWarrior = dynamic_cast<warrior::WarriorCurInfo * >(pObj);
        if(pWarrior != NULL)
        {
            m_pDelegate->showInfoView(kRecruitSettleView,pWarrior);
        }
    }
}

void SoulListTable::setSelectedCell(SoulCell * pCell,SoulCell * curCell,bool boo)
{
	if(m_pCell == pCell)
	{
		return;
	}

	if(m_pCell != NULL && !boo)
	{
		m_pCell->setSelected(false);
		m_pCell->release();
	}

	m_pCell = pCell;
	if(m_pCell != NULL)
	{
		m_pCell->setSelected(true);
		m_pCell->retain();
	}
}

#pragma mark ----------------SoulListView--------------------

SoulListView * SoulListView::create(const cocos2d::CCSize &size)
{
    SoulListView * pRet = new SoulListView;
    if(pRet && pRet->init(size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    return pRet;
}

bool SoulListView::init(const cocos2d::CCSize & size)
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pTable = SoulListTable::create(size);
    addChild(m_pTable);
    
    return true;
}

void SoulListView::onEnter()
{
    DLayer::onEnter();
}

void SoulListView::onExit()
{
    DLayer::onExit();
}

#pragma mark ----------------SoulSelectTable 多选列表--------------------
SoulSelectTable * SoulSelectTable::create(cocos2d::CCSize size)
{
	SoulSelectTable * pRet = new SoulSelectTable();
	if(pRet && pRet->init(size))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}
	return pRet;
}

SoulSelectTable::SoulSelectTable()
	:SoulListTable()
	,m_pSelectedSoul(NULL)
	,m_cSelecteMore(NULL)
	,m_bMoreSelete(false)
{

}

SoulSelectTable::~SoulSelectTable()
{
	CC_SAFE_RELEASE_NULL(m_pSelectedSoul);
	CC_SAFE_RELEASE_NULL(m_pData);
	if(m_cSelecteMore)
	{
		m_cSelecteMore->removeAllObjects();
	}
	CC_SAFE_RELEASE(m_cSelecteMore);
}

bool SoulSelectTable::init(cocos2d::CCSize size)
{
	if(!SoulListTable::init(size))
	{
		return false;
	}
	m_cSelecteMore = CCArray::create();
	CC_SAFE_RETAIN(m_cSelecteMore);
	//m_pData = readCCData("ui/ccb/warrior/warriorListSelectCell");
	//m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_xy_cell");
	CC_SAFE_RELEASE_NULL(m_pData);
	m_pData = readCCData("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_soul_cell");
	CC_SAFE_RETAIN(m_pData);
	return true;
}

void SoulSelectTable::onEnter()
{
	BINDING_EXEC(DM_GET_WARRIOR_MODEL, this, SoulSelectTable::updateView, D_EVENT_WARRIORLIST_UPDATE);
	BINDING_EXEC(DM_GET_WARRIOR_MODEL, this, SoulSelectTable::updateView, D_EVENT_SOULLIST_UPDATE);
	DLayer::onEnter();
}

void SoulSelectTable::updateView()
{
	if(!DM_GET_WARRIOR_MODEL->isSynced)
	{
		return;
	}

	CCArray * pSouls = DM_GET_WARRIOR_MODEL->getSoulVec();
	m_pSoulList->removeAllObjects();
	//m_pSoulList->addObjectsFromArray(pSouls);
	CCObject* obj=NULL;
	warrior::SoulInfo* info =NULL;
	warrior::WarriorGrowInfo * pGrowInfo = NULL;
	CCARRAY_FOREACH(pSouls,obj)
	{
		info = static_cast<warrior::SoulInfo*>(obj);
		pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(info->getBaseInfo()->getRealGrade());
		bool isSelect = DM_GET_BURN_MODEL->isSoulSelected(info->getWid());
		if(!isSelect && info->getNum()>=pGrowInfo->getRctSoul())
		{
			m_pSoulList->addObject(obj);
		}
	}
	if(m_bIsReverse)
	{
		m_pSoulList->reverseObjects();
	}

	m_pTableView->reloadData();
}

void SoulSelectTable::setSelectedCell(SoulCell * pCell,SoulCell * curCell,bool boo)
{
	if(m_bMoreSelete && m_cSelecteMore!=NULL && boo)
	{
		//多选执行
		if(DM_GET_BURN_MODEL->getBurnNum() + m_cSelecteMore->count()>=TOTAL_BURN_NUM)
		{
			//已达到最大选择数
			Tips_Alert(D_LOCAL_STRING("$LianhuaHeroEnough").c_str());
			return;
		}
	}
	SoulListTable::setSelectedCell(pCell,curCell,m_bMoreSelete);//是否可以多选
	CC_SAFE_RELEASE_NULL(m_pSelectedSoul);
	if(pCell != NULL)
	{
		m_pSelectedSoul = pCell->getSoul();
		CC_SAFE_RETAIN(m_pSelectedSoul);
	}
	if(NULL != curCell && m_bMoreSelete) 
	{
		changeSelectList(curCell,boo);
	}
}

//多选执行
void SoulSelectTable::changeSelectList(SoulCell * pCell,bool boo)
{
	if(pCell)
	{
		if(!boo)
		{
			if(m_cSelecteMore->containsObject(pCell->getSoul()))
			{
				m_cSelecteMore->removeObject(pCell->getSoul());
			}
		}
		else
		{
			m_cSelecteMore->addObject(pCell->getSoul());
		}

	}

	CCLOG(CCString::createWithFormat("soul-m_cSelecteMore:%d",m_cSelecteMore->count())->getCString());
}

cocos2d::extension::CCTableViewCell* SoulSelectTable::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	warrior::SoulInfo * pWarrior = (warrior::SoulInfo *) m_pSoulList->objectAtIndex(idx);

	SoulSelectCell * pCell = (SoulSelectCell *)table->dequeueCell();
	if(pCell == NULL)
	{
		pCell = SoulSelectCell::create(m_pData);
	}
	pCell->setSelectDelegate(this);
	pCell->setSoul(pWarrior);
	if(m_bMoreSelete)
	{
		if(m_cSelecteMore)
		{
			if(m_cSelecteMore->containsObject(pCell->getSoul()))
			{
				pCell->setSelected(true);
			}
			else
			{
				pCell->setSelected(false);
			}
		}
	}
	else
	{
		if(pWarrior == m_pSelectedSoul)
		{
			setSelectedCell(pCell);
			pCell->setSelected(true);
		}
		else
		{
			pCell->setSelected(false);
		}
	}
	return pCell;
}

unsigned int SoulSelectTable::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_pSoulList->count();
}
void SoulSelectTable::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{

}
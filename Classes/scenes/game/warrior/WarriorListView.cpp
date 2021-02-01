//
//  WarriorListView.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WarriorListView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../common/gui/WarriorTypeMark.h"
#include "Warrior.h"
#include <algorithm>

USING_NS_CC;
USING_NS_CC_EXT;

#define  WARRIOR_CELL_SIZE                      CCSizeMake(640, 130)    //单元大小
#define  WARRIOR_EXPLODE_CELL_SIZE              CCSizeMake(640, 210)    //展开单元大小
#define  WARRIOR_LIST_SELECT_SIZE               CCSizeMake(640, 535)    //弟子选择列表大小

#define D_EVENT_WARRIOR_SELECT_CHANGED     "warrior select changed"

#pragma mark ----------------WarriorCell 弟子单元基类------------------
WarriorCell * WarriorCell::create(cocos2d::extension::CCData *pData)
{
    WarriorCell * pRet = new WarriorCell();
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

WarriorCell::WarriorCell()
: CCTableViewCell()
, m_pWarrior(NULL)
, m_pName(NULL)
//, m_pRgbaGrade(NULL)
, m_pLevel(NULL)
, m_pUpLevel(NULL)
, m_pFight(NULL)
, m_pInFormation(NULL)
, m_pAvatorContainer(NULL)
//, m_pGradeLevel(NULL)
, m_pWarriorTypeMark(NULL)
, m_pListTableDelegate(NULL)
,m_pZizhiNum(NULL)
{
	m_gradeBgNodes.insert(m_gradeBgNodes.begin(), warrior::kWarGradeRed + 1, (CCNode *)NULL);
}

WarriorCell::~WarriorCell()
{
    removeAllChildren();
    CC_SAFE_RELEASE_NULL(m_pWarrior);
    CC_SAFE_RELEASE_NULL(m_pName);
	//CC_SAFE_RELEASE_NULL(m_pRgbaGrade);
    CC_SAFE_RELEASE_NULL(m_pLevel);
    CC_SAFE_RELEASE_NULL(m_pUpLevel);
    CC_SAFE_RELEASE_NULL(m_pFight);
    CC_SAFE_RELEASE_NULL(m_pInFormation);
    CC_SAFE_RELEASE_NULL(m_pAvatorContainer);
   // CC_SAFE_RELEASE_NULL(m_pGradeLevel);
    CC_SAFE_RELEASE_NULL(m_pWarriorTypeMark);
	CC_SAFE_RELEASE_NULL(m_pZizhiNum);

	for (uint32_t i = 0; i < m_gradeBgNodes.size(); i++)
	{
		CC_SAFE_RELEASE_NULL(m_gradeBgNodes[i]);
	}
}

bool WarriorCell::init(cocos2d::extension::CCData *pData)
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

void WarriorCell::onEnter()
{
    CCTableViewCell::onEnter();
}

void WarriorCell::onExit()
{
    CCTableViewCell::onExit();
}

bool WarriorCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, m_pName);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpriteGrade", CCNodeRGBA *, m_pRgbaGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeGreen", CCNode *, m_gradeBgNodes[warrior::kWarGradeGreen]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeBlue", CCNode *, m_gradeBgNodes[warrior::kWarGradeBlue]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradePurple", CCNode *, m_gradeBgNodes[warrior::kWarGradePurple]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeOrange", CCNode *, m_gradeBgNodes[warrior::kWarGradeRed]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, m_pUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, m_pFight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInFormation", CCSprite *, m_pInFormation);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer", CCNode *, m_pAvatorContainer);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, m_pGradeLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum" , CCLabelBMFont*, m_pZizhiNum);
    return false;
}

void WarriorCell::updateView()
{
    
    m_pName->setString(m_pWarrior->getBaseInfo()->getWarriorName().c_str());
    
    m_pLevel->setString(CCString::createWithFormat("%d", m_pWarrior->getWarriorLv())->getCString());
	if(m_pZizhiNum) 
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pWarrior->getBaseInfo()->getQuality())->getCString());
	}
    
	if(m_pUpLevel)
		m_pUpLevel->setString(CCString::createWithFormat("+%d", m_pWarrior->getWarriorUpLv())->getCString());
    //m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(m_pWarrior->getWarriorUpLv()));
    
    m_pFight->setString(CCString::createWithFormat("%d", m_pWarrior->computeWarriorForce())->getCString());
    
	if(m_pInFormation)
		m_pInFormation->setVisible(DM_GET_FORMATION_MODEL->isWarriorBattle(m_pWarrior->getWid()));
    
    m_pAvatorContainer->removeAllChildren();
    m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pWarrior->getAvatarId()));
	for (uint32_t i = 0; i < m_gradeBgNodes.size(); i++)
	{
		if (m_gradeBgNodes[i])
		{
			m_gradeBgNodes[i]->setVisible(false);
		}
	}

	if (m_gradeBgNodes[m_pWarrior->getBaseInfo()->getColorGrade()])
	{
		m_gradeBgNodes[m_pWarrior->getBaseInfo()->getColorGrade()]->setVisible(true);
	}
	
	//m_pRgbaGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByColorGrade(m_pWarrior->getBaseInfo()->getColorGrade()));
    //m_pGradeLevel->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByColorGrade(m_pWarrior->getBaseInfo()->getColorGrade()));
}

void WarriorCell::setWarrior(warrior::WarriorCurInfo *pWarrior, bool updateView)
{
    if(m_pWarrior == pWarrior) return;
    CC_SAFE_RETAIN(pWarrior);
    CC_SAFE_RELEASE_NULL(m_pWarrior);
    m_pWarrior = pWarrior;    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarrior->getBaseInfo());
    
    if(updateView)
    {
        this->updateView();
    }
}

void WarriorCell::setSelected(bool selected)
{
    
}

CCNode* WarriorCell::getGradeNodeByColorGrade( warrior::WarriorGrade grade )
{
	if (grade < warrior::kWarGradeGreen || m_gradeBgNodes.size() < grade)
	{
		return NULL;
	}
	
	return m_gradeBgNodes[grade];
}



#pragma mark ----------------WarriorListCell 弟子列表单元------------------
WarriorListCell * WarriorListCell::create(cocos2d::extension::CCData *pData)
{
    WarriorListCell * pRet = new WarriorListCell();
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

WarriorListCell::WarriorListCell()
: WarriorCell()
, m_pUpAvailable(NULL)
, m_pExplodeContainer(NULL)
, m_pStrengthen(NULL)
, m_pDevelop(NULL)
, m_pPromote(NULL)
, m_pAvatarButton(NULL)
, m_pExplodeButton(NULL)
, m_pRoot(NULL)
//, m_pPromoteContainer(NULL)
, m_pWarriorDelegate(NULL)
, m_bExploded(false)
{
    
}


WarriorListCell::~WarriorListCell()
{
    CC_SAFE_RELEASE_NULL(m_pUpAvailable);
    CC_SAFE_RELEASE_NULL(m_pExplodeContainer);
    CC_SAFE_RELEASE_NULL(m_pStrengthen);
    CC_SAFE_RELEASE_NULL(m_pDevelop);
    CC_SAFE_RELEASE_NULL(m_pPromote);
    CC_SAFE_RELEASE_NULL(m_pAvatarButton);
    CC_SAFE_RELEASE_NULL(m_pExplodeButton);
    CC_SAFE_RELEASE_NULL(m_pRoot);
   // CC_SAFE_RELEASE_NULL(m_pPromoteContainer);
}

bool WarriorListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpAvailable", CCSprite *, m_pUpAvailable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExplodeContainer", CCNode *, m_pExplodeContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStrengthen", DButton *, m_pStrengthen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDevelop", DButton *, m_pDevelop);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPromote", DButton *, m_pPromote);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatarButton", CCMenuItem *, m_pAvatarButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExplodeButton", CCMenuItem *, m_pExplodeButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRoot", CCNode *, m_pRoot);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPromoteContainer", CCNode *, m_pPromoteContainer);
    
    return WarriorCell::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

void WarriorListCell::setWarrior(warrior::WarriorCurInfo * pWarrior, bool updateView)
{
    UNBINDING_ALL(m_pWarrior, this);
    WarriorCell::setWarrior(pWarrior, false);
    binding();
    if(updateView)
    {
        this->updateView();
    }
}

void WarriorListCell::binding()
{
    BINDING_ONLY(m_pWarrior, this, WarriorListCell::updateView, D_EVENT_WARRIORINFO_UPDATE);
}

void WarriorListCell::onEnter()
{
    binding();
    WarriorCell::onEnter();
}

void WarriorListCell::onExit()
{
    UNBINDING_ALL(m_pWarrior, this);
    WarriorCell::onExit();
}

void WarriorListCell::updateView()
{
    WarriorCell::updateView();    
    m_pUpAvailable->setVisible(DM_GET_WARRIOR_MODEL->canWarriorUpNow(m_pWarrior) == warrior::kUpErrorNone);
        
    m_pAvatarButton->setTarget(this, menu_selector(WarriorListCell::onAvatarClick));
    m_pExplodeButton->setTarget(this, menu_selector(WarriorListCell::onExplodeClick));
    
    updatePos();
}

void WarriorListCell::updatePos()
{
    if(m_bExploded)
    {
        m_pStrengthen->getButton()->setTarget(this, menu_selector(WarriorListCell::onTransferClick));
        m_pDevelop->getButton()->setTarget(this, menu_selector(WarriorListCell::onDevelopClick));
        m_pPromote->getButton()->setTarget(this, menu_selector(WarriorListCell::onPromoteClick));
        m_pPromote->getButton()->setEnabled(m_pWarrior->isPromoteable() && !m_pWarrior->checkPromoteLimit());
        m_pStrengthen->getButton()->setEnabled(!m_pWarrior->checkLevelLimit());
        //        m_pPromoteContainer->setVisible(m_pWarrior->isPromoteable() && !m_pWarrior->checkPromoteLimit());
        
        static float dy = WARRIOR_EXPLODE_CELL_SIZE.height - WARRIOR_CELL_SIZE.height;
        m_pRoot->setPositionY(dy);
    }
    else
    {
        m_pRoot->setPositionY(0);
    }
    m_pExplodeContainer->setVisible(m_bExploded);
}

void WarriorListCell::setSelected(bool selected)
{
    m_bExploded = selected;
}

void WarriorListCell::onTransferClick(cocos2d::CCObject *sender)
{
    m_pWarriorDelegate->showInfoView(kUpgradeView, m_pWarrior);
}

void WarriorListCell::onDevelopClick(cocos2d::CCObject *sender)
{
    if(!m_pWarrior->checkDevelopLevel())
    {
        Tips_Alert(D_LOCAL_STRING("WarriorLevel%dCantDev",WARRIOR_DEV_LV_MIN).c_str());
        return;
    }
    m_pWarriorDelegate->showInfoView(kDevelopView, m_pWarrior);
}

void WarriorListCell::onPromoteClick(cocos2d::CCObject *sender)
{
    m_pWarriorDelegate->showInfoView(kPromotionView, m_pWarrior);
}

void WarriorListCell::onAvatarClick(cocos2d::CCObject *sender)
{
    m_pWarriorDelegate->showInfoView(kWarriorInfo, m_pWarrior);
}

void WarriorListCell::onExplodeClick(cocos2d::CCObject *sender)
{
    m_bExploded = !m_bExploded;
    if(m_pListTableDelegate != NULL)
    {
        m_pListTableDelegate->setSelectedCell(m_bExploded ? this : NULL);
    }
}

#pragma mark ----------------WarriorListCellSelect 弟子选择列表单元------------------
WarriorListCellSelect * WarriorListCellSelect::create(cocos2d::extension::CCData *pData)
{
    WarriorListCellSelect * pRet = new WarriorListCellSelect();
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

WarriorListCellSelect::WarriorListCellSelect()
: WarriorCell()
, m_pSelect(NULL)
, m_bSelected(false)
{
    
}

WarriorListCellSelect::~WarriorListCellSelect()
{
    CC_SAFE_RELEASE_NULL(m_pSelect);
}

bool WarriorListCellSelect::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelect", CCMenuItemImage *, m_pSelect);
    
    return WarriorCell::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_MenuHandler WarriorListCellSelect::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectClick", WarriorListCellSelect::onSelectClick);
    return NULL;
}

void WarriorListCellSelect::updateView()
{
    WarriorCell::updateView();
    
	if(m_bSelected)
	{
		m_pSelect->selected();
	}
	else
	{
		m_pSelect->unselected();
	}
}

void WarriorListCellSelect::setSelected(bool selected)
{
	m_bSelected = selected;
	if(m_bSelected)
	{
		m_pSelect->selected();
	}
	else
	{
		m_pSelect->unselected();
	}
}

void WarriorListCellSelect::onSelectClick(cocos2d::CCObject *sender)
{
    m_bSelected = !m_bSelected;
    if(m_pListTableDelegate != NULL)
    {
        m_pListTableDelegate->setSelectedCell(m_bSelected ? this : NULL,this,m_bSelected);
    }
}
#pragma mark ----------------FiltableWarriorList 弟子列表------------------

FiltableWarriorList::FiltableWarriorList():
DLayer(),
m_pTableView(NULL),
m_pFilter(NULL),
m_pData(NULL),
m_pWarriorList(NULL),
m_pCell(NULL),
m_bIsReverse(false)
{
    
}

FiltableWarriorList::~FiltableWarriorList()
{
    removeAllChildren();
    CC_SAFE_RELEASE_NULL(m_pFilter);
    CC_SAFE_RELEASE_NULL(m_pWarriorList);
    //CC_SAFE_RELEASE_NULL(m_pCell);
}

bool FiltableWarriorList::init(cocos2d::CCSize size)
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pWarriorList = CCArray::create();
    m_pWarriorList->retain();
    
    m_pTableView = DTableView::create(this, size);
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setAnchorPoint(CCPointZero);
	m_pTableView->setPosition(CCPointZero);
	m_pTableView->setDelegate(this);
	m_pTableView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	addChild(m_pTableView);
    
    return true;
}

void FiltableWarriorList::onEnter()
{
	BINDING_EXEC(DM_GET_WARRIOR_MODEL, this, FiltableWarriorList::updateView, D_EVENT_WARRIORLIST_UPDATE);
    DLayer::onEnter();
}

void FiltableWarriorList::onExit()
{
	//退出该界面后，清除已有的选择
	CC_SAFE_RELEASE_NULL(m_pCell);
    UNBINDING_ALL(DM_GET_WARRIOR_MODEL,this);
    DLayer::onExit();
}

cocos2d::CCSize FiltableWarriorList::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return WARRIOR_CELL_SIZE;
}

cocos2d::CCSize FiltableWarriorList::cellSizeForTable(cocos2d::extension::CCTableView *table)
{
    return WARRIOR_CELL_SIZE;
}

unsigned int FiltableWarriorList::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return m_pWarriorList->count();
}

void FiltableWarriorList::setFilter(WarriorFilterDelegate *pFilter)
{
    CC_SAFE_RETAIN(pFilter);
    CC_SAFE_RELEASE_NULL(m_pFilter);
    m_pFilter = pFilter;
    
}

void FiltableWarriorList::setSelectedCell(WarriorCell * pCell,WarriorCell * curCell,bool boo)
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

static int WarriorListSort(const CCObject* p1, const CCObject* p2)
{
	warrior::WarriorCurInfo * pWarrior1 = (warrior::WarriorCurInfo*)p1;
	warrior::WarriorCurInfo * pWarrior2 = (warrior::WarriorCurInfo*)p2;

	uint8_t b1 = (int)(DM_GET_FORMATION_MODEL->isWarriorBattle(pWarrior1->getWid()));
	uint8_t b2 = (int)(DM_GET_FORMATION_MODEL->isWarriorBattle(pWarrior2->getWid()));
	//return b1 > b2;
	if(b1==b2)
	{
		return static_cast<int>(pWarrior1->getBaseInfo()->getColorGrade()) > static_cast<int>(pWarrior2->getBaseInfo()->getColorGrade());
	}

	return b1>b2;
}

/**战斗力排序**/
static int sortWarriorByFight(const CCObject* p1, const CCObject* p2)
{
	warrior::WarriorCurInfo * pWarrior1 = (warrior::WarriorCurInfo*)p1;
	warrior::WarriorCurInfo * pWarrior2 = (warrior::WarriorCurInfo*)p2;
	return pWarrior1->computeWarriorForce()>pWarrior2->computeWarriorForce();
}

/**等级排序**/
static int sortWarriorByLevel(const CCObject* p1, const CCObject* p2)
{
	warrior::WarriorCurInfo * pWarrior1 = (warrior::WarriorCurInfo*)p1;
	warrior::WarriorCurInfo * pWarrior2 = (warrior::WarriorCurInfo*)p2;
	return pWarrior1->getWarriorLv()>pWarrior2->getWarriorLv();
}

/**品质排序**/
static int sortWarriorByGrade(const CCObject* p1, const CCObject* p2)
{
	warrior::WarriorCurInfo * pWarrior1 = (warrior::WarriorCurInfo*)p1;
	warrior::WarriorCurInfo * pWarrior2 = (warrior::WarriorCurInfo*)p2;
	return static_cast<int>(pWarrior1->getBaseInfo()->getColorGrade()) > static_cast<int>(pWarrior2->getBaseInfo()->getColorGrade());
}

/**资质排序**/
static int sortWarriorByZizhi(const CCObject* p1, const CCObject* p2)
{
	warrior::WarriorCurInfo * pWarrior1 = (warrior::WarriorCurInfo*)p1;
	warrior::WarriorCurInfo * pWarrior2 = (warrior::WarriorCurInfo*)p2;
	return pWarrior1->getBaseInfo()->getQuality() > pWarrior2->getBaseInfo()->getQuality();
}

void FiltableWarriorList::updateView()
{
    if(!DM_GET_WARRIOR_MODEL->isSynced)
    {
        return;
    }
    
    CCArray * pWarriors = DM_GET_WARRIOR_MODEL->getWarriorVec();
    m_pWarriorList->removeAllObjects();
    CCObject * obj(NULL);
    CCARRAY_FOREACH(pWarriors, obj)
    {
        warrior::WarriorCurInfo * pWarrior = (warrior::WarriorCurInfo*)obj;
		CCLOG(CCString::createWithFormat("HeroList WARRIORID %d",pWarrior->getWid())->getCString());
        if(m_pFilter == NULL || (*m_pFilter)(pWarrior))
        {
            m_pWarriorList->addObject(pWarrior);
        }
    }
    if(m_bIsReverse)
    {
        m_pWarriorList->reverseObjects();
    }

	std::sort(m_pWarriorList->data->arr, m_pWarriorList->data->arr + m_pWarriorList->data->num, WarriorListSort);
   
    m_pTableView->reloadData();
}

void FiltableWarriorList::setSortList(warrior::WarriorSortType type)
{
	switch(type)
	{
	case warrior::kSortTypeLvl:
		std::sort(m_pWarriorList->data->arr, m_pWarriorList->data->arr + m_pWarriorList->data->num, sortWarriorByLevel);
		break;
	case warrior::kSortTypeGrade:
		std::sort(m_pWarriorList->data->arr, m_pWarriorList->data->arr + m_pWarriorList->data->num, sortWarriorByGrade);
		break;
	case warrior::kSortTypeZizhi:
		std::sort(m_pWarriorList->data->arr, m_pWarriorList->data->arr + m_pWarriorList->data->num, sortWarriorByZizhi);
		break;
	case warrior::kSortTypeFight:
		std::sort(m_pWarriorList->data->arr, m_pWarriorList->data->arr + m_pWarriorList->data->num, sortWarriorByFight);
		break;
	}
	m_pTableView->reloadData();
	m_pTableView->setContentOffset(m_pTableView->minContainerOffset());
}

#pragma mark ----------------WarriorListTable 弟子列表------------------

WarriorListTable * WarriorListTable::create(cocos2d::CCSize size)
{
    WarriorListTable * pRet = new WarriorListTable;
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

WarriorListTable::WarriorListTable():
FiltableWarriorList(),
m_pExplodedWarrior(NULL),
m_pWarriorDelegate(NULL)
{
    
}

WarriorListTable::~WarriorListTable()
{
    CC_SAFE_RELEASE_NULL(m_pExplodedWarrior);
    CC_SAFE_RELEASE_NULL(m_pData);
}

bool WarriorListTable::init(cocos2d::CCSize size)
{
    if(!FiltableWarriorList::init(size))
    {
        return false;
    }
    
    m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_xy_cell");
	//m_pData = readCCData("ui/ccb/warrior/warriorListCell");
	
    CC_SAFE_RETAIN(m_pData);	
    
    return true;
}

void WarriorListTable::setSelectedCell(WarriorCell * pCell,WarriorCell * curCell,bool boo)
{
    WarriorCell * pOldCell = m_pCell;
    
    FiltableWarriorList::setSelectedCell(pCell);
    CC_SAFE_RELEASE_NULL(m_pExplodedWarrior);
    
    if(pOldCell != NULL && pOldCell && pOldCell->getIdx() < m_pWarriorList->count())
    {
        m_pTableView->updateCellPositions(pOldCell);
    }
    
    if(pCell  != NULL)
    {
        m_pExplodedWarrior = pCell->getWarrior();
        m_pExplodedWarrior->retain();
        m_pTableView->updateCellPositions(pCell);
    }
    m_pTableView->reloadData();
}

cocos2d::CCSize WarriorListTable::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    warrior::WarriorCurInfo * pWarrior = (warrior::WarriorCurInfo *) m_pWarriorList->objectAtIndex(idx);
    if(pWarrior == m_pExplodedWarrior)
    {
        return WARRIOR_EXPLODE_CELL_SIZE;
    }
    else
    {
        return WARRIOR_CELL_SIZE;
    }
}

cocos2d::extension::CCTableViewCell* WarriorListTable::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    warrior::WarriorCurInfo * pWarrior = (warrior::WarriorCurInfo *) m_pWarriorList->objectAtIndex(idx);
    
    WarriorListCell * pCell = (WarriorListCell *)table->dequeueCell();
    if(pCell == NULL)
    {
        pCell = WarriorListCell::create(m_pData);
    }
    WarriorListTableDelegate * pDelegate = this;
    pCell->setDelegate(pDelegate);
    pCell->setDelegate(m_pWarriorDelegate);
    pCell->setWarrior(pWarrior);
    
    if(pWarrior == m_pExplodedWarrior)
    {
        pCell->setExploded(true);
    }
    else
    {
        pCell->setExploded(false);
    }
    
    pCell->updatePos();
    
    return pCell;
}

void WarriorListTable::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{
    
}

#pragma mark ----------------WarriorListSelectTable 弟子选择列表------------------

WarriorListSelectTable * WarriorListSelectTable::create(cocos2d::CCSize size)
{
    WarriorListSelectTable * pRet = new WarriorListSelectTable;
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

WarriorListSelectTable::WarriorListSelectTable():
FiltableWarriorList(),
m_pSelectedWarrior(NULL)
,m_cSelecteMore(NULL)
,m_bMoreSelete(false)
{
    
}

WarriorListSelectTable::~WarriorListSelectTable()
{
    CC_SAFE_RELEASE_NULL(m_pSelectedWarrior);
    CC_SAFE_RELEASE_NULL(m_pData);
	if(m_cSelecteMore)
	{
		m_cSelecteMore->removeAllObjects();
	}
	CC_SAFE_RELEASE(m_cSelecteMore);
}

bool WarriorListSelectTable::init(cocos2d::CCSize size)
{
    if(!FiltableWarriorList::init(size))
    {
        return false;
    }
    
    //m_pData = readCCData("ui/ccb/warrior/warriorListSelectCell");
	//m_pData = readCCData("PL_ui/ccb/pl_hero/PL_hero_xy_cell");
	m_pData = readCCData("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_liebiao_cell");
    CC_SAFE_RETAIN(m_pData);
    return true;
}

void WarriorListSelectTable::setSelectedCell(WarriorCell * pCell,WarriorCell * curCell,bool boo)
{
    FiltableWarriorList::setSelectedCell(pCell);//是否可以多选
    CC_SAFE_RELEASE_NULL(m_pSelectedWarrior);
    if(pCell != NULL)
    {
        m_pSelectedWarrior = pCell->getWarrior();
        CC_SAFE_RETAIN(m_pSelectedWarrior);
    }
}

cocos2d::extension::CCTableViewCell* WarriorListSelectTable::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    warrior::WarriorCurInfo * pWarrior = (warrior::WarriorCurInfo *) m_pWarriorList->objectAtIndex(idx);
    
    WarriorListCellSelect * pCell = (WarriorListCellSelect *)table->dequeueCell();
    if(pCell == NULL)
    {
        pCell = WarriorListCellSelect::create(m_pData);
    }
    pCell->setDelegate(this);
    pCell->setWarrior(pWarrior);
    
	if(pWarrior == m_pSelectedWarrior)
	{
		setSelectedCell(pCell);
		pCell->setSelected(true);
	}
	else
	{
		pCell->setSelected(false);
	}
    return pCell;
}

void WarriorListSelectTable::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{
    
}

#pragma mark ----------------WarriorListView 弟子列表----------------------

WarriorListView * WarriorListView::create(WarriorDelegate * pWarriorDelegate, WarriorFilterDelegate * pFilter, const CCSize & size)
{
    WarriorListView * pRet = new WarriorListView;
    if(pRet && pRet->init(pWarriorDelegate, pFilter, size))
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

bool WarriorListView::init(WarriorDelegate * pWarriorDelegate,WarriorFilterDelegate *pFilter, const CCSize & size)
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pTable = WarriorListTable::create(size);
    m_pTable->setFilter(pFilter);
    m_pTable->setDelegate(pWarriorDelegate);
    addChild(m_pTable);
    
    return true;
}

void WarriorListView::onEnter()
{
    DLayer::onEnter();
}

void WarriorListView::onExit()
{
    DLayer::onExit();
}

void WarriorListView::setSortList(warrior::WarriorSortType type)
{
	m_pTable->setSortList(type);
}

#pragma mark ----------------WarriorSelectPopup 弟子选择弹出------------------
WarriorSelectPopup::WarriorSelectPopup()
: DLayer()
, m_pSelectTable(NULL)
, m_pContainer(NULL)
, m_pConfirmButton(NULL)
, m_pBackButton(NULL)
,isMoreSelet(false)
{
    
}

WarriorSelectPopup::~WarriorSelectPopup()
{
    CC_SAFE_RELEASE_NULL(m_pContainer);
    CC_SAFE_RELEASE_NULL(m_pConfirmButton);
    CC_SAFE_RELEASE_NULL(m_pBackButton);
}
WarriorSelectPopup * WarriorSelectPopup::create(WarriorFilterDelegate * pFilter)
{
    WarriorSelectPopup * pRet = new WarriorSelectPopup;
    if(pRet && pRet->init(pFilter))
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

bool WarriorSelectPopup::init(WarriorFilterDelegate *pFilter)
{
    if(!DLayer::init())
    {
        return false;
    }
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/warrior/warriorListSelect");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhuanhun/PL_zhuanhun_liebiao");
    
    //m_pSelectTable = WarriorListSelectTable::create(m_pContainer->getContentSize());
	m_pSelectTable = WarriorListSelectTable::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	
    m_pSelectTable->setFilter(pFilter);
    m_pContainer->addChild(m_pSelectTable);
    
    return true;
}

void WarriorSelectPopup::onEnter()
{
    m_pConfirmButton->getButton()->setTarget(this, menu_selector(WarriorSelectPopup::onConfirmClick));
    m_pBackButton->getButton()->setTarget(this, menu_selector(WarriorSelectPopup::onBackClick));
    DLayer::onEnter();
	Game::sharedGame()->setNavigatorVisable(false);
}

void WarriorSelectPopup::onExit()
{
	Game::sharedGame()->setNavigatorVisable(true);
    DLayer::onExit();
}

bool WarriorSelectPopup::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton *, m_pConfirmButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", DButton *, m_pBackButton);
    
    return false;
}

void WarriorSelectPopup::setFilter(WarriorFilterDelegate *pFilter)
{
    m_pSelectTable->setFilter(pFilter);
}

void WarriorSelectPopup::setSelectedWarrior(warrior::WarriorCurInfo *pSelectedWarrior)
{
    m_pSelectTable->setSelectedWarrior(pSelectedWarrior);
}

void WarriorSelectPopup::onConfirmClick(cocos2d::CCObject *sender)
{
	m_pDelegate->setSelectedWarrior(m_pSelectTable->getSelectedWarrior());
    removeFromParent();
}

void WarriorSelectPopup::onBackClick(cocos2d::CCObject *sender)
{
    m_pDelegate->setSelectedWarrior(NULL);
    removeFromParent();
}


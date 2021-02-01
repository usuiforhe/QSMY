//
//  OtherFormationView.cpp
//  QSMY
//
//  Created by cll on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "OtherFormationView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;

#define PAGE_MARGIN 50
#define PAGE_SIZE_WIDTH (220 + PAGE_MARGIN*2)


#pragma mark - OtherFormationView
#pragma mark - - init etc.
OtherFormationView::OtherFormationView()
:m_pFormationListView(NULL)
,m_pPageView(NULL)
,m_iCurrentPage(0)
,m_pWarriorInfoViews(NULL)
,m_pWarriorListContainer(NULL)
,m_pWarriorInfoContainer(NULL)
,m_pAttack(NULL)
,m_pSpeed(NULL)
,m_pDefence(NULL)
,m_pHp(NULL)
,m_pPrice(NULL)
,m_pTotalPrice(NULL)
,m_pSkillBox(NULL)
,m_pSkillBoxSP(NULL)
,m_pWarriorData(NULL)
,m_pUpLevel(NULL)
,m_pFormationInfo(NULL)
{
    D_RESET_C_ARRAY(m_pEquipBox,EQUIP_BOX_COUNT);
    D_RESET_C_ARRAY(m_pEquipNone,EQUIP_BOX_COUNT);
	D_RESET_C_ARRAY(m_pFate,MAX_FATE_COUNT);
	D_RESET_C_ARRAY(m_pFateHighlight,MAX_FATE_COUNT);
    
}

OtherFormationView::~OtherFormationView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pWarriorInfoViews);
    CC_SAFE_RELEASE(m_pWarriorListContainer);
    CC_SAFE_RELEASE(m_pWarriorInfoContainer);
    CC_SAFE_RELEASE(m_pAttack);
    CC_SAFE_RELEASE(m_pSpeed);
    CC_SAFE_RELEASE(m_pDefence);
    CC_SAFE_RELEASE(m_pHp);
    
    CC_SAFE_RELEASE(m_pPrice);
    CC_SAFE_RELEASE(m_pTotalPrice);
    CC_SAFE_RELEASE(m_pSkillBox);
    CC_SAFE_RELEASE(m_pSkillBoxSP);
    CC_SAFE_RELEASE(m_pWarriorData);
    CC_SAFE_RELEASE(m_pUpLevel);
    D_SAFE_RELEASE_C_ARRAY(m_pEquipBox,EQUIP_BOX_COUNT);
    D_SAFE_RELEASE_C_ARRAY(m_pEquipNone,EQUIP_BOX_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pFate,MAX_FATE_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pFateHighlight,MAX_FATE_COUNT);
    
    SAFE_RELEASE_UNBINDING_ALL(m_pFormationInfo, this);
}

bool OtherFormationView::init()
{
    if(!DPopup::init())
    {
        return false;
    }
    
    CC_SAFE_RELEASE(m_pWarriorInfoViews);
    m_pWarriorInfoViews = CCArray::create();
    CC_SAFE_RETAIN(m_pWarriorInfoViews);
    
    removeAllChildren();
    CCB_READER_AND_ADDCHILD("ui/ccb/formation/otherFormation");
    
    CC_SAFE_RELEASE(m_pWarriorData);
    m_pWarriorData = readCCData("ui/ccb/formation/otherWarriorShow");
    CC_SAFE_RETAIN(m_pWarriorData);
    
    /**
     *   初始化弟子展示界面
     */
    
    m_pPageView = DPageView::create(this,m_pWarriorInfoContainer->getContentSize());
    m_pPageView->setContentOffset(CCPointZero);
    m_pPageView->setTouchEnabled(false);
    m_pPageView->setDelegate(this);
    m_pPageView->setFade(false);
    m_pWarriorInfoContainer->addChild(m_pPageView);
    
    /**
     *   初始化弟子列表
     */
    m_pFormationListView = OtherFormationListView::create();
    m_pFormationListView->setDelegate(this);
    m_pWarriorListContainer->addChild(m_pFormationListView);
    
//     m_fateActive = m_pFate[0]->getColor();
//     
//     float gray = m_fateActive.r*0.299f + m_fateActive.g*0.587f + m_fateActive.b*0.114f;
//     m_fateDeActive.r = m_fateDeActive.g = m_fateDeActive.b = gray;
    
    for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
    {
        m_pEquipBox[i]->getTouchMenu()->setEnabled(false);
		m_pEquipBox[i]->setTypeNum(i);
		m_pEquipBox[i]->setShowCanEnhance(false);
        //m_pEquipNone[i]->setVisible(true);
    }
    
	this->m_pSkillBox->getTouchMenu()->setEnabled(false);
	m_pSkillBox->setShowCanEnhance(false);
	this->m_pSkillBoxSP->getTouchMenu()->setEnabled(false);
	m_pSkillBoxSP->setShowCanEnhance(false);
    
    return true;
}

bool OtherFormationView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorListContainer", CCNode *, this->m_pWarriorListContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorInfoContainer", CCNode *, this->m_pWarriorInfoContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCNode *, this->m_pUpLevel);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont *, this->m_pPrice);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTotalPrice", CCLabelBMFont *, this->m_pTotalPrice);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox", SkillBox *, this->m_pSkillBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBoxSP", SkillBox *, this->m_pSkillBoxSP);

	D_CCB_ASSIGN_ARRAY("m_pFate", CCLabelTTF *, m_pFate, MAX_FATE_COUNT);
	D_CCB_ASSIGN_ARRAY("m_pFateHighlight", CCLabelTTF *, m_pFateHighlight, MAX_FATE_COUNT);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox1", EquipBox *, this->m_pEquipBox[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox2", EquipBox *, this->m_pEquipBox[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox3", EquipBox *, this->m_pEquipBox[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox4", EquipBox *, this->m_pEquipBox[3]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone1", CCNode *, this->m_pEquipNone[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone2", CCNode *, this->m_pEquipNone[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone3", CCNode *, this->m_pEquipNone[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone4", CCNode *, this->m_pEquipNone[3]);
    
    return false;
}

SEL_MenuHandler OtherFormationView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", OtherFormationView::closeCallBack);
    return NULL;
}

void OtherFormationView::closeCallBack(cocos2d::CCObject *obj)
{
	closePopup();
}

void OtherFormationView::onExit()
{
    reset();
    SAFE_RELEASE_UNBINDING_ALL(m_pFormationInfo, this);
    m_pFormationInfo = NULL;
    DPopup::onExit();
    ResourceManager::sharedResourceManager()->clearCache();
}

void OtherFormationView::onEnter()
{
    
    if(!m_pFormationInfo->getIsRobot())
    {
        if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut())
        {
            BINDING_ONLY(m_pFormationInfo, this, OtherFormationView::updateFormation, D_EVENT_OTHERFORMATION_UPDATE);
            HTTP_CLIENT->getOtherFormation(m_pFormationInfo->getID());
        }
    }
    updateFormation();
    DPopup::onEnter();
}

void OtherFormationView::updateFormation()
{
    
    showFormations();
    updateView();
}

void OtherFormationView::reset()
{
    m_pPageView->setCurrentPage(0);
    m_iCurrentPage = 0;
    m_pFormationListView->reset();
    m_pWarriorInfoViews->removeAllObjects();
    m_pPageView->reloadData();
}



float OtherFormationView::cellSizeForTable(DPageView *view)
{
    return PAGE_SIZE_WIDTH+PAGE_MARGIN;
}

cocos2d::CCNode* OtherFormationView::cellAtIndex(DPageView *view, unsigned int idx)
{
    return (cocos2d::CCNode*)m_pWarriorInfoViews->objectAtIndex(idx);
}

uint32_t OtherFormationView::numberOfCellsInTableView(DPageView *view)
{
    return m_pWarriorInfoViews->count();
}

#pragma mark - - CCScrollViewDelegate
void OtherFormationView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    setCurrentPage(m_pPageView->getCurrentPage());
}

#pragma mark - - methods private


int  OtherFormationView::pageToPositionIdx(const int &page)
{
    uint32_t count = m_pFormationInfo->getPositionsCount();
    int counter = -1;
    for(uint32_t i = 0; i < count; ++i)
    {
        counter++;
        if(counter==page)
            return i;
    }
    return -1;
}

void OtherFormationView::updateView()
{
    updateInfo();
    
    //更新pageview的信息
    m_pPageView->setCurrentPage(m_iCurrentPage);
    
    //更新弟子列表信息
    m_pFormationListView->setFormationInfo(m_pFormationInfo);
    m_pFormationListView->updateView();
}

void OtherFormationView::updateInfo()
{
    m_pUpLevel->removeAllChildren();
    
    do
    {
        //更新阵容位置上的装备 技能 弟子属性的等信息
        formation::PositionInfo *pPositionInfo = m_pFormationInfo->getPositionInfoByIndex(pageToPositionIdx(m_iCurrentPage));
        CC_BREAK_IF(pPositionInfo==NULL);
        
        otherFormation::WarriorPropInfo* pWarriorInfo = m_pFormationInfo->getWarriorPropInfoByWid(pPositionInfo->iWarriorId);
        CC_BREAK_IF(pWarriorInfo==NULL);
        
        int nTotalPrice = m_pFormationInfo->getFormationTotalForce();
        m_pTotalPrice->setString(CCString::createWithFormat("%d",nTotalPrice)->getCString());
        
        m_pAttack->setString(CCString::createWithFormat("%d",pWarriorInfo->att)->getCString());
        m_pDefence->setString(CCString::createWithFormat("%d",pWarriorInfo->def)->getCString());
        m_pHp->setString(CCString::createWithFormat("%d",pWarriorInfo->hp)->getCString());
        m_pSpeed->setString(CCString::createWithFormat("%d",pWarriorInfo->speed)->getCString());
        
        
        m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(pWarriorInfo->uplv));
        
        skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(pWarriorInfo->getBaseInfo()->getSkillId());
        m_pSkillBox->updateView(pSkill,pWarriorInfo->slv);
        m_pSkillBox->getTouchMenu()->setTag(pWarriorInfo->getBaseInfo()->getSkillId());
        
        pSkill = DM_GET_SKILL_MODEL->getSkill(pWarriorInfo->getBaseInfo()->getSpSkillId());
        m_pSkillBoxSP->updateView(pSkill,m_pFormationInfo->getSpecialSkillLv(pSkill));
        m_pSkillBoxSP->getTouchMenu()->setTag(pWarriorInfo->getBaseInfo()->getSpSkillId());
        
        m_pPrice->setString(CCString::createWithFormat("%d",pWarriorInfo->fight)->getCString());
        
        for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
        {
            equip::EquipCurInfo* pEquip = m_pFormationInfo->getEquipInfoByPidAndType(pPositionInfo->iId, (equip::EquipType)(i+1));
            m_pEquipBox[i]->setEquipInfo(pEquip, 0);
            m_pEquipNone[i]->setVisible(pEquip==NULL);
        }
        
		warrior::FateInfo *pFateInfo = NULL;
		for (uint8_t i=0;i<MAX_FATE_COUNT;++i)
		{
			if(i<pWarriorInfo->getBaseInfo()->getFateArray()->count())
			{
				pFateInfo = static_cast<warrior::FateInfo *>(pWarriorInfo->getBaseInfo()->getFateArray()->objectAtIndex(i));
				m_pFate[i]->setString(pFateInfo->name.c_str());
				m_pFateHighlight[i]->setString(pFateInfo->name.c_str());
				if(m_pFormationInfo->checkFateActivated(pFateInfo))
				{
					m_pFate[i]->setVisible(false);
					m_pFateHighlight[i]->setVisible(true);
				}else
				{
					m_pFate[i]->setVisible(true);
					m_pFateHighlight[i]->setVisible(false);
				}
			}else
			{
				m_pFate[i]->setVisible(false);
				m_pFateHighlight[i]->setVisible(false);
			}
		}
//         for (uint8_t i=0;i<MAX_FATE_COUNT;++i)
//         {
//             if(i<pWarriorInfo->getBaseInfo()->getFateArray()->count())
//             {
//                 pFateInfo = static_cast<warrior::FateInfo *>(pWarriorInfo->getBaseInfo()->getFateArray()->objectAtIndex(i));
//                 m_pFate[i]->setVisible(true);
//                 m_pFate[i]->setString(pFateInfo->name.c_str());
//                 if(m_pFormationInfo->checkFateActivated(pFateInfo))
//                 {
//                     m_pFate[i]->setColor(m_fateActive);
//                 }else
//                 {
//                     m_pFate[i]->setColor(m_fateDeActive);
//                 }
//             }else
//             {
//                 m_pFate[i]->setVisible(false);
//             }
//         }
        return;
    }while (0);
    m_pTotalPrice->setString("0");
    m_pAttack->setString("0");
    m_pDefence->setString("0");
    m_pHp->setString("0");
    m_pSpeed->setString("0");
    m_pPrice->setString("0");
    m_pSkillBox->reset();
    m_pSkillBoxSP->reset();
    for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
    {
        m_pEquipBox[i]->reset();
        m_pEquipNone[i]->setVisible(true);
	}
	for (uint8_t i=0;i<MAX_FATE_COUNT;++i)
	{
		m_pFate[i]->setVisible(false);
		m_pFateHighlight[i]->setVisible(false);
	}
    m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(0));
}

void OtherFormationView::showFormations()
{
    
    m_pWarriorInfoViews->removeAllObjects();
    
    if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut()) return;
    
    uint32_t count = m_pFormationInfo->getPositionsCount();
    
    for(uint32_t i = 0; i < count; ++i)
    {
        formation::PositionInfo* pPos = m_pFormationInfo->getPositionInfoByIndex(i);
        
        OtherFormationWarriorInfoView *pFormationWarriorInfoView = OtherFormationWarriorInfoView::create(m_pWarriorData,i);
        pFormationWarriorInfoView->setDelegate(this);
        m_pWarriorInfoViews->addObject(pFormationWarriorInfoView);
        pFormationWarriorInfoView->setWarriorInfo(m_pFormationInfo->getWarriorPropInfoByWid(pPos->iWarriorId));
        
    }
    
    m_pPageView->reloadData();
}

int  OtherFormationView::getPageIndexForWarriorID(const uint32_t &wid)
{
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(m_pWarriorInfoViews, pObj)
    {
        OtherFormationWarriorInfoView *pFormationWarriorInfoView = dynamic_cast<OtherFormationWarriorInfoView *>(pObj);
        if(pFormationWarriorInfoView && pFormationWarriorInfoView->getWid()==wid)
        {
            return pFormationWarriorInfoView->getPid();
        }
    }
    return 0;
}

#pragma mark - - methods public
void OtherFormationView::setCurrentPage(uint8_t i)
{
    if(i==m_iCurrentPage) return;
    
    m_iCurrentPage = i;
    
    updateView();
}

void OtherFormationView::showWarrior(const uint32_t &wid)
{
    m_pPageView->setCurrentPage(getPageIndexForWarriorID(wid));
}

#pragma mark --------------------FormationWarriorInfoView--------------------

OtherFormationWarriorInfoView::OtherFormationWarriorInfoView()
:m_pLevel(NULL)
,m_pName(NULL)
,m_pGrade(NULL)
,m_pExp(NULL)
,m_pExpBar(NULL)
,m_pWarriorTypeMark(NULL)
,m_pContainer(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorButton(NULL)
,m_pDefault(NULL)
,m_pWarrior(NULL)
{
}

OtherFormationWarriorInfoView::~OtherFormationWarriorInfoView()
{
    removeAllChildren();
    
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pGrade);
    
    CC_SAFE_RELEASE(m_pDefault);
    CC_SAFE_RELEASE(m_pWarrior);
    
    CC_SAFE_RELEASE(m_pExp);
    CC_SAFE_RELEASE(m_pExpBar);
    
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pWarriorInfo);
    
    CC_SAFE_RELEASE(m_pWarriorButton);
}
OtherFormationWarriorInfoView * OtherFormationWarriorInfoView::create(cocos2d::extension::CCData* pData,uint32_t pid)
{
    OtherFormationWarriorInfoView *pFormationWarriorInfoView = new OtherFormationWarriorInfoView();
    if(pFormationWarriorInfoView && pFormationWarriorInfoView->init(pData,pid))
    {
        pFormationWarriorInfoView->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pFormationWarriorInfoView);
    }
    
    return pFormationWarriorInfoView;
}
bool OtherFormationWarriorInfoView::init(cocos2d::extension::CCData* pData,uint32_t pid)
{
    m_nPid = pid;
    CCB_READER_DATA_AND_ADDCHILD(pData);
    updateView();
    return true;
}

bool OtherFormationWarriorInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefault", CCNode *, this->m_pDefault);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior", CCNode *, this->m_pWarrior);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelBMFont *, this->m_pExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpBar", DProgressBar *, this->m_pExpBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorButton", CCMenuItem *, this->m_pWarriorButton);
    return false;
}



#pragma mark - - methods public
void OtherFormationWarriorInfoView::updateView()
{
    if(m_pWarriorInfo)
    {
        
        m_pContainer->removeAllChildren();
        m_pContainer->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(m_pWarriorInfo->getAvatarId()));
        
        m_pLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->lv)->getCString());
        m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
        
        
        //弟子经验
        int nCurGetExp = m_pWarriorInfo->exp;
        int nCurExp = DM_GET_WARRIOR_MODEL->getWarriorCurExpByGetExp(nCurGetExp, m_pWarriorInfo->lv, m_pWarriorInfo->getBaseInfo()->getRealGrade());
        int nUpExp = DM_GET_WARRIOR_MODEL->getWarriorUpExp(m_pWarriorInfo->getNextLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
        m_pExp->setString(CCString::createWithFormat("%d/%d",nCurExp,nUpExp)->getCString());
        m_pExpBar->setPercent((float)nCurExp/nUpExp);
        
        m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pWarriorInfo->wid));
        m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
        m_pDefault->setVisible(false);
        m_pWarrior->setVisible(true);
    }else
    {
        m_pDefault->setVisible(true);
        m_pWarrior->setVisible(false);
    }
    
}
void OtherFormationWarriorInfoView::setWarriorInfo(otherFormation::WarriorPropInfo* pWarriorInfo)
{
    if(pWarriorInfo==m_pWarriorInfo) return;
    CC_SAFE_RELEASE(m_pWarriorInfo);
    CC_SAFE_RETAIN(pWarriorInfo);
    m_pWarriorInfo = pWarriorInfo;
    updateView();
}
uint32_t OtherFormationWarriorInfoView::getWid()
{
    return m_pWarriorInfo?m_pWarriorInfo->wid:0;
}
uint32_t OtherFormationWarriorInfoView::getPid()
{
    return m_nPid;
}

void OtherFormationWarriorInfoView::setDelegate(OtherFormationView *pDelegate)
{
    m_pDelegate = pDelegate;
}


//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - OtherFormationListView
#pragma mark - - init etc.
OtherFormationListView::OtherFormationListView()
:m_pView(NULL)
,m_pDelegate(NULL)
,m_pCellData(NULL)
,m_pFormationInfo(NULL)
{
    
}

OtherFormationListView::~OtherFormationListView()
{
    CC_SAFE_RELEASE(m_pCellData);
    CC_SAFE_RELEASE(m_pFormationInfo);
}


bool OtherFormationListView::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
        
        m_pCellData = readCCData("ui/ccb/formation/warriorHeadSmallView");
        CC_SAFE_RETAIN(m_pCellData);
		m_pView = DTableView::create(this, CCSizeMake(640, 170));
		m_pView->setDirection(kCCScrollViewDirectionHorizontal);
		m_pView->setDelegate(this);
		addChild(m_pView);
        
        m_pView->reloadData();
        
		bRet = true;
	}while(0);
    
	return bRet;
}

#pragma mark - - CCTableViewDelegate
void OtherFormationListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
    if(m_pDelegate) m_pDelegate->setCurrentPage(cell->getIdx());
    
}

#define CELL_SIZE CCSizeMake(110, 150)

CCSize OtherFormationListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CELL_SIZE;
}

CCTableViewCell* OtherFormationListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
    
    OtherFormationListViewCell *pCell = static_cast<OtherFormationListViewCell*>(table->dequeueCell());
	if (!pCell)
	{
		pCell = OtherFormationListViewCell::create(m_pCellData);
	}
    
    pCell->setHighlight(m_pDelegate && m_pDelegate->getCurrentPage() == idx);
    
    formation::PositionInfo* pPos = m_pFormationInfo->getPositionInfoByIndex(idx);
    if(pPos) pCell->setWarriorInfo(m_pFormationInfo->getWarriorPropInfoByWid(pPos->iWarriorId));
    
    return pCell;
}

uint32_t OtherFormationListView::numberOfCellsInTableView(CCTableView *table)
{
    return m_pFormationInfo?m_pFormationInfo->getPositionsCount():0;
}

void OtherFormationListView::reset()
{
    m_pView->setContentOffset(ccp(m_pView->maxContainerOffset().x,0) , false);
}

void OtherFormationListView::updateView()
{
    if(m_pDelegate)
    {
        m_pView->reloadData();
        
        int nCurrentPage = m_pDelegate->getCurrentPage();
        
        float offsetX = -(nCurrentPage-1)*CELL_SIZE.width;
        
        offsetX = MIN(MAX((m_pView->minContainerOffset()).x, offsetX), (m_pView->maxContainerOffset()).x);
        
        
        m_pView->setContentOffset(ccp(m_pView->getContentOffset().x,0) , false);
        
        m_pView->setContentOffset(ccp(offsetX,0) , true);
    }
}


#pragma mark - OtherFormationListViewCell
#pragma mark - - init etc.
OtherFormationListViewCell::OtherFormationListViewCell()
:m_pWarriorInfo(NULL)
,m_pWarrior(NULL)
,m_pUnlock(NULL)
,m_pUnlockLevel(NULL)
,m_pHighLight(NULL)
,m_pSoundItem(NULL)
{
    
}

OtherFormationListViewCell::~OtherFormationListViewCell()
{
    CC_SAFE_RELEASE(m_pWarriorInfo);
    CC_SAFE_RELEASE(m_pWarrior);
    CC_SAFE_RELEASE(m_pUnlock);
    CC_SAFE_RELEASE(m_pUnlockLevel);
    CC_SAFE_RELEASE(m_pHighLight);
    CC_SAFE_RELEASE(m_pSoundItem);
}

OtherFormationListViewCell* OtherFormationListViewCell::create(cocos2d::extension::CCData *pData)
{
    OtherFormationListViewCell* pRet = new OtherFormationListViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool OtherFormationListViewCell::init(cocos2d::extension::CCData *pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
    setHighlight(false);
    updateView();
    return true;
}

bool OtherFormationListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior", WarriorHeadSmall*, this->m_pWarrior);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlock", CCNode*, this->m_pUnlock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlockLevel", CCLabelBMFont*, this->m_pUnlockLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLight", CCSprite*, this->m_pHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoundItem", CCMenuItem*, this->m_pSoundItem);
    return false;
}

#pragma mark - - methods private
void OtherFormationListViewCell::updateView()
{
    m_pWarrior->setVisible(true);
    m_pUnlock->setVisible(false);
    if(m_pWarriorInfo) m_pWarrior->setWarriorInfo(m_pWarriorInfo->getBaseInfo(),m_pWarriorInfo->getAvatarId());
    else m_pWarrior->resetWarriorInfo();
}

void OtherFormationListViewCell::setUnLockInfo(uint32_t level)
{
    m_pWarrior->setVisible(false);
    m_pUnlock->setVisible(true);
    m_pUnlockLevel->setString(CCString::createWithFormat("%d",level)->getCString());
}

#pragma mark - - methods private
void OtherFormationListViewCell::setHighlight(bool val)
{
    m_pHighLight->setVisible(val);
    m_pSoundItem->setVisible(!val);
}

void OtherFormationListViewCell::setWarriorInfo(otherFormation::WarriorPropInfo* pWarriorInfo)
{
    if(pWarriorInfo == m_pWarriorInfo) return;
    if(pWarriorInfo==m_pWarriorInfo) return;
    CC_SAFE_RELEASE(m_pWarriorInfo);
    CC_SAFE_RETAIN(pWarriorInfo);
    m_pWarriorInfo = pWarriorInfo;
    updateView();
}
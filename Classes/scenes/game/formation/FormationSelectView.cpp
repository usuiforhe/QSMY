//
//  ChangeFormationSelectView.cpp
//  Ó¢ÐÛÉÏÕóÑ¡Ôñ
//  Created by zl on 14-7-3.
//  Copyright (c) 2014, thedream. All rights reserved.
//

#include "FormationSelectView.h"
#include "../formation/Formation.h"
#include "../../../managers/GameManager.h"
#include "../../../managers/TipsManager.h"
#include "../../../managers/ResourceManager.h"
#include "../common/gui/WarriorTypeMark.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  FORMATION_TABLE_CELL_SIZE        CCSizeMake(614, 132)

FormationSelectView::FormationSelectView()
	//:m_pDelegate(NULL)
	:m_pWarriorModel(NULL)
	,m_pFormationModel(NULL)
	,m_pTableView(NULL)
	,m_pCellData(NULL)
	,m_pWarriorList(NULL)
	,m_pChangeEquipLabel(NULL)
	,m_pChangeHeroLabel(NULL)
	,m_pPageContainer(NULL)
	,m_pPosInfo(NULL)
	,m_pDelegate(NULL)
	,m_pPageContainerEnd(NULL)
	,m_pZhaomuBox(NULL)
	,m_pZhaomuBtn(NULL)
{
    
}

FormationSelectView::~FormationSelectView()
{
	UNBINDING_ALL(m_pFormationModel, this);
	CC_SAFE_RELEASE_NULL(m_pWarriorList);
    CC_SAFE_RELEASE(m_pCellData);
    CC_SAFE_RELEASE(m_pWarriorList);
	CC_SAFE_RELEASE(m_pPageContainer);
	CC_SAFE_RELEASE(m_pChangeEquipLabel);
	CC_SAFE_RELEASE(m_pChangeHeroLabel);

	CC_SAFE_RELEASE(m_pWarriorModel);
	CC_SAFE_RELEASE(m_pFormationModel);
	CC_SAFE_RELEASE(m_pPageContainerEnd);
	CC_SAFE_RELEASE(m_pZhaomuBtn);
	CC_SAFE_RELEASE(m_pZhaomuBox);
	CC_SAFE_RELEASE(m_pPosInfo);
}

FormationSelectView* FormationSelectView::create(formation::PositionInfo* pPosInfo)
{
	FormationSelectView* pRet = new FormationSelectView();
	if(pRet && pRet->init(pPosInfo))
	{
		pRet->autorelease();
		return pRet;
	} else 
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool FormationSelectView::init(formation::PositionInfo* pPosInfo)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	CC_SAFE_RETAIN(pPosInfo);
	CC_SAFE_RELEASE(m_pPosInfo);
	m_pPosInfo = pPosInfo;

	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
	m_pFormationModel= DM_GET_FORMATION_MODEL;
	CC_SAFE_RETAIN(m_pWarriorModel);
	CC_SAFE_RETAIN(m_pFormationModel);

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhenrong/PL_zhenrong_lb");

	m_pCellData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_xy_cell");
	CC_SAFE_RETAIN(m_pCellData);

	m_pWarriorList = CCArray::create();
	CC_SAFE_RETAIN(m_pWarriorList);

	m_pTableView = DTableView::create(this, PL_WITHNAV_GET_CONTEENT_SIZE(m_pPageContainer,m_pPageContainerEnd));
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setFlyInStyle(kCCTableViewFlyInOnEnter);

	m_pPageContainerEnd->setContentSize(PL_WITHNAV_GET_CONTEENT_SIZE(m_pPageContainer,m_pPageContainerEnd));
	m_pPageContainerEnd->addChild(m_pTableView);
	m_pTableView->setPosition(ccp(12.0,0.0));
	m_pChangeEquipLabel->setVisible(false);
	m_pChangeHeroLabel->setVisible(true);
	BINDING_EXEC(m_pFormationModel, this, FormationSelectView::updateWarriorList, D_EVENT_FORMATION_UPDATE);
	m_pZhaomuBtn->getButton()->setTarget(this,menu_selector(FormationSelectView::openMallLottery));
	return true;
}

void FormationSelectView::onEnter()
{
	DLayer::onEnter();
	updateWarriorList();
}

void FormationSelectView::onExit()
{
	DLayer::onExit();
}

bool FormationSelectView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeEquipLabel", CCNode *, this->m_pChangeEquipLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeHeroLabel", CCNode *, this->m_pChangeHeroLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainer", CCNode *, this->m_pPageContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainerEnd", CCNode *, this->m_pPageContainerEnd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZhaomuBox", CCNode *, this->m_pZhaomuBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZhaomuBtn", DButton *, this->m_pZhaomuBtn);
    return false;
}

SEL_MenuHandler FormationSelectView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callbackReturn", FormationSelectView::callbackReturn);
	return NULL;
}

void FormationSelectView::callbackReturn(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		
		if (kFormationView == m_pDelegate->getLastPage() || kPartnerView == m_pDelegate->getLastPage())
		{
			m_pDelegate->showFormationView();
		} else
		{
			m_pDelegate->showChangeFormationView();
		}
	}
}


void FormationSelectView::returnButtonCallback(CCObject * obj)
{
   if (m_pDelegate)
   {
	   m_pDelegate->showChangeFormationView();
   }
}


void FormationSelectView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

CCSize FormationSelectView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return FORMATION_TABLE_CELL_SIZE;
}

CCTableViewCell* FormationSelectView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	FormationSelectCell *cell = (FormationSelectCell*)table->dequeueCell();
	if (!cell)
	{
		cell = FormationSelectCell::create(m_pCellData);
	}
    
    warrior::WarriorCurInfo* pWarrior = static_cast<warrior::WarriorCurInfo *>(m_pWarriorList->objectAtIndex(idx));
    
    //cell->setSelected(m_selectIndex==idx);
    cell->setPosInfo(m_pPosInfo);
	cell->setWarrior(pWarrior);
	cell->setDelegate(m_pDelegate);
	
	return cell;
}

uint32_t FormationSelectView::numberOfCellsInTableView(CCTableView *table)
{
	if(m_pWarriorList)
	{
		if(m_pWarriorList->count()>0)
		{
			m_pZhaomuBox->setVisible(false);
		}
		else
		{
			m_pZhaomuBox->setVisible(true);
		}
	}
	return m_pWarriorList == NULL ? 0 : m_pWarriorList->count();
}

void FormationSelectView::setPosInfo(formation::PositionInfo* pPosInfo)
{
	CC_SAFE_RETAIN(pPosInfo);
	CC_SAFE_RELEASE(m_pPosInfo);
	m_pPosInfo = pPosInfo;
	updateWarriorList();
}

void FormationSelectView::updateWarriorList()
{
	m_pWarriorModel->getWarriorsUnBattle(m_pWarriorList);
	if(m_pDelegate) m_pTableView->reloadData();
}

void FormationSelectView::onFormationJoined()
{

}

void FormationSelectView::openMallLottery(CCObject*)
{
	Game::sharedGame()->NavigateTo(navigation::kNavMallLottery);
}



FormationSelectCell::FormationSelectCell()
:m_pWarrior(NULL)
,m_pContainer(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pCurrentFight(NULL)
,m_pAddFight(NULL)
,m_pSubFight(NULL)
,m_pUpArrow(NULL)
,m_pDownArrow(NULL)
,m_pWarriorTypeMark(NULL)
,m_pJoinButton(NULL)
,m_pPosInfo(NULL)
,m_pDelegate(NULL)
{
	m_gradeBgNodes.insert(m_gradeBgNodes.begin(),  warrior::kWarGradeRed + 1, (CCNode*)NULL);
}

FormationSelectCell::~FormationSelectCell()
{
    UNBINDING_ALL(m_pWarrior, this);
	CC_SAFE_RELEASE(m_pWarrior);
    CC_SAFE_RELEASE(m_pContainer);

	CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pCurrentFight);
	CC_SAFE_RELEASE(m_pAddFight);
    CC_SAFE_RELEASE(m_pSubFight);
	CC_SAFE_RELEASE(m_pUpArrow);
	CC_SAFE_RELEASE(m_pDownArrow);
	CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pJoinButton);

	for (unsigned int i = 0; i < m_gradeBgNodes.size(); i++)
	{
		CC_SAFE_RELEASE_NULL(m_gradeBgNodes[i]);
	}
}

FormationSelectCell* FormationSelectCell::create(cocos2d::extension::CCData* pData)
{
    FormationSelectCell* pRet = new FormationSelectCell();
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

bool FormationSelectCell::init(cocos2d::extension::CCData* pData)
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCNodeLoaderLibrary * pCCNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pCCNodeLoaderLibrary);
	pCCNodeLoaderLibrary->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());
	CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(pData, pCCNodeLoaderLibrary);

	m_pJoinButton->getButton()->setTarget(this, menu_selector(FormationSelectCell::joinButtonCallback));
	return true;
}

bool FormationSelectCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeGreen", CCNode *, m_gradeBgNodes[warrior::kWarGradeGreen]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeBlue", CCNode *, m_gradeBgNodes[warrior::kWarGradeBlue]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradePurple", CCNode *, m_gradeBgNodes[warrior::kWarGradePurple]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gradeOrange", CCNode *, m_gradeBgNodes[warrior::kWarGradeRed]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurrentFight", CCLabelBMFont *, m_pCurrentFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddFight", CCLabelBMFont *, m_pAddFight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubFight", CCLabelBMFont *, m_pSubFight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpArrow", CCSprite *, m_pUpArrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDownArrow", CCSprite *, m_pDownArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, m_pWarriorTypeMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pJoinButton", DButton *, this->m_pJoinButton);
   return false;
}

cocos2d::SEL_MenuHandler FormationSelectCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "viewCallBack", FormationSelectCell::showWarriorInfo);
	return NULL;
}

void FormationSelectCell::joinButtonCallback(cocos2d::CCObject *sender)
{
	if (m_pDelegate && m_pPosInfo && m_pWarrior)
	{
		m_pDelegate->joinFormation(m_pWarrior->getWid(), m_pPosInfo->iId);
		//m_pDelegate->showChangeFormationView();
	}
}

void FormationSelectCell::setWarrior(warrior::WarriorCurInfo *pWarrior, bool _updateView)
{
	 if(m_pWarrior != pWarrior)
	 {
		 UNBINDING_ALL(m_pWarrior, this);
		 CC_SAFE_RETAIN(pWarrior);
		 CC_SAFE_RELEASE_NULL(m_pWarrior);
		 m_pWarrior = pWarrior;    
		 binding();
	 }
	
	 if(_updateView)
	 {
		 this->updateCellView();
	 }
}


void FormationSelectCell::updateCellView()
{
	if(!m_pWarrior||!m_pWarrior->getBaseInfo())
	{
		return;
	}

    m_pName->setString(m_pWarrior->getBaseInfo()->getWarriorName().c_str());
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pWarrior->getAvatarId()));
    
	m_pLevel->setString(CCString::createWithFormat("%d", m_pWarrior->getWarriorLv())->getCString());
	
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
	uint32_t fight = 0;
	if(m_pDelegate && kPartnerView == m_pDelegate->getLastPage())
	{
		fight = m_pWarrior->computeWarriorForce();
	} else 
	{
		fight = m_pWarrior->computeWarriorForce(m_pPosInfo);
	}
	
	m_pCurrentFight->setString(CCString::createWithFormat("%d", fight)->getCString());
	int changedFight  = fight;
	if (m_pPosInfo)
	{
		WarriorModel* m_pWarriorModel = DM_GET_WARRIOR_MODEL;
		warrior::WarriorCurInfo * targrtWarriorInfo = m_pWarriorModel->getWarriorCurInfoByWID(m_pPosInfo->getWarriorId());
		if (targrtWarriorInfo == NULL)
		{
			m_pUpArrow->setVisible(true);
			m_pDownArrow->setVisible(false);
            m_pAddFight->setVisible(true);
            m_pSubFight->setVisible(false);
		}
		else
		{
			changedFight = (int)fight - (int)(targrtWarriorInfo->computeWarriorForce());
			m_pUpArrow->setVisible(changedFight >=0);
			m_pDownArrow->setVisible(changedFight < 0);
            m_pAddFight->setVisible(changedFight >=0);
            m_pSubFight->setVisible(changedFight < 0);
		}
	}
	else
	{
		m_pUpArrow->setVisible(true);
		m_pDownArrow->setVisible(false);
        m_pAddFight->setVisible(true);
        m_pSubFight->setVisible(false);
	}
	
    m_pAddFight->setString(CCString::createWithFormat("%d", changedFight)->getCString());
	m_pSubFight->setString(CCString::createWithFormat("%d", changedFight)->getCString());
	 m_pWarriorTypeMark->setWarriorInfo(m_pWarrior->getBaseInfo());
}

void FormationSelectCell::binding()
{
	BINDING_ONLY(m_pWarrior, this, FormationSelectCell::updateCellView, D_EVENT_WARRIORINFO_UPDATE);
}

void FormationSelectCell::showWarriorInfo( cocos2d::CCObject* pTarget )
{
	if (m_pDelegate)
	{
		m_pDelegate->showInfoView(kWarriorInfo, m_pWarrior);
	}
}




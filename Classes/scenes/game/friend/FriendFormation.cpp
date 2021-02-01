//
//  FriendFormation.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FriendFormation.h"
#include "../formation/FormationView.h"
#include "../warrior/WarriorInfo.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "managers/SoundManager.h"

USING_NS_CC;
//避免960界面误点到叉叉
#define HEAD_TABLE_SIZE CCSizeMake(480, 150)

FriendFormationListViewCell::FriendFormationListViewCell()
	:m_pWarriorInfo(NULL)
	,m_pWarrior(NULL)
	,m_pUnlock(NULL)
	,m_pUnlockLevel(NULL)
	,m_pHighLight(NULL)
	,m_pSoundItem(NULL)
	,m_pWarriorHeadGradeBg(NULL)
{

}

FriendFormationListViewCell::~FriendFormationListViewCell()
{
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo,this);
	CC_SAFE_RELEASE(m_pWarrior);
	CC_SAFE_RELEASE(m_pUnlock);
	CC_SAFE_RELEASE(m_pUnlockLevel);
	CC_SAFE_RELEASE(m_pHighLight);
	CC_SAFE_RELEASE(m_pSoundItem);
	CC_SAFE_RELEASE(m_pWarriorHeadGradeBg);
}

FriendFormationListViewCell* FriendFormationListViewCell::create(cocos2d::extension::CCData *pData)
{
	FriendFormationListViewCell* pRet = new FriendFormationListViewCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}

bool FriendFormationListViewCell::init(cocos2d::extension::CCData *pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);
	setHighlight(false);
	m_pWarrior->setVisible(true);
	//updateView();
	return true;
}

bool FriendFormationListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior", WarriorHeadSmall*, this->m_pWarrior);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorHeadGradeBg", WarriorHeadGradeBg*, this->m_pWarriorHeadGradeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlock", CCNode*, this->m_pUnlock);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlockLevel", CCLabelBMFont*, this->m_pUnlockLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLight", CCSprite*, this->m_pHighLight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoundItem", CCMenuItem*, this->m_pSoundItem);

	return false;
}

void FriendFormationListViewCell::setWarriorBoxState(WarriorBoxState st)
{
	switch (st)
	{
	case kWarBoxStLock:
		{
			setLock(true);
			if (m_pWarrior->getTouchToChange())
			{
				m_pWarrior->getTouchToChange()->setVisible(false);
			}
		}; break;
	case kWarBoxStNoWar:
		{
			setLock(false);
			m_pWarrior->resetWarriorInfo();
			if (m_pWarrior->getTouchToChange())
			{
				m_pWarrior->getTouchToChange()->setVisible(true);
			}
		}; break;
	case kWarBoxStHasWar:
		{

			setLock(false);
		}; break;
	}

}

#pragma mark - - methods private
void FriendFormationListViewCell::updateView()
{
	//setLock(false);
	m_pWarrior->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
	if(m_pWarriorInfo){
		m_pWarriorHeadGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
	} else {
		m_pWarriorHeadGradeBg->setWarriorGrade(warrior::WarriorGrade(0));
	}
}

void FriendFormationListViewCell::setUnLockInfo(uint32_t level)
{
	m_pUnlockLevel->setString(CCString::createWithFormat("%d",level)->getCString());
}

void FriendFormationListViewCell::setLock(bool val)
{
	m_pUnlock->setVisible(val);
	m_pSoundItem->setVisible(!m_pHighLight->isVisible() && !m_pUnlock->isVisible());
}

#pragma mark - - methods private
void FriendFormationListViewCell::setHighlight(bool val)
{
	m_pHighLight->setVisible(val);
	m_pSoundItem->setVisible(!m_pHighLight->isVisible() && !m_pUnlock->isVisible());
}

void FriendFormationListViewCell::setWarriorInfo(otherFormation::WarriorPropInfo* pWarriorCurInfo)
{
	//if(NULL == m_pWarriorCurInfo) m_pWarrior->getNotInBattle()->setVisible(true);
	if(pWarriorCurInfo == m_pWarriorInfo) return;
	CC_SAFE_RETAIN(pWarriorCurInfo);
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo,this);
	m_pWarriorInfo = pWarriorCurInfo;
	BINDING_EXEC(m_pWarriorInfo, this, FriendFormationListViewCell::updateView, D_EVENT_WARRIORINFO_UPDATE);
}



/************************************************************************/
/* FriendFormationListView                                                                     */
/************************************************************************/

FriendFormationListView::FriendFormationListView()
	:m_pView(NULL)
	,m_pDelegate(NULL)
	,m_pCellData(NULL)
	,m_pFormationInfo(NULL)
{

}

FriendFormationListView::~FriendFormationListView()
{
	CC_SAFE_RELEASE(m_pCellData);
	CC_SAFE_RELEASE(m_pFormationInfo);
}


bool FriendFormationListView::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());

		m_pCellData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_xx_yx_icon");
		CC_SAFE_RETAIN(m_pCellData);
		m_pView = DTableView::create(this, HEAD_TABLE_SIZE);
		m_pView->setPositionX(20);
		m_pView->setDirection(kCCScrollViewDirectionHorizontal);
		m_pView->setDelegate(this);
		addChild(m_pView);

		m_pView->reloadData();

		bRet = true;
	}while(0);

	return bRet;
}

void FriendFormationListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if(cell->getIdx()==m_pDelegate->getCurrentPage()) return;
	if(m_pDelegate) m_pDelegate->setCurrentPage(cell->getIdx());

}

CCSize FriendFormationListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return CCSizeMake(95, 130);
}

CCTableViewCell* FriendFormationListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{

	FriendFormationListViewCell *pCell = static_cast<FriendFormationListViewCell*>(table->dequeueCell());
	if (!pCell)
	{
		pCell = FriendFormationListViewCell::create(m_pCellData);
	}
	pCell->setHighlight(m_pDelegate && m_pDelegate->getCurrentPage() == idx);

	formation::PositionInfo* pPos = m_pFormationInfo->getPositionInfoByIndex(idx);
	if(pPos) pCell->setWarriorInfo(m_pFormationInfo->getWarriorPropInfoByWid(pPos->iWarriorId));
	pCell->setWarriorBoxState(kWarBoxStHasWar);

	//formation::PositionInfo* pPos = m_pFormationModel->getPositionInfoByIndex(idx);
	////英雄更新之后，再更新对应的信息
	//if(idx==m_pFormationModel->getUnlockedPositionsCount())
	//{
	//	pCell->setUnLockInfo(m_pFormationModel->getUnlockLevel(pPos->iPosId));
	//	pCell->setWarriorBoxState(kWarBoxStLock);
	//	pCell->setWarriorInfo(NULL);
	//}else
	//{
	//	if(pPos && pPos->iWarriorId > 0){ 
	//		pCell->setWarriorInfo(m_pWarriorModel->getWarriorCurInfoByWID(pPos->iWarriorId));
	//		pCell->setWarriorBoxState(kWarBoxStHasWar);
	//	} else {
	//		pCell->setWarriorInfo(NULL);
	//		pCell->setWarriorBoxState(kWarBoxStNoWar);
	//	}
	//}
	return pCell;
}

uint32_t FriendFormationListView::numberOfCellsInTableView(CCTableView *table)
{
	//if(m_pFormationModel->getUnlockedPositionsCount()==FORMATION_POS_MAX) return FORMATION_POS_MAX;
	//return m_pFormationModel->getUnlockedPositionsCount()+1; //unlock

	return m_pFormationInfo?m_pFormationInfo->getPositionsCount():0;
}


void FriendFormationListView::updateView()
{
	if(m_pDelegate)
	{
		m_pView->reloadData();
		int nCurrentPage = m_pDelegate->getCurrentPage();

		if(nCurrentPage == 0)
		{
			m_pView->setContentOffset(ccp(2,0) , false);
			m_pView->setContentOffset(ccp(2,0) , true);
		} else 
		{
			CCSize tableSize = tableCellSizeForIndex(NULL, -1);
			float offsetX = -(nCurrentPage-1)*tableSize.width;
			offsetX = MIN(MAX((m_pView->minContainerOffset()).x, offsetX), (m_pView->maxContainerOffset()).x);
			m_pView->setContentOffset(ccp(m_pView->getContentOffset().x,0) , false);
			m_pView->setContentOffset(ccp(offsetX,0) , true);
		}
	}
}


/************************************************************************/
/* FriendFormationView                                                                     */
/************************************************************************/

#define PAGE_MARGIN 150
#define PAGE_SIZE_WIDTH (220 + PAGE_MARGIN*2)


FriendFormationView::FriendFormationView()
:m_pDelegate(NULL)
,m_pFormationListView(NULL)
,m_pPageView(NULL)
,m_pModel(NULL)
,m_iCurrentPage(0)
,m_pWarriorInfoViews(NULL)
,m_pWarriorListContainer(NULL)
,m_pWarriorInfoContainer(NULL)
,m_pAttack(NULL)
,m_pSpeed(NULL)
,m_pDefence(NULL)
,m_pHp(NULL)
//,m_pPrice(NULL)
,m_pTotalPrice(NULL)
,m_pSkillBox(NULL)
,m_pSkillBoxSP(NULL)
,m_pWarriorData(NULL)
,m_pUpLevel(NULL)
,m_uCurrentWarriorID(0)
,m_pFateDisable(NULL)
,m_pFateEnable(NULL)
,m_pButtonsNode(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pPrice(NULL)
//,m_pProgressBar(NULL)
,m_pWarriorTypeMark(NULL)
,m_pFateMenu(NULL)
,m_pWarriorGradeBg(NULL)
,m_pHorn(NULL)
,m_nSoundID(0)
,m_pArmature(NULL)
,m_pZizhiNum(NULL)
,m_pZizhiBg(NULL)
,m_pChangeFormationMenu(NULL)
,m_pCloseButton(NULL)
,m_pFormationInfo(NULL)
{
    D_RESET_C_ARRAY(m_pEquipBox,EQUIP_BOX_COUNT);
    D_RESET_C_ARRAY(m_pEquipNone,EQUIP_BOX_COUNT);
    D_RESET_C_ARRAY(m_pFate,MAX_FATE_COUNT);
    D_RESET_C_ARRAY(m_pFateHighlight,MAX_FATE_COUNT);
    D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

FriendFormationView::~FriendFormationView()
{
    CC_SAFE_RELEASE(m_pWarriorInfoViews);
    CC_SAFE_RELEASE(m_pWarriorListContainer);
    CC_SAFE_RELEASE(m_pWarriorInfoContainer);
    CC_SAFE_RELEASE(m_pAttack);
    CC_SAFE_RELEASE(m_pSpeed);
    CC_SAFE_RELEASE(m_pDefence);
    CC_SAFE_RELEASE(m_pHp);
    
    //CC_SAFE_RELEASE(m_pPrice);
    CC_SAFE_RELEASE(m_pTotalPrice);
    CC_SAFE_RELEASE(m_pSkillBox);
    CC_SAFE_RELEASE(m_pSkillBoxSP);
    CC_SAFE_RELEASE(m_pWarriorData);
    CC_SAFE_RELEASE(m_pUpLevel);
    D_SAFE_RELEASE_C_ARRAY(m_pEquipBox,EQUIP_BOX_COUNT);
    D_SAFE_RELEASE_C_ARRAY(m_pEquipNone,EQUIP_BOX_COUNT);
    D_SAFE_RELEASE_C_ARRAY(m_pFate,MAX_FATE_COUNT);
    D_SAFE_RELEASE_C_ARRAY(m_pFateHighlight,MAX_FATE_COUNT);
	CC_SAFE_RELEASE(m_pFateEnable);
	CC_SAFE_RELEASE(m_pFateDisable);
	CC_SAFE_RELEASE(m_pButtonsNode);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pPrice);
	CC_SAFE_RELEASE(m_pWarriorTypeMark);
	//CC_SAFE_RELEASE(m_pProgressBar);
	CC_SAFE_RELEASE(m_pFateMenu);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);

	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
	CC_SAFE_RELEASE(m_pHorn);
	CC_SAFE_RELEASE(m_pZizhiNum);
	CC_SAFE_RELEASE(m_pZizhiBg);

	CC_SAFE_RELEASE(m_pChangeFormationMenu);
	CC_SAFE_RELEASE(m_pCloseButton);
}

bool FriendFormationView::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
	m_pModel = DM_GET_OTHERFORMATION_MODEL;
    
    CC_SAFE_RELEASE(m_pWarriorInfoViews);
    m_pWarriorInfoViews = CCArray::create();
    CC_SAFE_RETAIN(m_pWarriorInfoViews);
    
    removeAllChildren();
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhenrong/PL_zhujiemian");
    
    CC_SAFE_RELEASE(m_pWarriorData);
    m_pWarriorData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_xx_yx_cell");
    CC_SAFE_RETAIN(m_pWarriorData);
    
	//弹出框关闭按钮
	m_pCloseButton->getButton()->setTarget(this, menu_selector(FriendFormationView::onClose));

    /**
     *   初始化弟子展示界面
     */
    
    m_pPageView = DPageView::create(this,m_pWarriorInfoContainer->getContentSize());
    m_pPageView->setContentOffset(CCPointZero);
    m_pPageView->setTouchEnabled(false);
    m_pPageView->setFade(false);
    m_pPageView->setDelegate(this);
    m_pWarriorInfoContainer->addChild(m_pPageView);
    
    /**
     *   初始化弟子列表
     */
	
    m_pFormationListView = FriendFormationListView::create();
    m_pFormationListView->setDelegate(this);
    m_pWarriorListContainer->addChild(m_pFormationListView);
    
    
    m_pSkillBox->getTouchMenu()->setTarget(this, menu_selector(FriendFormationView::touchSkillCallBack));
    m_pSkillBoxSP->getTouchMenu()->setTarget(this, menu_selector(FriendFormationView::touchSkillCallBack));
    
    
    for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
    {
        m_pEquipBox[i]->getTouchMenu()->setTarget(this, menu_selector(FriendFormationView::touchEquipCallBack));
        m_pEquipBox[i]->getTouchMenu()->setTag(i);
		m_pEquipBox[i]->getEquipOnMenu()->setTarget(this, menu_selector(FriendFormationView::touchEquipCallBack));
		m_pEquipBox[i]->getEquipOnMenu()->setTag(i);
		m_pEquipBox[i]->setTypeNum(i);
    }
    
    return true;
}

bool FriendFormationView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorListContainer", CCNode *, this->m_pWarriorListContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorInfoContainer", CCNode *, this->m_pWarriorInfoContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, this->m_pUpLevel);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTotalPrice", CCLabelBMFont *, this->m_pTotalPrice);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox", SkillBox *, this->m_pSkillBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBoxSP", SkillBox *, this->m_pSkillBoxSP);
    
	D_CCB_ASSIGN_ARRAY("m_pFate", CCLabelTTF *, m_pFate, MAX_FATE_COUNT);
	
    
    D_CCB_ASSIGN_ARRAY("m_pFateHighlight", CCSprite *, m_pFateHighlight, MAX_FATE_COUNT);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox1", EquipBox *, this->m_pEquipBox[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox2", EquipBox *, this->m_pEquipBox[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox3", EquipBox *, this->m_pEquipBox[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipBox4", EquipBox *, this->m_pEquipBox[3]);
	
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateDisable", CCNode *, this->m_pFateDisable);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateEnable", CCNode *, this->m_pFateEnable);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonsNode", CCNode *, this->m_pButtonsNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateMenu", CCMenuItem*, this->m_pFateMenu);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont *, this->m_pPrice);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark*, this->m_pWarriorTypeMark);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar*, this->m_pProgressBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg*, this->m_pWarriorGradeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHorn", CCMenuItem *, this->m_pHorn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum" , CCLabelBMFont*,m_pZizhiNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiBg" ,CCNode *, m_pZizhiBg);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeFormationMenu" ,CCNode *, m_pChangeFormationMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton" ,DButton *, m_pCloseButton);

    return false;
}

SEL_MenuHandler FriendFormationView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "touchWarrior", FriendFormationView::touchWarrior);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callbackFormation", FriendFormationView::callbackFormation);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "playVoiceCallback", FriendFormationView::playVoiceCallback);
    return NULL;
}

void FriendFormationView::setCurrentPage(uint8_t i)
{
	if(i==m_iCurrentPage) return;
	m_iCurrentPage = i;
	updateView();
	stopHeroSound();
	return;


	/*formation::PositionInfo *pPositionInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(pageToPositionIdx(i));
	if(!pPositionInfo || !pPositionInfo->iWarriorId)
	{
		m_pDelegate->showInfoView(kFormationSelectView, NULL, static_cast<CCObject*>(pPositionInfo));
		m_pDelegate->setLastPage(kFormationView);
	} else 
	{
		if(i==m_iCurrentPage) return;

		m_iCurrentPage = i;
		updateView();
		stopHeroSound();
	}*/
}

void FriendFormationView::playVoiceCallback(cocos2d::CCObject *sender)
{
	m_pHorn->setEnabled(false);
	
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(m_uCurrentWarriorID, 0, "voice");

	if(NULL == pSound || !pSound->getDuration()) return;
	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat(pSound->getName().c_str()));
	//scheduleOnce(schedule_selector(FriendFormationView::stopHeroSound), );
	runAction(CCSequence::create(CCDelayTime::create(pSound->getDuration()), CCCallFunc::create(this, callfunc_selector(FriendFormationView::stopHeroSound)), NULL));
	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("horn");
	m_pArmature->setPosition(m_pHorn->getPosition());
	m_pHorn->getParent()->getParent()->addChild(m_pArmature, 10);
}
void FriendFormationView::stopHeroSound()
{
	m_pHorn->setEnabled(true);
	if (m_nSoundID) {
		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
		m_nSoundID = 0;
	}
	if(m_pArmature){
		
		m_pArmature->removeFromParent();
		m_pArmature = NULL;
	}
}

void FriendFormationView::onExit()
{
	UNBINDING_ALL(m_pFormationInfo , this);
    DLayer::onExit();
}

void FriendFormationView::onEnter()
{
	if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut())
	{
		BINDING_ONLY(m_pFormationInfo, this, FriendFormationView::updateFormation, D_EVENT_OTHERFORMATION_UPDATE);
		HTTP_CLIENT->getOtherFormation(m_pFormationInfo->getID());
	}
    
    updateFormation();
	
    DLayer::onEnter();
}

void FriendFormationView::updateFormation()
{
	//获取到数据之前，不处理显示
	if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut()) return;

    showFormations();
    updateView();
}



float FriendFormationView::cellSizeForTable(DPageView *view)
{
    return PAGE_SIZE_WIDTH+PAGE_MARGIN;
}

cocos2d::CCNode* FriendFormationView::cellAtIndex(DPageView *view, unsigned int idx)
{
    return (cocos2d::CCNode*)m_pWarriorInfoViews->objectAtIndex(idx);
}

uint32_t FriendFormationView::numberOfCellsInTableView(DPageView *view)
{
    return m_pWarriorInfoViews->count();
}

#pragma mark - - CCScrollViewDelegate
void FriendFormationView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    int32_t i = m_pPageView->getCurrentPage();
	if(i==m_iCurrentPage) return;

	m_iCurrentPage = i;
	updateView();
	stopHeroSound();
}

int  FriendFormationView::pageToPositionIdx(const int &page)
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

void FriendFormationView::showOtherAndRobotFormation(otherFormation::FormationInfo* pFormationInfo)
{
	Game::sharedGame()->setNavigatorVisable(false);
	m_pCloseButton->setVisible(true);
	m_pChangeFormationMenu->setVisible(false);

	m_pFormationInfo = pFormationInfo;//m_pModel->getForamtionInfoByUid(uid);
}

void FriendFormationView::onClose(CCObject*)
{
	m_pDelegate->close();
	
}

void FriendFormationView::updateView()
{
    updateInfo();
    
    //更新pageview的信息
    m_pPageView->setCurrentPage(m_iCurrentPage);
    
    //更新弟子列表信息
	m_pFormationListView->setFormationInfo(m_pFormationInfo);
    m_pFormationListView->updateView();
}

void FriendFormationView::updateInfo()
{
	formation::PositionInfo *pPositionInfo = m_pFormationInfo->getPositionInfoByIndex(pageToPositionIdx(m_iCurrentPage));
	if(pPositionInfo == NULL) return;

	otherFormation::WarriorPropInfo* pWarriorInfo = m_pFormationInfo->getWarriorPropInfoByWid(pPositionInfo->iWarriorId);
	if(pWarriorInfo == NULL) return;

	//记录当前英雄ID
	m_uCurrentWarriorID = pWarriorInfo->getBaseInfo()->getWarriorId();

	m_pUpLevel->setString(CCString::createWithFormat("%d", pWarriorInfo->uplv)->getCString());
	this->m_pFateEnable->setVisible(true);
	//不显示按钮
	this->m_pButtonsNode->setVisible(false);

	m_pLevel->setVisible(true);
	m_pName->setVisible(true);
	m_pPrice->setVisible(true);

	m_pFateMenu->setEnabled(true);

	int nTotalPrice = m_pFormationInfo->getFormationTotalForce();
	m_pTotalPrice->setString(CCString::createWithFormat("%d",nTotalPrice)->getCString());

	m_pAttack->setString(CCString::createWithFormat("%d",pWarriorInfo->att)->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d",pWarriorInfo->def)->getCString());
	m_pHp->setString(CCString::createWithFormat("%d",pWarriorInfo->hp)->getCString());
	m_pSpeed->setString(CCString::createWithFormat("%d",pWarriorInfo->speed)->getCString());

	if(m_pZizhiBg) m_pZizhiBg->setVisible(true);
	if(m_pZizhiNum) m_pZizhiNum->setString(CCString::createWithFormat("%d",pWarriorInfo->getBaseInfo()->getQuality())->getCString());

	m_pName->setString(pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	m_pLevel->setString(CCString::createWithFormat("%d",pWarriorInfo->lv)->getCString());
	m_pPrice->setString(CCString::createWithFormat("%d",pWarriorInfo->fight)->getCString());
	m_pWarriorTypeMark->setWarriorInfo(pWarriorInfo->getBaseInfo());
	m_pWarriorGradeBg->setWarriorGrade(pWarriorInfo->getBaseInfo()->getColorGrade());

	DProgressBar*progressbar = NULL;
	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pProgressBar[i]->setVisible( color == i+1);
		if(color == i+1)progressbar = m_pProgressBar[i];
	}

	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(pWarriorInfo->getBaseInfo()->getSkillId());
	m_pSkillBox->updateView(pSkill,pWarriorInfo->slv);
	m_pSkillBox->getTouchMenu()->setTag(pWarriorInfo->getBaseInfo()->getSkillId());

	pSkill = DM_GET_SKILL_MODEL->getSkill(pWarriorInfo->getBaseInfo()->getSpSkillId());
	m_pSkillBoxSP->updateView(pSkill,m_pFormationInfo->getSpecialSkillLv(pSkill));
	m_pSkillBoxSP->getTouchMenu()->setTag(pWarriorInfo->getBaseInfo()->getSpSkillId());



	for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
	{
		equip::EquipCurInfo* pEquip = m_pFormationInfo->getEquipInfoByPidAndType(pPositionInfo->iId, (equip::EquipType)(i+1));
		m_pEquipBox[i]->setEquipInfo(pEquip, 0);
		//隐藏加号
		m_pEquipBox[i]->getCanAdd()->setVisible(false);
		m_pEquipBox[i]->getCanEnhance()->setVisible(false);
	}

	warrior::FateInfo *pFateInfo = NULL;
	for (uint8_t i=0;i<MAX_FATE_COUNT;++i)
	{
		if(i<pWarriorInfo->getBaseInfo()->getFateArray()->count())
		{
			pFateInfo = static_cast<warrior::FateInfo *>(pWarriorInfo->getBaseInfo()->getFateArray()->objectAtIndex(i));
			m_pFate[i]->setString(pFateInfo->name.c_str());
			m_pFate[i]->setVisible(true);
			if(m_pFormationInfo->checkFateActivated(pFateInfo))
			{
				m_pFate[i]->setColor(ccc3(73, 30, 12));
				m_pFateHighlight[i]->setVisible(true);
			}else
			{
				m_pFate[i]->setColor(ccWHITE);
				m_pFateHighlight[i]->setVisible(false);
			}
		}else
		{
			m_pFate[i]->setVisible(false);
			m_pFateHighlight[i]->setVisible(false);
		}
	}
}

void FriendFormationView::showFormations()
{
	uint32_t count = m_pFormationInfo->getPositionsCount();

    for(uint32_t i = 0; i < count; ++i)
    {
		formation::PositionInfo* pPos = m_pFormationInfo->getPositionInfoByIndex(i);
        //formation::PositionInfo* pPos = m_pFormationModel->getUnlockedPositionInfoByIndex(i);
        
        FormationWarriorInfoView *pFormationWarriorInfoView = NULL;
        if(i<m_pWarriorInfoViews->count())
        {
            pFormationWarriorInfoView = (FormationWarriorInfoView *)m_pWarriorInfoViews->objectAtIndex(i);
        }
        if(!pFormationWarriorInfoView)
        {
            pFormationWarriorInfoView = FormationWarriorInfoView::create(m_pWarriorData,i);
            //pFormationWarriorInfoView->setDelegate(this);
			pFormationWarriorInfoView->setPositionInfo(pPos);
            m_pWarriorInfoViews->addObject(pFormationWarriorInfoView);
        }
		warrior::WarriorCurInfo* pWarriorCurInfo = warrior::WarriorCurInfo::create(pPos->iWarriorId);
        if(pFormationWarriorInfoView) pFormationWarriorInfoView->setWarriorInfo(pWarriorCurInfo);
        
    }
    m_pPageView->reloadData();
}



/************************************************************************/
/* FriendFormation                                                                     */
/************************************************************************/



FriendFormation::FriendFormation()
:m_pFormation(NULL)
,m_pWarriorModel(NULL)
,m_Container(NULL)
,m_pModel(NULL)
,m_pPopupLayer(NULL)
,m_bFirstEnter(true)
,m_pFormationInfo(NULL)
,m_pDelegate(NULL)
{
    
}

FriendFormation::~FriendFormation()
{
    CC_SAFE_RELEASE(m_pFormation);
	//SAFE_RELEASE_UNBINDING_ALL(m_pFormationInfo, this);
}

bool FriendFormation::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pModel = DM_GET_OTHERFORMATION_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    m_Container = CCNode::create();
    addChild(m_Container);
    m_pPopupLayer = CCNode::create();
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pPopupLayer);
    addChild(m_pPopupLayer,1);

    return true;
}

void FriendFormation::close()
{
	if(m_pDelegate)
	{
		//目前仅 帮会中使用了代理
		//简化操作这里进去掉底部导行显示
		removeFromParent();
	}
	else
	{
		Game::sharedGame()->setNavigatorVisable(true);
		removeFromParent();
	}
}

void FriendFormation::onEnter()
{
    DLayer::onEnter();
    //if(m_bFirstEnter) showFormationView();
}

void FriendFormation::onExit()
{
	UNBINDING_ALL(m_pFormationInfo, this);
	DLayer::onExit();
}


void FriendFormation::setInfo(unsigned int id , bool isRobot , otherFormation::FormationInfo* pFormationInfo)
{
	if(isRobot)
	{
		//机器人阵容
		//m_pFormation->showRobotFormation(id);
		//return;
		m_pFormationInfo =  pFormationInfo;
	}
	else{
		//查看他人阵容
		m_pFormationInfo =  m_pModel->getForamtionInfoByUid(id);

		if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut())
		{
			BINDING_ONLY(m_pFormationInfo, this, FriendFormation::updateFormation, D_EVENT_OTHERFORMATION_UPDATE);
			HTTP_CLIENT->getOtherFormation(m_pFormationInfo->getID());
		}

		//updateFormation();
	}

	updateFormation();
	
}

void FriendFormation::updateFormation()
{
	if(!m_pFormationInfo->isSynced || m_pFormationInfo->isTimeOut())
	{
		return;
	}

	showFormationView();
}

void FriendFormation::showFormationView()
{
    m_bFirstEnter = false;
    m_Container->setVisible(true);
    m_Container->removeAllChildren();
    m_pPopupLayer->removeAllChildren();
    if(m_pFormation == NULL)
    {
        m_pFormation = FriendFormationView::create();
        m_pFormation->setDelegate(this);
        m_pFormation->retain();
        
    }
	m_pFormation->showOtherAndRobotFormation(m_pFormationInfo);
    m_Container->addChild(m_pFormation);

	/*if (kFormationView == m_pLastPage)
	{
		m_pFormation->updateFormation();
	}
    
    CCInteger *pId = (CCInteger *)getUserObject();
    if(pId)
    {
        m_pFormation->showWarrior(pId->getValue());
        setUserObject(NULL);
    }
	FormationDelegate::setLastPage(kChangeFormationView);
	WarriorDelegate::closeInfoView();*/
}

//
//void FriendFormation::showEquipInfoView(uint32_t eid,formation::PositionInfo* pPos)
//{
//    if(pPos!=NULL) m_pSelectPos = pPos;
//    CC_ASSERT(m_pSelectPos);
//    m_pSelectPos = pPos;
//	m_pPopupLayer->removeAllChildren();
//    if(m_pEquipInfo==NULL)
//    {
//        m_pEquipInfo = EquipInfoView::create(EquipInfoView::kTypeEnhanceAndChange);
//		PL_MOVE_WITH_RESOLUTION_DIFF(m_pEquipInfo , -1);
//        m_pEquipInfo->retain();
//    }
//	m_pEquipInfo->setEquip(DM_GET_EQUIP_MODEL->getEquipCurInfoByID(eid));
//	m_pEquipInfo->setFormationInfo(this);
//	m_pPopupLayer->addChild(m_pEquipInfo);
//	setUserObject(CCInteger::create(pPos->getWarriorId()));
//}
//
//void FriendFormation::closeInfoView(WarriorViewType type)
//{
//    if(type==kSkillInfoView && m_lastType==kWarriorInfo)
//    {
//        showInfoView(kWarriorInfo, m_pWarrior);
//		m_eShowingType = m_lastType;
//    }
//	else if(type == kWarriorInfo && m_lastType == kFormationSelectView)
//	{
//		showInfoView(kFormationSelectView, m_pWarrior, m_pSelectPos);
//		m_eShowingType = m_lastType;
//	}
//	else
//    {
//        showFormationView();
//        WarriorDelegate::closeInfoView(type);
//		m_eShowingType = kWarriorNone;
//    }
//}
//
//void FriendFormation::showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param)
//{
//	m_eShowingType = type;
//    WarriorDelegate::showInfoView(type, pWarrior, param);
//    m_pPopupLayer->removeAllChildren();
//    warrior::PotBuff* pPotBuff = NULL;
//    Game::sharedGame()->setNavigatorVisable(true);
//    switch(type)
//	{
//        case kWarriorInfo:
//        {
//            if(m_pWarriorInfoView==NULL)
//            {
//                m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeOperate);
//                m_pWarriorInfoView->retain();
//                m_pWarriorInfoView->setDelegate(this);
//            }
//			Game::sharedGame()->setNavigatorVisable(false);
//            m_pWarriorInfoView->setWarrior(pWarrior);
//            m_pPopupLayer->addChild(m_pWarriorInfoView);
//			
//            break;
//        }
//            
//        case kPromotionView:		//突破详细信息
//            if(m_pPromotionView==NULL)
//            {
//                m_pPromotionView = PromotionView::create();
//                m_pPromotionView->setDelegate(this);
//                m_pPromotionView->retain();
//            }
//            m_pPromotionView->setWarrior(pWarrior);
//            m_pPopupLayer->addChild(m_pPromotionView);
//            break;
//        case kUpgradeView:		//英雄强化
//            if(m_pUpgradeView==NULL)
//            {
//                m_pUpgradeView = WarriorUpgradeView::create();
//                m_pUpgradeView->setWarriorDelegate(this);
//                m_pUpgradeView->retain();
//            }
//            m_pUpgradeView->setWarrior(pWarrior);
//            m_pPopupLayer->addChild(m_pUpgradeView);
//            break;
//            
//        case kDevelopView:		//培养详细信息
//            
//            if(m_pDevelopView==NULL)
//            {
//                m_pDevelopView = WarriorDevelopView::create();
//                m_pDevelopView->setDelegate(this);
//                m_pDevelopView->retain();
//            }
//            m_pDevelopView->setWarrior(pWarrior);
//            m_pPopupLayer->addChild(m_pDevelopView);
//            
//            pPotBuff = DM_GET_WARRIOR_MODEL->getWarriorPotBuff(pWarrior);
//            
//            //如果有培养buff未处理，切换到结算界面
//          /*  if (pPotBuff)
//            {
//                showDevSettleView(pWarrior);
//            }*/
//            break;
//            
//        case kTransSettleView:
//            if(m_pTransSettleView==NULL)
//            {
//                m_pTransSettleView = WarriorTransForceSettleView::create();
//                m_pTransSettleView->setDelegate(this);
//                m_pTransSettleView->retain();
//            }
//            m_pTransSettleView->setWarrior(pWarrior);
//            m_pPopupLayer->addChild(m_pTransSettleView);
//            break;
//        case kDevSettleView:
//            showDevSettleView(pWarrior);
//            break;
//        case kRecruitSettleView:
//            if(m_pRecruitSettleView==NULL)
//            {
//                m_pRecruitSettleView = RecruitSettleView::create();
//                m_pRecruitSettleView->setDelegate(this);
//                m_pRecruitSettleView->retain();
//            }
//            m_pRecruitSettleView->setWarrior(pWarrior);
//            m_pPopupLayer->addChild(m_pRecruitSettleView);
//            break;
//        case kSkillInfoView:
//        {
//            m_pPopupLayer->removeAllChildren();
//            if(m_pSkillInfo==NULL)
//            {
//                m_pSkillInfo = SkillInfoView::create();
//                m_pSkillInfo->retain();
//                m_pSkillInfo->setDelegate(this);
//            }
//            CCInteger* pSkillId = static_cast<CCInteger*>(param);
//            m_pSkillInfo->setSkillInfo(pWarrior,pSkillId->getValue());
//            m_pPopupLayer->addChild(m_pSkillInfo);
//            break;
//        }
//        case kSkillUpgradeView:
//        {
//            m_pPopupLayer->removeAllChildren();
//            if(m_pSkillUpgrade==NULL)
//            {
//                m_pSkillUpgrade = SkillUpgradeView::create();
//                m_pSkillUpgrade->retain();
//                m_pSkillUpgrade->setDelegate(this);
//            }
//            m_pSkillUpgrade->setWarriorInfo(pWarrior);
//            m_pPopupLayer->addChild(m_pSkillUpgrade);
//            break;
//        }
//		case kFormationSelectView:
//			{
//				if(param!=NULL)
//				{
//					m_pSelectPos = static_cast< formation::PositionInfo*> (param);
//				}
//				CC_ASSERT(m_pSelectPos);
//
//				m_pPopupLayer->removeAllChildren();
//
//				if(m_pFormationSelectView==NULL)
//				{
//					m_pFormationSelectView = FormationSelectView::create(m_pSelectPos);
//					m_pFormationSelectView->setDelegate(this);
//					m_pFormationSelectView->retain();
//				}
//
//				m_pFormationSelectView->setPosInfo(m_pSelectPos);
//				m_pPopupLayer->addChild(m_pFormationSelectView);
//				break;
//			}
//        case kSoulInfo:
//        default:
//            break;
//            
//	}
//    
//}



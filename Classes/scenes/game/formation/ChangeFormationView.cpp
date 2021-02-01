//
//  ChangeFormationView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ChangeFormationView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "model/WarriorModel.h"
#include "../common/common.h"
#include "../Game.h"
#include "../../../components/gui/PLAdaptiveNode.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;

#pragma mark - - init etc.
ChangeFormationView::ChangeFormationView()
:m_pDelegate(NULL)
,m_pChangeFormationListView(NULL)
,m_pFormationModel(NULL)
,m_pWarriorModel(NULL)
,m_pWarriorInfo(NULL)
,m_bChangeStatus(kChangeStatusFalse)
,m_pSelectedPos(NULL)
,m_pFight(NULL)
,m_pWarriorListContainer(NULL)
,m_pNoneWarrior(NULL)
,m_pLotteryButton(NULL)
,m_pBackButton(NULL)
,m_nSelectedWarriorId(0)
,m_bFromEmpty(false)
{
    D_RESET_C_ARRAY(m_pWarriors, FORMATION_POS_MAX);
}

ChangeFormationView::~ChangeFormationView()
{
    CC_SAFE_RELEASE(m_pNoneWarrior);
    CC_SAFE_RELEASE(m_pLotteryButton);
    UNBINDING_ALL(m_pFormationModel, this);
    CC_SAFE_RELEASE(m_pFight);
    CC_SAFE_RELEASE(m_pWarriorListContainer);
    D_SAFE_RELEASE_C_ARRAY(m_pWarriors, FORMATION_POS_MAX);
    
    CC_SAFE_RELEASE(m_pWarriorModel);
    CC_SAFE_RELEASE(m_pFormationModel);
	CC_SAFE_RELEASE(m_pBackButton);
}

bool ChangeFormationView::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
	m_pFormationModel= DM_GET_FORMATION_MODEL;
    CC_SAFE_RETAIN(m_pWarriorModel);
    CC_SAFE_RETAIN(m_pFormationModel);
    
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("ChangeFormationItem", ChangeFormationItemLoader::loader());
    
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_zhenrong/PL_zhenrong_bz.ccbi",pNodeLoaderLibrary);
    
    //弟子列表
   /* m_pChangeFormationListView = ChangeFormationListView::create();
    m_pChangeFormationListView->setDelegate(this);
    m_pWarriorListContainer->addChild(m_pChangeFormationListView);*/
    
   // m_pLotteryButton->getButton()->setTarget(this, menu_selector(ChangeFormationView::navToLottery));
    
    for (uint8_t i = 0 ; i<FORMATION_POS_MAX; ++i)
    {
        m_pWarriors[i]->setIndex(i);
        m_pWarriors[i]->setDelegate(this);
		
		/*CCLabelTTF* id = CCLabelTTF::create(CCString::createWithFormat("%d", i)->getCString(), "Arial", 30);
		id->setColor(ccRED);
		m_pWarriors[i]->addChild(id,10);*/
    }
    
	m_pBackButton->setTarget(this, menu_selector(ChangeFormationView::navToFormation));
    return true;
}

void ChangeFormationView::navToLottery(cocos2d::CCObject* obj)
{
    Game::sharedGame()->NavigateTo(navigation::kNavLottery);
}

bool ChangeFormationView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorListContainer", CCNode *, this->m_pWarriorListContainer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior1", ChangeFormationItem *, this->m_pWarriors[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior2", ChangeFormationItem *, this->m_pWarriors[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior3", ChangeFormationItem *, this->m_pWarriors[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior4", ChangeFormationItem *, this->m_pWarriors[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior5", ChangeFormationItem *, this->m_pWarriors[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior6", ChangeFormationItem *, this->m_pWarriors[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior7", ChangeFormationItem *, this->m_pWarriors[6]);
    
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoneWarrior", CCNode *, this->m_pNoneWarrior);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLotteryButton", DButton *, this->m_pLotteryButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBackButton", CCMenuItemImage*, this->m_pBackButton);
    
    return false;
}

void ChangeFormationView::onEnter()
{
    BINDING_EXEC(m_pFormationModel, this, ChangeFormationView::updateView, D_EVENT_FORMATION_UPDATE);
    DLayer::onEnter();
    
    /*for (uint8_t i = 0 ; i<FORMATION_POS_MAX; ++i)
    {
        m_pWarriors[i]->runFlipAction();
    }*/
}


void ChangeFormationView::onExit()
{
    UNBINDING_ALL(m_pFormationModel, this);
    reset();
    DLayer::onExit();
}

void ChangeFormationView::reset()
{
    m_bChangeStatus = kChangeStatusFalse;
    m_nSelectedWarriorId = 0;
    m_pSelectedPos = NULL;
    //m_pChangeFormationListView->resetChangeState();
}

#pragma mark - - methods private
void ChangeFormationView::updateView()
{
    for (uint8_t i = 0 ; i<FORMATION_POS_MAX; ++i)
    {
        m_pWarriors[i]->updateView();
    }
    
    
    int nTotalPrice = DM_GET_FORMATION_MODEL->getFormationTotalForce();
    m_pFight->setString(CCString::createWithFormat("%d",nTotalPrice)->getCString());
    
    
   /* if(m_pWarriorModel->isHasWarriorsUnBattle())
    {
        m_pChangeFormationListView->setVisible(true);
        m_pNoneWarrior->setVisible(false);
        m_pChangeFormationListView->updateView();
    }else
    {
        m_pChangeFormationListView->setVisible(false);
        m_pNoneWarrior->setVisible(true);
    }*/
    
}

void ChangeFormationView::setChangeState(ChangeStatus changeState)
{
    m_bChangeStatus = changeState;
    
    for (uint8_t i = 0 ; i< FORMATION_POS_MAX; ++i)
    {
        //只有在切换到替换状态时候 才会检测是否点击了同一个位置
        formation::PositionInfo *pPositionInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(m_pWarriors[i]->getIndex());
        if(!pPositionInfo) continue;
        if(m_pSelectedPos && pPositionInfo->iId == m_pSelectedPos->iId)
        {
            m_pWarriors[i]->setSelected(m_bChangeStatus!=kChangeStatusFalse);
            continue;
        }
        
        if(m_bChangeStatus==kChangeStatusFromTop)
        {
            m_pWarriors[i]->setChangeState(m_pWarriors[i]->isHaveWarrior() && !m_bFromEmpty);
        }else
        {
            m_pWarriors[i]->setChangeState(m_bChangeStatus!=kChangeStatusFalse);
        }
        
    }
}


#pragma mark - - methods public

void ChangeFormationView::change(uint32_t posId1,uint32_t posId2)
{
    HTTP_CLIENT->changeFormation(posId1,posId2,this,callfuncJson_selector(ChangeFormationView::onCallBack));
   //reset();
}

void ChangeFormationView::join(uint32_t wid,uint32_t posId)
{
    HTTP_CLIENT->joinFormation(wid,posId,this,callfuncJson_selector(ChangeFormationView::onCallBack));
    //reset();
}

void ChangeFormationView::onCallBack(const Json::Value&,const Json::Value& responeData)
{
    if(responeData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    reset();
}

void ChangeFormationView::touchChangeFormationItem(ChangeFormationItem *pItem)
{
    CCLOG("点击了弟子 %d" , pItem->getWarriorID());
    
    switch (m_bChangeStatus) {
        case kChangeStatusFromTop:
        {
            formation::PositionInfo *pSelectedPos = m_pFormationModel->getUnlockedPositionInfoByIndex(pItem->getIndex());
            
            if(!pSelectedPos) return;
            
            //如果点击的位置是同一个弟子
            if(m_pSelectedPos->iId == pSelectedPos->iId)
            {
                setChangeState(kChangeStatusFalse);
                reset();
                return;
            }
            
            //如果点击的位置没有弟子 直接返回
            if(!pItem->isHaveWarrior() || m_bFromEmpty)
            {
                Tips_Alert(D_LOCAL_STRING("CanNotChangeFormation").c_str());
                return;
            }
            
            
            
            //替换弟子
            change(pSelectedPos->iId,m_pSelectedPos->iId);
            break;
        }
        case kChangeStatusFalse:
        {
            //普通状态点击了一个弟子
            
            if(pItem->getLock()) return;
            
            //--> 设置激活的位置
            m_pSelectedPos = m_pFormationModel->getUnlockedPositionInfoByIndex(pItem->getIndex());
            
            m_bFromEmpty = !pItem->isHaveWarrior();
            
            //--> 切换到替换状态
            setChangeState(kChangeStatusFromTop);
           // m_pChangeFormationListView->setChangeState(true);
            break;
        }
            
        case kChangeStatusFromBottom:
        {
            formation::PositionInfo *pSelectedPos = m_pFormationModel->getUnlockedPositionInfoByIndex(pItem->getIndex());
            
            if(!pSelectedPos) return;
            
            join(m_nSelectedWarriorId, pSelectedPos->iId);
            
            break;
        }
        default:
            break;
    }
    
}
void ChangeFormationView::touchChangeListViewCell(ChangeFormationListViewCell *pCell)
{
    CCLOG("点击了弟子 %d" , pCell->getWarriorID());
    
    switch (m_bChangeStatus)
    {
        case kChangeStatusFromTop:
        {
            join(pCell->getWarriorID(),m_pSelectedPos->iId);
            break;
        }
        case kChangeStatusFalse:
        {
            m_nSelectedWarriorId = pCell->getWarriorID();
            setChangeState(kChangeStatusFromBottom);
            pCell->setSelected(true);
            break;
        }
        case kChangeStatusFromBottom:
        {
            if(m_nSelectedWarriorId == pCell->getWarriorID())
            {
                m_nSelectedWarriorId = 0;
                setChangeState(kChangeStatusFalse);
                pCell->setSelected(false);
            }else
            {
               // m_pChangeFormationListView->resetChangeState();
                m_nSelectedWarriorId = pCell->getWarriorID();
                pCell->setSelected(true);
            }
            break;
        }
    }
}

void ChangeFormationView::touchMoveBeganFormationItem(ChangeFormationItem *pItem)
{
    for (int i = 0; i < FORMATION_POS_MAX; i++)
	{
		if (m_pWarriors[i] == pItem)
		{
			continue;
		}
        
        if (m_pWarriors[i]) {
            //只有该位置被禁用时遮罩才会显示
            m_pWarriors[i]->setMaskVisible(true);
        }
    }
}


void ChangeFormationView::touchMoveEndFormationItem( ChangeFormationItem *pItem )
{
	if (pItem == NULL)
	{
		return;
	}
    
    for (int i = 0; i < FORMATION_POS_MAX; i++)
	{
        if (m_pWarriors[i]) {
            m_pWarriors[i]->setMaskVisible(false);
        }
    }
	
	int changeIndex = -1;
	float overLappingArea = 0.0f;
	CCRect selectRect = CCRectMake(pItem->getPositionX()
		,pItem->getPositionY()
		,pItem->getContentSize().width
		,pItem->getContentSize().height);
	for (int i = 0; i < FORMATION_POS_MAX; i++)
	{
		if (m_pWarriors[i] == pItem || m_pWarriors[i] == NULL)
		{
			continue;
		}

		CCRect otherRect = CCRectMake(m_pWarriors[i]->getPositionX()
			,m_pWarriors[i]->getPositionY()
			,m_pWarriors[i]->getContentSize().width
			,m_pWarriors[i]->getContentSize().height);
		if (otherRect.intersectsRect(selectRect))
		{
			//判断拖动位置更靠近哪个英雄栏
			float lappingWidth = 0.0f;
			float lappingHeight = 0.0f;
			if (otherRect.origin.x  < selectRect.origin.x)
			{
				lappingWidth = otherRect.origin.x + otherRect.size.width - selectRect.origin.x;
			}
			else
			{
				lappingWidth = selectRect.origin.x + selectRect.size.width - otherRect.origin.x;
			}
			
			if (otherRect.origin.y < selectRect.origin.y)
			{
				lappingHeight = otherRect.origin.y + otherRect.size.height -  selectRect.origin.y;
			}
			else
			{
				lappingHeight = selectRect.origin.y + selectRect.size.height - otherRect.origin.y;
			}
			
			if (lappingWidth * lappingHeight > overLappingArea)
			{
				overLappingArea = lappingWidth * lappingHeight;
				changeIndex = i;
			}
		}
	}// for end
    
	if (changeIndex >= 0 && changeIndex < FORMATION_POS_MAX)
	{
		formation::PositionInfo *pSelectedPos = m_pFormationModel->getUnlockedPositionInfoByIndex(pItem->getIndex());
		formation::PositionInfo *pTargetPos = m_pFormationModel->getUnlockedPositionInfoByIndex(m_pWarriors[changeIndex]->getIndex());
		if (pSelectedPos == NULL || pTargetPos == NULL)
		{
			//todo tip
			return;
		}
		
		if ( (pSelectedPos->isLeader() || pTargetPos->isLeader())
			&& (pSelectedPos->getWarriorId() == 0 || pTargetPos->getWarriorId() == 0))
		{
			Tips_Alert(D_LOCAL_STRING("swapLeaderWithNoWarrior"));
			return;
		}

		change(pSelectedPos->iId, pTargetPos->iId);
	}
}

void ChangeFormationView::navToFormation( cocos2d::CCObject* obj )
{
	Game::sharedGame()->NavigateTo(navigation::kNavFormation, true);
}

void ChangeFormationView::showFormationSelectView(formation::PositionInfo* pPos)
{
	if (m_pDelegate)
	{
		m_pDelegate->showInfoView(kFormationSelectView, m_pWarriorInfo, pPos);
	}
}





#pragma mark  - ChangeFormationItem
#pragma mark - - init etc.
ChangeFormationItem::ChangeFormationItem()
:m_pDelegate(NULL)
,m_bEmpty(true)
,m_nIndex(-1)
,m_bLock(true)
,m_bRunningFlipAction(false)
,m_touchMoved(false)
,m_nWid(-1)
,m_pWarrior(NULL)
//,m_pCanChange(NULL)
,m_pSelected(NULL)
,m_pTouchMenu(NULL)
,m_pWarriorTypeMark(NULL)
,m_pFormationModel(NULL)
,m_pWarriorModel(NULL)
,m_bChangeState(false)
,m_pTitleChange(NULL)
,m_pTitleUp(NULL)
,m_pFlipNode(NULL)
,m_pNoWarriorNode(NULL)
,m_pNoWarriorBg(NULL)
,m_pMask(NULL)
,m_pGradeNode(NULL)
,m_pWarriorInfoNode(NULL)
,m_pChangeButtonNode(NULL)
,m_pLockedLabel(NULL)
,m_pGradeOrange(NULL)
,m_pGradePurple(NULL)
,m_pGradeBlue(NULL)
,m_pGradeGreen(NULL)
,m_pHeadContainer(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pChangeButton(NULL)
{
    
}
ChangeFormationItem::~ChangeFormationItem()
{
    CC_SAFE_RELEASE(m_pWarrior);
    //CC_SAFE_RELEASE(m_pCanChange);
    CC_SAFE_RELEASE(m_pTouchMenu);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pSelected);
	CC_SAFE_RELEASE(m_pTitleChange);
	CC_SAFE_RELEASE(m_pTitleUp);
    CC_SAFE_RELEASE(m_pFlipNode);
	CC_SAFE_RELEASE(m_pNoWarriorNode);
    CC_SAFE_RELEASE(m_pNoWarriorBg);
    CC_SAFE_RELEASE(m_pMask);
	CC_SAFE_RELEASE(m_pGradeNode);
	CC_SAFE_RELEASE(m_pWarriorInfoNode);
	CC_SAFE_RELEASE(m_pChangeButtonNode);
	CC_SAFE_RELEASE(m_pLockedLabel);
	CC_SAFE_RELEASE(m_pGradeOrange);
	CC_SAFE_RELEASE(m_pGradePurple);
	CC_SAFE_RELEASE(m_pGradeBlue);
	CC_SAFE_RELEASE(m_pGradeGreen);
	CC_SAFE_RELEASE(m_pHeadContainer);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pChangeButton);
}

bool ChangeFormationItem::init()
{
	CCLayer::init();
    
    setTouchEnabled(true);

    m_pFormationModel = DM_GET_FORMATION_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    return true;
}


void ChangeFormationItem::onEnter()
{
	CCLayer::onEnter();

	m_originalPosition = this->getPosition();
	m_touchRect = CCRectMake(0,
                             m_pChangeButton->getContentSize().height,
                             this->getContentSize().width,
                             this->getContentSize().height - m_pChangeButton->getContentSize().height);
}

bool ChangeFormationItem::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor1Sprite",  cocos2d::CCSprite*, this->m_pGradeOrange);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor2Sprite",  cocos2d::CCSprite*, this->m_pGradePurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor3Sprite",  cocos2d::CCSprite*, this->m_pGradeBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgColor4Sprite",  cocos2d::CCSprite*, this->m_pGradeGreen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLockedLabel", CCLabelTTF *, this->m_pLockedLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFlipNode",  cocos2d::CCNode*, this->m_pFlipNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoWarriorNode",  cocos2d::CCNode*, this->m_pNoWarriorNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoWarriorBg",  cocos2d::CCSprite*, this->m_pNoWarriorBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMask", cocos2d::CCSprite*, this->m_pMask);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeNode",  cocos2d::CCNode*, this->m_pGradeNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorInfoNode",  cocos2d::CCNode*, this->m_pWarriorInfoNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeButtonNode",  cocos2d::CCNode*, this->m_pChangeButtonNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeadContainer",  cocos2d::CCNode*, this->m_pHeadContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleChange", cocos2d::CCSprite*, this->m_pTitleChange);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleUp", cocos2d::CCSprite*, this->m_pTitleUp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeButton", CCMenuItemImage *, this->m_pChangeButton);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
   
	return false;
}


SEL_MenuHandler ChangeFormationItem::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	 CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "changeButtonCallBack", ChangeFormationItem::changeButtonCallBack);
    return NULL;
}

#pragma mark - - methods private
void ChangeFormationItem::changeButtonCallBack(CCObject *obj)
{
	formation::PositionInfo* pPosInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(this->getIndex());
	if (pPosInfo == NULL)
	{
		return;
	}
	
	if (pPosInfo->isOpened() == false)
	{
		return;
	}

    if(m_pDelegate)
    {
        m_pDelegate->showFormationSelectView(pPosInfo);
    }
}

#pragma mark - - methods public
int ChangeFormationItem::getWarriorID()
{
    return m_nWid;
}

bool ChangeFormationItem::isHaveWarrior()
{
    return !getEmpty() && !getLock();
}

bool ChangeFormationItem::getEmpty()
{
    return m_bEmpty;
}
void ChangeFormationItem::setEmpty(const bool &empty)
{
    m_bEmpty = empty;
}

bool ChangeFormationItem::getLock()
{
    return m_bLock;
}

void ChangeFormationItem::setSelected(bool value)
{
    //m_pSelected->setVisible(value);
}

void ChangeFormationItem::setLock(const bool &Lock)
{
    m_bLock = Lock;
    //锁定精灵
   // m_pTouchMenu->setEnabled(!Lock);
    //m_pWarrior->setFlash(false);
    //m_pCanChange->setVisible(false);
    //m_pSelected->setVisible(false);
	setWarriorInfoVisible(!Lock);
	m_pChangeButtonNode->setVisible(!Lock);
}

int  ChangeFormationItem::getIndex()
{
    return m_nIndex;
}
void ChangeFormationItem::setIndex(const int &index)
{
    m_nIndex = index;
    //m_pTouchMenu->setTag(index);
    updateView();
}
bool ChangeFormationItem::getChangeState()
{
    return m_bChangeState;
}

void ChangeFormationItem::setChangeState(const bool &changeState)
{
    m_bChangeState = changeState;
    //m_pWarrior->setFlash(changeState);
    //m_pCanChange->setVisible(changeState);
}

void ChangeFormationItem::updateView()
{
    if (m_pMask) {
        m_pMask->stopAllActions();
        m_pMask->setOpacity(0);
		m_pMask->setVisible(false);
    }
    
    formation::PositionInfo *pUnlockedPosInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(m_nIndex);
    if(!pUnlockedPosInfo || !pUnlockedPosInfo->isOpened())
    {
        //锁定
        setLock(true);
		formation::PositionInfo *pNormalPosInfo = m_pFormationModel->getPositionInfoByIndex(m_nIndex);
		if(pNormalPosInfo) 
		{
			updateWarriorInfo(NULL, pNormalPosInfo->getPosId());
		}
    }
    else
    {
        //解锁
        setLock(false);
        
        warrior::WarriorCurInfo *pWarriorCurInfo = m_pWarriorModel->getWarriorCurInfoByWID(pUnlockedPosInfo->iWarriorId);
        if(!pWarriorCurInfo)
        {
            //位置上没有弟子
            setEmpty(true);
			updateWarriorInfo(NULL, 0);
			m_pTitleUp->setVisible(true);
			m_pTitleChange->setVisible(false);
        }
        else
        {
            //位置上有弟子
            setEmpty(false);
            m_nWid = pUnlockedPosInfo->iWarriorId;
            //更新弟子
			updateWarriorInfo(pWarriorCurInfo, 0);
			m_pTitleUp->setVisible(false);
			m_pTitleChange->setVisible(true);
        }
    }
}

void ChangeFormationItem::updateWarriorInfo( warrior::WarriorCurInfo * pWarriorInfo, uint8_t iPosId /* = -1 */)
{
	if (pWarriorInfo == NULL)
	{
		//被锁定的位置
		setWarriorInfoVisible(false);
		if (iPosId > 0)
		{
			m_pChangeButtonNode->setVisible(false);
			uint32_t unLockLevel = DM_GET_FORMATION_MODEL->getUnlockLevel(iPosId);
			//m_pLockedLabel->setString(CCString::createWithFormat("%d%s",unLockLevel, D_LOCAL_STRING("FormationUnlock"))->getCString());
			char lv[50];
			sprintf(lv, "%d", unLockLevel);
			std::string tipString(lv);
			tipString.append(D_LOCAL_STRING("FormationUnlock"));
			m_pLockedLabel->setString(tipString.c_str());
		}
		else
		{
			//空位置
			m_pLockedLabel->setString("");
			m_pChangeButtonNode->setVisible(true);
		}
		return;
	}

	setWarriorInfoVisible(true);
	m_pGradeOrange->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() >= warrior::kWarGradeRed);
	m_pGradePurple->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() == warrior::kWarGradePurple);
	m_pGradeBlue->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() == warrior::kWarGradeBlue);
	m_pGradeGreen->setVisible(pWarriorInfo->getBaseInfo()->getColorGrade() <= warrior::kWarGradeGreen);

	m_pLevel->setString(CCString::createWithFormat("%d",pWarriorInfo->getWarriorLv())->getCString());
	m_pName->setString(pWarriorInfo->getBaseInfo()->getWarriorName().c_str());

	if(m_pHeadContainer) 
	{
		m_pHeadContainer->removeAllChildren();
		//m_pHeadContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForFormation(pWarriorInfo->getAvatarId()));
		m_pHeadContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIconForChangeFormation(pWarriorInfo->getAvatarId()));
	}

	  m_pWarriorTypeMark->setWarriorInfo(pWarriorInfo->getBaseInfo());
}

static const int TouchMoveZOrder = 10;
bool ChangeFormationItem::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (this->getParent() == NULL || m_pDelegate == NULL || m_bLock == true || m_bRunningFlipAction == true)
	{
		return false;
	}
	
	CCPoint touchPoint = convertTouchToNodeSpace(pTouch);
	if (m_touchRect.containsPoint(touchPoint) == false)
	{
		return false;
	}

	m_touchBeganPoint = this->getParent()->convertTouchToNodeSpaceAR(pTouch);
	m_pChangeButtonNode->setVisible(false);
	UIUtil::setOpacityRecursive(this, 150);
	changeZOrder(TouchMoveZOrder);
	m_touchMoved = false;
	return true;
}

void ChangeFormationItem::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if (m_bEmpty)
	{
		m_touchMoved = true;
		return;
	}
	
	CCPoint touchPoint = this->getParent()->convertTouchToNodeSpaceAR(pTouch);
	CCPoint offset = ccpSub(touchPoint, m_touchBeganPoint);
	this->setPosition(ccpAdd(getPosition(), offset));
	m_touchBeganPoint = touchPoint;
    if (!m_touchMoved) {
        if (m_pDelegate) {
            m_pDelegate->touchMoveBeganFormationItem(this);
        }
        m_touchMoved = true;
    }
}

void ChangeFormationItem::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	changeZOrder(150);
	m_pChangeButtonNode->setVisible(true);
	UIUtil::setOpacityRecursive(this, 255);

	if (m_touchMoved == false)
	{
		changeButtonCallBack(NULL);
		return;
	}
	
	if (m_pDelegate)
	{
		m_pDelegate->touchMoveEndFormationItem(this);
	}
	this->setPosition(m_originalPosition);
}

void ChangeFormationItem::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	this->setPosition(m_originalPosition);
	changeZOrder(1);
	m_pChangeButtonNode->setVisible(true);
	UIUtil::setOpacityRecursive(this, 255);
}

void ChangeFormationItem::setWarriorInfoVisible(bool visible)
{
	m_pGradeNode->setVisible(visible);
	m_pWarriorInfoNode->setVisible(visible);
	m_pNoWarriorNode->setVisible(!visible);
}

void ChangeFormationItem::changeZOrder( int order )
{
	CCNode* parentNode = this->getParent();
	if (dynamic_cast<PLAdaptiveNode*>(parentNode) != NULL)
	{
		parentNode->setZOrder(order);
	}
	else
	{
		this->setZOrder(order);
	}
}

void ChangeFormationItem::runFlipAction()
{
    if (m_pFlipNode == NULL) {
        return;
    }
    
    const float actionDuration = 0.8f;
    float originalScale = getScale();
    m_pNoWarriorNode->setZOrder(100);
    m_pNoWarriorBg->setFlipX(true);
    m_pNoWarriorNode->setVisible(true);
    m_pFlipNode->setScaleX(-originalScale);
    m_pFlipNode->runAction(CCScaleTo::create(actionDuration, originalScale));
    m_pChangeButtonNode->setVisible(false);
    m_pGradeNode->setVisible(false);
    m_pWarriorInfoNode->setVisible(false);
    m_pMask->setVisible(false);
    this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(actionDuration * 0.5f), CCCallFunc::create(this, callfunc_selector(ChangeFormationItem::changeFlipActionState))));
    this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(actionDuration), CCCallFunc::create(this, callfunc_selector(ChangeFormationItem::flipActionDone))));
    
    m_bRunningFlipAction = true;
}

void ChangeFormationItem::changeFlipActionState()
{
    m_pNoWarriorNode->setZOrder(0);
    m_pNoWarriorBg->setFlipX(false);
    m_pNoWarriorNode->setVisible(false);
    m_pMask->setVisible(true);
    m_pMask->setZOrder(1);
    updateView();
}

void ChangeFormationItem::flipActionDone()
{
    m_bRunningFlipAction = false;
}

void ChangeFormationItem::setMaskVisible(bool visible)
{
    if (m_pMask && m_bLock) {
        m_pMask->stopAllActions();
        CCLOG("setMaskVisible: %d", (int)visible);
        if (visible) {
			m_pMask->setVisible(true);
            m_pMask->runAction(CCFadeTo::create(0.1f, 255));
        }
        else
        {
			m_pMask->setVisible(false);
            m_pMask->setOpacity(0);
        }
    }
}






#pragma mark  - ChangeFormationListView
#pragma mark - - init etc.
ChangeFormationListView::ChangeFormationListView()
:m_pView(NULL)
,m_pDelegate(NULL)
,m_pWarriorModel(NULL)
,m_pFormationModel(NULL)
,m_pWarriors(NULL)
,m_pCellData(NULL)
{
    
}

ChangeFormationListView::~ChangeFormationListView()
{
    UNBINDING_ALL(m_pFormationModel, this);
    CC_SAFE_RELEASE(m_pWarriors);
    CC_SAFE_RELEASE(m_pCellData);
}


bool ChangeFormationListView::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
        
        CC_SAFE_RELEASE(m_pWarriors);
        CC_SAFE_RELEASE(m_pCellData);
        
        m_pWarriors = CCArray::create();
        CC_SAFE_RETAIN(m_pWarriors);
        
        m_pCellData = readCCData("ui/ccb/formation/warriorHeadSmallSelect");
        CC_SAFE_RETAIN(m_pCellData);
        
        m_pWarriorModel = DM_GET_WARRIOR_MODEL;
        m_pFormationModel= DM_GET_FORMATION_MODEL;
        
        removeAllChildren();
        
        updateWarriorList();
        
		m_pView = DTableView::create(this, CCSizeMake(640, 190));
		m_pView->setDirection(kCCScrollViewDirectionHorizontal);
		m_pView->setDelegate(this);
		m_pView->setVerticalFillOrder(kCCTableViewFillTopDown);
		addChild(m_pView);
		m_pView->reloadData();
        
		bRet = true;
	}while(0);
    
	return bRet;
}

void ChangeFormationListView::onEnter()
{
    BINDING_ONLY(m_pFormationModel, this, ChangeFormationListView::onFormationJoined, D_EVENT_FORMATION_JOIN);
    DLayer::onEnter();
}

void ChangeFormationListView::onExit()
{
    UNBINDING_ALL(m_pFormationModel, this);
    DLayer::onExit();
}

#pragma mark - - CCTableViewDelegate
void ChangeFormationListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    if(m_pDelegate)
    {
        ChangeFormationListViewCell *pCell = static_cast<ChangeFormationListViewCell *>(cell);
        m_pDelegate->touchChangeListViewCell(pCell);
    }
}

CCSize ChangeFormationListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(135, 190);
}

CCTableViewCell* ChangeFormationListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
    ChangeFormationListViewCell *pCell = static_cast<ChangeFormationListViewCell *>(table->dequeueCell());
    if (!pCell)
    {
        pCell = ChangeFormationListViewCell::create(m_pCellData);
    }
    
    warrior::WarriorCurInfo* pWarrior = (warrior::WarriorCurInfo*)m_pWarriors->objectAtIndex(idx);
    
    pCell->setWarriorInfo(pWarrior);
    
    if(m_pDelegate)
    {
        switch (m_pDelegate->getChangeStatus()) {
            case ChangeFormationView::kChangeStatusFalse:
                pCell->setChangeState(false);
                pCell->setSelected(false);
                break;
            case ChangeFormationView::kChangeStatusFromTop:
                pCell->setChangeState(true);
                pCell->setSelected(false);
                break;
            case ChangeFormationView::kChangeStatusFromBottom:
                pCell->setChangeState(false);
                pCell->setSelected(pCell->getWarriorID() == m_pDelegate->getSelectedWarriorId());
                break;
            default:
                break;
        }
    }
    return pCell;
}

uint32_t ChangeFormationListView::numberOfCellsInTableView(CCTableView *table)
{
    return m_pWarriors->count();
}

#pragma mark - - methods private
void ChangeFormationListView::onFormationJoined()
{
    updateWarriorList();
    
    updateView();
}

void ChangeFormationListView::updateWarriorList()
{
    m_pWarriorModel->getWarriorsUnBattle(m_pWarriors);
}

#pragma mark - - methods public
void ChangeFormationListView::updateView()
{
    m_pView->reloadData();
}


void ChangeFormationListView::resetChangeState()
{
    
    for(uint32_t i = 0 ; i < m_pWarriors->count() ; i++)
    {
        CCTableViewCell *cell = m_pView->cellAtIndex(i);
        ChangeFormationListViewCell *pCell = (ChangeFormationListViewCell *)cell;
        if(pCell)
        {
            pCell->setChangeState(false);
            pCell->setSelected(false);
        }
    }
}

void ChangeFormationListView::setChangeState(const bool &changeState)
{
    for(uint32_t i = 0 ; i < m_pWarriors->count() ; i++)
    {
        CCTableViewCell *cell = m_pView->cellAtIndex(i);
        ChangeFormationListViewCell *pCell = (ChangeFormationListViewCell *)cell;
        if(pCell)
        {
            pCell->setChangeState(changeState);
        }
    }
}



#pragma mark - - ChangeFormationListViewCell
#pragma mark - - init etc.
ChangeFormationListViewCell::ChangeFormationListViewCell()
:m_bChangeState(false)
,m_pWarriorInfo(NULL)
,m_pPrice(NULL)
,m_pWarriorTypeMark(NULL)
,m_pSelected(NULL)
,m_pCanChange(NULL)
,m_pStars(NULL)
,m_pUpLevel(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
//,m_pBorderHighLight(NULL)
//,m_pBorder(NULL)
,m_pContainer(NULL)
,m_pGrade(NULL)
{
    
}

ChangeFormationListViewCell::~ChangeFormationListViewCell()
{
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo,this);
    
    CC_SAFE_RELEASE(m_pPrice);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pSelected);
    CC_SAFE_RELEASE(m_pCanChange);
    CC_SAFE_RELEASE(m_pStars);
    CC_SAFE_RELEASE(m_pUpLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
//    CC_SAFE_RELEASE(m_pBorderHighLight);
//    CC_SAFE_RELEASE(m_pBorder);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pGrade);
}


ChangeFormationListViewCell* ChangeFormationListViewCell::create(cocos2d::extension::CCData* pData)
{
    ChangeFormationListViewCell* pRet = new ChangeFormationListViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


bool ChangeFormationListViewCell::init(cocos2d::extension::CCData* pData)
{
    
    CCB_READER_DATA_AND_ADDCHILD(pData);
    setChangeState(false);
    setSelected(false);
    return true;
}


bool ChangeFormationListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPrice", CCLabelBMFont *, this->m_pPrice);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelected", CCNode *, this->m_pSelected);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanChange", CCNode *, this->m_pCanChange);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStars", CCNode *, this->m_pStars);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCNode *, this->m_pUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBorderHighLight", CCScale9Sprite *, this->m_pBorderHighLight);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBorder", CCScale9Sprite *, this->m_pBorder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCScale9Sprite *, this->m_pGrade);
    
    return false;
}

#pragma mark - - methods private
void ChangeFormationListViewCell::updateView()
{
    CCAssert(m_pWarriorInfo, "错误的弟子ID");
    
    m_pPrice->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorForce())->getCString());
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    
//    m_pBorderHighLight->setVisible(m_pWarriorInfo->getBaseInfo()->getColorGrade() > warrior::kWarGradeBlue);
//    m_pBorder->setVisible(m_pWarriorInfo->getBaseInfo()->getColorGrade() <= warrior::kWarGradeBlue);
    m_pStars->setVisible(m_pWarriorInfo->getBaseInfo()->getColorGrade() > warrior::kWarGradePurple);
    
    
    m_pUpLevel->removeAllChildren();
    m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(m_pWarriorInfo->getWarriorUpLv()));
    m_pLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getWarriorLv())->getCString());
    m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
    
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForChangeFormation(m_pWarriorInfo->getAvatarId()));
    m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pWarriorInfo->getWid()));
}


#pragma mark - - methods public


int  ChangeFormationListViewCell::getWarriorID()
{
    return m_pWarriorInfo?m_pWarriorInfo->getWid():0;
}
void ChangeFormationListViewCell::setWarriorInfo(warrior::WarriorCurInfo * pWarriorCurInfo)
{
    if(pWarriorCurInfo== m_pWarriorInfo) return;
    CC_SAFE_RETAIN(pWarriorCurInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo,this);
    
    m_pWarriorInfo = pWarriorCurInfo;
    BINDING_ONLY(m_pWarriorInfo, this, ChangeFormationListViewCell::updateView, D_EVENT_WARRIORINFO_UPDATE);
    updateView();
}

void ChangeFormationListViewCell::setSelected(bool value)
{
    m_pSelected->setVisible(value);
}

void ChangeFormationListViewCell::setChangeState(bool changeState)
{
    m_bChangeState = changeState;
    
    m_pCanChange->setVisible(m_bChangeState);
}

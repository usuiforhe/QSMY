//  Warrior.cpp
//	弟子魂魄列表

//  Created by cll on 13-3-29
//  Copyright (c) 2013, thedream. All rights reserved.
//
#include "Warrior.h"
#include "../user/userInfo.h"
#include "managers/GameManager.h"
#include "WarriorInfo.h"
#include "Promotion.h"
#include "TransForce.h"
#include "WarriorDevelop.h"
#include "WarriorTransForceSettle.h"
#include "WarriorDevelopSettle.h"
#include "RecruitSettle.h"
#include "../Game.h"
#include "WarriorListView.h"
#include "SoulListView.h"
#include "../skill/SkillInfo.h"
#include "../skill/SkillUpgrade.h"
#include "WarriorUpgrade.h"
#include "WarriorSortView.h"

USING_NS_CC;

using namespace  warrior;

#define  WARRIOR_TAB_RECT		CCRectMake(0, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(0, PL_MAX_HEIGHT_DIFF / 2), 640, PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(780, -PL_MAX_HEIGHT_DIFF))



/************************ warrior class begin*****************************/

Warrior::Warrior()
:m_pContainer(NULL)
,m_pPageContainer(NULL)
,m_pDTabView(NULL)
,m_pWarriorInfoView(NULL)
,m_pSoulInfoView(NULL)
,m_pPromotionView(NULL)
,m_pUpgradeView(NULL)
,m_pTransForceView(NULL)
,m_pTransSettleView(NULL)
,m_pDevelopView(NULL)
,m_pDevSettleView(NULL)
,m_pRecruitSettleView(NULL)
,m_pTabContainer(NULL)
,m_pSkillInfo(NULL)
,m_pSkillUpgrade(NULL)
,m_eShowingType(kWarriorNone)
,m_pWarriorListView(NULL)
,m_pSoulList(NULL)
,m_pSortView(NULL)
,m_pSortButton(NULL)
,m_pPictureCollect_Diff(NULL)
{
    
}

Warrior::~Warrior()
{
    removeAllChildren();
	if(m_pSortView && m_pSortView->getParent())
	{
		m_pSortView->removeFromParent();
	}
    CC_SAFE_RELEASE(m_pSkillInfo);
    CC_SAFE_RELEASE(m_pSkillUpgrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pPageContainer);
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pWarriorInfoView);
    CC_SAFE_RELEASE(m_pSoulInfoView);
	CC_SAFE_RELEASE(m_pPromotionView);
	CC_SAFE_RELEASE(m_pTransForceView);
	CC_SAFE_RELEASE(m_pDevelopView);
	CC_SAFE_RELEASE(m_pTransSettleView);
	CC_SAFE_RELEASE(m_pDevSettleView);
	CC_SAFE_RELEASE(m_pRecruitSettleView);
    CC_SAFE_RELEASE(m_pTabContainer);
    CC_SAFE_RELEASE(m_pUpgradeView);
	CC_SAFE_RELEASE(m_pWarriorListView);
	CC_SAFE_RELEASE(m_pSoulList);
	CC_SAFE_RELEASE(m_pSortView);
	CC_SAFE_RELEASE(m_pPictureCollect_Diff);
	CC_SAFE_RELEASE(m_pSortButton);
}

bool Warrior::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_hero/PL_hero_lb");
    
	m_pDTabView = DTabView::create(WARRIOR_TAB_RECT,20);
	m_pDTabView->setDelegate(this);
	m_pDTabView->retain();
    
    m_pWarriorListView = WarriorListView::create(this, NULL, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pPageContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	//m_pDTabView->addTabWithFontImg(pWarriorListView,"ui/common/font_sheet_disciple.png");
	m_pDTabView->addTab(m_pWarriorListView, "", "PL_ui/all/all_btn_fenye_lan_up.png", "PL_ui/all/all_btn_fenye_lv.png", NULL, "PL_ui/liebiao/hero_lb_wenzi_yingxiong.png");
	CC_SAFE_RETAIN(m_pWarriorListView);

	m_pSoulList = SoulListView::create(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pPageContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
    m_pSoulList->setDelegate(this);
	//m_pDTabView->addTabWithFontImg(m_pSoulList,"ui/common/font_sheet_soul.png");
	m_pDTabView->addTab(m_pSoulList, "", "PL_ui/all/all_btn_fenye_lan_up.png", "PL_ui/all/all_btn_fenye_lv.png", NULL, "PL_ui/liebiao/hero_lb_wenzi_hunpo.png");
	CC_SAFE_RETAIN(m_pSoulList);
    
   const CCSize containerSize = m_pPageContainer->getContentSize();
	m_pPictureCollect_Diff = PictureCollect::create(this,PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(containerSize, -PL_MAX_HEIGHT_DIFF));
	m_pDTabView->addTab(m_pPictureCollect_Diff,"","PL_ui/all/all_btn_fenye_lan_up.png", "PL_ui/all/all_btn_fenye_lv.png", NULL, "PL_ui/tujian/tujian_btn_txt_tujiani.png");
	CC_SAFE_RETAIN(m_pPictureCollect_Diff);

	m_pTabContainer->addChild(m_pDTabView);
	
	showViewList();

	//popup自适应
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pContainer);
    
	m_pSortButton->setTarget(this,menu_selector(Warrior::onSortClick));
	return true;
}

void Warrior::onEnter()
{
    DLayer::onEnter();
    CCInteger *pId = dynamic_cast<CCInteger *>(getUserObject());
    if(pId)
    {
        m_pDTabView->selectedTab(pId->getValue());
    }
}

bool Warrior::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPageContainer", CCNode *, m_pPageContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTabContainer", CCNode *, m_pTabContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSortButton" , CCMenuItemImage*,m_pSortButton);
    
    return false;
}

void Warrior::showViewList()
{
    m_pContainer->removeAllChildrenWithCleanup(false);
    UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	UserInfoView::sharedUserInfoView()->setDisplayType(user::kUserInfoFull);
	//m_pContainer->addChild(UserInfoView::sharedUserInfoView());	//添加基本信息title

	CCNode *userInfo = CCNode::create();
	userInfo->setPosition(ccp(0,0));
	PL_MOVE_NODE_ABOVE_CENTER_Y(userInfo);
	userInfo->addChild(UserInfoView::sharedUserInfoView());	//添加基本信息title
	m_pContainer->addChild(userInfo);
}



void Warrior::showDevSettleView(warrior::WarriorCurInfo * pWarrior)
{
   /* if(m_pDevSettleView==NULL)
    {
        m_pDevSettleView = WarriorDevelopSettleView::create();
        m_pDevSettleView->setDelegate(this);
        m_pDevSettleView->retain();
    }
    m_pDevSettleView->setWarrior(pWarrior);
    Game::sharedGame()->openPopup(m_pDevSettleView);*/
}

void Warrior::closeInfoView(WarriorViewType type)
{
    if(type==kSkillInfoView && m_lastType==kWarriorInfo)
    {
		showInfoView(kWarriorInfo, m_pWarrior);
		m_eShowingType = m_lastType;
    }else
    {
        showViewList();
		WarriorDelegate::closeInfoView(type);
		m_eShowingType = kWarriorNone;
		Game::sharedGame()->setNavigatorVisable(true);
    }
}

void Warrior::showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param)
{
	m_eShowingType = type;
    WarriorDelegate::showInfoView(type, pWarrior, param);
    
	warrior::PotBuff* pPotBuff = NULL;
    
	m_pContainer->removeAllChildrenWithCleanup(false);
    Game::sharedGame()->setNavigatorVisable(true);
	switch(type)
	{
        case kWarriorInfo:		//弟子详细信息
            if(m_pWarriorInfoView==NULL)
            {
                m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeOperate);
                m_pWarriorInfoView->setDelegate(this);
                m_pWarriorInfoView->retain();
            }
            m_pWarriorInfoView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pWarriorInfoView);
			Game::sharedGame()->setNavigatorVisable(false);
            break;
        case kSoulInfo:		//弟子魂魄详细信息
            if(m_pSoulInfoView==NULL)
            {
                m_pSoulInfoView = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
                m_pSoulInfoView->setDelegate(this);
                m_pSoulInfoView->retain();
            }
            m_pSoulInfoView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pSoulInfoView);
			//Game::sharedGame()->setNavigatorVisable(false);
            break;
        case kPromotionView:		//突破详细信息
            if(m_pPromotionView==NULL)
            {
                m_pPromotionView = PromotionView::create();
                m_pPromotionView->setDelegate(this);
                m_pPromotionView->retain();
            }
            m_pPromotionView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pPromotionView);
            break;
        case kUpgradeView:
            if (m_pUpgradeView == NULL)
            {
                m_pUpgradeView = WarriorUpgradeView::create();
                m_pUpgradeView->setWarriorDelegate(this);
                m_pUpgradeView->retain();
            }
            m_pContainer->addChild(m_pUpgradeView);
            m_pUpgradeView->setWarrior(pWarrior);
            break;
        case kTransForceView:		//传功详细信息
            if(m_pTransForceView==NULL)
            {
                m_pTransForceView = TransForceView::create();
                m_pTransForceView->setDelegate(this);
                m_pTransForceView->retain();
            }
            m_pTransForceView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pTransForceView);
            break;
            
        case kDevelopView:		//培养详细信息
            if(m_pDevelopView==NULL)
            {
                m_pDevelopView = WarriorDevelopView::create();
                m_pDevelopView->setDelegate(this);
                m_pDevelopView->retain();
            }
            m_pDevelopView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pDevelopView);
            
            //pPotBuff = DM_GET_WARRIOR_MODEL->getWarriorPotBuff(pWarrior);
            //
            ////如果有培养buff未处理，切换到结算界面
            //if (pPotBuff)
            //{
            //    showDevSettleView(pWarrior);
            //}
            break;
            
        case kTransSettleView:
            if(m_pTransSettleView==NULL)
            {
                m_pTransSettleView = WarriorTransForceSettleView::create();
                m_pTransSettleView->setDelegate(this);
                m_pTransSettleView->retain();
            }
            m_pTransSettleView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pTransSettleView);
            break;
        case kDevSettleView:
            //showDevSettleView(pWarrior);
            break;
        case kRecruitSettleView:
            if(m_pRecruitSettleView==NULL)
            {
                m_pRecruitSettleView = RecruitSettleView::create();
                m_pRecruitSettleView->setDelegate(this);
                m_pRecruitSettleView->retain();
            }
            m_pRecruitSettleView->setWarrior(pWarrior);
            m_pContainer->addChild(m_pRecruitSettleView);
            break;
        case kSkillInfoView:
        {
            if(m_pSkillInfo==NULL)
            {
                m_pSkillInfo = SkillInfoView::create();
                m_pSkillInfo->retain();
                m_pSkillInfo->setDelegate(this);
            }
			//魂魄模式进去？不能点击修炼技能
			m_pSkillInfo->setSkillViewOnly(!DM_GET_WARRIOR_MODEL->isMyWarrior(pWarrior));

            CCInteger* pSkillId = static_cast<CCInteger*>(param);
            m_pSkillInfo->setSkillInfo(pWarrior,pSkillId->getValue());
            m_pContainer->addChild(m_pSkillInfo);
            break;
        }
        case kSkillUpgradeView:
        {
            if(m_pSkillUpgrade==NULL)
            {
                m_pSkillUpgrade = SkillUpgradeView::create();
                m_pSkillUpgrade->retain();
                m_pSkillUpgrade->setDelegate(this);
            }
            m_pSkillUpgrade->setWarriorInfo(pWarrior);
            m_pContainer->addChild(m_pSkillUpgrade);
            break;
        }
        case kWarriorNone:
        default:
            closeInfoView(type);
            break;
	}
    
}

/*void Warrior::showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,PictureCollectCell* param_t,cocos2d::CCObject*)
{
	m_eShowingType = type;
	WarriorDelegate::showInfoView(type, pWarrior,param_t,NULL);
	warrior::PotBuff* pPotBuff = NULL;
	m_pContainer->removeAllChildrenWithCleanup(false);
	Game::sharedGame()->setNavigatorVisable(true);
	switch(type)
	{
	case kWarriorInfo:		//弟子详细信息
		if(m_pWarriorInfoView==NULL)
		{
			m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeOperate);
			m_pWarriorInfoView->setDelegate(this);
			m_pWarriorInfoView->retain();
		}
		m_pWarriorInfoView->setWarrior(pWarrior,param_t);
		m_pContainer->addChild(m_pWarriorInfoView);
		Game::sharedGame()->setNavigatorVisable(false);
		break;
	default:
		closeInfoView(type);
		break;
	}
}*/

void Warrior::onBackKey()
{
	if (m_eShowingType != kWarriorNone)
	{
		closeInfoView(m_eShowingType);
		return;
	}
	BackKeyInterface::onBackKey();
}

void Warrior::SelectTabAtIndex(DTabView *tabView, uint32_t idx)
{
	switch (idx)
	{
	case 0:
		m_pSortButton->setVisible(true);
		break;
	case 1:
		m_pSortButton->setVisible(false);
		break;
	}
	
}

void Warrior::showPopView()
{
	if(m_pSortView ==NULL)
	{
		m_pSortView = WarriorSortView::create();
		m_pSortView->setDelegate(this);
		CC_SAFE_RETAIN(m_pSortView);
	}
	this->addChild(m_pSortView);
}

void Warrior::setSortList(warrior::WarriorSortType type)
{
	if(type == warrior::kSortTypeNone) return;
	m_pWarriorListView->setSortList(type);
}

void Warrior::onSortClick(cocos2d::CCObject* psender)
{
	showPopView();
}
/************************ warrior class end*****************************/
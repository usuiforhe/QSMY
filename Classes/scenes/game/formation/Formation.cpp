//
//  Formation.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Formation.h"
#include "FormationView.h"
#include "ChangeFormationView.h"
#include "FormationSelectView.h"
#include "../skill/SkillInfo.h"
#include "../skill/SkillUpgrade.h"
#include "../equip/EquipSelectView.h"
#include "../equip/EquipInfoView.h"
#include "../equip/EquipEnhance.h"
#include "../equip/EquipRefine.h"
#include "../warrior/WarriorInfo.h"
#include "managers/GameManager.h"

#include "../warrior/Promotion.h"
#include "../warrior/TransForce.h"
#include "../warrior/WarriorDevelop.h"
#include "../warrior/WarriorTransForceSettle.h"
#include "../warrior/WarriorDevelopSettle.h"
#include "../warrior/RecruitSettle.h"
#include "../warrior/WarriorUpgrade.h"
#include "../Game.h"

USING_NS_CC;

#pragma mark - Formation

#pragma mark - - init etc.
Formation::Formation()
:m_pChangeFormation(NULL)
,m_pFormation(NULL)
,m_pSkillInfo(NULL)
,m_pSkillUpgrade(NULL)
,m_pDressChange(NULL)
,m_pEquipList(NULL)
,m_pEquipRefine(NULL)
,m_pWarriorModel(NULL)
,m_pWarriorInfoView(NULL)
,m_pSelectPos(NULL)
,m_Container(NULL)
,m_pPromotionView(NULL)
,m_pUpgradeView(NULL)
,m_pTransSettleView(NULL)
,m_pDevelopView(NULL)
,m_pDevSettleView(NULL)
,m_pRecruitSettleView(NULL)
,m_pFormationModel(NULL)
,m_pFormationSelectView(NULL)
,m_pPopupLayer(NULL)
,m_bFirstEnter(true)
,m_eShowingType(kWarriorNone)
{
    
}

Formation::~Formation()
{
    CC_SAFE_RELEASE(m_pChangeFormation);
    CC_SAFE_RELEASE(m_pFormation);
	CC_SAFE_RELEASE(m_pSkillInfo);
	CC_SAFE_RELEASE(m_pSkillUpgrade);
	CC_SAFE_RELEASE(m_pDressChange);
	CC_SAFE_RELEASE(m_pEquipList);
	CC_SAFE_RELEASE(m_pEquipRefine);
	CC_SAFE_RELEASE(m_pWarriorInfoView);
    CC_SAFE_RELEASE(m_pFormationSelectView);
    
    CC_SAFE_RELEASE(m_pPromotionView);
    CC_SAFE_RELEASE(m_pUpgradeView);
    CC_SAFE_RELEASE(m_pTransSettleView);
    CC_SAFE_RELEASE(m_pDevelopView);
    CC_SAFE_RELEASE(m_pDevSettleView);
    CC_SAFE_RELEASE(m_pRecruitSettleView);
}

bool Formation::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pFormationModel = DM_GET_FORMATION_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    m_Container = CCNode::create();
    addChild(m_Container);
    m_pPopupLayer = CCNode::create();
	PL_MOVE_NODE_BELOW_CENTER_Y(m_pPopupLayer);
    addChild(m_pPopupLayer,1);
    FormationDelegate::setLastPage(kChangeFormationView);
    return true;
}

void Formation::onEnter()
{
    DLayer::onEnter();
    if(m_bFirstEnter) showFormationView();
}

#pragma mark - - FormationDelegate
void Formation::showFormationView()
{
    m_bFirstEnter = false;
    m_Container->setVisible(true);
    m_Container->removeAllChildren();
    m_pPopupLayer->removeAllChildren();
    if(m_pFormation == NULL)
    {
        m_pFormation = FormationView::create();
        m_pFormation->setDelegate(this);
        m_pFormation->retain();
        
    }
    m_Container->addChild(m_pFormation);

	if (kFormationView == m_pLastPage)
	{
		m_pFormation->updateFormation();
	} else if(kPartnerView == m_pLastPage)
	{
		m_pFormation->showPartnerView(true);
	}
    
    CCInteger *pId = (CCInteger *)getUserObject();
    if(pId)
    {
        m_pFormation->showWarrior(pId->getValue());
        setUserObject(NULL);
    }
	FormationDelegate::setLastPage(kChangeFormationView);
	WarriorDelegate::closeInfoView();
}

void Formation::showChangeFormationView()
{
    m_bFirstEnter = false;
    m_pSelectPos = NULL;
    m_Container->setVisible(true);
    m_Container->removeAllChildren();
    m_pPopupLayer->removeAllChildren();
    if(m_pChangeFormation==NULL)
    {
        m_pChangeFormation = ChangeFormationView::create();
        m_pChangeFormation->setDelegate(this);
        m_pChangeFormation->retain();
		m_pChangeFormation->setWarriorInfo(m_pWarrior);
    }
    m_Container->addChild(m_pChangeFormation);
	WarriorDelegate::closeInfoView();
}

void Formation::showEquipSelectView(cocos2d::CCArray* pEquiList,formation::PositionInfo* pPos,uint32_t currEquipProperty)
{
    if(pPos!=NULL) m_pSelectPos = pPos;
    CC_ASSERT(m_pSelectPos);
	m_pPopupLayer->removeAllChildren();
    if(m_pEquipList==NULL)
    {
        m_pEquipList = EquipSelectView::create();
        m_pEquipList->setEquipSelecteDelegate(this);
        m_pEquipList->retain();
	}
	m_pEquipList->setEquipLastProperty(currEquipProperty);
	m_pEquipList->setEquipList(pEquiList,m_pSelectPos->iWarriorId);
	m_pPopupLayer->addChild(m_pEquipList);
    m_Container->setVisible(false);
}

void Formation::showEquipInfoView(uint32_t eid,formation::PositionInfo* pPos)
{
    if(pPos!=NULL) m_pSelectPos = pPos;
    CC_ASSERT(m_pSelectPos);
    m_pSelectPos = pPos;
	m_pPopupLayer->removeAllChildren();
  //  if(m_pEquipInfo==NULL)
  //  {
  //      m_pEquipInfo = EquipInfoView::create(EquipInfoView::kTypeEnhanceAndChange);
		//PL_MOVE_WITH_RESOLUTION_DIFF(m_pEquipInfo , -1);
  //      m_pEquipInfo->retain();
  //  }

	EquipInfoView *m_pEquipInfo = EquipInfoView::create(EquipInfoView::kTypeEnhanceAndChange);
	PL_MOVE_WITH_RESOLUTION_DIFF(m_pEquipInfo , -1);

	m_pEquipInfo->setEquip(DM_GET_EQUIP_MODEL->getEquipCurInfoByID(eid));
	m_pEquipInfo->setFormationInfo(this);
	m_pPopupLayer->addChild(m_pEquipInfo);
	setUserObject(CCInteger::create(pPos->getWarriorId()));
}

void Formation::showEquipEnhanceView(uint32_t eid)
{
	m_pPopupLayer->removeAllChildren();
	/*if(m_pEquipEnhance==NULL)
	{
	m_pEquipEnhance = EquipEnhance::create();
	m_pEquipEnhance->retain();
	PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipEnhance);
	}
	m_pEquipEnhance->setEquip(DM_GET_EQUIP_MODEL->getEquipCurInfoByID(eid));
	m_pEquipEnhance->setFormationInfo(this);*/

	EquipEnhance* m_pEquipEnhance = EquipEnhance::create();
	PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquipEnhance);
	m_pEquipEnhance->setEquip(DM_GET_EQUIP_MODEL->getEquipCurInfoByID(eid));
	m_pEquipEnhance->setFormationInfo(this);

	//处理点击穿透
	CCLayerColor* colorLayer = CCLayerColor::create(ccc4(0,0,0,100));
	PL_MOVE_NODE_BELOW_CENTER_Y(colorLayer);
	m_pPopupLayer->addChild(colorLayer);

	m_pPopupLayer->addChild(m_pEquipEnhance);
}

void Formation::showEquipRefineView(uint32_t eid)
{
	m_pPopupLayer->removeAllChildren();
	if (m_pEquipRefine==NULL)
	{
		m_pEquipRefine = EquipRefine::create();
		m_pEquipRefine->retain();
	}

	m_pEquipRefine->setEquip(DM_GET_EQUIP_MODEL->getEquipCurInfoByID(eid));
	m_pEquipRefine->setFormationDelegate(this);
	m_pPopupLayer->addChild(m_pEquipRefine);
}

void Formation::showFormationSelectView( formation::PositionInfo* pPos )
{
	if(pPos!=NULL)
	{
		m_pSelectPos = pPos;
	}
	CC_ASSERT(m_pSelectPos);

	m_pPopupLayer->removeAllChildren();
	
	if(m_pFormationSelectView==NULL)
	{
		m_pFormationSelectView = FormationSelectView::create(pPos);
		m_pFormationSelectView->setDelegate(this);
		m_pFormationSelectView->retain();
	}

	m_pFormationSelectView->setPosInfo(pPos);
	m_pPopupLayer->addChild(m_pFormationSelectView);
	m_Container->setVisible(false);
}

void Formation::selectedEquip(equip::EquipCurInfo* pEquip)
{
    if (pEquip && m_pSelectPos != NULL)
    {
        if(DM_GET_EQUIP_MODEL->isEquipExist(pEquip->getId()))
        {
            HTTP_CLIENT->equipWarrior(m_pSelectPos->iId,pEquip->getId());
        }else
        {
            //tip
        }
        
    }
    setUserObject(CCInteger::create(m_pSelectPos->iWarriorId));
    showFormationView();
    
}

void Formation::closeInfoView(WarriorViewType type)
{
    if(type==kSkillInfoView && m_lastType==kWarriorInfo)
    {
        showInfoView(kWarriorInfo, m_pWarrior);
		m_eShowingType = m_lastType;
    }
	else if(type == kWarriorInfo && m_lastType == kFormationSelectView)
	{
		showInfoView(kFormationSelectView, m_pWarrior, m_pSelectPos);
		m_eShowingType = m_lastType;
	}
	else
    {
        showFormationView();
        WarriorDelegate::closeInfoView(type);
		m_eShowingType = kWarriorNone;
    }
}

void Formation::showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param)
{
	m_eShowingType = type;
    WarriorDelegate::showInfoView(type, pWarrior, param);
    m_pPopupLayer->removeAllChildren();
    warrior::PotBuff* pPotBuff = NULL;
    Game::sharedGame()->setNavigatorVisable(true);
    switch(type)
	{
        case kWarriorInfo:
        {
            if(m_pWarriorInfoView==NULL)
            {
                m_pWarriorInfoView = WarriorInfoView::create(WarriorInfoView::kTypeOperate);
                m_pWarriorInfoView->retain();
                m_pWarriorInfoView->setDelegate(this);
            }
			Game::sharedGame()->setNavigatorVisable(false);
            m_pWarriorInfoView->setWarrior(pWarrior);
            m_pPopupLayer->addChild(m_pWarriorInfoView);
			
            break;
        }
            
        case kPromotionView:		//突破详细信息
            if(m_pPromotionView==NULL)
            {
                m_pPromotionView = PromotionView::create();
                m_pPromotionView->setDelegate(this);
                m_pPromotionView->retain();
            }
            m_pPromotionView->setWarrior(pWarrior);
            m_pPopupLayer->addChild(m_pPromotionView);
            break;
        case kUpgradeView:		//英雄强化
            if(m_pUpgradeView==NULL)
            {
                m_pUpgradeView = WarriorUpgradeView::create();
                m_pUpgradeView->setWarriorDelegate(this);
                m_pUpgradeView->retain();
            }
            m_pUpgradeView->setWarrior(pWarrior);
            m_pPopupLayer->addChild(m_pUpgradeView);
            break;
            
        case kDevelopView:		//培养详细信息
            
            if(m_pDevelopView==NULL)
            {
                m_pDevelopView = WarriorDevelopView::create();
                m_pDevelopView->setDelegate(this);
                m_pDevelopView->retain();
            }
            m_pDevelopView->setWarrior(pWarrior);
            m_pPopupLayer->addChild(m_pDevelopView);
            
            pPotBuff = DM_GET_WARRIOR_MODEL->getWarriorPotBuff(pWarrior);
            
            //如果有培养buff未处理，切换到结算界面
          /*  if (pPotBuff)
            {
                showDevSettleView(pWarrior);
            }*/
            break;
            
        case kTransSettleView:
            if(m_pTransSettleView==NULL)
            {
                m_pTransSettleView = WarriorTransForceSettleView::create();
                m_pTransSettleView->setDelegate(this);
                m_pTransSettleView->retain();
            }
            m_pTransSettleView->setWarrior(pWarrior);
            m_pPopupLayer->addChild(m_pTransSettleView);
            break;
        case kDevSettleView:
            showDevSettleView(pWarrior);
            break;
        case kRecruitSettleView:
            if(m_pRecruitSettleView==NULL)
            {
                m_pRecruitSettleView = RecruitSettleView::create();
                m_pRecruitSettleView->setDelegate(this);
                m_pRecruitSettleView->retain();
            }
            m_pRecruitSettleView->setWarrior(pWarrior);
            m_pPopupLayer->addChild(m_pRecruitSettleView);
            break;
        case kSkillInfoView:
        {
            m_pPopupLayer->removeAllChildren();
            if(m_pSkillInfo==NULL)
            {
                m_pSkillInfo = SkillInfoView::create();
                m_pSkillInfo->retain();
                m_pSkillInfo->setDelegate(this);
            }
            CCInteger* pSkillId = static_cast<CCInteger*>(param);
            m_pSkillInfo->setSkillInfo(pWarrior,pSkillId->getValue());
            m_pPopupLayer->addChild(m_pSkillInfo);
            break;
        }
        case kSkillUpgradeView:
        {
            m_pPopupLayer->removeAllChildren();
            if(m_pSkillUpgrade==NULL)
            {
                m_pSkillUpgrade = SkillUpgradeView::create();
                m_pSkillUpgrade->retain();
                m_pSkillUpgrade->setDelegate(this);
            }
            m_pSkillUpgrade->setWarriorInfo(pWarrior);
            m_pPopupLayer->addChild(m_pSkillUpgrade);
            break;
        }
		case kFormationSelectView:
			{
				if(param!=NULL)
				{
					m_pSelectPos = static_cast< formation::PositionInfo*> (param);
				}
				CC_ASSERT(m_pSelectPos);
				m_pPopupLayer->removeAllChildren();

				if(m_pFormationSelectView==NULL)
				{
					m_pFormationSelectView = FormationSelectView::create(m_pSelectPos);
					m_pFormationSelectView->setDelegate(this);
					m_pFormationSelectView->retain();
				}
				m_pFormationSelectView->setDelegate(this);
				m_pFormationSelectView->setPosInfo(m_pSelectPos);
				m_pPopupLayer->addChild(m_pFormationSelectView);
				break;
			}
		case kDressView:
		{
			m_pPopupLayer->removeAllChildren();
			if(m_pDressChange==NULL)
			{
				m_pDressChange = DressChange::create();
				m_pDressChange->retain();
				m_pDressChange->setDelegate(this);
			}
			m_pDressChange->setWarriorInfo(pWarrior);
			m_pPopupLayer->addChild(m_pDressChange);
			break;
		}
        case kSoulInfo:
        default:
            break;
            
	}
    
}


void Formation::showDevSettleView(warrior::WarriorCurInfo * pWarrior)
{
    /*if(m_pDevSettleView==NULL)
    {
        m_pDevSettleView = WarriorDevelopSettleView::create();
        m_pDevSettleView->setDelegate(this);
        m_pDevSettleView->retain();
    }
    m_pDevSettleView->setWarrior(pWarrior);
    Game::sharedGame()->openPopup(m_pDevSettleView);*/
}

void Formation::onBackKey()
{
	if (m_eShowingType != kWarriorNone)
	{
		closeInfoView(m_eShowingType);
		return;
	}
	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	{
		showFormationView();
		return;
	}
	
	BackKeyInterface::onBackKey();
}

void Formation::joinFormation( uint32_t wid,uint32_t posId )
{
	WarriorDelegate::closeInfoView(kFormationSelectView);
	if (kChangeFormationView == m_pLastPage)
	{
		m_pChangeFormation->join(wid, posId);
		showChangeFormationView();
	} else if (kFormationView == m_pLastPage) {
		HTTP_CLIENT->joinFormation(wid,posId,this,callfuncJson_selector(FormationView::onCallBack));
		showFormationView();
		//m_pFormation->setCurrentPage();  
	} else if(kPartnerView == m_pLastPage)
	{
		HTTP_CLIENT->joinGundam(posId, wid);
		showFormationView();
	}
}



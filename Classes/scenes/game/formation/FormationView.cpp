//
//  FormationView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "FormationView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/SoundManager.h"
#include "utils/StringUtil.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gundam_engineer_formation ;

#define PAGE_MARGIN 150
#define PAGE_SIZE_WIDTH (220 + PAGE_MARGIN*2)
#define PARTNER_ICON_NUM 1


#pragma mark - FormationView
#pragma mark - - init etc.
FormationView::FormationView()
:m_pDelegate(NULL)
,m_pFormationListView(NULL)
,m_pPageView(NULL)
,m_pFormationModel(NULL)
,m_pWarriorModel(NULL)
,m_iCurrentPage(0)
,m_pWarriorInfoViews(NULL)
,m_pEquipModel(NULL)
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
,m_pWarriorInfo(NULL)
,m_pFateDisable(NULL)
,m_pFateEnable(NULL)
,m_pQuickEquip(NULL)
,m_pChangeHero(NULL)
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
,m_pCloseButton(NULL)
,m_pFormationNode(NULL)
,m_pPartnerNode(NULL)
,m_pWHeadData(NULL)
,m_pGundamEFModel(NULL)
,m_pGundamFateListView(NULL)
,m_pGundamFateListContainer(NULL)
,m_pGundamFateListTopContainer(NULL)
,m_pPartnerAddAttack(NULL)
,m_pPartnerAddDefence(NULL)
,m_pPartnerAddHp(NULL)
,m_pMenuAvatar(NULL)
{
    D_RESET_C_ARRAY(m_pEquipBox,EQUIP_BOX_COUNT);
    D_RESET_C_ARRAY(m_pEquipNone,EQUIP_BOX_COUNT);
    D_RESET_C_ARRAY(m_pFate,MAX_FATE_COUNT);
    D_RESET_C_ARRAY(m_pFateHighlight,MAX_FATE_COUNT);
    D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
    D_RESET_C_ARRAY(m_pPartner, PARTNER_NUM);
    D_RESET_C_ARRAY(m_pPartnerCells, PARTNER_NUM);
}

FormationView::~FormationView()
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
	CC_SAFE_RELEASE(m_pQuickEquip);
	CC_SAFE_RELEASE(m_pChangeHero);
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

	CC_SAFE_RELEASE(m_pCloseButton);
    CC_SAFE_RELEASE(m_pPartnerNode);
    CC_SAFE_RELEASE(m_pFormationNode);
    D_SAFE_RELEASE_C_ARRAY(m_pPartner, PARTNER_NUM);
    D_SAFE_RELEASE_C_ARRAY(m_pPartnerCells, PARTNER_NUM);
    CC_SAFE_RELEASE(m_pWHeadData);
	CC_SAFE_RELEASE(m_pGundamFateListView);
	CC_SAFE_RELEASE(m_pGundamFateListContainer);
    CC_SAFE_RELEASE(m_pGundamFateListTopContainer);
	CC_SAFE_RELEASE(m_pPartnerAddAttack);
	CC_SAFE_RELEASE(m_pPartnerAddDefence);
	CC_SAFE_RELEASE(m_pPartnerAddHp);
	CC_SAFE_RELEASE(m_pMenuAvatar);
}

bool FormationView::init()
{
    if(!DLayer::init())
    {
        return false;
    }

	//DM_GET_GUNDAM_EF_MODEL->requestGundamData();

    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    m_pFormationModel = DM_GET_FORMATION_MODEL;
    m_pEquipModel = DM_GET_EQUIP_MODEL;
	m_pGundamEFModel = DM_GET_GUNDAM_EF_MODEL;
    
    CC_SAFE_RELEASE(m_pWarriorInfoViews);
    m_pWarriorInfoViews = CCArray::create();
    CC_SAFE_RETAIN(m_pWarriorInfoViews);
    
    removeAllChildren();
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_zhenrong/PL_zhujiemian");
    
    CC_SAFE_RELEASE(m_pWarriorData);
    m_pWarriorData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_xx_yx_cell");
    CC_SAFE_RETAIN(m_pWarriorData);
    
	CC_SAFE_RELEASE(m_pWHeadData);
    m_pWHeadData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_xx_yx_icon");
    CC_SAFE_RETAIN(m_pWHeadData);
    
	//弹出框关闭按钮
	m_pCloseButton->setVisible(false);

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
	
    m_pFormationListView = FormationListView::create();
    m_pFormationListView->setDelegate(this);
    m_pWarriorListContainer->addChild(m_pFormationListView);
    
    
    m_pSkillBox->getTouchMenu()->setTarget(this, menu_selector(FormationView::touchSkillCallBack));
    m_pSkillBoxSP->getTouchMenu()->setTarget(this, menu_selector(FormationView::touchSkillCallBack));
    
    
    for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
    {
        m_pEquipBox[i]->getTouchMenu()->setTarget(this, menu_selector(FormationView::touchEquipCallBack));
        m_pEquipBox[i]->getTouchMenu()->setTag(i);
		m_pEquipBox[i]->getEquipOnMenu()->setTarget(this, menu_selector(FormationView::touchEquipCallBack));
		m_pEquipBox[i]->getEquipOnMenu()->setTag(i);
		m_pEquipBox[i]->setTypeNum(i);
    }

	 /**
     *   小伙伴缘分信息列表
     */

	//float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pGundamFateListContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	//float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pGundamFateListContainer->getContentSize().height+5, -PL_GET_HEIGHT_DIFF+80);
    float height = m_pGundamFateListTopContainer->getPositionY() - m_pGundamFateListContainer->getPositionY();
    m_pGundamFateListView = GundamFateListView::create(CCSizeMake(370, height-(int)(PL_GET_HEIGHT_DIFF / 2)));
    m_pGundamFateListView->setDelegate(this);
	//m_pGundamFateListContainer->setPositionY(posY);
    m_pGundamFateListContainer->addChild(m_pGundamFateListView);
	CC_SAFE_RETAIN(m_pGundamFateListView);
    return true;
}

void FormationView::touchEquipCallBack(cocos2d::CCObject* sender)
{
    if(m_pDelegate == NULL) return;
    if(m_pWarriorInfo==NULL) return;
    CCMenuItem* pItem = static_cast<CCMenuItem*>(sender);
    int tag = pItem->getTag();
    formation::PositionInfo* pPos = m_pFormationModel->getUnlockedPositionInfoByIndex(pageToPositionIdx(m_iCurrentPage));
    
    EquipBox* pEquipBox = m_pEquipBox[tag];
    equip::EquipCurInfo* pEquipInfo = pEquipBox->getEquipInfo();
	if(pEquipInfo)
	{
		m_pDelegate->showEquipInfoView(pEquipBox->getEquipInfo()->getId(),pPos);
	} else 
	{
		uint32_t equipProperty = 0;
		if(pEquipInfo)
		{
            std::string valueTypeString;
			equipProperty = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo,DM_GET_EQUIP_MODEL->getEquipPropType(pEquipInfo, valueTypeString));
		}
		cocos2d::CCArray* pList = CCArray::create();
		m_pEquipModel->getEquipListByTypeIgnoreID(pEquipBox->getEquipType(), 0,*pList);
		
		m_pDelegate->showEquipSelectView(pList,pPos, equipProperty);
	}
	stopHeroSound();
}



void FormationView::touchSkillCallBack(cocos2d::CCObject* sender)
{
    if(m_pDelegate==NULL) return;
    if(m_pWarriorInfo==NULL) return;
    CCMenuItem* pItem = static_cast<CCMenuItem*>(sender);
    int tag = pItem->getTag();
    if(tag==0) return;
    //m_pDelegate->showSkillInfoView(m_pWarriorInfo->warriorId,tag);
    m_pDelegate->showInfoView(kSkillInfoView, m_pWarriorInfo,CCInteger::create(tag));
	stopHeroSound();
}

bool FormationView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
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
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pQuickEquip", CCMenuItem*, this->m_pQuickEquip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pChangeHero", CCMenuItem*, this->m_pChangeHero);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateMenu", CCMenuItem*, this->m_pFateMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuAvatar", CCMenuItemImage*, this->m_pMenuAvatar);
	
	/*
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone1", CCNode *, this->m_pEquipNone[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone2", CCNode *, this->m_pEquipNone[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone3", CCNode *, this->m_pEquipNone[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipNone4", CCNode *, this->m_pEquipNone[3]);
    */
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

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton" ,DButton *, m_pCloseButton);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPartnerNode" ,CCNode *, m_pPartnerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFormationNode" ,CCNode *, m_pFormationNode);
    D_CCB_ASSIGN_ARRAY("m_pPartner", CCNode *, m_pPartner, PARTNER_NUM);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGundamFateListContainer" ,CCNode *, m_pGundamFateListContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGundamFateListTopContainer" ,CCNode *, m_pGundamFateListTopContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPartnerAddAttack", CCLabelBMFont *, this->m_pPartnerAddAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPartnerAddDefence", CCLabelBMFont *, this->m_pPartnerAddDefence);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPartnerAddHp", CCLabelBMFont *, this->m_pPartnerAddHp);
    return false;
}

SEL_MenuHandler FormationView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "touchWarrior", FormationView::touchWarrior);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callbackFormation", FormationView::callbackFormation);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "playVoiceCallback", FormationView::playVoiceCallback);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "partnerCallback", FormationView::partnerCallback);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "avatarCallback", FormationView::avatarCallback);
    return NULL;
}

void FormationView::playVoiceCallback(cocos2d::CCObject *sender)
{
	//m_pHorn->setEnabled(false);
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(m_pWarriorInfo->getWid(), 0, "voice");
	if(NULL == pSound || !pSound->getDuration()) return;
	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat(pSound->getName().c_str()));
	//scheduleOnce(schedule_selector(FormationView::stopHeroSound), );
	runAction(CCSequence::create(CCDelayTime::create(pSound->getDuration()), CCCallFunc::create(this, callfunc_selector(FormationView::stopHeroSound)), NULL));
	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("horn");
	m_pArmature->setPosition(m_pHorn->getPosition());
	m_pHorn->getParent()->getParent()->addChild(m_pArmature, 10);
}
void FormationView::stopHeroSound()
{
	//m_pHorn->setEnabled(true);
	if (m_nSoundID) {
		SoundManager::sharedSoundManager()->stopSFX(m_nSoundID);
		m_nSoundID = 0;
	}
	if(m_pArmature){
		
		m_pArmature->removeFromParent();
		m_pArmature = NULL;
	}
}

void FormationView::callbackFormation(CCObject* sender)
{
	CCMenuItem* item = static_cast<CCMenuItem*>(sender);
	switch(item->getTag())
	{
	case kChangeFormationTag:
		this->m_pDelegate->showChangeFormationView();break;
	case kQuickEquipTag:
		this->quickEquip();break;
	case kChangeHeroTag:
		formation::PositionInfo *pPositionInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(pageToPositionIdx(m_iCurrentPage));
		m_pDelegate->showInfoView(kFormationSelectView, m_pWarriorInfo, pPositionInfo);
		m_pDelegate->setLastPage(kFormationView);
		stopHeroSound();
		break;
	}
	
}

void FormationView::quickEquip()
{
	Json::Value equipIds(Json::arrayValue);
	unsigned int pAddEid = 0;
	equip::EquipCurInfo* pEquipInfo = NULL;
	cocos2d::CCArray* pList = NULL;
	for(uint8_t i=0; i<EQUIP_BOX_COUNT; i++)
	{
		pAddEid = 0;
		pEquipInfo = m_pEquipBox[i]->getEquipInfo();
		pList = CCArray::create();
		m_pEquipModel->getEquipNotEquipedListByType(m_pEquipBox[i]->getEquipType(),*pList);
		CCObject* pobj = NULL;
		equip::EquipCurInfo* pMaxEquip = NULL;
		uint32_t pMaxEquipProp = 0;
		uint32_t pTempProp = 0;
		equip::EquipCurInfo* pTempEquip = NULL;
        std::string valueTypeString;
		CCARRAY_FOREACH(pList, pobj)
		{
			pTempEquip = static_cast<equip::EquipCurInfo*>(pobj);
			pTempProp = DM_GET_EQUIP_MODEL->computeEquipInfo(pTempEquip,DM_GET_EQUIP_MODEL->getEquipPropType(pTempEquip, valueTypeString));
			if(pTempProp > pMaxEquipProp)
			{
				pMaxEquipProp = pTempProp;
				pMaxEquip = pTempEquip;
			}
		}
		uint32_t currEquipProperty = 0;
		if(pEquipInfo)
		{
			currEquipProperty = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipInfo,DM_GET_EQUIP_MODEL->getEquipPropType(pEquipInfo,valueTypeString));
		}
		//如果当前装备的属性值不低于其它同类型装备，不用更换

		if(currEquipProperty >= pMaxEquipProp)
		{
			if(currEquipProperty == 0 && pList->count()>0)
			{
				pAddEid = (static_cast<equip::EquipCurInfo*>(pList->objectAtIndex(0)))->getId();
			}
		}else  //如果当前位置没有装备或者装备属性值低于同类型装备，更换
		{
			if(pMaxEquip) pAddEid = pMaxEquip->getId();
		}
		if((!pEquipInfo && pAddEid) || (pEquipInfo && pAddEid && pAddEid != pEquipInfo->getId()))
		{
		    equipIds.append(pAddEid);
		}
	}
	if(equipIds.size() > 0)
	{
		formation::PositionInfo* pPos = m_pFormationModel->getUnlockedPositionInfoByIndex(pageToPositionIdx(m_iCurrentPage));
		HTTP_CLIENT->quickEquipWarrior(pPos->iId,equipIds);
	} else 
	{
		Tips_Alert(D_LOCAL_STRING("$yijianzhuangbTips").c_str());
	}
}

void FormationView::touchWarrior(CCObject *sender)
{
    this->touchWarriorIcon(m_pWarriorInfo);
}

void FormationView::onExit()
{
    UNBINDING_ALL(m_pFormationModel, this);
    UNBINDING_ALL(m_pEquipModel, this);
	UNBINDING_ALL(m_pGundamEFModel, this);
    m_pWarriorInfo = NULL;
    DLayer::onExit();
}

void FormationView::onEnter()
{
    Game::sharedGame()->setNavigatorVisable(true);
    BINDING_ONLY(m_pFormationModel, this, FormationView::updateFormation, D_EVENT_FORMATION_UPDATE);
    BINDING_ONLY(m_pEquipModel, this, FormationView::updateInfo, D_EVENT_EQUIPEDLIST_UPDATE);
	BINDING_ONLY(m_pGundamEFModel, this, FormationView::updateFormation, D_EVENT_GUNDAM_E_FORMATION_UPDATE);
    updateFormation();
	if(m_pWarriorInfo){
		sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(m_pWarriorInfo->getWid(), 0, "voice");
		if(NULL == pSound || !pSound->getDuration()) {
			m_pHorn->setEnabled(false);
		}
	}
    showPartnerView(false);
    DLayer::onEnter();
}

void FormationView::updateFormation()
{
    showFormations();
    updateView();
	partnerViewUpdate();
}



float FormationView::cellSizeForTable(DPageView *view)
{
    return PAGE_SIZE_WIDTH+PAGE_MARGIN;
}

cocos2d::CCNode* FormationView::cellAtIndex(DPageView *view, unsigned int idx)
{
    return (cocos2d::CCNode*)m_pWarriorInfoViews->objectAtIndex(idx);
}

uint32_t FormationView::numberOfCellsInTableView(DPageView *view)
{
    return m_pWarriorInfoViews->count();
}

#pragma mark - - CCScrollViewDelegate
void FormationView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    int32_t i = m_pPageView->getCurrentPage();
	if(i==m_iCurrentPage) return;

	m_iCurrentPage = i;
	updateView();
	stopHeroSound();
}

#pragma mark - - methods private


int  FormationView::pageToPositionIdx(const int &page)
{
    uint32_t count = m_pFormationModel->getUnlockedPositionsCount();
    int counter = -1;
    for(uint32_t i = 0; i < count; ++i)
    {
        counter++;
        if(counter==page)
            return i;
    }
    return -1;
}

void FormationView::updateView()
{
    updateInfo();
    
    //更新pageview的信息
    m_pPageView->setCurrentPage(m_iCurrentPage);
    
    //更新弟子列表信息
    m_pFormationListView->updateView();

	//更新小伙伴信息
	m_pGundamFateListView->updateView();
}

void FormationView::updateInfo()
{
    //更新阵容位置上的装备 技能 弟子属性的等信息
    formation::PositionInfo *pPositionInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(pageToPositionIdx(m_iCurrentPage));
    if(!pPositionInfo) return;
    
    m_pWarriorInfo = m_pWarriorModel->getWarriorCurInfoByWID(pPositionInfo->iWarriorId);
    
    int nTotalPrice = DM_GET_FORMATION_MODEL->getFormationTotalForce();
    m_pTotalPrice->setString(CCString::createWithFormat("%d",nTotalPrice)->getCString());
    
    if(!m_pWarriorInfo)
    {
        m_pAttack->setString("0");
        m_pDefence->setString("0");
        m_pHp->setString("0");
        m_pSpeed->setString("0");
        //m_pPrice->setString("0");
        m_pSkillBox->reset();
        m_pSkillBoxSP->reset();
        for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
        {
            m_pEquipBox[i]->reset();
            //m_pEquipNone[i]->setVisible(true);
        }
        for (uint8_t i=0;i<MAX_FATE_COUNT;++i)
        {
            m_pFate[i]->setVisible(false);
            m_pFateHighlight[i]->setVisible(false);
        }
		this->m_pFateEnable->setVisible(false);
		this->m_pFateDisable->setVisible(true);
		this->m_pQuickEquip->setEnabled(false);
		this->m_pChangeHero->setEnabled(false);
		m_pLevel->setVisible(false);
		m_pName->setVisible(false);
		m_pPrice->setVisible(false);
		if(m_pWarriorTypeMark) m_pWarriorTypeMark->reset();
		//if(m_pProgressBar) m_pProgressBar->setVisible(false);

		for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
			m_pProgressBar[i]->setVisible( false );
		}

		m_pFateMenu->setEnabled(false);
		m_pWarriorGradeBg->reset();

        //m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(0));
		if(m_pZizhiBg) m_pZizhiBg->setVisible(false);
		m_pHorn->setEnabled(false);
        return;
    }
    m_pUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv())->getCString());
	this->m_pFateEnable->setVisible(true);
	this->m_pQuickEquip->setEnabled(true);
	this->m_pChangeHero->setEnabled(true);
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(m_pWarriorInfo->getWid(), 0, "voice");
	if(NULL == pSound || !pSound->getDuration()) {
		m_pHorn->setEnabled(false);
	} else {
		m_pHorn->setEnabled(true);
	}
	
	m_pLevel->setVisible(true);
	m_pName->setVisible(true);
	m_pPrice->setVisible(true);
	
	m_pFateMenu->setEnabled(true);
    
    m_pAttack->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak))->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence))->getCString());
	m_pHp->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp))->getCString());
	m_pSpeed->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed))->getCString());

	m_pLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getWarriorLv())->getCString());
	if(m_pZizhiBg) m_pZizhiBg->setVisible(true);
	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getBaseInfo()->getQuality())->getCString());
	}
	m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	m_pPrice->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorForce())->getCString());
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
	m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());

	DProgressBar*progressbar = NULL;
	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pProgressBar[i]->setVisible( color == i+1);
		if(color == i+1)progressbar = m_pProgressBar[i];
	}

	if (progressbar) {
		progressbar->setPercent(0.f);
		int nCurGetExp = m_pWarriorInfo->getWarriorExp();
		int nCurExp = DM_GET_WARRIOR_MODEL->getWarriorCurExpByGetExp(nCurGetExp, m_pWarriorInfo->getWarriorLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
		int nUpExp = DM_GET_WARRIOR_MODEL->getWarriorUpExp(m_pWarriorInfo->getNextLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
		float percent = (float)nCurExp/nUpExp;
		percent = percent > 1.0 ? 1.0 : percent;
		progressbar->setPercent(percent,0.2f);
		progressbar->setVisible(true);
	}
    
    //m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(m_pWarriorInfo->getWarriorUpLv()));
    
    skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(m_pWarriorInfo->getBaseInfo()->getSkillId());
    m_pSkillBox->updateView(pSkill,m_pWarriorInfo);
    m_pSkillBox->getTouchMenu()->setTag(m_pWarriorInfo->getBaseInfo()->getSkillId());
    
    pSkill = DM_GET_SKILL_MODEL->getSkill(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    m_pSkillBoxSP->updateView(pSkill,DM_GET_SKILL_MODEL->getSpecialSkillLv(pSkill));
    m_pSkillBoxSP->getTouchMenu()->setTag(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    
    //m_pPrice->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorForce())->getCString());
    
    for (uint8_t i =0; i<EQUIP_BOX_COUNT; ++i)
    {
		equip::EquipType pEquipType = (equip::EquipType)(i+1);
        equip::EquipCurInfo* pEquip = m_pEquipModel->getEquipedEquipByIdAndType(pPositionInfo->iId, pEquipType);
		
        m_pEquipBox[i]->setEquipInfo(pEquip, m_pWarriorInfo->getWid(),pEquipType);
        //m_pEquipNone[i]->setVisible(pEquip==NULL);
    }
    
	
    warrior::FateInfo *pFateInfo = NULL;
    for (uint8_t i=0;i<MAX_FATE_COUNT;++i)
    {
        if(i<m_pWarriorInfo->getBaseInfo()->getFateArray()->count())
        {
            pFateInfo = static_cast<warrior::FateInfo *>(m_pWarriorInfo->getBaseInfo()->getFateArray()->objectAtIndex(i));
            m_pFate[i]->setString(pFateInfo->name.c_str());
            //m_pFateHighlight[i]->setString(pFateInfo->name.c_str());
			m_pFate[i]->setVisible(true);
			if(DM_GET_WARRIOR_MODEL->checkFateActivated(pFateInfo))
			{
				m_pFateHighlight[i]->setVisible(true);
				m_pFate[i]->setColor(ccc3(73, 30, 12));
			} else 
			{
				m_pFateHighlight[i]->setVisible(false);
				m_pFate[i]->setColor(ccWHITE);
			}
        }else
        {
            m_pFate[i]->setVisible(false);
            m_pFateHighlight[i]->setVisible(false);
        }
    }
}

void FormationView::showFormations()
{
    uint32_t count = m_pFormationModel->getUnlockedPositionsCount();
    for(uint32_t i = 0; i < count; ++i)
    {
        formation::PositionInfo* pPos = m_pFormationModel->getUnlockedPositionInfoByIndex(i);
        
        FormationWarriorInfoView *pFormationWarriorInfoView = NULL;
        if(i<m_pWarriorInfoViews->count())
        {
            pFormationWarriorInfoView = (FormationWarriorInfoView *)m_pWarriorInfoViews->objectAtIndex(i);
        }
        if(!pFormationWarriorInfoView)
        {
            pFormationWarriorInfoView = FormationWarriorInfoView::create(m_pWarriorData,i);
            pFormationWarriorInfoView->setDelegate(this);
			pFormationWarriorInfoView->setPositionInfo(pPos);
            m_pWarriorInfoViews->addObject(pFormationWarriorInfoView);
        }
        if(pFormationWarriorInfoView) pFormationWarriorInfoView->setWarriorInfo(m_pWarriorModel->getWarriorCurInfoByWID(pPos->iWarriorId));
        
    }
    m_pPageView->reloadData();
}

int  FormationView::getPageIndexForWarriorID(const uint32_t &wid)
{
	if(0 == wid) return 0;
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(m_pWarriorInfoViews, pObj)
    {
        FormationWarriorInfoView *pFormationWarriorInfoView = dynamic_cast<FormationWarriorInfoView *>(pObj);
        if(pFormationWarriorInfoView && pFormationWarriorInfoView->getWid()==wid)
        {
            return pFormationWarriorInfoView->getPid();
        }
    }
    return 0;
}

#pragma mark - - methods public
void FormationView::setCurrentPage(uint8_t i)
{
    showPartnerView(false);
	formation::PositionInfo *pPositionInfo = m_pFormationModel->getUnlockedPositionInfoByIndex(pageToPositionIdx(i));
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
	}
}

void FormationView::showPartnerView(bool showFlag)
{
    m_pPartnerNode->setVisible(showFlag);
    m_pFormationNode->setVisible(!showFlag);
    if(showFlag)
    {
        partnerViewUpdate();
    }
}

void FormationView::partnerViewUpdate()
{
	//更新小伙伴信息
    m_pGundamFateListView->updateView();
    m_pGundamEFModel->updateLockStatusByLv(DM_GET_USER_MODEL->getUserInfo()->getUserLv());

    for(int i=0; i<PARTNER_NUM; i++)
    {
        FormationListViewCell* pCell = m_pPartnerCells[i];
        if(!pCell)
        {
            pCell = FormationListViewCell::create(m_pWHeadData);
            pCell->retain();
            m_pPartnerCells[i] = pCell;
        }
        unsigned int unlockLv = m_pGundamEFModel->getUnlockLevel(i+1);
        gundam_engineer_formation::GundamEngineerPosInfo* pPos = m_pGundamEFModel->getPosInfoByIndex(i);
        if(pPos && pPos->isOpened())
        {
            if(pPos && pPos->warriorId > 0){
				pCell->setWarriorInfo(m_pWarriorModel->getWarriorCurInfoByWID(pPos->warriorId));
				pCell->showWarriorName(true);
				pCell->setWarriorBoxState(kWarBoxStHasWar);
			} else {
				pCell->setWarriorInfo(NULL);
				pCell->setWarriorBoxState(kWarBoxStNoWar);
			}
        } else
        {
            pCell->setUnLockInfo(unlockLv);
			pCell->setWarriorBoxState(kWarBoxStLock);
			pCell->setWarriorInfo(NULL);
        }
        m_pPartner[i]->removeAllChildren();
        m_pPartner[i]->addChild(pCell);
        
    }
    uint32_t addAttack,addDefence,addHp;
	addAttack=addDefence=addHp=0;
	m_pGundamEFModel->calcGundamPropertyAdd(&addAttack, &addDefence, &addHp);
	m_pPartnerAddAttack->setString(CCString::createWithFormat("%d", addAttack)->getCString());
	m_pPartnerAddDefence->setString(CCString::createWithFormat("%d", addDefence)->getCString());
	m_pPartnerAddHp->setString(CCString::createWithFormat("%d", addHp)->getCString());
}

void FormationView::avatarCallback(cocos2d::CCObject * obj)
{
	CCLOG("avatarCallback");
	if(m_pDelegate)
	{
		m_pDelegate->showInfoView(kDressView, m_pWarriorInfo);
	}
}

void FormationView::partnerCallback(cocos2d::CCObject * obj)
{
    CCMenuItem* menu = (CCMenuItem*)obj;
    CCLog("menu tag: %d", menu->getTag());
    GundamEngineerPosInfo* pPos = DM_GET_GUNDAM_EF_MODEL->getPosInfoByIndex(menu->getTag());
    if(!pPos || !pPos->isOpened()) return;

	m_pDelegate->setLastPage(kPartnerView);
	formation::PositionInfo* pPosInfo = formation::PositionInfo::create();
	pPosInfo->iWarriorId = pPos->warriorId;
	pPosInfo->iPosId = pPos->posId;
	pPosInfo->iId = pPos->id;
	m_pDelegate->showInfoView(kFormationSelectView, NULL, static_cast<CCObject*>(pPosInfo));
}
void FormationView::touchWarriorIcon(warrior::WarriorCurInfo* pWarrior)
{
    if(m_pDelegate)
    {
		stopHeroSound();
        m_pDelegate->showInfoView(kWarriorInfo, pWarrior);
    }
}
void FormationView::showWarrior(const uint32_t &wid)
{
    m_iCurrentPage = getPageIndexForWarriorID(wid);
	updateView();
}

void FormationView::onCallBack(const Json::Value&,const Json::Value& responeData)
{
	if(responeData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	//reset();
}

#pragma mark --------------------FormationWarriorInfoView--------------------

FormationWarriorInfoView::FormationWarriorInfoView()
:
m_pGrade(NULL)
,m_pExp(NULL)
,m_pExpBar(NULL)
,m_pWarriorTypeMark(NULL)
,m_pContainer(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorButton(NULL)
,m_pDefault(NULL)
,m_pWarrior(NULL)
,m_pBgContainer(NULL)
,m_pTouchToChange(NULL)
,m_pDelegate(NULL)
{
}
FormationWarriorInfoView::~FormationWarriorInfoView()
{
	UNBINDING_ALL(DM_GET_EQUIP_MODEL, this);
    removeAllChildren();
    
    CC_SAFE_RELEASE(m_pGrade);
    
    CC_SAFE_RELEASE(m_pDefault);
    CC_SAFE_RELEASE(m_pWarrior);
    
    CC_SAFE_RELEASE(m_pExp);
    CC_SAFE_RELEASE(m_pExpBar);
    
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pContainer);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	
    CC_SAFE_RELEASE(m_pWarriorButton);
	CC_SAFE_RELEASE(m_pBgContainer);
	
	CC_SAFE_RELEASE(m_pTouchToChange);
}
FormationWarriorInfoView * FormationWarriorInfoView::create(cocos2d::extension::CCData* pData,uint32_t pid)
{
    FormationWarriorInfoView *pFormationWarriorInfoView = new FormationWarriorInfoView();
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
bool FormationWarriorInfoView::init(cocos2d::extension::CCData* pData,uint32_t pid)
{
    m_nPid = pid;
    CCB_READER_DATA_AND_ADDCHILD(pData);
    updateView();
    return true;
}

bool FormationWarriorInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior", CCNode *, this->m_pWarrior);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgContainer", CCNode *, this->m_pBgContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchToChange", CCSprite*, this->m_pTouchToChange);
	/*
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefault", CCNode *, this->m_pDefault);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelBMFont *, this->m_pExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpBar", DProgressBar *, this->m_pExpBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorButton", CCMenuItem *, this->m_pWarriorButton);
    */
	return false;
}

SEL_MenuHandler FormationWarriorInfoView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "touchWarrior", FormationWarriorInfoView::touchWarrior);
    return NULL;
}


#pragma mark - - methods public
void FormationWarriorInfoView::touchWarrior(CCObject *obj)
{
    if(m_pDelegate && m_pWarriorInfo)
    {
        m_pDelegate->touchWarriorIcon(m_pWarriorInfo);
    }
    if(!m_pWarriorInfo)
    {
        //Game::sharedGame()->NavigateTo(navigation::kNavChangeFormation);
		m_pDelegate->getDelegate()->showInfoView(kFormationSelectView, m_pWarriorInfo, static_cast<CCObject*>(m_pPositionInfo));
		m_pDelegate->getDelegate()->setLastPage(kFormationView);
    }
}

#pragma mark - - methods public
void FormationWarriorInfoView::updateView()
{
	//unsigned short warriorBgColor = 0;
    if(m_pWarriorInfo)
    {
        
        m_pContainer->removeAllChildren();
        //m_pContainer->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(m_pWarriorInfo->getAvatarId()));
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorInfo->getAvatarId()));
        
        /*
        //弟子经验
        int nCurGetExp = m_pWarriorInfo->getWarriorExp();
        int nCurExp = DM_GET_WARRIOR_MODEL->getWarriorCurExpByGetExp(nCurGetExp, m_pWarriorInfo->getWarriorLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
        int nUpExp = DM_GET_WARRIOR_MODEL->getWarriorUpExp(m_pWarriorInfo->getNextLv(), m_pWarriorInfo->getBaseInfo()->getRealGrade());
        m_pExp->setString(CCString::createWithFormat("%d/%d",nCurExp,nUpExp)->getCString());
        m_pExpBar->setPercent((float)nCurExp/nUpExp);
        
        m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pWarriorInfo->getWid()));
        m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
        m_pDefault->setVisible(false);
		*/

		//m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pWarriorInfo->getWid()));
		//m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
		//m_pDefault->setVisible(false);
        m_pWarrior->setVisible(true);
		//warriorBgColor = DM_GET_WARRIOR_MODEL->getWarriorGradeColorByWid(m_pWarriorInfo->getWid());
		
		m_pTouchToChange->setVisible(false);
    }else
    {
        //m_pDefault->setVisible(true);
        m_pWarrior->setVisible(false);
		
		m_pTouchToChange->setVisible(true);
    }
    /*CCSprite* warriorGradeBg = CCSprite::create(CCString::createWithFormat("PL_ui/all/all_bg_grade%d.png", warriorBgColor)->getCString());
	warriorGradeBg->setPosition(m_pBgContainer->getPosition());
	this->addChild(warriorGradeBg, -1);*/
	
}
void FormationWarriorInfoView::setWarriorInfo(warrior::WarriorCurInfo *pWarriorCurInfo)
{
    if(pWarriorCurInfo==m_pWarriorInfo) return;
    CC_SAFE_RETAIN(pWarriorCurInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo,this);
    m_pWarriorInfo = pWarriorCurInfo;
    BINDING_ONLY(m_pWarriorInfo, this, FormationWarriorInfoView::updateView, D_EVENT_WARRIORINFO_UPDATE);
	BINDING_ONLY(DM_GET_EQUIP_MODEL, this, FormationWarriorInfoView::updateView, D_EVENT_EQUIPEDLIST_UPDATE);
    updateView();
}
uint32_t FormationWarriorInfoView::getWid()
{
    return m_pWarriorInfo?m_pWarriorInfo->getWid():0;
}
uint32_t FormationWarriorInfoView::getPid()
{
    return m_nPid;
}

void FormationWarriorInfoView::setDelegate(FormationView *pDelegate)
{
    m_pDelegate = pDelegate;
}


//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - FormationListView
#pragma mark - - init etc.
FormationListView::FormationListView()
:m_pView(NULL)
,m_pDelegate(NULL)
,m_pFormationModel(NULL)
,m_pWarriorModel(NULL)
,m_pCellData(NULL)
{
    
}

FormationListView::~FormationListView()
{
    CC_SAFE_RELEASE(m_pCellData);
}


bool FormationListView::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
        
        m_pFormationModel = DM_GET_FORMATION_MODEL;
        m_pWarriorModel = DM_GET_WARRIOR_MODEL;
        
        m_pCellData = readCCData("PL_ui/ccb/pl_zhenrong/PL_zhenrong_xx_yx_icon");
        CC_SAFE_RETAIN(m_pCellData);
		m_pView = DTableView::create(this, CCSizeMake(527, 150));
		m_pView->setDirection(kCCScrollViewDirectionHorizontal);
		m_pView->setDelegate(this);
		addChild(m_pView);
        
        m_pView->reloadData();
        
		bRet = true;
	}while(0);
    
	return bRet;
}

#pragma mark - - CCTableViewDelegate
void FormationListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
	unsigned int unlockCout = m_pFormationModel->getUnlockedPositionsCount() ;
	bool isAllUnlock = ( unlockCout == FORMATION_POS_MAX ) ;
	uint32_t idx = cell->getIdx();
    bool bOpen = DM_GET_GUNDAM_EF_MODEL->isGundamOpen(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
    unsigned int parnterIconNum = bOpen ? PARTNER_ICON_NUM : 0;
	if((isAllUnlock && idx == unlockCout) || (!isAllUnlock && idx == unlockCout+parnterIconNum))
	{
		m_pDelegate->showPartnerView(true);
		return ;
	} else if(idx==unlockCout)
	{
		return ;
	}
	if ( m_pDelegate ) 
	{
		m_pDelegate->setCurrentPage( cell->getIdx() ) ;
	}
}

CCSize FormationListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(95, 130);
}

CCTableViewCell* FormationListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
    
    FormationListViewCell *pCell = static_cast<FormationListViewCell*>(table->dequeueCell());
	if (!pCell)
	{
		pCell = FormationListViewCell::create(m_pCellData);
	}
    pCell->setHighlight(m_pDelegate && m_pDelegate->getCurrentPage() == idx);
	formation::PositionInfo* pPos = m_pFormationModel->getPositionInfoByIndex(idx);
    //英雄更新之后，再更新对应的信息
    /*CCLOG("formation count:%d, %d", idx, m_pFormationModel->getUnlockedPositionsCount()+PARTNER_ICON_NUM);
    if(idx == m_pFormationModel->getUnlockedPositionsCount())
    {
        CCLOG("formation in:%d, %d", idx, m_pFormationModel->getUnlockedPositionsCount()+PARTNER_ICON_NUM);
    }*/
	unsigned int unlockCout = m_pFormationModel->getUnlockedPositionsCount() ;
	bool isAllUnlock = ( unlockCout == FORMATION_POS_MAX ) ;
    bool bOpen = DM_GET_GUNDAM_EF_MODEL->isGundamOpen(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
    unsigned int parnterIconNum = bOpen ? PARTNER_ICON_NUM : 0;
	if((isAllUnlock && idx == unlockCout) || (!isAllUnlock && idx == unlockCout+parnterIconNum))
	{
		pCell->setWarriorInfo(NULL);
		pCell->showPartner(true);
	} else 
	{
		if(idx==unlockCout && pPos)
		{
			pCell->setUnLockInfo(m_pFormationModel->getUnlockLevel(pPos->iPosId));
			pCell->setWarriorBoxState(kWarBoxStLock);
			pCell->setWarriorInfo(NULL);
		}else
		{
			if(pPos && pPos->iWarriorId > 0){
				pCell->setWarriorInfo(m_pWarriorModel->getWarriorCurInfoByWID(pPos->iWarriorId));
				pCell->setWarriorBoxState(kWarBoxStHasWar);
			} else {
				pCell->setWarriorInfo(NULL);
				pCell->setWarriorBoxState(kWarBoxStNoWar);
			}
		}
		pCell->showPartner(false);
	}
    
    //pCell->setDelegate(m_pDelegate);
    return pCell;
}

uint32_t FormationListView::numberOfCellsInTableView(CCTableView *table)
{
    bool bOpen = DM_GET_GUNDAM_EF_MODEL->isGundamOpen(DM_GET_USER_MODEL->getUserInfo()->getUserLv());
    unsigned int parnterIconNum = bOpen ? PARTNER_ICON_NUM : 0;
    if(m_pFormationModel->getUnlockedPositionsCount()==FORMATION_POS_MAX) return FORMATION_POS_MAX+parnterIconNum;
    return m_pFormationModel->getUnlockedPositionsCount()+1+parnterIconNum; //unlock
}


#pragma mark - - methods private
void FormationListView::updateView()
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


#pragma mark - FormationListViewCell
#pragma mark - - init etc.
FormationListViewCell::FormationListViewCell()
:m_pWarriorCurInfo(NULL)
,m_pWarrior(NULL)
,m_pUnlock(NULL)
,m_pUnlockLevel(NULL)
,m_pHighLight(NULL)
,m_pSoundItem(NULL)
,m_pWarriorHeadGradeBg(NULL)
,m_pParterIcon(NULL)
,m_pPartnerBtn(NULL)
,m_pWarriorNameNode(NULL)
,m_pWarriorName(NULL)
{
    
}

FormationListViewCell::~FormationListViewCell()
{
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorCurInfo,this);
    CC_SAFE_RELEASE(m_pWarrior);
    CC_SAFE_RELEASE(m_pUnlock);
    CC_SAFE_RELEASE(m_pUnlockLevel);
    CC_SAFE_RELEASE(m_pHighLight);
    CC_SAFE_RELEASE(m_pSoundItem);
	CC_SAFE_RELEASE(m_pWarriorHeadGradeBg);
    CC_SAFE_RELEASE(m_pParterIcon);
    CC_SAFE_RELEASE(m_pPartnerBtn);
    CC_SAFE_RELEASE(m_pWarriorNameNode);
    CC_SAFE_RELEASE(m_pWarriorName);
}

FormationListViewCell* FormationListViewCell::create(cocos2d::extension::CCData *pData)
{
    FormationListViewCell* pRet = new FormationListViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool FormationListViewCell::init(cocos2d::extension::CCData *pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
    setHighlight(false);
	m_pWarrior->setVisible(true);
	showPartner(false);
	m_pWarriorNameNode->setVisible(false);
    //updateView();
    return true;
}

bool FormationListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior", WarriorHeadSmall*, this->m_pWarrior);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorHeadGradeBg", WarriorHeadGradeBg*, this->m_pWarriorHeadGradeBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlock", CCNode*, this->m_pUnlock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUnlockLevel", CCLabelBMFont*, this->m_pUnlockLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLight", CCSprite*, this->m_pHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoundItem", CCMenuItem*, this->m_pSoundItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pParterIcon", CCNode*, this->m_pParterIcon);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPartnerBtn", CCMenuItem*, this->m_pPartnerBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorNameNode", CCNode*, this->m_pWarriorNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorName", CCLabelTTF*, this->m_pWarriorName);
    return false;
}

void FormationListViewCell::setWarriorBoxState(WarriorBoxState st)
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
void FormationListViewCell::updateView()
{
    //setLock(false);
    m_pWarrior->setWarriorInfo(m_pWarriorCurInfo);
	if(m_pWarriorCurInfo){
		m_pWarriorHeadGradeBg->setWarriorGrade(m_pWarriorCurInfo->getBaseInfo()->getColorGrade());
	} else {
		m_pWarriorHeadGradeBg->setWarriorGrade(warrior::WarriorGrade(0));
	}
}

void FormationListViewCell::setUnLockInfo(uint32_t level)
{
    m_pUnlockLevel->setString(CCString::createWithFormat("%d",level)->getCString());
}

void FormationListViewCell::setLock(bool val)
{
    m_pUnlock->setVisible(val);
    m_pSoundItem->setVisible(!m_pHighLight->isVisible() && !m_pUnlock->isVisible());
}

void FormationListViewCell::showPartner(bool showFlag)
{
	if(showFlag){
        setLock(false);
    }
    m_pParterIcon->setVisible(showFlag);
    
    //m_pPartnerBtn->setEnabled(false);
    //if(showFlag)
    //   m_pPartnerBtn->setTarget(this, menu_selector(FormationListViewCell::partnerCallback));
}

void FormationListViewCell::showWarriorName(bool showFlag)
{
	if(showFlag && m_pWarriorCurInfo){
		m_pWarriorNameNode->setVisible(true);
		m_pWarriorName->setString(m_pWarriorCurInfo->getBaseInfo()->getWarriorName().c_str());
	}
}

void FormationListViewCell::partnerCallback(CCObject *obj)
{
    m_pDelegate->showPartnerView(true);
}

#pragma mark - - methods private
void FormationListViewCell::setHighlight(bool val)
{
    m_pHighLight->setVisible(val);
    m_pSoundItem->setVisible(!m_pHighLight->isVisible() && !m_pUnlock->isVisible());
}

void FormationListViewCell::setWarriorInfo(warrior::WarriorCurInfo* pWarriorCurInfo)
{
    //m_pParterIcon->setVisible(false);
	//if(NULL == m_pWarriorCurInfo) m_pWarrior->getNotInBattle()->setVisible(true);
    if(pWarriorCurInfo == m_pWarriorCurInfo) return;
    CC_SAFE_RETAIN(pWarriorCurInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorCurInfo,this);
    m_pWarriorCurInfo = pWarriorCurInfo;
    BINDING_EXEC(m_pWarriorCurInfo, this, FormationListViewCell::updateView, D_EVENT_WARRIORINFO_UPDATE);
}




	
//////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - GundamFateListView
GundamFateListView::GundamFateListView()
:m_pView(NULL)
,m_pDelegate(NULL)
,m_pFormationModel(NULL)
,m_pWarriorModel(NULL)
,m_pCellData(NULL)
,m_pBattleWarriors(NULL)
,m_pEquipModel(NULL)
{
    
}

GundamFateListView::~GundamFateListView()
{
    CC_SAFE_RELEASE(m_pCellData);
	CC_SAFE_RELEASE(m_pBattleWarriors);
}

GundamFateListView* GundamFateListView::create(const CCSize& size)
{
	GundamFateListView* pRet = new GundamFateListView();
	if(pRet && pRet->init(size))
	{
		pRet->autorelease();
		return pRet;
	} else 
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
		return NULL;
	}
}

bool GundamFateListView::init(const CCSize& size)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !DLayer::init());
        
        m_pFormationModel = DM_GET_FORMATION_MODEL;
        m_pWarriorModel = DM_GET_WARRIOR_MODEL;
		m_pEquipModel = DM_GET_EQUIP_MODEL;
        
        m_pCellData = readCCData("PL_ui/ccb/pl_zhenrong/PL_yuanfen_info");
        CC_SAFE_RETAIN(m_pCellData);

		CC_SAFE_RELEASE(m_pBattleWarriors);
		m_pBattleWarriors = CCArray::create();
		CC_SAFE_RETAIN(m_pBattleWarriors);
		m_pFormationModel->getBattleWarriors(m_pBattleWarriors);

		m_pView = DTableView::create(this, size);
		m_pView->setDirection(kCCScrollViewDirectionVertical);
		m_pView->setVerticalFillOrder(kCCTableViewFillTopDown);
		m_pView->setDelegate(this);
		addChild(m_pView);
        
		bRet = true;
	}while(0);
    
	return bRet;
}

#pragma mark - - CCTableViewDelegate
void GundamFateListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
   
}

CCSize GundamFateListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(370, 400);
}

CCTableViewCell* GundamFateListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
    GundamFateListViewCell *pCell = static_cast<GundamFateListViewCell*>(table->dequeueCell());
	if (!pCell)
	{
		pCell = GundamFateListViewCell::create(m_pCellData);
	}
    CCInteger* warriorId = static_cast<CCInteger*>(m_pBattleWarriors->objectAtIndex(idx));
    warrior::WarriorCurInfo* warriorCurrInfo = m_pWarriorModel->getWarriorCurInfoByWID(warriorId->getValue());
	pCell->setWarriorInfo(warriorCurrInfo);
	
    return pCell;
}

uint32_t GundamFateListView::numberOfCellsInTableView(CCTableView *table)
{
    return m_pBattleWarriors->count(); //unlock
}


void GundamFateListView::onExit()
{
	//UNBINDING_ALL(m_pFormationModel, this);
	UNBINDING_ALL(m_pEquipModel, this);
	DLayer::onExit();
}

void GundamFateListView::onEnter()
{
	//BINDING_ONLY(m_pFormationModel, this, GundamFateListView::updateView, D_EVENT_FORMATION_UPDATE);
	BINDING_EXEC(m_pEquipModel, this, GundamFateListView::updateView, D_EVENT_EQUIPEDLIST_UPDATE);
	DLayer::onEnter();
}

#pragma mark - - methods private
void GundamFateListView::updateView()
{
	CC_SAFE_RELEASE(m_pBattleWarriors);
	m_pBattleWarriors = CCArray::create();
	CC_SAFE_RETAIN(m_pBattleWarriors);
	m_pFormationModel->getBattleWarriors(m_pBattleWarriors);
    m_pView->reloadData();
}


#pragma mark - GundamFateListViewCell
GundamFateListViewCell::GundamFateListViewCell()
:m_pWarriorCurInfo(NULL)
,m_pFateInfo(NULL)
,m_pWarrirorName(NULL)
,m_pWarriorModel(NULL)
{
    
}

GundamFateListViewCell::~GundamFateListViewCell()
{
    //SAFE_RELEASE_UNBINDING_ALL(m_pWarriorCurInfo,this);
	CC_SAFE_RELEASE(m_pWarriorCurInfo);
	CC_SAFE_RELEASE(m_pFateInfo);
	CC_SAFE_RELEASE(m_pWarrirorName);

}

GundamFateListViewCell* GundamFateListViewCell::create(cocos2d::extension::CCData *pData)
{
    GundamFateListViewCell* pRet = new GundamFateListViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool GundamFateListViewCell::init(cocos2d::extension::CCData *pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);

	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    return true;
}

bool GundamFateListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateInfo", CCRichLabelTTF*, this->m_pFateInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrirorName", CCRichLabelTTF*, this->m_pWarrirorName);

    
    return false;
}

#pragma mark - - methods private
void GundamFateListViewCell::updateView()
{
    m_pWarrirorName->setString(m_pWarriorCurInfo->getBaseInfo()->getWarriorName().c_str());
	ccColor3B color;
	switch (m_pWarriorCurInfo->getBaseInfo()->getColorGrade())
	{
	case warrior::kWarGradeGreen:   
		color = ccc3(175, 255, 1);
		break;
	case warrior::kWarGradeBlue:  
		color = ccc3(1, 183, 255);
		break;
	case warrior::kWarGradePurple:  
		color = ccc3(255, 1, 211);
		break;
	case warrior::kWarGradeRed:	
		color = ccc3(255, 180, 1);
		break;
	}
	m_pWarrirorName->setColor(color);
	updateFateContent();
}

void GundamFateListViewCell::setWarriorInfo(warrior::WarriorCurInfo* pWarriorCurInfo)
{
    //if(pWarriorCurInfo == m_pWarriorCurInfo) return;
    CC_SAFE_RETAIN(pWarriorCurInfo);
    //SAFE_RELEASE_UNBINDING_ALL(m_pWarriorCurInfo,this);
	CC_SAFE_RELEASE(m_pWarriorCurInfo);
    m_pWarriorCurInfo = pWarriorCurInfo;
	updateView();
    //BINDING_EXEC(m_pWarriorCurInfo, this, GundamFateListViewCell::updateView, D_EVENT_WARRIORINFO_UPDATE);
}


static const char* color_yellow = "fff113";
static const char* color_red = "ff3f3f";
static const char* color_gray = "cfcfcf";
static const char* color_green = "7cee21";

void GundamFateListViewCell::updateFateContent()
{
	unsigned int fateStringLines = 0;
	warrior::WarriorCurInfo* m_pWarrior = m_pWarriorCurInfo;
	cocos2d::CCArray *		  pFateArray = m_pWarriorCurInfo->getBaseInfo()->getFateArray();

	if (pFateArray == NULL || m_pWarrior == NULL)
	{
		return;
	}

	std::string fateString;
	//cell->setFateInfo((warrior::FateInfo *)m_pFateArray->objectAtIndex(idx),m_pWarrior);
	for (uint32_t i = 0; i < pFateArray->count(); i++)
	{
		warrior::FateInfo *pFateInfo = (warrior::FateInfo *)pFateArray->objectAtIndex(i);
		if (pFateInfo == NULL)
		{
			continue;
		}

		bool isActivated = m_pWarriorModel->checkFateActivated(pFateInfo);
		CCInteger *member_id = NULL;
		CCObject *obj = NULL;
		const char *warrior_name = NULL;
		uint32_t addNum = 0;
		std::vector<std::string> warriorNames;
		std::string  desc;
		std::string fateName(pFateInfo->name.c_str());
		std::string fateDesc;

		if (pFateInfo->type == warrior::kFateTypeWarrior)
		{
			warrior_name = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pFateInfo->owner_id)->getWarriorName().c_str();
			warriorNames.push_back(CCString::createWithFormat("%s",warrior_name)->getCString());

			CCARRAY_FOREACH(pFateInfo->member_ids, obj)
			{
				member_id = (CCInteger*)obj;
				warrior_name = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(member_id->getValue())->getWarriorName().c_str();
				warriorNames.push_back(CCString::createWithFormat("%s",warrior_name)->getCString());
			}

			fateDesc.append(CCString::create(D_LOCAL_STRING("FateDesc1%s", join(warriorNames,", ").c_str()).c_str())->getCString());
			fateDesc.append(",");
			//attack
			if(pFateInfo->add_attack>0)
			{
				fateDesc.append(D_LOCAL_STRING("Attack"));
				addNum = pFateInfo->add_attack;
				fateDesc.append(CCString::createWithFormat("+%d%%,",addNum)->getCString());
			}			

			//defence
			if(pFateInfo->add_defence>0)
			{
				fateDesc.append(D_LOCAL_STRING("Defence"));
				addNum = pFateInfo->add_defence;
				fateDesc.append(CCString::createWithFormat("+%d%%,",addNum)->getCString());
			}

			//hp
			if(pFateInfo->add_hp>0)
			{
				fateDesc.append(D_LOCAL_STRING("HP"));
				addNum = pFateInfo->add_hp;
				fateDesc.append(CCString::createWithFormat("+%d%%",addNum)->getCString());
			}
		}else
		{
			equip::EquipBaseInfo* equipBaseInfo = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pFateInfo->equip_id);
			if (equipBaseInfo == NULL) {
				CCLOG("Error equip id: %d", pFateInfo->equip_id);
				continue;
			}
			const char *equip_name = equipBaseInfo->getEquipName().c_str();
			fateDesc.append(D_LOCAL_STRING("FateDesc2%s",equip_name));
			fateDesc.append(",");

			//attack
			if(pFateInfo->add_attack>0)
			{
				fateDesc.append(D_LOCAL_STRING("Attack"));
				addNum = pFateInfo->add_attack;
				fateDesc.append(CCString::createWithFormat("+%d%%,",addNum)->getCString());
			}			

			//defence
			if(pFateInfo->add_defence>0)
			{
				fateDesc.append(D_LOCAL_STRING("Defence"));
				addNum = pFateInfo->add_defence;
				fateDesc.append(CCString::createWithFormat("+%d%%,",addNum)->getCString());
			}

			//hp
			if(pFateInfo->add_hp>0)
			{
				fateDesc.append(D_LOCAL_STRING("HP"));
				addNum = pFateInfo->add_hp;
				fateDesc.append(CCString::createWithFormat("+%d%%",addNum)->getCString());
			}
		}
		

		std::string singleFateString;
		unsigned int stringLength;
		if (isActivated)
		{
			singleFateString.assign(CCString::createWithFormat("[color=ff%s]%s: [/color][color=ff%s]%s[/color]\n",
				color_green, fateName.c_str(),
				color_yellow, 
				fateDesc.c_str()
				)->getCString());
			//subtract color symblo
			stringLength = getUtf8Length(singleFateString.c_str()) - (32 + 16);
		}
		else
		{
			singleFateString.assign(CCString::createWithFormat("[color=ff%s]%s: %s[/color]\n", color_gray, fateName.c_str(), fateDesc.c_str())->getCString());
			//subtract color symblo
			stringLength = getUtf8Length(singleFateString.c_str()) - (16 + 8);
		}

		fateString.append(singleFateString);
		fateStringLines++;

		if (stringLength > m_pFateInfo->getContentSize().width / m_pFateInfo->getFontSize() + 3)
		{
			fateStringLines++;
		}
	}

	
	m_pFateInfo->setString(fateString.c_str());

	/*if (fateStringLines > 0)
	{
	int fontSize = m_pFateInfo->getFontSize();
	m_pFateInfo->setDimensions(CCSizeMake(m_pFateInfo->getDimensions().width,  m_pFateListBg->getContentSize().height));
	}*/
}
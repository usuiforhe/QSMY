//  WarriorInfo.cpp
//	弟子详情
//  Created by cll on 13-3-29
//  Copyright (c) 2013, thedream. All rights reserved.
//
bool m_bIsfromCollect_t;
#include "WarriorInfo.h"
#include "Warrior.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../formation/Formation.h"
#include "managers/TipsManager.h"
#include "WarriorFate.h"
#include "utils/StringUtil.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../pve/Pve.h"
#include "managers/SoundManager.h"
//#include "PictureCollectCell.h"
USING_NS_CC;

using namespace  warrior;

WarriorInfoView::WarriorInfoView()
:m_pDelegate(NULL)
,m_pWarriorInfo(NULL)
,m_warriorInfoType(kTypeOperate)
,m_pFateView(NULL)
,m_pWarriorModel(NULL)

//m_pCloseBut(NULL),
,m_pWarriorSprite(NULL)
,m_pUpLevel(NULL)
,m_pScrollView(NULL)
,m_pMarkNode(NULL)
,m_pTalentNode(NULL)
,m_pDesc(NULL)
,m_pFateList(NULL)
,m_pFateLabel(NULL)
,m_pFateListBg(NULL)
,m_pTalentLabel(NULL)
,m_pTalentBg(NULL)
,m_pSkillBox1(NULL)
,m_pSkillBox2(NULL)
,m_pAttack(NULL)
,m_pDefence(NULL)
,m_pSpeed(NULL)
,m_pFight(NULL)
,m_pHp(NULL)
,m_pWarriorGradeBg(NULL)
//,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pOperate(NULL)
,m_pCloseOnly(NULL)
,m_pCloseButton(NULL)
,m_pStrengthenButton(NULL)
,m_pDevelopButton(NULL)
,m_pPromotionButton(NULL)
,m_pWarriorTypeMark(NULL)
,m_pDressUp(NULL)
,m_pDressOut(NULL)
,m_pveDelegate(NULL)
,m_pHorn(NULL)
,m_nSoundID(0)
,m_pArmature(NULL)
,m_pZizhiNum(NULL)
,m_pBottom(NULL)
{
    D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

WarriorInfoView::~WarriorInfoView()
{
    removeAllChildren();
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RELEASE(m_pScrollView);
	CC_SAFE_RELEASE(m_pMarkNode);
	CC_SAFE_RELEASE(m_pTalentNode);
	CC_SAFE_RELEASE(m_pFateView);
	CC_SAFE_RELEASE(m_pFateLabel);
	CC_SAFE_RELEASE(m_pFateListBg);
	CC_SAFE_RELEASE(m_pTalentLabel);
	CC_SAFE_RELEASE(m_pTalentBg);
	CC_SAFE_RELEASE(m_pWarriorSprite);
	CC_SAFE_RELEASE(m_pUpLevel);
	CC_SAFE_RELEASE(m_pDesc);
	CC_SAFE_RELEASE(m_pFateList);
	CC_SAFE_RELEASE(m_pSkillBox1);
	CC_SAFE_RELEASE(m_pSkillBox2);
	CC_SAFE_RELEASE(m_pAttack);
	CC_SAFE_RELEASE(m_pDefence);
	CC_SAFE_RELEASE(m_pSpeed);
	CC_SAFE_RELEASE(m_pFight);
	CC_SAFE_RELEASE(m_pHp);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
	//CC_SAFE_RELEASE(m_pGrade);
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pZizhiNum);
	
    
    CC_SAFE_RELEASE(m_pOperate);
    CC_SAFE_RELEASE(m_pCloseOnly);
    
    CC_SAFE_RELEASE(m_pCloseButton);
    CC_SAFE_RELEASE(m_pStrengthenButton);
    CC_SAFE_RELEASE(m_pDevelopButton);
    CC_SAFE_RELEASE(m_pPromotionButton);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pDressUp);
    CC_SAFE_RELEASE(m_pDressOut);

	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
	CC_SAFE_RELEASE(m_pHorn);
	CC_SAFE_RELEASE(m_pBottom);
}


WarriorInfoView* WarriorInfoView::create(WarriorInfoType type)
{
	WarriorInfoView *pRet = new WarriorInfoView();
	if (pRet && pRet->init(type))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool WarriorInfoView::init()
{
    return init(kTypeCloseOnly);
}


bool WarriorInfoView::init(WarriorInfoType type)
{
    
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
	m_warriorInfoType = type;
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_hero/PL_hero");
    
	/*m_pFateView  = WarriorFateView::create();
	m_pFateView->retain();*/
    
    m_pOperate->setVisible(type == kTypeOperate);
   // m_pCloseOnly->setVisible(type == kTypeCloseOnly);
    
    m_pCloseButton->getButton()->setTarget(this, menu_selector(WarriorInfoView::closeCallBack));
    
    m_pStrengthenButton->getButton()->setTarget(this, menu_selector(WarriorInfoView::menuCallBack));
    m_pDevelopButton->getButton()->setTarget(this, menu_selector(WarriorInfoView::menuCallBack));
    m_pPromotionButton->getButton()->setTarget(this, menu_selector(WarriorInfoView::menuCallBack));
    
    m_pStrengthenButton->getButton()->setTag(kUpgradeBut);
    m_pDevelopButton->getButton()->setTag(kDevelopBut);
    m_pPromotionButton->getButton()->setTag(kPromotionBut);
    
   
    m_pSkillBox1->getTouchMenu()->setTarget(this, menu_selector(WarriorInfoView::touchSkillCallBack));
    m_pSkillBox2->getTouchMenu()->setTarget(this, menu_selector(WarriorInfoView::touchSkillCallBack));
    
    //m_pDesc->setVerticalTextStyle(kCCVerticalTextStyleRightToLeft);
	//m_pDesc->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	//m_pDesc->setHorizontalAlignment(kCCTextAlignmentCenter);

    //m_pDesc->setFontName("FZJZK--GBK1-0");
    //m_pFateLabel->setFontName("FZJZK--GBK1-0");
    
	/*m_pDressUp->setTarget(this, menu_selector(WarriorInfoView::doDressUp));
	m_pDressOut->setTarget(this, menu_selector(WarriorInfoView::doDressOut));*/
	return true;
}

void WarriorInfoView::doDressUp(cocos2d::CCObject *sender)
{
    //avatar::AvatarInfo *pAvatar = DM_GET_AVATAR_MODEL->findAnAvatarForWarrior(m_pWarriorInfo->getWid());
    //DM_GET_WARRIOR_MODEL->dressUp(pAvatar);
}

void WarriorInfoView::doDressOut(cocos2d::CCObject *sender)
{
    m_pWarriorInfo->unDress();
}

void WarriorInfoView::touchSkillCallBack(cocos2d::CCObject* sender)
{
    if(m_pDelegate==NULL) return;
    if(m_pWarriorInfo==NULL) return;

	//没有英雄也可以查看，viewonly模式
	////如果没有此英雄，不能点击
	//if(!DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(m_pWarriorInfo->getWid()) || !m_pWarriorInfo->isSynced) return;

    CCMenuItem* pItem = static_cast<CCMenuItem*>(sender);
    int tag = pItem->getTag();
    if(tag==0) return;
    //m_pDelegate->showSkillInfoView(m_pWarriorInfo->warriorId,tag);
    m_pDelegate->showInfoView(kSkillInfoView, m_pWarriorInfo,CCInteger::create(tag));
}


bool WarriorInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCLOG("%s", pMemberVariableName);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorSprite", CCNode *, this->m_pWarriorSprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pScrollView", CCScrollView*, this->m_pScrollView);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMarkNode", CCNode*, this->m_pMarkNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTalentNode", CCNode*, this->m_pTalentNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", CCLabelBMFont *, this->m_pUpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateList", CCNode *, this->m_pFateList);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateLabel", CCRichLabelTTF *, this->m_pFateLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateListBg", CCScale9Sprite *, this->m_pFateListBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTalentBg", CCScale9Sprite*, this->m_pTalentBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTalentLabel", CCRichLabelTTF*, this->m_pTalentLabel);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox1", SkillBox *, this->m_pSkillBox1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox2", SkillBox *, this->m_pSkillBox2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCLabelBMFont *, this->m_pFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg *, this->m_pWarriorGradeBg);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pOperate", CCNode *, this->m_pOperate);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseOnly", CCNode *, this->m_pCloseOnly);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStrengthen", DButton *, this->m_pStrengthenButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDevelopButton", DButton *, this->m_pDevelopButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPromotionButton", DButton *, this->m_pPromotionButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDressUp", CCMenuItem *, this->m_pDressUp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDressOut", CCMenuItem *, this->m_pDressOut);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar1", DProgressBar *, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar2", DProgressBar *, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar4", DProgressBar *, this->m_pProgressBar[3]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHorn", CCMenuItem *, this->m_pHorn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum" , CCLabelBMFont* ,this->m_pZizhiNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom" , CCNode*, m_pBottom);
    return false;
}


SEL_MenuHandler WarriorInfoView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", WarriorInfoView::closeCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "playVoiceCallback", WarriorInfoView::playVoiceCallback);
	return NULL;
}

void WarriorInfoView::playVoiceCallback(cocos2d::CCObject *sender)
{
	m_pHorn->setEnabled(false);
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(m_pWarriorInfo->getWid(), 0, "voice");
	if(NULL == pSound || !pSound->getDuration()) return;
	m_nSoundID = SoundManager::sharedSoundManager()->playSFX(CCString::createWithFormat(pSound->getName().c_str()));
	scheduleOnce(schedule_selector(WarriorInfoView::stopHeroSound), pSound->getDuration());
	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("horn");
	m_pArmature->setPosition(m_pHorn->getPosition());
	m_pHorn->getParent()->getParent()->addChild(m_pArmature, 10);
}
void WarriorInfoView::stopHeroSound(float delay)
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

void WarriorInfoView::setWarrior(warrior::WarriorBaseInfo * pWarrior)
{
    setWarrior(warrior::WarriorCurInfo::create(pWarrior));
}

void WarriorInfoView::setWarrior(warrior::WarriorCurInfo *pWarrior)
{
	if (pWarrior == m_pWarriorInfo) return;

	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);

	CC_SAFE_RETAIN(pWarrior);
	m_pWarriorInfo = pWarrior;

	binding();
}

void WarriorInfoView::binding()
{
	BINDING_EXEC(m_pWarriorInfo,this,WarriorInfoView::updateWarriorInfo,D_EVENT_WARRIORINFO_UPDATE);
}

void WarriorInfoView::onEnter()
{
	binding();
	runAction(CCSequence::create(CCDelayTime::create(0.2f), CCCallFuncO::create(this, callfuncO_selector(WarriorInfoView::playVoiceCallback), NULL), NULL));
	sound::Sound* pSound = DM_GET_SOUND_MODEL->getUnitSound(m_pWarriorInfo->getWid(), 0, "voice");
	if(NULL == pSound || !pSound->getDuration()) {
		m_pHorn->setEnabled(false);
	}
	DLayer::onEnter();
}


void WarriorInfoView::onExit()
{
	UNBINDING_ALL(m_pWarriorInfo,this);
	stopHeroSound();
	DLayer::onExit();
}

void WarriorInfoView::menuCallBack(CCObject * obj)
{
    if(m_pDelegate==NULL) return;
	CCMenuItemImage * pMenuItem = (CCMenuItemImage*) obj;
	int tag = pMenuItem->getTag();
    
	switch ( tag )
	{
        case kUpgradeBut:
            m_pDelegate->showInfoView(kUpgradeView, m_pWarriorInfo);
            break;
        case kPromotionBut:
            m_pDelegate->showInfoView(kPromotionView, m_pWarriorInfo);
            break;
        case kDevelopBut:
            if(!m_pWarriorInfo->checkDevelopLevel())
            {
                Tips_Alert(D_LOCAL_STRING("WarriorLevel%dCantDev",WARRIOR_DEV_LV_MIN).c_str());
                return;
            }
            m_pDelegate->showInfoView(kDevelopView, m_pWarriorInfo);
            break;
        default:
            break;
	}
    
}


void WarriorInfoView::updateWarriorInfo()
{
	if(m_bIsfromCollect_t==true)
	{
		m_pOperate->setVisible(false);
	}
	
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
	/*m_pFateList->removeAllChildren();
	m_pFateList->addChild(m_pFateView);*/

	updateFateContent();
	updateTalentConten();

//	if (m_pMarkNode && m_pScrollView)
//	{
//        float newContentHeight = m_pMarkNode->getContentSize().height + m_pFateListBg->getContentSize().height + m_pTalentBg->getContentSize().height + 100;
//        m_pScrollView->setContentSize(CCSizeMake(m_pScrollView->getContentSize().width, newContentHeight));
//	}

	if (m_pTalentNode)
	{
		m_pTalentNode->setPositionY(m_pFateListBg->getPositionY() - m_pFateListBg->getContentSize().height -10);
	}

	if(m_pWarriorInfo->getBaseInfo()->getColorGrade() == warrior::kWarGradeGreen)
	{
		//绿色不显示有缘天赋
		if(m_pBottom) m_pBottom->setVisible(false);
	}
	else
	{
		if(m_pBottom) m_pBottom->setVisible(true);
	}

	m_pScrollView->setViewSize(PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(CCSizeMake(640,910), -PL_MAX_HEIGHT_DIFF));
    m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
	
	//m_pFateView->setFateArray(m_pWarriorInfo->getBaseInfo()->getFateArray(),m_pWarriorInfo);
    
	m_pWarriorSprite->removeAllChildren();
	//m_pWarriorSprite->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(m_pWarriorInfo->getAvatarId()));
	m_pWarriorSprite->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon(m_pWarriorInfo->getAvatarId(), 0.8f));
	
	//m_pWarriorSprite->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(m_pWarriorInfo->getAvatarId()));

    m_pUpLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getWarriorUpLv())->getCString());

	if(m_pZizhiNum)
	{
		m_pZizhiNum->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getBaseInfo()->getQuality())->getCString());
	}
	
	//m_pGrade->setColor(m_pWarriorModel->getWarriorColorByGrade(m_pWarriorInfo->getBaseInfo()->getRealGrade()));
	m_pWarriorGradeBg->setWarriorGrade(m_pWarriorInfo->getBaseInfo()->getColorGrade());
	m_pFight->setString(CCString::createWithFormat("%d", m_pWarriorInfo->computeWarriorForce())->getCString());
	m_pLevel->setString(CCString::createWithFormat("%i",m_pWarriorInfo->getWarriorLv())->getCString());
	m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	m_pDesc->setString(m_pWarriorInfo->getBaseInfo()->getWarriorDesc().c_str());
	m_pAttack->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak))->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence))->getCString());
	m_pHp->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp))->getCString());
	m_pSpeed->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed))->getCString());
    
    skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(m_pWarriorInfo->getBaseInfo()->getSkillId());
	m_pSkillBox1->updateView(pSkill,m_pWarriorInfo->getSlv());
    m_pSkillBox1->getTouchMenu()->setTag(m_pWarriorInfo->getBaseInfo()->getSkillId());
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    
    skill::Skill *pSkill2 = DM_GET_SKILL_MODEL->getSkill(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    int8_t sp_skill_level = DM_GET_SKILL_MODEL->getSpecialSkillLv(pSkill2);
    m_pSkillBox2->updateView(pSkill2,sp_skill_level);
    m_pSkillBox2->getTouchMenu()->setTag(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    
    m_pPromotionButton->getButton()->setEnabled(m_pWarriorInfo->isPromoteable() && !m_pWarriorInfo->checkPromoteLimit());
    m_pStrengthenButton->getButton()->setEnabled(!m_pWarriorInfo->checkLevelLimit());
    
    //设置name底框颜色
	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = (int)m_pWarriorInfo->getBaseInfo()->getColorGrade();
		m_pProgressBar[i]->setVisible( color == i+1);
	}
    
    /*m_pDressUp->setVisible(false);
    m_pDressOut->setVisible(false);
    
    if(DM_GET_WARRIOR_MODEL->isMyWarrior(m_pWarriorInfo))
    {
        if(m_pWarriorInfo->getDressId()!=0)
        {
            m_pDressOut->setVisible(true);
        }else
        {
            m_pDressUp->setVisible(true);
            m_pDressUp->setEnabled(DM_GET_AVATAR_MODEL->hasAvatarOfWarrior(m_pWarriorInfo->getWid()));
        }
    }*/
}

static const char* color_yellow = "fff113";
static const char* color_red = "ff3f3f";
static const char* color_gray = "cfcfcf";
static const char* color_green = "7cee21";

void WarriorInfoView::updateFateContent()
{
	unsigned int fateStringLines = 0;
	warrior::WarriorCurInfo* m_pWarrior = m_pWarriorInfo;
	cocos2d::CCArray *		  m_pFateArray = m_pWarriorInfo->getBaseInfo()->getFateArray();

	if (m_pFateArray == NULL || m_pWarrior == NULL)
	{
		return;
	}
	
	std::string fateString;
	//cell->setFateInfo((warrior::FateInfo *)m_pFateArray->objectAtIndex(idx),m_pWarrior);
	for (uint32_t i = 0; i < m_pFateArray->count(); i++)
	{
		warrior::FateInfo *pFateInfo = (warrior::FateInfo *)m_pFateArray->objectAtIndex(i);
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
			
		}
		else
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
        
		if (stringLength > m_pFateLabel->getContentSize().width / m_pFateLabel->getFontSize() + 3)
		{
			fateStringLines++;
		}
	}

    //test
	//std::string fontPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("fonts/whzzlsf.TTF");
	//m_pFateLabel->setFontName(fontPath.c_str());
	m_pFateLabel->setString(fateString.c_str());

	if (fateStringLines > 0)
	{
		int fontSize = m_pFateLabel->getFontSize();
		m_pFateListBg->setContentSize(CCSizeMake(m_pFateListBg->getContentSize().width, (fontSize + 6) * (fateStringLines) + 45));
		m_pFateLabel->setDimensions(CCSizeMake(m_pFateLabel->getDimensions().width,  m_pFateListBg->getContentSize().height));
	}
}

void WarriorInfoView::updateTalentConten()
{
	if (m_pWarriorInfo == NULL || m_pTalentLabel == NULL || m_pTalentBg == NULL)
	{
		return;
	}
	
	m_pTalentLabel->setString(" ");

	CCDictionary* talentMap = m_pWarriorModel->getWarriorTalentsDic(m_pWarriorInfo->getWid());
	if (talentMap == NULL || talentMap->count() == 0)
	{
		return;
	}

	std::string talentString;
	unsigned int talentStringLines = 0;
	for (uint32_t i = WARRIOR_UP_LV_MIN; i <= WARRIOR_UP_LV_MAX;  i++)
	{
		CCInteger* pTalentId = dynamic_cast<CCInteger*>(talentMap->objectForKey(i));
		if (pTalentId == NULL)
		{
			continue;
		}
		
		warrior::TalentInfo* info = m_pWarriorModel->getTalentInfoById(pTalentId->getValue());
		if (info == NULL)
		{
			continue;
		}

		std::string singleTalentString;
		if (m_pWarriorInfo->getWarriorUpLv() >= i)
		{
			singleTalentString.assign(CCString::createWithFormat("[color=ff%s]%s:[/color][color=ff%s]%s[/color]\n",
				color_green, info->name.c_str(),
				color_yellow, info->_description.c_str())->getCString());
		}
		else
		{
			singleTalentString.assign(CCString::createWithFormat("[color=ff%s]%s:[color=ff%s](%d%s)[/color]%s[/color]\n",
											color_gray,
											info->name.c_str(),
											color_red,
											i,
											D_LOCAL_STRING("TalentOpen").c_str(),
											info->_description.c_str())->getCString());
		}

		talentString.append(singleTalentString);
		talentStringLines++;
        //subtract color symblo
        unsigned int stringLength = getUtf8Length(singleTalentString.c_str()) - (32 + 16);
        //CCLOG("talent%d, length:%d", i, stringLength);
		if (stringLength > m_pTalentLabel->getContentSize().width / m_pTalentLabel->getFontSize() + 4)
		{
			talentStringLines++;
		}
	}
	//CCLOG("talent lines:%d.", talentStringLines);
	m_pTalentLabel->setString(talentString.c_str());

	int fontSize = m_pTalentLabel->getFontSize();
	m_pTalentBg->setContentSize(CCSizeMake(m_pTalentBg->getContentSize().width, (fontSize + 6) * (talentStringLines) + 45));
	m_pTalentLabel->setDimensions(CCSizeMake(m_pTalentLabel->getDimensions().width, m_pTalentBg->getContentSize().height));
}


void WarriorInfoView::closeCallBack(cocos2d::CCObject* obj)
{
	m_bIsfromCollect_t=false;
	m_pOperate->setVisible(true);
	Game::sharedGame()->setNavigatorVisable(true);
	if(m_pDelegate)
	{
		m_pDelegate->closeInfoView(kWarriorInfo);
	}
	if(m_pveDelegate)
	{
		m_pveDelegate->closePopup();
	}
    else
    {
        removeFromParent();
    }
}

/////////////////////////////////////////////////////
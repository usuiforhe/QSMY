//  SkillInfo.cpp
//	弟子详情
//  Created by cll on 13-5-29
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "SkillInfo.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/FormationModel.h"
#include "../formation/Formation.h"
#include "managers/TipsManager.h"
#include "../common/common.h"
#include "../user/UserInfo.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define CELL_WIDTH 145

SkillInfoView::SkillInfoView()
:m_pSkillInfo(NULL)
,m_pSkillInfoSP(NULL)
{
    
}

SkillInfoView::~SkillInfoView()
{
    CC_SAFE_RELEASE(m_pSkillInfo);
    CC_SAFE_RELEASE(m_pSkillInfoSP);
}

bool SkillInfoView::init()
{
    do
    {
        CC_BREAK_IF(!DLayer::init());
        m_pSkillInfo = new SkillInfo();
        CC_BREAK_IF(!m_pSkillInfo || !m_pSkillInfo->init());
        m_pSkillInfoSP = new SkillInfoSP();
        CC_BREAK_IF(!m_pSkillInfoSP || !m_pSkillInfoSP->init());
        return  true;
    } while (0);
    
    CC_SAFE_DELETE(m_pSkillInfo);
    CC_SAFE_DELETE(m_pSkillInfoSP);

    return false;
}

void SkillInfoView::setSkillViewOnly(bool bViewOnly)
{
	if(m_pSkillInfo)  m_pSkillInfo->setViewOnly(bViewOnly);
}

void SkillInfoView::setDelegate(WarriorDelegate* pDelegate)
{
    if(m_pSkillInfoSP) m_pSkillInfoSP->setDelegate(pDelegate);
    if(m_pSkillInfo)  m_pSkillInfo->setDelegate(pDelegate);
}

void SkillInfoView::setSkillInfo(warrior::WarriorCurInfo* pWarrior, const uint32_t &sid)
{
    removeAllChildren();
    if(pWarrior->getBaseInfo()->getSkillId()==sid)
    {
        m_pSkillInfo->setWarrior(pWarrior);
        addChild(m_pSkillInfo);
    }else
    {
        m_pSkillInfoSP->setWarrior(pWarrior);
        addChild(m_pSkillInfoSP);
    }
}


#pragma mark SkillInfo
SkillInfo::SkillInfo()
:m_pDelegate(NULL)
,m_pSkillModel(NULL)
,m_pWarriorModel(NULL)
,m_pWarriorInfo(NULL)
,m_pSubmitButton(NULL)
,m_pSkillBox(NULL)
,m_pDesc(NULL)
,m_pStory(NULL)
,m_pTriggerProbability(NULL)
,m_pZone(NULL)
,m_pAttackRatio(NULL)
,m_pCloseButton(NULL)
,m_pBigSkillLevel(NULL)
,m_pNodeLevel(NULL)
//,m_pCloseLabel(NULL)
//,m_pSubmitLabel(NULL)
{
    D_RESET_C_ARRAY(m_pSkillType, skill::kSkillTypes);
}

SkillInfo::~SkillInfo()
{
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
    CC_SAFE_RELEASE(m_pSubmitButton);
    
    CC_SAFE_RELEASE(m_pSkillBox);
    CC_SAFE_RELEASE(m_pDesc);
    CC_SAFE_RELEASE(m_pStory);
    CC_SAFE_RELEASE(m_pTriggerProbability);
    CC_SAFE_RELEASE(m_pZone);
  /*  CC_SAFE_RELEASE(m_pCloseLabel);
    CC_SAFE_RELEASE(m_pSubmitLabel);*/
    D_SAFE_RELEASE_C_ARRAY(m_pSkillType,skill::kSkillTypes);
    CC_SAFE_RELEASE(m_pAttackRatio);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pBigSkillLevel);
	CC_SAFE_RELEASE(m_pNodeLevel);
}

bool SkillInfo::init()
{
    if(!DLayer::init()) return false;
    m_pSkillModel = DM_GET_SKILL_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    loadUI();

    return true;
}

void SkillInfo::setViewOnly(bool bViewOnly)
{
	//按钮和文字都隐藏
	if(m_pSubmitButton)  m_pSubmitButton->getParent()->setVisible(!bViewOnly);
}

bool SkillInfo::loadUI()
{
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_jineng/PL_jinengjieshao");
    
    m_pSubmitButton->getButton()->setTarget(this, menu_selector(SkillInfo::menuCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(SkillInfo::closeCallBack));

    return true;
}

bool SkillInfo::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox", SkillBox *, this->m_pSkillBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF *, this->m_pDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStory", CCLabelTTF *, this->m_pStory);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTriggerProbability", CCLabelTTF *, this->m_pTriggerProbability);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZone", CCLabelTTF *, this->m_pZone);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackRatio", CCLabelBMFont *, this->m_pAttackRatio);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillType0", CCSprite *, this->m_pSkillType[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillType1", CCSprite *, this->m_pSkillType[1]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton", DButton *, this->m_pSubmitButton);
     CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBigSkillLevel", CCLabelBMFont *, this->m_pBigSkillLevel);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeLevel", CCNode*, this->m_pNodeLevel);
   /* CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseLabel", CCNode *, this->m_pCloseLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitLabel", CCNode *, this->m_pSubmitLabel);*/
    return false;
}

SEL_MenuHandler SkillInfo::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", SkillInfo::closeCallBack);
    return NULL;
}

void SkillInfo::onEnter()
{
	binding();
	DLayer::onEnter();
}


void SkillInfo::onExit()
{
	UNBINDING_ALL(m_pWarriorInfo,this);
	DLayer::onExit();
}

void SkillInfo::setDelegate(WarriorDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
}

void SkillInfo::setWarrior(warrior::WarriorCurInfo* pWarrior)
{
    if(m_pWarriorInfo == pWarrior) return;
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RETAIN(pWarrior);
    m_pWarriorInfo= pWarrior;
	binding();
}

void SkillInfo::binding()
{
    BINDING_EXEC(m_pWarriorInfo,this,SkillInfo::_updateSkillInfo,D_EVENT_WARRIORINFO_UPDATE);
}


void SkillInfo::menuCallBack(cocos2d::CCObject * obj)
{
	//如果没有此英雄，不能点击
	if(!DM_GET_WARRIOR_MODEL->isMyWarrior(m_pWarriorInfo)) return;
    if(m_pDelegate)
    {
          m_pDelegate->showInfoView(kSkillUpgradeView,m_pWarriorInfo);
    }
}

void SkillInfo::closeCallBack(cocos2d::CCObject * obj)
{
    if(m_pDelegate) m_pDelegate->closeInfoView(kSkillInfoView);
    else removeFromParent();
}

void SkillInfo::_updateSkillInfo()
{
    updateSkillInfo();
}

void SkillInfo::updateSkillInfo()
{
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView(), 100);	//添加用户基本信息

    skill::Skill *pSkill = m_pSkillModel->getSkill(m_pWarriorInfo->getBaseInfo()->getSkillId());
    m_pSkillBox->updateView(pSkill,m_pWarriorInfo->getSlv());
    
	m_pBigSkillLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getSlv())->getCString());
    m_pDesc->setString(pSkill->desc.c_str());
    m_pStory->setString(pSkill->story.c_str());
    m_pTriggerProbability->setString(pSkill->trigger_probability.c_str());
    m_pZone->setString(m_pSkillModel->getSkillZoneInfo(pSkill->zone)->desc.c_str());
    m_pSubmitButton->getButton()->setEnabled(!m_pWarriorInfo->checkSkillLimit());
    
    for (uint8_t i=0; i<skill::kSkillTypes; ++i)
    {
        m_pSkillType[i]->setVisible(pSkill->type==i);
    }
    float attack_ratio = pSkill->getAttackRatio(m_pWarriorInfo->getSlv());
    m_pAttackRatio->setString(CCString::createWithFormat("%3.0f%%", attack_ratio*100.f)->getCString());
    
   /* m_pSubmitLabel->setVisible(m_pWarriorModel->isMyWarrior(m_pWarriorInfo));
    m_pCloseLabel->setVisible(!m_pSubmitLabel->isVisible());*/
    
}

#pragma mark SkillInfoSP

SkillInfoSP::SkillInfoSP()
:m_pSPData(NULL)
,m_pWarNum(NULL)
,m_pWarContainer(NULL)
{
    
}

SkillInfoSP::~SkillInfoSP()
{
    CC_SAFE_RELEASE(m_pSPData);
    
    CC_SAFE_RELEASE(m_pWarNum);
    CC_SAFE_RELEASE(m_pWarContainer);
}

bool SkillInfoSP::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarNum", CCLabelTTF *, this->m_pWarNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarContainer", CCNode *, this->m_pWarContainer);
    
    return SkillInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

bool SkillInfoSP::loadUI()
{
    m_pSPData = readCCData("PL_ui/ccb/pl_common/WarriorHeadUpLevel");
    CC_SAFE_RETAIN(m_pSPData);
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("SkillBox", SkillBoxLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_jineng/PL_zuhejineng", pNodeLoaderLibrary);
	    
    m_pSubmitButton->getButton()->setTarget(this, menu_selector(SkillInfoSP::menuCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(SkillInfoSP::menuCallBack));
    return true;
}

void SkillInfoSP::menuCallBack(cocos2d::CCObject * obj)
{
    SkillInfo::closeCallBack(obj);
}

void SkillInfoSP::updateSkillInfo()
{
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView(), 100);	//添加用户基本信息

    skill::Skill *pSkill = m_pSkillModel->getSkill(m_pWarriorInfo->getBaseInfo()->getSpSkillId());
    m_pDesc->setString(pSkill->desc.c_str());
    //m_pStory->setString(pSkill->story.c_str()); 组合技能没有故事
    m_pTriggerProbability->setString(pSkill->trigger_probability.c_str());
    m_pZone->setString(m_pSkillModel->getSkillZoneInfo(pSkill->zone)->desc.c_str());
    m_pBigSkillLevel->setString("");
	m_pNodeLevel->setVisible(false);

    for (uint8_t i=0; i<skill::kSkillTypes; ++i)
    {
        m_pSkillType[i]->setVisible(pSkill->type==i);
    }
    
    m_pWarContainer->removeAllChildren();
    
    
        
    //组合技能攻击系数成长公式：基础攻击系数+基础攻击系数0.15*（技能等级-1））
    
    warrior::WarriorBaseInfo *pWarriorBase = NULL;
    warrior::WarriorCurInfo *pWarrior = NULL;
    
    uint8_t warrior_num = 0;	//技能需要合体弟子个数
    uint8_t inpos_num = 0;		//在阵容中个数
    
    uint32_t wids[] = {pSkill->wid1,pSkill->wid2,pSkill->wid3};
    
    for (uint8_t i=0;i<D_CONFIG_UINT_FOR_KEY(SKILL_SPECIAL_MAXWARRIOR);i++)
    {
        pWarrior = NULL;
        pWarriorBase = NULL;
        
        if(wids[i]<=0) continue;
        pWarriorBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wids[i]);
        if(!pWarriorBase) continue;
        
        cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
        CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
        pNodeLoaderLibrary->registerCCNodeLoader(" WarriorHeadUpLevel", WarriorHeadUpLevelLoader::loader());
        
        DCCBReader * pReader = new DCCBReader(pNodeLoaderLibrary);
        WarriorHeadUpLevel* pNode= (WarriorHeadUpLevel*)pReader->readNodeGraphFromData(m_pSPData, this, CCDirector::sharedDirector()->getWinSize());
        pReader->release();
        if(pNode) m_pWarContainer->addChild(pNode);
        pNode->setPosition(ccp(CELL_WIDTH*warrior_num,0));
		pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wids[i]);
		if (pWarrior)
		{
			pNode->setWarriorInfo(pWarrior);
		}
		else
		{
			pNode->setWarriorInfo(pWarriorBase);
		}
        
		CCSprite* pHead = ResourceManager::sharedResourceManager()->getWarriorHeadIcon(pWarriorBase->getWarriorId());
		pNode->setHeadSprite(pHead);
        
        //高亮框
        if(DM_GET_FORMATION_MODEL->isWarriorBattle(wids[i]) || DM_GET_GUNDAM_EF_MODEL->isEgineerBattle(wids[i]))
        {
            inpos_num++;
			pNode->setNotInBattleTipVisible(false);
        }else
        {
           pNode->setNotInBattleTipVisible(pWarrior!=NULL);
            pHead->setShaderProgram(D_SHADER_MASK_GRAY);
            pHead->setOpacity(160);
        }
        warrior_num++;
    }
    
    int8_t upLevel = m_pSkillModel->getSpecialSkillLv(pSkill);
    
    if (upLevel>=0)
    {
        float attack_ratio = pSkill->getAttackRatio(upLevel);
        m_pAttackRatio->setString(CCString::createWithFormat("%3.0f%%", attack_ratio*100.f)->getCString());
        m_pSkillBox->updateView(pSkill,upLevel);
        
    }else
    {
        m_pAttackRatio->setString("???");
		//todo bmFont add "?"

        m_pSkillBox->updateView(pSkill,upLevel);
        //m_pSkillBox->getSkillUpLevel()->setVisible(false);
        //m_pSkillBox->getSkillLevel()->setVisible(false);
        m_pSkillBox->getLevelNode()->setVisible(false);
    }
    
    m_pWarNum->setString(CCString::createWithFormat("%d/%d",inpos_num,warrior_num)->getCString());
    
}




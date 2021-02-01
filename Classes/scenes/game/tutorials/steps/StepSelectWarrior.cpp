//
//  StepSelectWarrior.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "StepSelectWarrior.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/SoundManager.h"
USING_NS_CC;
StepSelectWarrior::StepSelectWarrior()
//:m_pDisposition(NULL)
//,m_pFeature(NULL)
//,m_pSkill(NULL)
//,m_pDesc(NULL)
//,m_pEffect(NULL)
:m_nPlayingSound(0u)
,m_selectedIndex(-1)
,m_pConfirmButton(NULL)
,m_pBg(NULL)
,m_pBg2(NULL)
,m_nSelectMode((DM_GET_TUTORIAL_MODEL->getSelectMode())==false?5:3)
{
	for (uint8_t i = 0; i< m_nSelectMode; i++)
	{
		m_pWarrior.push_back(NULL);
	}
}

StepSelectWarrior::~StepSelectWarrior()
{
	if (m_nPlayingSound)
	{
		SoundManager::sharedSoundManager()->stopSFX(m_nPlayingSound);
	}

	for (uint8_t i = 0; i< m_nSelectMode; i++)
	{
		CC_SAFE_RELEASE(m_pWarrior.at(i));
	}
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE_NULL(m_pBg);
	CC_SAFE_RELEASE_NULL(m_pBg2);
  //  CC_SAFE_RELEASE(m_pDisposition);
  //  CC_SAFE_RELEASE(m_pFeature);
  //  CC_SAFE_RELEASE(m_pSkill);
  //  CC_SAFE_RELEASE(m_pDesc);
  //  CC_SAFE_RELEASE(m_pEffect);

}

void StepSelectWarrior::finishStep()
{
	tutorial::WarriorInfo* pWarriorInfo = DM_GET_TUTORIAL_MODEL->getWarriorInfoByIndex(m_selectedIndex);
	if (pWarriorInfo == NULL)
	{
		pWarriorInfo = DM_GET_TUTORIAL_MODEL->getWarriorInfoByIndex(0);
	}
	
	if (pWarriorInfo)
	{
		HTTP_CLIENT->finishRookie(m_pStepInfo->getID(), pWarriorInfo->getID());
	}
	else
	{
		//todo
		Tips_Alert("error data.");
	}
}


void StepSelectWarrior::loadUI()
{
    m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("tutorial_Choice");
    CC_SAFE_RETAIN(m_pEffect);
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("TutorialWarrior", TutorialWarriorLoader::loader());
	if(m_nSelectMode==kSelectMode5){
		CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_xinshou/pl_xinshou_xuanren_new", pNodeLoaderLibrary);
	}
	if(m_nSelectMode==kSelectMode3)
	{
		CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_xinshou/pl_xinshou_xuanren", pNodeLoaderLibrary);
	}

   	bool updated = false;
	m_pConfirmButton->getButton()->setTarget(this, menu_selector(StepSelectWarrior::selectCallBack));
	for (uint8_t i = 0; i< m_nSelectMode; ++i)
	{
		tutorial::WarriorInfo* pWarriorInfo = DM_GET_TUTORIAL_MODEL->getWarriorInfoByIndex(i);
		if (pWarriorInfo->getSelectedDefault())
		{
			updated = true;
			updateInfo(i);
			break;
		}
	}

	if (updated == false)
	{
		updateInfo(0);
	}
	

	/*int defaultSelected = 0;
	for (uint8_t i = 0; i< m_nSelectMode; ++i)
	{
	tutorial::WarriorInfo* pWarriorInfo = DM_GET_TUTORIAL_MODEL->getWarriorInfoByIndex(i);
	m_pWarrior[i]->setWarriorInfo(pWarriorInfo);
	m_pWarrior[i]->getSelectStatus()->setEnabled(true);
	m_pWarrior[i]->getSelectStatus()->setTarget(this, menu_selector(StepSelectWarrior::clickWarriorCallBack));
	m_pWarrior[i]->getSelectStatus()->setTag(i);
	if(pWarriorInfo->getSelectedDefault())
	{
	defaultSelected = i;
	}
	}
	updateInfo(defaultSelected);*/
    m_sContentId = CCString::createWithFormat("%d",m_pStepInfo->getID() * 10 + m_nCurrentSubStep)->getCString();
}


bool StepSelectWarrior::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	if(m_nSelectMode==kSelectMode5){
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior1", CCSprite*, this->m_pWarrior[0]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior2", CCSprite*, this->m_pWarrior[1]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior3", CCSprite*, this->m_pWarrior[2]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior4", CCSprite*, this->m_pWarrior[3]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior5", CCSprite*, this->m_pWarrior[4]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg", CCSprite*, this->m_pBg);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg2", CCSprite*, this->m_pBg2);
	}
	if(m_nSelectMode==kSelectMode3){
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior1", CCSprite*, this->m_pWarrior[0]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior2", CCSprite*, this->m_pWarrior[1]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior3", CCSprite*, this->m_pWarrior[2]);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg", CCSprite*, this->m_pBg);
	} 
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton*, this->m_pConfirmButton);
	/* CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior0", TutorialWarrior*, this->m_pWarrior[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior1", TutorialWarrior*, this->m_pWarrior[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarrior2", TutorialWarrior*, this->m_pWarrior[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorImg0", CCSprite*, this->m_pWarriorName[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorImg1", CCSprite*, this->m_pWarriorName[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorImg2", CCSprite*, this->m_pWarriorName[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tutorial_warrior_0", CCSprite*, this->m_pWarriorImage[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tutorial_warrior_1", CCSprite*, this->m_pWarriorImage[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tutorial_warrior_2", CCSprite*, this->m_pWarriorImage[2]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDisposition", CCLabelTTF*, this->m_pDisposition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFeature", CCLabelTTF*, this->m_pFeature);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkill", CCLabelTTF*, this->m_pSkill);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDesc", CCLabelTTF*, this->m_pDesc);*/
    return StepEnForce::onAssignCCBMemberVariable(pTarget, pMemberVariableName , pNode);
}

SEL_MenuHandler StepSelectWarrior::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "chooseMenuCallBack", StepSelectWarrior::chooseMenuCallBack);
    return NULL;
}

void StepSelectWarrior::updateInfo(uint32_t index)
{
	if (m_selectedIndex == index
		|| index >= m_nSelectMode)
	{
		return;
	}

	m_selectedIndex = index;

	for (uint8_t i = 0; i< m_nSelectMode; i++)
	{
		m_pWarrior[i]->setVisible(i == index);
	}
	
	
	/*for (uint8_t i = 0; i< m_nSelectMode; ++i)
	{
	m_pWarrior[i]->getSelectStatus()->setEnabled(i!=index);
	m_pWarriorName[i]->setVisible(i==index);
	if(i==index)
	{
	m_pEffect->removeFromParent();
	m_pWarrior[i]->getEffectNode()->addChild(m_pEffect);
	m_pWarriorImage[i]->setShaderProgram(D_SHADER_NORMAL);
	}
	else
	{
	m_pWarriorImage[i]->setShaderProgram(D_SHADER_GARY);
	}
	}
	tutorial::WarriorInfo* pWarriorInfo = DM_GET_TUTORIAL_MODEL->getWarriorInfoByIndex(index);
	warrior::WarriorBaseInfo* pWarriorBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pWarriorInfo->getID());
	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(pWarriorBaseInfo->getSkillId());

	m_pDisposition->setString(pWarriorInfo->getDisposition().c_str());
	m_pFeature->setString(pWarriorInfo->getFeature().c_str());
	m_pSkill->setString(pSkill->getSkillName());
	m_pDesc->setString(pWarriorBaseInfo->getWarriorDesc().c_str());
	if (m_nPlayingSound)
	{
	SoundManager::sharedSoundManager()->stopSFX(m_nPlayingSound);
	}

	m_nPlayingSound = SoundManager::sharedSoundManager()->playSFX(ccs(pWarriorInfo->getSound()));*/
}

void StepSelectWarrior::selectCallBack(cocos2d::CCObject* obj)
{
    finishStep();
}

void StepSelectWarrior::chooseMenuCallBack( cocos2d::CCObject* obj )
{
	CCMenuItem* pMenuItem = dynamic_cast<CCMenuItem*>(obj);
	if (pMenuItem)
	{
		updateInfo(pMenuItem->getTag());
		if(pMenuItem->getTag()==kWarriorTag3||pMenuItem->getTag()==kWarriorTag4)
		{
			this->m_pBg2->setVisible(true);
			this->m_pBg->setVisible(false);
		}
		if(pMenuItem->getTag()==kWarriorTag0||pMenuItem->getTag()==kWarriorTag1||pMenuItem->getTag()==kWarriorTag2)
		{
			this->m_pBg->setVisible(true);
			if(m_nSelectMode==kSelectMode5)
			{
				this->m_pBg2->setVisible(false);
			}
	     }
	}
}



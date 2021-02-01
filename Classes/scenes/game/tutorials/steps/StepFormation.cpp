//
//  StepFormation.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "StepFormation.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"

USING_NS_CC;

void StepFormation::loadUI()
{
    std::string suffix(CCB_SUFFIX);
    suffix+=SUFFIX;
    if(CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(CCString::createWithFormat("ui/ccb/tutorial/tutorial%d%s",m_pStepInfo->getID() * 10 + m_nCurrentSubStep,suffix.c_str())->getCString())))
    {
        StepEnForce::loadUI();
        return;
    }
    
    cocos2d::CCArray* pWarriors =  DM_GET_TUTORIAL_MODEL->getWarriors();
    tutorial::WarriorInfo *pWarrior = NULL;
    warrior::WarriorCurInfo* pWarriorCurInfo = NULL;
    CCObject*obj = NULL;
    CCARRAY_FOREACH(pWarriors, obj)
    {
        pWarrior = static_cast<tutorial::WarriorInfo *>(obj);
        pWarriorCurInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pWarrior->getID());
        if(pWarriorCurInfo!=NULL) break;
    }
    uint8_t type =  pWarriorCurInfo->getBaseInfo()->getLocation() == warrior::kLocationTypeDPS?1:2;
    CCB_READER_AND_ADDCHILD(CCString::createWithFormat("ui/ccb/tutorial/tutorial%d%d",m_pStepInfo->getID() * 10 + m_nCurrentSubStep,type)->getCString());
    if(m_pAwContainer) m_pAwContainer->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("tutorialaw"));
    m_sContentId = CCString::createWithFormat("%d%d",m_pStepInfo->getID() * 10 + m_nCurrentSubStep,type)->getCString();
}

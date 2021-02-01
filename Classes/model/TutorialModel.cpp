//
//  TutorialModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TutorialModel.h"
#include "../managers/GameManager.h"
#include "../managers/DataManager.h"
#include "../managers/PlatformManager.h"
#include "../utils/StringUtil.h"
#include "../scenes/game/Game.h"


USING_NS_CC;

bool tutorial::StepInfo::isComplete()
{
	int lastId = DM_GET_TUTORIAL_MODEL->getLastStepId();
    return lastId >= id;
}

bool tutorial::StepInfo::isTrigger()
{
    switch (trigger_type) {
        case kTutorialTriggerTypeMapComplete:
            return DM_GET_PVE_MODEL->getMapInfo(trigger_condition)->iScores>0;
            break;
		case kTutorialTriggerTypeFirst3Star:
			return DM_GET_PVE_MODEL->getChapterInfo(trigger_condition)->canGetAllStarReward();
			break;
		case kTutorialTriggerTypeReputation:
			return DM_GET_PVE_MODEL->getChapterInfo(trigger_condition)->canGetPrestigeReward();
			break;
		case kTutorialTriggerTypeFirsrWonder:  
			return DM_GET_TUTORIAL_MODEL->getHasWonder();
			break;
        case kTutorialTriggerTypeCompleteStep:
            return DM_GET_TUTORIAL_MODEL->getLastStepId() >= trigger_condition;
            break;
        case kTutorialTriggerTypeUserLvel:
            return DM_GET_USER_MODEL->getUserInfo()->getUserLv() >= trigger_condition;
            break;
		case kTutorialTriggerTypeFirstSignin:
			return DM_GET_USER_MODEL->getUserInfo()->getUserLv() >= trigger_condition && Game::sharedGame()->getCurrentPage() == navigation::kNavHome;
			break;
        default:
            return false;
            break;
    }
    return false;
}


tutorial::StepInfo* tutorial::StepInfo::nextStep()
{
    return DM_GET_TUTORIAL_MODEL->getStepInfoByIndex(index+1);
}


TutorialModel::TutorialModel()
:m_pTutorials(NULL)
,m_pTutorialsMap(NULL)
,m_pWarriors(NULL)
,m_pTutorialContents(NULL)
,m_nLastStep(0)
,m_nMaxLevel(0)
,m_bRunning(false)
,m_nLastUserStep(0)
,m_bHasWonder(false)
,m_bSelectMode(false)
{

}

TutorialModel::~TutorialModel()
{
    CC_SAFE_RELEASE(m_pTutorials);
    CC_SAFE_RELEASE(m_pTutorialsMap);
    CC_SAFE_RELEASE(m_pWarriors);
    CC_SAFE_RELEASE(m_pTutorialContents);
}


bool TutorialModel::init()
{
    m_pTutorials = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pTutorials);
    
    m_pTutorialsMap = cocos2d::CCDictionary::create();
    CC_SAFE_RETAIN(m_pTutorialsMap);
    
   
    m_pTutorialContents = cocos2d::CCDictionary::create();
    CC_SAFE_RETAIN(m_pTutorialContents);
    
    
    m_pWarriors = cocos2d::CCArray::create();
    CC_SAFE_RETAIN(m_pWarriors);
    
	std::string userStepString = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_TUTORIAL_SUBSTEP);
	if (userStepString.length() >0 && m_nLastUserStep == 0)
	{
		m_nLastUserStep = atoi(userStepString.c_str());
	}

    loadData();
    return Model::init();
}

void TutorialModel::loadData()
{
    parseData( CSVDecoder::create(TUTORIAL_CONFIG_PATH)->getData());
    parseWarriorData( CSVDecoder::create(FIRST_WARRIOR_INFO_CONFIG_PATH)->getData());
    parseContentData( CSVDecoder::create(TUTORIAL_CONTENT_CONFIG_PATH)->getData());
}

void TutorialModel::parseContentData(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
		//m_pTutorialContents->setObject(ccs(val[i][1]), val[i][0]);
		tutorial::StepDialog *pDialog = tutorial::StepDialog::create();
		pDialog->m_sID = val[i][0];
		pDialog->m_sContents = val[i][1];
		pDialog->m_sResID = val[i][2].c_str();
		pDialog->m_fDelay = atof(val[i][3].c_str());
		pDialog->m_uEmotionID = atoi(val[i][4].c_str());
		m_pTutorialContents->setObject(pDialog, pDialog->m_sID);
		pDialog->m_dialogPosition = ccp(atoi(val[i][5].c_str()), atoi(val[i][6].c_str()));
		pDialog->m_touchRect = CCRectMake(atoi(val[i][7].c_str()),
																		atoi(val[i][8].c_str()),
																		atoi(val[i][9].c_str()),
																		atoi(val[i][10].c_str()));
		pDialog->m_fTouchAjustFactor = atof(val[i][11].c_str());
		pDialog->m_TouchType = (tutorial::TutorialTouchType)(atoi(val[i][12].c_str()));
        pDialog->m_profileSide = (tutorial::TutorialProfileSide)(atoi(val[i][13].c_str()));
		pDialog->m_sPromptIconPath = val[i][14];
	}
}

void TutorialModel::parseWarriorData(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
		tutorial::WarriorInfo *pWarrior = tutorial::WarriorInfo::create();
        if(pWarrior)
        {
            pWarrior->m_nId         = atoi(val[i][0].c_str());
            pWarrior->m_sDisposition    = val[i][1].c_str();
            pWarrior->m_sFeature    = val[i][2].c_str();
            pWarrior->m_bSelectedDefault = atoi(val[i][3].c_str())==1;
			pWarrior->m_sSound = CCString::createWithFormat("unit/%s", val[i][4].c_str())->getCString();
            m_pWarriors->addObject(pWarrior);
        }
	}
}

void TutorialModel::parseData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		tutorial::StepInfo *pStep = tutorial::StepInfo::create();
		if (pStep)
		{
			pStep->id                   = atoi(val[i][0].c_str());
            pStep->type                 = (tutorial::TutorialType)(atoi(val[i][1].c_str()));
            pStep->trigger_type         = (tutorial::TutorialTriggerType)(atoi(val[i][2].c_str()));
            pStep->trigger_condition    = atoi(val[i][3].c_str());
            pStep->steps                = atoi(val[i][4].c_str());
            pStep->navId                = atoi(val[i][5].c_str());
			pStep->keyStepId		= atoi(val[i][6].c_str());
            pStep->index                = i;
            if(pStep->trigger_type == tutorial::kTutorialTriggerTypeUserLvel && pStep->trigger_condition>m_nMaxLevel) m_nMaxLevel = pStep->trigger_condition;
            m_pTutorialsMap->setObject(pStep, pStep->id);
			m_pTutorials->addObject(pStep);
		}
	}
}

const char* TutorialModel::getContent(const char* step)
{
    tutorial::StepDialog* pDialog = static_cast<tutorial::StepDialog*>(m_pTutorialContents->objectForKey(step));
    if(!pDialog) return "";
    return pDialog->getContents().c_str();
}

tutorial::StepDialog *TutorialModel::getStepDialog(const char *step)
{
	return (tutorial::StepDialog *)(m_pTutorialContents->objectForKey(step));
}

void TutorialModel::setLastStep(uint16_t step)
{
    if(step!=m_nLastStep)
    {
        m_nLastStep = step;
        tutorial::StepInfo* pStep = getLastStep();
        if(pStep)  pStep->postNotification(D_EVENT_TUTORIAL_STEP_COMPLETE);
    }
    if(!isSynced)
    {
        isSynced = true;
        postNotification(D_EVENT_TUTORIAL_INITED);
    }
}

tutorial::StepInfo* TutorialModel::getStepInfoById(uint16_t id)
{
    return (tutorial::StepInfo*)m_pTutorialsMap->objectForKey(id);
}

tutorial::StepInfo* TutorialModel::getStepInfoByIndex(uint32_t index)
{
    if(index>=m_pTutorials->count()) return NULL;
    return (tutorial::StepInfo*)m_pTutorials->objectAtIndex(index);
}

tutorial::StepInfo* TutorialModel::getNextStep()
{
    tutorial::StepInfo* pStep = getLastStep();
    if(pStep) return pStep->nextStep();
    if(m_nLastStep==0) return getStepInfoByIndex(0);
    return NULL;
}

tutorial::StepInfo* TutorialModel::getLastStep()
{
    return getStepInfoById(m_nLastStep);
}

bool TutorialModel::isAllStepComplete()
{
    //if(CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_TUTORIAL_FINISH,false)) return true;
    if(!isSynced) return true; //如果新手引导做完了 是不会同步的
    return m_nLastStep!=0 && getLastStep() == NULL;
}

bool TutorialModel::whetherNeedToMuteMusic()
{
    tutorial::StepInfo* info = getNextStep();;
    if (info && info->getID() == G_FIRST_BATTLE_ID) {
        return true;
    }
    
    return false;
}

tutorial::WarriorInfo* TutorialModel::getWarriorInfoByIndex(uint32_t index)
{
    if(index>=m_pWarriors->count()) return NULL;
    return (tutorial::WarriorInfo*)m_pWarriors->objectAtIndex(index);
}

tutorial::WarriorInfo* TutorialModel::getDefaultWarrior()
{
    CCObject* obj=NULL;
    tutorial::WarriorInfo* pWarrior = NULL;
    CCARRAY_FOREACH(m_pWarriors, obj)
    {
        pWarrior = (tutorial::WarriorInfo*)obj;
        if(pWarrior->getSelectedDefault()) return pWarrior;
    }
    return NULL;
}

void TutorialModel::getLockedNavSteps(std::vector<uint8_t> &ids)
{
    ids.clear();
    if(isAllStepComplete()) return;
    uint32_t i = 0;
    tutorial::StepInfo* pStepInfo = getLastStep();
    if(pStepInfo) i = getLastStep()->index+1;
    
    for (; i<m_pTutorials->count(); ++i)
    {
        pStepInfo = (tutorial::StepInfo*)m_pTutorials->objectAtIndex(i);
        if(!pStepInfo || pStepInfo->navId ==0) continue;
        ids.push_back(pStepInfo->navId);
    }
}

bool TutorialModel::isNeedHide()
{
    tutorial::StepInfo* pStepInfo = getNextStep();
    if(!pStepInfo) return false;
    return pStepInfo->type== tutorial::kTutorialTypePrompt;
}

void TutorialModel::getRookieInfo()
{
    if(isSynced) return;
    HTTP_CLIENT->getRookieInfo();
}

void TutorialModel::reset()
{
	CCLog("hehe");
    //do nothing

}

cocos2d::CCPoint TutorialModel::getDialogPosition( const char* step )
{

	CCLog("hehe");
	tutorial::StepDialog* pDialog = static_cast<tutorial::StepDialog*>(m_pTutorialContents->objectForKey(step));
	if(!pDialog) return CCPointZero;
	return pDialog->getDialogPosition();
}

cocos2d::CCRect TutorialModel::getTouchRect( const char* step )
{
	tutorial::StepDialog* pDialog = static_cast<tutorial::StepDialog*>(m_pTutorialContents->objectForKey(step));
	if(!pDialog) return CCRectMake(0,0,0,0);
	return pDialog->getTouchRect();
}

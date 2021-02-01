//
//  StoryTalk.cpp
//  QSMY
//
//  Created by wanghejun on 13-10-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "StoryTalk.h"
#include "model/WarriorModel.h"
#include "managers/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

StoryTalk::StoryTalk()
:m_pDialogRight(NULL)
,m_pDialogLeft(NULL)
,m_pDialogNoHead(NULL)
,m_pContainerRight(NULL)
,m_pContainerLeft(NULL)
,m_pDialogLabelRight(NULL)
,m_pDialogLabelLeft(NULL)
,m_pDialogLabelNoHead(NULL)
,m_pNextDialogButton(NULL)
,first(true)
,m_pDialogRichLabelRight(NULL)
,m_pDialogRichLabelLeft(NULL)
,m_pDialogRichLabelNoHead(NULL)
{
    
}

StoryTalk::~StoryTalk()
{
    CC_SAFE_RELEASE(m_pDialogRight);
    CC_SAFE_RELEASE(m_pDialogLeft);
	CC_SAFE_RELEASE(m_pDialogNoHead);
    CC_SAFE_RELEASE(m_pContainerRight);
    CC_SAFE_RELEASE(m_pContainerLeft);
    CC_SAFE_RELEASE(m_pDialogLabelRight);
    CC_SAFE_RELEASE(m_pDialogLabelLeft);
	CC_SAFE_RELEASE(m_pDialogLabelNoHead);
    CC_SAFE_RELEASE(m_pNextDialogButton);
    CC_SAFE_RELEASE(m_pDialogRichLabelRight);
    CC_SAFE_RELEASE(m_pDialogRichLabelLeft);
	CC_SAFE_RELEASE(m_pDialogRichLabelNoHead);
}


bool StoryTalk::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogRight",  CCNode*, this->m_pDialogRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogLeft",  CCNode*, this->m_pDialogLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogNoHead", CCNode*, this->m_pDialogNoHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainerRight",  CCNode*, this->m_pContainerRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainerLeft",  CCNode*, this->m_pContainerLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogLabelRight",  CCLabelTTF*, this->m_pDialogLabelRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogLabelLeft",  CCLabelTTF*, this->m_pDialogLabelLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDialogLabelNoHead", CCLabelTTF *, this->m_pDialogLabelNoHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNextDialogButton",  CCMenuItem*, this->m_pNextDialogButton);
    
    return false;
}

void StoryTalk::showInfo(battle::BattleDialogInfo* pInfo)
{
    if(first)
    {
        m_pDialogRichLabelLeft = CCRichLabelTTF::create();
        m_pDialogRichLabelRight = CCRichLabelTTF::create();
		m_pDialogRichLabelNoHead = CCRichLabelTTF::create();
        
        CC_SAFE_RETAIN(m_pDialogRichLabelRight);
		CC_SAFE_RETAIN(m_pDialogRichLabelLeft);
		CC_SAFE_RETAIN(m_pDialogRichLabelNoHead);
        
        replaceLabel(m_pDialogLabelLeft,m_pDialogRichLabelLeft);
		replaceLabel(m_pDialogLabelRight,m_pDialogRichLabelRight);
		replaceLabel(m_pDialogLabelNoHead, m_pDialogRichLabelNoHead);
        
        first=false;
    }
    
    m_pContainerLeft->removeAllChildren();
    m_pContainerRight->removeAllChildren();
    if(pInfo==NULL) return;
	
	uint32_t warriorId = pInfo->getWarriorId();
	uint32_t flipRes = 0;
	if(battle::kDialogDirectionRight == pInfo->getDirection())
	{
		warrior::WarriorBaseInfo* warriorInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pInfo->getWarriorId());
		flipRes = warriorInfo->getFlipDialogRes();
		warriorId = flipRes > 0 ? flipRes : warriorId;
	}
	
    CCSprite* pHead = ResourceManager::sharedResourceManager()->getWarriorIcon(warriorId);
    switch (pInfo->getDirection()) {
        case battle::kDialogDirectionLeft:
        {
            m_pDialogLeft->setVisible(true);
            m_pDialogRight->setVisible(false);
			m_pDialogNoHead->setVisible(false);
            m_pDialogRichLabelLeft->setString(pInfo->getContent().c_str());
            m_pContainerLeft->addChild(pHead);
        }
            break;
        case battle::kDialogDirectionRight:
        {
            m_pDialogLeft->setVisible(false);
			m_pDialogRight->setVisible(true);
			m_pDialogNoHead->setVisible(false);
            m_pDialogRichLabelRight->setString(pInfo->getContent().c_str());
            m_pContainerRight->addChild(pHead);
			
			if(flipRes == 0) pHead->setFlipX(true);
        }
            break;
		case battle::kDialogDirectionNoHead:
		{
			m_pDialogLeft->setVisible(false);
			m_pDialogRight->setVisible(false);
			m_pDialogNoHead->setVisible(true);
			m_pDialogRichLabelNoHead->setString(pInfo->getContent().c_str());
			m_pContainerRight->addChild(pHead);
		}
			break;
        default:
            break;
    }
}






//
//  TutorialWarrior.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TutorialWarrior.h"
USING_NS_CC;
USING_NS_CC_EXT;

TutorialWarrior::TutorialWarrior()
:m_pContainer(NULL)
,m_pSelectStatus(NULL)
,m_pWarriorInfo(NULL)
,m_pEffectNode(NULL)
,m_pWarriorImage(NULL)
{
    
}


TutorialWarrior::~TutorialWarrior()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pSelectStatus);
    CC_SAFE_RELEASE(m_pWarriorInfo);
    CC_SAFE_RELEASE(m_pEffectNode);
	CC_SAFE_RELEASE(m_pWarriorImage);
}


bool TutorialWarrior::init()
{
	if(!CCNode::init())
	{
		return false;
	}
    
	return true;
    
}

bool TutorialWarrior::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",  CCNode*, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectStatus",  CCMenuItem*, this->m_pSelectStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode",  CCNode*, this->m_pEffectNode);
	
    return false;
}

void TutorialWarrior::setWarriorImage(cocos2d::CCSprite *pImage)
{
	CC_SAFE_RETAIN(pImage);
	CC_SAFE_RELEASE(m_pWarriorImage);
	m_pWarriorImage = pImage;
	m_pContainer->removeAllChildren();
	m_pContainer->addChild(m_pWarriorImage);
}
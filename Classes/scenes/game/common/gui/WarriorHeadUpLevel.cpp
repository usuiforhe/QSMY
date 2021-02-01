#include "WarriorHeadUpLevel.h"
#include "WarriorHeadSmall.h"
#include "managers/GameManager.h"

WarriorHeadUpLevel::WarriorHeadUpLevel()
	:m_pWarriorHeadSmall(NULL)
	,m_pName(NULL)
	,m_pUpLevel(NULL)
{

}

WarriorHeadUpLevel::~WarriorHeadUpLevel()
{
	CC_SAFE_RELEASE(m_pWarriorHeadSmall);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pUpLevel);
}

bool WarriorHeadUpLevel::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorHeadSmall", WarriorHeadSmall*, this->m_pWarriorHeadSmall);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", cocos2d::CCLabelTTF*, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpLevel", cocos2d::CCLabelBMFont*, this->m_pUpLevel);

	return false;
}

void WarriorHeadUpLevel::setWarriorInfo( warrior::WarriorBaseInfo* pWarriorBaseInfo )
{
	resetWarriorInfo();
	if (m_pWarriorHeadSmall)
	{
		m_pWarriorHeadSmall->setWarriorInfo(pWarriorBaseInfo);
	}
	updateWarriorInfo(pWarriorBaseInfo);
}

void WarriorHeadUpLevel::setWarriorInfo( warrior::WarriorCurInfo* pWarriorInfo )
{
	setWarriorInfo(pWarriorInfo->getBaseInfo());
	if (m_pUpLevel)
	{
		m_pUpLevel->setString(CCString::createWithFormat("%d", pWarriorInfo->getWarriorUpLv())->getCString());
	}
}

void WarriorHeadUpLevel::resetWarriorInfo()
{
	if (m_pName)
	{
		m_pName->setString("");
	}
	
	if (m_pUpLevel)
	{
		m_pUpLevel->setString("0");
	}
}

void WarriorHeadUpLevel::updateWarriorInfo( warrior::WarriorBaseInfo* pWarriorBaseInfo )
{
	if (pWarriorBaseInfo == NULL)
	{
		return;
	}

	if (m_pName)
	{
		m_pName->setString(pWarriorBaseInfo->getWarriorName().c_str());
	}
}

void WarriorHeadUpLevel::setNotInBattleTipVisible( bool visible )
{
	if (m_pWarriorHeadSmall && m_pWarriorHeadSmall->getNotInBattle())
	{
		m_pWarriorHeadSmall->getNotInBattle()->setVisible(visible);
	}
}

void WarriorHeadUpLevel::setHeadSprite( cocos2d::CCSprite* pHead )
{
	if (NULL == pHead)
	{
		return;
	}

	CCNode* pHeadContainer = m_pWarriorHeadSmall->getHeadContainer();
	if (m_pWarriorHeadSmall && pHeadContainer)
	{
		pHeadContainer->removeAllChildren();
		pHeadContainer->addChild(pHead);
	}
}



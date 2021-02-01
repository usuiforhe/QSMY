#include "WarriorHead.h"


WarriorHead::WarriorHead()
	:m_pIconSprite(NULL)
	,m_pItemName(NULL)
	,m_pSoulMark(NULL)
	,m_pBMFNum(NULL)
	,m_pBase(NULL)
{
}

WarriorHead::~WarriorHead()
{
	CC_SAFE_RELEASE(m_pIconSprite);
	CC_SAFE_RELEASE(m_pItemName);
	CC_SAFE_RELEASE(m_pSoulMark);
	CC_SAFE_RELEASE(m_pBMFNum);
	CC_SAFE_RELEASE(m_pBase);
}

bool WarriorHead::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	bool ret = WarriorGradeBg::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
	if (ret)
	{
		return true;
	}
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconSprite", cocos2d::CCNode *, m_pIconSprite);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", cocos2d::CCLabelTTF *, m_pItemName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulMark", cocos2d::CCNode *, m_pSoulMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNum", cocos2d::CCLabelBMFont *, m_pBMFNum);
	return false;
}

void WarriorHead::setWarriorBaseInfo(warrior::WarriorBaseInfo *pBase)
{
	CC_SAFE_RETAIN(pBase);
	CC_SAFE_RELEASE(m_pBase);
	m_pBase = pBase;
	updateView();
}

void WarriorHead::setNum(unsigned int uNum)
{
	if (m_pBMFNum == NULL)
	{
		return;
	}
	m_pBMFNum->setVisible(uNum > 0);
	m_pBMFNum->setString(D_CSTRING_FROM_UINT(uNum));
}

void WarriorHead::setIsSoul(bool isSoul)
{
	if (m_pSoulMark == NULL)
	{
		return;
	}
	m_pSoulMark->setVisible(isSoul);
}

void WarriorHead::updateView()
{
	setWarriorGrade(m_pBase->getColorGrade());
	if (m_pIconSprite != NULL)
	{
		m_pIconSprite->removeAllChildrenWithCleanup(true);
		CCNode *pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(m_pBase->getWarriorId(), 0.35f);
		m_pIconSprite->addChild(pNode);
	}
	if (m_pItemName)
	{
		m_pItemName->setString(m_pBase->getWarriorName().c_str());
	}
}

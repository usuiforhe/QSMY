#include "WarriorHeadGradeBg.h"


WarriorHeadGradeBg::WarriorHeadGradeBg()
	:m_eGrade(warrior::WarriorGrade(0))
{
	m_vGradeBgs.insert(m_vGradeBgs.begin(), warrior::kWarGradeRed + 1, (cocos2d::CCSprite*)NULL);
}

WarriorHeadGradeBg::~WarriorHeadGradeBg()
{
	for (unsigned int i = 0; i < m_vGradeBgs.size(); i++)
	{
		CC_SAFE_RELEASE(m_vGradeBgs[i]);
	}
}

bool WarriorHeadGradeBg::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeNone", cocos2d::CCNode *, m_vGradeBgs[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradeGreen]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradeBlue]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradePurple]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradeRed]);
	return false;
}

void WarriorHeadGradeBg::setWarriorGrade( warrior::WarriorGrade grade )
{
	if (grade && grade == m_eGrade)
	{
		return;
	}

	m_eGrade = grade;
	updateView();
}

void WarriorHeadGradeBg::updateView()
{
	for (unsigned int i = 0; i < m_vGradeBgs.size(); i++)
	{
		if (m_vGradeBgs[i])
		{
			m_vGradeBgs[i]->setVisible(false);
		}
	}

	m_vGradeBgs[m_eGrade]->setVisible(true);
}

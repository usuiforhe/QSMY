#include "WarriorGradeBg.h"

WarriorGradeBg::WarriorGradeBg()
	:m_eGrade(warrior::WarriorGrade(0))
{
	m_vGradeBgs.insert(m_vGradeBgs.begin(), warrior::kWarGradeRed + 1, (cocos2d::CCSprite*)NULL);
	D_RESET_C_ARRAY(m_pGradeLabel, WARRIOR_GRADE_NUM);
}

WarriorGradeBg::~WarriorGradeBg()
{
	for (unsigned int i = 0; i < m_vGradeBgs.size(); i++)
	{
		CC_SAFE_RELEASE(m_vGradeBgs[i]);
	}
	D_SAFE_RELEASE_C_ARRAY(m_pGradeLabel, WARRIOR_GRADE_NUM);
}

bool WarriorGradeBg::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeNone", cocos2d::CCNode *, m_vGradeBgs[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeGreen", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradeGreen]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeBlue", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradeBlue]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradePurple", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradePurple]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGradeOrange", cocos2d::CCNode *, m_vGradeBgs[warrior::kWarGradeRed]);
	D_CCB_ASSIGN_ARRAY("m_pGradeLabel", cocos2d::CCNode*, m_pGradeLabel, WARRIOR_GRADE_NUM);
	return false;
}

void WarriorGradeBg::setWarriorGrade( warrior::WarriorGrade grade )
{
	if (grade == m_eGrade)
	{
		return;
	}

	if (grade < warrior::kWarGradeNone || grade > warrior::kWarGradeRed)
	{
		return;
	}

	m_eGrade = grade;
	updateView();
}

void WarriorGradeBg::updateView()
{
	for (unsigned int i = 0; i < m_vGradeBgs.size(); i++)
	{
		if (m_vGradeBgs[i])
		{
			m_vGradeBgs[i]->setVisible(false);
		}
	}

	if (m_eGrade < m_vGradeBgs.size() &&  m_vGradeBgs[m_eGrade])
	{
		m_vGradeBgs[m_eGrade]->setVisible(true);
	}
}

void WarriorGradeBg::reset()
{
	for (unsigned int i = 1; i < m_vGradeBgs.size(); i++)
	{
		if (m_vGradeBgs[i])
		{
			m_vGradeBgs[i]->setVisible(false);
		}
	}
	m_eGrade = warrior::WarriorGrade(0);
	m_vGradeBgs[0]->setVisible(true);
}

void WarriorGradeBg::hideGradeLabel()
{
	for(int i=0; i<WARRIOR_GRADE_NUM; i++)
	{
		if (m_pGradeLabel[i])
		{
			m_pGradeLabel[i]->setVisible(false);
		}
	}
}

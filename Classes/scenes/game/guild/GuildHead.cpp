//
//  GuildHead.h
//  公会头部
//
//  Created by Eci on 13-10-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

//#define NOTICE_MAX	2

#include "GuildHead.h"

USING_NS_CC;
USING_NS_CC_EXT;

/*

cocos2d::CCLabelTTF *m_pLabelName;
cocos2d::CCLabelTTF *m_pLabelPosition;
cocos2d::CCLabelBMFont *m_pLabelLevel;
cocos2d::CCLabelBMFont *m_pLabelExp;
DProgressBar* m_pExpBar;				
cocos2d::CCLabelBMFont *m_pLabelNextExp;

cocos2d::CCLabelTTF *m_pLabelNotice0;
cocos2d::CCLabelTTF *m_pLabelNotice1;
cocos2d::CCLabelTTF *m_pLabelNotice2;
*/

GuildHead::GuildHead()
	: m_pLabelName(NULL)
	, m_pLabelPosition(NULL)
	, m_pLabelLevel(NULL)
	, m_pLabelExp(NULL)
	, m_pExpBar(NULL)
	, m_pLabelNextExp(NULL)
	, m_pLabelNotice0(NULL)
	, m_pLabelNotice1(NULL)
	, m_pRichLabelNotice0(NULL)
	, m_pRichLabelNotice1(NULL)
{
    
}


GuildHead::~GuildHead()
{
    CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelPosition);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelExp);
	CC_SAFE_RELEASE(m_pExpBar);
	CC_SAFE_RELEASE(m_pLabelNextExp);
	CC_SAFE_RELEASE(m_pLabelNotice0);
	CC_SAFE_RELEASE(m_pLabelNotice1);
	CC_SAFE_RELEASE(m_pRichLabelNotice0);
	CC_SAFE_RELEASE(m_pRichLabelNotice1);
}


bool GuildHead::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_position", CCLabelTTF *, this->m_pLabelPosition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp", CCLabelBMFont *, this->m_pLabelExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp_bar", DProgressBar *, this->m_pExpBar);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_next_exp", CCLabelBMFont *, this->m_pLabelNextExp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_0", CCLabelTTF *, this->m_pLabelNotice0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_1", CCLabelTTF *, this->m_pLabelNotice1);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_2", CCLabelTTF *, this->m_pLabelNotice2);

    return false;
}

void GuildHead::setName(const char *sName)
{
	m_pLabelName->setString(sName);
}

void GuildHead::setLevel(uint32_t iLevel)
{
	m_pLabelLevel->setString(CCString::createWithFormat("%u", iLevel)->getCString());
}

void GuildHead::setKnighthood(const char *sPosition)
{
	m_pLabelPosition->setString(sPosition);
}

void GuildHead::setGuildExp(uint32_t exp, uint32_t next_exp, float dur/* = .2f*/)
{
	if (next_exp > 0)
	{
		m_pLabelNextExp->setString(CCString::createWithFormat("%u/%u", exp, next_exp)->getCString());
		float percent = (float)exp / next_exp;
		m_pExpBar->setPercent(percent, dur);
	}
	else
	{
		m_pLabelNextExp->setString("--/--");
		m_pExpBar->setPercent(1.0, dur);
	}
}

void GuildHead::setUserExp(uint32_t next_exp)
{
	if (next_exp > 0)
	{
		m_pLabelExp->setString(CCString::createWithFormat("%u", next_exp)->getCString());
	}
	else
	{
		m_pLabelExp->setString("--");
	}
}

void GuildHead::setNotice(CCArray *pNotice)
{
	if (NULL == m_pRichLabelNotice0 && NULL == m_pRichLabelNotice1)
	{

		m_pRichLabelNotice0 = CCRichLabelTTF::create();
		m_pRichLabelNotice1 = CCRichLabelTTF::create();

		CC_SAFE_RETAIN(m_pRichLabelNotice0);
		CC_SAFE_RETAIN(m_pRichLabelNotice1);

		replaceLabel(m_pLabelNotice0,m_pRichLabelNotice0);
		replaceLabel(m_pLabelNotice1,m_pRichLabelNotice1);
		m_pLabelNotice0->setString("");
		m_pLabelNotice1->setString("");
	}
	
	m_pRichLabelNotice0->setString("");
	m_pRichLabelNotice1->setString("");
	//m_pLabelNotice2->setString("");
	if (NULL == pNotice || 0 == pNotice->count())
	{
		return;
	}
	CCArray *noticeArray = CCArray::create();
	for (int i = 0; i < NOTICE_MAX && i < pNotice->count(); i++)
	{
		noticeArray->addObject(pNotice->objectAtIndex(i));
	}
	noticeArray->reverseObjects();
	guild::GuildNotice *pNot = (guild::GuildNotice *)noticeArray->objectAtIndex(0);
	m_pRichLabelNotice0->setString(pNot->getMarquee().c_str());
	if (1 == noticeArray->count())
	{
		return;
	}
	pNot = (guild::GuildNotice *)noticeArray->objectAtIndex(1);
	m_pRichLabelNotice1->setString(pNot->getMarquee().c_str());
/*	if (2 == pNotice->count())
	{
		return;
	}
	pNot = (guild::GuildNotice *)pNotice->objectAtIndex(2);
	m_pLabelNotice2->setString(pNot->getMarquee().c_str());*/
}
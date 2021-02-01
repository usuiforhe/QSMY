//
//  GuildHead.h
//  公会头部
//
//  Created by Eci on 13-10-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildHead__
#define __QSMY__GuildHead__

#include "AppInclude.h"
#include "components/DProgressBar.h"
#include "managers/DataManager.h"

#define NOTICE_MAX	3

#define GUILD_HEAD_UPDATE(__GUILD_INFO__ , __SELF_INFO__) \
{m_pLabelName->setString(__GUILD_INFO__->getName().c_str());\
	m_pLabelLevel->setString(D_CSTRING_FROM_UINT(__GUILD_INFO__->getLevel()));\
	unsigned int __EXP__ = __GUILD_INFO__->getContributionLevel();\
	unsigned int __NEXT_EXP__ = __GUILD_INFO__->getContributionNext();\
	if (__NEXT_EXP__ > 0){\
		m_pLabelNextExp->setString(CCString::createWithFormat("%u/%u", __EXP__, __NEXT_EXP__)->getCString());\
		m_pExpBar->setPercent((float)__EXP__ / __NEXT_EXP__, .0f);}else{\
		m_pLabelNextExp->setString("--/--");\
		m_pExpBar->setPercent(1.0, .0f);}\
	m_pLabelExp->setString(D_CSTRING_FROM_UINT(__SELF_INFO__->getNextExp()));\
	m_pLabelPosition->setString(__SELF_INFO__->getKnighthoodName().c_str());}


class GuildHead
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
public:
    CREATE_FUNC(GuildHead);
    GuildHead();
    virtual ~GuildHead();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
private:
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelPosition;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelExp;
	DProgressBar* m_pExpBar;				/*经验条*/
	cocos2d::CCLabelBMFont *m_pLabelNextExp;

	cocos2d::CCLabelTTF *m_pLabelNotice0;
	cocos2d::CCLabelTTF *m_pLabelNotice1;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice0;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice1;
	//cocos2d::CCLabelTTF *m_pLabelNotice2;
public:

#pragma mark setter

	void setName(const char *sName);
	void setKnighthood(const char *sPosition);
	void setLevel(uint32_t iLevel);
	void setGuildExp(uint32_t exp, uint32_t next_exp, float dur = .2f);
	void setUserExp(uint32_t next_exp);
	void setNotice(CCArray *pNotice);
};


class GuildHeadLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildHeadLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildHead);
};

#endif /* defined(__QSMY__GuildHead__) */

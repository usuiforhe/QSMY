//
//  GuildQuestionView.h
//  公会问答
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildQuestionView.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "effect/GuildExpEffect.h"

using namespace sp;

GuildQuestionView::GuildQuestionView()
	: m_pButtonYes(NULL)
	, m_pButtonNo(NULL)
	, m_pButtonCancel(NULL)
	, m_pLabelQuestion(NULL)
	, m_pLabelTime(NULL)
	, m_pModel(NULL)
	, m_fTime(.0f)
	, m_pLabelAnswer0(NULL)
	, m_pLabelAnswer1(NULL)
	, m_pDelegate(NULL)
	, m_isExpShown(false)
	, m_nDropID(0)
	,m_pLabelName(NULL),
	m_pLabelPosition(NULL),
	m_pLabelLevel(NULL),
	m_pLabelExp(NULL),
	m_pExpBar(NULL),
	m_pLabelNextExp(NULL),
	m_pRichLabelNotice0(NULL),
	m_pRichLabelNotice1(NULL),
	m_pRichLabelNotice2(NULL)
{
}

GuildQuestionView::~GuildQuestionView()
{
	CC_SAFE_RELEASE(m_pButtonYes);
	CC_SAFE_RELEASE(m_pButtonNo);
	CC_SAFE_RELEASE(m_pButtonCancel);
	CC_SAFE_RELEASE(m_pLabelQuestion);
	CC_SAFE_RELEASE(m_pLabelTime);
	CC_SAFE_RELEASE(m_pLabelAnswer0);
	CC_SAFE_RELEASE(m_pLabelAnswer1);
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelPosition);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelExp);
	CC_SAFE_RELEASE(m_pExpBar);
	CC_SAFE_RELEASE(m_pLabelNextExp);
	CC_SAFE_RELEASE(m_pRichLabelNotice0);
	CC_SAFE_RELEASE(m_pRichLabelNotice1);
	CC_SAFE_RELEASE(m_pRichLabelNotice2);
}

GuildQuestionView* GuildQuestionView::create()
{
	GuildQuestionView *pRet = new GuildQuestionView();
	if (pRet && pRet->init())
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool GuildQuestionView::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("GuildHead", GuildHeadLoader::loader());


	m_pModel = DM_GET_GUILD_MODEL;
	//CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/guild/guildBook", pNodeLoaderLibrary);
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_dati", pNodeLoaderLibrary);
	m_pLabelQuestion->setString("");
	m_pButtonYes->getButton()->setTarget(this, menu_selector(GuildQuestionView::callBackYes));
	m_pButtonNo->getButton()->setTarget(this, menu_selector(GuildQuestionView::callBackNo));
	m_pButtonCancel->setTarget(this, menu_selector(GuildQuestionView::callBackCancel));


	return true;
}

void GuildQuestionView::onEnter()
{
	DLayer::onEnter();
	m_nDropID = 0;
	TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	BINDING_EXEC(m_pModel, this, GuildQuestionView::updateView, D_EVENT_GUILD_GET);
	BINDING_EXEC(m_pModel, this, GuildQuestionView::updateNotice, D_EVENT_GUILD_GET_NOTICE);
	m_pModel->syncFromServer();
	m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeAnswer);
}

void GuildQuestionView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	m_isExpShown = false;
}

bool GuildQuestionView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_yes", DButton *, m_pButtonYes);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_no", DButton *, m_pButtonNo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_cancel", CCMenuItem *, m_pButtonCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_question", CCLabelTTF *, m_pLabelQuestion);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_time", CCLabelBMFont *, m_pLabelTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_answer0", CCLabelTTF *, m_pLabelAnswer0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_answer1", CCLabelTTF *, m_pLabelAnswer1);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_position", CCLabelTTF *, this->m_pLabelPosition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp", CCLabelBMFont *, this->m_pLabelExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp_bar", DProgressBar *, this->m_pExpBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_next_exp", CCLabelBMFont *, this->m_pLabelNextExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_0", CCRichLabelTTF *, this->m_pRichLabelNotice0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_1", CCRichLabelTTF *, this->m_pRichLabelNotice1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_2", CCRichLabelTTF *, this->m_pRichLabelNotice2);
	return false;
}

void GuildQuestionView::updateQuestion()
{
	if (0 != m_pModel->getSelfInfo()->getQuestionID())
	{
		m_pLabelQuestion->setString(m_pModel->getGuildQuestion()->questionStr.c_str());
		m_pLabelAnswer0->setString(m_pModel->getGuildQuestion()->answer1Str.c_str());
		m_pLabelAnswer1->setString(m_pModel->getGuildQuestion()->answer2Str.c_str());
		m_pButtonYes->getButton()->setEnabled(true);
		m_pButtonNo->getButton()->setEnabled(true);
	}
	else
	{
		m_pLabelQuestion->setString("");
		m_pLabelAnswer0->setString("");
		m_pLabelAnswer1->setString("");
		m_pButtonYes->getButton()->setEnabled(false);
		m_pButtonNo->getButton()->setEnabled(false);
	}
	int iTime = TimeUtil::getDiff(m_pModel->getSelfInfo()->getLastAnswer() , GameManager::sharedGameManager()->getServerTime());
	if (iTime < 0)
	{
#if QSMY_DEBUG
		CCLOG("Answer:%u	now:%u", m_pModel->getSelfInfo()->getLastAnswer() , GameManager::sharedGameManager()->getServerTime());
#endif
		iTime = 0;
	}
	if (iTime > K_GUILD_QUESTION_INTERVAL)
	{
		iTime = 0;
	}
	else
	{
		iTime = K_GUILD_QUESTION_INTERVAL - iTime;
	}
	m_fTime = iTime;
}

void GuildQuestionView::updateView()
{
	guild::GuildInfo *guildInfo = m_pModel->getGuildInfo();
	guild::GuildUser *selfInfo = m_pModel->getSelfInfo();
	if(guildInfo && selfInfo) GUILD_HEAD_UPDATE(guildInfo , selfInfo);
	/*m_pGuildHead->setName(guildInfo->getName().c_str());
	m_pGuildHead->setKnighthood(selfInfo->getKnighthoodName().c_str());
	m_pGuildHead->setLevel(guildInfo->getLevel());
	if (m_isExpShown)
	{
		m_pGuildHead->setGuildExp(guildInfo->getContributionLevel(), guildInfo->getContributionNext(), .2f);
	}
	else
	{
		m_pGuildHead->setGuildExp(guildInfo->getContributionLevel(), guildInfo->getContributionNext(), .0f);
		m_isExpShown = true;
	}
	m_pGuildHead->setUserExp(selfInfo->getNextExp());*/
	updateQuestion();
}

void GuildQuestionView::updateNotice()
{
	//m_pGuildHead->setNotice(m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeAnswer));
	m_pRichLabelNotice0->setString("");
	m_pRichLabelNotice1->setString("");
	m_pRichLabelNotice2->setString("");
	CCArray *tmpArray = m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeNone);
	if (!tmpArray)
	{
		return;
	}
	CCArray *noticeArray = CCArray::create();
	for (unsigned int i = 0; i < NOTICE_MAX && i < tmpArray->count(); i++)
	{
		noticeArray->addObject(tmpArray->objectAtIndex(i));
	}
	if (noticeArray && noticeArray->count() > 0)
	{
		noticeArray->reverseObjects();
		guild::GuildNotice *notice0 = (guild::GuildNotice *)noticeArray->objectAtIndex(0);
		m_pRichLabelNotice0->setString(notice0->getMarquee().c_str());
		if (noticeArray->count() > 1)
		{
			guild::GuildNotice *notice1 = (guild::GuildNotice *)noticeArray->objectAtIndex(1);
			m_pRichLabelNotice1->setString(notice1->getMarquee().c_str());
			if (noticeArray->count() > 2)
			{
				guild::GuildNotice *notice2 = (guild::GuildNotice *)noticeArray->objectAtIndex(2);
				m_pRichLabelNotice2->setString(notice2->getMarquee().c_str());
			}
		}
	}
}

void GuildQuestionView::update(float dt)
{
	m_fTime -= dt;
	if (m_fTime < 0.f)
	{
		m_fTime = 0.f;
	}

	if (D_FLOAT_EQUALS(m_fTime, 0.f))
	{
		m_pLabelTime->setVisible(false);
		HTTP_CLIENT->guildGet();
		m_fTime = K_GUILD_QUESTION_INTERVAL;
	}
	else
	{
		m_pLabelTime->setVisible(true);
		m_pLabelTime->setString(TimeUtil::getSeconds2String(m_fTime, "%H:%M:%S"));
	}
}

void GuildQuestionView::callBackYes(cocos2d::CCObject *sender)
{
	if (!m_pModel->getSelfInfo()->getQuestionID())
	{
		return;
	}
	HTTP_CLIENT->guildQues(2, this, callfuncJson_selector(GuildQuestionView::callBackAnswer));
}

void GuildQuestionView::callBackNo(cocos2d::CCObject *sender)
{
	if (!m_pModel->getSelfInfo()->getQuestionID())
	{
		return;
	}
	HTTP_CLIENT->guildQues(1, this, callfuncJson_selector(GuildQuestionView::callBackAnswer));
}

void GuildQuestionView::callBackCancel(cocos2d::CCObject *sender)
{
	if (m_pDelegate)
	{
		m_pDelegate->willCloseQuestionView(m_nDropID);
	}
}

void GuildQuestionView::callBackAnswer(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;

	int res = responseData["data"]["res"].asInt();
	m_nDropID = 0;
	if (1 == res)
	{
// 		GuildExpEffect* pEffect = GuildExpEffect::create(m_pModel->getSelfInfo()->getContributionAdded(), m_pModel->getGuildInfo()->getContributionAdded());
// 		pEffect->setCallBack(this,callfuncO_selector(GuildQuestionView::callBackShowExp));
// 		Game::sharedGame()->playEffect(pEffect,true);

		m_nDropID = requestData[UUID].asUInt();

		Armature* pArmature =  GuildExpEffect::getArmature(m_pModel->getGuildInfo()->getContributionAdded(), m_pModel->getSelfInfo()->getContributionAdded());
		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildQuestionView::callBackShowExp), COMPLETE);

		addChild(pArmature); 

		
	}
	else if (2 == res)
	{
		Tips_Alert(D_LOCAL_STRING("GuildWrongAnswer").c_str());
        this->updateView();
        if (m_pDelegate)
        {
            m_pDelegate->willCloseQuestionView();
        }
	}
	//this->updateQuestion();
}

void GuildQuestionView::callBackShowExp(Armature * target, const char* event,const char* data)
{
	if (m_nDropID)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(m_nDropID));
		m_nDropID = 0;
	}
	target->removeFromParent();
	this->updateView();
	if (m_pDelegate)
	{
		m_pDelegate->willCloseQuestionView();
	}
}

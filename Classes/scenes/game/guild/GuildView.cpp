//
//  GuildView.h
//  公会主页
//
//  Created by Eci on 13-10-21.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "GuildView.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "GuildNoticeView.h"
#include "effect/GuildExpEffect.h"
#include "GuildCell.h"
#include "utils/TimerUtil.h"

using namespace sp;

#define CELL_MAX 5
#define CELL_ENABLED 4
#define GUILD_VIEW_CELL_SIZE CCSizeMake(210, 400)
//#define NOTICE_MAX	3

//const char* kPageFileN[] = {"ui/guild/page1_n.png", "ui/guild/page2_n.png", 
//							"ui/guild/page3_n.png", "ui/guild/page4_n.png",
//							"ui/guild/page5_n.png"};
const char* kPageFileN[] = {"PL_ui/banghui/banghui_tu_qiandao.png", "PL_ui/banghui/banghui_tu_jianshe.png", 
	"PL_ui/banghui/banghui_tu_dati.png", "PL_ui/banghui/banghui_tu_shop.png",
	"PL_ui/banghui/banghui_tu_qidai.png"};

GuildView::GuildView():
m_pModel(NULL),
m_pLabelName(NULL),
m_pLabelPosition(NULL),
m_pLabelLevel(NULL),
m_pLabelExp(NULL),
m_pExpBar(NULL),
m_pLabelNextExp(NULL),
//m_pLabelNotice0(NULL),
//m_pLabelNotice1(NULL),
//m_pLabelNotice2(NULL),
m_pRichLabelNotice0(NULL),
m_pRichLabelNotice1(NULL),
m_pRichLabelNotice2(NULL),
m_pLabelAnnouncement(NULL),
m_pMarquee(NULL),
m_pButtonEditAnnouncement(NULL),
m_pButtonManage(NULL),
m_pButtonRank(NULL),
m_pBtnShop( NULL ),
m_pDelegate(NULL)
,m_pContainer(NULL)
,m_pListView(NULL)
,m_pNodeRequestNum(NULL)
// ,m_pQuestionView(NULL)
// ,m_pTreeView(NULL)
,m_fTime(.0f)
,m_fQuetionTime(.0f)
{
}

GuildView::~GuildView()
{
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelPosition);
	CC_SAFE_RELEASE(m_pLabelLevel);
	CC_SAFE_RELEASE(m_pLabelExp);
	CC_SAFE_RELEASE(m_pExpBar);
	CC_SAFE_RELEASE(m_pLabelNextExp);
	//CC_SAFE_RELEASE(m_pLabelNotice0);
	//CC_SAFE_RELEASE(m_pLabelNotice1);
	//CC_SAFE_RELEASE(m_pLabelNotice2);
	CC_SAFE_RELEASE(m_pRichLabelNotice0);
	CC_SAFE_RELEASE(m_pRichLabelNotice1);
	CC_SAFE_RELEASE(m_pRichLabelNotice2);
	CC_SAFE_RELEASE(m_pLabelAnnouncement);
	CC_SAFE_RELEASE(m_pButtonEditAnnouncement);
	CC_SAFE_RELEASE(m_pButtonManage);
	CC_SAFE_RELEASE(m_pButtonRank);
	CC_SAFE_RELEASE( m_pBtnShop ) ;
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pListView);
    CC_SAFE_RELEASE(m_pNodeRequestNum);
// 	CC_SAFE_RELEASE(m_pQuestionView);
// 	CC_SAFE_RELEASE(m_pTreeView);
}

GuildView* GuildView::create()
{
	GuildView *pRet = new GuildView();
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

bool GuildView::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILD_MODEL;

	//CCB_READER_AND_ADDCHILD("ui/ccb/guild/guild");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_dating");

	m_pLabelAnnouncement->setString("");

	m_pButtonEditAnnouncement->setTarget(this, menu_selector(GuildView::callBackEditAnnouncement));
	m_pButtonManage->setTarget(this, menu_selector(GuildView::callBackManage));
	m_pButtonRank->setTarget(this, menu_selector(GuildView::callBackRank));
	m_pBtnShop->setTarget(this, menu_selector(GuildView::callBackShop));

	m_pListView = DTableView::create(this, m_pContainer->getContentSize());
	m_pListView->setDirection(kCCScrollViewDirectionHorizontal);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->retain();
	m_pContainer->addChild(m_pListView);
	m_pExpBar->setPercent(0, .0f);


	/*if (NULL == m_pRichLabelNotice0 && NULL == m_pRichLabelNotice1 && NULL == m_pRichLabelNotice2)
	{

	m_pRichLabelNotice0 = CCRichLabelTTF::create();
	m_pRichLabelNotice1 = CCRichLabelTTF::create();
	m_pRichLabelNotice2 = CCRichLabelTTF::create();

	CC_SAFE_RETAIN(m_pRichLabelNotice0);
	CC_SAFE_RETAIN(m_pRichLabelNotice1);
	CC_SAFE_RETAIN(m_pRichLabelNotice2);

	replaceLabel(m_pLabelNotice0,m_pRichLabelNotice0);
	replaceLabel(m_pLabelNotice1,m_pRichLabelNotice1);
	replaceLabel(m_pLabelNotice2,m_pRichLabelNotice2);
	m_pLabelNotice0->setString("");
	m_pLabelNotice1->setString("");
	m_pLabelNotice2->setString("");
	}*/
	return true;
}

void GuildView::onEnter()
{
	DLayer::onEnter();

	if (m_pMarquee)
	{
		m_pMarquee->setStack(0);				//remove all text
		m_pMarquee->setFirst(true);
	}

    
    m_pModel->syncGuildApplyList();

    
    BINDING_ONLY(m_pModel, this, GuildView::updateView, D_EVENT_GUILD_APPLY_LIST);    
	BINDING_EXEC(m_pModel, this, GuildView::updateView, D_EVENT_GUILD_GET);
	BINDING_EXEC(m_pModel, this, GuildView::updateNoticeView, D_EVENT_GUILD_GET_NOTICE);

	//m_pModel->getGuildNoticeByType(guild::GuildNoticeTypeNone);

}

CCString * GuildView::getMarqueeText()
{
	if (m_pModel->getGuildInfo())
	{
		return CCString::create(m_pModel->getGuildInfo()->getAnnouncement());
	}
	else
	{
		return CCString::create("");
	}
}

void GuildView::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	unschedule(schedule_selector(GuildView::updateQuestion));
}

void GuildView::update(float delta)
{
	m_fTime -= delta;
	if (m_fTime < 0.f)
	{
		m_fTime = 0.f;
		m_pModel->getSelfInfo()->setIsSigned(false);
		this->updateView();
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	}
	GuildCell *cell = (GuildCell*)m_pListView->cellAtIndex( kGuildSign ) ;
	if ( cell && m_pModel->getSelfInfo()->getIsSigned() )
	{
		cell->setTime(m_fTime);
	}
	
}

void GuildView::updateQuestion(float dt)
{
	m_fQuetionTime -= dt;
	if (m_fQuetionTime < .0f)
	{
		m_fQuetionTime = 0;
		HTTP_CLIENT->guildGet();
		unschedule(schedule_selector(GuildView::updateQuestion));
	}
	GuildCell *cell = (GuildCell*)m_pListView->cellAtIndex( kGuildQuestion ) ;
	if ( cell )
	{
		if (m_pModel->getSelfInfo()->getQuestionID() == 0)
		{
			cell->setTime(m_fQuetionTime);
		}
		else
		{
			cell->setTime(0);
		}
	}
}

void GuildView::updateViewWithDurOfPB(float duration)
{
	guild::GuildInfo *guildInfo = m_pModel->getGuildInfo();
	guild::GuildUser *selfInfo = m_pModel->getSelfInfo();
	if(guildInfo && selfInfo) GUILD_HEAD_UPDATE(guildInfo , selfInfo);

	if (NULL == m_pMarquee)
	{
		m_pMarquee = DMarqueeView::create(m_pLabelAnnouncement->getDimensions().width, ccc3(0x00, 0x00, 0x00), D_FONT_DEFAULT, NOTICE_FONT_SIZE);
		m_pMarquee->setDataSource(this);
		m_pLabelAnnouncement->addChild(m_pMarquee);
	}
	//是否可以编辑公告
	if (m_pModel->getSelfInfo()->getPosition() == guild::GuildPositionAssistant || m_pModel->getSelfInfo()->getPosition() == guild::GuildPositionPresident)
	{
	 	m_pButtonEditAnnouncement->setEnabled(true);
	}
	else
	{
	 	m_pButtonEditAnnouncement->setEnabled(false);
	}
	m_fTime = TimeUtil::getDiff(GameManager::sharedGameManager()->getServerTime() , TimeUtil::getTimeToday(TimeUtil::getStartTimeOfTomorrow(GameManager::sharedGameManager()->getServerTime()),0));
	
	if (m_pModel->getSelfInfo()->getIsSigned())
	{
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
		TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	}
	
	if (m_pModel->getSelfInfo()->getQuestionID())
	{
		m_fQuetionTime = K_GUILD_QUESTION_INTERVAL;
	}
	else
	{
		int diff = TimeUtil::getDiff(m_pModel->getSelfInfo()->getLastAnswer() , GameManager::sharedGameManager()->getServerTime());
		unschedule(schedule_selector(GuildView::updateQuestion));
		if (diff < 0)
		{
#if QSMY_DEBUG
			CCLOG("Answer:%u	now:%u", m_pModel->getSelfInfo()->getLastAnswer() , GameManager::sharedGameManager()->getServerTime());
#endif
		}
		else if (diff > K_GUILD_QUESTION_INTERVAL)
		{
			m_fQuetionTime = 0;
			updateQuestion(1);
		}
		else
		{
			m_fQuetionTime = K_GUILD_QUESTION_INTERVAL - diff;
			schedule(schedule_selector(GuildView::updateQuestion));
		}
	}
}

void GuildView::updateView()
{
    uint32_t count  = 0;
    if(m_pModel->getGuildApplication())
        count = m_pModel->getGuildApplication()->count();
    
//    GuildPositionPresident,
	//m_pNodeRequestNum->setVisible(count > 0 && (m_pModel->getSelfInfo()->getPosition()==guild::GuildPositionAssistant||m_pModel->getSelfInfo()->getPosition()==guild::GuildPositionPresident));
	updateViewWithDurOfPB(.2f);
}

void GuildView::updateNoticeView()
{
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

bool GuildView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_name", CCLabelTTF *, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_level", CCLabelBMFont *, this->m_pLabelLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_position", CCLabelTTF *, this->m_pLabelPosition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp", CCLabelBMFont *, this->m_pLabelExp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_exp_bar", DProgressBar *, this->m_pExpBar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_next_exp", CCLabelBMFont *, this->m_pLabelNextExp);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_0", CCRichLabelTTF *, this->m_pRichLabelNotice0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_1", CCRichLabelTTF *, this->m_pRichLabelNotice1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notice_2", CCRichLabelTTF *, this->m_pRichLabelNotice2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_announce", CCRichLabelTTF *, this->m_pLabelAnnouncement);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_edit_announce", CCMenuItem *, this->m_pButtonEditAnnouncement);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_manage", CCMenuItem *, this->m_pButtonManage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_rank", CCMenuItem *, this->m_pButtonRank);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_shop", CCMenuItem *, this->m_pBtnShop);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRequestNum", CCNode *, m_pNodeRequestNum);


	return false;
}

SEL_MenuHandler GuildView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName )
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE( this, "pageCallBack",GuildView::pageCallBack );
	return NULL ;
}

void GuildView::pageCallBack(cocos2d::CCObject *sender)
{
	CCNode* pMenu = dynamic_cast<CCNode*>(sender);
	int tag = pMenu->getTag();
	int delta = tag == 0 ? -1 : 1;
	
	CCPoint offset = m_pListView->getContentOffset();
	int index = ((int)offset.x)/(int)GUILD_VIEW_CELL_SIZE.width;
	index -= delta;
	if(index <= 0 && index >= -2)
	{
		//不能超限
		m_pListView->setContentOffsetInDuration(ccp(index * GUILD_VIEW_CELL_SIZE.width , offset.y) , 0.1f);
	}
}

void GuildView::callBackEditAnnouncement(CCObject *sender)
{

// 	if (m_pModel->getSelfInfo()->getPosition() != guild::GuildPositionPresident && m_pModel->getSelfInfo()->getPosition() != guild::GuildPositionAssistant)
// 	{
// 		Tips_Alert(D_LOCAL_STRING("GuildAnnouncementError").c_str());
// 		return;
// 	}
	if (m_pDelegate)
	{
		m_pDelegate->willShowNoticeView();
	}
// 	GuildNoticeView *noticeView = GuildNoticeView::create();
// 	this->addChild(noticeView);
}

void GuildView::callBackManage(CCObject *sender)
{
	m_pDelegate->willShowManageView();
}

void GuildView::callBackRank(CCObject *sender)
{
	m_pDelegate->willShowRankView();
}

void GuildView::callBackShop(CCObject *sender)
{
	m_pDelegate->willShowMallView();
}

CCTableViewCell* GuildView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildCell *cell = (GuildCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildCell::create();
	}
	cell->setFileName(kPageFileN[idx]);
	cell->setEnabled(idx < CELL_ENABLED);
	if (idx == kGuildSign && m_pModel->getSelfInfo()->getIsSigned())
	{
		cell->setTime(m_fTime);
	}
	else if (idx == kGuildQuestion && m_pModel->getSelfInfo()->getQuestionID() == 0)
	{
		cell->setTime(m_fQuetionTime);
	}
	else
	{
		cell->setTime(0);
	}
	return cell;
}

uint32_t GuildView::numberOfCellsInTableView(CCTableView *table)
{
	return CELL_MAX;
}


CCSize GuildView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return GUILD_VIEW_CELL_SIZE;
}

void GuildView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if (this->isVisible() == false)
	{
		return;
	}
	switch( cell->getIdx() )
	{
		case kGuildShop :
		{
			if (m_pDelegate)
			{
				m_pDelegate->willShowMallView();
			}
			break ;
		}

		case kGuildTree :
		{
			if (m_pDelegate)
			{
				m_pDelegate->willShowTreeView();
			}
			break ;
		}

		case kGuildSign :
		{
			if (m_pModel->getSelfInfo()->getIsSigned())
			{
				Tips_Alert(D_LOCAL_STRING("GuildSignError").c_str());
				return;
			}
			HTTP_CLIENT->guildSign(this, callfuncJson_selector(GuildView::callBackSign)) ;
			break ;
		}

		case kGuildQuestion :
		{
			if (m_pModel->getSelfInfo()->getQuestionID() == 0)
			{
				Tips_Alert(D_LOCAL_STRING("GuildQuestionError").c_str());
				return;
			}
			if (m_pDelegate)
			{
				m_pDelegate->willShowQuestionView();
			}
			break ;
		}
	}
}


void GuildView::callBackSign(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
// 	GuildExpEffect* pEffect = GuildExpEffect::create(m_pModel->getSelfInfo()->getContributionAdded(), m_pModel->getGuildInfo()->getContributionAdded());
// 	pEffect->setCallBack(this,callfuncO_selector(GuildView::callBackShowExp));
// 	Game::sharedGame()->playEffect(pEffect,true);

	TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);

	Armature* pArmature =  GuildExpEffect::getArmature(m_pModel->getGuildInfo()->getContributionAdded(), m_pModel->getSelfInfo()->getContributionAdded());
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildView::callBackShowExp), COMPLETE);

	addChild(pArmature); 
}

void GuildView::callBackShowExp(Armature * target, const char* event,const char* data)
{
	target->removeFromParent();
	this->updateView();
}

// void GuildView::willCloseQuestionView()
// {
// 	m_pQuestionView->removeFromParent();
// 	this->updateView();
// }
// 
// void GuildView::willCloseTreeView()
// {
// 	m_pTreeView->removeFromParent();
// 	this->updateView();
// }
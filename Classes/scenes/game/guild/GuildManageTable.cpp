//
//  GuildManageTable.h
//  公会管理
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "GuildManageTable.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "GuildManageCell.h"
#include "model/OtherFormationModel.h"
#include "../common/OtherFormationView.h"
#include <algorithm>
#include "utils/TimerUtil.h"

#define UNSELECT -1


static int sortUserByLevel(const CCObject *p1, const CCObject *p2)
{
	return ((guild::GuildUserBasics*)p1)->getLevel() > ((guild::GuildUserBasics*)p2)->getLevel();
}

GuildManageTable::GuildManageTable()
:m_pModel(NULL)
,m_pMessageView(NULL)
,m_pListView(NULL)
,m_pUserList(NULL)
,m_iSelectIndex(UNSELECT)
,m_iType(0)
,m_iUserID(0)
,m_iPosition(guild::GuildPositionNone)
,m_pDelegate(NULL)
,m_bGuildBossSyn(false)
,m_fWaitTime(0.f)
{
}

GuildManageTable::~GuildManageTable()
{
	CC_SAFE_RELEASE(m_pMessageView);
	CC_SAFE_RELEASE(m_pListView);
	CC_SAFE_RELEASE(m_pUserList);
}

GuildManageTable* GuildManageTable::create(const cocos2d::CCSize tableSize)
{
	GuildManageTable *pRet = new GuildManageTable();
	if (pRet && pRet->init(tableSize))
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

bool GuildManageTable::init(const cocos2d::CCSize tableSize)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pModel = DM_GET_GUILD_MODEL;
    
	m_pUserList = CCArray::create();
	m_pUserList->retain();
    
	m_pListView = DTableView::create(this, tableSize);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->retain();
	this->addChild(m_pListView);
    
    
	return true;
}

void GuildManageTable::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pModel, this, GuildManageTable::updateView, D_EVENT_GUILD_GET);
	BINDING_ONLY(DM_GET_GUILDBOSS_MODEL, this, GuildManageTable::guildBossSynchro, D_EVENT_GUILDBOSS_GET_RANK);
	if( DM_GET_GUILDBOSS_MODEL->getIsOpen() && DM_GET_GUILDBOSS_MODEL->getConfig()->getStartTime() < GameManager::sharedGameManager()->getServerTime() && !m_bGuildBossSyn)//如果公会战已经开启 并且时间在每日中午12点以后 则去请求公会战的数据
	{
		HTTP_CLIENT->guildBossMemberRank(m_pModel->getGuildInfo()->getGuildID());		
	}
	if (m_pModel->getGuildUser()->count() == 0)
	{
		HTTP_CLIENT->guildGet();
	}
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(GuildManageTable::guildGet), this, 3600.f);
}

void GuildManageTable::scheduleGuilBossIfNeed()
{
	m_fWaitTime = DM_GET_GUILDBOSS_MODEL->getCountingDownOfLivingPoint();
	if( m_fWaitTime > 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildManageTable::updateGuilBossDeadLine), this);
		TimerUtil::sharedTimerUtil()->scheduleSelector(schedule_selector(GuildManageTable::updateGuilBossDeadLine), this, 1,kCCRepeatForever,m_fWaitTime,false);
	}
}

void GuildManageTable::updateGuilBossDeadLine(float dt)
{
	m_fWaitTime -= dt;
	if (m_fWaitTime < 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildManageTable::updateGuilBossDeadLine), this);
		HTTP_CLIENT->guildBossMemberRank(m_pModel->getGuildInfo()->getGuildID());
	}
}

void GuildManageTable::guildGet(float dt)
{
	HTTP_CLIENT->guildGet();
}

void GuildManageTable::guildBossSynchro()
{
	m_bGuildBossSyn = true;
	m_pListView->reloadData();
}

void GuildManageTable::updateView()
{
	m_pUserList->removeAllObjects();
    
	CCArray *m_pAssistantList = CCArray::create();
	CCArray *m_pMemberList = CCArray::create();
    
	for (uint32_t i = 0; i < m_pModel->getGuildUser()->count(); i++)
	{
		guild::GuildUserBasics *pUser = (guild::GuildUserBasics *)m_pModel->getGuildUser()->objectAtIndex(i);
		if (pUser->getPosition() == guild::GuildPositionPresident)
		{
			m_pUserList->addObject(pUser);
		}
		else if (pUser->getPosition() == guild::GuildPositionAssistant)
		{
			m_pAssistantList->addObject(pUser);
		}
		else
		{
			m_pMemberList->addObject(pUser);
		}
	}
    
	std::sort(m_pAssistantList->data->arr, m_pAssistantList->data->arr + m_pAssistantList->data->num, sortUserByLevel);
	std::sort(m_pMemberList->data->arr, m_pMemberList->data->arr + m_pMemberList->data->num, sortUserByLevel);
	
	m_pUserList->addObjectsFromArray(m_pAssistantList);
	m_pUserList->addObjectsFromArray(m_pMemberList);
    
	m_pListView->reloadData();

	scheduleGuilBossIfNeed();
}

void GuildManageTable::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	UNBINDING_ALL(DM_GET_GUILDBOSS_MODEL, this);
	TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
}

CCTableViewCell* GuildManageTable::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildManageCell *cell = (GuildManageCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildManageCell::create();
		cell->setDelegate(this);
		cell->getSelectButton()->setTarget(this, menu_selector(GuildManageTable::onTouch));
	}
	ValueObjcet *pData = (ValueObjcet *)m_pUserList->objectAtIndex(idx);
	cell->setData(pData);
	cell->getSelectButton()->setUserData(cell);
	cell->setExpand(idx == m_iSelectIndex);
	return cell;
}

uint32_t GuildManageTable::numberOfCellsInTableView(CCTableView *table)
{
	return m_pUserList->count();
}


CCSize GuildManageTable::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	if (idx == m_iSelectIndex)
	{
		return MANAGE_CELL_SIZE_EXPAND;
	}
	else
	{
		return MANAGE_CELL_SIZE;
	}
}

void GuildManageTable::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void GuildManageTable::setType(uint32_t iType)
{
	if(iType == m_iType)
	{
		return;
	}
	m_iType = iType;
	m_pListView->reloadData();
}


void GuildManageTable::onTouch(cocos2d::CCObject* obj)
{
	cocos2d::CCMenuItem* pItem = static_cast<cocos2d::CCMenuItem*>(obj);
    
	GuildManageCell* pCell = static_cast<GuildManageCell*>(pItem->getUserData());
	if(!pCell) return;
	//CCLOG("equip cell touched at index: %i,%i", idx,pCell->getIdx());
	if(m_iSelectIndex == pCell->getIdx())
	{
		m_iSelectIndex = - 1;
		m_pListView->updateCellPositions(pCell);
		m_pListView->reloadData();
	}
	else
	{
		m_iSelectIndex = pCell->getIdx();
		m_pListView->updateCellPositions(pCell);
		m_pListView->reloadData();
	}
}


void GuildManageTable::willChangePosition(uint32_t userID, guild::GuildPosition position)
{
	if (guild::GuildPositionAssistant == position)
	{
		guild::GuildInfo *guildInfo = m_pModel->getGuildInfo();
		if (0 != guildInfo->getAssistant1() && 0 != guildInfo->getAssistant2() && 0 != guildInfo->getAssistant3())
		{
			Tips_Alert(D_LOCAL_STRING("GuildAddAssistantError").c_str());
			return;
		}
	}
	m_iUserID = userID;
	m_iPosition = position;
	switch (position)
	{
        case guild::GuildPositionAssistant:
            DMessageBox::showMessageBox(D_LOCAL_STRING("GuildAssistantConfirm").c_str(), this, messageBox_callfunc_selector(GuildManageTable::willChangePosition));
			break;
        case guild::GuildPositionMember:
            DMessageBox::showMessageBox(D_LOCAL_STRING("GuildMemberConfirm").c_str(), this, messageBox_callfunc_selector(GuildManageTable::willChangePosition));
            break;
        case guild::GuildPositionRemoved:
            DMessageBox::showMessageBox(D_LOCAL_STRING("GuildRemoveConfirm").c_str(), this, messageBox_callfunc_selector(GuildManageTable::willChangePosition));
            break;
        case guild::GuildPositionPresident:
        default:
            break;
	}
}

void GuildManageTable::willChangePosition(MessageType type,DMessageBox* target)
{
	if(type == kMessageTypeCancel) return;
	HTTP_CLIENT->guildPosition(m_iUserID, m_iPosition);
}

void GuildManageTable::willChangePosition(const Json::Value &,const Json::Value &)
{
	m_iSelectIndex = -1;
}

void GuildManageTable::willReplaceBoss(){
    bool canReplaceBoss = m_pModel->canReplaceBoss();
    if(canReplaceBoss){
        if(m_pModel->guildUserCanReplace()==1){
            DMessageBox::showMessageBox(D_LOCAL_STRING("ReplaceguildBoss").c_str(), this, messageBox_callfunc_selector(GuildManageTable::guildSubstitute));
        }else if(m_pModel->guildUserCanReplace()==2){
            Tips_Alert(D_LOCAL_STRING("CanReplaceBossByPos").c_str());
        }else if(m_pModel->guildUserCanReplace()==3){
            Tips_Alert(D_LOCAL_STRING("CanReplaceBossByGongxian").c_str());
        }
    }else{
        Tips_Alert(D_LOCAL_STRING("CanReplaceBossByTime").c_str());
    }
}

void GuildManageTable::guildSubstitute(MessageType type,DMessageBox* target){
    if(type == kMessageTypeCancel) return;
	/************************************************************************/
	/* 11.11编译不通过                                                                     */
	/************************************************************************/
    //HTTP_CLIENT->guildSubstitute();
}


void GuildManageTable::willQuitGuild()
{
	DMessageBox::showMessageBox(D_LOCAL_STRING("GuildExitConfirm").c_str(), this, messageBox_callfunc_selector(GuildManageTable::willQuitGuild));
}

void GuildManageTable::willQuitGuild(const Json::Value &,const Json::Value &responseData)
{
    
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	//Game::sharedGame()->NavigateTo(navigation::kNavHome);
    
}

void GuildManageTable::willQuitGuild(MessageType type,DMessageBox* target)
{
	if(type == kMessageTypeCancel) return;
	HTTP_CLIENT->guildQuit(this, callfuncJson_selector(GuildManageTable::willQuitGuild));
}

void GuildManageTable::willDemise()
{
	if (m_pDelegate)
	{
		m_pDelegate->willShowDemiseView();
	}
}

void GuildManageTable::willDissolve()
{
	if (m_pModel->getGuildUser()->count() > 1)
	{
		Tips_Alert(D_LOCAL_STRING("GuildDissolveError").c_str());
		return;
	}
	DMessageBox::showMessageBox(D_LOCAL_STRING("GuildDissolveConfirm").c_str(), this, messageBox_callfunc_selector(GuildManageTable::willDissolve));
}

void GuildManageTable::willDissolve(MessageType type,DMessageBox* target)
{
	if(type == kMessageTypeCancel) return;
	HTTP_CLIENT->guildQuit(this, callfuncJson_selector(GuildManageTable::willQuitGuild));
}

void GuildManageTable::willShowDetails(uint32_t userID)
{
	showOtherFormation(DM_GET_OTHERFORMATION_MODEL->getForamtionInfoByUid(userID));
}

void GuildManageTable::willSendMessage(uint32_t userID)
{
	/*GuildMessageView *messageView = GuildMessageView::create();
     messageView->setDelegate(this);
     addChild(messageView);*/
    
	if (NULL == m_pMessageView)
	{
		m_pMessageView = GuildMessageView::create();
		m_pMessageView->setPopupDelegate(this);
		m_pMessageView->setDelegate(this);
		m_pMessageView->retain();
	}
	//setVisible(false);
	Game::sharedGame()->openPopup(m_pMessageView);
}



void GuildManageTable::showOtherFormation(otherFormation::FormationInfo* pInfo)
{
	if (m_pDelegate)
	{
		unsigned int userid = pInfo->getID();
		FriendFormation* pFormation = FriendFormation::create();
		pFormation->setInfo(userid);
		pFormation->setDelegate(this);
		PL_MOVE_NODE_ABOVE_CENTER_Y(pFormation);
		m_pDelegate->willShowFormation(pFormation);
	}

	/*OtherFormationView *m_pOtherFormationView = OtherFormationView::create();
	m_pOtherFormationView->setPopupDelegate(this);
	setVisible(false);
	m_pOtherFormationView->setFormationInfo(pInfo);
	Game::sharedGame()->openPopup(m_pOtherFormationView);*/
}

void GuildManageTable::onPopupClose(DPopup* pPopup)
{
	setVisible(true);
}


void GuildManageTable::willSendMessage(const char *msg)
{
	if (m_iSelectIndex == UNSELECT)
	{
		return;
	}
	
	guild::GuildUserBasics *pUser = (guild::GuildUserBasics *)m_pUserList->objectAtIndex(m_iSelectIndex);
	HTTP_CLIENT->guildMsg(pUser->getUserId(), msg, this, callfuncJson_selector(GuildManageTable::willSendMessage));
}


void GuildManageTable::willCloseMessage()
{
	m_pMessageView->removeFromParent();
}


void GuildManageTable::willSendMessage(const Json::Value &,const Json::Value &responseData)
{
    
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	m_pMessageView->removeFromParent();
    
}
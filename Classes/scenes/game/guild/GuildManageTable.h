//
//  GuildManageTable.h
//  公会管理
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildManageTable__
#define __QSMY__GuildManageTable__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildManageCell.h"
#include "GuildMessageView.h"
#include "../friend/FriendFormation.h"

class GuildManageTableDelegate
{
public:
	virtual void willShowDemiseView() = 0;
	virtual void willShowFormation(DLayer*) = 0;
};


class GuildManageTable
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
	,public GuildManageCellDelegate
	,public DPopupDelegate
	,public GuildMessageViewDelegate
	,public FriendFormationDelegate
{
public:
	GuildManageTable();
	~GuildManageTable();

	static GuildManageTable* create(const cocos2d::CCSize tableSize);
	virtual bool init(const cocos2d::CCSize tableSize);

	virtual void onEnter();
	virtual void onExit();

#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

#pragma mark setter
	void setType(uint32_t iType);
protected:
	uint32_t m_iType;
	CC_SYNTHESIZE(GuildManageTableDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE(bool , m_bGuildBossSyn, GuildBossSyn);	//是否已经同步了公会战信息
private:
	GuildModel *m_pModel;
	GuildMessageView *m_pMessageView;
	DTableView *m_pListView;

	cocos2d::CCArray *m_pUserList;

	int32_t m_iSelectIndex;

	uint32_t m_iUserID;
	guild::GuildPosition m_iPosition;
	float m_fWaitTime;

	void updateView();
	void onTouch(cocos2d::CCObject*);
	void guildBossSynchro();
	void scheduleGuilBossIfNeed();
	void updateGuilBossDeadLine(float dt);
	void guildGet(float dt);
public:

#pragma mark GuildManageCellDelegate
	virtual void willChangePosition(uint32_t userID, guild::GuildPosition position);
	void willChangePosition(MessageType type,DMessageBox* target);
	void willChangePosition(const Json::Value &,const Json::Value &);
	virtual void willQuitGuild();
    
    void guildSubstitute(MessageType type,DMessageBox* target);
    
    virtual void willReplaceBoss();
    
	void willQuitGuild(const Json::Value &,const Json::Value &);
	void willQuitGuild(MessageType type,DMessageBox* target);
	virtual void willDemise();
	virtual void willDissolve();
	void willDissolve(MessageType type,DMessageBox* target);
	virtual void willShowDetails(uint32_t userID);
	virtual void willSendMessage(uint32_t userID);

#pragma mark DPopupDelegate
	virtual void onPopupClose(DPopup*);
	void showOtherFormation(otherFormation::FormationInfo* pInfo);

#pragma mark GuildMessageViewDelegate
	virtual void willSendMessage(const char *msg);
	void willSendMessage(const Json::Value &,const Json::Value &responseData);
	virtual void willCloseMessage();


};

#endif
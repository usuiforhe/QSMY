
//
//  GuildManageCell.h
//  公会管理
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//




#ifndef __QSMY__GuildManageCell__
#define __QSMY__GuildManageCell__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

#define  MANAGE_CELL_SIZE        CCSizeMake(614, 140)
#define  MANAGE_CELL_SIZE_EXPAND        CCSizeMake(614, 220)

class GuildManageCellDelegate
{
public:
	virtual void willChangePosition(uint32_t userID, guild::GuildPosition position) = 0;
    virtual void willReplaceBoss() = 0;
    virtual void willQuitGuild() = 0;
	virtual void willDemise() = 0;
	virtual void willDissolve() = 0;
	virtual void willShowDetails(uint32_t userID) = 0;
	virtual void willSendMessage(uint32_t userID) = 0;
};

enum GuildManageType
{
	GuildManageTypeNone = 0,
	GuildManagePresident2Self = 1,
	GuildManagePresident2Assistant,
	GuildManagePresident2Member,
	GuildManageAssistant2Self,
	GuildManageAssistant2President,
	GuildManageAssistant2Member,
    GuildManageMember2President,//成员看会长
	GuildManageAssistant2Assistant = GuildManageAssistant2President,
	GuildManageMember2Self = GuildManageAssistant2Self,
	GuildManageMember2Other = GuildManageAssistant2President,
};

class GuildManageCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBSelectorResolver
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildManageCell();
	~GuildManageCell();

	static GuildManageCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

#pragma mark setter

	void setData(ValueObjcet *pData);
	void setType(GuildManageType iType);
	void setExpand(bool isExpanded);
	CC_SYNTHESIZE(GuildManageCellDelegate *, m_pDelegate, Delegate);
	void updateUserJoinGuildBoss();
private:
	cocos2d::CCNode *m_pContainer;
	cocos2d::CCNode *m_pAvatorContainer;
	cocos2d::CCNode *m_pNodePresident;
	cocos2d::CCNode *m_pNodeAssistant;
	cocos2d::CCNode *m_pNodePresident2Self;
	cocos2d::CCNode *m_pNodePresident2Assistant;
	cocos2d::CCNode *m_pNodePresident2Member;
	cocos2d::CCNode *m_pNodeAssistant2Self;
	cocos2d::CCNode *m_pNodeAssistant2President;
	cocos2d::CCNode *m_pNodeAssistant2Member;
    cocos2d::CCNode *m_pNodeAssistant2assistant;

	cocos2d::CCNode *m_pMessageBox;

	cocos2d::CCNode *m_pNotInBoss;
	cocos2d::CCNode *m_pInBoss;
	cocos2d::CCNode *m_pNotOpenBoss;	
	cocos2d::CCRichLabelTTF *m_pLastLoginDesc;

	cocos2d::CCNode *m_pNodeExpand;
	cocos2d::CCLabelTTF *m_pLabelKnighthood; //爵位名称
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelBMFont *m_pLabelContribution;

#pragma mark callback function

	void callBackDemise(cocos2d::CCObject *sender);
	void callBackDissolve(cocos2d::CCObject *sender);
	void callBackAddAssistant(cocos2d::CCObject *sender);
	void callBackRemove(cocos2d::CCObject *sender);
	void callBackDetails(cocos2d::CCObject *sender);
	void callBackMessage(cocos2d::CCObject *sender);
    void callBackReplaceBoss(cocos2d::CCObject *sender);//取代会长
    
	void callBackRemoveAssistant(cocos2d::CCObject *sender);
	void callBackExit(cocos2d::CCObject *sender);
protected:
	GuildManageType m_iType;
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*,m_pSelectButton,SelectButton);

private:
	guild::GuildUserBasics *m_pData;
};


#endif
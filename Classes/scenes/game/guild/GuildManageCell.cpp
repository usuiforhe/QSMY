//
//  GuildManageCell.h
//  公会管理
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildManageCell.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"

GuildManageCell::GuildManageCell()
	:m_pContainer(NULL)
	,m_pAvatorContainer(NULL)
	,m_pNodePresident(NULL)
	,m_pNodeAssistant(NULL)
	,m_pNodePresident2Self(NULL)
	,m_pNodePresident2Assistant(NULL)
	,m_pNodePresident2Member(NULL)
	,m_pNodeAssistant2Self(NULL)
	,m_pNodeAssistant2Member(NULL)
	,m_pNodeAssistant2President(NULL)
    //,m_pNodeAssistant2assistant(NULL)


	,m_pSelectButton(NULL)
	,m_pNodeExpand(NULL)
	,m_pLabelName(NULL)
	//,m_pLabelKnighthood(NULL)
	,m_pLabelContribution(NULL)
	,m_pDelegate(NULL)
	,m_pData(NULL)
	,m_pNotInBoss(NULL)
	,m_pInBoss(NULL)
	,m_pNotOpenBoss(NULL)
	,m_pLastLoginDesc(NULL)
	,m_pMessageBox(NULL)
{
}

GuildManageCell::~GuildManageCell()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pAvatorContainer);
	CC_SAFE_RELEASE(m_pNodePresident);
	CC_SAFE_RELEASE(m_pNodeAssistant);
	CC_SAFE_RELEASE(m_pNodePresident2Self);
	CC_SAFE_RELEASE(m_pNodePresident2Assistant);
	CC_SAFE_RELEASE(m_pNodePresident2Member);
	CC_SAFE_RELEASE(m_pNodeAssistant2Self);
	CC_SAFE_RELEASE(m_pNodeAssistant2Member);
	CC_SAFE_RELEASE(m_pNodeAssistant2President);
    //CC_SAFE_RELEASE(m_pNodeAssistant2assistant);
	CC_SAFE_RELEASE(m_pSelectButton);
	CC_SAFE_RELEASE(m_pNodeExpand);
	CC_SAFE_RELEASE(m_pLabelName);
	//CC_SAFE_RELEASE(m_pLabelKnighthood);
	CC_SAFE_RELEASE(m_pLabelContribution);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pNotInBoss);
	CC_SAFE_RELEASE(m_pInBoss);
	CC_SAFE_RELEASE(m_pNotOpenBoss);
	CC_SAFE_RELEASE(m_pLastLoginDesc);
	CC_SAFE_RELEASE(m_pMessageBox);
}

GuildManageCell* GuildManageCell::create()
{
	GuildManageCell *pRet = new GuildManageCell();
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

bool GuildManageCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_guanli_cell");

	m_pNodePresident2Self->setVisible(false);
	m_pNodePresident2Assistant->setVisible(false);
	m_pNodePresident2Member->setVisible(false);
	m_pNodeAssistant2Self->setVisible(false);
	m_pNodeAssistant2President->setVisible(false);
	m_pNodeAssistant2Member->setVisible(false);
	m_pNotInBoss->setVisible(false);
	m_pInBoss->setVisible(false);
	m_pNotOpenBoss->setVisible(false);
	m_pLastLoginDesc->setVisible(false);
    //m_pNodeAssistant2assistant->setVisible(false);
	return true;
}

void GuildManageCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void GuildManageCell::onExit()
{
	CCTableViewCell::onExit();
}

bool GuildManageCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildPresident", CCNode *, this->m_pNodePresident);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildAssistant", CCNode *, this->m_pNodeAssistant);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildExpand", CCNode *, this->m_pNodeExpand);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRoot", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer", CCNode *, this->m_pAvatorContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_president2self", CCNode *, this->m_pNodePresident2Self);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_president2assistant", CCNode *, this->m_pNodePresident2Assistant);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_president2member", CCNode *, this->m_pNodePresident2Member);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_assistant2self", CCNode *, this->m_pNodeAssistant2Self);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_assistant2president", CCNode *, this->m_pNodeAssistant2President);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_assistant2assistant", CCNode *, this->m_pNodeAssistant2assistant);
    
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_assistant2member", CCNode *, this->m_pNodeAssistant2Member);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildName", CCLabelTTF *, this->m_pLabelName);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_knighthood", CCLabelTTF *, this->m_pLabelKnighthood);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_contribution", CCLabelBMFont *, this->m_pLabelContribution);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notinboos", CCNode *, this->m_pNotInBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_inboos", CCNode *, this->m_pInBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_notopenboos", CCNode *, this->m_pNotOpenBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "guild_lastlogindesc", CCRichLabelTTF *, this->m_pLastLoginDesc);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectButton", CCMenuItem *, this->m_pSelectButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMessageBox" , CCNode*, m_pMessageBox)

	return false;
}


SEL_MenuHandler GuildManageCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackDemise", GuildManageCell::callBackDemise);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackDissolve", GuildManageCell::callBackDissolve);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackAddAssistant", GuildManageCell::callBackAddAssistant);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRemove", GuildManageCell::callBackRemove);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackDetails", GuildManageCell::callBackDetails);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackMessage", GuildManageCell::callBackMessage);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRemoveAssistant", GuildManageCell::callBackRemoveAssistant);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackExit", GuildManageCell::callBackExit);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackReplaceBoss", GuildManageCell::callBackReplaceBoss);
    
	return NULL;
}

void GuildManageCell::updateUserJoinGuildBoss()
{
	switch(m_pData->getGuildJoinBossState())
	{
	case guild::GuildJoinBossHaveJoined:
		m_pInBoss->setVisible(true);
		m_pNotInBoss->setVisible(false);
		m_pNotOpenBoss->setVisible(false);
		break;
	case guild::GuildJoinBossNotJoin:
		m_pNotInBoss->setVisible(true);
		m_pInBoss->setVisible(false);
		m_pNotOpenBoss->setVisible(false);
		break;
	case guild::GuildJoinBossNotOpen:
	default:
		m_pInBoss->setVisible(false);
		m_pNotInBoss->setVisible(false);
		m_pNotOpenBoss->setVisible(true);
		break;
	}
}

void GuildManageCell::setData(ValueObjcet *pData)
{
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = dynamic_cast<guild::GuildUserBasics *>(pData);
	m_pData->isSynced = false;
	guild::GuildUser *selfInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	guild::GuildUserBasics *userInfo = m_pData;

	//头像
	m_pAvatorContainer->removeAllChildren();
	if(userInfo->getLeaderDress() != 0)
	{		
		m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(userInfo->getLeaderDress()));
	}

	if (guild::GuildPositionPresident == userInfo->getPosition())
	{
		m_pNodePresident->setVisible(true);
		m_pNodeAssistant->setVisible(false);
	}
	else if (guild::GuildPositionAssistant == userInfo->getPosition())
	{
		m_pNodePresident->setVisible(false);
		m_pNodeAssistant->setVisible(true);
	}
	else
	{
		m_pNodePresident->setVisible(false);
		m_pNodeAssistant->setVisible(false);
	}
	m_pLabelName->setString(userInfo->getName().c_str());
	m_pLabelContribution->setString(CCString::createWithFormat("%d", userInfo->getLevel())->getCString());
	//m_pLabelKnighthood->setString(userInfo->getKnighthoodName().c_str());
    
    
	//m_pLastLoginDesc->setString(DM_GET_GUILD_MODEL->getLastLoginDescByInveter(userInfo).c_str());
	int offtime = MAX(GameManager::sharedGameManager()->getServerTime() - userInfo->getLastLoginTs() , 0);
	m_pLastLoginDesc->setString(DM_GET_FRIEND_MODEL->getStateString(offtime));
    m_pLastLoginDesc->setVisible(true);
	updateUserJoinGuildBoss();

	GuildManageType iType = GuildManageTypeNone;
	if (guild::GuildPositionPresident == selfInfo->getPosition())
	{
		if (guild::GuildPositionPresident == userInfo->getPosition())
		{
			iType = GuildManagePresident2Self;
		}
		else if (guild::GuildPositionAssistant == userInfo->getPosition())
		{
			iType = GuildManagePresident2Assistant;
		}
		else
		{
			iType = GuildManagePresident2Member;
		}
	}
	else if (guild::GuildPositionAssistant == selfInfo->getPosition())
	{
		if (guild::GuildPositionPresident == userInfo->getPosition())
		{
            iType = GuildManageMember2President;
		}
		else if (guild::GuildPositionAssistant == userInfo->getPosition() && selfInfo->getUserId() == userInfo->getUserId())
		{
			iType = GuildManageAssistant2Self;
		}
		else if (guild::GuildPositionAssistant == userInfo->getPosition())
		{
			iType = GuildManageAssistant2Assistant;
		}
		else
		{
			iType = GuildManageAssistant2Member;
		}
	}
	else
	{
		if (selfInfo->getUserId() == userInfo->getUserId())
		{
			iType = GuildManageMember2Self;
		}else if(userInfo->getPosition()==guild::GuildPositionPresident){
            iType = GuildManageMember2President;
        }
		else
		{
			iType = GuildManageMember2Other;
		}
	}
	setType(iType);
}


void GuildManageCell::setType(GuildManageType iType)
{
	if(iType == m_iType)
	{
		return;
	}
    m_pNodePresident2Self->setVisible(false);
    m_pNodePresident2Assistant->setVisible(false);
    m_pNodePresident2Member->setVisible(false);
    m_pNodeAssistant2Self->setVisible(false);
    m_pNodeAssistant2President->setVisible(false);
    m_pNodeAssistant2Member->setVisible(false);
   // m_pNodeAssistant2assistant->setVisible(false);
    //m_pMessageBox->setVisible(true);
	m_iType = iType;
	switch (m_iType)
	{
	case GuildManageTypeNone:
		break;
    //case GuildManageMember2President:
        //m_pNodeAssistant2assistant->setVisible(true);
		//break;
    case GuildManagePresident2Self:
		m_pNodePresident2Self->setVisible(true);
		//m_pMessageBox->setVisible(false);
		break;
	case GuildManagePresident2Assistant:
		m_pNodePresident2Assistant->setVisible(true);
		break;
	case GuildManagePresident2Member:
		m_pNodePresident2Member->setVisible(true);
		break;
	case GuildManageAssistant2Self:
		m_pNodeAssistant2Self->setVisible(true);
		//m_pMessageBox->setVisible(false);
		break;
	case GuildManageMember2President:
	case GuildManageAssistant2President:
		m_pNodeAssistant2President->setVisible(true);
		break;
	case GuildManageAssistant2Member:
		m_pNodeAssistant2Member->setVisible(true);
		break;
	default:
		break;
	}
}

void GuildManageCell::setExpand(bool isExpanded)
{
	m_pNodeExpand->setVisible(isExpanded);
	if (isExpanded)
	{
		m_pContainer->setPositionY(MANAGE_CELL_SIZE_EXPAND.height - MANAGE_CELL_SIZE.height -5);
	}
	else
	{
		m_pContainer->setPositionY(-5);
	}
}

void GuildManageCell::callBackDemise(CCObject *sender)
{
	m_pDelegate->willDemise();
}
void GuildManageCell::callBackDissolve(CCObject *sender)
{
	m_pDelegate->willDissolve();
}
void GuildManageCell::callBackAddAssistant(CCObject *sender)
{
	m_pDelegate->willChangePosition(m_pData->getUserId(), guild::GuildPositionAssistant);
}
void GuildManageCell::callBackRemove(CCObject *sender)
{
	m_pDelegate->willChangePosition(m_pData->getUserId(), guild::GuildPositionRemoved);
}
void GuildManageCell::callBackDetails(CCObject *sender)
{
	m_pDelegate->willShowDetails(m_pData->getUserId());
}
void GuildManageCell::callBackMessage(CCObject *sender)
{
	m_pDelegate->willSendMessage(m_pData->getUserId());
}

void GuildManageCell::callBackReplaceBoss(CCObject *sender)
{
    m_pDelegate->willReplaceBoss();
//	m_pDelegate->willSendMessage(m_pData->getUserId());
}

void GuildManageCell::callBackRemoveAssistant(CCObject *sender)
{
	m_pDelegate->willChangePosition(m_pData->getUserId(), guild::GuildPositionMember);
}
void GuildManageCell::callBackExit(CCObject *sender)
{
	m_pDelegate->willQuitGuild();
}

//
//  KingTombGuildListCell.cpp
//  LibQSMY
//
//  Created by jk on 14-8-7.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTombGuildListCell.h"


KingTombGuildListCell::KingTombGuildListCell()
:m_pTTFName(NULL)
,m_pBMFLevel(NULL)
,m_pData(NULL)
,m_pBtnAdd(NULL)
,m_pNodeAvatorContainer(NULL)
{

}

KingTombGuildListCell::~KingTombGuildListCell(){
    CC_SAFE_RELEASE(m_pTTFName);
    CC_SAFE_RELEASE(m_pBMFLevel);
    CC_SAFE_RELEASE(m_pNodeAvatorContainer);
    CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RELEASE(m_pBtnAdd);
}




bool KingTombGuildListCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_jl_cy_cell") ;

	m_pBtnAdd->getButton()->setTarget(this , menu_selector(KingTombGuildListCell::seleteCallBack));
	return true;
}


void KingTombGuildListCell::setData(guild::GuildUserBasics *pData){
    if (!pData)
	{
		return;
	}
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
    m_pData = pData;
    m_pTTFName->setString(m_pData->getName().c_str());
	m_pBMFLevel->setString(D_CSTRING_FROM_UINT(m_pData->getLevel()));
    m_pNodeAvatorContainer->removeAllChildren();
    m_pNodeAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(pData->getLeaderDress()));
}


void KingTombGuildListCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void KingTombGuildListCell::onExit()
{
	CCTableViewCell::onExit();
}

bool KingTombGuildListCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFLevel", CCLabelBMFont *, m_pBMFLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pNodeAvatorContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAdd", DButton *, m_pBtnAdd);
	return false;
}

//选择回调
void KingTombGuildListCell::seleteCallBack(CCObject *pSender)
{
    m_pDelegate->guildPlaySelete(m_pData);
}
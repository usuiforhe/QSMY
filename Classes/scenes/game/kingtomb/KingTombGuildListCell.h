//
//  KingTombGuildListCell.h
//  LibQSMY
//  公会成员列表cell
//  Created by jk on 14-8-7.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__KingTombGuildListCell__
#define __LibQSMY__KingTombGuildListCell__


#include "AppInclude.h"
#include "../Game.h"
#include "managers/DataManager.h"


class KingTombGuildListCell;

class KingTombGuildListCellDelegate
{
public:
	virtual void guildPlaySelete(guild::GuildUserBasics *pdata) = 0;
};


class KingTombGuildListCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE(KingTombGuildListCellDelegate *, m_pDelegate, Delegate);
    
    
public:
	KingTombGuildListCell();
	~KingTombGuildListCell();
    
    CREATE_FUNC( KingTombGuildListCell ) ;
    
	virtual bool init();
    
	virtual void onEnter();
	virtual void onExit();
    
	void updateView();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    void setData(guild::GuildUserBasics *pData);
private:
	CCLabelTTF *m_pTTFName;
    CCLabelBMFont *m_pBMFLevel;
	CCNode *m_pNodeAvatorContainer;
    DButton *m_pBtnAdd;
    guild::GuildUserBasics *m_pData;
    
	void seleteCallBack(CCObject *pSender);
};
#endif /* defined(__LibQSMY__KingTombGuildListCell__) */

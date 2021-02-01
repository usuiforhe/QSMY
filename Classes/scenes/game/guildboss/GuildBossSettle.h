//
//  GuildBossSettle.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossSettle__
#define __QSMY__GuildBossSettle__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../guild/DLoadingTabelView.h"

class GuildBossViewDelegate;


class GuildBossSettle 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossSettle();
	~GuildBossSettle();

	static GuildBossSettle* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void callBackClose(CCObject *pSender);

	void updateRefresh(float dt);
protected:
	CC_SYNTHESIZE(GuildBossViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(int, m_iSelected, Selected);
private:
	CCLabelBMFont *m_pBMFDamageAllSelf;
	CCLabelBMFont *m_pBMFDamageRobSelf;
	CCLabelBMFont *m_pBMFDamageGotSelf;
	CCLabelBMFont *m_pBMFDamageAllGuild;
	CCLabelBMFont *m_pBMFDamageRobGuild;
	CCLabelBMFont *m_pBMFDamageGotGuild;
	CCLabelBMFont *m_pBMFDamageAllTarget;
	CCLabelBMFont *m_pBMFDamageRobTarget;
	CCLabelBMFont *m_pBMFDamageGotTarget;

	CCNode *m_pNodeToday;
	CCNode *m_pNodeYesterday;

	CCLabelTTF *m_pTTFName;
	CCLabelBMFont *m_pBMFMailTime;

	DButton *m_pDBtnBack;

	GuildBossModel *m_pModel;


	float m_fWaitTime;
};

#endif
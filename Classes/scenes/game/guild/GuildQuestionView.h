//
//  GuildQuestionView.h
//  公会问答
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildQuestionView__
#define __QSMY__GuildQuestionView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildHead.h"

class GuildQuestionViewDelegate
{
public:
	virtual void willCloseQuestionView(uint32_t dropID = 0) = 0;
};

class GuildQuestionView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildQuestionView();
	~GuildQuestionView();

	static GuildQuestionView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark callback function
	void callBackYes(cocos2d::CCObject *sender);
	void callBackNo(cocos2d::CCObject *sender);
	void callBackCancel(cocos2d::CCObject *sender);

	void callBackAnswer(const Json::Value &requestData,const Json::Value &responseData);

	void callBackShowExp(sp::Armature * target, const char* event,const char* data);

protected:
	CC_SYNTHESIZE(GuildQuestionViewDelegate *, m_pDelegate, Delegate);
private:

	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelPosition;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelExp;
	DProgressBar* m_pExpBar;				/*经验条*/
	cocos2d::CCLabelBMFont *m_pLabelNextExp;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice0;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice1;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice2;

	DButton *m_pButtonYes;
	DButton *m_pButtonNo;
	CCMenuItem *m_pButtonCancel;

	cocos2d::CCLabelTTF *m_pLabelQuestion;
	cocos2d::CCLabelBMFont *m_pLabelTime;

	cocos2d::CCLabelTTF *m_pLabelAnswer0;
	cocos2d::CCLabelTTF *m_pLabelAnswer1;

	GuildModel *m_pModel;

	void updateQuestion();
	void updateView();
	void updateNotice();

	float m_fTime;

	virtual void update(float dt);

	bool m_isExpShown;

	uint32_t m_nDropID;
};

#endif
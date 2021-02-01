//
//  Chat.h
//  聊天
//  Created by mzp on 14-04-22.
//	Modified 
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Chat__
#define __QSMY_Chat__

#include "AppInclude.h"
#include "components/components.h"
#include "../settings/UserFeedback.h"



class Chat
	: public DPopup
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public EditBoxDelegate
	, public DTabViewDelegate
{
public:
	Chat();
	~Chat();

	CREATE_FUNC(Chat);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual void editBoxReturn(EditBox* editBox);
	virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
	virtual void SelectTabAtIndex(DTabView* tabView, uint32_t idx);

private:
	void menuCallback(cocos2d::CCObject* obj);
	void updataChatItem();
	void buySuccess();
	void requestGuildMsg(float dt);
	void requestWorldMsg(float dt);

	void guildGetSuc();
	void guildSendSuc();
	void worldGetSuc();
	void worldSendSuc();

	void updateGuildInfo();

private:
	cocos2d::CCNode* m_pContainer;
	cocos2d::CCNode* m_pTabContainer;
	DButton* m_pBtnSend;
	DButton* m_pBtnClose;
	cocos2d::CCNode* m_pMsgContainer;
	MyTextFieldTTF* m_pTextField;
	DTabView* m_pDTabView;
	cocos2d::CCNode* m_pChatItem;
	cocos2d::CCLabelBMFont* m_pChatItemNum;

	unsigned int m_nSelectIndex;
	// 是否需要请求工会消息
	bool m_bRequestGuild;
};


#endif	// __QSMY_Chat__
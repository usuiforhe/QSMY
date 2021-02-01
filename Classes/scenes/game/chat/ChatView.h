//
//  ChatView.h
//  聊天
//  Created by mzp on 14-04-22.
//	Modified 
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Chat_View__
#define __QSMY_Chat_View__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/ChatModel.h"


class ChatView
	: public DLayer
	, public cocos2d::extension::CCTableViewDataSource
	, public cocos2d::extension::CCTableViewDelegate
{
public:
	ChatView();
	~ChatView();

	static ChatView* create(chat::ChatChannel type, const cocos2d::CCSize& size);
	virtual bool init(chat::ChatChannel type, const cocos2d::CCSize& size);
	virtual void onEnter();
	virtual void onExit();

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView* table);

private:
	void updateMsg();

private:
	DTableView* m_pListView;
	cocos2d::extension::CCData* m_pData;
	std::map<unsigned int, cocos2d::CCSize> m_sizeMap;

	ChatModel* m_pChatModel;
	chat::ChatChannel m_kChatChannel;
};


class ChatViewCell
	: public cocos2d::extension::CCTableViewCell
	, public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(float, m_fHeight, Height);

public:
	ChatViewCell();
	virtual ~ChatViewCell();

	static ChatViewCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

public:
	void setChatInfo(chat::ChatChannel channel, chat::ChatInfo* pInfo);

private:
	cocos2d::CCNode* m_pNode;
	cocos2d::CCNode* m_pOtherBj;
	cocos2d::CCNode* m_pSelfBj;
	cocos2d::CCRichLabelTTF* m_pName;
	cocos2d::CCRichLabelTTF* m_pText;

	cocos2d::CCNode* m_pGuild;
	cocos2d::CCLabelTTF* m_pGuildName;
	cocos2d::CCNode* m_pVip;
	cocos2d::CCLabelBMFont* m_pVipTxt;

	cocos2d::CCPoint m_oOriginPoint;
	cocos2d::CCSize m_oOriginSize;
	cocos2d::CCNode* m_pArrow;
	cocos2d::CCNode* m_pArrowSelf;
	cocos2d::CCNode* m_pTitleSystemBg;
	cocos2d::CCNode* m_pTitleNormalBg;
	cocos2d::CCNode* m_pArrowSystem;
	cocos2d::CCNode* m_pSystemBg;
};



#endif
//
//  ChatButton.h
//  聊天按钮
//  Created by mzp on 14-04-21.
//	Modified 
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Chat_Button__
#define __QSMY_Chat_Button__


#include "components/components.h"


class ChatButton
	: public DDraggingView
{
public:
	ChatButton();
	virtual ~ChatButton();

	CREATE_FUNC(ChatButton);
	virtual bool init();

	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

	void setHaveNew(bool have);

private:
	void adjustPosition();
	void chatCallback(cocos2d::CCObject* sender);

private:
	sp::Armature            *m_pTips;
};



#endif	// __QSMY_Chat_Button__

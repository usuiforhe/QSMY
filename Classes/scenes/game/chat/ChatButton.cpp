//
//  ChatButton.cpp
//  聊天按钮
//  Created by mzp on 14-04-21.
//	Modified 
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "ChatButton.h"
#include "Chat.h"
#include "../Game.h"


USING_NS_CC;



ChatButton::ChatButton()
	: m_pTips(NULL)
{

}

ChatButton::~ChatButton()
{
	CC_SAFE_RELEASE(m_pTips);
}

bool ChatButton::init()
{
	if (!DDraggingView::init())
	{
		return false;
	}
	
	CCMenuItemImage* pChatMenuImgage = CCMenuItemImage::create("PL_ui/liaotian/icon_chat_n.png","PL_ui/liaotian/icon_chat_h.png",this, menu_selector(ChatButton::chatCallback));
	CCMenu* pChatMenu = CCMenu::create(pChatMenuImgage, NULL);
	pChatMenu->setAnchorPoint(ccp(0.0f, 0.0f));
	pChatMenuImgage->setAnchorPoint(ccp(0.0f, 0.0f));
	pChatMenu->setPosition(ccp(0.0f, 0.0f));
	pChatMenuImgage->setPosition(ccp(0.0f, 0.0f));
	pChatMenu->setContentSize(pChatMenuImgage->getContentSize());
	pChatMenu->setTouchEnabled(false);

	// 新消息提示
	m_pTips = ResourceManager::sharedResourceManager()->getUIAnimate("chatPrompt");
	CC_SAFE_RETAIN(m_pTips);
	m_pTips->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pTips->setPosition(ccp(65.0f,52.0f));
	m_pTips->setVisible(false);

	this->setContentSize(pChatMenuImgage->getContentSize());
	this->addChild(pChatMenu, 0);
	this->addChild(m_pTips, 1);
	return true;
}

void ChatButton::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
	DDraggingView::ccTouchEnded(pTouch, pEvent);

	// 调整位置
	adjustPosition();
}

void ChatButton::adjustPosition()
{
	// 计算当前点离哪边最近
	const CCSize& winSize = CCDirector::sharedDirector()->getWinSize();
	const CCPoint& point = this->getPosition();
	const CCSize& size = this->getContentSize();
	CCPoint center(point.x + size.width / 2, point.y + size.height / 2);

	// 0,左，1，右，2，下，3，上
	int type = 0;
	float disX = 0.0f;
	int typeX = 0;
	if (center.x < DESIGN_SIZE.width / 2)
	{
		typeX = 0;
		disX = abs(center.x);
	}
	else
	{
		typeX = 1;
		disX = abs(DESIGN_SIZE.width - center.x);
	}
	
	float disY = 0.0f;
	int typeY = 2;
	if (center.y < DESIGN_SIZE.height / 2)
	{
		typeY = 2;
		disY = abs(center.y);
	}
	else
	{
		typeY = 3;
		disY = abs(DESIGN_SIZE.height - center.y);
	}

	if (disX < disY)
	{
		type = typeX;
	}
	else
	{
		type = typeY;
	}

	float minX = (winSize.width - DESIGN_SIZE.width) * 0.5f;
	float maxX = minX + DESIGN_SIZE.width - size.width;
	float minY = (winSize.height - DESIGN_SIZE.height) * 0.5f +PL_MAX_HEIGHT_DIFF/2;
	float maxY = minY + DESIGN_SIZE.height - size.height - PL_MAX_HEIGHT_DIFF;
	this->stopAllActions();
	switch (type)
	{
	case 0:
		{
			float y = point.y;
			if (y < minY || y > maxY)
			{
				y = typeY == 2 ? minY : maxY;
			}
			
			CCActionInterval* move = CCMoveTo::create(0.1f, ccp(minX, y));
			this->runAction(move);
			break;
		}
	case 1:
		{
			float y = point.y;
			if (y < minY || y > maxY)
			{
				y = typeY == 2 ? minY : maxY;
			}

			CCActionInterval* move = CCMoveTo::create(0.1f, ccp(maxX, y));
			this->runAction(move);
			break;
		}
	case 2:
		{
			float x = point.x;
			if (x < minX || x > maxX)
			{
				x = typeX == 0 ? minX : maxX;
			}

			CCActionInterval* move = CCMoveTo::create(0.1f, ccp(x, minY));
			this->runAction(move);
			break;
		}
	case 3:
		{
			float x = point.x;
			if (x < minX || x > maxX)
			{
				x = typeX == 0 ? minX : maxX;
			}

			CCActionInterval* move = CCMoveTo::create(0.1f, ccp(x, maxY));
			this->runAction(move);
			break;
		}
	}
}

void ChatButton::chatCallback(cocos2d::CCObject* sender)
{
	Game::sharedGame()->openPopup(Chat::create());
	//Game::sharedGame()->NavigateTo(navigation::kNavChat);
}

void ChatButton::setHaveNew(bool have)
{
	if (!m_pTips)
	{
		return;
	}
	
	m_pTips->setVisible(have);
}

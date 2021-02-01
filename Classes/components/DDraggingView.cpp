//
//  DDraggingView.cpp
//  可拖动节点
//
//  Created by mzp on 14-04-18.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "DDraggingView.h"


USING_NS_CC;



DDraggingView* DDraggingView::create(const cocos2d::CCSize& size)
{
	DDraggingView* pRet = new DDraggingView();
	if (pRet && pRet->initWithContentSize(size))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

DDraggingView::DDraggingView()
{

}

DDraggingView::~DDraggingView()
{

}

bool DDraggingView::init()
{
	return this->initWithContentSize(CCSizeMake(200, 200));
}

bool DDraggingView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	if (!this->isVisible())
	{
		return false;
	}
	
	CCPoint pt = pTouch->getLocation();
	CCRect rcBoundingBox = CCRectMake(0, 0, getContentSize().width, getContentSize().height);
	rcBoundingBox = CCRectApplyAffineTransform( rcBoundingBox, nodeToWorldTransform());
	if (!rcBoundingBox.containsPoint(pt))
	{
		return false;
	}

	return true;
}

void DDraggingView::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	// 移动偏移量
	CCPoint effset = pTouch->getLocation() - pTouch->getPreviousLocation();
	this->setPosition(this->getPosition() + effset);
}

void DDraggingView::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{

}

void DDraggingView::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
{

}

bool DDraggingView::initWithContentSize(const cocos2d::CCSize& size)
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setContentSize(size);

	return true;
}

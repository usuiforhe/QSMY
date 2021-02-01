//
//  DDraggingView.h
//  可拖动节点
//
//  Created by mzp on 14-04-18.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_DDragging_Layer__
#define __QSMY_DDragging_Layer__

#include "cocos2d.h"


class DDraggingView
	: public cocos2d::CCLayer
{
public:
	DDraggingView();
	virtual ~DDraggingView();

	CREATE_FUNC(DDraggingView);
	static DDraggingView* create(const cocos2d::CCSize& size);
	virtual bool init();

	virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

private:
	bool initWithContentSize(const cocos2d::CCSize& size);
};



#endif	// __QSMY_DDragging_Layer__
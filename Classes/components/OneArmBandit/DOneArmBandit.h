//
//  DOneArmBandit.h
//  老虎机
//
//  Created by ldr123 on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DOneArmBandit__
#define __QSMY__DOneArmBandit__

#include "AppInclude.h"
#include "DragonBone.h"

class DOneArmBanditDelegate
{
public:
	virtual void onStart(cocos2d::CCNode *pNode) = 0;
	virtual void onComplete(cocos2d::CCNode *pNode) = 0;
};

#define FAKE_LAYER_NUM 5
class DOneArmBandit : public cocos2d::CCNode
{
public:
	DOneArmBandit();
	virtual ~DOneArmBandit();
public:
	static DOneArmBandit* create(cocos2d::CCNode* pDefault, DOneArmBanditDelegate* p, const cocos2d::CCSize& contentSize);
protected:
	virtual bool init(cocos2d::CCNode* pDefault, DOneArmBanditDelegate* p, const cocos2d::CCSize& contentSize);
	virtual void visit();
public:
	void start();
	void setData(cocos2d::CCNode* pTrueObj, cocos2d::CCNode** pFakeObj);
public:
	void onComplete(sp::Armature *, const char *, const char *);
private:
	cocos2d::CCNode*	m_pContainer;
	cocos2d::CCNode*	m_pTrueObj;
	cocos2d::CCNode*	m_pFakeObj[FAKE_LAYER_NUM];
	bool				m_bStart;
private:
	DOneArmBanditDelegate*	m_pDelegate;
};

#endif /* defined(__QSMY__DOneArmBandit__) */

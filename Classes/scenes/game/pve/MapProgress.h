//
//  MapProgress.h
//  地图进度
//
//  Created by Eci on 13-11-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MapProgress__
#define __QSMY__MapProgress__

#define MAX_STEP 5
#define MIN_STEP 2

#define FIT_STEP MAX_STEP - MIN_STEP + 1

#include "AppInclude.h"
#include "managers/DataManager.h"

class MapProgress
	:public cocos2d::CCNode
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	CREATE_FUNC(MapProgress);
	bool init();
	MapProgress();
	virtual ~MapProgress();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
	cocos2d::CCNode *m_pNodeMap[FIT_STEP];
	cocos2d::CCDictionary *m_pArrayStep[FIT_STEP];
	cocos2d::CCNode *m_pPoint;
	cocos2d::CCNode *m_pBoss;
	cocos2d::CCNode *m_pFight;

	int m_nTotalStep;
public:
	void setTotalStep(int nStep);
	void setIsBoss(bool isBoss);
	void moveToNthStep(int nth, bool animated = true);
};


class MapProgressLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MapProgressLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MapProgress);
};

#endif /* defined(__QSMY__MapProgress__) */

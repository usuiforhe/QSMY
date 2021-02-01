//
//  MapProgress.h
//  地图进度
//
//  Created by Eci on 13-11-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#define FINAL_POINT_POSTIONY_PLUS 30


#include "MapProgress.h"

USING_NS_CC;


MapProgress::MapProgress()
:m_pPoint(NULL)
,m_pBoss(NULL)
,m_pFight(NULL)
{
    for (int i = 0; i < FIT_STEP; i++)
    {
		m_pNodeMap[i] = NULL;
		m_pArrayStep[i] = NULL;
    }
}


MapProgress::~MapProgress()
{
	CC_SAFE_RELEASE(m_pPoint);
	CC_SAFE_RELEASE(m_pBoss);
	CC_SAFE_RELEASE(m_pFight);
	for (int i = 0; i < FIT_STEP; i++)
	{
		CC_SAFE_RELEASE(m_pNodeMap[i]);
		CC_SAFE_RELEASE(m_pArrayStep[i]);
	}
}

bool MapProgress::init()
{
	if (!CCNode::init())
	{
		return false;
	}
	
	for (int i = 0; i < FIT_STEP; i++)
	{
		m_pArrayStep[i] = CCDictionary::create();
		CC_SAFE_RETAIN(m_pArrayStep[i]);
	}
    return true;
}

bool MapProgress::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint", CCNode *, this->m_pPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoss", CCNode *, this->m_pBoss);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFight", CCNode *, this->m_pFight);
    
	for (int i = 0; i < FIT_STEP; i++)
	{
		char name[20] = {0};
		sprintf(name, "m_pNodeMap%d", i + MIN_STEP);
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, name, CCNode *, this->m_pNodeMap[i]);
		
		for (int j = 0; j < i + MIN_STEP; j++)
		{
			sprintf(name, "m_pMap%dStep%d", i + MIN_STEP, j + 1);
			if (strcmp(name, pMemberVariableName)==0)
			{
				m_pArrayStep[i]->setObject(pNode, j);
                return true;
			}
		}
		
	}
    return false;
}

void MapProgress::setTotalStep(int nStep)
{
	if (nStep < MIN_STEP || nStep > MAX_STEP)
	{
		return;
	}
	m_nTotalStep = nStep - MIN_STEP;
	for (int i = 0; i < FIT_STEP; i++)
	{
		m_pNodeMap[i]->setVisible(false);
	}
	m_pNodeMap[m_nTotalStep]->setVisible(true);
	CCPoint pt = ((CCNode *)m_pArrayStep[m_nTotalStep]->objectForKey(nStep-1))->getPosition();
	pt.y += FINAL_POINT_POSTIONY_PLUS;
	m_pBoss->setPosition(pt);
	m_pFight->setPosition(pt);
}

void MapProgress::setIsBoss(bool isBoss)
{
	m_pBoss->setVisible(isBoss);
    m_pFight->setVisible(!isBoss);
}

void MapProgress::moveToNthStep(int nth, bool animated /* = true */)
{
	if (!animated || 0 == nth)
	{
		CCPoint point = ((CCNode *)m_pArrayStep[m_nTotalStep]->objectForKey(nth))->getPosition();
		m_pPoint->setPosition(point);
	}
	else
	{
		CCPoint oldPoint = ((CCNode *)m_pArrayStep[m_nTotalStep]->objectForKey(nth - 1))->getPosition();
		m_pPoint->setPosition(oldPoint);
		CCPoint newPoint = ((CCNode *)m_pArrayStep[m_nTotalStep]->objectForKey(nth))->getPosition();
		m_pPoint->runAction(CCMoveTo::create(0.2f, newPoint));
	}
}
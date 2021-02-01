//
//  DOneArmBandit.cpp
//  老虎机
//
//  Created by ldr123 on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AppMacros.h"
#include "DOneArmBandit.h"
#include "DOneArmBanditData.h"

USING_NS_CC;
using namespace sp;

DOneArmBandit::DOneArmBandit():m_pContainer(NULL), m_pTrueObj(NULL), m_bStart(false)
{
	for (int i=0; i<5; ++i)
	{
		m_pFakeObj[i] = NULL;
	}
}

DOneArmBandit::~DOneArmBandit()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pTrueObj);
	for (int i=0; i<FAKE_LAYER_NUM; ++i)
	{
		CC_SAFE_RELEASE(m_pFakeObj[i]);
	}
}

DOneArmBandit* DOneArmBandit::create(cocos2d::CCNode* pDefault, DOneArmBanditDelegate* p, const cocos2d::CCSize& contentSize)
{
	DOneArmBandit* pRet = new DOneArmBandit();
	if (pRet && pRet->init(pDefault, p, contentSize))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool DOneArmBandit::init(cocos2d::CCNode* pDefault, DOneArmBanditDelegate* p, const cocos2d::CCSize& contentSize)
{
	m_pContainer = CCNode::create();
	CC_SAFE_RETAIN(m_pContainer);
	
	pDefault->ignoreAnchorPointForPosition(false);
	pDefault->setAnchorPoint(ccp(0.0f, 0.0f));
	pDefault->setPosition(0.f, 0.f);

 	m_pContainer->addChild(pDefault);
	m_pContainer->setContentSize(contentSize);

	addChild(m_pContainer);
	m_pDelegate = p;

	return CCNode::init();
}

void DOneArmBandit::visit()
{
	if (!m_bVisible)
	{
		return;
	}

	kmGLPushMatrix();
	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
		transformAncestors();
	}

	transform();
	CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
	CCSize szT = m_pContainer->getContentSize();
	CCRect frame =  CCRectMake(screenPos.x, screenPos.y, szT.width, szT.height);
	glEnable(GL_SCISSOR_TEST);

	CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
	CCNode* pNode = NULL;
	unsigned int i = 0;
	if(m_pChildren && m_pChildren->count() > 0)
	{
		sortAllChildren();
		ccArray *arrayData = m_pChildren->data;
		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];

			if ( pNode && pNode->getZOrder() < 0 )
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}

		draw();
		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];
			if (pNode)
			{
				pNode->visit();
			}
		}
	}
	else
	{
		draw();
	}

	glDisable(GL_SCISSOR_TEST);

	m_uOrderOfArrival = 0;

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	kmGLPopMatrix();
}

void DOneArmBandit::setData(cocos2d::CCNode* pTrueObj, cocos2d::CCNode** pFakeObj)
{
	if (m_bStart)
	{
		return;
	}

	if (!pFakeObj)
	{
		return;
	}

	m_pTrueObj = pTrueObj;
	CC_SAFE_RETAIN(m_pTrueObj);
	m_pTrueObj->ignoreAnchorPointForPosition(false);
	m_pTrueObj->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pTrueObj->setPosition(0.f, 0.f);

	for (int i=0; i<FAKE_LAYER_NUM; ++i)
	{
		m_pFakeObj[i] = pFakeObj[i];
		CC_SAFE_RETAIN(m_pFakeObj[i]);

		if (m_pFakeObj[i])
		{
			m_pFakeObj[i]->ignoreAnchorPointForPosition(false);
			m_pFakeObj[i]->setAnchorPoint(ccp(0.0f, 0.0f));
			m_pFakeObj[i]->setPosition(0.f, 0.f);
		}
	}
}

void DOneArmBandit::start()
{
	if (m_bStart)
	{
		return;
	}

	m_bStart = true;

	m_pContainer->removeAllChildren();
    
	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("OneArmBandit", g_OneArmBanditData, strlen(g_OneArmBanditData));
	Armature *pArmature = Armature::create("OneArmBandit", "OneArmBanditFk");

	SPContainer *pOne =  SPContainer::create(pArmature->getRenderType());
	pOne->addChild(m_pTrueObj);
    
    pArmature ->getBone("six")->replaceDisplayAt(pOne, 0);

	const std::string fakeFk[] = {"one","two","three","four","five","six"};
	SPContainer *pTmp = NULL;
	for (int i=0; i<FAKE_LAYER_NUM; ++i)
	{
		if (m_pFakeObj[i])
		{
			pTmp =  SPContainer::create(pArmature->getRenderType());
			pTmp->addChild(m_pFakeObj[i]);
            pArmature ->getBone(fakeFk[i].c_str())->replaceDisplayAt(pTmp, 0);
		}
	}

	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(DOneArmBandit::onComplete), COMPLETE);

	pArmature->getAnimation()->playByIndex(0);
    pArmature->getAnimation()->setAnimationScale(0.5f);

	m_pContainer->addChild(pArmature);

	if (m_pDelegate)
	{
		m_pDelegate->onStart(this);
	}
}

void DOneArmBandit::onComplete(sp::Armature *_armature, const char *_eventType, const char *_movementID)
{
	m_bStart = false;
	m_pContainer->removeAllChildren();
	m_pTrueObj->removeFromParent();
	m_pContainer->addChild(m_pTrueObj);

	if (m_pDelegate)
	{
		m_pDelegate->onComplete(this);
	}
}

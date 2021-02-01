//
//  DLoadingTableView.h
//  QSMY
//
//  Created by Eci on 13-11-04.
//  Copyright (c) 2013年 thedream. All rights reserved.
//


#include "DLoadingTabelView.h"
#include "managers/ResourceManager.h"

using namespace sp;

#define EFFECT_LAYER_POSITION ccp(.0, -500)
#define EFFECT_LAYER_HEIGHT 50

USING_NS_CC;
USING_NS_CC_EXT;

DLoadingTableView::DLoadingTableView()
	:m_pLoadingDelegate(NULL)
	,m_isReachTheEnd(false)
	,m_pEffectLayer(NULL)
	,m_isLoadingEnabled(true)
	,m_uPageSize(0)
	,m_uLastCount(0)
	,m_ptLastPoint(0, 0)
	,m_bIsLoading(false)
{

}

DLoadingTableView::~DLoadingTableView()
{
	CC_SAFE_RELEASE(m_pEffectLayer);
}

DLoadingTableView *DLoadingTableView::create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size)
{

	return DLoadingTableView::create(dataSource, size, NULL);
}

DLoadingTableView *DLoadingTableView::create(cocos2d::extension::CCTableViewDataSource* dataSource, cocos2d::CCSize size, CCNode *container)
{

	DLoadingTableView *table = new DLoadingTableView();
	table->initWithViewSize(size, container);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();

	table->m_pEffectLayer = DLayer::create();
	table->m_pEffectLayer->retain();
	//table->m_pEffectLayer->setAnchorPoint(ccp(0.0, 0.0));
	table->addChild(table->m_pEffectLayer, 1000);

	table->m_pEffectLayer->setPosition(ccp(size.height / 2.0, -25));
	return table;
}



void DLoadingTableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

	if (this->getContentOffset().y > EFFECT_LAYER_HEIGHT && m_isReachTheEnd == false && m_isLoadingEnabled == true)
	{

		if (m_pTouches->containsObject(pTouch))
		{
			m_pTouches->removeObject(pTouch);
		} 

		if (m_pTouches->count() == 0)
		{
			m_bDragging = false;    
			m_bTouchMoved = false;
		}
		m_ptLastPoint = this->getContentOffset();
		this->setContentOffset(ccp(0, EFFECT_LAYER_HEIGHT), true);
		m_uLastCount = m_pDataSource->numberOfCellsInTableView(this);
		m_bIsLoading = true;
		if (m_pLoadingDelegate)
		{
			m_pLoadingDelegate->startLoading();
		}

		/*RefreshEffect* pEffect = RefreshEffect::create();
		pEffect->setCallBack(this,callfuncO_selector(DLoadingTableView::callBackRefresh));
		m_pEffectLayer->addNode(pEffect,true);*/
		
		Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("listloading",sp::ZORDER);
		//pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(RefreshEffect::animateEnd), COMPLETE);
		//pArmature->setPosition(EFFECT_LAYER_POSITION);
		m_pEffectLayer->addChild(pArmature);
		
	}
	else
	{
		DTableView::ccTouchEnded(pTouch, pEvent);
	}
}

void DLoadingTableView::endLoading()
{
	//this->setContentOffset(m_ptLastPoint, false);
	if (m_bIsLoading)
	{
		int addedCount = m_pDataSource->numberOfCellsInTableView(this) - m_uLastCount;
		int posY = (addedCount - 2) * m_pDataSource->tableCellSizeForIndex(this, 0).height;
		this->setContentOffset(ccp(0, -posY), false);
		m_pEffectLayer->removeAllChildren();
		m_bIsLoading = false;
	}
	if (m_uPageSize)
	{
		if (m_pDataSource->numberOfCellsInTableView(this) % m_uPageSize)
		{
			m_isReachTheEnd = true;
		}
		else
		{
			m_isReachTheEnd = false;
		}
	}
}

void DLoadingTableView::setReachTheEnd(bool isReachTheEnd)
{
	m_isReachTheEnd = isReachTheEnd;
	
}

void DLoadingTableView::setLoadingEnabled(bool isLoadingEnabled)
{
	m_isLoadingEnabled = isLoadingEnabled;
	m_pEffectLayer->setVisible(isLoadingEnabled);
}

void DLoadingTableView::callBackRefresh(CCObject *sender)
{

}
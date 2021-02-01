//
//  kingTombMine.cpp
//  LibQSMY
//
//  Created by jk on 14-8-4.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTombMine.h"

KingTombMine::KingTombMine()
:m_pNodeUnopen(NULL)
,m_pBoxOpen(NULL)
,m_pBoxContainer(NULL)
,m_pBoxLost(NULL)
,m_pBoxOld(NULL)
{
}

KingTombMine::~KingTombMine()
{
}

bool KingTombMine::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    setContentSize(DESIGN_SIZE);
    
	m_pNodeUnopen = CCNode::create();
	addChild(m_pNodeUnopen);

	//空
	m_pBoxLost = CCSprite::create("PL_ui/banghui/banghui_tanxian_bx_1.png");
	m_pBoxLost->setPosition(D_DESIGN_POINT_CENTER);
	addChild(m_pBoxLost);
	m_pBoxLost->setVisible(false);
	m_pBoxLost->setPositionY(D_DESIGN_SIZE_HEIGHT_HALF + 88);
	//PL_MOVE_NODE_BELOW_CENTER_Y(m_pBoxLost);

	//旧
	m_pBoxOld = CCSprite::create("PL_ui/banghui/banghui_tanxian_bx_2.png");
	m_pBoxOld->setPosition(D_DESIGN_POINT_CENTER);
	addChild(m_pBoxOld);
	m_pBoxOld->setVisible(false);
	m_pBoxOld->setPositionY(D_DESIGN_SIZE_HEIGHT_HALF + 88);
	//PL_MOVE_NODE_BELOW_CENTER_Y(m_pBoxOld);

	//有宝物
	m_pBoxOpen = ResourceManager::sharedResourceManager()->getUIAnimate("tombBox", sp::ZORDER);
	m_pBoxOpen->setPosition(D_DESIGN_POINT_CENTER);
	addChild(m_pBoxOpen);
	m_pBoxOpen->setVisible(false);

	//图片添加节点
	m_pBoxContainer = CCNode::create();
	sp::SPContainer *pSPContainer =  sp::SPContainer::create(m_pBoxOpen->getRenderType());
	pSPContainer->addChild(m_pBoxContainer);
	m_pBoxOpen->getBone("bptsbaoxiang")->replaceDisplayAt(pSPContainer, 0);

	return true;
}

void KingTombMine::onExit()
{
	DLayer::onExit();
}

void KingTombMine::setData(int type){
	m_pNodeUnopen->removeAllChildren();
	if(type == 0 || type >=6 )
	{
		//无宝物
		sp::Armature* m_pBoxUnopen = ResourceManager::sharedResourceManager()->getUIAnimate("tombUnopen");
		m_pBoxUnopen->setPosition(D_DESIGN_POINT_CENTER);
		m_pNodeUnopen->addChild(m_pBoxUnopen);
	}
    //设置要显示 那个矿  更具类型判断
	m_pNodeUnopen->setVisible(type==0);
	m_pBoxOpen->setVisible(type>= 3 && type <= 5);
	if(m_pBoxOpen->isVisible())
	{
		//向图片节点加入对应宝箱图片
		m_pBoxContainer->removeAllChildren();
		const char* path = CCString::createWithFormat("PL_ui/banghui/banghui_tanxian_bx_%d.png" , type)->getCString();
		CCSprite* pBox = CCSprite::create(path);
		if(pBox) m_pBoxContainer->addChild(pBox);
	}

	//处理图片
	m_pBoxLost->setVisible(type==1);
	m_pBoxOld->setVisible(type==2);
}

void KingTombMine::onEnter()
{
    DLayer::onEnter();
}
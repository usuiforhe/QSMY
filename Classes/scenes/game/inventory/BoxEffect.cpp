//
//  BoxEffect.cpp
//  开宝箱动画
//
//  Created by fengkerong.billy on 14-10-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BoxEffect.h"
#include "model/ItemModel.h"

USING_NS_CC;
using namespace sp;

BoxEffect::BoxEffect()
	:m_pFlash(NULL)
	,m_pItemInfo(NULL)
{
}

BoxEffect::~BoxEffect()
{
	CC_SAFE_RELEASE(m_pFlash);
}

BoxEffect* BoxEffect::create(item::ItemInfo *info)
{
	BoxEffect* pRet = new BoxEffect();
	if(pRet && pRet->init(info))
	{
		pRet->autorelease();
		return pRet;
	}else
	{
		CC_SAFE_DELETE(pRet);
	}
	return NULL;
}

bool BoxEffect::init(item::ItemInfo *info)
{
	if(!DEffectNode::init()) return false;
	m_pItemInfo = info;

	//不同宝箱，不同动画
	CCString * flashname = ccs("boxEffect");
	if(info->getBaseInfo()->getItemId() == 10009) flashname = ccs("boxEffectPurple");
	if(info->getBaseInfo()->getItemId() == 10008) flashname = ccs("boxEffectOrange");

	CCLayerColor* m_pBg = CCLayerColor::create(ccc4(0,0,0,205));
	m_pBg->setContentSize(cocos2d::CCSizeMake(cocos2d::CCDirector::sharedDirector()->getWinSize().width , DESIGN_SIZE.height));
	addChild(m_pBg);

	m_pFlash =  ResourceManager::sharedResourceManager()->getUIAnimate(flashname->getCString(),sp::ZORDER);
	m_pFlash->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BoxEffect::animateEnd), COMPLETE);
	m_pFlash->setPosition(D_DESIGN_POINT_CENTER);
	CC_SAFE_RETAIN(m_pFlash);

	play();
	return true;
}

void BoxEffect::play()
{
	m_pFlash->getAnimation()->playByIndex(0);
	addChild(m_pFlash);
}

void BoxEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
	callback();
	removeFromParent();
}
//
//  DProgressBar.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-31.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DProgressBar.h"
#include "../../libs/SPArmature/SPArmature.h"
#include "../managers/ResourceManager.h"

USING_NS_CC;


DProgressBar::DProgressBar()
:m_pProgressSprite(NULL)
,m_pProgressShining(NULL)
,m_pBackgroundSprite(NULL)
,m_percent(1.0f)
,m_mColorForPecent()
,m_pFlash(NULL)
{
    
}

DProgressBar::~DProgressBar()
{
    CC_SAFE_RELEASE(m_pProgressSprite);
	CC_SAFE_RELEASE(m_pProgressShining);
    CC_SAFE_RELEASE(m_pBackgroundSprite);
}

void DProgressBar::setBgSpriteFrame(cocos2d::CCSpriteFrame* pBg)
{
    m_pBackgroundSprite->setDisplayFrame(pBg);
    setContentSize(m_pBackgroundSprite->getContentSize());
    m_pBackgroundSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pBackgroundSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
    m_pProgressSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pProgressSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
    this->needsLayout(m_percent);
}

void DProgressBar::setBarSpriteFrame(cocos2d::CCSpriteFrame* pBar)
{
    m_pProgressSprite->setDisplayFrame(pBar);
	if(m_pProgressShining)m_pProgressShining->setDisplayFrame(pBar);
}

void DProgressBar::setMaskSpriteFrame(cocos2d::CCSpriteFrame* pMask)
{
    m_pProgressSprite->setMask(pMask->getTexture());
	if(m_pProgressShining)m_pProgressShining->setMask(pMask->getTexture());
}

DProgressBar* DProgressBar::create(const char* pbgFileName, const char* progressFile, const char* pmaskFileName)
{
    // Prepare background for slider
    CCSprite *backgroundSprite      = CCSprite::create(pbgFileName);
    
    // Prepare progress for slider
    DMaskedSprite *progressSprite   = DMaskedSprite::create(progressFile,pmaskFileName);
    
    
    return DProgressBar::create(backgroundSprite, progressSprite);
}

DProgressBar* DProgressBar::create(CCSprite * backgroundSprite, DMaskedSprite* pogressSprite)
{
    DProgressBar *pRet = new DProgressBar();
    pRet->initWithSprites(backgroundSprite, pogressSprite);
    pRet->autorelease();
    return pRet;
}

bool DProgressBar::init()
{
    return initWithSprites(CCSprite::create(),DMaskedSprite::create());
}

bool DProgressBar::initWithSprites(CCSprite * backgroundSprite, DMaskedSprite* progressSprite)
{
    if (CCNode::init())
    {
        CCAssert(backgroundSprite,  "Background sprite must be not nil");
        CCAssert(progressSprite,    "Progress sprite must be not nil");
        
        ignoreAnchorPointForPosition(false);
        
        setAnchorPoint(ccp(0.5,0.5));
        
        m_pBackgroundSprite = backgroundSprite;
        m_pProgressSprite = progressSprite;
        
        CC_SAFE_RETAIN(m_pBackgroundSprite);
        CC_SAFE_RETAIN(m_pProgressSprite);
        
        addChild(m_pBackgroundSprite , 1);
        addChild(m_pProgressSprite , 2);
        
        setContentSize(m_pBackgroundSprite->getContentSize());
        
        
        m_pBackgroundSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pBackgroundSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        
        m_pProgressSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        m_pProgressSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        

		//加载动画
		m_pFlash = ResourceManager::sharedResourceManager()->getUIAnimate("progress",sp::ZORDER);
		m_pFlash->setVisible(false);
		addChild(m_pFlash);

		//shine
		m_pProgressShining = DMaskedSprite::create();
		CC_SAFE_RETAIN(m_pProgressShining);
		addChild(m_pProgressShining , 3);
		m_pProgressShining->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pProgressShining->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
		m_pProgressShining->setVisible(false);

        this->needsLayout(m_percent);
        return true;
    }
    else
    {
        return false;
    }
}

void DProgressBar::setPercentWithShine(float val)
{
	//percent
	float percent = val;
	float pos = (percent-1.0f) * (m_pProgressShining->getContentSize().width-2);//修正进度条不准的问题 遮罩需要2边各一像素透明减掉2像素
	m_pProgressShining->setMaskPosition(ccp(-pos,0));
	m_pProgressShining->setPosition(ccp(this->getContentSize().width / 2+pos,this->getContentSize().height / 2));	
	int percent100 = percent * 100;

	//shine
	CCNumberTo* pShine = CCNumberTo::create(1.6f,0,255,tween_num_selector(DProgressBar::shining));
	CCNumberTo* pShineReverse = CCNumberTo::create(1.6f,255,0,tween_num_selector(DProgressBar::shining));
	CCRepeatForever* pShineRepeat = CCRepeatForever::create(CCSequence::createWithTwoActions(pShine , pShineReverse));
	runAction(pShineRepeat);	
}

void DProgressBar::setPercent(float val,float dur)
{
    m_percent = MIN(1.0f,MAX(0.0f, val));
    //stopActionByTag(0);
	stopAllActions();

    if(D_FLOAT_EQUALS(dur, 0.f))
    {
        needsLayout(val);
        return;
    }

	CCAction * pAction = CCNumberTo::create(dur,m_realPercent,m_percent,tween_num_selector(DProgressBar::needsLayout));
	pAction->setTag(0);
	runAction(pAction);
    
    //this->needsLayout(dur);
	
}

void DProgressBar::setShining(bool val)
{
	//是否闪烁
	m_pProgressShining->setVisible(val);	
}

void DProgressBar::shining(float val)
{
	m_pProgressShining->setOpacity(val);
}

void DProgressBar::setPercentWithFlash(float val,float dur , int deltalevel , bool showflash )
{
	//上次动画没播完，一定要从上次的末尾开始播
	//没升级，也要注意
	if(m_percent!=0 || deltalevel == 0)
	{
		m_realPercent = m_percent;
	}

	m_percent = MIN(1.0f,MAX(0.0f, val));
	//stopActionByTag(0);
	stopAllActions();
	if(D_FLOAT_EQUALS(dur, 0.f))
	{
		needsLayout(val);
		return;
	}

	//是否播放动画
	m_pFlash->setVisible(showflash);
	m_pFlash->setOpacity(255);

	//处理升级后的两段动画
	if(deltalevel != 0){
		//升级！
		//CCLog(CCString::createWithFormat("----------------------up__now_%f_to_%f" , m_realPercent , m_percent)->getCString());
		CCNumberTo * pAction1 = CCNumberTo::create(dur,m_realPercent,1.0f,tween_num_selector(DProgressBar::needsLayout));
		CCNumberTo * pAction2 = CCNumberTo::create(dur,0.0f,m_percent,tween_num_selector(DProgressBar::needsLayout));
		runAction(CCSequence::create(pAction1 , /*CCDelayTime::create(0.1f) ,*/ pAction2 , CCCallFunc::create(this, callfunc_selector(DProgressBar::OnFlashEnd)) , NULL));
	}
	else{
		//不升级
		//CCLog(CCString::createWithFormat("----------------------no__now_%f_to_%f" , m_realPercent , m_percent)->getCString());
		CCNumberTo * pAction = CCNumberTo::create(dur,m_realPercent,m_percent,tween_num_selector(DProgressBar::needsLayout));
		runAction(CCSequence::create(pAction , CCCallFunc::create(this, callfunc_selector(DProgressBar::OnFlashEnd)) , NULL));
	}
}

void DProgressBar::OnFlashEnd()
{
	if(m_pFlash->isVisible())
	{
		//Fade out
		CCAction * pAction = CCFadeOut::create(0.5f);
		m_pFlash->runAction(pAction);
		//m_pFlash->setVisible(false);
	}
}

void DProgressBar::needsLayout(float percent)
{
    if (NULL == m_pProgressSprite || NULL == m_pBackgroundSprite)
    {
        return;
    }
    
    m_realPercent = percent;
    float pos = (percent-1.0f) * (m_pProgressSprite->getContentSize().width-2);//修正进度条不准的问题 遮罩需要2边各一像素透明减掉2像素
    m_pProgressSprite->setMaskPosition(ccp(-pos,0));
	m_pProgressSprite->setPosition(ccp(this->getContentSize().width / 2+pos,this->getContentSize().height / 2));

	if(m_pFlash->isVisible())
	{
		//如果有动画，动画与进度一致
		m_pFlash->setPosition(ccp(this->getContentSize().width * percent,this->getContentSize().height / 2));
	}		

	int percent100 = percent * 100;
	int maxNotLarger = -1;
	for (std::map<int, cocos2d::ccColor3B>::iterator i = m_mColorForPecent.begin(); i != m_mColorForPecent.end(); i++)
	{
		int key = i->first;
		if (key <= percent100 && key > maxNotLarger)
		{
			maxNotLarger = key;
		}
	}
	if (maxNotLarger >= 0)
	{
		m_pProgressSprite->setColor(m_mColorForPecent[maxNotLarger]);
	}
}

void DProgressBar::setColorWithPercent(cocos2d::ccColor3B color, int percent)
{
	if (percent > 100 || percent < 0)
	{
		return;
	}
	
	m_mColorForPecent[percent] = color;
}





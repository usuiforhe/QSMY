//
//  TipsManager.cpp
//  提示管理器
//
//  Created by ldr123 on 13-6-25.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TipsManager.h"
#include "../components/DTipsComponets.h"
#include "AppMacros.h"

BaseTips::BaseTips()
{
    m_pComponets = DTipsComponets::create(/*nMaxLine*/3, /*nTimeout*/2, /*pFontName*/D_FONT_DEFAULT, /*nFontSize*/D_FONT_SIZE_12, /*colorFont*/cocos2d::ccc3(255,0,0));
    m_pComponets->retain();
}

BaseTips::~BaseTips()
{
    CC_SAFE_RELEASE_NULL(m_pComponets);
}

void BaseTips::append(const std::string& msg, const cocos2d::ccColor3B &colorFont ,  float posy)
{
    CCAssert(m_pComponets, "请初始化相关数据！");
    
    m_pComponets->append(msg.c_str(), colorFont);

	//处理弹框位置，默认0，进阶成功后100
	m_pComponets->setPositionY(posy);
}

void BaseTips::alert(const std::string &msg)
{
    append(msg, cocos2d::ccc3(255,255,255));
    CCLOGINFO(msg.c_str());
}

void BaseTips::warning(const std::string &msg)
{
    append(msg, cocos2d::ccc3(255,255,255));
    CCLOGINFO(msg.c_str());
}

void BaseTips::error(const std::string &msg)
{
    append(msg, cocos2d::ccc3(255,255,255));
    CCLOGINFO(msg.c_str());
}

BaseTips* BaseTips::sharedTips()
{
    static BaseTips s_tips;
    
    return &s_tips;
}

void BaseTips::setParent(cocos2d::CCNode *pLayer)
{
    if (m_pComponets)
    {
        m_pComponets->removeFromParentAndCleanup(false);
    }
    
    pLayer->addChild(m_pComponets);
}
 
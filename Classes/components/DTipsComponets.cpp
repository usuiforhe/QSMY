//
//  DTipsComponets
//  tips组件
//
//  Created by ldr123 on 13-6-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DTipsComponets.h"
#include "AppMacros.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PADDING_LEFT_RIGHT 100.f
#define PADDING_BOTTOM_TOP 30.f
#define THRESHOLD_STRLEN 130

DTipsComponets::DTipsComponets():m_nMaxLine(0), m_nTimeout(0),m_strFontName(""), m_nFontSize(0)
{
    m_vecLabel.clear();
}

DTipsComponets::~DTipsComponets()
{
    m_vecLabel.clear();
}

DTipsComponets* DTipsComponets::create(int nMaxLine, int nTimeout, const char *pFontName, int nFontSize, const ccColor3B &colorFont)
{
    DTipsComponets *pRet = new DTipsComponets();
    if (pRet && pRet->init(nMaxLine, nTimeout, pFontName, nFontSize, colorFont))
    {
        pRet->autorelease();
        return pRet;
    }
    
    delete pRet;
    return NULL;
}

bool DTipsComponets::init(int nMaxLine, int nTimeout, const char *pFontName, int nFontSize, const ccColor3B &colorFont)
{
    if (!CCNode::init())
    {
        return false;
    }
    
    m_nMaxLine  = nMaxLine;
    m_nTimeout  = nTimeout;
    
    if (pFontName)
    {
        m_strFontName = pFontName;
    }
    else
    {
        m_strFontName = "";
    }
    
    m_nFontSize = nFontSize;
    m_colorFont = colorFont;
    
    return check();
}

bool DTipsComponets::check()
{
    //非法数据
    return !(m_nMaxLine==0 || m_strFontName=="" || m_nFontSize==0);
}

void DTipsComponets::append(const char *pMsg)
{
    append(pMsg, m_colorFont);    
}

void DTipsComponets::append(const char *pMsg, const cocos2d::ccColor3B &colorFont)
{
    if (!pMsg || strlen(pMsg)<1)
    {
        return;
    }
    
    if (m_vecLabel.size() >= m_nMaxLine)
    {
        t_VecLabel::iterator itLabel = m_vecLabel.begin();
        if (itLabel != m_vecLabel.end())
        {
            CCNode *pLabel = *itLabel;
            if (pLabel)
            {
                pLabel->removeFromParent();
            }
            
            m_vecLabel.erase(itLabel);
        }
    }
    
    CCNode* pNode = CCNode::create();
    CCLabelTTF *pLabel = CCLabelTTF::create(pMsg, m_strFontName.c_str(), m_nFontSize);
    CCScale9Sprite* pBg = CCScale9Sprite::create("PL_ui/all/all_jinjiejiantou_di.png");
    
	if (strlen(pMsg) > THRESHOLD_STRLEN)
	{
		pBg->setPreferredSize(CCSize(pLabel->getContentSize().width+PADDING_LEFT_RIGHT / 2,pLabel->getContentSize().height+PADDING_BOTTOM_TOP));
	}
	else
	{
		pBg->setPreferredSize(CCSize(pLabel->getContentSize().width+PADDING_LEFT_RIGHT,pLabel->getContentSize().height+PADDING_BOTTOM_TOP));
	}
    pBg->setInsetLeft(25.f);
    pBg->setInsetRight(25.f);
    pLabel->setColor(colorFont);
    pLabel->runAction(CCSequence::create(CCDelayTime::create(m_nTimeout), CCFadeOut::create(0.5f), CCCallFuncN::create(this, callfuncN_selector(DTipsComponets::eraseFromAction)), NULL));
    pBg->runAction(CCSequence::create(CCDelayTime::create(m_nTimeout), CCFadeOut::create(0.5f), NULL));
    
    pNode->addChild(pBg);
    pNode->addChild(pLabel);
    pNode->setContentSize(pBg->getContentSize());
    addChild(pNode);
    
    m_vecLabel.push_back(pNode);
    
    refresh();
}

void DTipsComponets::eraseFromAction(cocos2d::CCObject *obj)
{
    CCNode *pNode = (CCNode*)obj;
    if (!pNode)
    {
        return;
    }
    
    t_VecLabel::iterator itLabel = m_vecLabel.begin();
    while (itLabel != m_vecLabel.end())
    {
        CCNode *pLabel = *itLabel;
        if (pLabel && pLabel==pNode->getParent())
        {
            pLabel->removeFromParent();
            m_vecLabel.erase(itLabel);
            
            refresh();
            
            break;
        }
        
        itLabel++;
    }
}

void DTipsComponets::refresh()
{
    if (m_vecLabel.empty())
    {
        return;
    }
    
    CCNode *pParent = getParent();
    if (!pParent)
    {
        return;
    }
    
    CCPoint ptCenter = ccp(pParent->getContentSize().width/2, pParent->getContentSize().height/2);
    
    float nHeight = 0.f;
    int nSize = m_vecLabel.size();
    for (int i=0; i<nSize; ++i)
    {
        CCNode *pLabel = m_vecLabel[nSize-i-1];
        pLabel->setPosition(ccp(ptCenter.x, ptCenter.y + nHeight));
        nHeight += pLabel->getContentSize().height + 1.f;
    }    
}





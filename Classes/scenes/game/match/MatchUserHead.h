//
//  MatchUserHead.h
//  杯赛所有出现的用户头像
//
//  Created by wanghejun on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchUserHead__
#define __QSMY__MatchUserHead__

#include "AppInclude.h"
#include "model/MatchModel.h"
#include "components/components.h"

class MatchUserHead
:public DLayer//cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
   
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, HeadContainer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pImgWin, ImgWin);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pImgLose, ImgLose);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pImgBeted, ImgBeted);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pTfName, TfName);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pTfLevel, TfLevel);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pPlayerImg,PlayerImg);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pDefaultImg, DefaultImg);
    
public:
    
    void setVSInfo(match::VSInfo* pVsInfo, match::VS vs);

	void updateView();

    CREATE_FUNC(MatchUserHead);
    MatchUserHead();
    virtual ~MatchUserHead();
    D_INIT_DEFAULT(DLayer);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	cocos2d::CCNode		*m_pBgWin;
	cocos2d::CCNode		*m_pBgLose;

protected:
	match::VSInfo *m_pVsInfo;
	match::VS m_eVS;
};


class MatchUserHeadLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MatchUserHeadLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MatchUserHead);
};

#endif /* defined(__QSMY__MatchUserHead__) */

//
//  DProgressBar.h
//  QSMY
//
//  Created by wanghejun on 13-5-31.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DProgressBar__
#define __QSMY__DProgressBar__

#include "AppInclude.h"
#include "DMaskedSprite.h"
#include "../utils/ActionUtil.h"
#include "../../libs/SPArmature/SPArmature.h"

class DProgressBar:public cocos2d::CCNode
{
    
public:
    
    DProgressBar();
    virtual ~DProgressBar();
    
    CREATE_FUNC(DProgressBar);
    
    virtual bool init();
    
    static DProgressBar* create(const char *pbgFileName,const char *pprogressFileName,const char *pmaskFileName);
    
    virtual bool initWithSprites(cocos2d::CCSprite * backgroundSprite, DMaskedSprite* progressSprite);
    
    static DProgressBar* create(cocos2d::CCSprite * backgroundSprite, DMaskedSprite* pogressSprite);
    
    
    CC_SYNTHESIZE_READONLY(float, m_percent, Percent);
    virtual void setPercent(float val,float dur=0.0f);
	//进度条更新带动画
    virtual void setPercentWithFlash(float val,float dur=0.0f , int deltalevel = 0 , bool showflash = false);
	//进度条动画callback
	virtual void OnFlashEnd();
	//进度条闪烁
	virtual void setPercentWithShine(float val);
    
    void setBgSpriteFrame(cocos2d::CCSpriteFrame*);
    void setBarSpriteFrame(cocos2d::CCSpriteFrame*);
    void setMaskSpriteFrame(cocos2d::CCSpriteFrame*);

	void setColorWithPercent(cocos2d::ccColor3B color, int percent);
    
	//该进度条是否要闪闪闪闪闪
	void setShining(bool val);
protected:
    
    void needsLayout(float percent);
    void shining(float percent);
protected:
    
    DMaskedSprite* m_pProgressSprite;
    cocos2d::CCSprite * m_pBackgroundSprite;
    float m_realPercent;

	std::map<int, cocos2d::ccColor3B> m_mColorForPecent;

	sp::Armature* m_pFlash;
    DMaskedSprite* m_pProgressShining;
};
#endif /* defined(__QSMY__DProgressBar__) */


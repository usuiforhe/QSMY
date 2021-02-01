//
//  DMarqueeView.h
//  跑马灯
//
//  Created by wanghejun on 13-3-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DMarqueeView__
#define __QSMY__DMarqueeView__

#include "AppInclude.h"
#include "DLayer.h"
#include "./label/CCRichLabelTTF.h"

struct DMarqueeViewDataSouce
{
    virtual ~DMarqueeViewDataSouce(){}
    virtual cocos2d::CCString * getMarqueeText() = 0;
};

class DMarqueeView:public DLayer{
    
    CC_SYNTHESIZE(int, m_fSpeed, Speed);
    CC_SYNTHESIZE(int, m_iPadding, Padding);
    CC_SYNTHESIZE_READONLY(uint32_t, m_iStack, Stack);

	CC_SYNTHESIZE(bool, m_bFirst, First);
        
public:
    
    DMarqueeView();
    virtual ~DMarqueeView();
    
    virtual bool init();
    
    CREATE_FUNC(DMarqueeView);
    static DMarqueeView* create(int width);
    static DMarqueeView* create(int width,
                                cocos2d::ccColor3B fontColor,
                                const char* fontName,
                                float fontSize);
    
    void addString(const char* str);
    void addString(cocos2d::CCString *str);
    
    void setStack(uint32_t num);
    
    void setDataSource(DMarqueeViewDataSouce * pDataSource);
    
    void onEnter();
    void onExit();
    
    
protected:
    
    virtual bool initWithWidth(int width);
    virtual bool initWithWidthAndFont(int width,
                                     cocos2d::ccColor3B fontColor,
                                     const char* fontName,
                                     float fontSize);
    
    void moveLabel(cocos2d::CCRichLabelTTF *move,cocos2d::CCRichLabelTTF *forward,float dt);
    
    cocos2d::CCString * getText();
    
private:
    
    cocos2d::CCArray *m_pStringStack;
    cocos2d::CCRichLabelTTF * m_pLabel1;
    cocos2d::CCRichLabelTTF * m_pLabel2;

    virtual void update(float fDelta);
    
    uint32_t m_currntPtr;
    
    bool m_bLabel1Move;
    
    //bool m_bFirst;
    
    DMarqueeViewDataSouce * m_pDataSource;
};



#endif /* defined(__QSMY__DMarqueeView__) */

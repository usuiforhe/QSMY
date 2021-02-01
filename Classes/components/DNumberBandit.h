//
//  DNumberBandit.h
//  转数字
//
//  Created by wanghejun on 13-11-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DNumberBandit__
#define __QSMY__DNumberBandit__

#include "AppInclude.h"

class DNumberBandit:public cocos2d::CCNode
{
public:
    DNumberBandit();
    ~DNumberBandit();
    CREATE_FUNC(DNumberBandit);
    
    /**
     *	@brief	创建一个对象
     *
     *	@param 	lap 	总圈数
     *	@param 	char_height 	字符高度
     *	@param 	sPath 	字符图片路径
     *
     *	@return	对象
     */
    static DNumberBandit* create(unsigned int lap,float char_height,const char* sPath);

    
    bool init();
    
    bool init(unsigned int lap,float char_height,const char* sPath);
    
    CC_SYNTHESIZE(unsigned int, m_nTextCount, TextCount);
    void setToTarget(unsigned char target);
    
    /**
     *	@brief	开始转
     *
     *	@param 	delay 	延迟开始
     *	@param 	target 	目标数字(0~9)
     */
    void start(float delay,unsigned char target);

    
    /**
     *	@brief	获取本次旋转持续时间
     *
     *	@return	持续时间
     */
    float getDur();
    
    void animationEndCallBack();

    
protected:
    cocos2d::CCNode          *m_pNumber;
    cocos2d::CCClippingNode  *m_pClippingNode;
    
    unsigned int              m_nLastTarget;
    unsigned int              m_nLap;
    float                     m_fCharHeight;
    std::string               m_sPath;
    float                     m_fDur;
};

#endif /* defined(__QSMY__DNumberBandit__) */

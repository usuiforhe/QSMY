//
//  DTipsComponets
//  tips组件
//
//  Created by ldr123 on 13-6-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DTipsManager__
#define __QSMY__DTipsManager__

#include "AppInclude.h"

class DTipsComponets : public cocos2d::CCNode
{
public:
    DTipsComponets();
    virtual ~DTipsComponets();
public:
    virtual bool init(int nMaxLine, int nTimeout, const char *pFontName, int nFontSize, const cocos2d::ccColor3B &colorFont);
    virtual void append(const char *pMsg);
    virtual void append(const char *pMsg, const cocos2d::ccColor3B& colorFont);
public:
    static DTipsComponets* create(int nMaxLine, int nTimeout, const char *pFontName, int nFontSize, const cocos2d::ccColor3B &colorFont);
private:
    bool check();
    void eraseFromAction(cocos2d::CCObject *);
    void refresh();
private:
    typedef std::vector<cocos2d::CCNode*> t_VecLabel;
    int                 m_nMaxLine;     // 最大行数
    int                 m_nTimeout;     // 如果没有被顶掉，显示多久（s）
    std::string         m_strFontName;  // 字体信息
    int                 m_nFontSize;
    cocos2d::ccColor3B  m_colorFont;    // 字体颜色
    t_VecLabel          m_vecLabel;     // label管理器
};

#endif /* defined(__QSMY__DTipsManager__) */

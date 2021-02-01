//
//  TipsManager.h
//  提示管理器
//
//  Created by ldr123 on 13-6-25.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TipsManager__
#define __QSMY__TipsManager__
/*
 sample
    xxLayer *pLayer = xx:create();
    TIPSMANAGER->setParent(pLayer);
        ......
    Tips_Alert("fk")
*/

#include "AppInclude.h"
#include <string>
class DTipsComponets;

class BaseTips:public cocos2d::CCObject
{
public:
    BaseTips();
    virtual ~BaseTips();
public:
    static BaseTips* sharedTips();
public:
    void setParent(cocos2d::CCNode *pLayer);
public:
    void append(const std::string& msg, const cocos2d::ccColor3B &colorFont , float posy = 0.0f);
    
    void alert(const std::string& msg);
    
    void warning(const std::string& msg);
    
    void error(const std::string& msg);
protected:
    DTipsComponets *m_pComponets;
};

#define TIPSMANAGER BaseTips::sharedTips()

//用个临时变量避免两次调用_msg对应的东西，比如Tips_Alert(fk[rand()%xx]);
#define Tips_Alert(_msg)     do{ TIPSMANAGER->alert(_msg); }while(0)
#define Tips_Warning(_msg)   do{ TIPSMANAGER->warning(_msg); }while(0)
#define Tips_Error(_msg)     do{ TIPSMANAGER->error(_msg); }while(0)


#endif /* defined(__QSMY__TipsManager__) */

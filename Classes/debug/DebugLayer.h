//
//  Debug.h
//  QSMY
//
//  Created by wanghejun on 13-5-25.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Debug__
#define __QSMY__Debug__

#include "AppInclude.h"
#include "../components/components.h"
#include "json/json.h"

class DebugButton;

class DebugLayer:public DPopup
{
public:
    DebugLayer()
    :m_pPveInput(NULL)
    ,m_pCtrlButton(NULL)
	,m_pSkillNameBMFont(NULL)
	,m_pSkillNameTTF(NULL)
    {
        
    }
    virtual ~DebugLayer()
    {
        
    }
    CREATE_FUNC(DebugLayer);
    virtual bool init();
    
    CC_SYNTHESIZE(DebugButton*, m_pCtrlButton, CtrlButton); //weak
private:
    void execute(cocos2d::CCObject*);
    
    void testBattle();
    void testPve();
    void startBattle(const Json::Value &,const Json::Value &);
    void changeServer();
    void loadBattle();
	void testSkillName();
    
private:    
    cocos2d::extension::CCEditBox   *m_pPveInput;
	cocos2d::CCLabelBMFont* m_pSkillNameBMFont;
	cocos2d::CCLabelTTF* m_pSkillNameTTF;

};



class DebugButton:public cocos2d::CCNode {
public:
    DebugButton()
    :m_pDebugLayer(NULL)
    {};
    virtual ~DebugButton(){
        CC_SAFE_RELEASE(m_pDebugLayer);
    }
    CREATE_FUNC(DebugButton);
    virtual bool init();
private:
    void toggleDebugLayer(cocos2d::CCObject*);
private:
    DebugLayer                      *m_pDebugLayer;
    
};

#endif /* defined(__QSMY__Debug__) */

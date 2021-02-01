//
//  kingTombfight.h
//  LibQSMY
//  战斗 信息界面
//  Created by jk on 14-8-5.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__KingTombfight__
#define __LibQSMY__KingTombfight__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "model/KingTombModel.h"
#include "managers/DataManager.h"
#include "KingTombfightCell.h"

#define TOMB_FIGHT_ROW	3
using namespace cocos2d;

class KingTombDelegate
{
public:
    virtual void openFightPanel() = 0;
    virtual void openGuildListPanel(uint32_t index) = 0;
};


class KingTombfight
:public DPopup
, public cocos2d::extension::CCBMemberVariableAssigner
, public KingTombfightCellDelegate
{
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(uint32_t, m_pos, Pos);
    CC_SYNTHESIZE(KingTombDelegate *, m_pDelegate, Delegate);
public:
    KingTombfight();
    ~KingTombfight();
    CREATE_FUNC( KingTombfight ) ;
	virtual bool init() ;
    virtual void onEnter();
	virtual void onExit() ;
    
	virtual void onBackKey(); //for android back key

    
public:
    
    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject *pTarget,
                                           const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;
	virtual void addAttack(uint32_t index);
private:
    KingTombModel *m_pmodel;
    CCRichLabelTTF *m_pNameTf;
    KingTombfightCell * m_pfightCell1;
    KingTombfightCell * m_pfightCell2;
    KingTombfightCell * m_pfightCell3;
	cocos2d::CCNode*		m_pIndex[TOMB_FIGHT_ROW];
	DButton	*m_pLeft;
	DButton	*m_pRight;

    CCArray *m_pFightCells;
    void fightCallBack(cocos2d::CCObject *sender);
    void backBtnCallBack(cocos2d::CCObject *sender);
    void updateView();
    


    
public:
    void setData(int pos);

};


#endif /* defined(__LibQSMY__KingTombfight__) */

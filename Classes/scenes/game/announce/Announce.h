//
//  Announce.h
//  公告
//
//  Created by 汪燕军 on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Announce__
#define __QSMY__Announce__

#include "AppInclude.h"
#include "components/components.h"

namespace announce {
    class AnnounceInfo;
}


class AnnounceDelegate {
public:
    virtual void NavigationTo(announce::AnnounceInfo *) = 0;
};

class Announce
: public DPopup
, public cocos2d::extension::CCTableViewDelegate
, public cocos2d::extension::CCTableViewDataSource
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
, public AnnounceDelegate
{
public:
    
    Announce();
    virtual ~Announce();
    
    CREATE_FUNC(Announce);
    
    virtual bool init();
    
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){ return NULL;};
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell){}
    
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
        
private:
    
    void NavigationTo(announce::AnnounceInfo *);

    void closeCallBack(cocos2d::CCObject * sender);
    
private:
    cocos2d::CCNode * m_pContainer;
    DTableView * m_pTableView;
    cocos2d::extension::CCData * m_pData;
    std::map<unsigned int,CCSize>  m_sizeMap;
};

#endif /* defined(__QSMY__Announce__) */

//
//  Guide.h
//  攻略
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Guide__
#define __QSMY__Guide__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SettingModel.h"

class Guide
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
public:
    Guide();
    virtual ~Guide();
    CREATE_FUNC(Guide);
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

private:
    cocos2d::CCNode     *m_pContainer;
    cocos2d::extension::CCData  *m_pCellData;
    DTableView          *m_pTableView;
};


class GuideCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    GuideCell();
    ~GuideCell();
    static GuideCell* create(cocos2d::extension::CCData* pData);
    bool init(cocos2d::extension::CCData* pData);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setGuideInfo(settings::GuideInfo*);
private:
    void updateView();
private:
    cocos2d::CCLabelTTF     *m_pInfoTTF;
    cocos2d::CCLabelTTF     *m_pTitleTTF;
    
    settings::GuideInfo*    m_pGuideInfo;
};

#endif /* defined(__QSMY__Guide__) */

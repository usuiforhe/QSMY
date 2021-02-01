//
//  WarriorUpgrade.h
//  英雄强化（升级）
//  Created by zhanglong on 14-7-21.
//  Copyright (c) 2014, thedream. All rights reserved.
//

#ifndef __PL_WarriorUpgrade_H__
#define __PL_WarriorUpgrade_H__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "../common/common.h"
#include "DragonBone.h"

#define WARRIOR_ENHANCE_ACTION "warrior_enhance"

class WarriorDelegate;
class WarriorGradeBg;

class WarriorUpgradeDelegate
{
public:
    virtual void itemUseCallBack(unsigned int itemId) = 0;
    virtual void itemKeepUseCallBack(unsigned int itemId, unsigned int amountPerUse = 1){};
	virtual void itemKeepUseEndCallBack(unsigned int itemId, unsigned int count){};
	virtual bool isLvlLimit(unsigned int itemId)=0;
};

class WarriorUpgradeView : public  DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public WarriorUpgradeDelegate
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
public:
	WarriorUpgradeView();
	~WarriorUpgradeView();

	CREATE_FUNC(WarriorUpgradeView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setWarrior(warrior::WarriorCurInfo * pWarriorInfo);
    void setWarriorDelegate(WarriorDelegate* delegate);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    //WarriorUpgradeDelegate
    virtual void itemUseCallBack(unsigned int itemId);
    virtual void itemKeepUseCallBack(unsigned int itemId,  unsigned int amountPerUse = 1);
	virtual void itemKeepUseEndCallBack(unsigned int itemId, unsigned int count);
	virtual bool isLvlLimit(unsigned int itemId);
    
    //tableview delegate
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};

	void upgradeResultCallBack(const Json::Value &requestData, const Json::Value &responseData);
    void animationPlayEnd(sp::Armature * target, const char* event,const char* data);
private:
	void binding();
	void updateWarriorInfo();
	void updateItems();

	void closeCallBack(cocos2d::CCObject* sender);
	void itemButtonCallBack(cocos2d::CCObject* sender);

	WarriorDelegate * m_pDelegate;
	warrior::WarriorCurInfo *m_pWarriorInfo;
	WarriorModel * m_pWarriorModel;
    DTableView*      m_pTableView;
    cocos2d::CCArray*   m_pUpgradeInfoArray;
	cocos2d::CCArray*   m_pItemInfoArray;

	//UI
private:
    cocos2d::extension::CCData  *m_pCCBData;
    cocos2d::CCMenuItemImage    *m_pReturnButton;
    cocos2d::CCNode         *m_pContainer;
    cocos2d::CCNode         *m_pTableNode;
    cocos2d::CCNode         *m_pAnimationNode;
	cocos2d::CCNode			*m_pWarriorSprite;	//弟子头像
	cocos2d::CCLabelTTF     *m_pDesc;			//弟子描述
	SkillBox				*m_pSkillBox1;		//技能1
	SkillBox				*m_pSkillBox2;		//技能2
	cocos2d::CCLabelBMFont *m_pFight;
	cocos2d::CCLabelBMFont	*m_pLevel;
    cocos2d::CCLabelBMFont  *m_pUpLevel;
	cocos2d::CCLabelTTF		*m_pName;
	WarriorGradeBg			*m_pWarriorGradeBg;
    WarriorTypeMark         *m_pWarriorTypeMark;
    //DProgressBar            *m_pProgressBar;
	DProgressBar	*m_pProgressBar[WARRIOR_GRADE_NUM];
    
    cocos2d::CCLabelBMFont* m_pAttackLabel;
    cocos2d::CCLabelBMFont* m_pAddAttackLabel;
    cocos2d::CCLabelBMFont* m_pDefenceLabel;
    cocos2d::CCLabelBMFont* m_pAddDefenceLabel;
    cocos2d::CCLabelBMFont* m_pHpLabel;
    cocos2d::CCLabelBMFont* m_pAddHpLabel;
    cocos2d::CCLabelBMFont* m_pSpeedLabel;
    cocos2d::CCLabelBMFont* m_pAddSpeedLabel;

	bool bProgressUpdate;
};


class WarriorUpgradeItemCell : public CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    WarriorUpgradeItemCell();
    ~WarriorUpgradeItemCell();
    
    static WarriorUpgradeItemCell* create(CCData* ccbData);
    
    virtual bool init(cocos2d::extension::CCData * pData);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setItemId(unsigned int itemId);
    unsigned int getItemId(){return m_itemId;};
    void updateView();
    
    CC_SYNTHESIZE(WarriorUpgradeDelegate*, m_pDelegate, Delegate);
    
    //CCObject*, CCControlEvent
    void upgradeButtonCallBack(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
    void upgradeButtonLongPressCallBack(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
	void upgradeButtonLongPressEndCallBack(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
private:
    unsigned int            m_itemId;
	unsigned int			   m_itemUseCount;//长按时记录用
    cocos2d::CCNode*        m_pCountNode;
    cocos2d::CCNode*        m_pIconContainer;
    cocos2d::CCLabelTTF*    m_pCountLabel;
    cocos2d::extension::CCControlButton* m_pTouchMenu;
	unsigned int m_nSoundID;
};


//class DLongPressButton : public cocos2d::extension::CCControlButton
//{
//public:
//    static DLongPressButton* create(cocos2d::extension::CCScale9Sprite* sprite);
//    virtual bool initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite* sprite);
//    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    
//    void waitForLongPress(float t);
//    void longPressCallBack(float t);
//private:
//    float   m_TouchDownTime;
//};



#endif
//
//  Promotion.h
//  弟子突破界面
//  Created by cll on 13-3-28.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__PROMOTION__
#define __QSMY__PROMOTION__

#include "AppInclude.h"
#include "../common/common.h"
#include "model/WarriorModel.h"
#include "DragonBone.h"

class WarriorDelegate;
class PromotionSettleView;
class PromotionItemListView;

class PromotionView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
public:
	PromotionView();
	~PromotionView();


	CREATE_FUNC(PromotionView);
    
	virtual bool init();

	virtual void onEnter(); 

	virtual void onExit();

	void setWarrior(warrior::WarriorCurInfo * pWarrior);

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    


private:
	void binding();
	void updatePromotion();
	void confirmButtonCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
    void promotionCallBack(const Json::Value &requestData,const Json::Value &responseData);
    void animateEnd(sp::Armature * target, const char* event,const char* data);
	void updateNextTalentName();

private:
	WarriorDelegate         *m_pDelegate;
	warrior::WarriorCurInfo *m_pWarriorInfo;
	WarriorModel            *m_pWarriorModel;
    sp::Armature            *m_pEffect;
	PromotionSettleView* m_pSettleView;
private:
    cocos2d::CCNode			*m_pContainer;      //弟子头像
	cocos2d::CCLabelBMFont		*m_pCurUpLevel;		//突破等级
	cocos2d::CCLabelBMFont		*m_pToUpLevel;		//突破等级
	cocos2d::CCNode					*m_pTableNode;
    
    cocos2d::CCLabelBMFont  *m_pCurProps;       //当前属性
    cocos2d::CCLabelBMFont  *m_pToProps;        //提升后属性
    
    cocos2d::CCLabelBMFont  *m_pCurItems;       //当前魂魄数
    
    cocos2d::CCSprite		*m_pGrade;			//品阶
	cocos2d::CCLabelBMFont	*m_pLevel;
	cocos2d::CCLabelTTF		*m_pName;

	std::string m_nextTalentName;

	CCLabelBMFont*	m_pHp;
	CCLabelBMFont*	m_pAttack;
	CCLabelBMFont*	m_pDefence;
	//CCLabelBMFont*	m_pSpeed;
    CCLabelBMFont*      m_pPotential;
	CCLabelBMFont*	m_pAddHp;
	CCLabelBMFont*	m_pAddAttack;
	CCLabelBMFont*	m_pAddDefence;
	//CCLabelBMFont*	m_pAddSpeed;
    CCLabelBMFont*	m_pAddPotential;

	CCSprite*	 m_pAttackArrow;
	CCSprite*	 m_pDefenceArrow;
	CCSprite*	m_pHpArrow;
	CCSprite*	m_pSpeedArrow;

	CCLabelBMFont*	m_pCurrentFight;
	CCLabelBMFont*	m_pNextFight;

	PromotionItemListView		*m_pListView;
    
    DButton                 *m_pConfirmButton;   //提交按钮
	DButton				  *m_pCancelButton;
    
    cocos2d::CCNode         *m_pEffectNode;
    
    WarriorTypeMark         *m_pWarriorTypeMark;
    WarriorGradeBg          *m_pWarriorGradeBg;
    
	cocos2d::CCNode	*m_pColorGrade[WARRIOR_GRADE_NUM];
};


class PromotionListItemCell;
#pragma mark - PromotionListView

class PromotionItemListView
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
#pragma  mark - - PromotionItemListView init etc.
public:
	PromotionItemListView();
	~PromotionItemListView();
	CREATE_FUNC(PromotionItemListView);
	virtual bool init();
	void setWarrior(warrior::WarriorCurInfo *pWarriorInfo);
	void updateView();
#pragma mark - - PromotionItemListView variable
private:
	CC_SYNTHESIZE(DTableView*, m_pTableView, TableView);
	cocos2d::extension::CCData	*m_pCellData;
	warrior::WarriorCurInfo *m_pWarriorInfo;
#pragma mark - - PromotionItemListView CCTableViewDelegate
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    void computeTableCellsCount();
    
    uint32_t m_uTableCellsCount;
};


#pragma mark -PromotionListItemCell
class PromotionListItemCell 
: public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	PromotionListItemCell();
	~PromotionListItemCell();
	virtual bool init(cocos2d::extension::CCData *pData);
	static PromotionListItemCell* create(cocos2d::extension::CCData *pData);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void setIconSprite(cocos2d::CCSprite* icon);
	void setItemCountString(CCString* countString);
	void setItemName(std::string nameString);
private:
	void updateView();

	ItemBox*								m_pItemBox;
};


class PromotionSettleView
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	PromotionSettleView();
	~PromotionSettleView();

	CREATE_FUNC(PromotionSettleView);
	virtual bool init();
	void setWarriorInfo(warrior::WarriorCurInfo * pWarriorInfo);

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){};

	virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

	void closeButtonCallBack(cocos2d::CCObject* obj);
private:
	void updateWarriorSettle();
	warrior::WarriorCurInfo * m_pWarriorInfo;

	cocos2d::CCNode			*m_pContainer;      //弟子头像
	cocos2d::CCLabelBMFont		*m_pRealUpLevel;
	cocos2d::CCLabelBMFont		*m_pCurUpLevel;		//突破等级(对比显示用)
	cocos2d::CCLabelBMFont		*m_pToUpLevel;		//突破等级(对比显示用)
	cocos2d::CCSprite		*m_pGrade;			//品阶
	cocos2d::CCLabelBMFont	*m_pLevel;
	cocos2d::CCLabelTTF		*m_pName;

	CCLabelBMFont*	m_pHp;
	CCLabelBMFont*	m_pAttack;
	CCLabelBMFont*	m_pDefence;
	//CCLabelBMFont*	m_pSpeed;
    CCLabelBMFont*	m_pPotential;
	CCLabelBMFont*	m_pAddHp;
	CCLabelBMFont*	m_pAddAttack;
	CCLabelBMFont*	m_pAddDefence;
	//CCLabelBMFont*	m_pAddSpeed;
    CCLabelBMFont*	m_pAddPotential;

	CCSprite*	 m_pAttackArrow;
	CCSprite*	 m_pDefenceArrow;
	CCSprite*	m_pHpArrow;
	CCSprite*	m_pSpeedArrow;
	WarriorTypeMark         *m_pWarriorTypeMark;
    WarriorGradeBg          *m_pWarriorGradeBg;

	CCNode*	m_pFadeInNode;

	cocos2d::CCNode	*m_pColorGrade[WARRIOR_GRADE_NUM];
};
#endif /* defined(__QSMY__PRPMOTION__) */

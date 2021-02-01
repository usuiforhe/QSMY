//
//  FormationView.h
//  阵容
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FormationView__
#define __QSMY__FormationView__

#include "AppInclude.h"
#include "Formation.h"
#include "../common/common.h"
#include "../common/gui/WarriorHeadGradeBg.h"
#include "model/GundamEngineerFModel.h"


#define EQUIP_BOX_COUNT 4
#define MAX_FATE_COUNT 6
#define PARTNER_NUM    8

class FormationViewDelegate
{
public:
	virtual void setCurrentPage(uint8_t i) = 0;
	virtual uint8_t getCurrentPage() = 0;
    virtual void showPartnerView(bool showFlag){};
};

class FormationListView;
class FormationWarriorInfoView;
class FormationListViewCell;
class GundamFateListView;
#pragma mark - FormationView
class FormationView
:public DLayer
,public DPageViewDataSource
,public cocos2d::extension::CCScrollViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public FormationViewDelegate
{

public:
    FormationView();
    ~FormationView();
    virtual bool init();
    CREATE_FUNC(FormationView);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual float cellSizeForTable(DPageView *view);
    virtual cocos2d::CCNode* cellAtIndex(DPageView *view, unsigned int idx);
    virtual uint32_t numberOfCellsInTableView(DPageView *view);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {return NULL;};
	void updateFormation();
private:
    CC_SYNTHESIZE(FormationDelegate*, m_pDelegate, Delegate);
    //CC_SYNTHESIZE_READONLY(int8_t, m_iCurrentPage, CurrentPage);
    int8_t m_iCurrentPage;

    FormationListView           *m_pFormationListView;
    DPageView                   *m_pPageView;
    FormationModel              *m_pFormationModel;
    WarriorModel                *m_pWarriorModel;
    cocos2d::CCArray            *m_pWarriorInfoViews;
    EquipModel                  *m_pEquipModel;
    GundamEngineerFModel		*m_pGundamEFModel;
    warrior::WarriorCurInfo     *m_pWarriorInfo;
	GundamFateListView			*m_pGundamFateListView;
	enum FormationCallbackTag
	{
		kChangeFormationTag,
		kQuickEquipTag,
		kChangeHeroTag
	};

private:
	void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
	void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    

private:
    
    void adjustScrollView(float offset);
    
    void showFormations();
    void updateView();
    
    void updateInfo();
    
    int  pageToPositionIdx(const int &page);
    
    int  getPageIndexForWarriorID(const uint32_t &wid);
    
	void callbackFormation(cocos2d::CCObject* sender);
	void quickEquip();
	void playVoiceCallback(cocos2d::CCObject *sender);
	void stopHeroSound(/*float delay=0*/);

public:
    void setCurrentPage(uint8_t i);
	uint8_t getCurrentPage(){
		return m_iCurrentPage;
	}

    void touchWarriorIcon(warrior::WarriorCurInfo* pWarrior);
    void showWarrior(const uint32_t &wid);
    
    void touchSkillCallBack(cocos2d::CCObject* sender);
    void touchEquipCallBack(cocos2d::CCObject* sender);
    
    void touchWarrior(cocos2d::CCObject* sender);
	void onCallBack(const Json::Value&,const Json::Value& responeData);
    void showPartnerView(bool showFlag);
    
    void partnerViewUpdate();
    void partnerCallback(cocos2d::CCObject *);
	void avatarCallback(cocos2d::CCObject *);
private:

	DButton					*m_pCloseButton;

    cocos2d::CCLabelBMFont  *m_pAttack;                 //攻
    cocos2d::CCLabelBMFont  *m_pSpeed;                  //速
    cocos2d::CCLabelBMFont  *m_pDefence;                //防
    cocos2d::CCLabelBMFont  *m_pHp;                     //血

	cocos2d::CCLabelBMFont  *m_pLevel;              //级别
	cocos2d::CCLabelTTF     *m_pName;               //名字
	cocos2d::CCLabelBMFont  *m_pPrice;                  //身价
	WarriorTypeMark         *m_pWarriorTypeMark;
	//DProgressBar            *m_pProgressBar;
    DProgressBar	*m_pProgressBar[WARRIOR_GRADE_NUM];
    
    cocos2d::CCLabelBMFont  *m_pTotalPrice;             //总身价
    EquipBox                *m_pEquipBox[EQUIP_BOX_COUNT];
    cocos2d::CCNode         *m_pEquipNone[EQUIP_BOX_COUNT];
    SkillBox                *m_pSkillBox;
    SkillBox                *m_pSkillBoxSP;
    cocos2d::CCLabelTTF     *m_pFate[MAX_FATE_COUNT];   //缘
    cocos2d::CCSprite     *m_pFateHighlight[MAX_FATE_COUNT];   //缘
    cocos2d::CCNode         *m_pWarriorListContainer;   //弟子列表
    cocos2d::CCNode         *m_pWarriorInfoContainer;   //弟子信息容器
    cocos2d::CCLabelBMFont         *m_pUpLevel;
    
    cocos2d::extension::CCData  *m_pWarriorData;          //弟子信息CCB
	cocos2d::CCNode          *m_pFateDisable;//没有缘信息的边框
	cocos2d::CCNode          *m_pFateEnable;//有缘信息的边框
	cocos2d::CCMenuItem          *m_pQuickEquip;//一键装备
	cocos2d::CCMenuItem          *m_pChangeHero;//更换英雄
	cocos2d::CCMenuItem          *m_pFateMenu;//有缘信息菜单触发
	WarriorGradeBg			*m_pWarriorGradeBg;
	cocos2d::CCMenuItem     *m_pHorn;
	unsigned int m_nSoundID;
	sp::Armature* m_pArmature;
	cocos2d::CCLabelBMFont  *m_pZizhiNum;
	cocos2d::CCNode         *m_pZizhiBg;
    cocos2d::CCNode         *m_pPartnerNode;
    cocos2d::CCNode         *m_pFormationNode;
    cocos2d::CCNode             *m_pPartner[PARTNER_NUM];
    FormationListViewCell       *m_pPartnerCells[PARTNER_NUM];
    cocos2d::extension::CCData  *m_pWHeadData;//warrior head icon ccb
	cocos2d::CCNode			*m_pGundamFateListContainer;
    cocos2d::CCNode         *m_pGundamFateListTopContainer;
    cocos2d::CCLabelBMFont  *m_pPartnerAddAttack;
    cocos2d::CCLabelBMFont  *m_pPartnerAddDefence;
    cocos2d::CCLabelBMFont  *m_pPartnerAddHp;
	cocos2d::CCMenuItemImage*m_pMenuAvatar;
};

#pragma mark - FormationWarriorInfoView
class  FormationWarriorInfoView
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{

public:
	CC_SYNTHESIZE(formation::PositionInfo*, m_pPositionInfo, PositionInfo);
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pTouchToChange, TouchToChange);
    FormationWarriorInfoView();
    ~FormationWarriorInfoView();
    static FormationWarriorInfoView * create(cocos2d::extension::CCData* pData, uint32_t pid);
    virtual bool init(cocos2d::extension::CCData* pData,uint32_t pid);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    

private:
    
    FormationView *m_pDelegate;
    warrior::WarriorCurInfo* m_pWarriorInfo;
    uint32_t m_nPid;
    
private:
    cocos2d::CCSprite       *m_pGrade;
    
    cocos2d::CCLabelBMFont  *m_pExp;                //经验
    DProgressBar            *m_pExpBar;             //经验条
    
    WarriorTypeMark         *m_pWarriorTypeMark;    //弟子类型
    cocos2d::CCNode         *m_pContainer;              //弟子容器·
	cocos2d::CCNode         *m_pBgContainer;              //弟子容器·

    
    cocos2d::CCMenuItem     *m_pWarriorButton;
    
    cocos2d::CCNode         *m_pDefault;        //默认弟子
    cocos2d::CCNode         *m_pWarrior;        //弟子
	

private:
    void touchWarrior(CCObject *obj);
    

public:
    void updateView();
    void setWarriorInfo(warrior::WarriorCurInfo*);
    uint32_t getWid();
    uint32_t getPid();
    
    void setDelegate(FormationView *pDelegate);
};

class FormationListViewCell;
#pragma mark - FormationListView
class FormationListView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
#pragma mark - - init etc.
public:
    FormationListView();
    ~FormationListView();
    CREATE_FUNC(FormationListView);
    virtual bool init();
    
#pragma mark - - variable
private:
    CC_SYNTHESIZE(DTableView *, m_pView, View);

	//不再传入父节点本身，改用dalegate
    CC_SYNTHESIZE(FormationViewDelegate*, m_pDelegate, Delegate);
    FormationModel *m_pFormationModel;
    WarriorModel *m_pWarriorModel;
    
    cocos2d::extension::CCData      *m_pCellData;
    
#pragma mark - - CCTableViewDelegate
private:
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
    virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    
#pragma mark - - methods private
public:
    void updateView();
};

enum WarriorBoxState{
	kWarBoxStLock,
	kWarBoxStNoWar,
	kWarBoxStHasWar
};

#pragma mark - FormationListViewCell
class FormationListViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE(FormationViewDelegate*, m_pDelegate, Delegate);
#pragma mark - - init etc.
public:
    FormationListViewCell();
    ~FormationListViewCell();
    virtual bool init(cocos2d::extension::CCData *pData);
    static FormationListViewCell* create(cocos2d::extension::CCData *pData);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void setWarriorBoxState(WarriorBoxState st = kWarBoxStLock);
    
#pragma mark - - variable
private:
    warrior::WarriorCurInfo *m_pWarriorCurInfo;
    
private:
    WarriorHeadSmall          *m_pWarrior;
	WarriorHeadGradeBg        *m_pWarriorHeadGradeBg;
    cocos2d::CCMenuItem       *m_pSoundItem;
    cocos2d::CCNode           *m_pUnlock;
    cocos2d::CCLabelBMFont    *m_pUnlockLevel;
    cocos2d::CCSprite         *m_pHighLight;
    cocos2d::CCNode           *m_pParterIcon;
    cocos2d::CCMenuItem       *m_pPartnerBtn;
    cocos2d::CCNode           *m_pWarriorNameNode;
    cocos2d::CCLabelTTF       *m_pWarriorName;
    
#pragma mark - - methods private
private:
    void updateView();
    
#pragma mark - - methods private
public:
    void setHighlight(bool val);
    
    void setUnLockInfo(uint32_t level);
    
    void setLock(bool val);
    
    void setWarriorInfo(warrior::WarriorCurInfo*);
    
    void showPartner(bool showFlag=false);

	void showWarriorName(bool showFlag=false);
    
    void partnerCallback(cocos2d::CCObject *obj);
};




class GundamFateListViewCell;
#pragma mark - GundamFateListView
class GundamFateListView
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
#pragma mark - - init etc.
public:
	GundamFateListView();
	~GundamFateListView();
	static GundamFateListView* create(const cocos2d::CCSize&);
	virtual bool init(const cocos2d::CCSize&);

	virtual void onEnter();
	virtual void onExit();
#pragma mark - - variable
private:
	CC_SYNTHESIZE(DTableView *, m_pView, View);

	//不再传入父节点本身，改用dalegate
	CC_SYNTHESIZE(FormationViewDelegate*, m_pDelegate, Delegate);
	FormationModel *m_pFormationModel;
	WarriorModel *m_pWarriorModel;
	EquipModel    *m_pEquipModel;
	cocos2d::CCArray *m_pBattleWarriors;

	cocos2d::extension::CCData      *m_pCellData;

#pragma mark - - CCTableViewDelegate
private:
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);


#pragma mark - - methods private
public:
	void updateView();
};



#pragma mark - GundamFateListViewCell
class GundamFateListViewCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE(FormationViewDelegate*, m_pDelegate, Delegate);
#pragma mark - - init etc.
public:
	GundamFateListViewCell();
	~GundamFateListViewCell();
	virtual bool init(cocos2d::extension::CCData *pData);
	static GundamFateListViewCell* create(cocos2d::extension::CCData *pData);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);


#pragma mark - - variable
private:
	warrior::WarriorCurInfo *m_pWarriorCurInfo;
    cocos2d::CCRichLabelTTF	*m_pFateInfo;
    cocos2d::CCRichLabelTTF	*m_pWarrirorName;
	WarriorModel			*m_pWarriorModel;

#pragma mark - - methods private
private:
	void updateView();

#pragma mark - - methods private
public:

	void setWarriorInfo(warrior::WarriorCurInfo*);

	void updateFateContent();
};

#endif /* defined(__QSMY__FormationView__) */

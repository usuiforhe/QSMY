//
//  Formation.h
//  阵容
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__FriendFormation__
#define __QSMY__FriendFormation__

#include "components/components.h"
#include "model/OtherFormationModel.h"
#include "model/WarriorModel.h"
#include "model/EquipModel.h"
#include "model/OtherFormationModel.h"
#include "../Game.h"
#include "../formation/FormationView.h"

//外部代理
class FriendFormationDelegate{
public:
	//简化处理 可以不重写
	virtual void close(){};
};

//内部代理
class FriendFormationViewDelegate{
public:
	virtual void close() = 0;
};
/************************************************************************/
/* FriendFormationListViewCell                                                                     */
/************************************************************************/

class FriendFormationListViewCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	FriendFormationListViewCell();
	~FriendFormationListViewCell();
	virtual bool init(cocos2d::extension::CCData *pData);
	static FriendFormationListViewCell* create(cocos2d::extension::CCData *pData);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void setWarriorBoxState(WarriorBoxState st = kWarBoxStLock);

private:
	warrior::WarriorCurInfo *m_pWarriorCurInfo;
	otherFormation::WarriorPropInfo *m_pWarriorInfo;
private:
	WarriorHeadSmall          *m_pWarrior;
	WarriorHeadGradeBg        *m_pWarriorHeadGradeBg;
	cocos2d::CCMenuItem       *m_pSoundItem;
	cocos2d::CCNode           *m_pUnlock;
	cocos2d::CCLabelBMFont    *m_pUnlockLevel;
	cocos2d::CCSprite         *m_pHighLight;

private:
	void updateView();

public:
	void setHighlight(bool val);
	void setUnLockInfo(uint32_t level);
	void setLock(bool val);
	void setWarriorInfo(otherFormation::WarriorPropInfo*);
};


/************************************************************************/
/* FriendFormationListView                                                                     */
/************************************************************************/
class FriendFormationListView
	:public DLayer
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
public:
	FriendFormationListView();
	~FriendFormationListView();
	CREATE_FUNC(FriendFormationListView);
	virtual bool init();

	CC_SYNTHESIZE_RETAIN(otherFormation::FormationInfo*, m_pFormationInfo, FormationInfo);

private:
	CC_SYNTHESIZE(DTableView *, m_pView, View);
	CC_SYNTHESIZE(FormationViewDelegate*, m_pDelegate, Delegate);

	cocos2d::extension::CCData      *m_pCellData;

private:
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

public:
	void updateView();
};


/************************************************************************/
/* FriendFormationView                                                                     */
/************************************************************************/

class FriendFormationView
	:public DLayer
	,public DPageViewDataSource
	,public cocos2d::extension::CCScrollViewDelegate
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public FormationViewDelegate
{

public:
	FriendFormationView();
	~FriendFormationView();
	virtual bool init();
	CREATE_FUNC(FriendFormationView);

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
	CC_SYNTHESIZE(FriendFormationViewDelegate*, m_pDelegate, Delegate);

	int8_t m_iCurrentPage;

	FriendFormationListView           *m_pFormationListView;
	DPageView                   *m_pPageView;

	OtherFormationModel		*m_pModel;
	cocos2d::CCArray            *m_pWarriorInfoViews;

	//当前英雄ID 用于播放声音等
	unsigned int m_uCurrentWarriorID;

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

	
	void quickEquip();
	void playVoiceCallback(cocos2d::CCObject *sender);
	void stopHeroSound(/*float delay=0*/);

public:

	void setCurrentPage(uint8_t i);
	uint8_t getCurrentPage(){
		return m_iCurrentPage;
	}

	void touchSkillCallBack(cocos2d::CCObject* sender){};
	void touchEquipCallBack(cocos2d::CCObject* sender){};
	void touchWarrior(cocos2d::CCObject* sender){};
	void callbackFormation(cocos2d::CCObject* sender){};

	//查看别人/机器人的阵容
	void showOtherAndRobotFormation(otherFormation::FormationInfo* pFormationInfo);
	//关闭界面
	void onClose(CCObject*);

private:

	otherFormation::FormationInfo* m_pFormationInfo;
	DButton					*m_pCloseButton;
	cocos2d::CCNode		*m_pChangeFormationMenu;

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
	cocos2d::CCNode          *m_pButtonsNode;//按钮节点 隐藏
	cocos2d::CCMenuItem          *m_pFateMenu;//有缘信息菜单触发
	WarriorGradeBg			*m_pWarriorGradeBg;
	cocos2d::CCMenuItem     *m_pHorn;
	unsigned int m_nSoundID;
	sp::Armature* m_pArmature;
	cocos2d::CCLabelBMFont  *m_pZizhiNum;
	cocos2d::CCNode         *m_pZizhiBg;
};



/************************************************************************/
/* FriendFormation                                                                     */
/************************************************************************/

class FriendFormation
	:public DLayer
	,public FriendFormationViewDelegate
{

public:
	FriendFormation();
	~FriendFormation();
	virtual bool init();
	CREATE_FUNC(FriendFormation);

	//delegate
	void close();			//close

	void onEnter();
	void onExit();
	void showFormationView();

	//设置内容
	void setInfo(unsigned int id , bool isRobot = false , otherFormation::FormationInfo* pFormationInfo = NULL);
	//更新界面
	void updateFormation();

private:
	CC_SYNTHESIZE(otherFormation::FormationInfo*, m_pFormationInfo, FormationInfo);
	CC_SYNTHESIZE(FriendFormationDelegate*, m_pDelegate, Delegate);
	
	FriendFormationView               *m_pFormation;
	
	OtherFormationModel              *m_pModel;
	WarriorModel                *m_pWarriorModel;
	formation::PositionInfo     *m_pSelectPos;


	cocos2d::CCNode     *m_pPopupLayer;
	cocos2d::CCNode     *m_Container;

	bool                m_bFirstEnter;

};

#endif /* defined(__QSMY__FriendFormation__) */

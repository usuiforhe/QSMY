//
//  EquipView.h
//  装备查看列表
//
//  Created by wanghejun on 13-4-12.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EquipView__
#define __QSMY__EquipView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/EquipModel.h"

class EquipViewSelectDelegate{
public:
	virtual void setSelected(int index)
	{
	}
};

class EquipDelegate;
class EquipViewCell;

class EquipView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public EquipViewSelectDelegate
{
    
public:
	EquipView();
	~EquipView();
    
	static EquipView* create(equip::EquipType equipType, const cocos2d::CCSize& size , bool bSelectBox = false);
    virtual bool init(equip::EquipType equipType, const cocos2d::CCSize& size , bool bSelectBox);


	virtual void onEnter();
	virtual void onExit();
    
	void setDelegate(EquipDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	virtual void setSelected(int index);

private:
    
    void onTouch(cocos2d::CCObject*);
	void updateEquipView();
	void updateEquipList();

private:
    int32_t  m_selectIndex;             //当前选中的cell index
	equip::EquipType m_equipType;		//装备类型  默认显示所有
    
    DTableView *m_pListView;
	EquipDelegate *m_pDelegate;
    EquipModel * m_pEquipModel;
    EquipModel::EquipCurInfoVec  m_EquipInfoVec;
private:
    cocos2d::extension::CCData  *m_pData;

	bool m_bSelectBox;

};




class EquipViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(uint8_t, m_type, CellType);

public:
	enum CellStatus
	{
		kNormalStatus,
		kSelectedStatus,
	};

public:
    
	EquipViewCell();
    virtual ~EquipViewCell();
    
	static EquipViewCell* create(cocos2d::extension::CCData* , bool bSelectBox);
	virtual bool init(cocos2d::extension::CCData* , bool bSelectBox);
    
    void setEquipInfo(equip::EquipCurInfo *pEquipInfo);

	void setDelegate(EquipDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
	void setSelectDelegate(EquipViewSelectDelegate* pDelegate)
	{
		m_pSelectDelegate = pDelegate;
	}

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	void adjust(CellStatus type);	
    
private:
    
	void updateCellView();
    void viewCallBack(cocos2d::CCObject* obj);
	void bottomCallBack(cocos2d::CCObject* obj);
	void enhanceCallBack(cocos2d::CCObject* obj);
    void sellCallBack(cocos2d::CCObject* obj);
	void refineCallBack(cocos2d::CCObject* obj);
private:
    EquipDelegate			*m_pDelegate;
	EquipViewSelectDelegate *m_pSelectDelegate;
    equip::EquipCurInfo *m_pEquipInfo;  //装备信息

	bool m_bSelectBox;
    
private:
    cocos2d::CCNode     *m_pInfoContainer;
    cocos2d::CCNode     *m_pExtNode;
    
    cocos2d::CCNode         *m_pContainer;
    
    cocos2d::CCSprite       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
    cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCNode			*m_pEquipUpLevel;
    
    cocos2d::CCLabelBMFont  *m_pProperty;
    cocos2d::CCSprite       *m_pPropertyType[equip::kEquipPropTotal];
    
    DButton                 *m_pEnhanceButton;
    DButton                 *m_pSellButton;
	DButton					*m_pRefineButton;
    
    cocos2d::CCNode         *m_pUnEquip;
    cocos2d::CCNode         *m_pEquiped;
    cocos2d::CCLabelTTF     *m_pEquipedOn;
	cocos2d::CCNode         *m_pTxtTunshi;
    
    cocos2d::CCSprite       *m_pGradeBg;

	cocos2d::CCNode			*m_pSellNode;
	cocos2d::CCNode			*m_pEnhanceNode;
	cocos2d::CCNode			*m_pGradeGreen;
	cocos2d::CCNode			*m_pGradeBlue;
	cocos2d::CCNode			*m_pGradePurple;
	cocos2d::CCNode			*m_pGradeOrange;
	cocos2d::CCLabelBMFont  *m_pZizhiNum;

	cocos2d::CCNode			*m_pBottom;
    DButton					*m_pButtonLeft;
	DButton					*m_pButtonRight;
	cocos2d::CCNode			*m_pFightNode;
	cocos2d::CCLabelBMFont  *m_pFight;

    CC_SYNTHESIZE(unsigned int,m_uIndex,Index);
	CC_SYNTHESIZE(bool,m_bSelected,Selected);
};



#endif /* defined(__QSMY__EquipView__) */

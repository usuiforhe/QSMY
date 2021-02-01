
//
//  WarriorInfo.h
//  弟子展示界面
//  Created by cll on 13-3-28.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__WARRIORINFO__
#define __QSMY__WARRIORINFO__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "../common/common.h"

class WarriorDelegate;
class FormationDelegate;
class WarriorFateView;
class WarriorGradeBg;
class PveDelegate;
//class PictureCollectCell;



class WarriorInfoView :public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{

public:
	enum WarriorInfoType
	{
		kTypeOperate,			//弟子列表进入,阵容进入
		kTypeCloseOnly,         //魂魄进入

	};
private:
	enum
	{
		kUpgradeBut,
		kPromotionBut,
		kDevelopBut
	};
public:
	WarriorInfoView();
	~WarriorInfoView();

	CREATE_FUNC(WarriorInfoView);
	static WarriorInfoView * create(WarriorInfoType type);

	virtual bool init(WarriorInfoType type);
	virtual bool init();

	virtual void onEnter(); 

	virtual void onExit();

	void setWarrior(warrior::WarriorCurInfo * pWarrior);
    void setWarrior(warrior::WarriorBaseInfo * pWarrior);
	//void setWarrior(warrior::WarriorCurInfo *pWarrior,PictureCollectCell* param);
	void stopHeroSound(float delay=0.f);

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	void setPveDelegate(PveDelegate* pDelegate)
	{
		m_pveDelegate = pDelegate;
	}

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}


private:

	void binding();

	void updateWarriorInfo();
	void updateFateContent();
	void updateTalentConten();

	void closeCallBack(cocos2d::CCObject *sender);
	void menuCallBack(cocos2d::CCObject * obj);
    void touchSkillCallBack(cocos2d::CCObject* sender);
	void playVoiceCallback(cocos2d::CCObject *sender);
    
    void doDressUp(cocos2d::CCObject *sender);
    void doDressOut(cocos2d::CCObject *sender);

public:
	//static bool m_bIsfromCollect;
	cocos2d::CCNode			*m_pWarriorSprite;	//弟子头像
	cocos2d::extension::CCScrollView* m_pScrollView;
	cocos2d::CCNode*							  m_pMarkNode;
	cocos2d::CCNode*							  m_pTalentNode;
	cocos2d::CCLabelBMFont*			m_pUpLevel;		//突破等级
	cocos2d::CCLabelTTF     *m_pDesc;			//弟子描述
	cocos2d::CCNode			*m_pFateList;		//缘
	cocos2d::extension::CCScale9Sprite* m_pFateListBg;
	cocos2d::CCRichLabelTTF	*m_pFateLabel;
	cocos2d::extension::CCScale9Sprite* m_pTalentBg;
	cocos2d::CCRichLabelTTF*				m_pTalentLabel;
	SkillBox				*m_pSkillBox1;		//技能1
	SkillBox				*m_pSkillBox2;		//技能2
	cocos2d::CCLabelBMFont *m_pFight;
	cocos2d::CCLabelBMFont	*m_pAttack;
	cocos2d::CCLabelBMFont	*m_pDefence;
	cocos2d::CCLabelBMFont	*m_pSpeed;
	cocos2d::CCLabelBMFont	*m_pHp;
	//cocos2d::CCSprite		*m_pGrade;			//品阶
	WarriorGradeBg				*m_pWarriorGradeBg;			
	cocos2d::CCLabelBMFont	*m_pLevel;
	cocos2d::CCLabelTTF		*m_pName;
    
	cocos2d::CCLabelBMFont  *m_pZizhiNum;

    cocos2d::CCNode         *m_pOperate;        
    cocos2d::CCNode         *m_pCloseOnly;     
	cocos2d::CCNode         *m_pBottom;
    
    DButton                 *m_pCloseButton;
    DButton                 *m_pStrengthenButton;
    DButton                 *m_pDevelopButton;
    DButton                 *m_pPromotionButton;
    
    WarriorTypeMark         *m_pWarriorTypeMark;
    
    cocos2d::CCMenuItem     *m_pDressUp;
    cocos2d::CCMenuItem     *m_pDressOut;


	DProgressBar	*m_pProgressBar[WARRIOR_GRADE_NUM];

	
private:
	WarriorDelegate * m_pDelegate;
	PveDelegate* m_pveDelegate;
	WarriorFateView		*m_pFateView;
	warrior::WarriorCurInfo *m_pWarriorInfo;
	uint8_t m_warriorInfoType;

	WarriorModel * m_pWarriorModel;
	uint32_t	m_fateCount;
	uint32_t	m_talentCount;
	unsigned int m_nSoundID;
	sp::Armature* m_pArmature;
	cocos2d::CCMenuItem     *m_pHorn;
};



#endif /* defined(__QSMY__WARRIORINFO__) */

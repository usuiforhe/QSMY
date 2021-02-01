//
//  Rebirth.h
//  英雄重生
//
#ifndef __PL_REBIRTH_H__
#define __PL_REBIRTH_H__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../common/common.h"
#include "../warrior/WarriorListView.h"
#include "../Game.h"
#include "managers/DataManager.h"

#define HERO_REBIRTH_ITEM_COUNT 7
#define HERO_REBIRTH_OPEN_FIRST "heroRebirthOpenFirst"
struct RebirthWarriorFilter
	: public WarriorFilterDelegate {
		CREATE_FUNC(RebirthWarriorFilter);
		bool init()
		{
			return true;
		}
		virtual ~RebirthWarriorFilter(){}
		virtual bool operator()(warrior::WarriorCurInfo * pWarrior);
};

class RebirthView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public WarriorSelectDelegate
{
public:
	RebirthView();
	virtual ~RebirthView();
	CREATE_FUNC(RebirthView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void showWarriorSelectList(cocos2d::CCObject * sender);
	virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual void setSelectedWarrior(warrior::WarriorCurInfo * pWarrior);
	//数据集合，heroType英雄1（魂魄2）
	virtual void setSelectedWarriorMore(cocos2d::CCArray *pWarrior,uint8_t heroType){};

private:
	WarriorSelectPopup	*m_pWarriorPopup;
	ItemBox			    *m_pBox[HERO_REBIRTH_ITEM_COUNT];

	DProgressBar	    *m_pProgressBar[WARRIOR_GRADE_NUM];

	cocos2d::CCNode						*m_pHideNode;
	cocos2d::CCMenuItemImage            *m_pBook;
	cocos2d::CCMenuItemImage            *m_pMenuHero;
	//cocos2d::CCMenuItem                 *m_pReselectBtn;
	DButton                     *m_pConfirmButton;
	DButton                     *m_pBtnBurn;
	DButton                     *m_pSelectBtn;

	cocos2d::CCNode			    *m_pWarriorSprite;	//弟子头像
	cocos2d::CCNode             *m_pNoneBox;
	cocos2d::CCNode             *m_pHeroBox;
	cocos2d::CCNode				*m_pHeroAwardList;
	cocos2d::CCNode             *m_pAwardNode;
	cocos2d::CCNode             *m_pAnimation;
	cocos2d::CCNode             *m_pSelectTxt;
	cocos2d::CCNode             *m_pReselectTxt;

	cocos2d::CCLabelBMFont      *m_pFight;
	cocos2d::CCLabelTTF         *m_pName;
	cocos2d::CCLabelBMFont      *m_pZizhiNum;
	cocos2d::CCLabelBMFont      *m_pLevel;
	cocos2d::CCLabelBMFont      *m_pUpLevel;		//突破等级

	WarriorGradeBg		        *m_pWarriorGradeBg;	
	WarriorTypeMark             *m_pWarriorTypeMark;
	warrior::WarriorCurInfo     *m_pWarriorInfo;

	sp::Armature            *m_pEffect;

private:
	void onBookClick(cocos2d::CCObject * sender);
	void updateInfo(warrior::WarriorCurInfo* pWarrior);
	void onOpenBurn(cocos2d::CCObject * sender);
	void setState();
	void onReselect(cocos2d::CCObject * sender);
	void getDropItem(const Json::Value &requestData,const Json::Value &responseData);
	void onRebirth(cocos2d::CCObject * sender);
	void setMenuTouchEnable(bool val);
	void animateEnd(sp::Armature * target, const char* event,const char* data);
	void confirmBirth(MessageType type, DMessageBox* target);
	void onBirthBack(const Json::Value &requestData,const Json::Value &responseData);
};
#endif //__PL_REBIRTH_H__
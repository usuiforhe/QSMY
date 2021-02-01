//
//  StepSelectWarrior.h
//  选择弟子
//
//  Created by wanghejun on 13-8-22.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__StepSelectWarrior__
#define __QSMY__StepSelectWarrior__
#define WARRIOR_COUNT 5 
#include "../Tutorial.h"
//#define WARRIOR_COUNT DM_GET_TUTORIAL_MODEL->getSelectMode()
class StepSelectWarrior
:public StepEnForce
{
public:
    StepSelectWarrior();
    virtual ~StepSelectWarrior();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
protected:
    void selectCallBack(cocos2d::CCObject* obj);
	void chooseMenuCallBack(cocos2d::CCObject* obj);
    
    /**
     *	@brief	刷新页面信息
     *
     *	@param 	index 	选中弟子索引
     */
    void updateInfo(uint32_t index);
    
protected:
    virtual void loadUI();
    virtual void finishStep();
    
private:
 //   TutorialWarrior         *m_pWarrior[WARRIOR_COUNT];     //选择形像
	//cocos2d::CCSprite       *m_pWarriorName[WARRIOR_COUNT];     //名称
	//cocos2d::CCSprite       *m_pWarriorImage[WARRIOR_COUNT];     //头像
 //   cocos2d::CCLabelTTF     *m_pDisposition;    //性格
 //   cocos2d::CCLabelTTF     *m_pFeature;        //特点
 //   cocos2d::CCLabelTTF     *m_pSkill;
 //   cocos2d::CCLabelTTF     *m_pDesc;

	//CCSprite* m_pWarrior[WARRIOR_COUNT];
	enum SelectMode
	{
	  kSelectMode3=3, //选人模式
	  kSelectMode5=5
	};
	enum WARRIOR_TAG
	{
		kWarriorTag0=0,//弟子TAG
		kWarriorTag1,
		kWarriorTag2,
		kWarriorTag3,
		kWarriorTag4
	};
	std::vector<CCSprite*> m_pWarrior;
	DButton*		m_pConfirmButton;

    cocos2d::CCNode         *m_pEffect;
	int m_selectedIndex;
	uint32_t m_nPlayingSound;
	CCSprite* m_pBg;
	CCSprite* m_pBg2;
	int m_nSelectMode;
};


#endif /* defined(__QSMY__StepSelectWarrior__) */

//
//  TutorialModel.h
//  新手引导
//
//  Created by wanghejun on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TutorialModel__
#define __QSMY__TutorialModel__

#include "Model.h"


#define D_EVENT_TUTORIAL_STEP_COMPLETE "step complete"
#define D_EVENT_TUTORIAL_INITED        "init complete"


class TutorialModel;

namespace tutorial
{
    
    
    enum TutorialType
    {
        kTutorialTypeEnforce    =   1,           //强制
        kTutorialTypePrompt,                    //提示
    };
    
    enum TutorialTriggerType
    {
        kTutorialTriggerTypeUserLvel = 1,  //用户等级
        kTutorialTriggerTypeCompleteStep,  //完成新手引导步骤
		kTutorialTriggerTypeReputation,				//声望
		kTutorialTriggerTypeFirst3Star,					//第一次三星
		kTutorialTriggerTypeFirsrWonder,		//第一次奇遇
		kTutorialTriggerTypeFirstSignin,		//第一次签到
        kTutorialTriggerTypeMapComplete,   //通关
		
    };
    
    enum TutorialProfileSide
    {
        kTutorialProfileSideLeft = 1,
        kTutorialProfileSideRight,
    };

	enum TutorialTouchType
	{
		kTutorialTouchTypePassToScene = 1, //下一步，并向游戏界面传递触屏事件
		kTutorialTouchTypeNext,	//下一步（更新对话框内容），触屏事件不向游戏内传递
	};
    
    
    class WarriorInfo:public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY(uint32_t, m_nId, ID);                                    //编号
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sDisposition, Disposition);   //性格
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sFeature, Feature);           //特点
        CC_SYNTHESIZE_READONLY(bool, m_bSelectedDefault, SelectedDefault);              //是否默认选中
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sSound, Sound);


        CREATE_FUNC(WarriorInfo);
        D_INIT_DEFAULT(ValueObjcet);
        
        friend class ::TutorialModel;
    };
    
    
    class StepInfo:public ValueObjcet
    {
        CC_SYNTHESIZE_READONLY(uint32_t, index, Index);                                 //索引
        CC_SYNTHESIZE_READONLY(uint16_t, id, ID);                                       //步骤编号
        CC_SYNTHESIZE_READONLY(TutorialType, type, Type);                               //类型
        CC_SYNTHESIZE_READONLY(TutorialTriggerType, trigger_type, TriggerType);         //触发类型
        CC_SYNTHESIZE_READONLY(uint16_t, trigger_condition, TriggerCondition);          //触发条件编号
        CC_SYNTHESIZE_READONLY(uint8_t, steps, Steps);                                  //包含的引导步骤数
        CC_SYNTHESIZE_READONLY(uint8_t, navId, NavId);                                  //导航编号
		CC_SYNTHESIZE_READONLY(uint32_t, keyStepId, KeyStepId);                                  //关键步骤id
        
        CREATE_FUNC(StepInfo);
        D_INIT_DEFAULT(ValueObjcet);
                    
        friend class ::TutorialModel;
        
    public:
		StepInfo() : keyStepId(0){};
        bool  isComplete();     //是否已经完成
        StepInfo* nextStep();   //下一步
        bool  isTrigger();      //是否已经触发
    };

	class StepDialog : public ValueObjcet
	{
		StepDialog()
			:m_dialogPosition(cocos2d::CCPointZero)
			,m_touchRect(cocos2d::CCRectMake(0,0,0,0))
			,m_TouchType(kTutorialTouchTypePassToScene)
            ,m_profileSide(kTutorialProfileSideLeft)
		{}

		CREATE_FUNC(StepDialog);
		D_INIT_DEFAULT(ValueObjcet);
		friend class ::TutorialModel;

		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sID, ID);//引导ID
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sContents, Contents);//对话内容
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, m_sResID, ResID);//声音资源ID
		CC_SYNTHESIZE_READONLY(float, m_fDelay, Delay);//声音延迟播放的时间
		CC_SYNTHESIZE_READONLY(uint32_t, m_uEmotionID, EmotionID);//对话框上资源ID
		CC_SYNTHESIZE_READONLY(float, m_fTouchAjustFactor, TouchAjustFactor);
		CC_SYNTHESIZE_READONLY(cocos2d::CCPoint, m_dialogPosition, DialogPosition);//对话框位置
		CC_SYNTHESIZE_READONLY(cocos2d::CCRect, m_touchRect, TouchRect);//触屏区域
		CC_SYNTHESIZE_READONLY(TutorialTouchType, m_TouchType, TouchType);//触屏类型
        CC_SYNTHESIZE_READONLY(TutorialProfileSide, m_profileSide, ProfileSide);//引导人物在左边还是右边。
		CC_SYNTHESIZE_READONLY(std::string, m_sPromptIconPath, PromptIconPath);//功能引导icon的路径
	};
}


class TutorialModel:public Model
{
    CC_SYNTHESIZE_READONLY(uint16_t, m_nLastStep, LastStepId); //完成的最后一步新手
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray*, m_pWarriors, Warriors);
	//CC_SYNTHESIZE(bool , m_bSelectMode, SelectMode);//选人模式
public:
    TutorialModel();
    virtual ~TutorialModel();
    virtual bool init();
	CC_SYNTHESIZE(bool,m_bSelectMode,SelectMode);//选人模式
    
public:
    
    //同步后台
    void getRookieInfo();
    
    void setLastStep(uint16_t step);
    tutorial::StepInfo* getStepInfoById(uint16_t id);
    tutorial::StepInfo* getStepInfoByIndex(uint32_t index);
    tutorial::StepInfo* getNextStep(); //获取下一步未开始的新手
    tutorial::StepInfo* getLastStep(); //获取已完成的最后一步
    bool isAllStepComplete();          //是否全部完成
    bool whetherNeedToMuteMusic();     //是否需要屏蔽背景音乐
	uint32_t getLastLocalUserStepId(){return m_nLastUserStep;};
   
    void getLockedNavSteps(std::vector<uint8_t> &ids);  //获取锁定的模块
    
    bool isNeedHide();
    
    //获取需要条件的最大等级
    uint16_t getMaxLevel()
    {
        return m_nMaxLevel;
    }
    
    //是否可以首刷
    bool getIsFirstDraw();
    
    
    const char* getContent(const char* step);

	cocos2d::CCRect		getTouchRect(const char* step);
	cocos2d::CCPoint		getDialogPosition(const char* step);
	

	tutorial::StepDialog *getStepDialog(const char *step);
    
    virtual void reset();
    
public:
    tutorial::WarriorInfo* getDefaultWarrior();
    tutorial::WarriorInfo* getWarriorInfoByIndex(uint32_t index);
    
private:
    void loadData();
    void parseData(const CSVDecoder::CSV &val);
    void parseContentData(const CSVDecoder::CSV &val);
    void parseWarriorData(const CSVDecoder::CSV &val);
private:
    cocos2d::CCArray* m_pTutorials;
    cocos2d::CCDictionary* m_pTutorialsMap;
    cocos2d::CCDictionary* m_pTutorialContents; //新手对话内容
private:
    uint16_t m_nMaxLevel;
	uint32_t m_nLastUserStep;//玩家上次退出游戏时的步骤id

protected:
	CC_SYNTHESIZE(bool, m_bRunning, IsRunning);
	CC_SYNTHESIZE(bool, m_bHasWonder, HasWonder);
    
};


#endif /* defined(__QSMY__TutorialModel__) */

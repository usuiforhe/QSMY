//
//  StepSecondPve.cpp
//  QSMY
//
//  Created by heven on 14-3-9.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "StepSecondPve.h"

#include "../../../../managers/DataManager.h"
#include "../../../../managers/ResourceManager.h"
#include "../../../../managers/PlatformManager.h"

USING_NS_CC;

StepSecondPve::StepSecondPve()
:m_bNeedFix(false)
{
    
}

void StepSecondPve::nextSubStep()
{
    if(!m_pStepInfo->isTrigger()) return;
    
    simTouch();
    removeAllChildren();

    //uint32_t unlockMapsCount = (uint32_t)-1;
    //if(DM_GET_PVE_MODEL->getChapterInfo(FIRST_CHAPTER_ID)->isSynced)
    //{
    //    unlockMapsCount = DM_GET_PVE_MODEL->getUnlockMapsCountByChapterId(FIRST_CHAPTER_ID);//探索之前获取解锁关卡
    //}
    //if(!m_bNeedFix && unlockMapsCount<2)
    //{
    //    m_bNeedFix = true;
    //}
    //
    //if(m_nCurrentSubStep==4 && m_bNeedFix) //修正第一次pve未完成退出对第二场pve新手引导的影响
    //{
    //    ++m_nCurrentSubStep;
    //    setVisible(false);
    //    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(StepSecondPve::continueStep),D_EVENT_SHOW_PVE_SETTLEMENT);
    //    return;
    //}

	int userStep = 0;
	std::string userStepString = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_TUTORIAL_SUBSTEP);
	if (userStepString.length() >0)
	{
		userStep = atoi(userStepString.c_str());
	}

	if (userStep != 0 
		&& (userStep - atoi(m_sContentId.c_str())) / 10 == 1130 
		&&   userStep != m_nCurrentSubStep  )
	{
		if (userStep % 10000 <= 8)
		{
			m_nCurrentSubStep = userStep;
			updateContentId();
		}
	}

    if(m_nCurrentSubStep==8)
    {
        setVisible(false);
        NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(StepSecondPve::continueStep),D_EVENT_SHOW_PVE_SETTLEMENT);
		updateContentId();
        return;
    }
    
    //if(m_nCurrentSubStep==10 && m_bNeedFix) //掉落也没了
    //{
    //    ++m_nCurrentSubStep;
    //}
    
    ++m_nCurrentSubStep;
    
    if(m_nCurrentSubStep>m_pStepInfo->getSteps())
    {
        nextStep(); //下一步
        return;
    }
    loadUI();
    updateContentInfo();
}

void StepSecondPve::continueStep()
{
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    setVisible(true);
    ++m_nCurrentSubStep;
    if(m_nCurrentSubStep>m_pStepInfo->getSteps())
    {
        nextStep(); //下一步
        return;
    }
    loadUI();
    updateContentInfo();
}
//
//  AchievementModel.cpp
//  公会
//
//  Created by Eci on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "AchievementModel.h"
#include "../network/HttpClient.h"
#include "../managers/GameManager.h"
#include <algorithm>
#include "utils/TimerUtil.h"

using namespace cocos2d;
using namespace achievement;

static int sortTaskByScore(const CCObject* p1, const CCObject* p2)
{
	//return ((achievement::TaskBase*)p1)->getScore() > ((achievement::TaskBase*)p2)->getScore();
	achievement::TaskInfo* t1 = (achievement::TaskInfo*)p1;
	achievement::TaskInfo* t2 = (achievement::TaskInfo*)p2;
	if(t1->getTaskState() == t2->getTaskState())
	{
		//同状态任务
		if(t1->getTaskState() == kTaskStateDone)
		{
			//已经完成的任务 按类别排序
			return ((int)(t1->getBaseInfo()->getType()) < (int)(t2->getBaseInfo()->getType()));
		}
		else
		{
			//未完成的任务 按score排序
			return t1->getBaseInfo()->getScore() < t2->getBaseInfo()->getScore();
		}
		
	}		
	else
	{
		return t1->getTaskState() > t2->getTaskState();
	}
		
};

AchievementModel::~AchievementModel()
{
	CC_SAFE_RELEASE(m_pDictAchievment);
	CC_SAFE_RELEASE(m_pTaskListDict);
	CC_SAFE_RELEASE(m_pDictAward);
}

bool AchievementModel::init()
{
	m_pDictAchievment = CCDictionary::create();
	CC_SAFE_RETAIN(m_pDictAchievment);

	m_pTaskListDict = CCDictionary::create();
	CC_SAFE_RETAIN(m_pTaskListDict);

	m_pDictAward = CCDictionary::create();
	CC_SAFE_RETAIN(m_pDictAward);

	LoadAchievementData (CSVDecoder::create(ACHIEVEMENT_CONFIG_PATH)->getData());
	LoadAwardData (CSVDecoder::create(ACHIEVEMENT_AWARD_PATH)->getData());
	return Model::init();
}

void AchievementModel::LoadAwardData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		AwardInfo *pData = AwardInfo::create();
		pData->m_index = atoi(val[i][0].c_str());
		pData->m_score = atoi(val[i][1].c_str());
		pData->m_type = (drop::DropType)atoi(val[i][2].c_str());
		pData->m_id = atoi(val[i][3].c_str());
		pData->m_amount = atoi(val[i][4].c_str());
		m_pDictAward->setObject(pData, pData->m_index);
	}
}

void AchievementModel::LoadAchievementData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{//ID	TaskID LastTaskID	TaskCategory	TaskType	TaskColor	CompleteID	TaskNum	TaskRewardType	TaskRewardID	RewardNum	TaskScore	TaskDescription
		TaskBase *pData = TaskBase::create();
		pData->m_uID = atoi(val[i][0].c_str());
		pData->m_uTaskID = atoi(val[i][1].c_str());
		pData->m_eCategory = (TaskCategory)atoi(val[i][3].c_str());
		pData->m_eType = (TaskType)atoi(val[i][4].c_str());
		pData->m_eColor = (TaskColor)atoi(val[i][5].c_str());
		pData->m_uCompleteID = atoi(val[i][6].c_str());
		pData->m_uTaskNum = atoi(val[i][7].c_str());
		pData->m_eRewardType = (drop::DropType)atoi(val[i][8].c_str());
		pData->m_uRewardID = atoi(val[i][9].c_str());
		pData->m_uRewardNum = atoi(val[i][10].c_str());
		pData->m_uScore = atoi(val[i][11].c_str());;
		pData->m_sDescription = val[i][12];

		CCArray *pArr = dynamic_cast<CCArray *>(m_pDictAchievment->objectForKey(pData->m_eCategory));
		if (!pArr)
		{
			pArr = CCArray::create();
			m_pDictAchievment->setObject(pArr, pData->m_eCategory);
		}
		pArr->addObject(pData);
		m_uTotalScore += pData->m_uScore;
	}
	//CCDictElement *pEle = NULL;
	//CCDICT_FOREACH(m_pDictAchievment, pEle)
	//{
	//	CCArray *pArr = dynamic_cast<CCArray *>(pEle->getObject());
	//	//std::sort(pArr->data->arr, pArr->data->arr + pArr->data->num, sortTaskByScore);
	//}
}

CCArray *AchievementModel::getAchievementByCategory(achievement::TaskCategory eCategory)
{
	/*CCArray *pArr = CCArray::create();
	CCArray *pAward = dynamic_cast<CCArray *>(m_pDictAchievment->objectForKey(eCategory));
	if (pAward != NULL)
	{
		pArr->addObjectsFromArray(pAward);
	}
	return pArr;*/

	CCArray *pArr = CCArray::create();
	CCArray *pAward = dynamic_cast<CCArray *>(m_pTaskListDict->objectForKey(eCategory));
	if (pAward != NULL)
	{
		pArr->addObjectsFromArray(pAward);
	}
	return pArr;
}

uint32_t AchievementModel::getAchievedNumByCate(TaskCategory eCategory)
{
	CCArray *pInfos = dynamic_cast<CCArray *>(m_pTaskListDict->objectForKey(eCategory));
	uint32_t result = 0;

	CCObject* obj = NULL;
	TaskInfo* pInfo = NULL;
	CCARRAY_FOREACH(pInfos, obj)
	{
		pInfo = (TaskInfo*)obj;
		if(pInfo->getTaskState() == kTaskStateDone)
		{
			result++;
		}
	}

	return result;
}

//network

void AchievementModel::onGetScoreAward(const Json::Value& requestData,const Json::Value& responseData)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));
	if(responseData.isMember("ScoreRewardId"))
	{
		m_uScoreRewardId = responseData["ScoreRewardId"].asUInt();
		postNotification(D_EVENT_ACHIEVEMENT_SCOREAWARD);
	}
}

void AchievementModel::updateCurrentAchievement(const Json::Value &responseData)
{
	/************************************************************************/
	/* 这里可以判断m_uCurrentAchievement 是否与 count 相等
		仅当不相等时，再post更新命令 */
	/************************************************************************/

	//成就更新 發送消息
	uint32_t num = responseData["CompleteNum"].asUInt();
	uint32_t score = responseData["Score"].asUInt();
	uint32_t ScoreRewardId = responseData["ScoreRewardId"].asUInt();

	m_uScoreRewardId = ScoreRewardId;
	m_uCurrentScore = score;
	m_uCurrentAchievement = num;

	postNotification(D_EVENT_ACHIEVEMENT_SCOREAWARD);
	//发送消息给显示层
	CCInteger *pNum = CCInteger::create(m_uCurrentAchievement);
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_ACHIEVEMENT_FLASH , pNum);//,cocos2d::CCInteger::create(requestData[UUID].asInt()));

}

void AchievementModel::getListFromServer()
{
	HTTP_CLIENT->getTaskList(this,callfuncJson_selector(AchievementModel::reloadCallBack));
}

void AchievementModel::getAward(uint32_t taskid)
{
	HTTP_CLIENT->getTaskAward(taskid , this,callfuncJson_selector(AchievementModel::getAwardCallBack));
}

void AchievementModel::getAwardCallBack(const Json::Value& requestData,const Json::Value& responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	//發送消息 通知view更新 (仅在 进入成就界面和点击领奖后，更新列表和currentscore)
	postNotification(D_EVENT_ACHIEVEMENT_UPDATE);

}

void AchievementModel::reloadCallBack(const Json::Value& requestData,const Json::Value& responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	//發送消息 通知view更新 (仅在 进入成就界面和点击领奖后，更新列表和currentscore)
	postNotification(D_EVENT_ACHIEVEMENT_UPDATE);
}

void AchievementModel::parseTasks(const Json::Value &val)
{
	//首先清理原有数据
	CCDictElement *pDictEle = NULL;
	if(m_pTaskListDict)
	{
		CCDICT_FOREACH(m_pTaskListDict, pDictEle)
		{
			CCArray *pArr = dynamic_cast<CCArray *>(pDictEle->getObject());
			if (pArr){
				pArr->removeAllObjects();
			}
		}
	}

	for (uint32_t i=0; i<val.size(); ++i)
	{
		parseOneTask(val[i]);
	}

	isSynced = true;

	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_pTaskListDict, pEle)
	{
		CCArray *pArr = dynamic_cast<CCArray *>(pEle->getObject());
		std::sort(pArr->data->arr, pArr->data->arr + pArr->data->num, sortTaskByScore);
	}

	////發送消息 通知view更新
	//postNotification(D_EVENT_ACHIEVEMENT_UPDATE);
}

void AchievementModel::parseOneTask(const Json::Value &val)
{
	TaskInfo * pTask = TaskInfo::create();

	//Read Json
	DJSON_UINT(pTask->task_id,val,"task_id");
	uint8_t state = 0;
	DJSON_UINT(state,val,"state");
	uint8_t taskcate = 0;
	DJSON_UINT(taskcate,val,"category");
	DJSON_UINT(pTask->progress,val,"progress");
	pTask->taskcate = static_cast<TaskCategory>(taskcate);
	pTask->state = static_cast<TaskState>(state);

	//Set BaseInfo
	CCObject* obj = NULL;
	TaskBase* pBase = NULL;
	CCArray *pBaseArray = dynamic_cast<CCArray *>(m_pDictAchievment->objectForKey(pTask->taskcate));
	CCARRAY_FOREACH(pBaseArray, obj)
	{
		pBase = (TaskBase*)obj;
		if(pBase->getTaskID() == pTask->task_id)
		{
			pTask->baseInfo = pBase;
			break;
		}
	}

	//Check BaseInfo
	if(pTask->baseInfo==NULL)
	{
		CCLOG("%s","error task!!");
		return;
	}

	//Checked , Add Info
	CCArray *pArr = dynamic_cast<CCArray *>(m_pTaskListDict->objectForKey(pTask->taskcate));
	if (!pArr)
	{
		pArr = CCArray::create();
		m_pTaskListDict->setObject(pArr, pTask->taskcate);
	}
	pArr->addObject(pTask);

	//Change m_uCurrentScore
	/*if( pTask->state == kTaskStateDone )
	{
		m_uCurrentScore += pTask->getBaseInfo()->getScore();
	}*/

	//m_productInfoVec->addObject(pTask);
}

void AchievementModel::reset()
{
	isSynced = false;
	m_pTaskListDict->removeAllObjects();
}
//
//  PveModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PveModel.h"
#include "utils/StringUtil.h"
#include "managers/GameManager.h"
#include "../scenes/game/pve/PveReward.h"

#define FAILURE_RESOURCE    "PL_ui/pve/failure/"

using namespace pve;

PveModel::PveModel()
:m_Chapters(NULL)
,m_UnlockChapters(NULL)
,m_Map(NULL)
,m_nFirstChapterId(0)
,m_nLastMapIDBeforeUnlock(0)
,m_nLastShowMapID(-1)
,m_nLastShowChapterID(-1)
,m_bShowChapter(false)
,m_bShowMap(false)
,m_pFailureMap(NULL)
,m_pMapDialogs(NULL)
,m_pChapterExtraInfo(NULL)
,m_pChapterDrop(NULL)
,m_Elite(NULL)
,m_pEliteDrop(NULL)
,m_nLastEliteID(0)
,m_ActivityChapters(NULL)
,m_UnlockActivityChapters(NULL)
,m_nLastShowActivityChapterID(-1)
,m_nLastShowActivityMapID(-1)
,m_nLastActivityMapIDBeforeUnlock(0)
,m_bShowActivityChapter(false)
,m_bShowActivityMap(false)
,m_pActivityID(-1)
,m_tStartTime(0)
,m_tEndTime(0)
{}

PveModel::~PveModel()
{
    CC_SAFE_RELEASE_NULL(m_Chapters);
    CC_SAFE_RELEASE_NULL(m_UnlockChapters);
	CC_SAFE_RELEASE_NULL(m_Map);
	CC_SAFE_RELEASE_NULL(m_pFailureMap);
	CC_SAFE_RELEASE(m_pMapDialogs);
	CC_SAFE_RELEASE(m_pChapterExtraInfo);
	CC_SAFE_RELEASE(m_pChapterDrop);
	CC_SAFE_RELEASE(m_Elite);
	CC_SAFE_RELEASE(m_pEliteDrop);
	CC_SAFE_RELEASE(m_ActivityChapters);
	CC_SAFE_RELEASE(m_UnlockActivityChapters);
}

bool PveModel::init()
{
    m_Chapters      = ChapterMap::create();
    m_UnlockChapters= ChapterMap::create();
	m_Map           = MapInfoMap::create();
	m_pFailureMap   = FailureMap::create();
	m_Elite         = MapInfoMap::create();
	m_ActivityChapters = ChapterMap::create();
	m_UnlockActivityChapters = ChapterMap::create();
	CC_SAFE_RETAIN(m_ActivityChapters);
	CC_SAFE_RETAIN(m_UnlockActivityChapters);
    m_pFightType = pve::PveFightNormal;
    CC_SAFE_RETAIN(m_Chapters);
    CC_SAFE_RETAIN(m_UnlockChapters);
	CC_SAFE_RETAIN(m_Map);
	CC_SAFE_RETAIN(m_pFailureMap);
	CC_SAFE_RETAIN(m_Elite);

	m_pMapDialogs = CCDictionary::create();
	CC_SAFE_RETAIN(m_pMapDialogs);

	m_pChapterExtraInfo = CCDictionary::create();
	CC_SAFE_RETAIN(m_pChapterExtraInfo);

	m_pChapterDrop = CCDictionary::create();
	CC_SAFE_RETAIN(m_pChapterDrop);
	m_pEliteDrop = CCDictionary::create();
	CC_SAFE_RETAIN(m_pEliteDrop);

	user::ExporeActivityInfo* m_activityInfo = DM_GET_USER_MODEL->getExploreActivityInfo();
	m_pActivityID = m_activityInfo->getExploreActivityId();
	m_tStartTime = m_activityInfo->getStartTs();
	m_tEndTime = m_activityInfo->getEndTs();
	m_nActivityAwardState = m_activityInfo->getAwardState();
	m_ActivityMid = m_activityInfo->getActivityMid();
    loadData();
    
    return Model::init();
}

void PveModel::loadData()
{
    parseChapter( CSVDecoder::create(CHAPTER_CONFIG_PATH)->getData());
    parseMap( CSVDecoder::create(MAP_CONFIG_PATH)->getData());
	parseFailure( CSVDecoder::create(PVE_FAILURE_CONFIG_PATH)->getData());
	parseMapDialog( CSVDecoder::create(MAP_DIALOGUE_CONFIG_PATH)->getData());
	parseChapterExtraInfo( CSVDecoder::create(CHAPTER_EXTRA_INFO_CONFIG_PATH)->getData());
	parseElite(CSVDecoder::create(PVE_ELITE_PATH)->getData());

	//updateChapterMap(m_Chapters);
	updateChapterMap(m_ActivityChapters);
}

void PveModel::parseMapDialog(const CSVDecoder::CSV &val)
{
	uint32_t lastCID = 0;
	uint32_t soundIdx = 0;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		uint32_t mapId = atoi(val[i][0].c_str());
		if (lastCID != mapId)
		{
			soundIdx = 0;
			lastCID = mapId;
		}
		
		cocos2d::CCArray* pDialog = createDialog(mapId);
		MapDialogInfo *pMapDialog = MapDialogInfo::create();

		pMapDialog->m_Direction  = static_cast<battle::DialogDirection>(atoi(val[i][1].c_str()));
		pMapDialog->m_uWid  = atoi(val[i][2].c_str());
		pMapDialog->m_Content  = val[i][3].c_str();
		replace(pMapDialog->m_Content,"\\n","\n");
		pMapDialog->m_sSound = CCString::createWithFormat("mapDialog/%u-%u.mp3", mapId, ++soundIdx)->getCString();
		pDialog->addObject(pMapDialog);
	}
}

cocos2d::CCArray* PveModel::getDialog(int cid)
{
	//if(m_pBattleInfo==NULL) return NULL;
	return static_cast<CCArray*>(m_pMapDialogs->objectForKey(cid));
	return NULL;
}

cocos2d::CCArray* PveModel::createDialog(uint32_t mapId)
{
	CCArray* pRet = static_cast<CCArray*>(m_pMapDialogs->objectForKey(mapId));
	if(pRet) return pRet;
	pRet = cocos2d::CCArray::create();
	m_pMapDialogs->setObject(pRet, mapId);
	return pRet;
}

void PveModel::parseChapter(const CSVDecoder::CSV &val)
{
	CCLOG("activityID:%d",m_pActivityID);
    for (uint32_t i=0; i<val.size(); ++i)
	{
        int iCid = atoi(val[i][0].c_str());
        if (iCid == 0)
        {
            continue;
        }

		if (m_nFirstChapterId == 0)
		{
			m_nFirstChapterId = iCid;
		}
        
        ChapterInfo *pChapterInfo = getChapterInfo(iCid);
        if(!pChapterInfo)
        {
            pChapterInfo = ChapterInfo::create(iCid);
            pChapterInfo->chapter_name      = val[i][1].c_str();
            pChapterInfo->prestige          = atoi(val[i][2].c_str());
            pChapterInfo->chapter_des       = atoi(val[i][3].c_str());
			pChapterInfo->chapterType       = atoi(val[i][8].c_str());
			if(pChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
			{
				m_Chapters->addObject(pChapterInfo);
			}
			else if(pChapterInfo->chapterType == m_pActivityID)
			{
				m_ActivityChapters->addObject(pChapterInfo);
			}
        }
        
        PrestigeReward* pPrestigeReward = PrestigeReward::create();
        pPrestigeReward->prestige_reward_need_viplv = atoi(val[i][4].c_str());
        pPrestigeReward->prestige_reward_type = (PveRewardType)atoi(val[i][5].c_str());
        pPrestigeReward->prestige_reward_quantity = atoi(val[i][6].c_str());
        pPrestigeReward->prestige_reward_id = atoi(val[i][7].c_str());
        pChapterInfo->m_pPrestigeRewards->addObject(pPrestigeReward);
        
	}
}

void PveModel::parseMap(const CSVDecoder::CSV &val)
{
	m_Map->removeAllObjects();
    
    for (uint32_t i=0; i<val.size(); ++i)
	{
        int iMapId = atoi(val[i][0].c_str());
        if (iMapId == 0)
        {
            continue;
        }
        
		MapInfo *pMapInfo = MapInfo::create(iMapId);
        pMapInfo->fightType = 0;
		pMapInfo->chapter_id        = atoi(val[i][1].c_str());
		pMapInfo->map_name          = val[i][2];
        pMapInfo->challenge_limit   = atoi(val[i][3].c_str());
        pMapInfo->mapLv             = atoi(val[i][4].c_str());
        pMapInfo->map_des           = val[i][5];
		pMapInfo->power_cost		= atoi(val[i][6].c_str());
        pMapInfo->fight_open        = atoi(val[i][7].c_str());
		split(val[i][8].c_str(), ",", pMapInfo->prizes_type );
		split(val[i][9].c_str(), ",", pMapInfo->prizes_id);
        pMapInfo->enemy_id          = atoi(val[i][10].c_str());
        pMapInfo->mapType           = atoi(val[i][11].c_str());
		pMapInfo->posX				= atoi(val[i][12].c_str());
		pMapInfo->posY				= atoi(val[i][13].c_str());
		pMapInfo->scale				= atof(val[i][14].c_str()) / 100.0;
		pMapInfo->enemyLittle_id    = atoi(val[i][15].c_str());
        
        pMapInfo->chapterInfo = getChapterInfo(pMapInfo->chapter_id);
		m_Map->addObject(pMapInfo);

		bool hasErrorData = false;
		for (int i =0; i < pMapInfo->prizes_type.size(); i++)
		{
			if (pMapInfo->prizes_type[i] == kPveRewardTypeNone)
			{
				hasErrorData = true;
				break;
			}
		}
		
		if (hasErrorData)
		{
			continue;
		}

		CCArray *pArr = (CCArray *)m_pChapterDrop->objectForKey(pMapInfo->chapter_id);
		if (pArr == NULL)
		{
			pArr = CCArray::create();
			m_pChapterDrop->setObject(pArr, pMapInfo->chapter_id);
		}
		pArr->insertObject(pMapInfo, 0);
	}
}

void PveModel::parseElite(const CSVDecoder::CSV &val)
{
	m_Elite->removeAllObjects();

	for (uint32_t i=0; i<val.size(); ++i)
	{
		int iMapId = atoi(val[i][0].c_str());
		if (iMapId == 0)
		{
			continue;
		}

		MapInfo *pMapInfo = MapInfo::create(iMapId);
		pMapInfo->fightType = 1;
		pMapInfo->chapter_id        = atoi(val[i][1].c_str());
		pMapInfo->map_name          = val[i][2];
		pMapInfo->challenge_limit   = atoi(val[i][3].c_str());
		pMapInfo->mapLv             = atoi(val[i][4].c_str());
		pMapInfo->map_des           = val[i][5];
		pMapInfo->power_cost		= atoi(val[i][6].c_str());
		pMapInfo->fight_open        = atoi(val[i][7].c_str());
		split(val[i][8].c_str(), ",", pMapInfo->prizes_type );
		split(val[i][9].c_str(), ",", pMapInfo->prizes_id);
		pMapInfo->enemy_id          = atoi(val[i][10].c_str());
		pMapInfo->mapType           = atoi(val[i][11].c_str());
		pMapInfo->posX				= atoi(val[i][12].c_str());
		pMapInfo->posY				= atoi(val[i][13].c_str());
		pMapInfo->scale				= atof(val[i][14].c_str()) / 100.0;
		pMapInfo->enemyLittle_id    = atoi(val[i][15].c_str());
		pMapInfo->eliteCoin         = atoi(val[i][16].c_str());
		pMapInfo->eliteGold         = atoi(val[i][17].c_str());

		pMapInfo->chapterInfo = getChapterInfo(pMapInfo->chapter_id);
		m_Elite->addObject(pMapInfo);

		bool hasErrorData = false;
		for (int i =0; i < pMapInfo->prizes_type.size(); i++)
		{
			if (pMapInfo->prizes_type[i] == kPveRewardTypeNone)
			{
				hasErrorData = true;
				break;
			}
		}

		if (hasErrorData)
		{
			continue;
		}

		CCArray *pArr = (CCArray *)m_pEliteDrop->objectForKey(pMapInfo->chapter_id);
		if (pArr == NULL)
		{
			pArr = CCArray::create();
			m_pEliteDrop->setObject(pArr, pMapInfo->chapter_id);
		}
		pArr->insertObject(pMapInfo, 0);
	}
}

void PveModel::updateChapterMap(CCArray* chapterArr)
{
	CCObject* objC = NULL;
	ChapterInfo* pChapterInfo = NULL;
	CCARRAY_FOREACH(chapterArr, objC)
	{
		pChapterInfo = (ChapterInfo*)objC;
		if (pChapterInfo)
		{
			CCObject* obj = NULL;
			MapInfo* pMapInfo = NULL;
			uint32_t nMapID = 0;
			uint32_t nMaxMapID = 999999;
			int cid = pChapterInfo->getCid();
			CCARRAY_FOREACH(m_Map, obj)
			{
				pMapInfo = (MapInfo*)obj;
				if (pMapInfo && pMapInfo->chapter_id==cid && pMapInfo->getMapId()>nMapID)
				{
					uint32_t nTmpMapID = pMapInfo->getMapId();
					if (nTmpMapID > nMapID)
					{
						nMapID = nTmpMapID;
						pChapterInfo->lastMid = nMapID;
					}
                    
					if (nTmpMapID < nMaxMapID)
					{
						nMaxMapID = nTmpMapID;
						pChapterInfo->firstMid = nMaxMapID;
					}
				}
			}
		}
	}
}

void PveModel::updateMaps(uint32_t cid,const Json::Value &val)
{
    if(val.size()>0)
    {
		uint32_t midTmp = 0;
		uint32_t valSize = val.size();
        for (uint32_t i = 0; i<valSize; i++)
        {
            uint32_t mid = val[i]["mid"].asUInt();
            if (mid != 0)
            {
                MapInfo *pMapInfo = getMapInfo(mid);
                if (pMapInfo)
                {
                    DJSON_UINT(pMapInfo->iScores,		val[i], "score");
					DJSON_UINT(pMapInfo->iTimes,		val[i], "times");
					DJSON_UINT(pMapInfo->todayTimes,	val[i], "todayTimes");
					DJSON_UINT(pMapInfo->lastBattleTs,  val[i], "lastBattleTs");
                    
                    pMapInfo->bUnlock = true;
                    pMapInfo->postNotification(D_EVENT_PVE_MAP_UPDATED);
                    
					if (mid > midTmp) midTmp = mid;
                }
            }
        }
        
		MapInfo *pMapInfo = getMapInfo(midTmp);
		ChapterInfo *pChapterInfo = getChapterInfo(pMapInfo->chapter_id);
        pChapterInfo->chapter_unlock = true;
        
		unlockNewMap(pMapInfo); //判断是否可以解锁新关卡
		
        pChapterInfo->isSynced = true;
        pChapterInfo->postNotification(D_EVENT_PVE_CHAPTER_UPDATED);
    }else
    {
        ChapterInfo *pChapterInfo = getChapterInfo(cid);
        pChapterInfo->chapter_unlock = false; //重置状态
        unlockNewChapter(cid);
		if(pChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
		{
			m_bShowChapter = false;               //重置状态   
		}
		else if(pChapterInfo->chapterType == m_pActivityID)
		{
			m_bShowActivityChapter = false;
		}
    }
}

void PveModel::addChapterObject(pve::ChapterInfo* pChapterInfo)
{
	ChapterMap* temp = NULL;
	if(pChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
	{
		temp = m_UnlockChapters;
	}
	else if(pChapterInfo->chapterType == m_pActivityID)
	{
		temp = m_UnlockActivityChapters;
	}

	if(temp)
	{
		unsigned int nCount = temp->count();
		for (unsigned int i=0; i<nCount; ++i)
		{
			if (temp->objectAtIndex(i)->isEqual(pChapterInfo))
			{
				return;
			}
		}

		temp->addObject(pChapterInfo);
	}
}

void MapInfo::setTodayTimes(uint32_t times)
{
    todayTimes = times;
    postNotification(D_EVENT_PVE_MAP_UPDATED);
}


void MapInfo::updateRush(const Json::Value &val)
{
    if(chapterInfo)
    {
        uint16_t pres = chapterInfo->pres;
        DJSON_UINT(chapterInfo->pres, val, "pres");
        if(pres!=chapterInfo->pres) chapterInfo->postNotification(D_EVENT_PVE_CHAPTER_PRES_ADD);
    }
    DJSON_UINT(iTimes, val, "times");
    DJSON_UINT(todayTimes, val, "todayTimes");
    postNotification(D_EVENT_PVE_MAP_UPDATED);
    chapterInfo->postNotification(D_EVENT_PVE_CHAPTER_UPDATED);
}

void PveModel::updateMap(const Json::Value &val)
{
	bool bDelay = false;
	
	int lastMapID = 0;
	if(m_pFightType ==pve::PveFightActivity)
	{
		lastMapID = m_nLastShowActivityMapID;
	}
	else
	{
		lastMapID = m_nLastShowMapID;
	}
    MapInfo *pMapInfo = getMapInfo(lastMapID);
    if (pMapInfo)
    {
        if (pMapInfo->chapterInfo)
        {
            if (pMapInfo->chapterInfo->pres < pMapInfo->chapterInfo->getMaxPrestige())
            {
                pMapInfo->chapterInfo->pres = val["pres"].asUInt();
                pMapInfo->chapterInfo->postNotification(D_EVENT_PVE_CHAPTER_PRES_ADD);
            }
            
            pMapInfo->chapterInfo->great_num = val["great_num"].asUInt();
        }
        
        pMapInfo->iTimes = val["times"].asUInt();
        pMapInfo->todayTimes = val["todayTimes"].asUInt();
        
        if (val["battle"].asBool())
        {
            //战斗
            uint16_t uGrade = val["grade"].asUInt();
			if (uGrade > 0)
			{
				//打了boss才更新本关卡星星数
				int nTimes		= pMapInfo->iTimes;
				int nFightOpen	= pMapInfo->fight_open;
				int nFightMod	= nTimes%nFightOpen;
                if (pMapInfo->iScores < uGrade && nFightMod == 0)
                {
                    pMapInfo->iScores = uGrade;
                }
                
				//胜利
				bDelay = true;
			}
        }
        
        if(bDelay) unlockNewMap(pMapInfo); //判断是否可以解锁新关卡
        
        pMapInfo->postNotification(D_EVENT_PVE_MAP_UPDATED, bDelay?4.f:0.f);
        
		if (pMapInfo->chapterInfo)
		{
			pMapInfo->chapterInfo->postNotification(D_EVENT_PVE_CHAPTER_UPDATED, bDelay?4.f:0.f);
		}
        
        //if(newMapUnlock) postNotification(D_EVENT_PVE_MAP_UNLOCK,bDelay?4.f:0.f);
    }
}
void PveModel::unlockNewMap(MapInfo *pMapInfo)
{
    if(pMapInfo==NULL) return;
    if (pMapInfo->iTimes < pMapInfo->fight_open) return;
    MapInfo *pNewMap = getMapInfo(pMapInfo->getMapId()+1);
	ChapterInfo* curChapterInfo = getChapterInfo(pMapInfo->chapter_id);
	if(!curChapterInfo) return;
    if (pNewMap)
    {
        if (!pNewMap->bUnlock && pNewMap->iTimes==0)
        {
			if(curChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
			{
				m_bShowMap = true;
				m_nLastMapIDBeforeUnlock = m_nLastShowMapID;
				m_nLastShowMapID = pNewMap->getMapId();
			}
			else if(curChapterInfo->chapterType == m_pActivityID)
			{
				m_bShowActivityMap = true;
				m_nLastActivityMapIDBeforeUnlock = m_nLastShowActivityMapID;
				m_nLastShowActivityMapID = pNewMap->getMapId();
			}
        }
        pNewMap->bUnlock = true;
    }
    
    if (pMapInfo->chapterInfo->lastMid == pMapInfo->getMapId())
    {
        unlockNewChapter(pMapInfo->chapter_id+1);
    }
}

void PveModel::unlockNewChapter(const int &cid)
{
	ChapterInfo *pChapterInfo = getChapterInfo(cid);
    
	if(pChapterInfo==NULL || pChapterInfo->chapter_unlock)
    {
        m_bShowChapter = false;
		m_bShowActivityChapter = false;
        return;
    }
    
	if (!pChapterInfo->chapter_unlock)
	{
		
		if(pChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
		{
			m_bShowChapter = true;
			m_nLastShowChapterID = pChapterInfo->getCid();
		}
		else if(pChapterInfo->chapterType == m_pActivityID)
		{
			m_bShowActivityChapter = true;
			m_nLastShowActivityChapterID = pChapterInfo->getCid();
		}
	}
    
	pChapterInfo->chapter_unlock = true;
    
    pChapterInfo->isSynced = true; //新解锁de关卡没有内容 默认同步
    
	addChapterObject(pChapterInfo);
    
	MapInfo *pMapInfo = getMapInfo(pChapterInfo->firstMid);
	if (pMapInfo)
	{
		if(pChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
		{
			m_bShowMap = true;
			m_nLastMapIDBeforeUnlock = m_nLastShowMapID;
			m_nLastShowMapID = pMapInfo->getMapId();
		}
		else if(pChapterInfo->chapterType == m_pActivityID)
		{
			m_bShowActivityMap = true;
			m_nLastActivityMapIDBeforeUnlock = m_nLastShowActivityMapID;
			m_nLastShowActivityMapID = pMapInfo->getMapId();
		}
		pMapInfo->bUnlock = true;
		pMapInfo->postNotification(D_EVENT_PVE_MAP_UPDATED);
	}
    
    pChapterInfo->postNotification(D_EVENT_PVE_CHAPTER_UPDATED);
    postNotification(D_EVENT_PVE_CHAPTERS_UPDATED);
	
}

void PveModel::updateChapters(const Json::Value &val)
{
	bool bSyncChapterId = (m_nLastShowChapterID==-1);
	bool bSyncActivtyChapterId = (m_nLastShowActivityChapterID==-1);
    for (uint32_t i = 0; i<val.size(); i++)
    {
		int nTmp = val[i]["cid"].asInt();
		ChapterInfo* curChapterInfo = getChapterInfo(nTmp);
		if(curChapterInfo)
		{
			if (bSyncChapterId && curChapterInfo->chapterType == static_cast<int>(pve::PveChapterNormal))
			{
				if (m_nLastShowChapterID < (int)nTmp)
				{
					m_nLastShowChapterID = nTmp;
				}
			}
			if (bSyncActivtyChapterId && curChapterInfo->chapterType == m_pActivityID)
			{
				if (m_nLastShowActivityChapterID < (int)nTmp)
				{
					m_nLastShowActivityChapterID = nTmp;
				}
			}
		}
		
        updateChapter(val[i]);
    }
    
    resetUnlockChapters();
    isSynced = true;
    postNotification(D_EVENT_PVE_CHAPTERS_UPDATED);
}

void PveModel::updateChapter(const Json::Value &val)
{
	ChapterInfo *pChapterInfo = getChapterInfo(val["cid"].asUInt());
	if (pChapterInfo)
	{
        DJSON_UINT(pChapterInfo->pres, val, "pres");
        DJSON_UINT(pChapterInfo->chapter_box_bGet, val, "af");
        DJSON_BOOL(pChapterInfo->grade_award, val, "grade_award");
        DJSON_UINT(pChapterInfo->great_num, val, "great_num");
        pChapterInfo->chapter_unlock	= true;
        pChapterInfo->postNotification(D_EVENT_PVE_CHAPTER_UPDATED);
	}
}


void PveModel::resetUnlockChapters()
{
    m_UnlockChapters->removeAllObjects();
    m_UnlockActivityChapters->removeAllObjects();
    CCObject* obj = NULL;
    ChapterInfo* pChapterInfo = NULL;
    CCARRAY_FOREACH(m_Chapters, obj)
    {
        pChapterInfo = (ChapterInfo*)obj;
        if(pChapterInfo->chapter_unlock)
        {
            addChapterObject(pChapterInfo);
        }
    }
	CCARRAY_FOREACH(m_ActivityChapters, obj)
	{
		pChapterInfo = (ChapterInfo*)obj;
		if(pChapterInfo->chapter_unlock)
		{
			addChapterObject(pChapterInfo);
		}
	}
}

pve::MapInfo* PveModel::getMapInfo(uint32_t mid)
{
    CCObject* obj = NULL;
    MapInfo* pMapInfo = NULL;
    CCARRAY_FOREACH(m_Map, obj)
    {
        pMapInfo = (MapInfo*)obj;
        if(pMapInfo->getMapId() == mid) return pMapInfo;
    }

	CCARRAY_FOREACH(m_Elite, obj)
	{
		pMapInfo = (MapInfo*)obj;
		if(pMapInfo->getMapId() == mid) return pMapInfo;
	}
    return NULL;
}

pve::MapInfo* PveModel::getEliteInfo(uint32_t mid)
{
	CCObject* obj = NULL;
	MapInfo* pMapInfo = NULL;
	CCARRAY_FOREACH(m_Elite, obj)
	{
		pMapInfo = (MapInfo*)obj;
		if(pMapInfo->getMapId() == mid) return pMapInfo;
	}
	return NULL;
}

void PveModel::getMapsByChapterId(uint32_t id,MapInfoMap& vec)
{
    vec.removeAllObjects();
    
    CCObject* obj = NULL;
    MapInfo* pMapInfo = NULL;
    CCARRAY_FOREACH(m_Map, obj)
    {
        pMapInfo = (MapInfo*)obj;
        if(pMapInfo->chapter_id==id)
        {
            vec.addObject(pMapInfo);
        }
    }

}

uint32_t PveModel::getUnlockMapsCountByChapterId(uint32_t id)
{
    MapInfoMap *vec = MapInfoMap::create();
    getUnlockedMapsByChapterId(id,*vec);
    return vec->count();
}

void PveModel::getUnlockedMapsByChapterId(uint32_t id,MapInfoMap& vec)
{
    pve::ChapterInfo* pInfo = getChapterInfo(id);
#ifdef QSMY_DEBUG
    CCAssert(pInfo->isSynced, "Chapter is not Syned");
#endif
    vec.removeAllObjects();
    
    
    CCObject* obj = NULL;
    MapInfo* pMapInfo = NULL;
    CCARRAY_FOREACH(m_Map, obj)
    {
        pMapInfo = (MapInfo*)obj;
        if(pMapInfo->chapter_id==id && pMapInfo->bUnlock)
        {
            vec.addObject(pMapInfo);
        }
    }
}

pve::ChapterInfo* PveModel::getChapterInfo(uint32_t cid)
{
    CCObject* obj = NULL;
    ChapterInfo* pChapterInfo = NULL;
    CCARRAY_FOREACH(m_Chapters, obj)
    {
        pChapterInfo = (ChapterInfo*)obj;
        if(pChapterInfo->getCid()==cid) return pChapterInfo;
    }
	CCARRAY_FOREACH(m_ActivityChapters, obj)
	{
		pChapterInfo = (ChapterInfo*)obj;
		if(pChapterInfo->getCid()==cid) return pChapterInfo;
	}
    return NULL;
}

void PveModel::reset()
{
    CCObject* obj = NULL;
    ChapterInfo* pChapterInfo = NULL;
    CCARRAY_FOREACH(m_Chapters, obj)
    {
        pChapterInfo = (ChapterInfo*)obj;
        pChapterInfo->reset();
    }

	CCARRAY_FOREACH(m_ActivityChapters, obj)
	{
		pChapterInfo = (ChapterInfo*)obj;
		pChapterInfo->reset();
	}
}

void PveModel::getChapters()
{
    if(!isSynced)
    {
        HTTP_CLIENT->getChapters();  //请求章节列表
        
        user::UserInfo *pUserInfo = DM_GET_USER_MODEL->getUserInfo();
        if (!pUserInfo)
        {
            Tips_Error("pve::init getUserInfo");
            return;
        }
        
        uint32_t mid = pUserInfo->getMid();
        freshChapterByMid(mid);
		freshChapterByMid(m_ActivityMid);
    }
}

//根据最新关卡mid（剧情or活动）更新章节列表
void PveModel::freshChapterByMid(uint32_t mid)
{
	if (mid < D_CONFIG_UINT_FOR_KEY(PVE_DEFAULT_MAP_ID))
	{
		mid = D_CONFIG_UINT_FOR_KEY(PVE_DEFAULT_MAP_ID);
	}

	pve::MapInfo *pMapInfo = getMapInfo(mid);
	if (!pMapInfo)
	{
		Tips_Error("pve::init pMapInfo");
		return;
	}

	if (!pMapInfo->chapterInfo)
	{
		Tips_Error("pve::init chapterInfo");
		return;
	}

	pMapInfo->bUnlock = true;

	uint32_t chapter_id(pMapInfo->chapter_id);
	uint32_t mapId(pMapInfo->getMapId());

	if (pMapInfo->getMapId() == pMapInfo->chapterInfo->lastMid)
	{
		pve::ChapterInfo *pNewChapter = getChapterInfo(pMapInfo->chapter_id+1);
		if (pNewChapter)
		{
			chapter_id = pNewChapter->getCid();
			mapId = pNewChapter->firstMid;
			//pNewChapter->chapter_unlock = true; 解锁状态由下面更新地图信息设置
		}

		HTTP_CLIENT->getMaps(chapter_id);
	}

	if(pMapInfo->chapterInfo->chapterType == m_pActivityID)
	{
		setLastShowActivityMapID(mapId);
		setLastShowActivityChapterID(chapter_id);
		m_bShowActivityMap = true;
	}
	else
	{
		setLastShowMapID(mapId);
		setLastShowChapterID(chapter_id);
		m_bShowMap = true;
	}
	
}

uint32_t PveModel::getMapsCount(uint32_t cid)
{
    cocos2d::CCArray* pMaps = cocos2d::CCArray::create();
    getMapsByChapterId(cid,*pMaps);
    return pMaps->count();
}

void PveModel::getMaps(uint32_t cid)
{
    pve::ChapterInfo* pChapterInfo = getChapterInfo(cid);
    if(!pChapterInfo->isSynced) HTTP_CLIENT->getMaps(pChapterInfo->getCid());
}

bool PveModel::isFailureReason(PveFailureType eType)
{
	PveFailureInfo *pInfo = (PveFailureInfo *)m_pFailureMap->objectForKey(eType);
	uint32_t uLevel = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
	if (uLevel < pInfo->uLevel)
	{
		return false;
	}
	if (eType == PveFailureFormationEmpty)
	{
		return !DM_GET_FORMATION_MODEL->isFormationFull();
	}
	if (eType == PveFailureEquipEnhance)
	{
		return DM_GET_EQUIP_MODEL->needToEnhance();
	}
	FormationModel *pFormationModel = DM_GET_FORMATION_MODEL;
	CCArray *pBattleWarriors = CCArray::create();
	pFormationModel->getBattleWarriors(pBattleWarriors);
	WarriorModel *pWarriorModel = DM_GET_WARRIOR_MODEL;
	if (eType == PveFailureWarriorPromote)
	{
		for (int i = 0; i < pBattleWarriors->count(); i++)
		{
			CCInteger *wid = (CCInteger *)pBattleWarriors->objectAtIndex(i);
			warrior::WarriorCurInfo *pWarrior = pWarriorModel->getWarriorCurInfoByWID(wid->getValue());
			if (pWarriorModel->canWarriorUpNow(pWarrior) == warrior::kUpErrorNone)
			{
				return true;
			}
		}
		return false;
	}
	if (eType == PveFailureWarriorDevelop)
	{
		return DM_GET_ITEM_MODEL->getItemNum(PEIYANG_DAN_ID) >= D_CONFIG_UINT_FOR_KEY(PVE_DEVELOP_ITEM_MIN);
	}
	SkillModel *pSkillModel = DM_GET_SKILL_MODEL;
	if (eType == PveFailureWarriorSkill)
	{
		uint32_t itemNum = DM_GET_ITEM_MODEL->getItemNum(ITEM_NEILIDAN_ID);;
		for (int i = 0; i < pBattleWarriors->count(); i++)
		{
			CCInteger *wid = (CCInteger *)pBattleWarriors->objectAtIndex(i);
			warrior::WarriorCurInfo *pWarrior = pWarriorModel->getWarriorCurInfoByWID(wid->getValue());
			skill::Skill *pSkill = pSkillModel->getSkill(pWarrior->getBaseInfo()->getSkillId());
			if (pWarrior->checkSkillLimit())
			{
				continue;
			}
			uint32_t needNum = pSkillModel->getSkillUpExp(pSkill->grade, pWarrior->getSlv()) - pWarrior->getSexp();
			if (needNum <= itemNum)
			{
				return true;
			}
		}
		return false;
	}
	if (eType == PveFailureWarriorLevel)
	{
		for (int i = 0; i < pBattleWarriors->count(); i++)
		{
			CCInteger *wid = (CCInteger *)pBattleWarriors->objectAtIndex(i);
			warrior::WarriorCurInfo *pWarrior = pWarriorModel->getWarriorCurInfoByWID(wid->getValue());
			if (!pWarrior->checkLevelLimit())
			{
				return true;
			}
		}
		return false;
	}
	if (eType == PveFailureEquipEmpty)
	{
		return !DM_GET_FORMATION_MODEL->isEquipFull();
	}
	
	if (eType == PveFailureCard)
	{
		return true;
	}
	
	return false;
}

PveFailureType PveModel::getFailureReason()
{
	for (int i = 1; i <= PveFailureNormalSize; i++)
	{
		PveFailureType type = (PveFailureType)i;
		if (isFailureReason(type))
		{
			return type;
		}
	}
	return PveFailureCard;
	
/*	FormationModel *pFormationModel = DM_GET_FORMATION_MODEL;
	CCArray *pBattleWarriors = CCArray::create();
	pFormationModel->getBattleWarriors(pBattleWarriors);
	//if (pFormationModel->getUnlockedPositionsCount() > pBattleWarriors->count())
	if (!pFormationModel->isFormationFull())
	{
		return PveFailureFormationEmpty;
	}
	EquipModel *pEquipModel = DM_GET_EQUIP_MODEL;
	CCArray *pEquiped = CCArray::create();
	pEquipModel->getEquiped(pEquiped);
	for (int i = 0; i < pEquiped->count(); i++)
	{
		equip::EquipCurInfo *pInfo = (equip::EquipCurInfo *)pEquiped->objectAtIndex(i);
		if (pInfo->isCanEnhance())
		{
			return PveFailureEquipEnhance;
		}
	}
	bool skillFailure = false;
	bool levelFailure = false;
	WarriorModel *pWarriorModel = DM_GET_WARRIOR_MODEL;
	for (int i = 0; i < pBattleWarriors->count(); i++)
	{
		CCInteger *wid = (CCInteger *)pBattleWarriors->objectAtIndex(i);
		warrior::WarriorCurInfo *pWarrior = pWarriorModel->getWarriorCurInfoByWID(wid->getValue());
		//if (pWarrior->isPromoteable() && !pWarrior->checkPromoteLimit())
		if (pWarriorModel->canWarriorUpNow(pWarrior))
        {
			return PveFailureWarriorPromote;
		}
		if (!pWarrior->checkSkillLimit())
		{
			skillFailure = true;
		}
		if (!pWarrior->checkLevelLimit())
		{
			levelFailure = true;
		}
	}
	ItemModel *pItemModel = DM_GET_ITEM_MODEL;
	if (pItemModel->getItemNum(PEIYANG_DAN_ID) >= PVE_DEVELOP_ITEM_MIN)
	{
		return PveFailureWarriorDevelop;
	}
	if (skillFailure)
	{
		return PveFailureWarriorSkill;
	}
	if (levelFailure)
	{
		return PveFailureWarriorLevel;
	}
	return PveFailureCard;
	*/
}

void PveModel::parseFailure(const CSVDecoder::CSV &val)
{
	m_pFailureMap->removeAllObjects();
	for (uint32_t i=0; i<val.size(); ++i)
	{
		PveFailureInfo *pInfo = PveFailureInfo::create();
		pInfo->nType = (PveFailureType)atoi(val[i][0].c_str());
		pInfo->sText = val[i][1];
		pInfo->uScene = atoi(val[i][2].c_str());
		pInfo->uIndex = atoi(val[i][3].c_str());
        pInfo->sImage = std::string(FAILURE_RESOURCE) + val[i][4] + std::string(".png");
		pInfo->uLevel = atoi(val[i][5].c_str());
		pInfo->uPriority = atoi(val[i][6].c_str());
		m_pFailureMap->setObject(pInfo, pInfo->nType);
	}
}

PveFailureInfo *PveModel::getFailureInfo()
{
	CCDictElement * pElement = NULL;
	PveFailureInfo * pRet = NULL;
	CCDICT_FOREACH(m_pFailureMap, pElement)
	{
		PveFailureInfo * pInfo = (PveFailureInfo *)pElement->getObject();
		if (0 == pInfo->uPriority)
		{
			continue;
		}
		if (pRet && pRet->uPriority < pInfo->uPriority)
		{
			continue;
		}
		if (isFailureReason(pInfo->nType))
		{
			pRet = pInfo;
		}
	}
	if (NULL == pRet)
	{
		pRet = (PveFailureInfo *)m_pFailureMap->objectForKey(PveFailureDefault);
	}
	
	return pRet;
//	PveFailureType type = getFailureReason();
//	return (PveFailureInfo *)m_pFailureMap->objectForKey(type);
}

PveFailureInfo *PveModel::getFailureInfoHigher()
{
	//PveFailureType type = randomHigherReason();
	//return (PveFailureInfo *)m_pFailureMap->objectForKey(type);
	uint32_t uLevel = DM_GET_USER_MODEL->getUserInfo()->getUserLv();
	CCArray *pCandidate = CCArray::create();
	for (uint32_t i = PveFailureHigherFirst; i < PveFailureHigherLast; i++)
	{
		PveFailureInfo *pInfo = (PveFailureInfo *)m_pFailureMap->objectForKey(i);
		if (pInfo->uLevel <= uLevel)
		{
			pCandidate->addObject(pInfo);
		}
	}
	return (PveFailureInfo *)pCandidate->randomObject();
}

bool PveModel::isChapterCleared(uint32_t cid)
{
	ChapterInfo *pChapter = getChapterInfo(cid);
	if (!pChapter)
	{
		return false;
	}
	MapInfo *pMap = getMapInfo(pChapter->lastMid);
	if (pMap != NULL)
	{
		return pMap->iScores > 0;
	}
	return false;
}

#pragma mark - ChapterInfo -

bool ChapterInfo::canGetAllStarReward()
{
    if(grade_award) return false;
    return great_num>=DM_GET_PVE_MODEL->getMapsCount(chapter_id);
}

bool ChapterInfo::canGetPrestigeReward()
{
	if(pres<getMaxPrestige()) return false;
	for (uint8_t i = 0; i<PRES_AWARD_COUNT; ++i)
	{
		pve::PrestigeReward* pInfo = getAwardByIndex(i);
		
		//pInfo->prestige_reward_need_viplv>DM_GET_USER_MODEL->getUserInfo()->getVipLevel()
		if(!isPresAwardIsGotten(pInfo->prestige_reward_need_viplv)) 
		{
			return true;
		}

	}
	return false;
}

uint16_t ChapterInfo::getMaxPrestige()
{
    UserModel *pUserModel = DM_GET_USER_MODEL;
    return prestige*pUserModel->getVipInfoByVipLevel(pUserModel->getUserInfo()->getVipLevel())->getPrestigeRatio();
}

PveChapterProgress ChapterInfo::getProgress()
{
	PveChapterProgress result = eChapterNew;
	PveModel *pPveModel = DM_GET_PVE_MODEL;
	pve::MapInfo* mapInfo = pPveModel->getMapInfo(lastMid);//>getMapInfoByIndex(lastMid);
	if(mapInfo->iScores == 0){
		result = eChapterNew;
	}
	else if(great_num == lastMid - firstMid +1){
		result = eChapterCleared;
	}
	else{
		result = eChapterCompete;
	}
	return result;
	//return prestige*pUserModel->getVipInfoByVipLevel(pUserModel->getUserInfo()->getVipLevel())->getPrestigeRatio();
}


bool ChapterInfo::isReWardGottenAll()
{
    pve::PrestigeReward* pPrestigeReward = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pPrestigeRewards, obj)
    {
        pPrestigeReward = static_cast<PrestigeReward*>(obj);
        if(!isPresAwardIsGotten(pPrestigeReward->prestige_reward_need_viplv))
        {
            return false;
        }
    }
    return true;
}

pve::PrestigeReward* ChapterInfo::getAwardToShow()
{
    //如果全部领完 显示0级vip
    //显示最低vip需求的奖励
    pve::PrestigeReward* pPrestigeReward = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pPrestigeRewards, obj)
    {
        pPrestigeReward = static_cast<PrestigeReward*>(obj);
        if(!isPresAwardIsGotten(pPrestigeReward->prestige_reward_need_viplv))
        {
            return pPrestigeReward;
        }
    }
    return static_cast<pve::PrestigeReward*>(m_pPrestigeRewards->objectAtIndex(0));
}

pve::PrestigeReward* ChapterInfo::getAwardByIndex(uint32_t index)
{
    if(index>=m_pPrestigeRewards->count()) return NULL;
    return static_cast<pve::PrestigeReward*>(m_pPrestigeRewards->objectAtIndex(index));
}

pve::PrestigeReward* ChapterInfo::getAwardByVipLv(uint8_t lv)
{
    pve::PrestigeReward* pPrestigeReward = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pPrestigeRewards, obj)
    {
        pPrestigeReward = static_cast<PrestigeReward*>(obj);
        if(pPrestigeReward->prestige_reward_need_viplv == lv)
        {
            return pPrestigeReward;
        }
    }
    return NULL;
}


bool ChapterInfo::isPresAwardIsGotten(uint8_t vip)
{
    vip = vip==0?1:vip;
    return ((chapter_box_bGet >> (vip - 1)) & 1) != 0;
}

void ChapterInfo::setChpterAwardGotten(uint8_t vip)
{
    vip = vip==0?1:vip;
    chapter_box_bGet |= 1 << (vip - 1);
    postNotification(D_EVENT_PVE_CHAPTER_UPDATED);
}

bool ChapterInfo::isPresAwardCanGet(uint8_t vip)
{
    if(isPresAwardIsGotten(vip)) return false;          //如果已经领过
    if(pres<getMaxPrestige()) return false;             //如果声望不够
    if(vip>DM_GET_USER_MODEL->getUserInfo()->getVipLevel()) return false; //vip等级不够
    return true;
}

void PveModel::parseChapterExtraInfo(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		ChapterExtraInfo *posInfo = ChapterExtraInfo::create();
		posInfo->id      = atoi(val[i][0].c_str());
		posInfo->pt		= ccp(atof(val[i][1].c_str()), atof(val[i][2].c_str()));
		posInfo->res	= atoi(val[i][3].c_str());
		posInfo->section = atoi(val[i][4].c_str());
		posInfo->ani = atoi(val[i][5].c_str());
		m_pChapterExtraInfo->setObject(posInfo,posInfo->id);
	}
}


pve::ChapterExtraInfo * PveModel::getChapterExtraInfo(uint32_t id)
{
	return ((pve::ChapterExtraInfo*)m_pChapterExtraInfo->objectForKey(id));
}

CCArray * PveModel::getChapterDrop(unsigned int id)
{
	return (CCArray *)m_pChapterDrop->objectForKey(id);
}

void PveModel::getElites()
{
	HTTP_CLIENT->getElites(this,callfuncJson_selector(PveModel::getElitesHandler),false);
}
void PveModel::getElitesHandler(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	const Json::Value &val = responseData["data"]["explores"];
	if(val.size()>0)
	{
		uint32_t midTmp = 0;
		uint32_t valSize = val.size();
		for (uint32_t i = 0; i<valSize; i++)
		{
			uint32_t mid = val[i]["mid"].asUInt();
			if (mid != 0)
			{
				MapInfo *pMapInfo = getEliteInfo(mid);
				if (pMapInfo)
				{
					DJSON_UINT(pMapInfo->iScores,		val[i], "score");
					DJSON_UINT(pMapInfo->iTimes,		val[i], "times");
					DJSON_UINT(pMapInfo->todayTimes,	val[i], "todayTimes");
					DJSON_UINT(pMapInfo->lastBattleTs,  val[i], "lastBattleTs");

					pMapInfo->eliteUnlock = true;
				}
			}
		}
		postNotification(D_EVENT_PVE_ELITE_UPDATED);
	}
}

CCArray * PveModel::getEliteDrop(unsigned int id)
{
	return (CCArray *)m_pEliteDrop->objectForKey(id);
}

//获取精英副本列表 开启条件解锁
void PveModel::getUnlockedElite(MapInfoMap& vec)
{
	vec.removeAllObjects();

	CCObject* obj = NULL;
	MapInfo* pMapInfo = NULL;
	CCARRAY_FOREACH(m_Elite, obj)
	{
		pMapInfo = (MapInfo*)obj;
		if(pMapInfo->eliteUnlock)
		{
			//解锁章节，显示战斗章节，再显示一章为战斗的章节即可
			if(pMapInfo->iScores>0)
			{
				vec.addObject(pMapInfo);
			}
			else
			{
				vec.addObject(pMapInfo);
				return;
			}
		}
		else
		{
			//未解锁章节，若走到这一步，解锁都已显示，再显示一章未解锁即可
			vec.addObject(pMapInfo);
			return;
		}
	}
}

uint32_t PveModel::getUnlockEliteCount()
{
	MapInfoMap *vec = MapInfoMap::create();
	getUnlockedElite(*vec);
	return vec->count();
}

bool PveModel::isActivityShow()
{
	bool isShow = false;
	if(m_pActivityID>0)
	{
		unsigned int now = GameManager::sharedGameManager()->getServerTime();
		if(now<m_tStartTime || now>m_tEndTime)
		{
			return false;
		}
		return true;
	}
	return isShow;
}

bool PveModel::isCanGetActivityAward()
{
	bool isCom = false;
	bool isGet = static_cast<bool>(m_nActivityAwardState);//是否领过
	pve::ChapterInfo* curInfo= static_cast<pve::ChapterInfo*>(m_ActivityChapters->objectAtIndex(m_ActivityChapters->count()-1));
	pve::MapInfo* mapInfo = this->getMapInfo(curInfo->lastMid);
	if(mapInfo->iScores == 0){
		//尚未全部完成
		isCom = false;
	}
	else
	{
		isCom = !isGet;
	}
	return isCom;
};

unsigned int PveModel::getActivityDownTime()
{
	unsigned int now = GameManager::sharedGameManager()->getServerTime();
	int downTime = m_tEndTime - now;
	if(downTime<0) downTime = 0;

	return downTime;
}
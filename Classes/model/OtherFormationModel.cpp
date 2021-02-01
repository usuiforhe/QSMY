//
//  OtherFormationModel.cpp
//  QSMY
//
//  Created by cll on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "OtherFormationModel.h"
#include "../managers/GameManager.h"
#include "../utils/TimeUtil.h"

USING_NS_CC;
using namespace otherFormation;
using namespace formation;
using namespace warrior;
using namespace equip;


OtherFormationModel::OtherFormationModel()
:m_pUserFormationMap(NULL)
,m_pRobotsMap(NULL)
,m_pRobotFormationInfoMap(NULL)

{
    
}


OtherFormationModel::~OtherFormationModel()
{
	CC_SAFE_RELEASE(m_pRobotsMap);
    CC_SAFE_RELEASE(m_pUserFormationMap);
    CC_SAFE_RELEASE(m_pRobotFormationInfoMap);
}

bool OtherFormationModel::init()
{
    m_pUserFormationMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_pUserFormationMap);

	m_pRobotsMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pRobotsMap);
    
    m_pRobotFormationInfoMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_pRobotFormationInfoMap);
	
	loadPvpFormationData( CSVDecoder::create(PVP_ROBOT_FORMATION_CONFIG_PATH)->getData());

    return Model::init();
}

void OtherFormationModel::loadPvpFormationData(const CSVDecoder::CSV &val)
{
	for(uint32_t i = 0 ; i < val.size() ; i++)
	{
		RobotFormationInfo * pRobotFormation = RobotFormationInfo::create();
		pRobotFormation->fid = atoi(val[i][0].c_str());
		//修复读表
		for(int j = 2 ; j <= FORMATION_POS_MAX*2 + 2 ; j=j+2)
		{
			/*只要存wid就可以了
			CCArray * pPosInfo = CCArray::create();

			for (int k = 2*j-1; k<=2*j; k++)
			{
				CCInteger * intValue = CCInteger::create(atoi(val[i][k].c_str()));
				pPosInfo->addObject(intValue);
			}
			pRobotFormation->posInfoArray->addObject(pPosInfo);
			*/
			uint32_t wid = atoi(val[i][j].c_str());
			if (wid>0)
			{
				pRobotFormation->posInfoArray->addObject(CCInteger::create(wid));
			}
			
		}
        pRobotFormation->name = val[i][16];

		m_pRobotsMap->setObject(pRobotFormation, pRobotFormation->fid);
	}
}


otherFormation::FormationInfo* OtherFormationModel::getRobotFormationInfoByFid(uint32_t id,uint32_t level , unsigned int m_rank)
{
    FormationInfo* pInfo = static_cast<FormationInfo*>(m_pRobotFormationInfoMap->objectForKey(id));
    
    if(pInfo==NULL)
    {
        RobotFormationInfo * pRobotFormation = getRobotFormationByFid(id);
        pInfo = FormationInfo::create();
        
        CCInteger* pId = NULL;
        CCObject* obj = NULL;
        
        CCARRAY_FOREACH(pRobotFormation->posInfoArray, obj)
        {
            pId = static_cast<CCInteger*>(obj);
            int wid = pId->getValue();
            
            formation::PositionInfo *pPos = formation::PositionInfo::create();
            //pPos->iId;             //唯一编号
            pPos->iWarriorId = wid;    //单位编号
            //pPos->iPosId ;
            pPos->iPosStatus = kPositionPosStatusUnLocked;
            pInfo->m_positions->addObject(pPos);
            
            otherFormation::WarriorPropInfo* pWarriorInfo = otherFormation::WarriorPropInfo::create();
            pWarriorInfo->wid = wid;
            pWarriorInfo->m_pBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
            pInfo->m_warPropInfoMap->setObject(pWarriorInfo, pWarriorInfo->wid);
        }
        
        pInfo->m_bIsRobot = true;
        pInfo->m_nUpdateTime = 0;
        pInfo->isSynced = true;
        m_pUserFormationMap->setObject(pInfo,id);
    }
    
    if(pInfo->m_nRobotLevel != level)
    {
        
        otherFormation::WarriorPropInfo* pWarriorInfo = NULL;
        CCDictElement* pElement = NULL;
        CCDictionary* pDic = pInfo->m_warPropInfoMap;
        
        CCDICT_FOREACH(pDic, pElement)
        {
            pWarriorInfo = static_cast<otherFormation::WarriorPropInfo*>(pElement->getObject());
			/************************************************************************/
			/* 这里是万年老坑！
				客户端机器人战斗力算法
				需要与服务器端保持一致（战前查看客户端这里算，战后结算服务端算完返回）
				rank200以内 阵中英雄等级均等于机器人等级*9
				rank200以外 阵中英雄等级均等于机器人等级*9再除以2
				服务端代码位置
				clsChallenge.php getNpc()
			*/
			/************************************************************************/
            pWarriorInfo->lv = (m_rank > 200 ? level * 9 /2 : level * 9);
            
            pWarriorInfo->exp = DM_GET_WARRIOR_MODEL->getWarriorAddExp(level, pWarriorInfo->getBaseInfo()->getRealGrade());

            pWarriorInfo->hp = WarriorModel::computeBasicAddProps(pWarriorInfo->getBaseInfo()->getWarriorId() , pWarriorInfo->getBaseInfo()->getRealGrade(), pWarriorInfo->getBaseInfo()->getBasicHp(), pWarriorInfo->getBaseInfo()->getBasicHpAdd(), pWarriorInfo->lv, pWarriorInfo->uplv , warrior::kTalentTypeHP);
            pWarriorInfo->att = WarriorModel::computeBasicAddProps(pWarriorInfo->getBaseInfo()->getWarriorId() , pWarriorInfo->getBaseInfo()->getRealGrade(), pWarriorInfo->getBaseInfo()->getBasicAttack(), pWarriorInfo->getBaseInfo()->getBasicAttackAdd(), pWarriorInfo->lv, pWarriorInfo->uplv , warrior::kTalentTypeAttack);
            pWarriorInfo->def = WarriorModel::computeBasicAddProps(pWarriorInfo->getBaseInfo()->getWarriorId() , pWarriorInfo->getBaseInfo()->getRealGrade(), pWarriorInfo->getBaseInfo()->getBasicDefence(), pWarriorInfo->getBaseInfo()->getBasicDefenceAdd(), pWarriorInfo->lv, pWarriorInfo->uplv , warrior::kTalentTypeDefence);
            
            pWarriorInfo->speed = pWarriorInfo->getBaseInfo()->getBasicSpeed();
            pWarriorInfo->block = 0;
            pWarriorInfo->crit = 0;
            pWarriorInfo->dodge = 0;
            
            
            float mix_skill = 0.f;
            skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(pWarriorInfo->getBaseInfo()->getSkillId());
            if(pSkill)
            {
                mix_skill = pSkill->getForceRatio(pWarriorInfo->slv);
            }
            
            pWarriorInfo->fight = WarriorModel::computeWarriorForce(pWarriorInfo->hp, pWarriorInfo->att, pWarriorInfo->def, pWarriorInfo->speed, pWarriorInfo->dodge, pWarriorInfo->block, pWarriorInfo->crit, mix_skill);

        }
        
        pInfo->m_nRobotLevel = level;
    }
    
    return pInfo;
}



otherFormation::FormationInfo* OtherFormationModel::getForamtionInfoByUid(uint32_t id)
{
    FormationInfo* pInfo = static_cast<FormationInfo*>(m_pUserFormationMap->objectForKey(id));
    if(pInfo==NULL)
    {
        pInfo = FormationInfo::create();
        pInfo ->m_nId = id;
        m_pUserFormationMap->setObject(pInfo,id);
    }
    return pInfo;
}

otherFormation::FormationInfo* OtherFormationModel::getBattleOtherForamtionInfo()
{
	return getForamtionInfoByUid(m_iCurrentWarriorId);
}

bool FormationInfo::checkFateActivated(warrior::FateInfo *pFateInfo)
{
	if (pFateInfo)
	{
		//弟子缘，判断有关弟子是否都上阵
		if (pFateInfo->type == kFateTypeWarrior)
		{
			CCInteger *member_id = NULL;
			CCObject *obj = NULL;
            
			CCARRAY_FOREACH(pFateInfo->member_ids, obj)
			{
				member_id = (CCInteger*)obj;
				if (!getWarriorPropInfoByWid(member_id->getValue()) && !getGundamPosInfoByWid(member_id->getValue()))
				{
					return false;
				}
			}
            
			return true;
		}
		//装备缘，判断弟子是否已装备某装备
		else if (pFateInfo->type == kFateTypeEquip)
		{
            //弟子是否在阵容中
            
            otherFormation::WarriorPropInfo* pWarrior =  getWarriorPropInfoByWid(pFateInfo->owner_id);
            if(pWarrior == NULL) return false;
            
            EquipBaseInfo* pEquip = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pFateInfo->equip_id);
            if(pEquip==NULL) return false;//装备信息错误 或者 装备还未开放
            formation::PositionInfo* pPos = NULL;
            cocos2d::CCObject* obj = NULL;
            EquipCurInfo* pEquipInfo = NULL;
            CCARRAY_FOREACH(m_positions, obj)
            {
                pPos = static_cast<formation::PositionInfo*>(obj);
                if(pPos->iWarriorId == pWarrior->wid)
                {
                    pEquipInfo =  getEquipInfoByPidAndType(pPos->iId, pEquip->getEquipType());
                    return pEquipInfo && pEquipInfo->getEquipId() == pEquip->getEquipId();
                }
            }
		}
	}
    
	return false;	
}


int8_t FormationInfo::getSpecialSkillLv(skill::Skill* pSkill)
{
    if(!pSkill) return -1;
    warrior::WarriorCurInfo *pWarrior = NULL;
    uint8_t upLevel  = -1; //max
    
    uint8_t warrior_num = 0;	//技能需要合体弟子个数
    uint8_t inpos_num = 0;		//在阵容中个数
    
    uint32_t wids[] = {pSkill->wid1,pSkill->wid2,pSkill->wid3};
    
    for (uint8_t i=0;i<D_CONFIG_UINT_FOR_KEY(SKILL_SPECIAL_MAXWARRIOR);i++)
    {
        pWarrior = NULL;
        
        if(wids[i]<=0) continue;
        
        WarriorPropInfo* pWarriorPropInfo = getWarriorPropInfoByWid(wids[i]);
        if(pWarriorPropInfo)
        {
            inpos_num++;
            if(upLevel>pWarriorPropInfo->uplv) upLevel=pWarriorPropInfo->uplv;
        }
        warrior_num++;
    }
    if (warrior_num == inpos_num) return upLevel;
    return -1;
}

void OtherFormationModel::parseOtherFomations(const Json::Value &val,const time_t &updatedTime)
{
    FormationInfo* pInfo = getForamtionInfoByUid(val["uid"].asUInt());
    pInfo->m_bIsRobot = false;
    
	pInfo->parseFomations(val["formations"]);

	pInfo->parseWarriors(val["warriors"]);

	pInfo->parseEquips(val["equips"]);

	pInfo->parseWarriorProps(val["natures"]);

	pInfo->parseGundamFomations(val["gundam"]);

    pInfo->m_nUpdateTime = updatedTime;
    
    pInfo->isSynced = true;
    
	pInfo->postNotification(D_EVENT_OTHERFORMATION_UPDATE);
}


bool FormationInfo::isTimeOut()
{
    return m_nUpdateTime!=0 && TimeUtil::getDiff(m_nUpdateTime, GameManager::sharedGameManager()->getServerTime())>OTHER_FORMATION_REFRESH_INTERVAL;
}

void FormationInfo::parseFomations(const Json::Value &val)
{
    m_positions->removeAllObjects();
    
    for (unsigned int i=0; i<val.size(); ++i){
        PositionInfo *pPos = parseFomation(val[i]);
        if(pPos!=NULL) m_positions->addObject(pPos);
    }
    sortPositions();
    
}

void FormationInfo::parseGundamFomations( const Value &val )
{
	// "gundamFormation":{"gundamId":0,"masters":{"1":0,"2":0,"3":0,"4":0,"5":0,"6":0,"7":0}}
	m_pGundamPositions->removeAllObjects() ;
	for (unsigned int i=1; i<=val.size(); ++i)
	{
		gundam_engineer_formation::GundamEngineerPosInfo *posInfo = gundam_engineer_formation::GundamEngineerPosInfo::create() ;
		posInfo->id = i ;
		posInfo->posId = i ;
		posInfo->warriorId = val["masters"][CCString::createWithFormat( "%d", i )->getCString()].asUInt() ;
		m_pGundamPositions->addObject( posInfo ) ;
	}
	
}

gundam_engineer_formation::GundamEngineerPosInfo* FormationInfo::getGundamPosInfoByWid( uint32_t wid)
{
	CCObject *obj = NULL;
	gundam_engineer_formation::GundamEngineerPosInfo *posInfo = NULL;
	CCARRAY_FOREACH( m_pGundamPositions, obj )
	{
		posInfo = (gundam_engineer_formation::GundamEngineerPosInfo*)obj ;
		if ( posInfo->warriorId == wid ) 
		{
			return posInfo ;
		}
	}
	return NULL ;
}

void FormationInfo::sortPositions()
{
    int i,j,length = m_positions->data->num;
    PositionInfo ** x = (PositionInfo**)m_positions->data->arr;
    PositionInfo *tempItem;
    
    for(i=1; i<length; i++)
    {
        tempItem = x[i];
        j = i-1;
        
        
        while(j>=0 && ( tempItem->iPosId < x[j]->iPosId) )
        {
            x[j+1] = x[j];
            j = j-1;
        }
        x[j+1] = tempItem;
    }
}

PositionInfo* FormationInfo::getPositionInfoByIndex(uint8_t index)
{
	if(index<0 || index>=m_positions->count()) return NULL;
	return (PositionInfo*)m_positions->objectAtIndex(index);
}


PositionInfo *FormationInfo::parseFomation(const Json::Value &val,PositionInfo * pPos)
{
    if(val["wid"].asUInt()==0) return NULL;
	if (pPos==NULL)
	{
		pPos = PositionInfo::create();
	}

	pPos->iId = val["id"].asUInt();
	pPos->iWarriorId = val["wid"].asUInt();
	pPos->iPosId = val["pid"].asUInt();
	pPos->iPosStatus = val["pstatus"].asUInt();

	return pPos;
}

uint32_t FormationInfo::getFormationLeader()
{
	CCObject* obj = NULL;
	PositionInfo* pPositionInfo = NULL;

	CCARRAY_FOREACH(m_positions, obj)
	{
		pPositionInfo = (PositionInfo*)obj;
		if(pPositionInfo->isLeader()) 
			return pPositionInfo->iWarriorId;
	}
	return 0;

}

RobotFormationInfo * OtherFormationModel::getRobotFormationByFid(uint32_t fid)
{
	return static_cast<RobotFormationInfo*>(m_pRobotsMap->objectForKey(fid));
}

EquipCurInfo * FormationInfo::getEquipInfoById(uint32_t id)
{
	EquipCurInfo *pEquip = NULL;
	CCDictElement* pElement = NULL;

	CCDICT_FOREACH(m_equipInfoMap, pElement)
	{
		pEquip = (EquipCurInfo*)pElement->getObject(); 
		if (pEquip->getId() == id)
		{
			return pEquip;
		}
	}

	return NULL;
}

EquipCurInfo * FormationInfo::getEquipInfoByPidAndType(uint32_t pid,equip::EquipType eType)
{
	EquipCurInfo *pEquip = NULL;
	CCDictElement* pElement = NULL;

	CCDICT_FOREACH(m_equipInfoMap, pElement)
	{
		pEquip = (EquipCurInfo*)pElement->getObject(); 

		if (pEquip->getPid() == pid && pEquip->getBaseInfo()->getEquipType() == eType)
		{
			return pEquip;
		}
		
	}

	return NULL;
}


//获取阵容总战斗值
int  FormationInfo::getFormationTotalForce()
{
    int nTotalForce = 0;
    
	otherFormation::WarriorPropInfo* pInfo = NULL;
    
    CCDictElement * pElement = NULL;
    
    CCDICT_FOREACH(m_warPropInfoMap, pElement)
	{
		pInfo = static_cast<otherFormation::WarriorPropInfo*>(pElement->getObject());
        nTotalForce+=pInfo->fight;
		//CCLOG("-----------------------------------------------new hero%d" , pInfo->wid);
		//CCLOG("-----------------------------------------------%d" , pInfo->hp);
		//CCLOG("-----------------------------------------------%d" , pInfo->att);
		//CCLOG("-----------------------------------------------%d" , pInfo->def);
		//CCLOG("-----------------------------------------------%d" , pInfo->speed);
		//CCLOG("-----------------------------------------------%d" , pInfo->fight);
	}
	//CCLOG("-----------------------------------------------total %d robot_id %d" , nTotalForce );
	
    
    return nTotalForce;
}



void FormationInfo::parseWarriorProps(const Json::Value &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		WarriorPropInfo *pWarrior = getWarriorPropInfoByWid(val[i]["wid"].asUInt());
		if (pWarrior)
		{
			parseWarriorProp(val[i],pWarrior);
		}
		else
		{
			pWarrior = parseWarriorProp(val[i]);
			pWarrior ->m_pBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pWarrior->wid);
            if(pWarrior->m_pBaseInfo)
            {
                m_warPropInfoMap->setObject(pWarrior, pWarrior->wid);
            }else
            {
                CCLOGWARN("error warrior info");
            }
		}
	}
}


void FormationInfo::parseWarriors(const Json::Value &val)
{
	m_warPropInfoMap->removeAllObjects();

	for (uint32_t i=0; i<val.size(); ++i)
	{
		WarriorPropInfo *pWarrior = getWarriorPropInfoByWid(val[i]["wid"].asUInt());

		if (pWarrior)
		{
			parseWarrior(val[i],pWarrior);
		}
		else
		{
			pWarrior = parseWarrior(val[i]);
            pWarrior ->m_pBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pWarrior->wid);
            if(pWarrior->m_pBaseInfo)
            {
               m_warPropInfoMap->setObject(pWarrior, pWarrior->wid); 
            }else
            {
                CCLOGWARN("error warrior info");
            }
			
		}
	
	}

}


WarriorPropInfo* FormationInfo::parseWarrior(const Json::Value &val, WarriorPropInfo *pWarrior)
{
	if(pWarrior == NULL)
	{
		pWarrior =  WarriorPropInfo::create();
	}

	DJSON_UINT(pWarrior->wid,val,"wid");
	DJSON_UINT(pWarrior->exp,val,"exp");
	DJSON_UINT(pWarrior->lv,val,"lv");
	DJSON_UINT(pWarrior->uplv,val,"uplv");
	DJSON_UINT(pWarrior->slv,val,"slv");
	DJSON_UINT(pWarrior->sexp,val,"sexp");
    DJSON_UINT(pWarrior->dressId, val, "dressId");

	return pWarrior;
}





void FormationInfo::parseEquips(const Json::Value &val)
{
	m_equipInfoMap->removeAllObjects();

	for (uint32_t i=0; i<val.size(); ++i)
	{
		EquipCurInfo *pEquip = parseEquip(val[i]);
		m_equipInfoMap->setObject(pEquip, pEquip->getId());
	}
}


EquipCurInfo* FormationInfo::parseEquip(const Json::Value &val, EquipCurInfo *pEquip)
{
    return DM_GET_EQUIP_MODEL->parseEquip(val,pEquip);
}


WarriorPropInfo * FormationInfo::parseWarriorProp(const Json::Value &val, WarriorPropInfo * pProp)
{
	if(pProp == NULL)
	{
		pProp = WarriorPropInfo::create();
	}

	DJSON_UINT(pProp->wid,val,"wid");
	DJSON_UINT(pProp->grade,val,"grade");
	DJSON_UINT(pProp->hp,val,"hp");
	DJSON_UINT(pProp->def,val,"def");
	DJSON_UINT(pProp->att,val,"att");
	DJSON_UINT(pProp->speed,val,"speed");
	DJSON_UINT(pProp->block,val,"block");
	DJSON_UINT(pProp->crit,val,"crit");
	DJSON_UINT(pProp->dodge,val,"dodge");
	DJSON_UINT(pProp->fight,val,"fight");

	return pProp;
}


WarriorPropInfo* FormationInfo::getWarriorPropInfoByWid(uint32_t wid)
{
	return (WarriorPropInfo*)m_warPropInfoMap->objectForKey(wid);
}

bool WarriorPropInfo::checkLevelLimit()
{
    return DM_GET_WARRIOR_MODEL->checkLevelLimit(lv);
}

uint32_t WarriorPropInfo::getAvatarId()
{
    if(dressId!=0) return D_FIX_DRESS_ID(dressId);
    WarriorBaseInfo* pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
    if(pWarrior) return pWarrior->getResId();
    return DEFAULT_UNIT;
}

//
//  BattleModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BattleModel.h"
#include "AppEventType.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "managers/PlatformManager.h"
#include "utils/StringUtil.h"

USING_NS_CC;
using namespace battle;


uint32_t BattleUnit::getAvatarId() const
{
    if(dress_id!=0) return D_FIX_DRESS_ID(dress_id)/* x1000 fix跟弟子id的冲突*/;
    warrior::WarriorBaseInfo* pUnit = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(sid);
    if(pUnit) return pUnit->getResId();
    return DEFAULT_UNIT;
}

BattleModel::BattleModel()
:m_pBattleInfo(NULL)
,m_pSettlementInfo(NULL)
,m_Skills(NULL)
,m_pSceneInfos(NULL)
,m_pBattleDialogs(NULL)
,m_pUnitDialogs(NULL)
,m_bSpeedUp(false)
,m_bShowResult(true)
,m_bShowStart(true)
,m_battleType(kBattleTypeNormal)
,m_bShowFateAni(0)
{
    
}

BattleModel::~BattleModel()
{
    cleanup();
    CC_SAFE_RELEASE(m_Skills);
    CC_SAFE_RELEASE(m_pSceneInfos);
    CC_SAFE_RELEASE(m_pSettlementInfo);
    CC_SAFE_RELEASE(m_pBattleDialogs);
    CC_SAFE_RELEASE(m_pUnitDialogs);
}

bool BattleModel::init()
{
    m_Skills = CCDictionary::create();
    CC_SAFE_RETAIN(m_Skills);
    
    m_pSceneInfos = CCDictionary::create();
    CC_SAFE_RETAIN(m_pSceneInfos);
    
    m_pBattleDialogs = CCDictionary::create();
    CC_SAFE_RETAIN(m_pBattleDialogs);
    
    m_pUnitDialogs = CCDictionary::create();
    CC_SAFE_RETAIN(m_pUnitDialogs);
    
    m_bSpeedUp = CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_BATTLE_SPEEDUP, false);
    
    loadData();
    return Model::init();
}

void BattleModel::loadData()
{
    parseSceneInfo(   CSVDecoder::create(SCENE_INFO_CONFIG_PATH)->getData());
    parseBattleDialog(   CSVDecoder::create(BATTLE_DIALOGUE_CONFIG_PATH)->getData());
    parseUnitDialog(   CSVDecoder::create(UNIT_DIALOGUE_CONFIG_PATH)->getData());
}

void BattleModel::parseBattleDialog(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        uint32_t mapId = atoi(val[i][0].c_str());
        cocos2d::CCArray* pDialogs = createDialog(m_pBattleDialogs,mapId);
        BattleDialogInfo *pDialog = BattleDialogInfo::create();
		
		pDialog->m_Direction  = static_cast<DialogDirection>(atoi(val[i][1].c_str()));
        pDialog->m_uWid  = atoi(val[i][2].c_str());
        pDialog->m_Content  = val[i][3].c_str();
        replace(pDialog->m_Content,"\\n","\n");
        pDialogs->addObject(pDialog);
	}
}

void BattleModel::parseUnitDialog(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        uint32_t dialog_id = atoi(val[i][0].c_str());
        cocos2d::CCArray* pDialogs = createDialog(m_pUnitDialogs,dialog_id);
        BattleDialogInfo *pDialog = BattleDialogInfo::create();
		
		pDialog->m_Direction  = static_cast<DialogDirection>(atoi(val[i][1].c_str()));
        pDialog->m_uWid  = atoi(val[i][2].c_str());
        pDialog->m_Content  = val[i][3].c_str();
        replace(pDialog->m_Content,"\\n","\n");
        pDialogs->addObject(pDialog);
	}
}

cocos2d::CCArray* BattleModel::getUnitDialog(uint32_t dialog_id)
{
    return static_cast<CCArray*>(m_pUnitDialogs->objectForKey(dialog_id));
}

cocos2d::CCArray* BattleModel::getBattleDialog()
{
    if(m_pBattleInfo==NULL) return NULL;
    return static_cast<CCArray*>(m_pBattleDialogs->objectForKey(m_pBattleInfo->map));
}

cocos2d::CCArray* BattleModel::createDialog(CCDictionary* m_pDialogMap, uint32_t id)
{
    CCArray* pRet = static_cast<CCArray*>(m_pDialogMap->objectForKey(id));
    if(pRet) return pRet;
    pRet = cocos2d::CCArray::create();
    m_pDialogMap->setObject(pRet, id);
    return pRet;
}

void BattleModel::parseSceneInfo(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        SceneInfo *pSceneInfo = SceneInfo::create();
		pSceneInfo->id      = atoi(val[i][0].c_str());
		pSceneInfo->effect  = atoi(val[i][1].c_str());
		pSceneInfo->res		= atoi(val[i][2].c_str());
        m_pSceneInfos->setObject(pSceneInfo,pSceneInfo->id);
	}
}

battle::SceneInfo* BattleModel::getSceneInfo(uint32_t id)
{
    return (battle::SceneInfo*)m_pSceneInfos->objectForKey(id);
}

void BattleModel::parseInfo(const Json::Value &val)
{
	if (val.size() == 0)
	{
		return;
	}
	
    cleanup();
    m_pBattleInfo = BattleInfo::create();
    CC_SAFE_RETAIN(m_pBattleInfo);
    m_pBattleInfo->scene = val["scene"].asUInt();
    m_pBattleInfo->map = val["map"].asUInt();
    m_pBattleInfo->grade = val["grade"].asUInt();
    m_pBattleInfo->winner = (ForceType)val["winner"].asUInt();
    
    parseForce(m_pBattleInfo->m_pForces,val["forces"]);
    parseAction(m_pBattleInfo->m_pActions,val["actions"]);
    /* Json::Value::Members members(val["forces"].getMemberNames());
     for (Json::Value::Members::iterator it =members.begin(); it!=members.end(); ++it) {
     const std::string &key =*it;
     
     }
     */
	m_pBattleInfo->validBattleData();
}

void BattleModel::parseInfo(std::vector<char> & vec){
    if(vec.size() < 9)
        return;
	cleanup();
    std::vector<char>::iterator iter = vec.begin();
    
    m_pBattleInfo = BattleInfo::create();
    CC_SAFE_RETAIN(m_pBattleInfo);
    m_pBattleInfo->scene = static_cast<uint32_t>(*iter) &0x000000ff;
    m_pBattleInfo->scene |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
    m_pBattleInfo->scene |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
    m_pBattleInfo->scene |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
    m_pBattleInfo->map = static_cast<uint32_t>(*++iter) &0x000000ff;
    m_pBattleInfo->map |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
    m_pBattleInfo->map |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
    m_pBattleInfo->map |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
    m_pBattleInfo->grade = static_cast<uint8_t>(*++iter);
    m_pBattleInfo->winner = (ForceType)static_cast<uint8_t>(*++iter);
	if(m_pBattleInfo->winner != kForceLeft) m_pBattleInfo->grade = 0;
    parseForce(m_pBattleInfo->m_pForces, vec, iter);
    parseAction(m_pBattleInfo->m_pActions, vec, iter);
	m_pBattleInfo->validBattleData();
}

void BattleModel::parseForce(cocos2d::CCArray * const pForces,const Json::Value &val)
{
    
    for (uint32_t i=0; i<val.size(); ++i) {
        BattleForce *pForce = BattleForce::create();
        pForce->id = val[i]["id"].asUInt();
        pForce->name  = val[i]["name"].asString();
        pForce->speed = val[i]["speed"].asUInt();
        pForce->user_id = val[i]["user_id"].asUInt();
        pForce->is_mob = val[i]["is_mob"].asBool();
        pForce->level = val[i]["level"].asUInt();
        parseUnit(m_pBattleInfo->m_pUnits, pForce, val[i]["units"]);
        pForces->addObject(pForce);
    }
}
void BattleModel::parseForce(cocos2d::CCArray * const pForces,std::vector<char> & vec, std::vector<char>::iterator & iter){
    uint8_t force_num = static_cast<uint8_t>(*++iter);
    for(uint8_t i = 0; i < force_num; ++i){
        BattleForce *pForce = BattleForce::create();
        pForce->id = static_cast<uint8_t>(*++iter);
        uint8_t name_size = static_cast<uint8_t>(*++iter);
        
        pForce->name = "";
        if(name_size > 0)
        {
            pForce->name.append(++iter, iter + name_size);
            iter += name_size - 1;
        }
        
        pForce->speed = static_cast<uint32_t>(*++iter) &0x000000ff;
        pForce->speed |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        pForce->speed |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        pForce->speed |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        
        pForce->user_id = static_cast<uint32_t>(*++iter) &0x000000ff;
        pForce->user_id |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        pForce->user_id |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        pForce->user_id |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        
        pForce->is_mob = *++iter == 0x01;
        
        pForce->level = static_cast<uint16_t>(*++iter) &0x00ff;
        pForce->level |= (static_cast<uint16_t>(*++iter) << 8) & 0xff00;
        
        parseUnit(m_pBattleInfo->m_pUnits, pForce, vec, iter);
        pForces->addObject(pForce);
    }
}


void BattleModel::parseUnit(cocos2d::CCArray* const pUnits,BattleForce * const pForce,const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); ++i) {
        BattleUnit *pUnit = BattleUnit::create();
        pUnit->id = val[i]["id"].asUInt();
        pUnit->sid  = val[i]["sid"].asUInt();
        pUnit->position = val[i]["position"].asUInt();
        pUnit->level = val[i]["level"].asUInt();
        pUnit->max_hp = val[i]["hp"].asUInt();
        pUnit->hp = val[i]["hp"].asUInt();
        pUnit->fight_factor = static_cast<float>(val[i]["fight_factor"].asUInt()) / 1000;
        pUnit->is_boss = val[i]["is_boss"].asBool();
        if(val[i].isMember("is_show")) pUnit->is_show = val[i]["is_show"].asBool();
        if(val[i].isMember("dress_id")) pUnit->dress_id = val[i]["dress_id"].asUInt();
        pUnit->m_pForce = pForce;
        pUnits->addObject(pUnit);
    }
}
void BattleModel::parseUnit(cocos2d::CCArray* const pUnits,battle::BattleForce * const pForce,std::vector<char> & vec, std::vector<char>::iterator & iter){
    uint8_t unit_num = static_cast<uint8_t>(*++iter);
    for(uint8_t i = 0; i < unit_num; ++i){
        BattleUnit *pUnit = BattleUnit::create();
        pUnit->id = static_cast<uint8_t>(*++iter);
        pUnit->sid = static_cast<uint32_t>(*++iter) &0x000000ff;
        pUnit->sid |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        pUnit->sid |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        pUnit->sid |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        pUnit->level = static_cast<uint16_t>(*++iter) &0x00ff;
        pUnit->level |= (static_cast<uint16_t>(*++iter) << 8) & 0xff00;
        pUnit->max_hp = static_cast<uint32_t>(*++iter) &0x000000ff;
        pUnit->max_hp |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        pUnit->max_hp |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        pUnit->max_hp |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        pUnit->hp = static_cast<uint32_t>(*++iter) &0x000000ff;
        pUnit->hp |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        pUnit->hp |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        pUnit->hp |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        pUnit->position = static_cast<uint8_t>(*++iter);
        pUnit->size = static_cast<uint8_t>(*++iter);
        
        uint32_t fight_factor = static_cast<uint32_t>(*++iter) &0x000000ff;
        fight_factor |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        fight_factor |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        fight_factor |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        
        pUnit->fight_factor = static_cast<float>(fight_factor) / 10000;
        pUnit->is_boss = *++iter == 0x01;        
        
        uint32_t dress_id = static_cast<uint32_t>(*++iter) &0x000000ff;
        dress_id |= (static_cast<uint32_t>(*++iter) << 8) & 0x0000ff00;
        dress_id |= (static_cast<uint32_t>(*++iter) << 16) & 0x00ff0000;
        dress_id |= (static_cast<uint32_t>(*++iter) << 24) & 0xff000000;
        pUnit->dress_id = dress_id;
        pUnit->is_show = *++iter == 0x01;
        pUnit->m_pForce = pForce;
		CCLOG("parse battle unit:id %d, sid %d, dressid %d, position:%d, is_show:%d",pUnit->id, pUnit->sid, pUnit->dress_id, pUnit->position, pUnit->is_show);
        pUnits->addObject(pUnit);
    }
}

void BattleModel::parseAction(cocos2d::CCArray * const pActions,const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); ++i){
        
        Action* pAction;
        ActionType type = (ActionType)val[i]["type"].asUInt();
        switch (type)
        {
            case kActionAttack:{
                ActionAttack *pAttack = ActionAttack::create();
                pAction = pAttack;
                break;
            }
            case kActionSkill:{
                ActionSkill *pSkill = ActionSkill::create();
                pSkill->skill_id = val[i]["skill_id"].asUInt();
                pSkill->position = val[i]["position"].asUInt();
                pSkill->target = (SkillTargetType)(pSkill->position>>6);
                parseCoactors(pSkill->m_pCoactors,val[i]["coactors"]);
                pAction = pSkill;
                break;
            }
            case kActionBuff:{
                ActionBuff *pBuff= ActionBuff::create();
                pBuff->buff_id = val[i]["buff_id"].asUInt();
                pAction = pBuff;
                break;
            }
            default:{
                pAction = Action::create();
                break;
            }
        }
        pAction->type = type;
        pAction->actor = val[i]["actor"].asUInt();
        parseResult(pAction, val[i]["results"]);
        pActions->addObject(pAction);
    }
}

void BattleModel::parseAction(cocos2d::CCArray * const pActions,std::vector<char> & vec, std::vector<char>::iterator & iter){
    uint16_t action_num = static_cast<uint16_t>(*++iter) &0x00ff;
    action_num |= (static_cast<uint16_t>(*++iter) << 8) & 0xff00;
    for(uint16_t i = 0; i < action_num; ++i){
        Action* pAction = NULL;
        ActionType type = (ActionType)static_cast<uint8_t>(*++iter);
        switch (type) {
            case kActionAttack:
            {
                ActionAttack * pAttack = ActionAttack::create();
                pAttack->actor = static_cast<uint8_t>(*++iter);
                pAction = pAttack;
                break;
            }
            case kActionSkill:
            {
                ActionSkill *pSkill = ActionSkill::create();
                pSkill->actor = static_cast<uint8_t>(*++iter);
                pSkill->skill_id = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pSkill->skill_id |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pSkill->skill_id |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pSkill->skill_id |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                uint8_t position = static_cast<uint8_t>(*++iter);
                pSkill->position = position & 0x3f;
                pSkill->target = (SkillTargetType)(position >> 6);
                parseCoactors(pSkill->m_pCoactors, vec, iter);
                pAction = pSkill;
                break;
            }
            case kActionBuff:
            {
                ActionBuff * pBuff = ActionBuff::create();
                pBuff->actor = static_cast<uint8_t>(*++iter);
                pBuff->buff_id = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                pAction = pBuff;
                break;
            }
            default:
            {
                pAction = Action::create();;
                pAction->type = type;
                break;
            }
        }
        pAction->type = type;
        parseResult(pAction, vec, iter);
        pActions->addObject(pAction);
    }
}

void BattleModel::parseResult(Action * const pAction,const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
    {
        Result * pResult;
        switch (val[i]["type"].asUInt())
        {
            case kResultDamage:
            {
                ResultDamage *pDamage = ResultDamage::create();
                pDamage->damange = val[i]["damage"].asUInt();
                pDamage->is_critical = val[i]["is_critical"].asBool();
                pDamage->is_dodge = val[i]["is_dodge"].asBool();
                pDamage->is_block = val[i]["is_block"].asBool();
                pResult = pDamage;
                break;
            }
            case kResultAddBuff:
            {
                ResultAddBuff *pBuff = ResultAddBuff::create();
                pBuff->buff_id = val[i]["buff_id"].asUInt();
                pResult = pBuff;
                break;
            }
            case kResultRemoveBuff:
            {
                ResultRemoveBuff *pBuff = ResultRemoveBuff::create();
                pBuff->buff_id = val[i]["buff_id"].asUInt();
                pResult = pBuff;
                break;
            }
            case kResultHeal:
            {
                ResultHeal *pHeal = ResultHeal::create();
                pHeal->heal = val[i]["heal"].asUInt();
                pHeal->is_critical = val[i]["is_critical"].asBool();
                pResult = pHeal;
                break;
            } 
            case kResultShow:
            {
                ResultShow *pShow = ResultShow::create();
                pResult = pShow;
                break;
            }
            case kResultHide:
            {
                ResultHide *pHide = ResultHide::create();
                pResult = pHide;
                break;
            }
            case kResultDialog:
            {
                ResultDialog *pDialog = ResultDialog::create();
                pDialog->dialog_id = val[i]["dialog_id"].asUInt();
                pResult = pDialog;
                break;
            }
            default:
            {
                pResult = Result::create();
                break;
            }
        }
        
        pResult->type = (ResultType)val[i]["type"].asUInt();
        pResult->actor = val[i]["actor"].asUInt();
        pResult->m_pAction = pAction;
        pAction->addResult(pResult);
    }
}
void BattleModel::parseResult(battle::Action * const pAction,std::vector<char> & vec, std::vector<char>::iterator & iter){
    uint8_t res_num = static_cast<uint8_t>(*++iter);
    for(uint8_t i = 0; i < res_num; ++i){
        uint8_t type = static_cast<uint8_t>(*++iter);
        Result * pResult = NULL;
        switch (type) {
            case kResultDamage:
            {
                ResultDamage * pDamage = ResultDamage::create();
                pDamage->actor = static_cast<uint8_t>(*++iter);
                pDamage->damange = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pDamage->damange |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pDamage->damange |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pDamage->damange |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                char special = *++iter;
                pDamage->is_critical = (DAMAGE_RESULT_CRITICAL_FLAG & special) == DAMAGE_RESULT_CRITICAL_FLAG;
                pDamage->is_dodge = (DAMAGE_RESULT_DODGE_FLAG & special) == DAMAGE_RESULT_DODGE_FLAG;
                pDamage->is_block = (DAMAGE_RESULT_BLOCK_FLAG & special) == DAMAGE_RESULT_BLOCK_FLAG;
                pResult = pDamage;
                break;
            }
            case kResultAddBuff:
            {
                ResultAddBuff *pBuff = ResultAddBuff::create();
                pBuff->actor = static_cast<uint8_t>(*++iter);
                pBuff->buff_id = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                pResult = pBuff;
                break;
            }
            case kResultRemoveBuff:
            {
                ResultRemoveBuff *pBuff = ResultRemoveBuff::create();
                pBuff->actor = static_cast<uint8_t>(*++iter);
                pBuff->buff_id = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pBuff->buff_id |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                pResult = pBuff;
                break;
            }
            case kResultHeal:
            {
                ResultHeal * pHeal = ResultHeal::create();
                pHeal->actor = static_cast<uint8_t>(*++iter);
                pHeal->heal = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pHeal->heal |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pHeal->heal |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pHeal->heal |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                pHeal->is_critical = static_cast<uint8_t>(*++iter) == 0x01;
                pResult = pHeal;
                break;
            }
            case kResultShow:
            {
                ResultShow *pShow = ResultShow::create();
                pShow->actor = static_cast<uint8_t>(*++iter);
                pResult = pShow;
                break;
            }
            case kResultHide:
            {
                ResultHide *pHide = ResultHide::create();
                pHide->actor = static_cast<uint8_t>(*++iter);
                pResult = pHide;
                break;
            }
            case kResultDialog:
            {
                ResultDialog *pDialog = ResultDialog::create();
                pDialog->actor = static_cast<uint8_t>(*++iter);
                pDialog->dialog_id = static_cast<uint32_t>(*++iter) & 0x000000ff;
                pDialog->dialog_id |= static_cast<uint32_t>(*++iter) << 8 & 0x0000ff00;
                pDialog->dialog_id |= static_cast<uint32_t>(*++iter) << 16 & 0x00ff0000;
                pDialog->dialog_id |= static_cast<uint32_t>(*++iter) << 24 & 0xff000000;
                pResult = pDialog;
                break;
            }
            default:
            {
                pResult = Result::create();
                break;
            }
        }
        pResult->type = ResultType(type);
        pResult->m_pAction = pAction;
        pAction->addResult(pResult);
    }
}

void BattleModel::parseCoactors(std::vector<uint32_t> * const pCoactors,const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); ++i) {
        pCoactors->push_back(val[i].asUInt());
    }
}

void BattleModel::parseCoactors(std::vector<uint32_t> * const pCoactors,std::vector<char> & vec, std::vector<char>::iterator & iter){
    uint8_t coactor_num = static_cast<uint8_t>(*++iter);
    for(uint8_t i = 0; i < coactor_num; ++ i){
        pCoactors->push_back(static_cast<uint8_t>(*++iter));
    }
}

void BattleModel::cleanup()
{
    CC_SAFE_RELEASE_NULL(m_pBattleInfo);
    CC_SAFE_RELEASE_NULL(m_pSettlementInfo);
    m_bShowResult = true;
    m_bShowStart = true;
   // if(m_Skills) m_Skills->removeAllObjects();
}


void BattleModel::parseSkill(const Json::Value &val)
{
    Skill *pSkill = Skill::create();
    //pSkill->sid = val["sid"].asUInt();
    DJSON_UINT2(pSkill,val,sid);
    
    DJSON_UINT2(pSkill, val, unit_effect_num);
    
    pSkill->screen_cover = val["screen_cover"].asBool();
    pSkill->action = val["action"].asBool();
    if(val.isMember("visible")) pSkill->visible = val["visible"].asBool();
    pSkill->effect_type = (EffectType)val["effect_type"].asUInt();
    pSkill->skill_type = (skill::SkillType)val["skill_type"].asUInt();
    pSkill->skill_layer = (SkillLayerType)val["skill_layer"].asUInt();
    pSkill->effect_layer = (SkillLayerType)val["effect_layer"].asUInt();
    pSkill->unit_effect_layer = (SkillLayerType)val["unit_effect_layer"].asUInt();
    
    parseDmgDelay(pSkill->dmg_delay,val["dmg_delay"]);
    
    pSkill->skill_delay = D_MILLISECOND_TO_SECOND(val["skill_delay"].asDouble());
    pSkill->effect_delay = D_MILLISECOND_TO_SECOND(val["effect_delay"].asDouble());
    pSkill->unit_effect_delay = D_MILLISECOND_TO_SECOND(val["unit_effect_delay"].asDouble());
    pSkill->unit_delay = D_MILLISECOND_TO_SECOND(val["unit_delay"].asDouble());
    pSkill->buff_delay = D_MILLISECOND_TO_SECOND(val["buff_delay"].asDouble());
    
    skill::Skill *pSkillInfo = DM_GET_SKILL_MODEL->getSkill(pSkill->sid);
    if(pSkillInfo==NULL)
    {
        pSkillInfo = skill::Skill::create();
        pSkillInfo->sid = DEFAULT_SKILL;
    }
    pSkill->setSkillInfo(pSkillInfo);
    
    m_Skills->setObject(pSkill, pSkill->sid);
}

void BattleModel::parseSkill(uint32_t sid, const char* filePath)
{
    if(getSkill(sid)) return;
    unsigned long size = 0;
    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(filePath, "rt", &size);
    if (pBuffer != NULL && size > 0)
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(pBuffer, root);
        root["sid"] = sid;
        parseSkill(root);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
}

battle::Skill* BattleModel::getSkill(uint32_t sid)
{
    if(sid==DEFAULT_SKILL) return (battle::Skill*)m_Skills->objectForKey(DEFAULT_SKILL);
    return  (battle::Skill*)m_Skills->objectForKey(DM_GET_SKILL_MODEL->getSkill(sid)->getResId());
}

void BattleModel::parseDmgDelay(cocos2d::CCArray * const pDmgDelays,const Json::Value &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
    {
        DmgDelay* pDmgDelay= DmgDelay::create();
        pDmgDelay->percent = val[i]["percent"].asDouble()/100.0f;
        pDmgDelay->delay = D_MILLISECOND_TO_SECOND(val[i]["delay"].asDouble());
        pDmgDelays->addObject(pDmgDelay);
    }
}


//static bool validUnit(uint32_t id)
//{
//    return DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(id)!=NULL && DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(id)->have_resource;
//}

static bool validSkill(uint32_t id)
{
    return DM_GET_SKILL_MODEL->getSkill(id)!=NULL && DM_GET_SKILL_MODEL->getSkill(id)->have_resource;
}

static bool validBuff(uint32_t id)
{
    return DM_GET_BUFF_MODEL->getBuff(id)!=NULL;
}



void BattleInfo::validBattleData()
{
    
    scene = MAX(scene, 1);
    CCObject* obj = NULL;
    
    //检查unit资源
    BattleUnit *pUnit = NULL;
    CCARRAY_FOREACH(m_pUnits, obj)
    {
        pUnit = (BattleUnit*)obj;
//        if(!validUnit(pUnit->sid))
//        {
//            CCLOG("找不到单位%d的配置信息",pUnit->sid);
//            pUnit->sid = DEFAULT_UNIT;
//        }
        if(!pUnit->position && !pUnit->is_show)
			continue;
		warrior::WarriorBaseInfo* pWarrorBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pUnit->getAvatarId());
		if(pUnit->m_pForce->id == battle::kForceRight && pWarrorBaseInfo && pWarrorBaseInfo->getFlipRes() > 0)
		{
			CCLOG("flip battle unit:%d", pWarrorBaseInfo->getFlipRes());
			flipUnits.insert(pWarrorBaseInfo->getFlipRes());
		} else 
		{
			CCLOG("battle unit:id %d, sid %d, dressid %d, avatarId %d",pUnit->id, pUnit->sid, pUnit->dress_id, pUnit->getAvatarId());
			units.insert(pUnit->getAvatarId());
		}
    }
    
    Action* pAction = NULL;
    ActionSkill *pActionSkill = NULL;
    ActionBuff *pActionBuff = NULL;
    
    
    Result* pResult = NULL;
    ResultBuff * pResultBuff = NULL;
    CCArray* pResults = NULL;
    
    
    CCARRAY_FOREACH(m_pActions, obj)
    {
        pAction = (Action*)obj;
        switch (pAction->type) {
            case kActionSkill:
            {
                pActionSkill = (ActionSkill*)pAction;
                
                std::vector<uint32_t>::iterator itr;
                
                for (itr=pActionSkill->m_pCoactors->begin(); itr!=pActionSkill->m_pCoactors->end(); ++itr)
                {
                    if(*itr==pActionSkill->actor)   //防止释放者包含在协助者列表中
                    {
                        pActionSkill->m_pCoactors->erase(itr);
                        break;
                    }
                }
				if(pActionSkill->m_pCoactors->size() > 0)
				{
					triggerCoAction = true;
				}
                if(!validSkill(pActionSkill->skill_id))
                {
                    CCLOG("找不到技能%d的配置信息",pActionSkill->skill_id);
                    pActionSkill->skill_id = DEFAULT_SKILL;
                    skills.insert(DEFAULT_SKILL);
                }else
                {
                    skills.insert(DM_GET_SKILL_MODEL->getSkill(pActionSkill->skill_id)->getResId());
                }
                
                break;
            }
            case kActionBuff:
                pActionBuff = (ActionBuff*)pAction;
                if(!validBuff(pActionBuff->buff_id))
                {
                    CCLOG("找不到Buff%d的配置信息",pActionBuff->buff_id);
                    pActionBuff->buff_id = DEFAULT_BUFF;
                }
                buffs.insert(pActionBuff->buff_id);
                break;
                
            default:
                break;
        }
        
        pResults = pAction->getResults();
        
        CCObject* obj2 = NULL;
        CCARRAY_FOREACH(pResults, obj2)
        {
            pResult = (Result*)obj2;
            switch (pResult->type) {
                case battle::kResultAddBuff:
                case battle::kResultRemoveBuff:
                    pResultBuff = (ResultBuff*)pResult;
                    if(!validBuff(pResultBuff->buff_id))
                    {
                        CCLOG("找不到Buff%d的配置信息",pResultBuff->buff_id);
                        pResultBuff->buff_id = DEFAULT_BUFF;
                    }
                    buffs.insert(pResultBuff->buff_id);
                    break;
                    
                default:
                    break;
            }
        }
    }
    
}

bool BattleModel::getSpeedUp()
{
    return m_bSpeedUp && canSpeedUp();
}

bool BattleModel::canSpeedUp()
{
    return DM_GET_USER_MODEL->getUserInfo()->getMonthCard() || DM_GET_USER_MODEL->checkPrivilegeType(user::PrivilegeTypeSpeed);
}


void BattleModel::setSpeedUp(bool val)
{
    if(val && !canSpeedUp())
    {
        Tips_Error(D_LOCAL_STRING("CantSpeedUp").c_str());
        return;
    }
    if(m_bSpeedUp== val) return;
    m_bSpeedUp = val;
    CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_BATTLE_SPEEDUP, val);
    CCUserDefault::sharedUserDefault()->flush();
}

float BattleModel::getBattleSpeed()
{
	ConfigModel *m_pConfigModel = DM_GET_CONFIG_MODEL;
    if(!canSpeedUp())return m_pConfigModel->getConfigByKey(BATTLE_SPEED_NORMAL)->floatValue(); //防止作弊
    return m_bSpeedUp?m_pConfigModel->getConfigByKey(BATTLE_SPEEDUP)->floatValue():m_pConfigModel->getConfigByKey(BATTLE_SPEED_NORMAL)->floatValue();
}


//
//  BurnModel.cpp
//  公会
//
//  Created by Eci on 13-10-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "BurnModel.h"
#include "../network/HttpClient.h"
#include "../managers/GameManager.h"

using namespace cocos2d;

BurnModel::~BurnModel()
{
	CC_SAFE_RELEASE(m_pBurnDict);
	CC_SAFE_RELEASE(m_pTreasureList);
	CC_SAFE_RELEASE(m_pEquipList);
	CC_SAFE_RELEASE(m_pSoulDict);
	CC_SAFE_RELEASE(m_pTreasureDict);
	CC_SAFE_RELEASE(m_pTreasureLevel);
	CC_SAFE_RELEASE(m_pTreasureResistForLevel);
}

bool BurnModel::init()
{
	m_pBurnDict = CCDictionary::create();
	CC_SAFE_RETAIN(m_pBurnDict);

	m_pSoulDict = CCDictionary::create();
	CC_SAFE_RETAIN(m_pSoulDict);

	m_pTreasureList = CCArray::create();
	CC_SAFE_RETAIN(m_pTreasureList);	

	m_pEquipList = CCArray::create();
	CC_SAFE_RETAIN(m_pEquipList);	
	
	m_pTreasureDict = CCDictionary::create();
	CC_SAFE_RETAIN(m_pTreasureDict);

	m_pTreasureLevel = CCDictionary::create();
	CC_SAFE_RETAIN(m_pTreasureLevel);

	m_pTreasureResistForLevel = CCDictionary::create();
	CC_SAFE_RETAIN(m_pTreasureResistForLevel);

	LoadBurnData(CSVDecoder::create(BURN_CONFIG_PATH)->getData());
	LoadTreasureInitData(CSVDecoder::create(TREASURE_INIT_PATH)->getData());
	LoadTreasureLevelData(CSVDecoder::create(TREASURE_LEVEL_PATH)->getData());
	LoadTreasureResistData(CSVDecoder::create(TREASURE_RESIST_PATH)->getData());
	return Model::init();
}

unsigned int BurnModel::getTreasureExpAdd(equip::EquipCurInfo* pInfo)
{
	unsigned int exp = 0;
	int color = (int)pInfo->getBaseInfo()->getEquipGrade();
	exp += m_initTreasureNum[color - 1];
	exp += pInfo->getExp() * TREASURE_RATE;
	return exp;
}

void BurnModel::LoadTreasureResistData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		CCArray* pList = CCArray::create();
		pList->addObject(CCInteger::create(0));
		pList->addObject(CCInteger::create(atoi(val[i][1].c_str())));
		pList->addObject(CCInteger::create(atoi(val[i][2].c_str())));
		pList->addObject(CCInteger::create(atoi(val[i][3].c_str())));
		m_pTreasureResistForLevel->setObject(pList , atoi(val[i][0].c_str()));
	}	
}

void BurnModel::LoadTreasureLevelData(const CSVDecoder::CSV &val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		CCArray* pList = CCArray::create();
		pList->addObject(CCInteger::create(0));
		pList->addObject(CCInteger::create(atoi(val[i][1].c_str())));
		pList->addObject(CCInteger::create(atoi(val[i][2].c_str())));
		pList->addObject(CCInteger::create(atoi(val[i][3].c_str())));
		m_pTreasureLevel->setObject(pList , atoi(val[i][0].c_str()));
	}	
}

void BurnModel::LoadTreasureInitData(const CSVDecoder::CSV &val)
{
	m_initTreasureNum[1] = atoi(val[0][1].c_str());
	m_initTreasureNum[2] = atoi(val[0][2].c_str());
	m_initTreasureNum[3] = atoi(val[0][3].c_str());
}

void BurnModel::LoadBurnData(const CSVDecoder::CSV &val)
{
	for(uint32_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++)
	{
		m_initCoinNum[i] = atoi(val[0][i+1].c_str());
		m_initExpNum[i] = atoi(val[0][i+1+WARRIOR_GRADE_NUM].c_str());
		m_initProNum[i] = atoi(val[0][i+1+WARRIOR_GRADE_NUM+WARRIOR_GRADE_NUM].c_str());
		m_initSoulNum[i] = atoi(val[0][i+1+WARRIOR_GRADE_NUM+WARRIOR_GRADE_NUM+WARRIOR_GRADE_NUM].c_str());
	}
}

bool BurnModel::isSelected(uint32_t wid , BurnModel::BurnType burnType)
{
	//是否在字典里
	bool isSelect = false;

	if(burnType == BurnModel::kBurnTypeHero)
	{
		if(m_pBurnDict->objectForKey(wid)) isSelect = true;
	}
	else if(burnType == BurnModel::kBurnTypeEquip)
	{
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(m_pEquipList, pObj)
		{
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObj);
			int id = pInfo->getId();
			if(id == wid){
				isSelect = true;
				break;
			}
		}
	}

	return isSelect;
}

bool BurnModel::isSoulSelected(uint32_t wid)
{
	bool isSelect = false;
	if(m_pSoulDict->objectForKey(wid)) isSelect = true;

	return isSelect;
}

void BurnModel::addToList(uint32_t wid , BurnModel::BurnType burnType)
{
	if(burnType == BurnModel::kBurnTypeHero)
	{
		//ADD
		warrior::WarriorCurInfo * warriorInfo = NULL;
		warriorInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
		if(warriorInfo)
		{
			m_pBurnDict->setObject(warriorInfo, wid);
		}
	}
	else if(burnType == BurnModel::kBurnTypeEquip)
	{
		//ADD
		//如果已经有了就不要添加
		bool bExist = false;
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(m_pEquipList, pObj)
		{		
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObj);
			if(pInfo->getId() == wid){
				bExist = true;
				break;
			}
		}
		if(!bExist){
			equip::EquipCurInfo* pInfo = DM_GET_EQUIP_MODEL->getEquipCurInfoByID(wid);
			m_pEquipList->addObject(pInfo);
		}		
	}
	

	//改变了回报
	changeItem(burnType);
}

void BurnModel::addToSoul(uint32_t wid)
{
	warrior::SoulInfo * warriorInfo = NULL;
	warriorInfo = DM_GET_WARRIOR_MODEL->getSoulInfoByWID(wid);
	if(warriorInfo)
	{
		m_pSoulDict->setObject(warriorInfo, wid);
	}
	changeItem(BurnModel::kBurnTypeHero);
}

void BurnModel::removeFromList(uint32_t wid , BurnModel::BurnType burnType,uint8_t heroType)
{
	if(burnType == BurnModel::kBurnTypeHero)
	{
		if(heroType == 1)
		{
			m_pBurnDict->removeObjectForKey(wid);
		}
		else if(heroType == 2)
		{
			m_pSoulDict->removeObjectForKey(wid);
		}
	}
	else if(burnType == BurnModel::kBurnTypeEquip)
	{
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(m_pEquipList, pObj)
		{
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObj);
			if(pInfo->getId() == wid){
				m_pEquipList->removeObject(pObj);
				break;
			}
		}
	}
	

	//改变了回报
	changeItem(burnType);
}

void BurnModel::changeItem(BurnModel::BurnType burnType)
{
	if(burnType == BurnModel::kBurnTypeHero)
	{
		//计算后 post

		m_uCoin = 0;
		m_uExp = 0;
		m_uPro = 0;
		for(uint8_t i = 0 ; i < HERO_BURN_PREVIEW_COUNT ; i++)
		{
			m_HeroBurnPreview[i] = 0;
		}

		CCDictElement *pEle = NULL;
		long wid =0;
		CCDICT_FOREACH(m_pBurnDict, pEle)
		{
			wid = pEle->getIntKey();

			setBurnGain(wid,1);			
		}
		//消耗魂魄获得进阶丹（魂魄数*对应的魂魄系数）
		CCDICT_FOREACH(m_pSoulDict, pEle)
		{
			wid = pEle->getIntKey();
			setBurnGain(wid,2);		
		}

		//更新界面icon
		postNotification(D_EVENT_BURN_UPDATE);
	}
	else if(burnType == BurnModel::kBurnTypeEquip)
	{

		//宝物炼化预览


		for(uint8_t i = 0 ; i < EQUIP_BURN_PREVIEW_COUNT ; i++)
		{
			m_EquipBurnPreview[i] = 0;
		}

		CCObject* pObj = NULL;
		CCARRAY_FOREACH(m_pEquipList, pObj)
		{
			//CCInteger* pInt = dynamic_cast<CCInteger*>(pObj);
			equip::EquipCurInfo* pEquipInfo = dynamic_cast<equip::EquipCurInfo*>(pObj);//DM_GET_EQUIP_MODEL->getEquipCurInfoByID(pInt->getValue());
			if(pEquipInfo)
			{
				m_EquipBurnPreview[0] += DM_GET_EQUIP_MODEL->getEquipBurnCount(pEquipInfo , 1);
				m_EquipBurnPreview[1] += DM_GET_EQUIP_MODEL->getEquipBurnCount(pEquipInfo , 2);
			}
			else
			{
				CCLOG("ERROR: BurnModel::changeItem");
			}
		}

		//更新界面icon
		postNotification(D_EVENT_BURN_UPDATE);
	}
	
}

void BurnModel::setBurnGain(int wid,int heroType)
{
	uint32_t itemExp1 = DM_GET_WARRIOR_MODEL->getItemExp(D_CONFIG_UINT_FOR_KEY(BURN_ITEM1));
	uint32_t itemExp2 = DM_GET_WARRIOR_MODEL->getItemExp(D_CONFIG_UINT_FOR_KEY(BURN_ITEM2));
	uint32_t itemExp3 = DM_GET_WARRIOR_MODEL->getItemExp(D_CONFIG_UINT_FOR_KEY(BURN_ITEM3));
	uint32_t itemExp4 = DM_GET_WARRIOR_MODEL->getItemExp(D_CONFIG_UINT_FOR_KEY(BURN_ITEM4));
	warrior::WarriorCurInfo * wInfo = NULL;
	if(heroType==1)
	{
		wInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
	}
	else if(heroType == 2)
	{
		wInfo = warrior::WarriorCurInfo::create(wid);
	}
	uint32_t exp = wInfo->getWarriorExp();
	uint32_t pro = wInfo->getWarriorUpLv();

	uint32_t proitem = 0;
	uint32_t coin = 0;
	uint32_t soulNum = 0;
	warrior::WarriorUpSoul *pWarUpSoul = DM_GET_WARRIOR_MODEL->getWarriorUpInfo(wInfo);

	//计算进阶产生的 铜币和进阶丹
	coin = DM_GET_WARRIOR_MODEL->getWarriorUpTotalAmount(wInfo , 1);
	proitem = DM_GET_WARRIOR_MODEL->getWarriorUpTotalAmount(wInfo , 2);
	soulNum = DM_GET_WARRIOR_MODEL->getWarriorUpTotalAmount(wInfo , 3);
	m_HeroBurnPreview[0] += (int)(BURN_RATE * coin);
	m_HeroBurnPreview[5] += (int)(BURN_RATE * proitem);
	m_HeroBurnPreview[5] += (int)((m_initSoulNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1])*soulNum); //消耗魂魄*魂魄系数
	//固定的铜币和进阶丹
	m_HeroBurnPreview[0] += m_initCoinNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1];
	m_HeroBurnPreview[5] += m_initProNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1];

	//计算修为丹
	if(wInfo->getSlv() > 1)
	{
		skill::Skill * pSkillInfo = DM_GET_SKILL_MODEL->getSkill(wInfo->getBaseInfo()->getSkillId());
		unsigned int skillitem = DM_GET_SKILL_MODEL->getSkillUpTotalAmount(pSkillInfo->grade, wInfo->getSlv());
		m_HeroBurnPreview[6] += (int)(BURN_RATE * skillitem);
	}


	//计算经验单
	uint32_t warriorlevel = wInfo->getWarriorLv();
	if(warriorlevel <= 30)
	{
		m_HeroBurnPreview[1] += (int)(exp/itemExp1 * BURN_EXP_RATE + 0.5f);
		m_HeroBurnPreview[1] += m_initExpNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1];
	}
	else if(warriorlevel <= 50)
	{
		m_HeroBurnPreview[2] += (int)(exp/itemExp2 * BURN_EXP_RATE + 0.5f);
		m_HeroBurnPreview[2] += m_initExpNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1];
	}
	else if(warriorlevel <= 70)
	{
		m_HeroBurnPreview[3] += (int)(exp/itemExp3 * BURN_EXP_RATE + 0.5f);
		m_HeroBurnPreview[3] += m_initExpNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1];
	}
	else{
		m_HeroBurnPreview[4] += (int)(exp/itemExp4 * BURN_EXP_RATE + 0.5f);
		m_HeroBurnPreview[4] += m_initExpNum[((int)wInfo->getBaseInfo()->getColorGrade()) - 1];
	}
}

void BurnModel::makeEquipAutoList()
{
	EquipModel::EquipCurInfoVec  m_EquipInfoVec;
	DM_GET_EQUIP_MODEL->getEquipListByTypeIgnoreID(equip::kEquipTypeALL, 0, m_EquipInfoVec);
	CCObject* pObjAll = NULL;
	CCARRAY_FOREACH_REVERSE(&m_EquipInfoVec, pObjAll)
	{		
		//加满了 就退出循环
		if(m_pEquipList->count() == TOTAL_BURN_NUM) break;

		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObjAll);

		//0.除去宝物
		if(pInfo->getBaseInfo()->getEquipType() == equip::kEquipTypeTreasure)
		{
			continue;
		}
		//1.判断该装备是否穿戴
		else if(pInfo->isEquiped())
		{
			continue;
		}
		else{
			bool bExist = false;

			CCObject* pObj = NULL;
			CCARRAY_FOREACH(m_pEquipList, pObj)
			{
				equip::EquipCurInfo* pInfoIn = dynamic_cast<equip::EquipCurInfo*>(pObj);
				
				if(pInfoIn->getId() == pInfo->getId())
				{
					bExist = true;
					break;
				}
			}
			//2.判断该装备是否已经加入
			if(!bExist) m_pEquipList->addObject(pInfo);			
		}
	}

	changeItem(BurnModel::kBurnTypeEquip);

	//最后更新界面
	postNotification(D_EVENT_BURN_AUTOLIST);
	if(m_pEquipList->count() == 0)Tips_Alert(D_LOCAL_STRING("TreasureTips4").c_str());
}

void BurnModel::makeTreasureAutoList(unsigned int m_uCurrentID)
{

	EquipModel::EquipCurInfoVec  m_EquipInfoVec;
	DM_GET_EQUIP_MODEL->getEquipListByTypeIgnoreID(equip::kEquipTypeTreasure, 0, m_EquipInfoVec);
	CCObject* pObjAll = NULL;
	CCARRAY_FOREACH_REVERSE(&m_EquipInfoVec, pObjAll)
	{		
		//加满了 就退出循环
		if(m_pTreasureDict->count() == TREASURE_BURN_MAX) break;

		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObjAll);

		//0.不能自己吞噬自己
		if(pInfo->getId() == m_uCurrentID)
		{
			continue;
		}
		//1.判断该装备是否穿戴
		else if(pInfo->isEquiped())
		{
			continue;
		}
		else{
			//判断是否已经在m_pTreasureDict中
			//若需要加 就加
			bool bExist = false;
			for(int i = 0 ; i < TREASURE_BURN_MAX ; i++)
			{
				if(m_pTreasureDict->objectForKey(i)) 
				{
					if(dynamic_cast<equip::EquipCurInfo*>(m_pTreasureDict->objectForKey(i))->getId() == pInfo->getId())
					{
						bExist = true;
						break;
					}
				}
			}
			//2.判断该装备是否已经加入
			if(bExist) continue;

			//3.加入
			for(int i = 0 ; i < TREASURE_BURN_MAX ; i++)
			{
				if(m_pTreasureDict->objectForKey(i)) continue;
				else m_pTreasureDict->setObject(pInfo , i);break;
			}
		}
	}

	//最后更新界面
	postNotification(D_EVENT_BURN_AUTOLIST);
	if(m_pTreasureDict->count() == 0)Tips_Alert(D_LOCAL_STRING("TreasureTips4").c_str());
}

//network

void BurnModel::doBurn(BurnModel::BurnType burnType)
{
	if(burnType == BurnModel::kBurnTypeHero)
	{
		//烧英雄的情况
		CCArray* list = CCArray::create();
		CCArray* soulList = CCArray::create();
		CCDictElement *pEle = NULL;
		CCDICT_FOREACH(m_pBurnDict, pEle)
		{
			int wid = pEle->getIntKey();
			//TODO
			//warriormodel 验证该ID是否有效
			CCInteger* pwid = CCInteger::create(wid);
			list->addObject(pwid);
		}
		CCDICT_FOREACH(m_pSoulDict, pEle)
		{
			int wid = pEle->getIntKey();
			CCInteger* pwid = CCInteger::create(wid);
			soulList->addObject(pwid);
		}

		HTTP_CLIENT->burn(list ,soulList, this,callfuncJson_selector(BurnModel::parseBurnHero));
	}
	else if(burnType == BurnModel::kBurnTypeEquip)
	{
		//烧宝物的情况
		CCArray* pBurnList = CCArray::create();
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(m_pTreasureList, pObj)
		{
			CCInteger* pInt = dynamic_cast<CCInteger*>(pObj);
			//CCLOG("+++++++++++++++++++equipID = %d" , pInt->getValue());

			equip::EquipCurInfo* pEquipInfo = DM_GET_EQUIP_MODEL->getEquipCurInfoByID(pInt->getValue());
			if(pEquipInfo)
			{
				pBurnList->addObject(pEquipInfo);
			}
			else{
				CCLOG("WRONG : BurnModel::doBurn");
			}
		}
		//烧！
		DM_GET_EQUIP_MODEL->decomposeEquip(pBurnList);
	}
}

bool BurnModel::willShowConfirm(BurnModel::BurnType burnType)
{
	//提示
	bool bShowConfirm = false;

	if(burnType == BurnModel::kBurnTypeHero)
	{
		CCDictElement *pEle = NULL;
		int wid = 0;
		warrior::WarriorBaseInfo* pBase = NULL;
		CCDICT_FOREACH(m_pBurnDict, pEle)
		{
			wid = pEle->getIntKey();
			//是否要做提示
			pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
			if(pBase && pBase->getColorGrade() == warrior::kWarGradeRed)
			{
				bShowConfirm = true;
				break;
			}
		}

		//魂魄可以视为英雄判断
		CCDICT_FOREACH(m_pSoulDict, pEle)
		{
			wid = pEle->getIntKey();
			//是否要做提示
			pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
			if(pBase && pBase->getColorGrade() == warrior::kWarGradeRed)
			{
				bShowConfirm = true;
				break;
			}
		}
	}
	else
	{
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(m_pEquipList, pObj)
		{
			equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pObj);
			if(pInfo->getBaseInfo()->getEquipGrade() == equip::kEquipGradeRed)
			{
				bShowConfirm = true;
				break;
			}
		}
	}
	

	return bShowConfirm;
}

void BurnModel::parseBurnHero(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	
	//0.保存 掉落id
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(requestData[UUID].asUInt()));

	//1.删除英雄 BurnModel and WarriorModel

	//WarriorModel
	CCDictElement *pEle = NULL;
	int wid =0;
	CCDICT_FOREACH(m_pBurnDict, pEle)
	{
		wid = pEle->getIntKey();
		CCArray* warriorlist = DM_GET_WARRIOR_MODEL->getWarriorVec();

		CCObject* obj = NULL;
		warrior::WarriorCurInfo* wInfo = NULL;
		CCARRAY_FOREACH(warriorlist, obj)
		{
			wInfo = (warrior::WarriorCurInfo*)obj;
			if(wInfo->getWid() == wid)
			{
				warriorlist->removeObject(wInfo);
				break;
			}
		}

	}
	CCDICT_FOREACH(m_pSoulDict, pEle)
	{
		wid = pEle->getIntKey();
		CCArray* soulList = DM_GET_WARRIOR_MODEL->getSoulVec();

		CCObject* obj = NULL;
		warrior::SoulInfo* sInfo = NULL;
		CCARRAY_FOREACH(soulList, obj)
		{
			sInfo = (warrior::SoulInfo*)(obj);
			if(sInfo->getWid() == wid)
			{
				//魂魄 不应该移除，而是减去数量 之后再判断是否移除
				warrior::WarriorGrowInfo * pGrowInfo = DM_GET_WARRIOR_MODEL->getWarriorGrowInfoByGrade(sInfo->getBaseInfo()->getRealGrade());
				warrior::SoulInfo *pSoulInfo = warrior::SoulInfo::create(wid,pGrowInfo->getRctSoul());
				DM_GET_WARRIOR_MODEL->removeSoul(pSoulInfo);
				break;
			}
		}

	}

	//BurnModel
	m_pBurnDict->removeAllObjects();
	m_pSoulDict->removeAllObjects();
	//更新界面icon
	postNotification(D_EVENT_BURN_ICON_UPDATE);
}

void BurnModel::parseBurnTreasure(const Json::Value &requestData,const Json::Value &responseData)
{
	//装备分解时 已经处理了掉落 和装备删除

	//BurnModel
	m_pTreasureList->removeAllObjects();
	//更新界面icon
	postNotification(D_EVENT_BURN_ICON_UPDATE);
}

void BurnModel::getAutoList()
{
	HTTP_CLIENT->getAutoList( TOTAL_BURN_NUM , this,callfuncJson_selector(BurnModel::parseAutoList));
}

void BurnModel::parseAutoList(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;

	//先清空
	m_pBurnDict->removeAllObjects();
	m_pSoulDict->removeAllObjects();
	//再根据返回结果添加
	/*for(uint32_t i = 0 ; i < responseData["data"]["wids"].size() ; i++)
	{
		uint32_t wid = responseData["data"]["wids"][i].asUInt();
		addToList(wid , BurnModel::kBurnTypeHero);
	}*/

	//{"code":"0000","data":{"wids":[{"62":2},{"62":2}]},"ts":1418989212}  
	for(uint32_t i = 0 ; i < responseData["data"]["wids"].size() ; i++)
	{
		 Json::Value::Members members(responseData["data"]["wids"][i].getMemberNames());
		 for (Json::Value::Members::reverse_iterator it = members.rbegin(); it!=members.rend(); ++it) {
			 const std::string &key =*it;

			 uint32_t wid =atoi(key.c_str());
			 if(responseData["data"]["wids"][i][key.c_str()] == 1)
			 {
				 addToList(wid , BurnModel::kBurnTypeHero);
			 }
			 else if(responseData["data"]["wids"][i][key.c_str()]==2)
			 {
				 addToSoul(wid);
			 }
		 }
	}

	//更新英雄
	postNotification(D_EVENT_BURN_AUTOLIST);
}

void BurnModel::reset()
{
	//isSynced = false;
	m_pBurnDict->removeAllObjects();
	m_pSoulDict->removeAllObjects();
	m_pTreasureList->removeAllObjects();
	m_pEquipList->removeAllObjects();
	m_uCoin = 0;
	m_uExp = 0;
	m_uPro = 0;
	m_uUid = 0;

	for(uint8_t i = 0 ; i < HERO_BURN_PREVIEW_COUNT ; i++)
	{
		m_HeroBurnPreview[i] = 0;
	}
	for(uint8_t i = 0 ; i < EQUIP_BURN_PREVIEW_COUNT ; i++)
	{
		m_EquipBurnPreview[i] = 0;
	}
}

int BurnModel::getBurnNum()
{
	return m_pSoulDict->count() + m_pBurnDict->count();
}

void BurnModel::doTreasureBurn(unsigned int id)
{
	if(m_pTreasureDict->count() == 0)
	{
		Tips_Alert(D_LOCAL_STRING("TreasureTips1").c_str());
		return;
	}

	bool bCheck = false;
	Json::Value equips(Json::arrayValue);
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_pTreasureDict, pEle)
	{
		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pEle->getObject());		
		equips.append(pInfo->getId());
		if(pInfo->getBaseInfo()->getEquipGrade() == equip::kEquipGradeRed) bCheck = true;
	}
	//是否需要验证
	if(bCheck)
	{
		DMessageBox* pPBox = DMessageBox::showMessageBox(D_LOCAL_STRING("TreasureTips5").c_str(), this, messageBox_callfunc_selector(BurnModel::confirmBurn));
		pPBox->setUserObject(CCInteger::create(id));
	}
	else{		
		HTTP_CLIENT->treasureBurn(id , equips);
	}	
}

void BurnModel::confirmBurn(MessageType type, DMessageBox* target)
{
	if(type == kMessageTypeCancel)return;

	//确认了就烧！！
	Json::Value equips(Json::arrayValue);
	CCDictElement *pEle = NULL;
	CCDICT_FOREACH(m_pTreasureDict, pEle)
	{
		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pEle->getObject());
		equips.append(pInfo->getId());		
	}
	CCInteger* pInt = dynamic_cast<CCInteger*>(target->getUserObject());
	HTTP_CLIENT->treasureBurn(pInt->getValue() , equips);
}

void BurnModel::parseTreasureBurn(const Json::Value &requestData,const Json::Value &responseData)
{
	//1.删除宝物
	CCDictElement *pEle = NULL;
	int eid =0;
	CCDICT_FOREACH(m_pTreasureDict, pEle)
	{
		equip::EquipCurInfo* pInfo = dynamic_cast<equip::EquipCurInfo*>(pEle->getObject());
		eid = pInfo->getId();
		DM_GET_EQUIP_MODEL->removeEquip(eid);
	}

	//BurnModel
	m_pTreasureDict->removeAllObjects();
	postNotification(D_EVENT_BURN_ICON_UPDATE);
}
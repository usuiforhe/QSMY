//
//  NavigationModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "NavigationModel.h"
#include "managers/DataManager.h"

using namespace navigation;
USING_NS_CC;

bool NavigationNode::isUnlocked()
{
    return m_bUnlocked && DM_GET_USER_MODEL->getUserInfo()->getUserLv()>=unLockLevel;
}

uint32_t NavigationNode::getUnlockLevel()
{
    return unLockLevel;
}

NavigationModel::NavigationModel()
:m_pNavigationNodes(NULL)
{
	//init();
}

NavigationModel::~NavigationModel()
{
    CC_SAFE_RELEASE(m_pNavigationNodes);
}


bool NavigationModel::init()
{
    m_pNavigationNodes = CCDictionary::create();
    m_pNavigationNodes->setObject(NavigationNode::create(), KNavCatRoot);   //添加根节点
    CC_SAFE_RETAIN(m_pNavigationNodes);
   
	loadNavLocalData();

	//CCArray* keys_ = m_pNavigationNodes->allKeys();
	//CCObject* pObj = NULL; 
	//CCARRAY_FOREACH(keys_, pObj)
	//{
	//	CCLOG("keys: %d", ((CCInteger*)pObj)->getValue());
	//}
    return Model::init();
}

void NavigationModel::loadNavLocalData()
{
    parseNavData( CSVDecoder::create(NAVIGATION_CONFIG_PATH)->getData());
}

void NavigationModel::parseNavData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
        uint8_t id = atoi(val[i][0].c_str());
        uint8_t parentId = atoi(val[i][1].c_str());
		NavigationNode *pNav = getNavNode(id);
        addToParent(pNav, parentId);
        pNav->m_bUnlocked = atoi(val[i][2].c_str())>0;
        pNav->m_kNavigationType = (NavigationType)atoi(val[i][3].c_str());
        pNav->unLockLevel = atoi(val[i][4].c_str());
	}
}

navigation::NavigationNode*  NavigationModel::getNavNode(uint8_t id)
{
    NavigationNode *pNav = getNavigationNodeById(id);
    if(!pNav)
    {
        pNav = NavigationNode::create();
        pNav-> m_nId = id;
        m_pNavigationNodes->setObject(pNav, id);
    }
    return pNav;
}

void NavigationModel::addToParent(navigation::NavigationNode*  pNode,uint8_t parentId)
{
    pNode->m_pParent = getNavNode(parentId);
    pNode->m_pParent->addChild(pNode);
}

cocos2d::CCArray* NavigationModel::getNavigationNodeChildrenById(uint8_t id)
{
    return getNavigationNodeById(id)->getChildren();
}

cocos2d::CCArray*  NavigationModel::getNavigationRootChildren()
{
    return getNavigationNodeById(KNavCatRoot)->getChildren();
}

navigation::NavigationNode* NavigationModel::getNavigationNodeById(uint8_t id)
{
    return (navigation::NavigationNode*)m_pNavigationNodes->objectForKey(id);
}

navigation::NavigationNode* NavigationModel::getNavigationRoot()
{
    return getNavigationNodeById(KNavCatRoot);
}

cocos2d::CCArray* NavigationModel::getNavigationBrothersById(uint8_t id)
{
    navigation::NavigationNode* pNode = getNavigationNodeById(id);
    if(pNode && pNode->m_pParent) return pNode->m_pParent->getChildren();
    return cocos2d::CCArray::createWithCapacity(0);
}

cocos2d::CCArray* NavigationModel::getNavigationBrothersByNavType(NavigationType tag)
{
    navigation::NavigationNode* pNode = getNavigationNodeByNavType(tag);
    if(pNode && pNode->m_pParent) return pNode->m_pParent->getChildren();
    return cocos2d::CCArray::createWithCapacity(0);
}

navigation::NavigationNode* NavigationModel::getNavigationNodeByNavType(navigation::NavigationType tag)
{
    CCDictElement* pItem = NULL;
    navigation::NavigationNode* pNode = NULL;
    CCDICT_FOREACH(m_pNavigationNodes, pItem)
    {
        pNode = (navigation::NavigationNode*)pItem->getObject();
        if(pNode->m_kNavigationType==tag) return pNode;
    }
    return NULL;
}

void NavigationModel::updateNavigationUnlockStatus()
{
	if (DM_GET_USER_MODEL->getUserInfo() == NULL)
	{
		return;
	}
	
	CCDictElement* pItem = NULL;
	navigation::NavigationNode* pNode = NULL;
	CCDICT_FOREACH(m_pNavigationNodes, pItem)
	{
		pNode = (navigation::NavigationNode*)pItem->getObject();
		if (pNode 
			&& pNode->isUnlocked() == false
			&& DM_GET_USER_MODEL->getUserInfo()->getUserLv() >= pNode->getUnlockLevel())
		{
			setUnlockStatus(pItem->getIntKey(), true);
		}
	}
}

void NavigationModel::setUnlockStatus(const std::vector<uint8_t> &ids,bool status)
{
    std::vector<uint8_t>::const_iterator itr;
    for (itr = ids.begin(); itr!=ids.end(); ++itr)
    {
        setUnlockStatus(*itr,status,false);
    }
    postNotification(D_EVENT_NAV_LOCK_STATUS_UPDATE);
}

void NavigationModel::setUnlockStatus(uint8_t id,bool status)
{
    setUnlockStatus(id,status,true);
}

void NavigationModel::setUnlockStatus(uint8_t id,bool status,bool notifyNavsUpdate)
{
    navigation::NavigationNode* pNav = getNavigationNodeById(id);
    if(pNav)
    {
        pNav->m_bUnlocked = status;
        //pNav->postNotification(D_EVENT_NAV_LOCK_STATUS_UPDATE);
        if(notifyNavsUpdate) postNotification(D_EVENT_NAV_LOCK_STATUS_UPDATE);
    }
}



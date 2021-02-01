//
//  LuaContainer.cpp
//  QSMY
//
//  Created by wanghejun on 13-11-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LuaContainer.h"

#include "LuaContainer.h"
#include "LuaQSMY.h"
#include "LuaProxy/LuaLoader.h"


USING_NS_CC;
USING_NS_CC_EXT;

/************************ LuaContainer class begin*****************************/

LuaContainer::LuaContainer()
:m_pWarrior(NULL)
,m_pSkillInfo(NULL)
,m_pInfoContainer(NULL)
,m_pContainer(NULL)
,m_pCacheList(NULL)
,m_pItemInfoView(NULL)
,isFirstEnter(true)
{
    
}

LuaContainer::~LuaContainer()
{
    CC_SAFE_RELEASE(m_pCacheList);
    removeAllChildren();
    CC_SAFE_RELEASE(m_pWarrior);
    CC_SAFE_RELEASE(m_pSkillInfo);
    CC_SAFE_RELEASE(m_pItemInfoView);
    
    int nHandler = this->getScriptHandler();
    if (nHandler)
    {
        CCScriptEngineProtocol* pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(nHandler, "dealloc");
    }
}


bool LuaContainer::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pCacheList = CCArray::create();
    CC_SAFE_RETAIN(m_pCacheList);
    
    m_pContainer = CCNode::create();
    addChild(m_pContainer,50);
    
    m_pInfoContainer = CCNode::create();
    addChild(m_pInfoContainer,100);
    
//    CCB_READER_AND_ADDCHILD("ui/ccb/common/bg3");
    
    return true;
}

void LuaContainer::onEnter()
{
    DLayer::onEnter();
    if(isFirstEnter)
    {
        isFirstEnter = false;
        runLua();
    }
}

void LuaContainer::closeInfoView(WarriorViewType type)
{
    if(type==kSkillInfoView && m_lastType==kWarriorInfo)
    {
        showInfoView(kWarriorInfo, WarriorDelegate::m_pWarrior);
    }else
    {
        m_pInfoContainer->removeAllChildren();
        WarriorDelegate::closeInfoView(type);

		//可以考虑 将活动与奇遇中，添加头的操作改在LuaContainer::onEnter中执行,这里解决了 从英雄信息弹框返回后没有头的问题
		UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
		addChild(UserInfoView::sharedUserInfoView());	//添加用户基本信息
		//隐藏底部导航
		Game::sharedGame()->setNavigatorVisable(false);
    }
}

void LuaContainer::closeInfoView(InventoryType type)
{
    m_pInfoContainer->removeAllChildren();
}

void LuaContainer::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
{
	
	m_pInfoContainer->removeAllChildren();
    
	if(m_pItemInfoView == NULL)
	{
		m_pItemInfoView = ItemInfoView::create();
		m_pItemInfoView->setDelegate(this);
		CC_SAFE_RETAIN(m_pItemInfoView);
	}
    
	m_pItemInfoView->setItemInfo(pItemInfo);
	m_pInfoContainer->addChild(m_pItemInfoView);
    
}

void LuaContainer::closeInfoView(EquipViewType)
{
    m_pInfoContainer->removeAllChildren();
}
void LuaContainer::showSkill(warrior::WarriorCurInfo * pWarrior , cocos2d::CCObject* param)
{
	if(m_pSkillInfo==NULL)
	{
		m_pSkillInfo = SkillInfoView::create();
		m_pSkillInfo->setSkillViewOnly(true);
		PL_MOVE_NODE_BELOW_CENTER_Y(m_pSkillInfo);
		m_pSkillInfo->setDelegate(this);				
		CC_SAFE_RETAIN(m_pSkillInfo);
	}			
	CCInteger* pSkillId = static_cast<CCInteger*>(param);
	m_pSkillInfo->setSkillInfo(pWarrior,pSkillId->getValue());
	m_pInfoContainer->addChild(m_pSkillInfo);
}
void LuaContainer::showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param)
{
    WarriorDelegate::showInfoView(type, pWarrior, param);
    m_pInfoContainer->removeAllChildren();
    switch (type) {
        case kSkillInfoView:
        {
   //         if(m_pSkillInfo==NULL)
   //         {
   //             m_pSkillInfo = SkillInfoView::create();
   //             m_pSkillInfo->setDelegate(this);				
   //             CC_SAFE_RETAIN(m_pSkillInfo);
   //         }			
   //         CCInteger* pSkillId = static_cast<CCInteger*>(param);
   //         m_pSkillInfo->setSkillInfo(pWarrior,pSkillId->getValue());
			////技能界面不能点击
			//m_pSkillInfo->setViewOnly(true);
   //         m_pInfoContainer->addChild(m_pSkillInfo);

			showSkill(pWarrior , param);
            break;
        }
        case kWarriorInfo:
        {
            if(m_pWarrior==NULL)
            {
                m_pWarrior = WarriorInfoView::create(WarriorInfoView::kTypeCloseOnly);
				PL_MOVE_NODE_BELOW_CENTER_Y(m_pWarrior);
                m_pWarrior->setDelegate(this);
                CC_SAFE_RETAIN(m_pWarrior);
            }
            m_pWarrior->setWarrior(pWarrior);
            m_pInfoContainer->addChild(m_pWarrior);
            break;
        }
        default:
            break;
    }
}

void LuaContainer::showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param)
{
	m_pInfoContainer->removeAllChildren();
	switch (type) {
	case kEquipInfo:
		{
			EquipInfoView* m_pEquip = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
			m_pEquip->setDelegate(this);
			PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquip);

			m_pEquip->setEquip(pEquip);
			m_pInfoContainer->addChild(m_pEquip);
			break;
		}
		//if(m_pEquip==NULL)
		//{
		//	m_pEquip = EquipInfoView::create(EquipInfoView::kTypeViewOnly);
		//	m_pEquip->setDelegate(this);
		//	PL_MOVE_NODE_ABOVE_CENTER_Y(m_pEquip);
		//	CC_SAFE_RETAIN(m_pEquip);
		//}


	default:
		break;
	}
}

void LuaContainer::showInfoView(cocos2d::CCNode* pNode,bool closeOtherInfo)
{
//     if(closeOtherInfo) closeInfoView();
//     m_pContainer->addChild(pNode);
	if (closeOtherInfo)
	{
		closeInfoView();
		m_pContainer->addChild(pNode);
	}
	else
	{
		m_pInfoContainer->removeAllChildren();
		m_pInfoContainer->addChild(pNode);
	}
}

void LuaContainer::closeInfoView()
{
    m_pContainer->removeAllChildren();
}


void LuaContainer::cacheMember(cocos2d::CCObject* pMember)
{
    m_pCacheList->addObject(pMember);
}

void LuaContainer::onBackKey()
{
	//if (m_pInfoContainer && m_pInfoContainer->getChildrenCount())
	//{
	//	m_pInfoContainer->removeAllChildren();
	//	return;
	//}
	//if (m_pContainer && m_pContainer->getChildrenCount())
	//{
	//	// 判断是否有子节点需要相应back事件，如果有则向下传递back信息
	//	CCArray* pChilds = m_pContainer->getChildren();
	//	CCObject* obj = NULL;
	//	CCARRAY_FOREACH(pChilds, obj)
	//	{
	//		BackKeyInterface* pInterface = dynamic_cast<BackKeyInterface*>(obj);
	//		if (pInterface)
	//		{
	//			pInterface->onBackKey();
	//			return;
	//		}
	//	}

	//	closeInfoView();
	//	return;
	//}
	this->onBackKeyForInfoView();
	BackKeyInterface::onBackKey();
}

bool LuaContainer::onBackKeyForInfoView()
{
	if (m_pInfoContainer && m_pInfoContainer->getChildrenCount())
	{
		m_pInfoContainer->removeAllChildren();
		return true;
	}
	if (m_pContainer && m_pContainer->getChildrenCount())
	{
		// 判断是否有子节点需要相应back事件，如果有则向下传递back信息
		CCArray* pChilds = m_pContainer->getChildren();
		CCObject* obj = NULL;
		CCARRAY_FOREACH(pChilds, obj)
		{
			BackKeyInterface* pInterface = dynamic_cast<BackKeyInterface*>(obj);
			if (pInterface)
			{
				pInterface->onBackKey();
				return true;
			}
		}

		closeInfoView();
		return true;
	}
	return false;
}

/************************ LuaContainer class end*****************************/
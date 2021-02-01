//
//  GuildLogTable.cpp
//  LibQSMY
//
//  Created by jk on 14-10-10.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildLogTable.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#define UNSELECT -1

GuildLogTable::GuildLogTable()
:m_pModel(NULL)
,m_pListView(NULL)
,m_pData(NULL)
{
}

GuildLogTable::~GuildLogTable()
{
	CC_SAFE_RELEASE(m_pListView);
    CC_SAFE_RELEASE(m_pData);
}

GuildLogTable* GuildLogTable::create(const cocos2d::CCSize tableSize)
{
	GuildLogTable *pRet = new GuildLogTable();
	if (pRet && pRet->init(tableSize))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool GuildLogTable::init(const cocos2d::CCSize tableSize)
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pData = readCCData("ui/ccb/guild/guildLogCell");
	CC_SAFE_RETAIN(m_pData);
    
	m_pModel = DM_GET_GUILD_MODEL;
    
	m_pListView = DTableView::create(this, tableSize);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->retain();
	this->addChild(m_pListView);
    
	return true;
}

void GuildLogTable::onEnter()
{
	DLayer::onEnter();
	/************************************************************************/
	/* 11.11编译不通过                                                                     */
	/************************************************************************/
    //HTTP_CLIENT->guildLog();
    BINDING_ONLY(m_pModel, this, GuildLogTable::updateView, D_EVENT_GUILD_GET_LOG);
}

void GuildLogTable::updateView()
{
	m_pListView->reloadData();
}

void GuildLogTable::onExit()
{
	DLayer::onExit();
    UNBINDING_ALL(m_pModel, this);
}



CCTableViewCell* GuildLogTable::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildLogTableCell *cell = (GuildLogTableCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildLogTableCell::create(m_pData);
	}
	CCArray * pList = m_pModel->getGuildLog();
	CCArray *pData = (CCArray *)pList->objectAtIndex(idx);
	cell->setData(pData);
	return cell;
}

uint32_t GuildLogTable::numberOfCellsInTableView(CCTableView *table)
{
    CCArray * pList = m_pModel->getGuildLog();
	if (NULL == pList)
	{
		return 0;
	}
	else
	{
		return pList->count();
	}
}


CCSize GuildLogTable::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    CCArray * pList = m_pModel->getGuildLog();
    CCArray * arr = (CCArray *)pList->objectAtIndex(idx);
    
    CCLOG("%d",arr->count());
    if(NULL != arr)
        return CCSizeMake(640 ,arr->count()*24+36+40);
    else
        return CCSizeMake(640,10);
}

void GuildLogTable::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}



void GuildLogTable::onTouch(cocos2d::CCObject* obj)
{

}







GuildLogTableCell::GuildLogTableCell()
:m_pSelfBj(NULL)
, m_pText(NULL)
, m_pGuildName(NULL)
, m_pTextBg(NULL)
, m_fHeight(0.0f)
{
    
}

GuildLogTableCell::~GuildLogTableCell()
{
	CC_SAFE_RELEASE(m_pSelfBj);
	CC_SAFE_RELEASE(m_pGuildName);
	CC_SAFE_RELEASE(m_pText);
    CC_SAFE_RELEASE(m_pTextBg);
}

GuildLogTableCell* GuildLogTableCell::create(cocos2d::extension::CCData* pData)
{
	GuildLogTableCell* pRet = new GuildLogTableCell();
	if (pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool GuildLogTableCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);

    m_pText->setFontFillColor(m_pText->getColor());
    m_pText->setColor(ccWHITE);
	return true;
}

void GuildLogTableCell::setData(CCArray * data){
    CCLOG("%d",data->count());
//    CCString *str = CCString::create("");
    
    

    std::string str = "";
    std::string timeStr  = "";
    
    uint32_t index =  0;
    if( NULL!= data){
        CCObject * obj = NULL;
        guild::GuildLog * glog = NULL;
        CCARRAY_FOREACH(data, obj){
            glog = (guild::GuildLog *)obj;
            
            uint32_t ts = glog->getTs();
            time_t time = (time_t)ts;
            struct tm * ptm;
            ptm = gmtime(&time);

            
            timeStr  = D_LOCAL_STRING("%dYear%dMonth%dDay",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);

            uint32_t hour = ptm->tm_hour;
            uint32_t min  = ptm->tm_min;
            
            CCString* hourStr = NULL;
            CCString* minStr = NULL;
            
            if(hour<10){
                hourStr = CCString::createWithFormat("0%d",hour);
            }
            else{
                hourStr = CCString::createWithFormat("%d",hour);
            }
            if(min<10){
                minStr = CCString::createWithFormat("0%d",min);
            }
            else{
                minStr = CCString::createWithFormat("%d",min);
            }


            
            
            std::string logStr;
            switch (glog->getType())
            {
                case guild::GuildJoin:
                {
                    guild::GuildLogJoin* data = (guild::GuildLogJoin*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%s%sGuildJoin",hourStr->getCString(),minStr->getCString(),data->getusername().c_str());
                }
                    break;
                case guild::GuildOut:
                {
                    guild::GuildLogOut* data = (guild::GuildLogOut*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%s%sGuildOut",hourStr->getCString(),minStr->getCString(),data->getoutname().c_str());
                }
                    break;
                case guild::GuildKick:
                {
                    guild::GuildLogKick* data = (guild::GuildLogKick*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%s%sGuild%sKick",hourStr->getCString(),minStr->getCString(),data->getactname().c_str(),data->getoutname().c_str());
                }
                    break;
                case guild::GuildUp:
                {
                    guild::GuildLogUp* data = (guild::GuildLogUp*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%sGuildUp%dLv",hourStr->getCString(),minStr->getCString(),data->getnewlv());
                }
                    break;
                case guild::GuildChangeName:
                {
                    guild::GuildLogChangeName* data = (guild::GuildLogChangeName*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%s%sGuild%sChangeName",hourStr->getCString(),minStr->getCString(),data->getnewName().c_str(),data->getoldName().c_str());
                }
                    break;
                case guild::GuildDragon:
                {
                    guild::GuildLogDragon* data = (guild::GuildLogDragon*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%sGuild%dDragon",hourStr->getCString(),minStr->getCString(),data->getrank());
                }
                    break;
                case guild::GuildUpKing:
                {
                    guild::GuildLogUpKing* data = (guild::GuildLogUpKing*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%sGuild%dUpKing",hourStr->getCString(),minStr->getCString(),data->getnewlv());
                }
                    break;
                case guild::GuildUpKnight:
                {
                    guild::GuildLogUpKnight* data = (guild::GuildLogUpKnight*)glog->getParams();
                    logStr  = D_LOCAL_STRING("%s%sGuild%dUpKnight",hourStr->getCString(),minStr->getCString(),data->getnewlv());
                }
                    break;
                default:
                    break;
            }
            if(index!=0)
                str.append("\n");
            index++;
            str.append(logStr);
        }
    }
    m_pText->setDimensions(CCSizeZero);
    m_pText->setString(str.c_str());
    m_pGuildName->setDimensions(CCSizeZero);
    m_pGuildName->setString(timeStr.c_str());
    CCSize size = CCSizeMake(580,data->count()*24+36);
    m_pSelfBj->setContentSize(size);
    m_pTextBg->setPosition(ccp(-7, size.height+6));

}


bool GuildLogTableCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelfBj", CCNode*, this->m_pSelfBj);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGuildName", CCLabelTTF*, this->m_pGuildName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pText", CCRichLabelTTF*, this->m_pText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTextBg", CCNode*, this->m_pTextBg);
    
    
	return false;
}



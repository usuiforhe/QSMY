//
// 200RedPacket.h
//  200∫Ï∞¸
//  Created by cll on 15-3-11.
//  Copyright (c) 2015, thedream. All rights reserved.
//
#ifndef __PL_200_Red_Packet_H__
#define __PL_200_Red_Packet_H__

#include "AppInclude.h"
#include "components/DPopup.h"
#include "model/EquipModel.h"

class ItemModel;
class MallModel;
class WarriorModel;
class EquipModel;
class WarriorGradeBg;
class WarriorTypeMark;

class RedPacket200View
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCTableViewDataSource
	, public cocos2d::extension::CCTableViewDelegate
{
public:
	RedPacket200View();
	~RedPacket200View();

	CREATE_FUNC(RedPacket200View);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	//ccb
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	//tableview
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    
    void setPacketId(unsigned int packetId);
private:

	void middleCellChanged(unsigned int index); //todo
	void updateInfoView();
	void updateWarriorInfo(unsigned int id, bool isSoul = false);
	void updateEquipInfo(unsigned int id);
	void updateItemInfo(unsigned int id);
    
    void purchase(cocos2d::CCObject *obj);
    void purchaseCallBack(const Json::Value &requestData,const Json::Value &responseData);
	void closeCallBack(cocos2d::CCObject * obj);
    
    unsigned int m_uPacketId;

	ItemModel                       *m_pItemModel;
	WarriorModel					*m_pWarriorModel;
	EquipModel						*m_pEquipModel;
	MallModel						*m_pMallModel;

	cocos2d::CCArray*					m_pProducts;
	unsigned int				m_pSelectProductId;

	cocos2d::extension::CCTableView*	m_pTableView;

	cocos2d::CCNode*	m_pTableContainer;
	cocos2d::CCNode*	m_pWarriorNode;
	cocos2d::CCNode*	m_pEquipNode;
	cocos2d::CCNode*	m_pItemNode;
    WarriorGradeBg*     m_pWarriorGradeBg;
    DButton*            m_pCloseButton;
    cocos2d::CCLabelBMFont*     m_pPrice;
    
    //warrior
    cocos2d::CCNode*           m_pWarriorContainer;
    cocos2d::CCLabelBMFont*    m_pZizhiNum;
    cocos2d::CCLabelBMFont*    m_pWarriorCapacity;//战斗力
    WarriorTypeMark*           m_pWarriorTypeMark;
    cocos2d::CCNode*           m_pSoulNode;
    cocos2d::CCLabelTTF*       m_pName;
    
    //equipment
    cocos2d::CCLabelBMFont*     m_pEquipCapacity;
    cocos2d::CCLabelBMFont*     m_pEquipProperty;
    cocos2d::CCSprite*          m_pPropertyType[equip::kEquipPropTotal];
    cocos2d::CCSprite*          m_pEquipMark[equip::kEquipPropTotal];
    cocos2d::CCLabelTTF*        m_pEquipName;
    cocos2d::CCLabelBMFont*     m_pEquipLevel;
    cocos2d::CCLabelBMFont*     m_pEquipZizhiNum;
    cocos2d::CCNode*            m_pEquipContainer;
    
    //item
    cocos2d::CCNode*            m_pItemContainer;
    cocos2d::CCSprite*          m_pPieceIcon;
    cocos2d::CCLabelTTF*        m_pItemName;
    cocos2d::CCLabelBMFont*     m_pItemAmount;
    
};

class PacketListCell :
	public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	PacketListCell();
	~PacketListCell();

	CREATE_FUNC(PacketListCell);
    
    virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	
	void setItemName(const std::string& name);
private:
	void updateView();

	cocos2d::CCLabelTTF* m_pName;
	std::string m_itemName;

};

#endif
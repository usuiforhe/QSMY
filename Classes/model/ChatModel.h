//
//  ChatModel.h
//  聊天信息
//
//  Created by mzp on 14-4-23.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Chat_Model__
#define __QSMY_Chat_Model__


#include "Model.h"



#define	 D_EVENT_CHAT_GUILD_GET_SUC	"chat guild get suc"	// 同步消息成功
#define  D_EVENT_CHAT_GUILD_UPDATE "chat guild update"		// 刷新工会聊天信息
#define  D_EVENT_CHAT_GUILD_SEND_SUC "chat guild send suc"	// 消息发送成功

#define	 D_EVENT_CHAT_WORLD_GET_SUC	"chat world get suc"	// 同步消息成功
#define  D_EVENT_CHAT_WORLD_UPDATE "chat world update"		// 刷新工会聊天信息
#define  D_EVENT_CHAT_WORLD_SEND_SUC "chat world send suc"	// 消息发送成功


class ChatModel;

namespace chat
{
	enum ChatType
	{
		kChatTypeNormal           = 0,  //普通聊天
		kChatTypeDrawCardOne =1,	    //抽卡10
		kChatTypeDrawCardTen,			//抽卡1
		kChatTypeCreateGuild,           //创建帮派
		kChatTypeChallengeBoss,         //挑战弃天帝
		kChatTypeTower ,                //千机楼
		kChatTypeDragonValleyPur,       //皇陵寻宝紫色宝箱
		kChatTypeDragonValleyOra,       //皇陵寻宝橙色宝箱
	};

	enum ChatCheckType
	{
		kChatCheckTypeNone,				// 无消息
		kChatCheckTypeWorld,			// 世界频道消息
		kChatCheckTypeGuild,			// 工会频道消息
		kChatCheckTypeAll,				// 所有频道
	};

	// 聊天频道
	enum ChatChannel
	{
		kChatChannelWorld,				// 世界
		kChatChannelGuild,				// 工会
		kChatChannelCount,
	};

	class ChatInfo : public ValueObjcet
	{
	public:
		CREATE_FUNC(ChatInfo);
		D_INIT_DEFAULT(ValueObjcet);

		friend class ::ChatModel;

		CC_SYNTHESIZE_READONLY(unsigned int, id, Id);				// 编号
		CC_SYNTHESIZE_READONLY(unsigned int, uid, Uid);				// 玩家编号
		CC_SYNTHESIZE_READONLY(std::string, name, Name);			// 玩家名字
		CC_SYNTHESIZE_READONLY(std::string, msg, Msg);				// 信息内容
		CC_SYNTHESIZE_READONLY(unsigned int, ts, Ts);				// 时间
	};
	
	class ChatFormat : public ValueObjcet
	{
	public:
		ChatFormat():chat_type(kChatTypeNormal),format_ct(""),format_title("")
		{

		};
		virtual ~ChatFormat()
		{

		};

		CREATE_FUNC(ChatFormat);
		D_INIT_DEFAULT(ValueObjcet);
	public:
		ChatType chat_type;
		std::string format_ct;
		std::string format_title;
	};
}


class ChatModel
	: public Model
{
	// 当前检测到的消息类型
	CC_SYNTHESIZE(chat::ChatCheckType, m_kChatCheckType, ChatCheckType);

public:
	ChatModel();
	~ChatModel();

	virtual bool init();

	// 开启聊天检测
	void openCheckNew();

	// 关闭聊天检测
	void closeCheckNew();

public:
	// 同步工会消息
	bool syncGuild(bool roll);
	// 同步全服消息
	bool syncWorld(bool roll);
	// 发送工会消息
	bool sendGuildMsg(const char* msg);
	// 发送世界信息
	bool sendWorldMsg(const char* msg);
	// 检测新聊天信息
	void checkNew(float dt);
	// 获取频道消息
	cocos2d::CCArray* getChatInfoByChannel(chat::ChatChannel channel);

public:
	void parseSendGuild(const Json::Value& requestData,const Json::Value& responseData);
	void parseGuildInfo(const Json::Value& requestData,const Json::Value& responseData);

	void parseSendWorld(const Json::Value& requestData,const Json::Value& responseData);
	void parseWorldInfo(const Json::Value& requestData,const Json::Value& responseData);

private:
	void onChatGetGuild(const Json::Value& requestData,const Json::Value& responseData);
	void onChatGetWorld(const Json::Value& requestData,const Json::Value& responseData);
	void onChatCheckNew(const Json::Value& requestData,const Json::Value& responseData);
	void updateGuildMsg(float dt);
	void updateWorldMsg(float dt);
	void filterGuildUser();
	void checkNumber(chat::ChatChannel channel);

	void updateGuildInfo();
	void loadChatFormatData(const CSVDecoder::CSV &val);
	chat::ChatFormat* getChatFormatByType(chat::ChatType kType);

private:
	cocos2d::CCDictionary* m_pChatInfoMap;
	cocos2d::CCDictionary* m_pChatInfoBufferMap;
	unsigned int m_nLastTime;		// 最后一次发言的服务器时间
	unsigned int m_nGuildLastId;	// 最后一条工会信息编号
	unsigned int m_nWorldLastId;	// 最后一条世界信息编号
	bool m_bGuildRoll;				// 是否滚动显示消息
	bool m_bWorldRoll;				// 是否滚动显示消息
	bool m_bGuildGetting;			// 正在同步消息
	bool m_bWorldGetting;			// 正在同步消息

	uint32_t m_nGuildID;			// 用来判断公会是否变更
	bool m_bCheckNew;

	cocos2d::CCDictionary* m_pChatFormatDic;
};



#endif	// __QSMY_Chat_Model__
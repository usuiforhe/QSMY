//
//  ErrorCode.h
//  客户端需要处理的错误码
//
//  Created by wanghejun on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_ErrorCode_h
#define QSMY_ErrorCode_h

#define SUC_CODE                "0000"

//系统错误
#define USER_NOT_LOGON          "9000"   //玩家未登录
#define SYSTEM_MAINTAIN         "9005"  //系统维护中

//Tower 千机楼
#define TOWER_OUT_OF_DATE       "1707"  //本轮已结束

#define SOUL_NOT_ENOUGH         "1306"  //魂魄数量不对（被别的玩家抢掉了）

//狩魂
#define SOUL_HUNTER_NOT_EXIST   "3603"  //抢夺的魂魄不存在了
#define SOUL_HUNTER_NO_MORE_SNATCH   "3604"  //抢夺的魂魄已经抢了5个

#define GUILD_APPLICATION_CANCELED	"3509"	//用户已取消申请或加入其它门派
#define GUILD_NO_GUILD	"3504"	//未加入帮派
#define GUILD_HAS_GUILD	"3501"	//已加入帮派
#define GUILDBOSS_DECLARE_WRONG_TIME	"3520"	//不在宣战时间内
#define GUILDBOSS_DECLARED "3521"	//已宣战过
#define GUILDBOSS_NO_DATA	"3536"	//无数据
#define GUILDBOSS_ERROR_USER_RESCUED	"3531"
#define GUILDBOSS_ROB_TARGET_OLD	"3535"	//信息错误
#define GUILDBOSS_NEW_JOIN	"3527"	//苍龙出现期间新加入
#define GUILD_PERMISSION_DENIED	"3510"	//没有权限

#define FRIENDS_NOT_EXIST	"4402"	//好友不存在
#define FRIENDS_TARGET_FULL	"4404"	//对方好友达上限
#define FRIENDS_REQUEST_NOT_EXIST	"4407" //申请不存在
#define FRIENDS_GIFT_FULL	"4416"	//对方礼物已满
#define FRIENDS_WARRIOR_NOT_EXIST	"4312"	//助阵弟子不存在

#define FRIENDS_SELF_FULL	"4403" //好友达上限（本地处理）
#define TEAM_WARRIOR_NOT_EXIST	"4313"	//助阵弟子不存在（本地处理）
#define TEAM_CHAPTER_LOCK	"4301"	//组队副本未解锁（本地处理）
#define TEAM_RESET_MAX	"4303"	//重置次数用完（本地处理）

#define INVAILD_TOKKEN  "8999"  //平台登陆信息验证失败

#define KINGTOME_SEARCH_NOT_EXIST  "5602"  //没有搜索到皇陵
#define KINGTOME_SEARCH_FAIL  "5603"  //搜索皇陵过期

#endif

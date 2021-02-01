//
//  SigninModel.h
//  公会
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__SigninModel__
#define __QSMY__SigninModel__

#define D_EVENT_SIGNIN_UPDATE	"signin_update"

#include "Model.h"
#include "UserModel.h"

class SigninModel;

namespace signin {
	class SigninAward: public ValueObjcet
	{
		CC_SYNTHESIZE_READONLY(unsigned int, m_uWeek, Week);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uDay, Day);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItem0ID, Item0ID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItem1ID, Item1ID);
		CC_SYNTHESIZE_READONLY(unsigned int, m_bDouble, IsDouble);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItem0Type, Item0Type);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItem1Type, Item1Type);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItem0Num, Item0Num);
		CC_SYNTHESIZE_READONLY(unsigned int, m_uItem1Num, Item1Num);
	public:
		SigninAward()
			:m_uWeek(0)
			,m_uDay(0)
			,m_uItem0ID(0)
			,m_uItem1ID(0)
			,m_uItem0Num(0)
			,m_uItem1Num(0)
			,m_uItem0Type(0)
			,m_uItem1Type(0)
			,m_bDouble(false)
		{

		}
		~SigninAward()
		{

		}
		CREATE_FUNC(SigninAward);
		D_INIT_DEFAULT(ValueObjcet);
	protected:
		friend class ::SigninModel;
	};
}

class SigninModel : public Model
{
	CC_SYNTHESIZE_READONLY(unsigned int, m_uDay, Day);
public:
	SigninModel();
	~SigninModel();
	virtual bool init();

	cocos2d::CCArray *getSigninAward();

	void parseData(const Json::Value &val);
	bool isSignedToday();
private:
	void LoadSigninAward(const CSVDecoder::CSV &val);
	cocos2d::CCDictionary *m_pDictSigninAward;
	unsigned int m_uWeek;
	unsigned int m_uSignTs;
};
#endif
//
//  ErrorTipModel.h
//  错误提示信息
//
//  Created by heven on 14-4-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__ErrorTipModel__
#define __QSMY__ErrorTipModel__

#include "Model.h"

class ErrorTipModel:public Model {
    
public:
    
    /**
     *	@brief	显示错误码对应的错误信息
     *
     *	@param 	error_code 	错误码
     */
    void showErrorTips(const std::string& error_code);
    
    /**
     *	@brief	获取错误码对应的错误信息
     *
     *	@param 	error_code 	错误码
     */
    const char* getErrorTip(const std::string& error_code);
    
    virtual bool init();
    
private:
    std::map<std::string,std::string> errorTips;            //错误信息
    
};

#endif /* defined(__QSMY__ErrorTipModel__) */

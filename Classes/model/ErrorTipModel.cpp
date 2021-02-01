//
//  ErrorTipModel.cpp
//  QSMY
//
//  Created by heven on 14-4-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "ErrorTipModel.h"
#include "managers/TipsManager.h"


bool ErrorTipModel::init()
{
    const CSVDecoder::CSV& val =   CSVDecoder::create(ERROR_TIPS_CONFIG_PATH)->getData();
    
    for (uint32_t i=0; i<val.size(); ++i)
    {
        errorTips[val[i][0]] = val[i][1];
    }
    return Model::init();
}

void ErrorTipModel::showErrorTips(const std::string& error_code)
{
    Tips_Error(getErrorTip(error_code));
}

const char* ErrorTipModel::getErrorTip(const std::string& id)
{
    return errorTips[id].c_str();
}
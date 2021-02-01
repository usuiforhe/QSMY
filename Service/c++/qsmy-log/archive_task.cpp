//
//  archive_task.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "archive_task.h"
#include "config.h"

using namespace qsmy_log;

void archive_task::run(){
    s_log.out_string("start archive log task");
    CONNECT_DB(conn);
    {
        PREPARE_SQL(conn, stmt, "CALL P_ARCHIVE_LOG();");
        stmt->executeUpdate();
        stmt->close();
    }
    
    s_log.out_string("log archived completed");
    
    {
        PREPARE_SQL(conn, stmt, "CALL P_KPI(UNIX_TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL -1 DAY)));");
        stmt->executeUpdate();
        stmt->close();
    }
    conn->close();
    
    s_log.out_string("kpi run completed");
}

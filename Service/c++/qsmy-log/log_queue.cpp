//
//  log_queue.cpp
//  qsmy-log
//
//  Created by 汪燕军 on 13-11-27.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#include "log_queue.h"
#include "log_model.h"

using namespace qsmy_log;

void log_queue::enque(boost::shared_ptr<log_model> log)
{
    if(!stopped_)
    {
        boost::mutex::scoped_lock lock(mutex_);
        queue_.push(log);
    }
}

void log_queue::enque(std::vector<boost::shared_ptr<log_model> > & logs)
{
    if(!stopped_)
    {
        boost::mutex::scoped_lock lock(mutex_);
        BOOST_FOREACH(boost::shared_ptr<log_model> log, logs)
        {
            queue_.push(log);
        }
    }
}

void log_queue::start()
{
    if(stopped_)
    {
        stopped_ = false;
        connection_ = boost::shared_ptr<sql::Connection>(SINGLETON(config).connect_db(true));
        connection_->setAutoCommit(false);
        work_thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&log_queue::work, this)));
    }
}

void log_queue::stop()
{
    if(!stopped_)
    {
        stopped_ = true;
        work_thread_->interrupt();
        work_thread_->join();
        work_thread_ = boost::shared_ptr<boost::thread>();
        connection_->close();
    }
}

void log_queue::work()
{
    boost::posix_time::milliseconds ms1(10);
    boost::posix_time::milliseconds ms2(500);
    while(!stopped_)
    {
        bool is_full(run_once());
        try{
            if(is_full)
            {
                boost::this_thread::sleep(ms1);
            }
            else if(!stopped_)
            {
                boost::this_thread::sleep(ms2);
            }
        }
        catch(boost::thread_interrupted & e)
        {
            s_log.out_string("log_queue::work thread interrupted");            
        }
    }
    
    run_once();
}

bool log_queue::run_once()
{
    bool is_full(true);
    {
        boost::mutex::scoped_lock lock(mutex_);
        for(size_t i = buffer_.size(); i < BUFFER_SIZE; ++i)
        {
            if(queue_.empty())
            {
                is_full = false;
                break;
            }
            boost::shared_ptr<log_model> log = queue_.front();
            queue_.pop();
            buffer_.push_back(log);
        }
    }
    
    if(buffer_.empty())
    {
        return false;
    }
    
    try
    {
        if(connection_->isClosed())
        {
            connection_ = boost::shared_ptr<sql::Connection>(SINGLETON(config).connect_db(true));
            connection_->setAutoCommit(false);
        }
        if(connection_->getSchema() != SINGLETON(config).mysql_db)
        {
            connection_->setSchema(SINGLETON(config).mysql_db);
        }
        
        time_t tt = TIME_NOW.get_time_t();
        std::tm * now = std::localtime(&tt);
        
        std::stringstream & ss = latte::fast_stringstream::ss();
        int year = now->tm_year + 1900;
        int month = now->tm_mon +1;
        int day = now->tm_mday;
        ss << "T_LOG_" << year;
        if(month < 10)
        {
            ss << "0";
        }
        ss << month;
        if(day < 10)
        {
            ss << "0";
        }
        ss << day;
        
        std::string table_name = ss.str();
        
        ss.clear();
        ss.str("");
        std::string sql("");
        if(last_table_ != table_name)
        {
            ss << "CREATE TABLE IF NOT EXISTS " << table_name << "\
            (\
            `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增主键',\
            `trans_id` char(36) NOT NULL COMMENT '失误编号',\
            `sequence` tinyint(3) unsigned NOT NULL COMMENT '操作顺序号',\
            `category` tinyint(3) unsigned NOT NULL COMMENT '日志打雷',\
            `user_id` int(10) unsigned NOT NULL COMMENT '玩家编号',\
            `title` varchar(255) NOT NULL DEFAULT '' COMMENT '标题',\
            `content` varchar(2048) NOT NULL DEFAULT '' COMMENT '内容',\
            `operate_id` smallint(5) unsigned NOT NULL COMMENT '操作编号',\
            `log_ts` int(10) unsigned NOT NULL COMMENT '日志时间',\
            `related1` int(10) NOT NULL DEFAULT '0' COMMENT '相关字段',\
            `related2` int(10) NOT NULL DEFAULT '0' COMMENT '相关字段',\
            `related3` int(10) NOT NULL DEFAULT '0' COMMENT '相关字段',\
            `related4` int(10) NOT NULL DEFAULT '0' COMMENT '相关字段',\
            `user_call` bit(1) NOT NULL DEFAULT b'1',\
            PRIMARY KEY (`id`),\
            KEY `log_ts` (`log_ts`)\
            ) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='当前日志表'\
            ";
            sql = ss.str();
            PREPARE_SQL(connection_, createstmt, sql);
            createstmt->executeUpdate();
            last_table_ = table_name;
            ss.clear();
            ss.str("");
        }
        
        ss << "INSERT INTO `" << table_name << "` \
        (                                           \
        `trans_id`,                                \
        `sequence`,                                \
        `category`,                                \
        `user_id`,                                 \
        `title`,                                   \
        `content`,                                 \
        `operate_id`,                              \
        `log_ts`,                                  \
        `related1`,                                \
        `related2`,                                \
        `related3`,                                \
        `related4`,                                \
        `user_call`)                                \
        VALUES                                      \
        (                                           \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?,                                         \
        ?                                          \
        );";
        
        sql = ss.str();
        
        PREPARE_SQL(connection_, stmt, sql);
        
        BOOST_FOREACH(boost::shared_ptr<log_model> log, buffer_)
        {
            log->setParams(stmt.get());
            stmt->executeUpdate();
        }
        stmt->close();
        connection_->commit();
        
#if DEBUG
        ss.clear();
        ss.str("");
        ss << buffer_.size() << " logs saved.";
        s_log.out_string(ss.str());
#endif
        buffer_.clear();
    }
    catch (boost::exception& e)
    {
        s_log.out_error("pili:log_queue:run_once throw exception. what:" + boost::diagnostic_information(e));
        try{
            if(connection_ && !connection_->isClosed())
            {
                connection_->rollback();
            }
        }
        catch(...)
        {
            
        }
        
    }
	catch (std::exception& e)
	{
		s_log.out_error("pili:log_queue:run_once throw exception. what:" + std::string(e.what()));
        try{
            if(connection_ && !connection_->isClosed())
            {
                connection_->rollback();
            }
        }
        catch(...)
        {
            
        }
	}
    
    return is_full;
}

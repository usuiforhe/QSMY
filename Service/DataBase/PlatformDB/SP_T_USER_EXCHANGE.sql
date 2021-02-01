CREATE PROCEDURE `SP_T_USER_EXCHANGE`()
BEGIN
/*活动开启时，程序调用执行,备份活动历史数据,备查询使用*/
	DECLARE tbl INT;
	DECLARE tblname CHAR(36);
	DECLARE curdt CHAR(8);
	SET curdt=REPLACE(DATE_ADD(CURDATE(),INTERVAL 0 DAY),'-','');
	SET tblname=CONCAT('T_USER_EXCHANGE_',curdt);
	
	SELECT COUNT(0) INTO tbl  FROM information_schema.TABLES WHERE TABLE_SCHEMA='PiLi_GameDB' AND TABLE_NAME=tblname;
	IF tbl=0 THEN
			SET @a=CONCAT('ALTER TABLE T_USER_EXCHANGE ',' RENAME TO ',tblname);
			PREPARE cmd FROM @a;
			#SELECT @a;
			EXECUTE cmd;

			SET @b=CONCAT('CREATE TABLE T_USER_EXCHANGE (
										id int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT ''自增编号'',
										activity_id int(10) unsigned NOT NULL COMMENT ''活动编号'',
										user_id int(10) unsigned NOT NULL COMMENT ''玩家编号'',
										contribute int(10) unsigned NOT NULL DEFAULT ''0'' COMMENT ''贡献值'',
										total_contribute int(10) unsigned NOT NULL DEFAULT ''0'' COMMENT ''总贡献度'',
										global_reward int(11) unsigned NOT NULL COMMENT ''全局奖励领取情况 2进制'',
										rank_reward bit(1) NOT NULL DEFAULT b''0'' COMMENT ''排名奖励是否已发放'',
										refresh_ts int(10) unsigned NOT NULL DEFAULT ''0'' COMMENT ''刷新时间'',
										ex_item1 int(10) unsigned NOT NULL DEFAULT ''0'' COMMENT ''兑换组合1'',
										ex_item2 int(10) unsigned NOT NULL DEFAULT ''0'' COMMENT ''兑换组合2'',
										ex_item3 int(11) unsigned NOT NULL DEFAULT ''0'' COMMENT ''兑换组合3'',
										ex_item1_status smallint(2) NOT NULL DEFAULT ''0'',
										ex_item2_status smallint(2) NOT NULL DEFAULT ''0'',
										ex_item3_status smallint(2) NOT NULL DEFAULT ''0'',
										PRIMARY KEY (id),
										UNIQUE KEY idx (activity_id,user_id),
										KEY tc (total_contribute,activity_id)) ENGINE=InnoDB  DEFAULT CHARSET=utf8;');

			PREPARE cmd FROM @b;
			#SELECT @b;
			EXECUTE cmd;

			DEALLOCATE PREPARE cmd;
	ELSE
			SELECT 'Repeat craete table Error!' AS Msg;
	END IF;
		
END
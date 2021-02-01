CREATE PROCEDURE `SP_P_USER_LIMIT_TIME_DRAW_CARD`()
BEGIN
/*凌晨执行，备份前一天数据到tablename_20141212格式的表中*/
	DECLARE tbl INT;
	DECLARE tblname CHAR(36);
	DECLARE curdt CHAR(8);
	SET curdt=REPLACE(DATE_ADD(CURDATE(),INTERVAL -1 DAY),'-','');
	SET tblname=CONCAT('P_USER_LIMIT_TIME_DRAW_CARD_',curdt);

	SELECT COUNT(0) INTO tbl  FROM information_schema.TABLES WHERE TABLE_SCHEMA='PiLi_GameDB' AND TABLE_NAME=tblname;
	IF tbl=0 THEN
		SET @a=CONCAT('CREATE TABLE ',tblname,'(',
		'id int(11) unsigned NOT NULL,
		user_id int(11) NOT NULL,
    name varchar(300) DEFAULT '''',
		point int(10) NOT NULL,
		is_award int(11) NOT NULL DEFAULT ''0'',
		create_ts int(11) NOT NULL,
		update_ts int(11) NOT NULL
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;');
	PREPARE cmd FROM @a;
	#SELECT @a,1;
	EXECUTE cmd;
	DEALLOCATE PREPARE cmd;
	
	/*set @b=CONCAT('INSERT INTO ',tblname ,' SELECT * FROM P_USER_LIMIT_TIME_DRAW_CARD WHERE create_ts>=',UNIX_TIMESTAMP(DATE_ADD(CURDATE(),INTERVAL -1 DAY)),
								' AND create_ts < ',UNIX_TIMESTAMP(CURDATE()),';');*/
	set @b=CONCAT('INSERT INTO ',tblname ,' SELECT * FROM P_USER_LIMIT_TIME_DRAW_CARD;');
	PREPARE cmd FROM @b;
	#SELECT @b,2;
	EXECUTE cmd;	
	
	DEALLOCATE PREPARE cmd;
	
 ELSE
	/*set @c=CONCAT('INSERT INTO ',tblname ,' SELECT * FROM P_USER_LIMIT_TIME_DRAW_CARD WHERE create_ts>=',UNIX_TIMESTAMP(DATE_ADD(CURDATE(),INTERVAL -1 DAY)),
								' AND create_ts <',UNIX_TIMESTAMP(CURDATE()),';');*/
	set @c=CONCAT('INSERT INTO ',tblname ,' SELECT * FROM P_USER_LIMIT_TIME_DRAW_CARD;');
	PREPARE cmd FROM @c;
	#SELECT @c,3;
	EXECUTE cmd;	
	DEALLOCATE PREPARE cmd;

	END IF;

END
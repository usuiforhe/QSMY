CREATE PROCEDURE `P_ARCHIVE_LOG`()
BEGIN
  DECLARE p_tbl_date VARCHAR(10);
	DECLARE p_recent_date INT UNSIGNED;
	DECLARE p_recent_sys_util_date DATE;
  
  SET p_tbl_date=REPLACE(ADDDATE(CURDATE(),INTERVAL -1 DAY),'-','');
	SET p_recent_date = UNIX_TIMESTAMP(CURDATE());
	SET p_recent_sys_util_date =  CURDATE();


	TRUNCATE TABLE T_LOG_RECENT;
  TRUNCATE TABLE T_LOG_SYS_UTIL_RECENT;

#插入日志
	SET @a=CONCAT('INSERT INTO T_LOG_RECENT
	(
		trans_id,
		sequence,
		category,
		user_id,
		title,
		content,
		operate_id,
		log_ts,
		related1,
		related2,
		related3,
		related4,
		user_call
	)
	SELECT
		trans_id,
		sequence,
		category,
		user_id,
		title,
		content,
		operate_id,
		log_ts,
		related1,
		related2,
		related3,
		related4,
		user_call
	FROM T_LOG_',p_tbl_date,' WHERE log_ts < ',p_recent_date);
	PREPARE cmd FROM @a;
  EXECUTE cmd;
	DEALLOCATE PREPARE cmd;

#插入系统使用率
	INSERT INTO T_LOG_SYS_UTIL_RECENT
	(
		date,
		user_id,
		pve,
		wonder,
		tower,
		challenge,
		sign_up,
		draw_card,
		cup,
		equip_enhance,
		warrior_train,
		warrior_up,
		eat_chicken
	)
	SELECT
		date,
		user_id,
		pve,
		wonder,
		tower,
		challenge,
		sign_up,
		draw_card,
		cup,
		equip_enhance,
		warrior_train,
		warrior_up,
		eat_chicken
	FROM T_LOG_SYS_UTIL
	WHERE date < p_recent_sys_util_date;

	DELETE FROM `T_LOG_SYS_UTIL` WHERE `date` < p_recent_sys_util_date;

END
CREATE TABLE `P_USER_WONDER_BOSS` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `boss_info_id` int(11) NOT NULL COMMENT '对应恶鬼罗刹的P_WONDER_BOSS_INFO自增量',
  `cause_hp` int(11) NOT NULL COMMENT '造成伤害',
  `cause_hp_info` varchar(255) NOT NULL COMMENT '伤害明细',
  `create_ts` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`),
  KEY `wdid` (`boss_info_id`)
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8;

CREATE TABLE `P_WONDER_BOSS_INFO` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(11) unsigned NOT NULL,
  `boss_id` smallint(6) unsigned NOT NULL COMMENT '恶鬼ID',
  `lv` smallint(6) unsigned NOT NULL COMMENT '恶鬼等级',
  `hp` int(11) unsigned NOT NULL COMMENT '血量',
  `cur_hp` int(11) unsigned NOT NULL COMMENT '当前血量',
  `is_help` tinyint(1) unsigned NOT NULL COMMENT '是否求助 1.求助 0.未求助',
  `is_kill` tinyint(1) NOT NULL COMMENT '是否干趴下 1.已趴 0.未趴',
  `is_award` tinyint(1) NOT NULL COMMENT '是否领奖 1.已领 0.未领',
  `create_ts` int(11) unsigned NOT NULL,
  `update_ts` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=53 DEFAULT CHARSET=utf8;
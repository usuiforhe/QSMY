ALTER TABLE `P_WONDER_BOSS_INFO`
CHANGE COLUMN `cur_hp` `formation`  blob NOT NULL COMMENT '当前信息' AFTER `hp`;


ALTER TABLE `P_USER_WONDER_BOSS`
CHANGE COLUMN `cause_hp_info` `cause_formation`  blob NOT NULL COMMENT '造成伤害明细' AFTER `cause_hp`;

ALTER TABLE `P_WONDER_BOSS_INFO`
ADD COLUMN `cur_hp`  int(11) UNSIGNED NOT NULL COMMENT '怪物当前血量' AFTER `hp`;
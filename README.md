All in one was wrote for one of my friend.
Now, I decided to release it.
Some plugins look like another plugin but recoded and some are new.

Some plugins have options type: !aio or !settings to see it.

/cfg/aio/"plugin"/config.cfg = contains all cvars for each plugins and are loaded
/sound/aio/"plugin"/.... = contains all sounds for each plugins
/addons/sourcemod/configs/aio/"plugin"/.. = contains settings for each plugins
/translations/aio.phrases.txt = All traductions contains FR and EN for sure.

[B][SIZE="4"]1 - AFK MANAGER[/SIZE][/B]

Will move player to spectator without losing frag or adding death.
Will kick player after time.
Admin can be immunised.

[PHP]active_afk_manager_csgo          		1
active_afk_manager_csgo_dev      		        0
active_afk_manager_immune_csgo   		2
active_afk_manager_immune_flag_csgo		""  
active_afk_manager_kick_min_csgo  		6
active_afk_manager_kick_time_csgo  		120.0
active_afk_manager_move_min_csgo  		0
active_afk_manager_move_time_csgo  		30.0
active_afk_manager_warn_time_csgo  		15.0
[/PHP]

[IMG]http://doyou.watch/images/aio/afk.jpg[/IMG]

[B][SIZE="4"]2 - BOT NAME[/SIZE][/B]
/addons/sourcemod/configs/aio/bot_names/settings.cfg = contains all names
You can choose Clan tag for Bot.
You can rename Bots.
You can simulate ping to Bots.


[PHP]active_bot_names_csgo          		1
active_bot_names_csgo_dev      			0

active_bot_names_clan_csgo   			AIO
active_bot_names_interval_ping_csgo  	        3
active_bot_names_max_ping_csgo  		55
active_bot_names_min_ping_csgo  		        35
[/PHP]

[IMG]http://doyou.watch/images/aio/bot.jpg[/IMG]

[B][SIZE="4"]3 - HIGH PING KICKER[/SIZE][/B]

Kick player with high ping.

[PHP]active_high_ping_kicker_csgo				1
active_high_ping_kicker_csgo_dev			0

active_high_ping_kicker_max_ping_csgo 		120
active_high_ping_kicker_max_checks_csgo 	10
active_high_ping_kicker_start_check_csgo	6.0
active_high_ping_kicker_admin_immune_csgo	1
[/PHP]

[IMG]http://doyou.watch/images/aio/ping.jpg[/IMG]

[B][SIZE="4"]4 - ONLY HS[/SIZE][/B]

You can only people with HS but you can use knife and grenades.

[PHP]active_only_hs_csgo				 0
active_only_hs_csgo_dev 		 0
[/PHP]

[B][SIZE="4"]5 - SHOW DAMAGE[/SIZE][/B]

Show damage when you hurt player. Body zone, health and armor.
You can disable it and save preference, type !aio

[PHP]active_show_damage_csgo 		 1
active_show_damage_csgo_dev 	 0
[/PHP]

[IMG]http://doyou.watch/images/aio/showdamage.jpg[/IMG]

[B][SIZE="4"]6 - SOUNDS KILL[/SIZE][/B]

It's like Quake sounds recoded and will add some features.
You can disable it, change sounds and save preference, type !aio

[PHP]active_sounds_kill_csgo 		 	1
active_sounds_kill_csgo_dev 		0

active_sounds_kill_team_kill_csgo 	1
active_sounds_kill_combo_time_csgo 	2.0
[/PHP]

[B][SIZE="4"]7 - START WEAPONS[/SIZE][/B]

Define weapons for both team when the round start.
You can delete buyzone and bombzone.
You can choose how many grenades. Ex He x 5, Flash x11...
You can choose primary, secondary, armor.... blank = no weapon

[PHP]active_start_weapon_csgo         			0
active_start_weapon_csgo_dev     			0

active_start_weapon_remove_buyzone      	0
active_start_weapon_remove_objectives   	0

active_start_weapon_csgo_ct_armor       	assaultsuit
active_start_weapon_csgo_ct_decoy       	1
active_start_weapon_csgo_ct_flash       	2
active_start_weapon_csgo_ct_gear        	defuser
active_start_weapon_csgo_ct_grenade     	1
active_start_weapon_csgo_ct_knife       	knife_butterfly
active_start_weapon_csgo_ct_molotov     	1
active_start_weapon_csgo_ct_primary     	m4a1_silencer
active_start_weapon_csgo_ct_secondary   	usp_silencer
active_start_weapon_csgo_ct_smoke       	1

active_start_weapon_csgo_terro_armor    	assaultsuit
active_start_weapon_csgo_terro_decoy    	1
active_start_weapon_csgo_terro_flash    	2
active_start_weapon_csgo_terro_gear     	c4
active_start_weapon_csgo_terro_grenade  	1
active_start_weapon_csgo_terro_knife    	knife_butterfly
active_start_weapon_csgo_terro_molotov  	1
active_start_weapon_csgo_terro_primary  	ak47
active_start_weapon_csgo_terro_secondary  	glock
active_start_weapon_csgo_terro_smoke    	1
[/PHP]

[B][SIZE="4"]8 - TRACK BOMB[/SIZE][/B]
/addons/sourcemod/configs/aio/track_bomb/settings.cfg = contains directions for defined maps.
Will show menu for Terro who bear the bomb.
Choose the site A or B or everything else write on the .cfg
After choosing the Site Bomb you can choose the way: middle, short..
Message wil be display to all Terrorists with sounds too.

When the bomb is, dropped, planted, bearer die.... with bomb message is show for all players with location: TSpawn, Middle, BombSiteA...

You can disable it and save preference, type !aio

[PHP]active_track_bomb_csgo           		1
active_track_bomb_csgo_dev       		0

active_track_bomb_alert_csgo     		1
active_track_bomb_c4_explode_csgo  		1
active_track_bomb_chat_csgo      		1	

active_track_bomb_delay_time_msg_csgo  	1.0
active_track_bomb_hint_csgo      		0
active_track_bomb_hit_num_csgo   		3
active_track_bomb_name_csgo      		0
active_track_bomb_sound_csgo     		1
[/PHP]

[IMG]http://doyou.watch/images/aio/trackbomb.jpg[/IMG]

[B][SIZE="4"]9 - WIN OR DIE[/SIZE][/B]

Simple, if you don't win the round, you will die but without loosing Frags but you will have one more death.

[PHP]active_wor_csgo                  1
active_wor_csgo_dev              0
[/PHP]

[IMG]http://doyou.watch/images/aio/winordie.jpg[/IMG]

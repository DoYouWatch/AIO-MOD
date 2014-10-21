/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//*****************************AIO MOD TRACK BOMB****************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_TRACK_BOMB_CSGO 				"[AIO TRACK BOMB] - "
#define MAX_MAP_TRACK_BOMB_CSGO				2
#define MAX_DIRECTION_TRACK_BOMB_CSGO		2 * MAX_MAP_TRACK_BOMB_CSGO	
#define MAX_WAY_TRACK_BOMB_CSGO				4 //* MAX_DIRECTION_TRACK_BOMB_CSGO * MAX_MAP_TRACK_BOMB_CSGO

//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_track_bomb_csgo;
new Handle:cvar_active_track_bomb_csgo_dev;
new Handle:cvar_active_track_bomb_c4_explode_csgo;
new Handle:cvar_active_track_bomb_menu_name_csgo;
//new Handle:cvar_active_track_bomb_menu_time_csgo;
new Handle:cvar_active_track_bomb_menu_chat_csgo;
new Handle:cvar_active_track_bomb_menu_hint_csgo;
new Handle:cvar_active_track_bomb_menu_alert_csgo;
new Handle:cvar_active_track_bomb_delay_time_msg_csgo;
new Handle:cvar_active_track_bomb_hit_num_csgo;

new Handle:mp_warmuptime;
new Handle:mp_roundtime;
new Handle:mp_freezetime;
//new Handle:mp_c4timer;

new Handle:H_timer_beacon_entity_csgo;

//Bool
new bool:B_active_track_bomb_csgo 					= false;
new bool:B_active_track_bomb_csgo_dev				= false;
new bool:B_active_track_bomb_c4_explode_csgo		= false;
new bool:B_active_track_bomb_menu_name_csgo			= false;
new bool:B_active_track_bomb_menu_chat_csgo			= false;
new bool:B_active_track_bomb_menu_hint_csgo			= false;
new bool:B_active_track_bomb_menu_alert_csgo		= false;

new bool:B_track_bomb_show_menu						= false;
new bool:B_the_round_is_warm_up						= false;
new bool:B_is_round_start							= false;

//String
new String:S_track_bomb_mapnames[MAX_MAP_TRACK_BOMB_CSGO][64];
new String:S_track_bomb_direction[MAX_MAP_TRACK_BOMB_CSGO][MAX_DIRECTION_TRACK_BOMB_CSGO][64];
new String:S_track_bomb_way[MAX_MAP_TRACK_BOMB_CSGO][MAX_DIRECTION_TRACK_BOMB_CSGO][MAX_WAY_TRACK_BOMB_CSGO][64];
new String:S_track_bomb_way_sound[MAX_MAP_TRACK_BOMB_CSGO][MAX_DIRECTION_TRACK_BOMB_CSGO][MAX_WAY_TRACK_BOMB_CSGO][PLATFORM_MAX_PATH];
new String:S_active_track_bomb_menu_direction[64];

new static String:M_LASERBEAM[] 				= "materials/sprites/laserbeam.vmt";
new static String:M_HALO01[] 					= "materials/sprites/halo01.vmt";
new static String:SND_BEEP[] 					= "buttons/blip1.wav";
new static String:SND_EXPLODE[] 				= "ambient/explosions/explode_8.wav";

//Float
new Float:F_active_track_bomb_delay_time_msg_csgo;

//Customs/Others
new C_has_bomb[MAXPLAYERS+1]						= false;
//new C_active_track_bomb_menu_time_csgo;
new C_progress_bar_start_time;
new C_progress_bar_duration;
new C_active_track_bomb_hit_num_csgo				= 0;
new C_hit_num										= 0;
new M_TACK_BOMB_DECALS_BLOOD[13];

/*
new ColorDefault[] 								= {255,255,255,255};
new ColorAqua[] 								= {0,255,255,255};
new ColorBlack[]								= {1,1,1,255};
new ColorBlue[] 								= {0,0,255,255};
new ColorFuschia[] 								= {255,0,255,255};
new ColorGray[] 								= {128,128,128,255};
new ColorGreen[] 								= {0,128,0,255};
new ColorLime[] 								= {0,255,0,255};
new ColorMaroon[] 								= {128,0,0,255};
new ColorNavy[] 								= {0,0,128,255};
*/
new ColorRed[] 									= {255,0,0,255};
/*
new ColorWhite[] 								= {255,255,255,255};
new ColorYellow[]								= {255,255,0,255};
new ColorSilver[]								= {192,192,192,255};
new ColorTeal[]									= {0,128,128,255};
new ColorPurple[]								= {128,0,128,255};
new ColorOlive[]								= {128,128,0,255};
*/

new M_LASERBEAM_PRECACHED;
new M_HALO01_PRECACHED;

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartTrackBombCsgo()
{
	LoadLocationsBombCsgo();
	CreateConVar("aio_track_bomb_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_track_bomb_csgo 					= CreateConVar("active_track_bomb_csgo",  					"0", 					"Enable/Disable Track the bomb", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_track_bomb_csgo_dev					= CreateConVar("active_track_bomb_csgo_dev", 				"0", 					"Enable/Disable Track the bomb Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	cvar_active_track_bomb_c4_explode_csgo 			= CreateConVar("active_track_bomb_c4_explode_csgo",  		"0", 					"Enable/Disable Explode bomb on holder", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_track_bomb_menu_name_csgo			= CreateConVar("active_track_bomb_name_csgo", 				"0", 					"Enable/Disable Name of holder in chat", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_track_bomb_menu_chat_csgo			= CreateConVar("active_track_bomb_chat_csgo", 				"1", 					"Enable/Disable Chat message", 				DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_track_bomb_menu_hint_csgo			= CreateConVar("active_track_bomb_hint_csgo", 				"0", 					"Enable/Disable Hint message", 				DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_track_bomb_menu_alert_csgo			= CreateConVar("active_track_bomb_alert_csgo", 				"1", 					"Enable/Disable Alert message", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	//cvar_active_track_bomb_menu_time_csgo			= CreateConVar("active_track_bomb_menu_time_csgo", 			"15.0", 				"Menu Track Bomb Time", 					DEFAULT_FLAGS);
	cvar_active_track_bomb_delay_time_msg_csgo		= CreateConVar("active_track_bomb_delay_time_msg_csgo", 	"1.0", 					"Delay time for Chat/Hint/Alert Message", 	DEFAULT_FLAGS);
	cvar_active_track_bomb_hit_num_csgo				= CreateConVar("active_track_bomb_hit_num_csgo", 			"3", 					"Number of hit for explode bomb", 			DEFAULT_FLAGS);
	
	mp_warmuptime 									= FindConVar("mp_warmuptime");
	mp_roundtime 									= FindConVar("mp_roundtime");
	mp_freezetime									= FindConVar("mp_freezetime");
	//mp_c4timer									= FindConVar("mp_c4timer");
	
	M_LASERBEAM_PRECACHED 							= PrecacheModel(M_LASERBEAM);
	M_HALO01_PRECACHED 								= PrecacheModel(M_HALO01);
	PrecacheSound(SND_BEEP, true);
	PrecacheSound(SND_EXPLODE, true);
	PrecacheDecalsBloodTrackBomb();
	
	C_progress_bar_start_time = FindSendPropOffs("CCSPlayer", "m_flProgressBarStartTime");
	if(C_progress_bar_start_time == -1)
	{
		SetFailState("Couldnt find the m_flProgressBarStartTime offset!");
	}
	
	C_progress_bar_duration = FindSendPropOffs("CCSPlayer", "m_iProgressBarDuration");
	if(C_progress_bar_duration == -1)
	{
		SetFailState("Couldnt find the m_iProgressBarDuration offset!");
	}
	
	for(new i = 1; i <= MaxClients; i++)
	{
		if(IsClientValid(i))
		{
			SDKHook(i, SDKHook_TraceAttack, OnTracekAttackTrackBombCsgo);
		}
	}	
	
	HookEventsTrackBombCsgo();
	
	RegAdminCmd("sm_trackbomb",		Command_TrackBombMenuCsgo, 		ADMFLAG_CHANGEMAP, 		"Display the track bomb menu.");
	
	ServerCommand("exec /aio/track_bomb/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsTrackBombCsgo()
{
	HookConVarChange(cvar_active_track_bomb_csgo, 					Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_csgo_dev, 				Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_c4_explode_csgo, 		Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_menu_chat_csgo, 		Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_menu_hint_csgo, 		Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_menu_alert_csgo, 		Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_menu_name_csgo, 		Event_CvarChangeTrackBombCsgo);
	//HookConVarChange(cvar_active_track_bomb_menu_time_csgo, 		Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_delay_time_msg_csgo, 	Event_CvarChangeTrackBombCsgo);
	HookConVarChange(cvar_active_track_bomb_hit_num_csgo, 			Event_CvarChangeTrackBombCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeTrackBombCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateTrackBombCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateTrackBombCsgo()
{
	B_active_track_bomb_csgo 					= GetConVarBool(cvar_active_track_bomb_csgo);
	B_active_track_bomb_csgo_dev 				= GetConVarBool(cvar_active_track_bomb_csgo_dev);
	B_active_track_bomb_c4_explode_csgo 		= GetConVarBool(cvar_active_track_bomb_c4_explode_csgo);
	B_active_track_bomb_menu_name_csgo 			= GetConVarBool(cvar_active_track_bomb_menu_name_csgo);
	B_active_track_bomb_menu_chat_csgo 			= GetConVarBool(cvar_active_track_bomb_menu_chat_csgo);
	B_active_track_bomb_menu_hint_csgo 			= GetConVarBool(cvar_active_track_bomb_menu_hint_csgo);
	B_active_track_bomb_menu_alert_csgo 		= GetConVarBool(cvar_active_track_bomb_menu_alert_csgo);
	//C_active_track_bomb_menu_time_csgo 		= GetConVarInt(cvar_active_track_bomb_menu_time_csgo);
	F_active_track_bomb_delay_time_msg_csgo 	= GetConVarFloat(cvar_active_track_bomb_delay_time_msg_csgo);
	C_active_track_bomb_hit_num_csgo 			= GetConVarInt(cvar_active_track_bomb_hit_num_csgo);
	
	LoadLocationsBombCsgo();

	M_LASERBEAM_PRECACHED 							= PrecacheModel(M_LASERBEAM);
	M_HALO01_PRECACHED 								= PrecacheModel(M_HALO01);
	PrecacheSound(SND_BEEP, true);
	PrecacheSound(SND_EXPLODE, true);
	PrecacheDecalsBloodTrackBomb();
	
	if(B_active_track_bomb_menu_hint_csgo && B_active_track_bomb_menu_alert_csgo)
	{
		SetFailState("%sYou need to choose between: active_track_bomb_hint_csgo or active_track_bomb_alert_csgo", TAG_TRACK_BOMB_CSGO);
	}
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedTrackBombCsgo()
{
	//UpdateStateTrackBomb();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartTrackBombCsgo()
{
	UpdateStateTrackBombCsgo();
}

/***********************************************************/
/********************** WHEN MAP END ***********************/
/***********************************************************/
public OnMapEndTrackBombCsgo()
{
	if(H_timer_beacon_entity_csgo != INVALID_HANDLE)
	{
		KillTimer(H_timer_beacon_entity_csgo);
		H_timer_beacon_entity_csgo = INVALID_HANDLE;
	}
}

/***********************************************************/
/******************** WHEN ROUND START *********************/
/***********************************************************/
public Action:Event_RoundStartTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{
		decl String:S_time[192];
		decl String:S_warm_up_time[192];
		decl String:S_round_time[192];
		GetEventString(event, "timelimit", S_time, sizeof(S_time));
		
		C_hit_num = 0;
		
		IntToString(GetConVarInt(mp_warmuptime), S_warm_up_time, sizeof(S_warm_up_time));
		IntToString(GetConVarInt(mp_roundtime)*60, S_round_time, sizeof(S_round_time));
		
		if( !StrEqual(S_time, S_round_time) )
		{	
			B_the_round_is_warm_up = true;
		}
		else
		{
			B_the_round_is_warm_up = false;
		}
		
		if(!B_the_round_is_warm_up)
		{
			B_track_bomb_show_menu = true;
			for(new c=1;c<MaxClients;++c)
			{
				if(IsClientValid(c))
				{
					new Teams:clientTeam = Teams:GetClientTeam(c);
					if(clientTeam == TeamT)
					{
						C_has_bomb[c] = false;
						new c4 = GetPlayerWeaponSlot(c, _:SlotC4);
						if(c4 != -1)
						{
							C_has_bomb[c] = true;
							if (Client_HasWeapon(c, "weapon_c4"))
							{
								BuildTrackBombMenuCsgo(c);
							}
						}
					}
				}
			}
		}
	}
}

/***********************************************************/
/******************* WHEN ROUND PRESTART *******************/
/***********************************************************/
public Action:Event_RoundPreStartTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{
		B_is_round_start = true;
		if(H_timer_beacon_entity_csgo != INVALID_HANDLE)
		{
			KillTimer(H_timer_beacon_entity_csgo);
			H_timer_beacon_entity_csgo = INVALID_HANDLE;
		}
	}
}

/***********************************************************/
/********************* WHEN FREEZE END *********************/
/***********************************************************/
public Action:Event_RoundFreezeEndTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{
		B_is_round_start = false;
	}
}

/***********************************************************/
/****************** WHEN PLAYER DROP BOMB ******************/
/***********************************************************/
public Action:Event_BombDroppedTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		new Teams:clientTeam = Teams:GetClientTeam(client);
		
		if(IsClientValid(client) && clientTeam == TeamT)
		{
			new c4 = GetPlayerWeaponSlot(client, _:SlotC4);
			if(c4 != -1)
			{
				new Handle:kv = CreateKeyValues("bomb");
				KvSetNum(kv, "c4", c4);		
				H_timer_beacon_entity_csgo = CreateTimer(2.0, Timer_Beacon_EntityCsgo, kv, TIMER_REPEAT);
			}
			
			new health = Entity_GetHealth(client);
			if(health > 0)
			{
				C_has_bomb[client] = false;
				CancelClientMenu(client);
				
				CreateTimer(F_active_track_bomb_delay_time_msg_csgo, DisplayMessageForTerroBombDroppedCsgo, client);
			}
			else if(health <= 0)
			{
				C_has_bomb[client] = false;
				B_track_bomb_show_menu = false;
				CancelClientMenu(client);
				
				//We need time 0.0 because location will be nothing if timer is delayed
				CreateTimer(0.0, DisplayMessageForTerroBombDroppedAndPlayerDeadCsgo, client);		
			}
		}
	}
}

/***********************************************************/
/****************** WHEN PLAYER PICKUP BOMB ****************/
/***********************************************************/
public Action:Event_BombPickupTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		new Teams:clientTeam = Teams:GetClientTeam(client);
		
		if(IsClientValid(client) && clientTeam == TeamT)
		{
			C_has_bomb[client] = true;
			if(B_track_bomb_show_menu)
			{
				BuildTrackBombMenuCsgo(client);
			}
			if(!B_is_round_start)
			{
				CreateTimer(F_active_track_bomb_delay_time_msg_csgo, DisplayMessageForTerroBombPickupCsgo, client);
			}
			if(H_timer_beacon_entity_csgo != INVALID_HANDLE)
			{
				KillTimer(H_timer_beacon_entity_csgo);
				H_timer_beacon_entity_csgo = INVALID_HANDLE;
			}
		}	
	}
}

/***********************************************************/
/****************** WHEN PLAYER ABORT BOMB *****************/
/***********************************************************/
public Action:Event_BombAbortPlantTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{	
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		new Teams:clientTeam = Teams:GetClientTeam(client);
		
		if(IsClientValid(client) && clientTeam == TeamT)
		{
			CreateTimer(F_active_track_bomb_delay_time_msg_csgo, DisplayMessageForTerroBombAbortedCsgo, client);	
		}
	}
}

/***********************************************************/
/****************** WHEN PLAYER PLANT BOMB *****************/
/***********************************************************/
public Action:Event_BombPlantedTrackBombCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_track_bomb_csgo)
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		new Teams:clientTeam = Teams:GetClientTeam(client);
		
		if(IsClientValid(client) && clientTeam == TeamT)
		{
			C_has_bomb[client] = false;
			CancelClientMenu(client);
			
			//Bomb explode after 2s
			//new Float:F_time_remaining;
			//F_time_remaining = GetConVarFloat(mp_c4timer) - 10.0;
			
			//Progress bar doesn't fucking work on CSGO
			//CreateTimer(F_time_remaining, ShowProgressBarBombCsgo);
			
			CreateTimer(F_active_track_bomb_delay_time_msg_csgo, DisplayMessageForTerroBombPlantedCsgo, client);
		}	
	}
}

/***********************************************************/
/********************* WHEN PLAYER HURT ********************/
/***********************************************************/
/* BACK = hitbox = 8 + hitgroup = 2 */
public Action:OnTracekAttackTrackBombCsgo(victim, &attacker, &inflictor, &Float:damage, &damagetype, &ammotype, hitbox, hitgroup)
{
	if(B_active_track_bomb_csgo)
	{
		if(B_active_track_bomb_c4_explode_csgo)
		{
			if(IsClientValid(victim) && IsClientValid(attacker))
			{
				new c4 = GetPlayerWeaponSlot(victim, _:SlotC4);
				if(c4 != -1)
				{
					if(hitbox == 8 && damagetype == DMG_SHOT_CSGO && hitgroup == 2)
					{
						new Teams:victimTeam = Teams:GetClientTeam(victim);
						new Teams:attackerTeam = Teams:GetClientTeam(attacker);

						if(victimTeam != attackerTeam)
						{
							C_hit_num += 1;
							damage = 0.0;
							
							new Handle:kv = CreateKeyValues("bomb");
							KvSetNum(kv, "hit", C_hit_num);	
							KvSetNum(kv, "total_hit", C_active_track_bomb_hit_num_csgo);	
							KvSetNum(kv, "victim", victim);
							KvSetNum(kv, "attacker", attacker);
							
							if(C_hit_num == C_active_track_bomb_hit_num_csgo)
							{	
								ExplodeCsgo(victim, attacker, c4);
								CreateTimer(F_active_track_bomb_delay_time_msg_csgo, DisplayMessageHolderExplodeCsgo, kv);	
								
								C_hit_num = 0;
							}
							else if(C_hit_num != 0 && C_hit_num < C_active_track_bomb_hit_num_csgo)
							{
								CreateTimer(F_active_track_bomb_delay_time_msg_csgo, DisplayMessageWarningBomb, kv);
							}
						}
						return Plugin_Changed;
					}
				}
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/********************** PLAYER EXPLODE *********************/
/***********************************************************/
ExplodeCsgo(victim, attacker, c4)
{
	decl Float:pos[3];
	GetClientAbsOrigin(victim, pos);
	new victim_death = GetEntProp(victim, Prop_Data, "m_iDeaths");
	new vicitm_frags = GetEntProp(victim, Prop_Data, "m_iFrags");
	new attacker_frags = GetEntProp(attacker, Prop_Data, "m_iFrags");
	
	new explosion = CreateEntityByName("env_explosion");
	
	if (explosion != -1)
	{
		SetEntityHealth(victim, 1);
		
		// Stuff we will need
		decl Float:vector[3];
		new damage = 500;
		new radius = 128;
		new team = GetEntProp(victim, Prop_Send, "m_iTeamNum");
					
		// We're going to use eye level because the blast can be clipped by almost anything.
		// This way there's no chance that a small street curb will clip the blast.
		GetClientEyePosition(victim, vector);
					
			
		SetEntProp(explosion, Prop_Send, "m_iTeamNum", team);
		SetEntProp(explosion, Prop_Data, "m_spawnflags", 264);
		SetEntProp(explosion, Prop_Data, "m_iMagnitude", damage);
		SetEntProp(explosion, Prop_Data, "m_iRadiusOverride", radius);
		
		DispatchKeyValue(explosion, "rendermode", "5");
					
		DispatchSpawn(explosion);
		ActivateEntity(explosion);

		TE_SetupExplosion(pos, M_HALO01_PRECACHED, 5.0, 1, 0, 100, 1500);
		TE_Start("World Decal");
		TE_WriteVector("m_vecOrigin", pos);
		new random = GetRandomInt(0, 12);
		TE_WriteNum("m_nIndex", M_TACK_BOMB_DECALS_BLOOD[random]);
	
		TE_SendToAll();
		
		RemovePlayerItem(victim, c4);
		AcceptEntityInput(c4, "Kill");
		
		TeleportEntity(explosion, vector, NULL_VECTOR, NULL_VECTOR);
		EmitSoundToAll("weapons/hegrenade/explode3.wav", explosion, 1, 90);		
		AcceptEntityInput(explosion, "Explode");

		EmitSoundToAll("weapons/c4/c4_explode1.wav", explosion, 1, 90);
		
		SetEntProp(victim, Prop_Data, "m_iFrags", vicitm_frags);
		SetEntProp(victim, Prop_Data, "m_iDeaths", victim_death + 1);
		SetEntProp(attacker, Prop_Data, "m_iFrags", attacker_frags + 1);
	}
}

/***********************************************************/
/****************** PRECACHE BLOOD DECALS ******************/
/***********************************************************/
PrecacheDecalsBloodTrackBomb()
{
	M_TACK_BOMB_DECALS_BLOOD[0] = PrecacheDecal("decals/blood_splatter.vtf");
	M_TACK_BOMB_DECALS_BLOOD[1] = PrecacheDecal("decals/bloodstain_003.vtf");
	M_TACK_BOMB_DECALS_BLOOD[2] = PrecacheDecal("decals/bloodstain_101.vtf");
	M_TACK_BOMB_DECALS_BLOOD[3] = PrecacheDecal("decals/bloodstain_002.vtf");
	M_TACK_BOMB_DECALS_BLOOD[4] = PrecacheDecal("decals/bloodstain_001.vtf");
	M_TACK_BOMB_DECALS_BLOOD[5] = PrecacheDecal("decals/blood8.vtf");
	M_TACK_BOMB_DECALS_BLOOD[6] = PrecacheDecal("decals/blood7.vtf");
	M_TACK_BOMB_DECALS_BLOOD[7] = PrecacheDecal("decals/blood6.vtf");
	M_TACK_BOMB_DECALS_BLOOD[8] = PrecacheDecal("decals/blood5.vtf");
	M_TACK_BOMB_DECALS_BLOOD[9] = PrecacheDecal("decals/blood4.vtf");
	M_TACK_BOMB_DECALS_BLOOD[10] = PrecacheDecal("decals/blood3.vtf");
	M_TACK_BOMB_DECALS_BLOOD[11] = PrecacheDecal("decals/blood2.vtf");
	M_TACK_BOMB_DECALS_BLOOD[12] = PrecacheDecal("decals/blood1.vtf");
}

/***********************************************************/
/******************** GET CHAT LOCATION ********************/
/***********************************************************/
bool:GetChatLocationCsgo(client, String:szBuffer[], size)
{
    if (!IsClientInGame(client) || !IsPlayerAlive(client))
        return false;

    new team = GetClientTeam(client);
    if (team != CS_TEAM_T && team != CS_TEAM_CT)
        return false;

    GetEntPropString(client, Prop_Send, "m_szLastPlaceName", szBuffer, size);
    return true;

}

/***********************************************************/
/************* DISPLAY BEACON AROUND THE BOMB **************/
/***********************************************************/
public Action:Timer_Beacon_EntityCsgo(Handle:timer, any:data)
{
	new Handle:kv = Handle:data;
	new entity = KvGetNum(kv, "c4", -1);

	if(!IsValidEntity(entity))
	{
		return Plugin_Stop;
	}
	
	new Float:position[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", position);
	TE_SetupBeamRingPoint(position, 10.0, 375.0, M_LASERBEAM_PRECACHED, M_HALO01_PRECACHED, 0, 10, 0.2, 1.0, 0.5, ColorRed, 10, 0);
	

	TE_SendToAllTerroCsgo();
	
	for (new i=1; i<=MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			new Teams:clientTeam = Teams:GetClientTeam(i);
			if(clientTeam == TeamT)//&& client != c
			{	
				//EmitAmbientSound(SND_BEEP, position, entity, SNDLEVEL_NORMAL);
				EmitSoundToClient(i, SND_BEEP, entity, _, _, _, 1.0, _, _, position);
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/********* SEND BEACON AROUND THE BOMB FOR TERRO ***********/
/***********************************************************/
stock TE_SendToAllTerroCsgo(Float:delay=0.0)
{
	new total = 0;
	new clients[MaxClients];
	for (new i=1; i<=MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			new Teams:clientTeam = Teams:GetClientTeam(i);
			if(clientTeam == TeamT)//&& client != c
			{
				clients[total++] = i;
			}
		}
	}
	return TE_Send(clients, total, delay);
}

/***********************************************************/
/************* SHOW PROGRESS BAR EXPLODE BOMB **************/
/***********************************************************/
/* Doesn't work on CSGO lot of bugs */
public Action:ShowProgressBarBombCsgo(Handle:timer)
{
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)//&& client != c
			{
				SetEntDataFloat(c, C_progress_bar_start_time, 0.0, true); 
				SetEntData(c, C_progress_bar_duration, 0, 4, true);
				
				if(GetEntProp(c, Prop_Send, "m_iProgressBarDuration") == 0)
				{
					new C_time = GetTime() + 10;
					SetEntDataFloat(c, C_progress_bar_start_time, GetGameTime(), true); 
					SetEntData(c, C_progress_bar_duration, C_time, 4, true);
					PrintToChat(c, "Bar start:%i, duration:%i", GetGameTime(), C_time);
				}
			}
		}
	}
}

/***********************************************************/
/********* DISPLAY MSG FOR TERRO WHEN BOMB PICKUP **********/
/***********************************************************/
public Action:DisplayMessageForTerroBombPickupCsgo(Handle:timer, any:client)
{
	decl String:message[128];
	GetChatLocationCsgo(client, message, sizeof(message));
	
	if(StrEqual(message, "", false)) return Plugin_Handled;
	
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)//&& client != c
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb pickup chat", message);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb pickup hint", message);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb pickup alert", message);
				}
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/********* DISPLAY MSG FOR TERRO WHEN BOMB PLANTED *********/
/***********************************************************/
public Action:DisplayMessageForTerroBombPlantedCsgo(Handle:timer, any:client)
{
	decl String:message[128];
	GetChatLocationCsgo(client, message, sizeof(message));
	
	if(StrEqual(message, "", false)) return Plugin_Handled;
	
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)//&& client != c
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb planted chat", message);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb planted hint", message);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb planted alert", message);
				}
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/********* DISPLAY MSG FOR TERRO WHEN BOMB ABORTED *********/
/***********************************************************/
public Action:DisplayMessageForTerroBombAbortedCsgo(Handle:timer, any:client)
{
	decl String:message[128];
	GetChatLocationCsgo(client, message, sizeof(message));
	
	if(StrEqual(message, "", false)) return Plugin_Handled;
	
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)//&& client != c
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb abort chat", message);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb abort hint", message);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb abort alert", message);
				}
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/********* DISPLAY MSG FOR TERRO WHEN BOMB DROPPED *********/
/***********************************************************/
public Action:DisplayMessageForTerroBombDroppedCsgo(Handle:timer, any:client)
{
	decl String:message[128];
	GetChatLocationCsgo(client, message, sizeof(message));
	
	if(StrEqual(message, "", false)) return Plugin_Handled;
	
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)//&& client != c
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb dropped chat", message);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb dropped hint", message);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb dropped alert", message);
				}
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/* DISPLAY MSG FOR TERRO WHEN BOMB DROPPED AND HOLDER DIE **/
/***********************************************************/
public Action:DisplayMessageForTerroBombDroppedAndPlayerDeadCsgo(Handle:timer, any:client)
{
	decl String:message[128];
	GetChatLocationCsgo(client, message, sizeof(message));
	
	if(StrEqual(message, "", false)) return Plugin_Handled;
	
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)//&& client != c
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb player die chat", message);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb player die hint", message);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb player die alert", message);
				}
			}
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/******** DISPLAY MSG FOR TERRO WHEN OBJECTIVE SET *********/
/***********************************************************/
public Action:DisplayMessageForTerroObjectiveCsgo(Handle:timer, any:data)
{							
	decl String:direction[64], String:way[64], String:sound[PLATFORM_MAX_PATH];
	new Handle:kv = Handle:data;
	new client = KvGetNum(kv, "client", -1);
	KvGetString(kv, "direction", direction, sizeof(direction), "NULL");
	KvGetString(kv, "way", way, sizeof(way), "NULL");
	KvGetString(kv, "sound", sound, sizeof(sound), "NULL");
	
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{	
			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					if(B_active_track_bomb_menu_name_csgo)
					{
						decl String:holder[64];
						GetClientName(client, holder, sizeof(holder));
						
						CPrintToChat(c, "%t", "Track Bomb direction and way chat holder", holder);
					}
					CPrintToChat(c, "%t", "Track Bomb direction and way chat direction", direction);
					CPrintToChat(c, "%t", "Track Bomb direction and way chat way", way);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track Bomb direction and way hint", direction, way);
				}
				else
				{
					PrintCenterText(c, "%t", "Track Bomb direction and way alert", direction, way);
				}
				if(strlen(sound) && C_sounds_of_track_bomb_csgo[c])
				{
					decl String:soundFile[PLATFORM_MAX_PATH];
					Format(soundFile, sizeof(soundFile), "*%s", sound);

					//ClientCommand(c, "play %s", soundFile);
					EmitSoundToClient(c, soundFile,_,_,_,_,1.0);
					//PrintToChat(c, "Sound: %s", soundFile);
				}
			}
		}
	}
}

/***********************************************************/
/******* DISPLAY MSG WHEN BOMB TOUCHED AND EXPLODED ********/
/***********************************************************/
public Action:DisplayMessageHolderExplodeCsgo(Handle:timer, any:data)
{
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{
			new Handle:kv = Handle:data;
			new victim 		= KvGetNum(kv, "victim", -1);
			new attacker 	= KvGetNum(kv, "attacker", -1);
			new hit			= KvGetNum(kv, "hit", -1);
			new total_hit	= KvGetNum(kv, "total_hit", -1);
			
			decl String:victim_name[64], String:attacker_name[64];
			GetClientName(victim, victim_name, sizeof(victim_name));
			GetClientName(attacker, attacker_name, sizeof(attacker_name));


	
			if(B_active_track_bomb_menu_chat_csgo)
			{
				CPrintToChat(c, "%t", "Track Bomb holder explode chat", attacker_name, victim_name, hit, total_hit);
			}
			
			if(B_active_track_bomb_menu_hint_csgo)
			{
				PrintHintText(c, "%t", "Track Bomb holder explode hint ", attacker_name, victim_name, hit, total_hit);
			}
			else
			{
				PrintCenterText(c, "%t", "Track Bomb holder explode alert", attacker_name, victim_name, hit, total_hit);
			}
		}
	}
}

/***********************************************************/
/************* DISPLAY MSG WHEN BOMB TOUCHED ***************/
/***********************************************************/
public Action:DisplayMessageWarningBomb(Handle:timer, any:data)
{
	for(new c=1;c<MaxClients;++c)
	{
		if(IsClientValid(c))
		{
			new Handle:kv = Handle:data;
			new hit			= KvGetNum(kv, "hit", -1);
			new total_hit	= KvGetNum(kv, "total_hit", -1);
			

			new Teams:clientTeam = Teams:GetClientTeam(c);
			if(clientTeam == TeamT)
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb hit terro chat", hit, total_hit);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb hit terro hint", hit, total_hit);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb hit terro alert", hit, total_hit);
				}
			}
			else if(clientTeam == TeamCT)
			{
				if(B_active_track_bomb_menu_chat_csgo)
				{
					CPrintToChat(c, "%t", "Track bomb bomb hit ct chat", hit, total_hit);
				}
				
				if(B_active_track_bomb_menu_hint_csgo)
				{
					PrintHintText(c, "%t", "Track bomb bomb hit ct hint", hit, total_hit);
				}
				else
				{
					PrintCenterText(c, "%t", "Track bomb bomb hit ct alert", hit, total_hit);
				}
			}
		}
	}
}
/***********************************************************/
/******************* COMMANDE TRACK BOMB *******************/
/***********************************************************/
public Action:Command_TrackBombMenuCsgo(client, args) 
{
	BuildTrackBombMenuCsgo(client);
	return Plugin_Handled;
}

/***********************************************************/
/****************** BUILD TRACK BOMB MENU ******************/
/***********************************************************/
BuildTrackBombMenuCsgo(client)
{
	decl String:title[40], String:direction[40];
	new Handle:menu = CreateMenu(TrackBombCsgo_Menu);
	
	decl String:Mapname[64];
	GetCurrentMap(Mapname, sizeof(Mapname));

	new c4 = GetPlayerWeaponSlot(client, _:SlotC4);
	if(c4 != -1)
	{	
		for(new p=0;p<MAX_MAP_TRACK_BOMB_CSGO;++p)
		{
			if(strlen(S_track_bomb_mapnames[p]) && StrEqual(Mapname, S_track_bomb_mapnames[p], false))
			{
				for(new i=0;i<MAX_DIRECTION_TRACK_BOMB_CSGO;++i)
				{	
					if(strlen(S_track_bomb_direction[p][i]))
					{
						Format(direction, sizeof(direction), "%T", "Track Bomb direction menu", client, S_track_bomb_direction[p][i]);
						AddMenuItem(menu, S_track_bomb_direction[p][i], direction);
					}
				}
			}
		}
		
		Format(title, sizeof(title), "%T", "Track Bomb site bomb menu title", client);
		SetMenuTitle(menu, title);
		DisplayMenu(menu, client, MENU_TIME_FOREVER);
	}
}

/***********************************************************/
/**************** BUILD TRACK BOMB SUB MENU ****************/
/***********************************************************/
public TrackBombCsgo_Menu(Handle:menu, MenuAction:action, param1, param2)
{
	if(IsClientValid(param1))
	{
		new c4 = GetPlayerWeaponSlot(param1, _:SlotC4);
		if(c4 != -1)
		{	
			switch(action)
			{
				case MenuAction_End:
				{
					CloseHandle(menu);	
				}
				case MenuAction_Select:
				{
					decl String:title[40];
					new Handle:waymenu = CreateMenu(TrackBombWayCsgo_Menu);
					
					new String:menu1[64];
					decl String:Mapname[64];
					
					GetMenuItem(menu, param2, menu1, sizeof(menu1));
					SetMenuExitBackButton(waymenu, true);
					GetCurrentMap(Mapname, sizeof(Mapname));
					
					for(new p=0;p<MAX_MAP_TRACK_BOMB_CSGO;++p)
					{
						if(strlen(S_track_bomb_mapnames[p]) && StrEqual(Mapname, S_track_bomb_mapnames[p], false))
						{
							for(new i=0;i<MAX_DIRECTION_TRACK_BOMB_CSGO;++i)
							{	
								if(strlen(S_track_bomb_direction[p][i]) && StrEqual(menu1, S_track_bomb_direction[p][i], false))
								{
									S_active_track_bomb_menu_direction = S_track_bomb_direction[p][i];
									for(new w=0;w<MAX_WAY_TRACK_BOMB_CSGO;++w)
									{
										if(strlen(S_track_bomb_way[p][i][w]))
										{
											decl String:way[64];
											
											Format(way, sizeof(way), "%T", "Track Bomb way menu", param1, S_track_bomb_way[p][i][w]);
											AddMenuItem(waymenu, S_track_bomb_way[p][i][w], way);
										}
									}
								}
							}
						}
					}

					Format(title, sizeof(title), "%T", "Track Bomb way menu title", param1);
					SetMenuTitle(waymenu, title);
					DisplayMenu(waymenu, param1, MENU_TIME_FOREVER);
				}
			}
		}
	}
}

/***********************************************************/
/************* DISPLAY MSG OBJECTIVE BY CHOICE *************/
/***********************************************************/
public TrackBombWayCsgo_Menu(Handle:menu, MenuAction:action, param1, param2)
{
	if(IsClientValid(param1))
	{
		new c4 = GetPlayerWeaponSlot(param1, _:SlotC4);
		if(c4 != -1)
		{	
			switch(action)
			{
				case MenuAction_End:
				{
					CloseHandle(menu);	
				}
				case MenuAction_Cancel:
				{
					if (param2 == MenuCancel_ExitBack)
					{	
						BuildTrackBombMenuCsgo(param1);
					}
				}
				case MenuAction_Select:
				{
					new String:menu1[64];
					decl String:Mapname[64];
					SetMenuExitBackButton(menu, true);
					
					GetMenuItem(menu, param2, menu1, sizeof(menu1));
					GetCurrentMap(Mapname, sizeof(Mapname));
					
					for(new p=0;p<MAX_MAP_TRACK_BOMB_CSGO;++p)
					{
						if(strlen(S_track_bomb_mapnames[p]) && StrEqual(Mapname, S_track_bomb_mapnames[p], false))
						{
							for(new i=0;i<MAX_DIRECTION_TRACK_BOMB_CSGO;++i)
							{	
								if(strlen(S_track_bomb_direction[p][i]) && StrEqual(S_active_track_bomb_menu_direction, S_track_bomb_direction[p][i], false))
								{
									for(new w=0;w<MAX_WAY_TRACK_BOMB_CSGO;++w)
									{
										if(strlen(S_track_bomb_way[p][i][w]) && StrEqual(menu1, S_track_bomb_way[p][i][w], false))
										{
											new Handle:kv = CreateKeyValues("objective");
											KvSetNum(kv, "client", param1);
											KvSetString(kv, "direction", S_track_bomb_direction[p][i]);
											KvSetString(kv, "way", S_track_bomb_way[p][i][w]);
											KvSetString(kv, "sound", S_track_bomb_way_sound[p][i][w]);
											
											new Float:F_time_show_menu;
											new Float:F_freezetime;
											
											F_freezetime = GetConVarFloat(mp_freezetime);
											if(B_is_round_start)
											{
												if(F_freezetime >= 4)
												{
													F_time_show_menu = F_freezetime;
												}
												else
												{
													F_time_show_menu = 4.0;
												}	
											}
											else
											{
												F_time_show_menu = 0.0;
											}
											
											CreateTimer(F_time_show_menu, DisplayMessageForTerroObjectiveCsgo, kv); 
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

/***********************************************************/
/****************** LOAD FILE SETTING BOMB *****************/
/***********************************************************/
public LoadLocationsBombCsgo()
{
	new String:hc[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, hc, sizeof(hc), "configs/aio/track_bomb/settings.cfg");
	
	new Handle:kv = CreateKeyValues("TrackBomb");
	FileToKeyValues(kv, hc);
	
	new max_map = 0;
	new max_direction = 0;
	
	if(KvGotoFirstSubKey(kv))
	{
		do
		{
			KvGetSectionName(kv, S_track_bomb_mapnames[max_map], 64);
			if(B_active_track_bomb_csgo_dev)
			{
				PrintToChatAll("%sMapname: %s", TAG_TRACK_BOMB_CSGO, S_track_bomb_mapnames[max_map]);
			}
			
			if(KvGotoFirstSubKey(kv))
			{
				if(KvGotoFirstSubKey(kv))
				{
					do
					{
						KvGetSectionName(kv, S_track_bomb_direction[max_map][max_direction], 64);
						if(B_active_track_bomb_csgo_dev)
						{
							PrintToChatAll("%sMapname: %s and direction: %s", TAG_TRACK_BOMB_CSGO, S_track_bomb_mapnames[max_map] , S_track_bomb_direction[max_map][max_direction]);
						}
						
						
						for(new i=0;i<MAX_WAY_TRACK_BOMB_CSGO;++i)
						{
							decl String:key[64], String:sound[PLATFORM_MAX_PATH];
							IntToString(i, key, 64);
							Format(sound, sizeof(sound), "sound%i", i);
							
							KvGetString(kv, key, S_track_bomb_way[max_map][max_direction][i], 64);
							KvGetString(kv, sound, S_track_bomb_way_sound[max_map][max_direction][i], PLATFORM_MAX_PATH);
							
							decl String:bufferString[PLATFORM_MAX_PATH];
							Format(bufferString, PLATFORM_MAX_PATH, "sound/%s", S_track_bomb_way_sound[max_map][max_direction][i]);
							
							if(FileExists(bufferString,true))
							{
								decl String:soundFile[PLATFORM_MAX_PATH];
								Format(soundFile, sizeof(soundFile), "*%s", S_track_bomb_way_sound[max_map][max_direction][i]);
								AddToStringTable(FindStringTable("soundprecache"), soundFile);
								
								AddFileToDownloadsTable(bufferString);
								
								if(B_active_track_bomb_csgo_dev)
								{
									LogMessage("%sAddToStringTable: %s | AddFileToDownloadsTable: %s", TAG_TRACK_BOMB_CSGO, soundFile, bufferString);
								}
							}
							
							if(B_active_track_bomb_csgo_dev)
							{
								if(strlen(S_track_bomb_way[max_map][max_direction][i]))
								{
									PrintToChatAll("%sMapname: %s and direction: %s and way: %s. Sound is: %s", TAG_TRACK_BOMB_CSGO, S_track_bomb_mapnames[max_map] , S_track_bomb_direction[max_map][max_direction], S_track_bomb_way[max_map][max_direction][i], S_track_bomb_way_sound[max_map][max_direction][i]);
								}
							}
						}
						
						++max_direction;
					}
					while (KvGotoNextKey(kv));
				}
				KvGoBack(kv);
			}
			
			max_map++;
			KvGoBack(kv);
			
		}
		while (KvGotoNextKey(kv));
	}
	CloseHandle(kv);
}
/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//****************************AIO MOD SOUNDS KILL****************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_SOUNDS_KILL_CSGO "[AIO SOUNDS KILL] - "
#define MAX_NUM_KILLS_CSGO 100

//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_sounds_kill_csgo;
new Handle:cvar_active_sounds_kill_csgo_dev;

new Handle:cvar_active_sounds_kill_team_kill_csgo;
new Handle:cvar_active_sounds_kill_combo_time_csgo;

//Bool
new bool:B_active_sounds_kill_csgo 					= false;
new bool:B_active_sounds_kill_csgo_dev				= false;

new bool:B_active_sounds_kill_team_kill_csgo		= false;

//Float
new Float:F_last_kill_time[MAXPLAYERS+1];
new Float:F_active_sounds_kill_combo_time_csgo;

//Strings
new String:S_sets_name_csgo							[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];

new String:S_headshot_sound							[MAX_NUM_SETS_CSGO][MAX_NUM_KILLS_CSGO][PLATFORM_MAX_PATH];
new String:S_grenade_sound							[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];
new String:S_self_kill_sound						[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];
new String:S_round_play_sound						[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];
new String:S_knife_sound							[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];
new String:S_kill_sound								[MAX_NUM_SETS_CSGO][MAX_NUM_KILLS_CSGO][PLATFORM_MAX_PATH];
new String:S_first_blood_sound						[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];
new String:S_team_kill_sound						[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];
new String:S_combo_sound							[MAX_NUM_SETS_CSGO][MAX_NUM_KILLS_CSGO][PLATFORM_MAX_PATH];
new String:S_join_sound								[MAX_NUM_SETS_CSGO][PLATFORM_MAX_PATH];

//Customs
new C_num_sets;

new C_headshot_config								[MAX_NUM_SETS_CSGO][MAX_NUM_KILLS_CSGO];
new C_grenade_config								[MAX_NUM_SETS_CSGO];
new C_self_kill_config								[MAX_NUM_SETS_CSGO];
new C_round_play_config								[MAX_NUM_SETS_CSGO];
new C_knife_config									[MAX_NUM_SETS_CSGO];
new C_kill_config									[MAX_NUM_SETS_CSGO][MAX_NUM_KILLS_CSGO];
new C_first_blood_config							[MAX_NUM_SETS_CSGO];
new C_team_kill_config								[MAX_NUM_SETS_CSGO];
new C_combo_config									[MAX_NUM_SETS_CSGO][MAX_NUM_KILLS_CSGO];
new C_join_config									[MAX_NUM_SETS_CSGO];

new C_total_kills = 0;
new C_consecutive_kills[MAXPLAYERS+1];
new C_combo_score[MAXPLAYERS+1];
new C_consecutive_headshots[MAXPLAYERS+1];

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartSoundsKillCsgo()
{
	CreateConVar("aio_sounds_kill_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_sounds_kill_csgo 					= CreateConVar("active_sounds_kill_csgo",  				"0", 					"Enable/Disable Sounds Kill", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_sounds_kill_csgo_dev				= CreateConVar("active_sounds_kill_csgo_dev", 			"0", 					"Enable/Disable Sounds Kill Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	cvar_active_sounds_kill_team_kill_csgo			= CreateConVar("active_sounds_kill_team_kill_csgo", 	"1", 					"Enable/Disable Team kill", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_sounds_kill_combo_time_csgo			= CreateConVar("active_sounds_kill_combo_time_csgo", 	"2.0", 					"Combo time", 							DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	HookEventsSoundsKillCsgo();
	LoadSoundSets();
	NewRoundInitialization();
	
	ServerCommand("exec /aio/sounds_kill/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsSoundsKillCsgo()
{
	HookConVarChange(cvar_active_sounds_kill_csgo, 				Event_CvarChangeSoundsKillCsgo);
	HookConVarChange(cvar_active_sounds_kill_csgo_dev, 			Event_CvarChangeSoundsKillCsgo);
	HookConVarChange(cvar_active_sounds_kill_team_kill_csgo, 	Event_CvarChangeSoundsKillCsgo);
	HookConVarChange(cvar_active_sounds_kill_combo_time_csgo, 	Event_CvarChangeSoundsKillCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeSoundsKillCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateSoundsKillCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateSoundsKillCsgo()
{
	B_active_sounds_kill_csgo 					= GetConVarBool(cvar_active_sounds_kill_csgo);
	B_active_sounds_kill_csgo_dev 				= GetConVarBool(cvar_active_sounds_kill_csgo_dev);
	B_active_sounds_kill_team_kill_csgo 		= GetConVarBool(cvar_active_sounds_kill_team_kill_csgo);
	F_active_sounds_kill_combo_time_csgo 		= GetConVarFloat(cvar_active_sounds_kill_combo_time_csgo);
	
	LoadSoundSets();
	NewRoundInitialization();
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedSoundsKillCsgo()
{
	//UpdateStateSoundsKillCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartSoundsKillCsgo()
{
	UpdateStateSoundsKillCsgo();
}

/***********************************************************/
/******************** WHEN ROUND START *********************/
/***********************************************************/
public Action:Event_RoundStartoundsKillCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_sounds_kill_csgo)
	{
		NewRoundInitialization();
	}
}

/***********************************************************/
/******************** WHEN FREEZE END **********************/
/***********************************************************/
public Action:Event_RoundFreezeEndSoundsKillCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_sounds_kill_csgo)
	{
		for(new i = 1; i <= GetMaxHumanPlayers(); i++)
		{
			if(IsClientInGame(i) && !IsFakeClient(i) && C_sound_preference[i] >= 0)
			{
				if(!StrEqual(S_round_play_sound[C_sound_preference[i]], "")  && (C_round_play_config[C_sound_preference[i]] & 1) || (C_round_play_config[C_sound_preference[i]] & 2) || (C_round_play_config[C_sound_preference[i]] & 4))
				{
					EmitSoundToClient(i, S_round_play_sound[C_sound_preference[i]], _, _, _, _, 1.0);
				}
				if(C_text_preference[i] && (C_round_play_config[C_sound_preference[i]] & 8) || (C_round_play_config[C_sound_preference[i]] & 16) || (C_round_play_config[C_sound_preference[i]] & 32))
				{
					PrintCenterText(i, "%t", "Sounds Kill round play");
				}
			}
		}
	}
}

/***********************************************************/
/******************** WHEN PLAYER DIE **********************/
/***********************************************************/
public Event_PlayerDeathSoundsKillCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_sounds_kill_csgo)
	{
		new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
		new String:S_attacker_name[MAX_NAME_LENGTH];
		GetClientName(attacker, S_attacker_name, MAX_NAME_LENGTH);
		new victim = GetClientOfUserId(GetEventInt(event, "userid"));
		new String:S_victim_name[MAX_NAME_LENGTH];
		GetClientName(victim, S_victim_name, MAX_NAME_LENGTH);
		new String:S_buffer_string[256];
		
		if(victim < 1 || victim > GetMaxHumanPlayers())
		{
			return;
		}
		else
		{
			if(attacker == victim || attacker == 0)
			{
				C_consecutive_kills[attacker] = 0;
				for(new i = 1; i <= GetMaxHumanPlayers(); i++)
				{
					if(IsClientInGame(i) && !IsFakeClient(i) && C_sound_preference[i] >= 0)
					{
						if(!StrEqual(S_self_kill_sound[C_sound_preference[i]],""))
						{
							if(C_self_kill_config[C_sound_preference[i]] & 1)
							{
								EmitSoundToClient(i,S_self_kill_sound[C_sound_preference[i]],_,_,_,_,1.0);
							}
							else if((C_self_kill_config[C_sound_preference[i]] & 2) && attacker == i)
							{
								EmitSoundToClient(i,S_self_kill_sound[C_sound_preference[i]],_,_,_,_,1.0);
							}
							else if((C_self_kill_config[C_sound_preference[i]] & 4) && victim == i)
							{
								EmitSoundToClient(i,S_self_kill_sound[C_sound_preference[i]],_,_,_,_,1.0);
							}
						}
						if(C_text_preference[i])
						{
							if(C_self_kill_config[C_sound_preference[i]] & 8)
							{
								PrintCenterText(i, "%t", "Sounds Kill selfkill", S_victim_name);
							}
							else if((C_self_kill_config[C_sound_preference[i]] & 16) && attacker == i)
							{
								PrintCenterText(i, "%t", "Sounds Kill selfkill", S_victim_name);
							}
							else if((C_self_kill_config[C_sound_preference[i]] & 32) && victim == i)
							{
								PrintCenterText(i, "%t", "Sounds Kill selfkill", S_victim_name);
							}
						}
					}
				}
			}
			
			else if(GetClientTeam(attacker) == GetClientTeam(victim) && !B_active_sounds_kill_team_kill_csgo)
			{
				C_consecutive_kills[attacker] = 0;
				for(new i = 1; i <= GetMaxHumanPlayers(); i++)
				{
					if(IsClientInGame(i) && !IsFakeClient(i) && C_sound_preference[i] >= 0)
					{
						if(!StrEqual(S_team_kill_sound[C_sound_preference[i]],""))
						{
							if(C_team_kill_config[C_sound_preference[i]] & 1)
							{
								EmitSoundToClient(i,S_team_kill_sound[C_sound_preference[i]],_,_,_,_,1.0);
							}
							else if((C_team_kill_config[C_sound_preference[i]] & 2) && attacker == i)
							{
								EmitSoundToClient(i,S_team_kill_sound[C_sound_preference[i]],_,_,_,_,1.0);
							}
							else if((C_team_kill_config[C_sound_preference[i]] & 4) && victim == i)
							{
								EmitSoundToClient(i,S_team_kill_sound[C_sound_preference[i]],_,_,_,_,1.0);
							}
						}
						if(C_text_preference[i])
						{
							if(C_team_kill_config[C_sound_preference[i]] & 8)
							{
								PrintCenterText(i,"%t","Sounds Kill teamkill",S_attacker_name,S_victim_name);
							}
							else if((C_team_kill_config[C_sound_preference[i]] & 16) && attacker == i)
							{
								PrintCenterText(i,"%t","Sounds Kill teamkill",S_attacker_name,S_victim_name);
							}
							else if((C_team_kill_config[C_sound_preference[i]] & 32) && victim == i)
							{
								PrintCenterText(i,"%t","Sounds Kill teamkill",S_attacker_name,S_victim_name);
							}
						}
					}
				}
			}
			else
			{
				C_total_kills++;
				C_consecutive_kills[attacker]++;
				new bool:firstblood;
				new bool:headshot;
				new bool:knife;
				new bool:grenade;
				new bool:combo;
				
				new String:S_weapon[GetMaxHumanPlayers()];
				GetEventString(event, "weapon" , S_weapon,GetMaxHumanPlayers());

				headshot = GetEventBool(event,"headshot");

				if(headshot)
				{
					C_consecutive_headshots[attacker]++;
				}
				new Float:F_temp_last_kill_time = F_last_kill_time[attacker];
				F_last_kill_time[attacker] = GetEngineTime();
				
				if(F_temp_last_kill_time == -1.0 || (F_last_kill_time[attacker] - F_temp_last_kill_time) > F_active_sounds_kill_combo_time_csgo)
				{
					C_combo_score[attacker] = 1;
					combo = false;
				}
				else
				{
					C_combo_score[attacker]++;
					combo = true;
				}
				if(C_total_kills == 1)
				{
					firstblood = true;
				}


				if(StrEqual(S_weapon,"inferno") || StrEqual(S_weapon,"hegrenade") || StrEqual(S_weapon,"flashbang") || StrEqual(S_weapon,"decoy") || StrEqual(S_weapon,"smokegrenade"))
				{
					grenade = true;
				}
				else if(StrEqual(S_weapon,"knife_default_ct") || StrEqual(S_weapon,"knife_default_t") || StrEqual(S_weapon,"knifegg") || StrEqual(S_weapon,"knife_flip") || StrEqual(S_weapon,"knife_gut") || StrEqual(S_weapon,"knife_karambit") || StrEqual(S_weapon,"bayonet") || StrEqual(S_weapon,"knife_m9_bayonet"))
				{
					knife = true;
				}

				for(new i = 1; i <= GetMaxHumanPlayers(); i++)
				{
					if(IsClientInGame(i) && !IsFakeClient(i) && C_sound_preference[i] >= 0)
					{
						if(firstblood && C_first_blood_config[C_sound_preference[i]] > 0)
						{
							if(!StrEqual(S_first_blood_sound[C_sound_preference[i]],""))
							{
								if(C_first_blood_config[C_sound_preference[i]] & 1)
								{
									EmitSoundToClient(i,S_first_blood_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
								else if((C_first_blood_config[C_sound_preference[i]] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_first_blood_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
								else if((C_first_blood_config[C_sound_preference[i]] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_first_blood_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i])
							{
								if(C_first_blood_config[C_sound_preference[i]] & 8)
								{
									PrintCenterText(i,"%t","Sounds Kill first blood",S_attacker_name);
								}
								else if((C_first_blood_config[C_sound_preference[i]] & 16) && attacker == i)
								{
									PrintCenterText(i,"%t","Sounds Kill first blood",S_attacker_name);
								}
								else if((C_first_blood_config[C_sound_preference[i]] & 32) && victim == i)
								{
									PrintCenterText(i,"%t","Sounds Kill first blood",S_attacker_name);
								}
							}
						}
						else if(headshot && C_headshot_config[C_sound_preference[i]][0] > 0)
						{
							if(!StrEqual(S_headshot_sound[C_sound_preference[i]][0],""))
							{
								if(C_headshot_config[C_sound_preference[i]][0] & 1)
								{
									EmitSoundToClient(i,S_headshot_sound[C_sound_preference[i]][0],_,_,_,_,1.0);
								}
								else if((C_headshot_config[C_sound_preference[i]][0] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_headshot_sound[C_sound_preference[i]][0],_,_,_,_,1.0);
								}
								else if((C_headshot_config[C_sound_preference[i]][0] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_headshot_sound[C_sound_preference[i]][0],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i])
							{
								if(C_headshot_config[C_sound_preference[i]][0] & 8)
								{
									PrintCenterText(i,"%t","Sounds Kill headshot",S_attacker_name);
								}
								else if((C_headshot_config[C_sound_preference[i]][0] & 16) && attacker == i)
								{
									PrintCenterText(i,"%t","Sounds Kill headshot",S_attacker_name);
								}
								else if((C_headshot_config[C_sound_preference[i]][0] & 32) && victim == i)
								{
									PrintCenterText(i,"%t","Sounds Kill headshot",S_attacker_name);
								}
							}
						}
						else if(headshot && C_consecutive_headshots[attacker] < MAX_NUM_KILLS_CSGO && C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] > 0)
						{
							if(!StrEqual(S_headshot_sound[C_sound_preference[i]][C_consecutive_headshots[attacker]],""))
							{
								if(C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] & 1)
								{
									EmitSoundToClient(i,S_headshot_sound[C_sound_preference[i]][C_consecutive_headshots[attacker]],_,_,_,_,1.0);
								}
								else if((C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_headshot_sound[C_sound_preference[i]][C_consecutive_headshots[attacker]],_,_,_,_,1.0);
								}
								else if((C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_headshot_sound[C_sound_preference[i]][C_consecutive_headshots[attacker]],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i] && C_consecutive_headshots[attacker] < MAX_NUM_KILLS_CSGO)
							{
								if(C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] & 8)
								{
									Format(S_buffer_string,256,"Sounds Kill headshot %i",C_consecutive_headshots[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
								else if((C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] & 16) && attacker == i)
								{
									Format(S_buffer_string,256,"Sounds Kill headshot %i",C_consecutive_headshots[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
								else if((C_headshot_config[C_sound_preference[i]][C_consecutive_headshots[attacker]] & 32) && victim == i)
								{
									Format(S_buffer_string,256,"Sounds Kill headshot %i",C_consecutive_headshots[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
							}
						}
						else if(knife && C_knife_config[C_sound_preference[i]] > 0)
						{
							if(!StrEqual(S_knife_sound[C_sound_preference[i]],""))
							{
								if(C_knife_config[C_sound_preference[i]] & 1)
								{
									EmitSoundToClient(i,S_knife_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
								else if((C_knife_config[C_sound_preference[i]] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_knife_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
								else if((C_knife_config[C_sound_preference[i]] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_knife_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i])
							{
								if(C_knife_config[C_sound_preference[i]] & 8)
								{
									PrintCenterText(i,"%t","Sounds Kill knife",S_attacker_name,S_victim_name);
								}
								else if((C_knife_config[C_sound_preference[i]] & 16) && attacker == i)
								{
									PrintCenterText(i,"%t","Sounds Kill knife",S_attacker_name,S_victim_name);
								}
								else if((C_knife_config[C_sound_preference[i]] & 32) && victim == i)
								{
									PrintCenterText(i,"%t","Sounds Kill knife",S_attacker_name,S_victim_name);
								}
							}
						}
						else if(grenade && C_grenade_config[C_sound_preference[i]] > 0)
						{
							if(!StrEqual(S_grenade_sound[C_sound_preference[i]],""))
							{
								if(C_grenade_config[C_sound_preference[i]] & 1)
								{
									EmitSoundToClient(i,S_grenade_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
								else if((C_grenade_config[C_sound_preference[i]] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_grenade_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
								else if((C_grenade_config[C_sound_preference[i]] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_grenade_sound[C_sound_preference[i]],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i])
							{
								if(C_grenade_config[C_sound_preference[i]] & 8)
								{
									PrintCenterText(i,"%t","Sounds Kill grenade",S_attacker_name,S_victim_name);
								}
								else if((C_grenade_config[C_sound_preference[i]] & 16) && attacker == i)
								{
									PrintCenterText(i,"%t","Sounds Kill grenade",S_attacker_name,S_victim_name);
								}
								else if((C_grenade_config[C_sound_preference[i]] & 32) && victim == i)
								{
									PrintCenterText(i,"%t","Sounds Kill grenade",S_attacker_name,S_victim_name);
								}
							}
						}
						else if(combo && C_combo_score[attacker] < MAX_NUM_KILLS_CSGO && C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] > 0)
						{
							if(!StrEqual(S_combo_sound[C_sound_preference[i]][C_combo_score[attacker]],""))
							{
								if(C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] & 1)
								{
									EmitSoundToClient(i,S_combo_sound[C_sound_preference[i]][C_combo_score[attacker]],_,_,_,_,1.0);
								}
								else if((C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_combo_sound[C_sound_preference[i]][C_combo_score[attacker]],_,_,_,_,1.0);
								}
								else if((C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_combo_sound[C_sound_preference[i]][C_combo_score[attacker]],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i] && C_combo_score[attacker] < MAX_NUM_KILLS_CSGO)
							{
								if(C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] & 8)
								{
									Format(S_buffer_string,256,"Sounds Kill combo %i",C_combo_score[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
								else if((C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] & 16) && attacker == i)
								{
									Format(S_buffer_string,256,"Sounds Kill combo %i",C_combo_score[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
								else if((C_combo_config[C_sound_preference[i]][C_combo_score[attacker]] & 32) && victim == i)
								{
									Format(S_buffer_string,256,"Sounds Kill combo %i",C_combo_score[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
							}
						}
						else
						{
							if(C_consecutive_kills[attacker] < MAX_NUM_KILLS_CSGO && !StrEqual(S_kill_sound[C_sound_preference[i]][C_consecutive_kills[attacker]],""))
							{
								if(C_kill_config[C_sound_preference[i]][C_consecutive_kills[attacker]] & 1)
								{
									EmitSoundToClient(i,S_kill_sound[C_sound_preference[i]][C_consecutive_kills[attacker]],_,_,_,_,1.0);
								}
								else if((C_kill_config[C_sound_preference[i]][C_consecutive_kills[attacker]] & 2) && attacker == i)
								{
									EmitSoundToClient(i,S_kill_sound[C_sound_preference[i]][C_consecutive_kills[attacker]],_,_,_,_,1.0);
								}
								else if((C_kill_config[C_sound_preference[i]][C_consecutive_kills[attacker]] & 4) && victim == i)
								{
									EmitSoundToClient(i,S_kill_sound[C_sound_preference[i]][C_consecutive_kills[attacker]],_,_,_,_,1.0);
								}
							}
							if(C_text_preference[i] && C_consecutive_kills[attacker] < MAX_NUM_KILLS_CSGO)
							{
								if(C_kill_config[C_sound_preference[i]][C_consecutive_kills[attacker]] & 8)
								{
									Format(S_buffer_string,256,"Sounds Kill killsound %i",C_consecutive_kills[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
								else if((C_kill_config[C_sound_preference[i]][C_consecutive_kills[attacker]] & 16) && attacker == i)
								{
									Format(S_buffer_string,256,"Sounds Kill killsound %i",C_consecutive_kills[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
								else if((C_kill_config[C_sound_preference[i]][C_consecutive_kills[attacker]] & 32) && victim == i)
								{
									Format(S_buffer_string,256,"Sounds Kill killsound %i",C_consecutive_kills[attacker]);
									PrintCenterText(i,"%t",S_buffer_string,S_attacker_name);
								}
							}
						}
					}
				}
			}
		}
		C_consecutive_kills[victim] = 0;
		C_consecutive_headshots[victim] = 0;
	}
}

/***********************************************************/
/********************* LOAD SOUND SETS *********************/
/***********************************************************/
public LoadSoundSets()
{
	new String:S_buffer_string[PLATFORM_MAX_PATH];
	new Handle:H_sound_set_key = CreateKeyValues("List");
	
	BuildPath(Path_SM,S_buffer_string, PLATFORM_MAX_PATH, "configs/aio/sounds_kill/setslist.cfg");
	if(FileToKeyValues(H_sound_set_key, S_buffer_string))
	{
		if(KvJumpToKey(H_sound_set_key,"sets"))
		{
			C_num_sets = 0;
			for(new i = 0; i < MAX_NUM_SETS_CSGO; i++)
			{
				Format(S_buffer_string, PLATFORM_MAX_PATH, "set %i", (i+1));
				KvGetString(H_sound_set_key, S_buffer_string, S_sets_name_csgo[i], PLATFORM_MAX_PATH);
				if(!StrEqual(S_sets_name_csgo[i], ""))
				{
					BuildPath(Path_SM,S_buffer_string,PLATFORM_MAX_PATH, "configs/aio/sounds_kill/sets/%s.cfg", S_sets_name_csgo[i]);
					PrintToServer("[SM] Quake Sounds v3: Loading sound set config '%s'.", S_buffer_string);
					LoadSet(S_buffer_string, i);
					C_num_sets++;
				}
			}
		}
		else
		{
			CloseHandle(H_sound_set_key);
			SetFailState("configs/aio/sounds_kill/setslist.cfg not correctly structured");
			return;
		}
	}
	else
	{
		CloseHandle(H_sound_set_key);
		SetFailState("configs/aio/sounds_kill/setslist.cfg not found");
	}
	CloseHandle(H_sound_set_key);
}

/***********************************************************/
/*********************** LOAD SETS *************************/
/***********************************************************/
public LoadSet(String:setFile[],setNum)
{
	new String:S_buffering_string[PLATFORM_MAX_PATH];
	new Handle:H_set_file_key = CreateKeyValues("Set");
	if(FileToKeyValues(H_set_file_key, setFile))
	{
		if(KvJumpToKey(H_set_file_key, "headshot"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				do
				{
					KvGetSectionName(H_set_file_key, S_buffering_string, PLATFORM_MAX_PATH);
					new killNum = StringToInt(S_buffering_string);
					if(killNum >= 0)
					{
						KvGetString(H_set_file_key, "sound", S_headshot_sound[setNum][killNum], PLATFORM_MAX_PATH);
						C_headshot_config[setNum][killNum] = KvGetNum(H_set_file_key, "config", 9);
						Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_headshot_sound[setNum][killNum]);
						if(FileExists(S_buffering_string, true))
						{
							AddSoundToCache(S_headshot_sound[setNum][killNum], PLATFORM_MAX_PATH);
							AddFileToDownloadsTable(S_buffering_string);
						}
						else
						{
							C_headshot_config[setNum][killNum] = 0;
							PrintToServer("[SM] Quake Sounds v3: File specified in 'headshot %i' does not exist in '%s', ignoring.", killNum, setFile);
						}
					}
				} 
				while (KvGotoNextKey(H_set_file_key));
				KvGoBack(H_set_file_key);
			}
			else
			{
				PrintToServer("[SM] Quake Sounds v3: 'headshot' section not configured correctly in %s.", setFile);
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'headshot' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "grenade"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'grenade' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_grenade_sound[setNum], PLATFORM_MAX_PATH);
				C_grenade_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_grenade_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_grenade_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_grenade_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'grenade' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'grenade' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "selfkill"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'selfkill' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_self_kill_sound[setNum], PLATFORM_MAX_PATH);
				C_self_kill_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_self_kill_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_self_kill_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_self_kill_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'selfkill' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'selfkill' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "round play"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'round play' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_round_play_sound[setNum], PLATFORM_MAX_PATH);
				C_round_play_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_round_play_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_round_play_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_round_play_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'round play' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'round play' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "knife"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'knife' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_knife_sound[setNum], PLATFORM_MAX_PATH);
				C_knife_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_knife_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_knife_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_knife_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'knife' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'knife' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "killsound"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				do
				{
					KvGetSectionName(H_set_file_key, S_buffering_string, PLATFORM_MAX_PATH);
					new killNum = StringToInt(S_buffering_string);
					if(killNum >= 0)
					{
						KvGetString(H_set_file_key, "sound", S_kill_sound[setNum][killNum], PLATFORM_MAX_PATH);
						C_kill_config[setNum][killNum] = KvGetNum(H_set_file_key, "config", 9);
						Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_kill_sound[setNum][killNum]);
						if(FileExists(S_buffering_string, true))
						{
							AddSoundToCache(S_kill_sound[setNum][killNum], PLATFORM_MAX_PATH);
							AddFileToDownloadsTable(S_buffering_string);
						}
						else
						{
							C_kill_config[setNum][killNum] = 0;
							PrintToServer("[SM] Quake Sounds v3: File specified in 'killsound %i' does not exist in '%s', ignoring.", killNum, setFile);
						}
					}
				} 
				while (KvGotoNextKey(H_set_file_key));
				KvGoBack(H_set_file_key);
			}
			else
			{
				PrintToServer("[SM] Quake Sounds v3: 'killsound' section not configured correctly in %s.", setFile);
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'killsound' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "first blood"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'first blood' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_first_blood_sound[setNum], PLATFORM_MAX_PATH);
				C_first_blood_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_first_blood_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_first_blood_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_first_blood_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'first blood' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'first blood' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "teamkill"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'teamkill' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_team_kill_sound[setNum], PLATFORM_MAX_PATH);
				C_team_kill_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_team_kill_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_team_kill_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_team_kill_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'teamkill' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'teamkill' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "combo"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				do
				{
					KvGetSectionName(H_set_file_key, S_buffering_string, PLATFORM_MAX_PATH);
					new killNum = StringToInt(S_buffering_string);
					if(killNum >= 0)
					{
						KvGetString(H_set_file_key, "sound", S_combo_sound[setNum][killNum], PLATFORM_MAX_PATH);
						C_combo_config[setNum][killNum] = KvGetNum(H_set_file_key, "config", 9);
						Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_combo_sound[setNum][killNum]);
						if(FileExists(S_buffering_string, true))
						{
							AddSoundToCache(S_combo_sound[setNum][killNum], PLATFORM_MAX_PATH);
							AddFileToDownloadsTable(S_buffering_string);
						}
						else
						{
							C_combo_config[setNum][killNum] = 0;
							PrintToServer("[SM] Quake Sounds v3: File specified in 'combo %i' does not exist in '%s', ignoring.", killNum, setFile);
						}
					}
				} 
				while (KvGotoNextKey(H_set_file_key));
				KvGoBack(H_set_file_key);
			}
			else
			{
				PrintToServer("[SM] Quake Sounds v3: 'combo' section not configured correctly in %s.", setFile);
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'combo' section missing in %s.", setFile);
		}
		KvRewind(H_set_file_key);
		if(KvJumpToKey(H_set_file_key, "join server"))
		{
			if(KvGotoFirstSubKey(H_set_file_key))
			{
				PrintToServer("[SM] Quake Sounds v3: 'join server' section not configured correctly in %s.", setFile);
				KvGoBack(H_set_file_key);
			}
			else
			{
				KvGetString(H_set_file_key, "sound", S_join_sound[setNum], PLATFORM_MAX_PATH);
				C_join_config[setNum] = KvGetNum(H_set_file_key, "config", 9);
				Format(S_buffering_string, PLATFORM_MAX_PATH, "sound/%s", S_join_sound[setNum]);
				if(FileExists(S_buffering_string, true))
				{
					AddSoundToCache(S_join_sound[setNum], PLATFORM_MAX_PATH);
					AddFileToDownloadsTable(S_buffering_string);
				}
				else
				{
					C_join_config[setNum] = 0;
					PrintToServer("[SM] Quake Sounds v3: File specified in 'join server' does not exist in '%s', ignoring.", setFile);
				}
			}
		}
		else
		{
			PrintToServer("[SM] Quake Sounds v3: 'join server' section missing in %s.", setFile);
		}
	}
	else
	{
		PrintToServer("[SM] Quake Sounds v3: Cannot parse '%s', file not found or incorrectly structured!", setFile);
	}
	CloseHandle(H_set_file_key);
}

/***********************************************************/
/******************* ADD SOUND TO CACHE ********************/
/***********************************************************/
stock AddSoundToCache(String:soundFile[],maxLength)
{
	Format(soundFile,maxLength,"*%s",soundFile);
	AddToStringTable(FindStringTable("soundprecache"),soundFile);
}

/***********************************************************/
/******************* RESET STATS ROUND *********************/
/***********************************************************/
public NewRoundInitialization()
{
	C_total_kills = 0;
	for(new i = 1; i <= GetMaxHumanPlayers(); i++) 
	{
		C_consecutive_headshots[i] = 0;
		F_last_kill_time[i] = -1.0;
	}
	if(B_active_sounds_kill_csgo_dev)
	{
		PrintToChatAll("%sKill stats reset", TAG_SOUNDS_KILL_CSGO);
	}
}
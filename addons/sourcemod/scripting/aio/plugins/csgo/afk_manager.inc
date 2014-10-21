/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//**************************AIO MOD AFK MANAGER CSGO*************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_AFK_MANAGER_CSGO 		"[AIO AFK MANAGER] - "
#define CHECK_ADMIN_IMMUNITY(%1) 	(C_admin_flag == 0 ? GetUserFlagBits(%1)!=0 : (GetUserFlagBits(%1) & C_admin_flag || GetUserFlagBits(%1) & ADMFLAG_ROOT))
#define AFK_CHECK_INTERVAL_CSGO 	5.0
#define AFK_THRESHOLD_CSGO 			30.0 //AFK_THRESHOLD_CSGO for amount of movement required to mark a player as AFK.

//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_afk_manager_csgo;
new Handle:cvar_active_afk_manager_csgo_dev;

new Handle:cvar_active_afk_manager_move_min_csgo;
new Handle:cvar_active_afk_manager_kick_min_csgo;
new Handle:cvar_active_afk_manager_move_time_csgo;
new Handle:cvar_active_afk_manager_kick_time_csgo;
new Handle:cvar_active_afk_manager_warn_time_csgo;
new Handle:cvar_active_afk_manager_immune_csgo;
new Handle:cvar_active_afk_manager_immune_flag_csgo;

//Bool
new bool:B_active_afk_manager_csgo 							= false;
new bool:B_active_afk_manager_csgo_dev						= false;
new bool:B_client_is_in[MAXPLAYERS+1]						= false;

//Float
new Float:F_active_afk_manager_move_time_csgo;
new Float:F_active_afk_manager_kick_time_csgo;
new Float:F_active_afk_manager_warn_time_csgo;

new Float:F_client_afk_time_csgo[MAXPLAYERS+1]				= {0.0, ...};
new Float:F_client_eye_position_csgo[MAXPLAYERS+1][3];
new Float:F_client_origin_position_csgo[MAXPLAYERS+1][3];

//Customs
new C_active_afk_manager_move_min_csgo;
new C_active_afk_manager_kick_min_csgo;
new C_active_afk_manager_immune_csgo;

//GetEntProp(client, Prop_Send, "m_iObserverMode");
/* mode 0 = playing view */
/* mode 2 = when die view */
/* mode 4 = first view */
/* mode 5 = third view */
/* mode 6 = free view */

new C_observer_mode_csgo[MAXPLAYERS+1];
new C_observer_target_csgo[MAXPLAYERS+1];
new C_admin_flag;

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartAfkManagerCsgo()
{
	CreateConVar("aio_afk_manager_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_afk_manager_csgo 					= CreateConVar("active_afk_manager_csgo",  				"0", 					"Enable/Disable AFK Manager", 											DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_afk_manager_csgo_dev				= CreateConVar("active_afk_manager_csgo_dev", 			"0", 					"Enable/Disable AFK Manager Dev Mod", 									DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	cvar_active_afk_manager_move_min_csgo			= CreateConVar("active_afk_manager_move_min_csgo", 		"4", 					"Minimum player to move player", 										DEFAULT_FLAGS);
	cvar_active_afk_manager_kick_min_csgo			= CreateConVar("active_afk_manager_kick_min_csgo", 		"6", 					"Minimum player to kick player", 										DEFAULT_FLAGS);
	cvar_active_afk_manager_move_time_csgo			= CreateConVar("active_afk_manager_move_time_csgo", 	"60.0", 				"Time to move player", 													DEFAULT_FLAGS);
	cvar_active_afk_manager_kick_time_csgo			= CreateConVar("active_afk_manager_kick_time_csgo", 	"120.", 				"Time to kick player", 													DEFAULT_FLAGS);
	cvar_active_afk_manager_warn_time_csgo			= CreateConVar("active_afk_manager_warn_time_csgo", 	"30.0", 				"Time to show warining message", 										DEFAULT_FLAGS);
	cvar_active_afk_manager_immune_csgo				= CreateConVar("active_afk_manager_immune_csgo", 		"1", 					"AFK admins immunity: 0 = DISABLED, 1 = COMPLETE, 2 = KICK, 3 = MOVE", 	DEFAULT_FLAGS);
	cvar_active_afk_manager_immune_flag_csgo		= CreateConVar("active_afk_manager_immune_flag_csgo", 	"", 					"Admin flag for immunity, blank=any flag", 								DEFAULT_FLAGS);
	
	SetImmuneFlagCsgo(cvar_active_afk_manager_immune_flag_csgo);
	
	HookEventsAfkManagerCsgo();
	
	AddCommandListener(Command_AfkManagerCsgoSay, "say");
	AddCommandListener(Command_AfkManagerCsgoSay, "say_team");
	
	RegAdminCmd("sm_afkcheck",		Command_AfkManagerCsgo, 		ADMFLAG_CHANGEMAP, 		"Display afk manager infos.");
	
	ServerCommand("exec /aio/afk_manager/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsAfkManagerCsgo()
{
	HookConVarChange(cvar_active_afk_manager_csgo, 				Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_csgo_dev, 			Event_CvarChangeAfkManagerCsgo);
	
	HookConVarChange(cvar_active_afk_manager_move_min_csgo, 	Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_kick_min_csgo, 	Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_move_time_csgo, 	Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_kick_time_csgo, 	Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_warn_time_csgo, 	Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_immune_csgo, 		Event_CvarChangeAfkManagerCsgo);
	HookConVarChange(cvar_active_afk_manager_immune_flag_csgo, 	Event_CvarChangeAfkManagerCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeAfkManagerCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateAfkManagerCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateAfkManagerCsgo()
{
	B_active_afk_manager_csgo 					= GetConVarBool(cvar_active_afk_manager_csgo);
	B_active_afk_manager_csgo_dev 				= GetConVarBool(cvar_active_afk_manager_csgo_dev);
	
	C_active_afk_manager_move_min_csgo 			= GetConVarInt(cvar_active_afk_manager_move_min_csgo);
	C_active_afk_manager_kick_min_csgo 			= GetConVarInt(cvar_active_afk_manager_kick_min_csgo);
	C_active_afk_manager_immune_csgo 			= GetConVarInt(cvar_active_afk_manager_immune_csgo);
	
	F_active_afk_manager_move_time_csgo 		= GetConVarFloat(cvar_active_afk_manager_move_time_csgo);
	F_active_afk_manager_kick_time_csgo 		= GetConVarFloat(cvar_active_afk_manager_kick_time_csgo);
	F_active_afk_manager_warn_time_csgo 		= GetConVarFloat(cvar_active_afk_manager_warn_time_csgo);
	
	SetImmuneFlagCsgo(cvar_active_afk_manager_immune_flag_csgo);
	
	if(B_active_afk_manager_csgo)
	{
		CreateTimer(AFK_CHECK_INTERVAL_CSGO, Timer_CheckPlayerAfkManagerCsgo, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
		for (new i=1; i<=MaxClients; i++)
		{
			B_client_is_in[i] = false;
			if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i))
			{
				InitializePlayerCsgo(i);
			}
		}
	}
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedAfkManagerCsgo()
{
	//UpdateStateAfkManagerCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartAfkManagerCsgo()
{
	UpdateStateAfkManagerCsgo();
}

/***********************************************************/
/************ WHEN CLIENT CONNECT WITH CHECKING ************/
/***********************************************************/
public OnClientPostAdminCheckAfkManagerCsgo(client)
{
	if(B_active_afk_manager_csgo)
	{
		if(!IsFakeClient(client))
		{
			B_client_is_in[client] = true;
		}
	}
}

public OnClientDisconnectAfkManagerCsgo(client)
{
	if(B_active_afk_manager_csgo)
	{
		B_client_is_in[client] = false;
	}
}

/***********************************************************/
/******************** WHEN PLAYER SPAWN ********************/
/***********************************************************/
public Action:Event_PlayerSpawnAfkManagerCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_afk_manager_csgo)
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		
		if(IsClientValid(client) && !IsFakeClient(client) && !IsClientObserver(client) && IsPlayerAlive(client))
		{
			ResetPlayerInfosCsgo(client);
			if(B_active_afk_manager_csgo_dev)
			{
				PrintToChat(client, "%sClient spawn, is alive so we reset infos.", TAG_AFK_MANAGER_CSGO);
			}
		}
	}
}

/***********************************************************/
/***************** WHEN PLAYER CHANGE TEAM *****************/
/***********************************************************/
public Action:Event_PlayerTeamAfkManagerCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_afk_manager_csgo)
	{
		new client 	= GetClientOfUserId(GetEventInt(event, "userid"));
		new team 	= GetEventInt(event, "team");
		if(IsClientValid(client) && !IsFakeClient(client))
		{
			if(!B_client_is_in[client])
			{
				InitializePlayerCsgo(client);
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sClient not in so we init him.", TAG_AFK_MANAGER_CSGO);
				}
			}
			
			if(team != 1)
			{
				if(B_client_is_in[client])
				{
					ResetPlayerInfosCsgo(client);
					if(B_active_afk_manager_csgo_dev)
					{
						PrintToChat(client, "%sTeam != Spec and client is in.", TAG_AFK_MANAGER_CSGO);
					}
				}
			}
			else
			{
				GetClientEyeAngles(client, F_client_eye_position_csgo[client]);
				C_observer_mode_csgo[client] 	= GetEntProp(client, Prop_Send, "m_iObserverMode");
				C_observer_target_csgo[client] 	= GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");

				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sEye Angles:%f, Mode:%i, Target:%i", TAG_AFK_MANAGER_CSGO, F_client_eye_position_csgo[client], C_observer_mode_csgo[client] , C_observer_target_csgo[client]);
				}
			}
		}
	}
}
/***********************************************************/
/******************** WHEN PLAYER DEATH ********************/
/***********************************************************/
public Action:Event_PlayerDeathAfkManagerCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_afk_manager_csgo)
	{
		new client 	= GetClientOfUserId(GetEventInt(event, "userid"));
		ResetPlayerInfosCsgo(client);
		if(B_active_afk_manager_csgo_dev)
		{
			PrintToChat(client, "%sPlayer die so we reset infos.", TAG_AFK_MANAGER_CSGO);
		}
	}
}

/***********************************************************/
/********************* IF PLAYER WRITE *********************/
/***********************************************************/
public Action:Command_AfkManagerCsgoSay(client, const String:command[], args)
{
	if(B_active_afk_manager_csgo)
	{
		ResetPlayerInfosCsgo(client);
		if(B_active_afk_manager_csgo_dev)
		{
			PrintToChat(client, "%sClient write so we reset infos.", TAG_AFK_MANAGER_CSGO);
		}
	}
}
/***********************************************************/
/******************** RESET PLAYER INFOS *******************/
/***********************************************************/
ResetPlayerInfosCsgo(client)
{
	F_client_afk_time_csgo[client] 		= 0.0;
	F_client_eye_position_csgo[client] 	= Float:{0.0,0.0,0.0};
	C_observer_mode_csgo[client] 		= C_observer_target_csgo[client] = 0;
}

/***********************************************************/
/************************ INIT PLAYER **********************/
/***********************************************************/
InitializePlayerCsgo(client)
{
	if (!(C_active_afk_manager_immune_csgo == 1 && CHECK_ADMIN_IMMUNITY(client)))
	{
		B_client_is_in[client] = true;
		ResetPlayerInfosCsgo(client);
	}
}

/***********************************************************/
/********************** SET IMMUNE FLAG ********************/
/***********************************************************/
SetImmuneFlagCsgo(Handle:cvar=INVALID_HANDLE)
{
	decl String:S_flags[4];
	new AdminFlag:C_flag;
	GetConVarString(cvar, S_flags, sizeof(S_flags));
	if (S_flags[0]!='\0' && FindFlagByChar(S_flags[0], C_flag))
	{
		 C_admin_flag = FlagToBit(C_flag);
	}
	else 
	{
		C_admin_flag = 0;
	}
}

/***********************************************************/
/*********************** CHECK IF AFK **********************/
/***********************************************************/
bool:CheckObserverAFKCsgo(client)
{
	new C_last_observer_target; 
	new C_last_observer_mode 	= C_observer_mode_csgo[client];
	C_observer_mode_csgo[client] 	= GetEntProp(client, Prop_Send, "m_iObserverMode");
	if(C_last_observer_mode > 0 && C_observer_mode_csgo[client] != C_last_observer_mode)
	{
		if(B_active_afk_manager_csgo_dev)
		{
			PrintToChat(client, "%sObserver mode changed: Last mode:%i, new mode: %i | Client is not AFK", TAG_AFK_MANAGER_CSGO, C_last_observer_mode, C_observer_mode_csgo);
		}
		return false;
	}

	decl Float:F_client_eye_location[3];
	F_client_eye_location = F_client_eye_position_csgo[client];
	
	if(C_observer_mode_csgo[client] == 6)
	{
		if(B_active_afk_manager_csgo_dev)
		{
			PrintToChat(client, "%sObserver mode: Free view", TAG_AFK_MANAGER_CSGO);
		}
		GetClientEyeAngles(client, F_client_eye_position_csgo[client]);
	}
	else
	{
		C_last_observer_target 		= C_observer_target_csgo[client];
		C_observer_target_csgo[client] 	= GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");

		if(C_last_observer_mode == 0 && C_last_observer_target == 0)
		{
			if(B_active_afk_manager_csgo_dev)
			{
				PrintToChat(client, "%sObserver mode: 0, observer target: 0 | Client is AFK", TAG_AFK_MANAGER_CSGO);
			}
			return true;
		}

		if(C_last_observer_target > 0 && C_observer_target_csgo[client] != C_last_observer_target)
		{
			if (C_last_observer_target > MaxClients || !IsClientConnected(C_last_observer_target) || !IsClientInGame(C_last_observer_target))
			{
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sObserver target: Old target:%i, new target:%i | Client is not AFK", TAG_AFK_MANAGER_CSGO, C_last_observer_target, C_observer_target_csgo);
				}
				return false;
			}
			if(B_active_afk_manager_csgo_dev)
			{
				PrintToChat(client, "%sObserver target: Target is not available anymore. | Client is not AFK", TAG_AFK_MANAGER_CSGO, C_last_observer_target, C_observer_target_csgo);
			}
			return (!IsPlayerAlive(C_last_observer_target));
		}
	}

	if((F_client_eye_position_csgo[client][0] == F_client_eye_location[0]) && (F_client_eye_position_csgo[client][1] == F_client_eye_location[1]) && (F_client_eye_position_csgo[client][2] == F_client_eye_location[2]))
	{
		if(B_active_afk_manager_csgo_dev)
		{
			PrintToChat(client, "%sClient look the same place. | Client is AFK", TAG_AFK_MANAGER_CSGO, C_last_observer_target, C_observer_target_csgo);
		}
		return true;
	}
	
	return false;
}

/***********************************************************/
/******************* TIMER CHECK PLAYER ********************/
/***********************************************************/
public Action:Timer_CheckPlayerAfkManagerCsgo(Handle:Timer, any:data)
{
	new client; 
	new clients = 0; 
	new C_team_num; 
	new Float:F_time_left;
	new bool:B_move_players = false;
	new bool:B_kick_players = false;
	
	decl Float:F_eye_location[3], Float:F_map_location[3];
	
	for (client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client) && !IsFakeClient(client))
		{
			B_move_players = (clients >= C_active_afk_manager_move_min_csgo && F_active_afk_manager_move_time_csgo > 0.0);
			B_kick_players = (clients >= C_active_afk_manager_kick_min_csgo && F_active_afk_manager_kick_time_csgo > 0.0);
			
			if (!B_move_players && !B_kick_players) //Do we have enough players?
			{
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sEnought player to move or kick", TAG_AFK_MANAGER_CSGO);
				}
				return Plugin_Continue;
			}
			clients++;
		}

		if (!B_client_is_in[client] || !IsClientInGame(client)) // Is this player actually in the game?
		{
			continue;
		}
		
		C_team_num = GetClientTeam(client);
		
		// Check for AFK
		if (IsClientObserver(client))
		{	
			// Unassigned, Spectator or Dead Player
			if (C_team_num > 1 && !IsPlayerAlive(client))
			{
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sPlayer is not a spectator", TAG_AFK_MANAGER_CSGO);
				}			
				continue; // Exclude dead players: player is not a spectator = he is dead
			}
			
			if (C_team_num == 0 || CheckObserverAFKCsgo(client))
			{
				F_client_afk_time_csgo[client] += AFK_CHECK_INTERVAL_CSGO;
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sAFK Time: %i", TAG_AFK_MANAGER_CSGO, F_client_afk_time_csgo[client]);
				}
			}
			else
			{
				F_client_afk_time_csgo[client] = 0.0;
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sAFK Time reset", TAG_AFK_MANAGER_CSGO);
				}
				continue;
			}
		}
		else
		{	// Normal player
			F_eye_location = F_client_eye_position_csgo[client]; // Store Previous Eye Angle/Origin & Map Location Values
			F_map_location = F_client_origin_position_csgo[client];
			GetClientEyeAngles(client, F_client_eye_position_csgo[client]);// Get New
			GetClientAbsOrigin(client, F_client_origin_position_csgo[client]);

			// Check Location (Origin) including thresholds && Check Eye Angles && Check if player is frozen
			if ((F_client_eye_position_csgo[client][0] == F_eye_location[0]) && 
				(F_client_eye_position_csgo[client][1] == F_eye_location[1]) &&
				(F_client_eye_position_csgo[client][2] == F_eye_location[2]) &&
				(FloatAbs(F_client_origin_position_csgo[client][0] - F_map_location[0]) < AFK_THRESHOLD_CSGO) &&
				(FloatAbs(F_client_origin_position_csgo[client][1] - F_map_location[1]) < AFK_THRESHOLD_CSGO) &&
				(FloatAbs(F_client_origin_position_csgo[client][2] - F_map_location[2]) < AFK_THRESHOLD_CSGO) &&
				!(GetEntityFlags(client) & FL_FROZEN))
			{
				F_client_afk_time_csgo[client] += AFK_CHECK_INTERVAL_CSGO;
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sAFK cause same location: %i", TAG_AFK_MANAGER_CSGO, F_client_afk_time_csgo[client]);
				}
			}
			else
			{
				F_client_afk_time_csgo[client] = 0.0;
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChat(client, "%sAFK Time reset cause location changed", TAG_AFK_MANAGER_CSGO);
				}
				continue;
			}
		}
		
		// Warn/Move/Kick client. If client isn't ab AFK, we will never be here
		if (B_move_players && C_team_num > 1 && ( !C_active_afk_manager_immune_csgo || C_active_afk_manager_immune_csgo == 2 || !CHECK_ADMIN_IMMUNITY(client)))
		{
			F_time_left = F_active_afk_manager_move_time_csgo - F_client_afk_time_csgo[client];
			if (F_time_left > 0.0)
			{
				if(F_time_left <= F_active_afk_manager_warn_time_csgo)
				{
					if(B_active_afk_manager_csgo_dev)
					{
						PrintToChat(client, "%sYou are AFK need to move you", TAG_AFK_MANAGER_CSGO);
					}
					CPrintToChat(client, "%t", "Move_Warning", RoundToFloor(F_time_left));
				}
			}
			else
			{
				decl String:S_client_name[MAX_NAME_LENGTH+4];
				Format(S_client_name,sizeof(S_client_name),"%N",client);
				
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChatAll("%sOne player move to spectator", TAG_AFK_MANAGER_CSGO);
				}

				CPrintToChatAll("%t", "Move_Announce", S_client_name);	
				
				new death = GetEntProp(client, Prop_Data, "m_iDeaths");
				new frags = GetEntProp(client, Prop_Data, "m_iFrags");
				FakeClientCommand(client, "jointeam 1");
				SetEntProp(client, Prop_Data, "m_iFrags", frags);
				SetEntProp(client, Prop_Data, "m_iDeaths", death);
			}
		}
		else if (B_kick_players && (!C_active_afk_manager_immune_csgo || C_active_afk_manager_immune_csgo == 3 || !CHECK_ADMIN_IMMUNITY(client)))
		{
			F_time_left = F_active_afk_manager_kick_time_csgo - F_client_afk_time_csgo[client];
			if (F_time_left > 0.0)
			{
				if (F_time_left <= F_active_afk_manager_warn_time_csgo)
				{
					if(B_active_afk_manager_csgo_dev)
					{
						PrintToChat(client, "%sYou are AFK need to kick you", TAG_AFK_MANAGER_CSGO);
					}
					CPrintToChat(client, "%t", "Kick_Warning", RoundToFloor(F_time_left));
				}
			}
			else
			{
				decl String:S_client_name[MAX_NAME_LENGTH+4];
				Format(S_client_name,sizeof(S_client_name),"%N",client);
				
				if(B_active_afk_manager_csgo_dev)
				{
					PrintToChatAll("%sOne player has been kicked", TAG_AFK_MANAGER_CSGO);
				}
				CPrintToChatAll("%t", "Kick_Announce", S_client_name);
				KickClient(client, "%t", "Kick_Message");
			}
		}
	}
	return Plugin_Continue;
}
/***********************************************************/
/******************* COMMANDE TRACK BOMB *******************/
/***********************************************************/
public Action:Command_AfkManagerCsgo(client, args) 
{
	new m_iObserverMode = GetEntProp(client, Prop_Send, "m_iObserverMode");
	new m_hObserverTarget = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
	new C_team_num = GetClientTeam(client);
	
	PrintToChat(client, "%sMode:%i, Taget:%i, Team:%i", TAG_AFK_MANAGER_CSGO, m_iObserverMode, m_hObserverTarget, C_team_num);
	CheckObserverAFKCsgo(client);
	return Plugin_Handled;
}

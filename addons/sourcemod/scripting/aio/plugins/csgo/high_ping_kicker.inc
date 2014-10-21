/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//**************************AIO MOD HIGH PING KICKER*************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_HIGH_PING_KICKER_CSGO "[AIO HIGH PING KICKER] - "
#define UMIN(%1,%2) (%1 < %2 ? %2 : %1)


//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_high_ping_kicker_csgo;
new Handle:cvar_active_high_ping_kicker_csgo_dev;

new Handle:cvar_active_high_ping_kicker_max_ping_csgo;
new Handle:cvar_active_high_ping_kicker_max_checks_csgo;
new Handle:cvar_active_high_ping_kicker_start_check_csgo;
new Handle:cvar_active_high_ping_kicker_admin_immune_csgo;

//Bool
new bool:B_active_high_ping_kicker_csgo 					= false;
new bool:B_active_high_ping_kicker_csgo_dev					= false;
new bool:B_active_high_ping_kicker_admin_immune_csgo		= false;

//Float
new Float:F_active_high_ping_kicker_start_check_csgo		= 0.0;

//Customs
new C_active_high_ping_kicker_max_ping_csgo;
new C_active_high_ping_kicker_max_checks_csgo;

new C_client_failed[MAXPLAYERS+1];
new C_client_ping[MAXPLAYERS+1];

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartHightPingKickerCsgo()
{
	CreateConVar("aio_high_ping_kicker_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_high_ping_kicker_csgo 					= CreateConVar("active_high_ping_kicker_csgo",  					"0", 					"Enable/Disable Sounds Kill", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_high_ping_kicker_csgo_dev				= CreateConVar("active_high_ping_kicker_csgo_dev", 					"0", 					"Enable/Disable Sounds Kill Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	cvar_active_high_ping_kicker_max_ping_csgo			= CreateConVar("active_high_ping_kicker_max_ping_csgo", 			"150", 					"Max ping allowed", 					DEFAULT_FLAGS);
	cvar_active_high_ping_kicker_max_checks_csgo		= CreateConVar("active_high_ping_kicker_max_checks_csgo", 			"10", 					"Number of max checks after kick", 		DEFAULT_FLAGS);
	cvar_active_high_ping_kicker_start_check_csgo		= CreateConVar("active_high_ping_kicker_start_check_csgo", 			"15.0", 				"Time after timer start", 				DEFAULT_FLAGS);
	cvar_active_high_ping_kicker_admin_immune_csgo		= CreateConVar("active_high_ping_kicker_admin_immune_csgo", 		"1", 					"Enable/Disable Admin immunity", 		DEFAULT_FLAGS);
	
	HookEventsHightPingKickerCsgo();
	
	ServerCommand("exec /aio/high_ping_kicker/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsHightPingKickerCsgo()
{
	HookConVarChange(cvar_active_high_ping_kicker_csgo, 				Event_CvarChangeHightPingKickerCsgo);
	HookConVarChange(cvar_active_high_ping_kicker_csgo_dev, 			Event_CvarChangeHightPingKickerCsgo);
	
	HookConVarChange(cvar_active_high_ping_kicker_max_ping_csgo, 		Event_CvarChangeHightPingKickerCsgo);
	HookConVarChange(cvar_active_high_ping_kicker_max_checks_csgo, 		Event_CvarChangeHightPingKickerCsgo);
	HookConVarChange(cvar_active_high_ping_kicker_start_check_csgo, 	Event_CvarChangeHightPingKickerCsgo);
	HookConVarChange(cvar_active_high_ping_kicker_admin_immune_csgo, 	Event_CvarChangeHightPingKickerCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeHightPingKickerCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateHightPingKickerCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateHightPingKickerCsgo()
{
	B_active_high_ping_kicker_csgo 					= GetConVarBool(cvar_active_high_ping_kicker_csgo);
	B_active_high_ping_kicker_csgo_dev 				= GetConVarBool(cvar_active_high_ping_kicker_csgo_dev);
	B_active_high_ping_kicker_admin_immune_csgo 	= GetConVarBool(cvar_active_high_ping_kicker_admin_immune_csgo);
	
	C_active_high_ping_kicker_max_ping_csgo 		= GetConVarInt(cvar_active_high_ping_kicker_max_ping_csgo);
	C_active_high_ping_kicker_max_checks_csgo		= GetConVarInt(cvar_active_high_ping_kicker_max_checks_csgo);
	
	F_active_high_ping_kicker_start_check_csgo		= GetConVarFloat(cvar_active_high_ping_kicker_start_check_csgo);
	
	if(B_active_high_ping_kicker_csgo)
	{
		CreateTimer(F_active_high_ping_kicker_start_check_csgo, Timer_CheckPingPlayerCsgo, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
		
		for(new i = 1; i < MaxClients; i++)
		{
			C_client_failed[i] 		= 0;
			C_client_ping[i] 		= 0;
		}
		
		if(B_active_high_ping_kicker_csgo_dev)
		{
			PrintToChatAll("%sTimer Started", TAG_HIGH_PING_KICKER_CSGO);
		}
	}
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedHightPingKickerCsgo()
{
	//UpdateStateHightPingKickerCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartHightPingKickerCsgo()
{
	UpdateStateHightPingKickerCsgo();
}

/***********************************************************/
/**************** WHEN CLIENT PUT IN SERVER ****************/
/***********************************************************/
public OnClientPutInServerHightPingKickerCsgo(client)
{
	if(B_active_high_ping_kicker_csgo)
	{
		C_client_failed[client] 		= 0;
		C_client_ping[client] 		= 0;
	}
}

/***********************************************************/
/******************** TIMER CHECK PING *********************/
/***********************************************************/
public Action:Timer_CheckPingPlayerCsgo(Handle:Timer)
{
	if(B_active_high_ping_kicker_csgo)
	{
		for(new i = 1; i < MaxClients; i++)
		{
			if(IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i) && !IsAdmin(i))
			{
				UpdatePingStatus(i);
			}
		}
		KickHighPingers();
	}
	return Plugin_Continue;
}

/***********************************************************/
/******************* UPDATE STATUS PING ********************/
/***********************************************************/
UpdatePingStatus(client)
{
	decl String:S_rate[32];
	GetClientInfo(client, "cl_cmdrate", S_rate, sizeof(S_rate));
	
	new Float:F_ping = GetClientAvgLatency(client, NetFlow_Outgoing);
	new Float:F_tick_rate = GetTickInterval();
	new C_cmd_rate = UMIN(StringToInt(S_rate), 20);

	F_ping -= ((0.5 / C_cmd_rate) + (F_tick_rate * 1.0));
	F_ping -= (F_tick_rate * 0.5);
	F_ping *= 1000.0;
	
	C_client_ping[client] = RoundToZero(F_ping);
	
	if(C_client_ping[client] > C_active_high_ping_kicker_max_ping_csgo)
	{
		C_client_failed[client]++;
		if(B_active_high_ping_kicker_csgo_dev)
		{
			PrintToChat(client, "%sYour ping: %i, check: %i", TAG_HIGH_PING_KICKER_CSGO, C_client_ping[client], C_client_failed[client]);
		}
	}
	else
	{
		if(C_client_failed[client] > 0)
		{
			C_client_failed[client]--;
			
			if(B_active_high_ping_kicker_csgo_dev)
			{
				PrintToChat(client, "%sCheck: %i", TAG_HIGH_PING_KICKER_CSGO, C_client_failed[client]);
			}
		}
	}
}

/***********************************************************/
/***************** KICK PLAYER HIGH PING *******************/
/***********************************************************/
KickHighPingers()
{
	for(new i = 1; i < MaxClients; i++)
	{
		if(IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i))
		{
			if(C_client_failed[i] >= C_active_high_ping_kicker_max_checks_csgo)
			{
				decl String:S_client_name[MAX_NAME_LENGTH];
				GetClientName(i, S_client_name, MAX_NAME_LENGTH);
				
				if(!B_active_high_ping_kicker_csgo_dev)
				{
					KickClient(i, "%t", "High Ping Kicker Kick me", C_client_ping[i], C_active_high_ping_kicker_max_ping_csgo);
				}
				
				CPrintToChatAll("%t", "High Ping Kicker Kick", S_client_name, C_client_ping[i], C_active_high_ping_kicker_max_ping_csgo);
				
				if(B_active_high_ping_kicker_csgo_dev)
				{
					PrintToChat(i, "%sYou should be kick for your high ping", TAG_HIGH_PING_KICKER_CSGO);
				}
			}
		}
	}
}

/***********************************************************/
/****************** CHECK IF IS AN ADMIN *******************/
/***********************************************************/
bool:IsAdmin(client)
{
    if(!B_active_high_ping_kicker_admin_immune_csgo)
	{
        return false;
	}

    new AdminId:admin = GetUserAdmin(client);

    if(admin == INVALID_ADMIN_ID)
	{
        return false;
	}

    return true;
}
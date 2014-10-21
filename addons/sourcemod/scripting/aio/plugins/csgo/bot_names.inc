/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//*****************************AIO MOD BOT NAMES*****************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_BOT_NAMES_CSGO 	"[AIO BOT NAMES] - "
#define BOT_NAMES_CSGO		64


//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_bot_names_csgo;
new Handle:cvar_active_bot_names_csgo_dev;

new Handle:cvar_active_bot_names_clan_csgo;
new Handle:cvar_active_bot_names_min_ping_csgo;
new Handle:cvar_active_bot_names_max_ping_csgo;
new Handle:cvar_active_bot_names_interval_ping_csgo;

//Bool
new bool:B_active_bot_names_csgo 					= false;
new bool:B_active_bot_names_csgo_dev				= false;

//String
new String:S_active_bot_names_clan_csgo[MAX_NAME_LENGTH];
new String:S_active_bot_names_name_csgo[BOT_NAMES_CSGO][MAX_NAME_LENGTH];

//Float
new Float:F_timer_interval							= 0.0;

//Customs
new C_active_bot_names_min_ping_csgo				= 0;
new C_active_bot_names_max_ping_csgo				= 0;
new C_active_bot_names_interval_ping_csgo			= 0;

new UserMsg:TextMsgBotNamesCsgo;
new UserMsg:SayTextBotNamesCsgo; 
new UserMsg:SayText2BotNamesCsgo;
//new UserMsg:RadioTextBotNamesCsgo;

new total_bot_name_csgo								= 0;
new C_cs_player_manager								= 0;
new C_ping											= -1;
new C_max_clients									= 0;
/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartBotNamesCsgo()
{
	LoadBotNamesCsgo();
	CreateConVar("aio_bot_names_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_bot_names_csgo 						= CreateConVar("active_bot_names_csgo",  				"0", 					"Enable/Disable Bot Names", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_bot_names_csgo_dev					= CreateConVar("active_bot_names_csgo_dev", 			"0", 					"Enable/Disable Bot Names Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_bot_names_clan_csgo					= CreateConVar("active_bot_names_clan_csgo", 			"AIO", 					"Clan name", 							DEFAULT_FLAGS);
	cvar_active_bot_names_min_ping_csgo				= CreateConVar("active_bot_names_min_ping_csgo", 		"35", 					"Fake ping bots min", 					DEFAULT_FLAGS);
	cvar_active_bot_names_max_ping_csgo				= CreateConVar("active_bot_names_max_ping_csgo", 		"55", 					"Fake ping bots max", 					DEFAULT_FLAGS);
	cvar_active_bot_names_interval_ping_csgo		= CreateConVar("active_bot_names_interval_ping_csgo", 	"3", 					"Fake ping bots interval", 				DEFAULT_FLAGS);
	
	HookEventsBotNamesCsgo();
	
	TextMsgBotNamesCsgo  	= GetUserMessageId("TextMsg");
	SayTextBotNamesCsgo  	= GetUserMessageId("SayText");
	SayText2BotNamesCsgo 	= GetUserMessageId("SayText2");
	//RadioTextBotNamesCsgo 	= GetUserMessageId("RadioText");
	HookUserMessage(TextMsgBotNamesCsgo,  	UserMessagesHookBotNamesCsgo, true);
	HookUserMessage(SayTextBotNamesCsgo,  	UserMessagesHookBotNamesCsgo, true);
	HookUserMessage(SayText2BotNamesCsgo, 	UserMessagesHookBotNamesCsgo, true);
	//HookUserMessage(RadioTextBotNamesCsgo, 	UserMessagesHookBotNamesCsgo, true);
	
	C_ping	= FindSendPropOffs("CPlayerResource", "m_iPing");
	
	ServerCommand("exec /aio/bot_names/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsBotNamesCsgo()
{
	HookConVarChange(cvar_active_bot_names_csgo, 				Event_CvarChangeBotNamesCsgo);
	HookConVarChange(cvar_active_bot_names_csgo_dev, 			Event_CvarChangeBotNamesCsgo);
	
	HookConVarChange(cvar_active_bot_names_clan_csgo, 			Event_CvarChangeBotNamesCsgo);
	HookConVarChange(cvar_active_bot_names_min_ping_csgo, 		Event_CvarChangeBotNamesCsgo);
	HookConVarChange(cvar_active_bot_names_max_ping_csgo, 		Event_CvarChangeBotNamesCsgo);
	HookConVarChange(cvar_active_bot_names_interval_ping_csgo, 	Event_CvarChangeBotNamesCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeBotNamesCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateBotNamesCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateBotNamesCsgo()
{
	B_active_bot_names_csgo 					= GetConVarBool(cvar_active_bot_names_csgo);
	B_active_bot_names_csgo_dev 				= GetConVarBool(cvar_active_bot_names_csgo_dev);
	
	C_active_bot_names_min_ping_csgo			= GetConVarInt(cvar_active_bot_names_min_ping_csgo);
	C_active_bot_names_max_ping_csgo			= GetConVarInt(cvar_active_bot_names_max_ping_csgo);
	C_active_bot_names_interval_ping_csgo		= GetConVarInt(cvar_active_bot_names_interval_ping_csgo);
	
	GetConVarString(cvar_active_bot_names_clan_csgo, S_active_bot_names_clan_csgo, sizeof(S_active_bot_names_clan_csgo));
	
	C_max_clients 		= GetMaxClients();
	C_cs_player_manager = FindEntityByClassname(C_max_clients + 1, "cs_player_manager");
	F_timer_interval	= 0.0;
	
	LoadBotNamesCsgo();
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedBotNamesCsgo()
{
	//UpdateStateBotNamesCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartBotNamesCsgo()
{
	UpdateStateBotNamesCsgo();
}

/***********************************************************/
/**************** WHEN CLIENT PUT IN SERVER ****************/
/***********************************************************/
public OnClientPutInServerBotNamesCsgo(client)
{
	if(B_active_bot_names_csgo)
	{
		if(IsClientValid(client) && IsFakeClient(client))
		{
			
			CS_SetClientClanTag(client, S_active_bot_names_clan_csgo);
	
			new userid = GetClientUserId(client);
			new id = GetClientOfUserId(userid);

			if(strlen(S_active_bot_names_name_csgo[id]))
			{
				SetClientInfo(client, "name", S_active_bot_names_name_csgo[id]);
			}
			
			if(B_active_bot_names_csgo_dev)
			{
				LogMessage("%sTotal names: %i, Userid:%i", TAG_BOT_NAMES_CSGO, total_bot_name_csgo, id);
			}
		}
	}
}

/***********************************************************/
/********************** ON GAME FRAME **********************/
/***********************************************************/
public OnGameFrame()
{
	if(B_active_bot_names_csgo)
	{
		if(F_timer_interval < GetGameTime() - C_active_bot_names_interval_ping_csgo)
		{
			F_timer_interval = GetGameTime();
			
			if(C_cs_player_manager == -1 || C_ping == -1)
			{
				return;
			}

			for(new i = 1; i <= C_max_clients; i++)
			{
				if(!IsValidEdict(i) || !IsClientInGame(i) || !IsFakeClient(i))
				{
					continue;
				}

				SetEntData(C_cs_player_manager, C_ping + (i * 4), GetRandomInt(C_active_bot_names_min_ping_csgo, C_active_bot_names_max_ping_csgo));
			}
		}
	}
}

/***********************************************************/
/******************* WHEN PLAYER MESSAGE *******************/
/***********************************************************/
public Action:UserMessagesHookBotNamesCsgo(UserMsg:msg_id, Handle:msg, const players[], playersNum, bool:reliable, bool:init)
{
	if(B_active_bot_names_csgo)
	{
		if(IsClientValid(playersNum) && IsFakeClient(playersNum))
		{
			return Plugin_Handled;
		}
	}
	return Plugin_Continue;
}

/***********************************************************/
/**************** LOAD FILE SETTING BOT NAMES **************/
/***********************************************************/
public LoadBotNamesCsgo()
{
	new String:hc[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, hc, sizeof(hc), "configs/aio/bot_names/settings.cfg");
	
	new Handle:kv = CreateKeyValues("BotNames");
	FileToKeyValues(kv, hc);
	
	if(KvGotoFirstSubKey(kv))
	{
		do
		{
			for(new i=1;i<MaxClients;++i)
			{
				decl String:key[64];
				IntToString(i, key, 64);
				KvGetString(kv, key, S_active_bot_names_name_csgo[i], MAX_NAME_LENGTH);
				
				if(strlen(S_active_bot_names_name_csgo[i]))
				{
					total_bot_name_csgo = i;
					if(B_active_bot_names_csgo_dev)
					{
						LogMessage("%sBot names: %s", TAG_BOT_NAMES_CSGO, S_active_bot_names_name_csgo[i]);
					}
				}
			}
		}
		while (KvGotoNextKey(kv));
	}
	CloseHandle(kv);
}
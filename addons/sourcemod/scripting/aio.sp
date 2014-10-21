/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//**********************************AIO MOD**********************************//
//***************************************************************************//
//***************************************************************************//

#pragma semicolon 1

//***********************************//
//*************DEFINES***************//
//***********************************//

#define PLUGIN_VERSION "1.0.0"
#define TAG_AIO "[AIO] - "
#define CVARS FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY
#define DEFAULT_FLAGS FCVAR_PLUGIN|FCVAR_NOTIFY

//***********************************//
//*************INCLUDE***************//
//***********************************//

//Include native
#include <sourcemod>
#include <sdktools>
#include <sdktools_functions>
#include <sdkhooks>
#include <cstrike>
#include <clientprefs>


//Include mode
#include "aio/smlib"
#include "aio/colors/colors"
#include "aio/enums/enums"

#include "aio/cookies/init"

#include "aio/cookies/csgo/general"
#include "aio/cookies/csgo/track_bomb"
#include "aio/cookies/csgo/show_damage"
#include "aio/cookies/csgo/sounds_kill"

#include "aio/menus/init"

#include "aio/menus/csgo/general"
#include "aio/menus/csgo/track_bomb"
#include "aio/menus/csgo/show_damage"
#include "aio/menus/csgo/sounds_kill"


#include "aio/events/events"
#include "aio/clients/clients"
#include "aio/objectifs/objectifs"

#include "aio/weapons/weapons"
#include "aio/weapons/weapons_csgo"

#include "aio/plugins/csgo/start_weapons"
#include "aio/plugins/csgo/only_hs"
#include "aio/plugins/csgo/track_bomb"
#include "aio/plugins/csgo/win_or_die"
#include "aio/plugins/csgo/show_damage"
#include "aio/plugins/csgo/sounds_kill"
#include "aio/plugins/csgo/afk_manager"
#include "aio/plugins/csgo/bot_names"
#include "aio/plugins/csgo/high_ping_kicker"

//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active;
new Handle:cvar_active_dev;

//String
new String:ENGINE_VERSION[256];

//Bool
new bool:B_active 								= false;
new bool:B_dev 									= false;

//Customs/Others
new EngineVersion:C_engine_version;

													
//Informations plugin
public Plugin:myinfo =
{
	name = "AIO MOD",
	author = "De Battista http://doyou.watch",
	description = "AIO MOD by DoYou.Watch",
	version = PLUGIN_VERSION,
	url = "http://doyou.watch"
}


/***********************************************************/
/******************** BEFORE PLUGIN START ******************/
/***********************************************************/
public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
	AskPluginLoad2CookieInit(myself, late, error, err_max);
	return APLRes_Success;
}

/**********************************************************************************************************************/
/************************************************** EVENTS SOURCEMOD **************************************************/
/**********************************************************************************************************************/


/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStart()
{
	//Chargement traduction
	LoadTranslations("aio.phrases");
	
	//Version
	CreateConVar("aio_version", PLUGIN_VERSION, "Version", CVARS);

	//Cvars
	cvar_active			 		= CreateConVar("active", "1", 				"Enable/Disable Mod", 				DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_dev				= CreateConVar("active_dev", "0", 			"Enable/Disable Dev Mod", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	DetectEngine();
	HookEvents();
	
	//Creation du CFG
	//AutoExecConfig(true, "settings", "aio");
	
	CreateTimer(30.0, Timer_WelcomeMsgChat, INVALID_HANDLE);
	CreateTimer(300.0, Timer_WelcomeMsgChat, INVALID_HANDLE, TIMER_REPEAT);
	
	ServerCommand("exec /aio/config.cfg");
}

/***********************************************************/
/*********************** DETECT ENGINE *********************/
/***********************************************************/
DetectEngine()
{
	//#include "aio/objectifs/objectifs"
	OnPluginStartObjectifs();
	//#include "aio/events/events"
	OnPluginStartEvents();
	//#include "aio/weapons/weapons"
	OnPluginStartWeapons();
	
	
	C_engine_version = GetEngineVersion();
	
	switch (C_engine_version)
	{
		//If Engine Unknown
		case Engine_Unknown:
		{
			ENGINE_VERSION = "UNKNOWN";			
		}
 
		//OnPluginStart For CSGO
		case Engine_CSGO:
		{
			ENGINE_VERSION = "CS:GO";
			
			//#include "aio/plugins/csgo/start_weapons"
			OnPluginStartStartWeaponCsgo();
			
			//#include "aio/plugins/csgo/only_hs"
			OnPluginStartOnlyHsCsgo();
			
			//#include "aio/plugins/csgo/track_bomb"
			OnPluginStartTrackBombCsgo();
			
			//#include "aio/plugins/csgo/win_or_die"
			OnPluginStartWODCsgo();
			
			//#include "aio/plugins/csgo/show_damage"
			OnPluginStartShowDamageCsgo();
			
			//#include "aio/plugins/csgo/sounds_kill"
			OnPluginStartSoundsKillCsgo();
			
			//#include "aio/plugins/csgo/afk_manager"
			OnPluginStartAfkManagerCsgo();
			
			//#include "aio/plugins/csgo/bot_names"
			OnPluginStartBotNamesCsgo();
			
			//#include "aio/plugins/csgo/high_ping_kicker"
			OnPluginStartHightPingKickerCsgo();
		}
	}

	/* Cookies and menus should be loaded in last position */
	//#include "aio/cookies/init"
	OnPluginStartCookieInit();
	//#include "aio/menus/init"
	OnPluginStartMenuInit();
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEvents()
{
	HookConVarChange(cvar_active, Event_CvarChange);
	HookConVarChange(cvar_active_dev, Event_CvarChange);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChange(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateState();
	
	//#include "aio/objectifs/objectifs"
	Event_CvarChangeObjectifs(cvar, oldValue, newValue);
			
	switch (C_engine_version)
	{
		case Engine_CSGO:
		{
			//#include "aio/plugins/csgo/start_weapons"
			Event_CvarChangeStartWeaponCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/only_hs"
			Event_CvarChangeOnlyHsCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/track_bomb"
			Event_CvarChangeTrackBombCsgo(cvar, oldValue, newValue);

			//#include "aio/plugins/csgo/win_or_die"
			Event_CvarChangeWODCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/show_damage"
			Event_CvarChangeShowDamageCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/sounds_kill"
			Event_CvarChangeSoundsKillCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/afk_manager"
			Event_CvarChangeAfkManagerCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/bot_names"
			Event_CvarChangeBotNamesCsgo(cvar, oldValue, newValue);
			
			//#include "aio/plugins/csgo/high_ping_kicker"
			Event_CvarChangeHightPingKickerCsgo(cvar, oldValue, newValue);
		}
	}
	
	//#include "aio/cookies/init"
	Event_CvarChangeCookieInit(cvar, oldValue, newValue);
	
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateState()
{
	B_active 	= GetConVarBool(cvar_active);
	B_dev 		= GetConVarBool(cvar_active_dev);
}
/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecuted()
{
	UpdateState();
	if(B_active)
	{
		//#include "aio/objectifs/objectifs"
		OnConfigsExecutedObjectifs();
				
		switch (C_engine_version)
		{
			case Engine_CSGO:
			{
				//#include "aio/plugins/csgo/start_weapons"
				OnConfigsExecutedStartWeaponCsgo();
				
				//#include "aio/plugins/csgo/only_hs"
				OnConfigsExecutedOnlyHsCsgo();
				
				//#include "aio/plugins/csgo/track_bomb"
				OnConfigsExecutedTrackBombCsgo();
				
				//#include "aio/plugins/csgo/win_or_die"
				OnConfigsExecutedWODCsgo();
				
				//#include "aio/plugins/csgo/show_damage"
				OnConfigsExecutedShowDamageCsgo();
				
				//#include "aio/plugins/csgo/sounds_kill"
				OnConfigsExecutedSoundsKillCsgo();
				
				//#include "aio/plugins/csgo/afk_manager"
				OnConfigsExecutedAfkManagerCsgo();
				
				//#include "aio/plugins/csgo/bot_names"
				OnConfigsExecutedBotNamesCsgo();
				
				//#include "aio/plugins/csgo/high_ping_kicker"
				OnConfigsExecutedHightPingKickerCsgo();
			}
		}
		
		//#include "aio/cookies/init"
		OnConfigsExecutedCookieInit();
	}
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStart()
{
	UpdateState();
	if(B_active)
	{
		if(B_dev)
		{
			LogMessage("%sEngine is %s", TAG_AIO, ENGINE_VERSION);
		}
		
		//#include "aio/objectifs/objectifs"
		OnMapStartObjectifs();

		switch (C_engine_version)
		{
			case Engine_CSGO:
			{
				//#include "aio/plugins/csgo/start_weapons"
				OnMapStartStartWeaponCsgo();
				
				//#include "aio/plugins/csgo/only_hs"
				OnMapStartOnlyHsCsgo();
				
				//#include "aio/plugins/csgo/track_bomb"
				OnMapStartTrackBombCsgo();
				
				//#include "aio/plugins/csgo/win_or_die"
				OnMapStartWODCsgo();
				
				//#include "aio/plugins/csgo/show_damage"
				OnMapStartShowDamageCsgo();
				
				//#include "aio/plugins/csgo/sounds_kill"
				OnMapStartSoundsKillCsgo();
				
				//#include "aio/plugins/csgo/afk_manager"
				OnMapStartAfkManagerCsgo();
				
				//#include "aio/plugins/csgo/bot_names"
				OnMapStartBotNamesCsgo();
				
				//#include "aio/plugins/csgo/high_ping_kicker"
				OnMapStartHightPingKickerCsgo();
			}
		}
		//#include "aio/cookies/init"
		OnMapStartCookieInit();
	}
}

/***********************************************************/
/********************** WHEN MAP END ***********************/
/***********************************************************/
public OnMapEnd()
{
	if(B_active)
	{
		switch (C_engine_version)
		{
			case Engine_CSGO:
			{
				//#include "aio/plugins/csgo/track_bomb"
				OnMapEndTrackBombCsgo();
			}
		}
	}
}

/**********************************************************************************************************************/
/*************************************************** EVENTS CLIENT ****************************************************/
/**********************************************************************************************************************/

/***********************************************************/
/**************** WHEN CLIENT PUT IN SERVER ****************/
/***********************************************************/
public OnClientPutInServer(client)
{
	if(B_active)
	{
		//#include "aio/cookies/init"
		OnClientPutInServerCookieInit(client);
		
		switch (C_engine_version)
		{
			case Engine_CSGO:
			{
				/*Welcome message and copyright */
				CreateTimer(30.0, Timer_WelcomeMsgHint, client);
				
				//#include "aio/plugins/csgo/only_hs"
				SDKHook(client, SDKHook_OnTakeDamage, OnTakeDamageOnlyHsCsgo);
				
				//#include "aio/plugins/csgo/track_bomb"
				SDKHook(client, SDKHook_TraceAttack, OnTracekAttackTrackBombCsgo);
				
				//#include "aio/plugins/csgo/bot_names"
				OnClientPutInServerBotNamesCsgo(client);
				
				//#include "aio/plugins/csgo/high_ping_kicker"
				OnClientPutInServerHightPingKickerCsgo(client);
			}
		}
	}
} 

/***********************************************************/
/************ WHEN CLIENT CONNECT WITH CHECKING ************/
/***********************************************************/
public OnClientPostAdminCheck(client)
{
	if(B_active)
	{
		switch (C_engine_version)
		{
			case Engine_CSGO:
			{
				//#include "aio/plugins/csgo/afk_manager"
				OnClientPostAdminCheckAfkManagerCsgo(client);
			}
		}
	}
}

/***********************************************************/
/******************** WHEN COOKIE CACHED *******************/
/***********************************************************/
public OnClientCookiesCached(client)
{
	if(B_active)
	{
		//#include "aio/cookies/init"
		OnClientCookiesCachedCookieInit(client);
	}
}

/***********************************************************/
/****************** WHEN CLIENT DISCONNECT *****************/
/***********************************************************/
public OnClientDisconnect(client)
{
	if(B_active)
	{
		switch (C_engine_version)
		{
			case Engine_CSGO:
			{
				//#include "aio/plugins/csgo/afk_manager"
				OnClientDisconnectAfkManagerCsgo(client);
			}
		}
	}
}

/**********************************************************************************************************************/
/*************************************************** EVENTS CUSTOMS ***************************************************/
/**********************************************************************************************************************/

/***********************************************************/
/*********************** WELCOME MSG ***********************/
/***********************************************************/
public Action:Timer_WelcomeMsgChat(Handle:timer,any:client)
{
	CPrintToChatAll("%t", "Copyright Msg Chat");
	CPrintToChatAll("%t", "Copyright Msg Chat2");
	return Plugin_Stop;
}

public Action:Timer_WelcomeMsgHint(Handle:timer, any:client)
{
	if (IsClientInGame(client) && IsPlayerAlive(client))
	{
		PrintHintText(client, "%t", "Copyright Msg Hint");
	}
	return Plugin_Stop;
}
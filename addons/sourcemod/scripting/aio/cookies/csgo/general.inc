/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//***************************AIO MOD COOKIE GENERAL**************************//
//***************************************************************************//
//***************************************************************************//


/***********************************************************/
/******************** BEFORE PLUGIN START ******************/
/***********************************************************/
public APLRes:AskPluginLoad2CookieGeneralCsgo(Handle:myself, bool:late, String:error[], err_max)
{
	AskPluginLoad2CookieTrackBombCsgo(myself, late, error, err_max);
	AskPluginLoad2CookieShowDamageCsgo(myself, late, error, err_max);
	AskPluginLoad2CookieSoundsKillCsgo(myself, late, error, err_max);
}

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartCookieGeneralCsgo()
{
	//#include "aio/cookies/csgo/track_bomb"
	OnPluginStartCookieTrackBombCsgo();
	
	//#include "aio/cookies/csgo/show_damage"
	OnPluginStartCookieShowDamageCsgo();
	
	//#include "aio/cookies/csgo/sounds_kill"
	OnPluginStartCookieSoundsKillCsgo();
	
	SetCookieMenuItem(CookieGeneralPrefMenuGeneralCsgo, 0, "AIO MOD");
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeCookieGeneralCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	//#include "aio/cookies/csgo/track_bomb"
	Event_CvarChangeCookieTrackBombCsgo(cvar, oldValue, newValue);
	
	//#include "aio/cookies/csgo/show_damage"
	Event_CvarChangeCookieShowDamageCsgo(cvar, oldValue, newValue);
	
	//#include "aio/cookies/csgo/sounds_kill"
	Event_CvarChangeCookieSoundsKillCsgo(cvar, oldValue, newValue);
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedCookieGeneralCsgo()
{
	//#include "aio/cookies/csgo/track_bomb"
	OnConfigsExecutedCookieTrackBombCsgo();
	
	//#include "aio/cookies/csgo/show_damage"
	OnConfigsExecutedCookieShowDamageCsgo();
	
	//#include "aio/cookies/csgo/sounds_kill"
	OnConfigsExecutedCookieSoundsKillCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartCookieGeneralCsgo()
{
	//#include "aio/cookies/csgo/track_bomb"
	OnMapStartCookieTrackBombCsgo();
	
	//#include "aio/cookies/csgo/show_damage"
	OnMapStartCookieShowDamageCsgo();
	
	//#include "aio/cookies/csgo/sounds_kill"
	OnMapStartCookieSoundsKillCsgo();
}

/***********************************************************/
/**************** WHEN CLIENT PUT IN SERVER ****************/
/***********************************************************/
public OnClientPutInServerCookieGeneralCsgo(client)
{
	//#include "aio/cookies/csgo/track_bomb"
	OnClientPutInServerCookieTrackBombCsgo(client);
	
	//#include "aio/cookies/csgo/show_damage"
	OnClientPutInServerCookieShowDamageCsgo(client);
	
	//#include "aio/cookies/csgo/sounds_kill"
	OnClientPutInServerCookieSoundsKillCsgo(client);
}
/***********************************************************/
/******************** WHEN COOKIE CACHED *******************/
/***********************************************************/
public OnClientCookiesCachedCookieGeneralCsgo(client)
{
	//#include "aio/cookies/csgo/track_bomb"
	OnClientCookiesCachedTrackBombCsgo(client);
	
	//#include "aio/cookies/csgo/show_damage"
	OnClientCookiesCachedShowDamageCsgo(client);
	
	//#include "aio/cookies/csgo/sounds_kill"
	OnClientCookiesCachedSoundsKillCsgo(client);
}

/***********************************************************/
/******************* COMMANDE !SETTINGS ********************/
/***********************************************************/
public CookieGeneralPrefMenuGeneralCsgo(client,CookieMenuAction:action,any:info,String:buffer[],maxlen)
{
	if(action == CookieMenuAction_SelectOption)
	{
		BuildMenuGeneralCsgo(client);
	}
}

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

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_SOUNDS_KILL_CSGO "[AIO SOUNDS KILL] - "


//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_sounds_kill_csgo;
new Handle:cvar_active_sounds_kill_csgo_dev;

//Bool
new bool:B_active_sounds_kill_csgo 					= false;
new bool:B_active_sounds_kill_csgo_dev				= false;

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartSoundsKillCsgo()
{
	CreateConVar("aio_sounds_kill_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_sounds_kill_csgo 					= CreateConVar("active_sounds_kill_csgo",  				"0", 					"Enable/Disable Sounds Kill", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_sounds_kill_csgo_dev				= CreateConVar("active_sounds_kill_csgo_dev", 			"0", 					"Enable/Disable Sounds Kill Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	HookEventsSoundsKillCsgo();
	
	ServerCommand("exec /aio/..../config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsSoundsKillCsgo()
{
	HookConVarChange(cvar_active_sounds_kill_csgo, 				Event_CvarChangeSoundsKillCsgo);
	HookConVarChange(cvar_active_sounds_kill_csgo_dev, 			Event_CvarChangeSoundsKillCsgo);
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
	B_active_sounds_kill_csgo 			= GetConVarBool(cvar_active_sounds_kill_csgo);
	B_active_sounds_kill_csgo_dev 		= GetConVarBool(cvar_active_sounds_kill_csgo_dev);
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

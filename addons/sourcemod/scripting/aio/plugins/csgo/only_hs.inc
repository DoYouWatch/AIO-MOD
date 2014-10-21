/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//******************************AIO MOD ONLY HS******************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_ONLY_HS_CSGO "[AIO ONLY HS] - "


//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_only_hs_csgo;
new Handle:cvar_active_only_hs_csgo_dev;

//Bool
new bool:B_active_only_hs_csgo 					= false;
new bool:B_active_only_hs_csgo_dev				= false;

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartOnlyHsCsgo()
{
	CreateConVar("aio_only_hs_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_only_hs_csgo 					= CreateConVar("active_only_hs_csgo",  				"0", 					"Enable/Disable Only Hs", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_only_hs_csgo_dev				= CreateConVar("active_only_hs_csgo_dev", 			"0", 					"Enable/Disable Only Hs Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	HookEventsOnlyHsCsgo();
	
	for(new i = 1; i <= MaxClients; i++)
	{
		if(IsClientValid(i))
		{
			SDKHook(i, SDKHook_OnTakeDamage, OnTakeDamageOnlyHsCsgo);
		}
	}
	
	ServerCommand("exec /aio/only_hs/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsOnlyHsCsgo()
{
	HookConVarChange(cvar_active_only_hs_csgo, 				Event_CvarChangeOnlyHsCsgo);
	HookConVarChange(cvar_active_only_hs_csgo_dev, 			Event_CvarChangeOnlyHsCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeOnlyHsCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateOnlyHsCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateOnlyHsCsgo()
{
	B_active_only_hs_csgo 			= GetConVarBool(cvar_active_only_hs_csgo);
	B_active_only_hs_csgo_dev 		= GetConVarBool(cvar_active_only_hs_csgo_dev);
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedOnlyHsCsgo()
{
	//UpdateStateOnlyHsCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartOnlyHsCsgo()
{
	UpdateStateOnlyHsCsgo();
}

/***********************************************************/
/***************** WHEN PLAYER TAKE DAMAGE *****************/
/***********************************************************/
public Action:OnTakeDamageOnlyHsCsgo(victim, &attacker, &inflictor, &Float:damage, &damagetype, &weapon, Float:damageForce[3], Float:damagePosition[3])
{
	if(B_active_only_hs_csgo)
	{
		if(B_active_only_hs_csgo_dev)
		{
			PrintToChat(attacker, "%sDamage type :%i", TAG_ONLY_HS_CSGO, damagetype);
		}
		
		if(damagetype == DMG_SHOT_CSGO)
		{
			if(GetClientTeam(victim) != GetClientTeam(attacker))
			{
				damage = 0.0;
				if(B_active_only_hs_csgo_dev)
				{
					PrintToChat(attacker, "%sYou need to HS to kill him", TAG_ONLY_HS_CSGO);
				}
				return Plugin_Changed;
			}
		}
	}
	return Plugin_Continue;
}
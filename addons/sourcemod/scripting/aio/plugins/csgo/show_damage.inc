/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//****************************AIO MOD SHOW DAMAGE****************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_SHOW_DAMAGE_CSGO "[AIO SHOW DAMAGE] - "


//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_show_damage_csgo;
new Handle:cvar_active_show_damage_csgo_dev;

//Bool
new bool:B_active_show_damage_csgo 					= false;
new bool:B_active_show_damage_csgo_dev				= false;

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartShowDamageCsgo()
{
	CreateConVar("aio_show_damage_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_show_damage_csgo 					= CreateConVar("active_show_damage_csgo",  				"0", 					"Enable/Disable Show Damage", 		DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_show_damage_csgo_dev				= CreateConVar("active_show_damage_csgo_dev", 			"0", 					"Enable/Disable Show Damage Dev", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	HookEventsShowDamageCsgo();
	
	ServerCommand("exec /aio/show_damage/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsShowDamageCsgo()
{
	HookConVarChange(cvar_active_show_damage_csgo, 				Event_CvarChangeShowDamageCsgo);
	HookConVarChange(cvar_active_show_damage_csgo_dev, 			Event_CvarChangeShowDamageCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeShowDamageCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateShowDamageCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateShowDamageCsgo()
{
	B_active_show_damage_csgo 			= GetConVarBool(cvar_active_show_damage_csgo);
	B_active_show_damage_csgo_dev 		= GetConVarBool(cvar_active_show_damage_csgo_dev);
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedShowDamageCsgo()
{
	//UpdateStateShowDamageCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartShowDamageCsgo()
{
	UpdateStateShowDamageCsgo();
}

/***********************************************************/
/******************** WHEN PLAYER HURTED *******************/
/***********************************************************/
public Action:Event_PlayerHurtShowDamageCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_show_damage_csgo)
	{
		new victim 			= GetClientOfUserId(GetEventInt(event, "userid"));
		new attacker 		= GetClientOfUserId(GetEventInt(event, "attacker"));
		new damage_health 	= GetEventInt(event, "dmg_health");	
		new damage_armor 	= GetEventInt(event, "dmg_armor");
		new hitgroup		= GetEventInt(event, "hitgroup");
		decl String:S_hitgroup_message[256];
		
		if(IsClientValid(attacker) && C_show_damage_csgo[attacker] && IsClientValid(victim))
		{
			//PrintToChat(attacker, "cookie:%i", C_show_damage_csgo[attacker]);
			/* hitgroup 0 = generic */
			/* hitgroup 1 = head */
			/* hitgroup 2 = chest */
			/* hitgroup 3 = stomach */
			/* hitgroup 4 = left arm */
			/* hitgroup 5 = right arm */
			/* hitgroup 6 = left leg */
			/* hitgroup 7 = right leg */

			switch(hitgroup)
			{
				case 0:
				{
					S_hitgroup_message = "";
				}
				case 1:
				{
					//S_hitgroup_message = "Head";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Head", attacker);
				}
				case 2:
				{
					//S_hitgroup_message = "Chest";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Chest", attacker);
				}
				case 3:
				{
					//S_hitgroup_message = "Stomach";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Stomach", attacker);
				}
				case 4:
				{
					//S_hitgroup_message = "Left arm";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Left arm", attacker);
				}
				case 5:
				{
					//S_hitgroup_message = "Right arm";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Right arm", attacker);
				}
				case 6:
				{
					//S_hitgroup_message = "Left leg";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Left leg", attacker);
				}
				case 7:
				{
					//S_hitgroup_message = "Right leg";
					Format(S_hitgroup_message, sizeof(S_hitgroup_message), "%T", "Right leg", attacker);
				}
			}
			
			if(attacker != victim && GetClientTeam(attacker) != GetClientTeam(victim))
			{
				if(strlen(S_hitgroup_message))
				{
					PrintCenterText(attacker, "%t", "Show damage hit message body", S_hitgroup_message, damage_health, damage_armor);
				}
				else
				{
					PrintCenterText(attacker, "%t", "Show damage hit message", damage_health, damage_armor);
				}
			}
			if(B_active_show_damage_csgo_dev)
			{
				PrintToChat(attacker, "%shealth:%i, armor:%i, hitgroup:%i", TAG_SHOW_DAMAGE_CSGO, damage_health, damage_armor, hitgroup);
			}
		}
	}
}
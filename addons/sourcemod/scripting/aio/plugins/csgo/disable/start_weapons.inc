/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//***************************AIO MOD START WEAPONS***************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define WEAPONS_LOADED
#define WEAPONS_CSGO_LOADED

#define TAG_START_WEAPON_CSGO "[AIO START WEAPON] - "
#define CSGO_ITEM_NAME_LEN 32
#define CSGO_ITEMS_TYPES_NUM 12
#define CSGO_SLOT_KNIFE 2
#define CSGO_TEAMS_NUM 4

//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_start_weapon_csgo;
new Handle:cvar_active_start_weapon_csgo_dev;
new Handle:cvar_active_start_weapon_remove_objectives;
new Handle:cvar_active_start_weapon_remove_buyzone;
new Handle:cvar_start_weapon_csgo_terro_primary;
new Handle:cvar_start_weapon_csgo_ct_primary;
new Handle:cvar_start_weapon_csgo_terro_secondary;
new Handle:cvar_start_weapon_csgo_ct_secondary;
new Handle:cvar_start_weapon_csgo_terro_knife;
new Handle:cvar_start_weapon_csgo_ct_knife;
new Handle:cvar_start_weapon_csgo_terro_grenade;
new Handle:cvar_start_weapon_csgo_ct_grenade;
new Handle:cvar_start_weapon_csgo_terro_smoke;
new Handle:cvar_start_weapon_csgo_ct_smoke;
new Handle:cvar_start_weapon_csgo_terro_flash;
new Handle:cvar_start_weapon_csgo_ct_flash;
new Handle:cvar_start_weapon_csgo_terro_molotov;
new Handle:cvar_start_weapon_csgo_ct_molotov;
new Handle:cvar_start_weapon_csgo_terro_decoy;
new Handle:cvar_start_weapon_csgo_ct_decoy;
new Handle:cvar_start_weapon_csgo_terro_gear;
new Handle:cvar_start_weapon_csgo_ct_gear;
new Handle:cvar_start_weapon_csgo_terro_armor;
new Handle:cvar_start_weapon_csgo_ct_armor;

//new Handle:mp_buytime;
new Handle:ammo_grenade_limit_total;
new Handle:ammo_grenade_limit_default;


//Customs/Others
//new backup_mp_buytime;
new C_start_weapon_csgo_terro_grenade;
new C_start_weapon_csgo_ct_grenade;
new C_start_weapon_csgo_terro_smoke;
new C_start_weapon_csgo_ct_smoke;
new C_start_weapon_csgo_terro_flash;
new C_start_weapon_csgo_ct_flash;
new C_start_weapon_csgo_terro_molotov;
new C_start_weapon_csgo_ct_molotov;
new C_start_weapon_csgo_terro_decoy;
new C_start_weapon_csgo_ct_decoy;

//Bool
new bool:B_active_start_weapon_csgo 			= false;
new bool:B_active_start_weapon_csgo_dev			= false;
new bool:B_start_weapon_remove_objectives		= false;
new bool:B_start_weapon_remove_buyzone			= false;
new bool:B_client_bot_takeover[MAXPLAYERS+1]	= false;
new bool:B_client_die[MAXPLAYERS+1]				= false;


//String
new String:S_teams_items[CSGO_TEAMS_NUM][CSGO_ITEMS_TYPES_NUM][CSGO_ITEM_NAME_LEN];

/**********************************************************************************************************************/
/************************************************** EVENTS SOURCEMOD **************************************************/
/**********************************************************************************************************************/


/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartStartWeaponCsgo()
{
	CreateConVar("aio_start_weapon_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_start_weapon_csgo 					= CreateConVar("active_start_weapon_csgo",  					"0", 					"Enable/Disable Weapons Start", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_start_weapon_csgo_dev				= CreateConVar("active_start_weapon_csgo_dev", 					"0", 					"Enable/Disable Weapons Start Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	cvar_active_start_weapon_remove_objectives		= CreateConVar("active_start_weapon_remove_objectives", 		"0", 					"Enable/Disable Objectives", 				DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_start_weapon_remove_buyzone			= CreateConVar("active_start_weapon_remove_buyzone", 			"0", 					"Enable/Disable Buyzone", 					DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	cvar_start_weapon_csgo_terro_primary 			= CreateConVar("active_start_weapon_csgo_terro_primary", 		"ak47", 				"Set primary weapon for terro", 			DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_primary				= CreateConVar("active_start_weapon_csgo_ct_primary", 			"m4a1_silencer",		"Set primary weapon for ct", 				DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_secondary 			= CreateConVar("active_start_weapon_csgo_terro_secondary", 		"glock", 				"Set secondary weapon for terro", 			DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_secondary				= CreateConVar("active_start_weapon_csgo_ct_secondary", 		"usp_silencer", 		"Set secondary weapon for ct", 				DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_knife 				= CreateConVar("active_start_weapon_csgo_terro_knife", 			"knife_butterfly",		"Set knife weapon for terro", 				DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_knife					= CreateConVar("active_start_weapon_csgo_ct_knife", 			"knife_butterfly", 		"Set knife weapon for ct", 					DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_grenade 			= CreateConVar("active_start_weapon_csgo_terro_grenade", 		"1", 					"Set num grenade weapon for terro", 		DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_grenade				= CreateConVar("active_start_weapon_csgo_ct_grenade", 			"1", 					"Set num grenade weapon for ct", 			DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_smoke 				= CreateConVar("active_start_weapon_csgo_terro_smoke", 			"1", 					"Set num smoke weapon for terro", 			DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_smoke 				= CreateConVar("active_start_weapon_csgo_ct_smoke", 			"1", 					"Set num smoke weapon for ct", 				DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_flash 				= CreateConVar("active_start_weapon_csgo_terro_flash", 			"2", 					"Set num flash weapon for terro", 			DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_flash					= CreateConVar("active_start_weapon_csgo_ct_flash", 			"2", 					"Set num flash weapon for ct", 				DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_molotov 			= CreateConVar("active_start_weapon_csgo_terro_molotov", 		"1", 					"Set num molotov weapon for terro", 		DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_molotov				= CreateConVar("active_start_weapon_csgo_ct_molotov", 			"1", 					"Set num molotov weapon for ct", 			DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_decoy 				= CreateConVar("active_start_weapon_csgo_terro_decoy", 			"1", 					"Set num decoy weapon for terro", 			DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_decoy					= CreateConVar("active_start_weapon_csgo_ct_decoy", 			"1", 					"Set num decoy weapon for ct", 				DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_gear 				= CreateConVar("active_start_weapon_csgo_terro_gear", 			"c4", 					"Set gear for terro", 						DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_gear					= CreateConVar("active_start_weapon_csgo_ct_gear", 				"defuser", 				"gear for ct", 								DEFAULT_FLAGS);
	
	cvar_start_weapon_csgo_terro_armor 				= CreateConVar("active_start_weapon_csgo_terro_armor", 			"assaultsuit", 			"Set armor for terro", 						DEFAULT_FLAGS);
	cvar_start_weapon_csgo_ct_armor					= CreateConVar("active_start_weapon_csgo_ct_armor", 			"assaultsuit", 			"armor for ct", 							DEFAULT_FLAGS);
	
	HookEventsStartWeaponCsgo();
	
	//mp_buytime 										= FindConVar("mp_buytime");
	ammo_grenade_limit_total 						= FindConVar("ammo_grenade_limit_total");
	ammo_grenade_limit_default 						= FindConVar("ammo_grenade_limit_default");
	
	//backup_mp_buytime 								= GetConVarInt(mp_buytime);
	
	ServerCommand("exec /aio/start_weapon/config.cfg");

}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsStartWeaponCsgo()
{
	HookConVarChange(cvar_active_start_weapon_csgo, 				Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_active_start_weapon_csgo_dev, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_active_start_weapon_remove_objectives, 	Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_active_start_weapon_remove_buyzone, 		Event_CvarChangeStartWeaponCsgo);
	
	HookConVarChange(cvar_start_weapon_csgo_terro_primary, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_primary, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_secondary, 		Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_secondary, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_knife, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_knife, 				Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_grenade, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_grenade, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_smoke, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_smoke, 				Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_flash, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_flash, 				Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_molotov, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_molotov, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_decoy, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_decoy, 				Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_gear, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_gear, 				Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_terro_armor, 			Event_CvarChangeStartWeaponCsgo);
	HookConVarChange(cvar_start_weapon_csgo_ct_armor, 				Event_CvarChangeStartWeaponCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeStartWeaponCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateStartWeaponCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateStartWeaponCsgo()
{
	B_active_start_weapon_csgo 			= GetConVarBool(cvar_active_start_weapon_csgo);
	B_active_start_weapon_csgo_dev 		= GetConVarBool(cvar_active_start_weapon_csgo_dev);
	B_start_weapon_remove_objectives 	= GetConVarBool(cvar_active_start_weapon_remove_objectives);
	B_start_weapon_remove_buyzone 		= GetConVarBool(cvar_active_start_weapon_remove_buyzone);
	
	C_start_weapon_csgo_terro_grenade 	= GetConVarInt(cvar_start_weapon_csgo_terro_grenade);
	C_start_weapon_csgo_ct_grenade 		= GetConVarInt(cvar_start_weapon_csgo_ct_grenade);
	C_start_weapon_csgo_terro_smoke 	= GetConVarInt(cvar_start_weapon_csgo_terro_smoke);
	C_start_weapon_csgo_ct_smoke 		= GetConVarInt(cvar_start_weapon_csgo_ct_smoke);
	C_start_weapon_csgo_terro_flash 	= GetConVarInt(cvar_start_weapon_csgo_terro_flash);
	C_start_weapon_csgo_ct_flash 		= GetConVarInt(cvar_start_weapon_csgo_ct_flash);
	C_start_weapon_csgo_terro_molotov 	= GetConVarInt(cvar_start_weapon_csgo_terro_molotov);
	C_start_weapon_csgo_ct_molotov 		= GetConVarInt(cvar_start_weapon_csgo_ct_molotov);
	C_start_weapon_csgo_terro_decoy 	= GetConVarInt(cvar_start_weapon_csgo_terro_decoy);
	C_start_weapon_csgo_ct_decoy 		= GetConVarInt(cvar_start_weapon_csgo_ct_decoy);
	
	ConVarRead(cvar_start_weapon_csgo_terro_primary,	cvar_start_weapon_csgo_terro_secondary,		cvar_start_weapon_csgo_terro_knife,		cvar_start_weapon_csgo_terro_gear, cvar_start_weapon_csgo_terro_armor);
	ConVarRead(cvar_start_weapon_csgo_ct_primary,		cvar_start_weapon_csgo_ct_secondary,		cvar_start_weapon_csgo_ct_knife,		cvar_start_weapon_csgo_ct_gear, cvar_start_weapon_csgo_ct_armor);
	
	if(B_active_start_weapon_csgo)
	{
		EnableSettingsStartWeaponCsgo();
		
		decl String:status_objective[10];
		status_objective = (B_start_weapon_remove_objectives) ? "Disable" : "Enable";
		SetObjectifMap(status_objective);
		
		if(B_start_weapon_remove_objectives)
		{
			if(StrEqual(S_map_objectif, "bomb", false))
			{
				RemoveC4();
				if(B_active_start_weapon_csgo_dev)
				{
					LogMessage("%sC4 removed", TAG_START_WEAPON_CSGO);
				}
			}
			else if(StrEqual(S_map_objectif, "hostage", false))
			{
				RemoveHostages();
				if(B_active_start_weapon_csgo_dev)
				{
					LogMessage("%sHostages removed", TAG_START_WEAPON_CSGO);
				}
			}
		}
		
		if(B_start_weapon_remove_buyzone)
		{
			RemoveBuyZoneMap();
		}
	}
	else
	{
		DisableSettingsStartWeaponCsgo();
	}
	
}

EnableSettingsStartWeaponCsgo()
{
	//SetConVarInt(mp_buytime, 0);
}

DisableSettingsStartWeaponCsgo()
{
	//SetConVarInt(mp_buytime, backup_mp_buytime);
}
/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedStartWeaponCsgo()
{
	//UpdateStateStartWeaponCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartStartWeaponCsgo()
{
	UpdateStateStartWeaponCsgo();
}

/***********************************************************/
/********************* READ CVARS **********************/
/***********************************************************/
public ConVarRead(Handle:H_primary_weapon, Handle:H_secondary_weapon, Handle:H_knife_weapon, Handle:H_gear_weapon, Handle:H_armor_weapon) 
{
		
	//PRIMARY WEAPON	
	if(cvar_start_weapon_csgo_terro_primary || cvar_start_weapon_csgo_ct_primary)
	{
		new Teams:clientTeam = (H_primary_weapon == cvar_start_weapon_csgo_terro_primary) ? TeamT : TeamCT;
		new String:primary_weapon_value[512];
		new String:primary_weapon_name[CSGO_ITEM_NAME_LEN];
		
		GetConVarString(H_primary_weapon, primary_weapon_value, sizeof(primary_weapon_value));
		Format(primary_weapon_name, sizeof(primary_weapon_name), "weapon_%s", primary_weapon_value);
	
		new weapons_heavy_csgo_num 		= sizeof(S_weapons_heavy_csgo);
		new weapons_smgs_csgo_num	 	= sizeof(S_weapons_smgs_csgo);
		new weapons_rifles_csgo_num 	= sizeof(S_weapons_rifles_csgo);
		
		new bool:primary_weapon = false;
		
		//We try for heavy weapons
		if(!primary_weapon)
		{
			for(new h=0; h<weapons_heavy_csgo_num; h++)
			{
				if(StrEqual(S_weapons_heavy_csgo[h], primary_weapon_name, false))
				{
					S_teams_items[clientTeam][_:SlotPrimary] = primary_weapon_name;
					primary_weapon = true;
				}
			}
		}
		
		//We try for smgs weapons
		if(!primary_weapon)
		{
			for(new s=0; s<weapons_smgs_csgo_num; s++)
			{
				if(StrEqual(S_weapons_smgs_csgo[s], primary_weapon_name, false))
				{
					S_teams_items[clientTeam][_:SlotPrimary] = primary_weapon_name;
					primary_weapon = true;
				}	
			}
		}
		
		//We try for rifles weapons
		if(!primary_weapon)
		{
			for(new r=0; r<weapons_rifles_csgo_num; r++)
			{
				if(StrEqual(S_weapons_rifles_csgo[r], primary_weapon_name, false))
				{
					S_teams_items[clientTeam][_:SlotPrimary] = primary_weapon_name;
					primary_weapon = true;
				}
			}
		}
		
		//Error no weapons
		if(!primary_weapon)
		{
			if(StrEqual(primary_weapon_value, "none", false))
			{
				S_teams_items[clientTeam][_:SlotPrimary] = "none";
				primary_weapon = true;
			}
			else
			{
				SetFailState("%sYou should check the primary weapon name", TAG_START_WEAPON_CSGO);	
			}
		}

		if(B_active_start_weapon_csgo_dev)
		{
			LogMessage("%sPrimary weapon is %s for team :%s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotPrimary], (clientTeam == TeamT) ? "Terro" : "CT");
		}		

	}
	
	//SECONDARY WEAPON	
	if(cvar_start_weapon_csgo_terro_secondary || cvar_start_weapon_csgo_ct_secondary)
	{
		new Teams:clientTeam = (H_secondary_weapon == cvar_start_weapon_csgo_terro_secondary) ? TeamT : TeamCT;
		
		new String:secondary_weapon_value[512];
		new String:secondary_weapon_name[CSGO_ITEM_NAME_LEN];
		
		GetConVarString(H_secondary_weapon, secondary_weapon_value, sizeof(secondary_weapon_value));
		Format(secondary_weapon_name, sizeof(secondary_weapon_name), "weapon_%s", secondary_weapon_value);
	
		new weapons_pistols_csgo_num = sizeof(S_weapons_pistols_csgo);
		
		new bool:secondary_weapon = false;	
		
		//We try for heavy weapons
		if(!secondary_weapon)
		{
			for(new p=0; p<weapons_pistols_csgo_num; p++)
			{
				if(StrEqual(S_weapons_pistols_csgo[p], secondary_weapon_name, false))
				{
					S_teams_items[clientTeam][_:SlotSecondary] = secondary_weapon_name;
					secondary_weapon = true;
				}
			}
		}
		
		//Error no weapons
		if(!secondary_weapon)
		{
			if(StrEqual(secondary_weapon_value, "none", false))
			{
				S_teams_items[clientTeam][_:SlotSecondary] = "none";
				secondary_weapon = true;
			}
			else
			{
				SetFailState("%sYou should check the secondary weapon name", TAG_START_WEAPON_CSGO);	
			}
		}
		
		if(B_active_start_weapon_csgo_dev)
		{
			LogMessage("%sSecondary weapon is %s for team :%s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotSecondary], (clientTeam == TeamT) ? "Terro" : "CT");
		}	
	}
	
	//KNIFE WEAPON	
	if(cvar_start_weapon_csgo_terro_knife || cvar_start_weapon_csgo_ct_knife)
	{
		new Teams:clientTeam = (H_knife_weapon == cvar_start_weapon_csgo_terro_knife) ? TeamT : TeamCT;
		
		new String:knife_weapon_value[512];
		new String:knife_weapon_name[CSGO_ITEM_NAME_LEN];
		
		GetConVarString(H_knife_weapon, knife_weapon_value, sizeof(knife_weapon_value));
		Format(knife_weapon_name, sizeof(knife_weapon_name), "weapon_%s", knife_weapon_value);
	
		new weapons_knife_csgo_num = sizeof(S_weapons_knife_csgo);
		
		new bool:knife_weapon = false;	
		
		//We try for heavy weapons
		if(!knife_weapon)
		{
			for(new k=0; k<weapons_knife_csgo_num; k++)
			{
				if(StrEqual(S_weapons_knife_csgo[k], knife_weapon_name, false))
				{
					S_teams_items[clientTeam][_:SlotKnife] = knife_weapon_name;
					knife_weapon = true;
				}
			}
		}
		
		//Error no weapons
		if(!knife_weapon)
		{
			if(StrEqual(knife_weapon_value, "none", false))
			{
				S_teams_items[clientTeam][_:SlotKnife] = "none";
				knife_weapon = true;
			}
			else
			{
				SetFailState("%sYou should check the knife weapon name", TAG_START_WEAPON_CSGO);	
			}
		}
		
		if(B_active_start_weapon_csgo_dev)
		{
			LogMessage("%sKnife weapon is %s for team :%s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotKnife], (clientTeam == TeamT) ? "Terro" : "CT");
		}	
	}

	//ARMOR WEAPON	
	if(cvar_start_weapon_csgo_terro_armor || cvar_start_weapon_csgo_ct_armor)
	{
		new Teams:clientTeam = (H_armor_weapon == cvar_start_weapon_csgo_terro_armor) ? TeamT : TeamCT;
		
		new String:armor_weapon_value[512];
		new String:armor_weapon_name[CSGO_ITEM_NAME_LEN];
		
		GetConVarString(H_armor_weapon, armor_weapon_value, sizeof(armor_weapon_value));
		Format(armor_weapon_name, sizeof(armor_weapon_name), "item_%s", armor_weapon_value);
	
		new weapons_armor_csgo_num = sizeof(S_weapons_armor_csgo);
		
		new bool:armor_weapon = false;	
		
		//We try for heavy weapons
		if(!armor_weapon)
		{
			for(new k=0; k<weapons_armor_csgo_num; k++)
			{
				if(StrEqual(S_weapons_armor_csgo[k], armor_weapon_name, false))
				{
					S_teams_items[clientTeam][_:SlotArmor] = armor_weapon_name;
					armor_weapon = true;
				}
			}
		}
		
		//Error no weapons
		if(!armor_weapon)
		{
			if(StrEqual(armor_weapon_value, "none", false))
			{
				S_teams_items[clientTeam][_:SlotArmor] = "none";
				armor_weapon = true;
			}
			else
			{
				SetFailState("%sYou should check the armor name", TAG_START_WEAPON_CSGO);	
			}
		}
		
		if(B_active_start_weapon_csgo_dev)
		{
			LogMessage("%sArmor is %s for team :%s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotArmor], (clientTeam == TeamT) ? "Terro" : "CT");
		}	
	}
	
	//GEAR WEAPON	
	if(cvar_start_weapon_csgo_terro_gear || cvar_start_weapon_csgo_ct_gear)
	{
		new Teams:clientTeam = (H_gear_weapon == cvar_start_weapon_csgo_terro_gear) ? TeamT : TeamCT;
		
		new String:gear_weapon_value[512];
		
		GetConVarString(H_gear_weapon, gear_weapon_value, sizeof(gear_weapon_value));
		
		new weapons_gear_t_csgo_num = sizeof(S_weapons_c4_csgo);
		new weapons_gear_ct_csgo_num = sizeof(S_weapons_defuse_csgo);
		
		new bool:gear_weapon_t = false;	
		new bool:gear_weapon_ct = false;
		
		if(clientTeam == TeamT)
		{
			new String:gear_weapon_name[CSGO_ITEM_NAME_LEN];
			Format(gear_weapon_name, sizeof(gear_weapon_name), "weapon_%s", gear_weapon_value);
			
			//We try for heavy weapons
			if(!gear_weapon_t)
			{
				for(new g=0; g<weapons_gear_t_csgo_num; g++)
				{
					if(StrEqual(S_weapons_c4_csgo[g], gear_weapon_name, false))
					{
						S_teams_items[clientTeam][_:SlotC4] = gear_weapon_name;
						gear_weapon_t = true;
					}
				}
			}
			
			if(!gear_weapon_t)
			{
				if(StrEqual(gear_weapon_value, "none", false))
				{
					S_teams_items[clientTeam][_:SlotC4] = "none";
					gear_weapon_t = true;
				}
				else
				{
					SetFailState("%sYou should check the gear name", TAG_START_WEAPON_CSGO);	
				}			
			}
			if(B_active_start_weapon_csgo_dev)
			{
				LogMessage("%sGear is %s for team :%s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotC4], (clientTeam == TeamT) ? "Terro" : "CT");
			}
		}
		
		if(clientTeam == TeamCT)
		{
			new String:gear_weapon_name[CSGO_ITEM_NAME_LEN];
			Format(gear_weapon_name, sizeof(gear_weapon_name), "item_%s", gear_weapon_value);
			
			//We try for heavy weapons
			if(!gear_weapon_ct)
			{
				for(new g=0; g<weapons_gear_ct_csgo_num; g++)
				{
					if(StrEqual(S_weapons_defuse_csgo[g], gear_weapon_name, false))
					{
						S_teams_items[clientTeam][_:SlotDefuse] = gear_weapon_name;
						gear_weapon_ct = true;
					}
				}
			}
			
			if(!gear_weapon_ct)
			{
				if(StrEqual(gear_weapon_value, "none", false))
				{
					S_teams_items[clientTeam][_:SlotDefuse] = "none";
					gear_weapon_ct = true;
				}
				else
				{
					SetFailState("%sYou should check the gear name", TAG_START_WEAPON_CSGO);	
				}			
			}
			
			if(B_active_start_weapon_csgo_dev)
			{
				LogMessage("%sGear is %s for team :%s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotDefuse], (clientTeam == TeamT) ? "Terro" : "CT");
			}
		}	
	}
	
}

/***********************************************************/
/******************** WHEN PLAYER SPAWN ********************/
/***********************************************************/
public Action:Event_PlayerSpawnStartWeaponCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_start_weapon_csgo)
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		new Teams:clientTeam = Teams:GetClientTeam(client);
		
		if(IsClientValid(client) && IsPlayerAlive(client) && (clientTeam == TeamT || clientTeam == TeamCT))
		{
			new weapon_pri 		= GetPlayerWeaponSlot(client, _:SlotPrimary);
			new weapon_sec 		= GetPlayerWeaponSlot(client, _:SlotSecondary);
			new knife 			= GetPlayerWeaponSlot(client, _:SlotKnife);
			new c4 				= GetPlayerWeaponSlot(client, _:SlotC4);
			new armor 			= GetEntData(client, m_ArmorValue);
			new helmet 			= GetEntData(client, m_bHasHelmet);
			new defuser 		= GetEntData(client, m_bHasDefuser);
			
			if(B_active_start_weapon_csgo_dev)
			{
				PrintToChat(client, "%sPlayer is alive and have team", TAG_START_WEAPON_CSGO);
				PrintToChat(client, "%sPri : %i", TAG_START_WEAPON_CSGO, weapon_pri);
				PrintToChat(client, "%sSec : %i", TAG_START_WEAPON_CSGO, weapon_sec);
				PrintToChat(client, "%sKnife : %i", TAG_START_WEAPON_CSGO, knife);
				PrintToChat(client, "%sHelmet : %i", TAG_START_WEAPON_CSGO, helmet);
				PrintToChat(client, "%sDefuse : %i", TAG_START_WEAPON_CSGO, defuser);
				PrintToChat(client, "%sC4 : %i", TAG_START_WEAPON_CSGO, c4);
			}
			
			//Give primary weapon
			if(strlen(S_teams_items[clientTeam][_:SlotPrimary]))
			{
				//if client doesn't have weapon primary and primary config weapon is set
				if(weapon_pri == -1 && !StrEqual(S_teams_items[clientTeam][_:SlotPrimary], "none", false))
				{

					GivePlayerItem(client, S_teams_items[clientTeam][_:SlotPrimary]);
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotPrimary]);
					}
				}
				
				//if client have weapon primary and primary config weapon is set
				else if (weapon_pri != -1 && !StrEqual(S_teams_items[clientTeam][_:SlotPrimary], "none", false))
				{
					decl String:weapon_pri_name[32];
					GetEntityClassname(weapon_pri, weapon_pri_name, sizeof(weapon_pri_name));
					
					//If client has weapon different of config weapon
					if(!StrEqual(weapon_pri_name, S_teams_items[clientTeam][_:SlotPrimary], false))
					{
						RemovePlayerItem(client, weapon_pri);
						GivePlayerItem(client, S_teams_items[clientTeam][_:SlotPrimary]);
						if(B_active_start_weapon_csgo_dev)
						{
							PrintToChat(client, "%sYou had: %s", TAG_START_WEAPON_CSGO, weapon_pri_name);
							PrintToChat(client, "%sYou received %s corresponding in your team", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotPrimary]);
						}
					}			
				}
				//If client have primary weapon and shouldn't have it
				else if (weapon_pri != -1 && StrEqual(S_teams_items[clientTeam][_:SlotPrimary], "none", false))
				{	
					RemovePlayerItem(client, weapon_pri);
					
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sNot allowed to have primary weapon", TAG_START_WEAPON_CSGO);
					}
				}
			}
			
			//Give secondary weapon
			if(strlen(S_teams_items[clientTeam][_:SlotSecondary]))
			{
				//if client doesn't have weapon secondary and secondary config weapon is set
				if(weapon_sec == -1 && !StrEqual(S_teams_items[clientTeam][_:SlotSecondary], "none", false))
				{
					GivePlayerItem(client, S_teams_items[clientTeam][_:SlotSecondary]);
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotSecondary]);
					}
				}
				
				//if client have weapon secondary and secondary config weapon is set
				else if (weapon_sec != -1 && !StrEqual(S_teams_items[clientTeam][_:SlotSecondary], "none", false))
				{
					decl String:weapon_sec_name[32];
					GetEntityClassname(weapon_sec, weapon_sec_name, sizeof(weapon_sec_name));
					
					//If client has weapon different of config weapon
					if(!StrEqual(weapon_sec_name, S_teams_items[clientTeam][_:SlotSecondary], false))
					{
						RemovePlayerItem(client, weapon_sec);
						GivePlayerItem(client, S_teams_items[clientTeam][_:SlotSecondary]);
						if(B_active_start_weapon_csgo_dev)
						{
							PrintToChat(client, "%sYou had: %s", TAG_START_WEAPON_CSGO, weapon_sec_name);
							PrintToChat(client, "%sYou received %s corresponding in your team", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotSecondary]);
						}
					}			
				}
				
				//If client have secondary weapon and shouldn't have it
				else if (weapon_sec != -1 && StrEqual(S_teams_items[clientTeam][_:SlotSecondary], "none", false))
				{	
					RemovePlayerItem(client, weapon_sec);
					
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sNot allowed to have secondary weapon", TAG_START_WEAPON_CSGO);
					}
				}
			}
			
			//Give knife
			if(strlen(S_teams_items[clientTeam][_:SlotKnife]))
			{
				//if client doesn't have weapon primary and primary config weapon is set
				if(knife == -1 && !StrEqual(S_teams_items[clientTeam][_:SlotKnife], "none", false))
				{
					if(!IsFakeClient(client))
					{
						new giveknife = GivePlayerItem(client, S_teams_items[clientTeam][_:SlotKnife]);
						EquipPlayerWeapon(client, giveknife);
					}
					else
					{
						GivePlayerItem(client, "weapon_knife");
					}
					
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s and was %s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotKnife], B_client_die[client] ? "Dead" : "Alive");
					}
				}
				
				//if client have weapon primary and primary config weapon is set
				else if (knife != -1 && !StrEqual(S_teams_items[clientTeam][_:SlotKnife], "none", false))
				{
					decl String:knife_name[32];
					GetEntityClassname(knife, knife_name, sizeof(knife_name));
					
					//If client has weapon different of config weapon
					if(!StrEqual(knife_name, S_teams_items[clientTeam][_:SlotKnife], false))
					{
						Client_RemoveWeapon(client, knife_name, true, false);
						new giveknife = GivePlayerItem(client, S_teams_items[clientTeam][_:SlotKnife]);
						EquipPlayerWeapon(client, giveknife);
						
						if(B_active_start_weapon_csgo_dev)
						{
							PrintToChat(client, "%sYou had: %s", TAG_START_WEAPON_CSGO, knife_name);
							PrintToChat(client, "%sYou received %s corresponding in your team", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotKnife]);
						}
					}			
				}
				//If client have primary weapon and shouldn't have it
				else if (knife != -1 && StrEqual(S_teams_items[clientTeam][_:SlotKnife], "none", false))
				{	
					decl String:knife_name[32];
					GetEntityClassname(knife, knife_name, sizeof(knife_name));
					
					Client_RemoveWeapon(client, knife_name, true, false);
					
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sNot allowed to have primary weapon", TAG_START_WEAPON_CSGO);
					}
				}
			}
			
			//Give armor
			if(strlen(S_teams_items[clientTeam][_:SlotArmor]))
			{
				//if client doesn't have weapon primary and primary config weapon is set
				if(armor == 0 && !StrEqual(S_teams_items[clientTeam][_:SlotArmor], "none", false))
				{

					GivePlayerItem(client, S_teams_items[clientTeam][_:SlotArmor]);
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotArmor]);
					}
				}
				
				//if client have weapon primary and primary config weapon is set
				else if (armor != 0 && !StrEqual(S_teams_items[clientTeam][_:SlotArmor], "none", false))
				{
					if(IsValidEntity(armor))
					{
						decl String:armor_name[32];
						GetEntityClassname(armor, armor_name, sizeof(armor_name));
						
						//If client has weapon different of config weapon
						if(!StrEqual(armor_name, S_teams_items[clientTeam][_:SlotArmor], false))
						{
							RemovePlayerItem(client, armor);
							GivePlayerItem(client, S_teams_items[clientTeam][_:SlotArmor]);
						
							if(B_active_start_weapon_csgo_dev)
							{
								PrintToChat(client, "%sYou had: %s", TAG_START_WEAPON_CSGO, armor_name);
								PrintToChat(client, "%sYou received %s corresponding in your team", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotArmor]);
							}
						}
					}
				}
				//If client have primary weapon and shouldn't have it
				else if (armor != 0 && StrEqual(S_teams_items[clientTeam][_:SlotArmor], "none", false))
				{	
					RemovePlayerItem(client, armor);
					
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sNot allowed to have primary weapon", TAG_START_WEAPON_CSGO);
					}
				}
			}
			
			//Give defuse kit
			if(strlen(S_teams_items[clientTeam][_:SlotDefuse]))
			{
				if(defuser == 0 && !StrEqual(S_teams_items[clientTeam][_:SlotDefuse], "none", false) && StrEqual(S_map_objectif, "bomb", false) && !B_start_weapon_remove_objectives)
				{
					SetEntData(client, m_bHasDefuser, 1);
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotDefuse]);
					}
				}
				else if(defuser != 0 && (StrEqual(S_teams_items[clientTeam][_:SlotDefuse], "none", false) || B_start_weapon_remove_objectives) && StrEqual(S_map_objectif, "bomb", false))
				{
					SetEntData(client, m_bHasDefuser, 0);
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou %s has been deleted", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotDefuse]);
					}			
				}
			}
			
			if(B_start_weapon_remove_objectives || StrEqual(S_teams_items[clientTeam][_:SlotC4], "none", false))
			{
				StripC4(client);
			}

			new TotalGrenadesTerro = C_start_weapon_csgo_terro_grenade + C_start_weapon_csgo_terro_smoke + C_start_weapon_csgo_terro_flash + C_start_weapon_csgo_terro_molotov + C_start_weapon_csgo_terro_decoy;
			new TotalGrenadesCT = C_start_weapon_csgo_ct_grenade + C_start_weapon_csgo_ct_smoke + C_start_weapon_csgo_ct_flash + C_start_weapon_csgo_ct_molotov + C_start_weapon_csgo_ct_decoy;
			if(TotalGrenadesTerro > GetConVarInt(ammo_grenade_limit_total) || TotalGrenadesCT > GetConVarInt(ammo_grenade_limit_total))
			{
				LogMessage("You souhld check 'ammo_grenade_limit_total = %i' and 'ammo_grenade_limit_default = %i' in your configuration", ammo_grenade_limit_default, ammo_grenade_limit_total);
			}
			
			if(clientTeam == TeamT)
			{	
				//We give Grenade if config allow
				if(C_start_weapon_csgo_terro_grenade > 0)
				{
					new hegrenade_num = C_start_weapon_csgo_terro_grenade;
					Client_RemoveWeapon(client, "weapon_hegrenade", false, true);

					S_teams_items[clientTeam][_:SlotGrenade] = "weapon_hegrenade";
					for(new h=1; h<=hegrenade_num; h++)
					{
						GivePlayerItem(client, "weapon_hegrenade");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotGrenade], hegrenade_num);
					}
				}
				
				//We give Smoke if config allow
				if(C_start_weapon_csgo_terro_smoke > 0)
				{
					new smokegrenade_num = C_start_weapon_csgo_terro_smoke;
					Client_RemoveWeapon(client, "weapon_smokegrenade", false, true);
					
					S_teams_items[clientTeam][_:SlotSmoke] = "weapon_smokegrenade";
					for(new h=1; h<=smokegrenade_num; h++)
					{
						GivePlayerItem(client, "weapon_smokegrenade");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotSmoke], smokegrenade_num);
					}
				}
				
				//We give Flash if config allow
				if(C_start_weapon_csgo_terro_flash > 0)
				{
					new flashbang_num = C_start_weapon_csgo_terro_flash;
					Client_RemoveWeapon(client, "weapon_flashbang", false, true);
					
					S_teams_items[clientTeam][_:SlotFlash] = "weapon_flashbang";
					for(new h=1; h<=flashbang_num; h++)
					{
						GivePlayerItem(client, "weapon_flashbang");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotFlash], flashbang_num);
					}
				}
				
				//We give Molotov if config allow
				if(C_start_weapon_csgo_terro_molotov > 0)
				{
					new molotov_num = C_start_weapon_csgo_terro_molotov;
					Client_RemoveWeapon(client, "weapon_molotov", false, true);
					
					S_teams_items[clientTeam][_:SlotMolotov] = "weapon_molotov";
					for(new h=1; h<=molotov_num; h++)
					{
						GivePlayerItem(client, "weapon_molotov");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotMolotov], molotov_num);
					}
				}
				
				//We give Decoy if config allow
				if(C_start_weapon_csgo_terro_decoy > 0)
				{
					new decoy_num = C_start_weapon_csgo_terro_decoy;
					Client_RemoveWeapon(client, "weapon_decoy", false, true);
					
					S_teams_items[clientTeam][_:SlotDecoy] = "weapon_decoy";
					for(new h=1; h<=decoy_num; h++)
					{
						GivePlayerItem(client, "weapon_decoy");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotDecoy], decoy_num);
					}
				}
				
			}
			else if(clientTeam == TeamCT)
			{	
				//We give Grenade if config allow
				if(C_start_weapon_csgo_ct_grenade > 0)
				{
					new hegrenade_num = C_start_weapon_csgo_ct_grenade;
					Client_RemoveWeapon(client, "weapon_hegrenade", false, true);

					S_teams_items[clientTeam][_:SlotGrenade] = "weapon_hegrenade";
					for(new h=1; h<=hegrenade_num; h++)
					{
						GivePlayerItem(client, "weapon_hegrenade");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotGrenade], hegrenade_num);
					}
				}
				
				//We give Smoke if config allow
				if(C_start_weapon_csgo_ct_smoke > 0)
				{
					new smokegrenade_num = C_start_weapon_csgo_ct_smoke;
					Client_RemoveWeapon(client, "weapon_smokegrenade", false, true);
					
					S_teams_items[clientTeam][_:SlotSmoke] = "weapon_smokegrenade";
					for(new h=1; h<=smokegrenade_num; h++)
					{
						GivePlayerItem(client, "weapon_smokegrenade");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotSmoke], smokegrenade_num);
					}
				}
				
				//We give Flash if config allow
				if(C_start_weapon_csgo_ct_flash > 0)
				{
					new flashbang_num = C_start_weapon_csgo_ct_flash;
					Client_RemoveWeapon(client, "weapon_flashbang", false, true);
					
					S_teams_items[clientTeam][_:SlotFlash] = "weapon_flashbang";
					for(new h=1; h<=flashbang_num; h++)
					{
						GivePlayerItem(client, "weapon_flashbang");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotFlash], flashbang_num);
					}
				}
				
				//We give Molotov if config allow
				if(C_start_weapon_csgo_ct_molotov > 0)
				{
					new molotov_num = C_start_weapon_csgo_ct_molotov;
					Client_RemoveWeapon(client, "weapon_molotov", false, true);
					
					S_teams_items[clientTeam][_:SlotMolotov] = "weapon_molotov";
					for(new h=1; h<=molotov_num; h++)
					{
						GivePlayerItem(client, "weapon_molotov");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotMolotov], molotov_num);
					}
				}
				
				//We give Decoy if config allow
				if(C_start_weapon_csgo_ct_decoy > 0)
				{
					new decoy_num = C_start_weapon_csgo_ct_decoy;
					Client_RemoveWeapon(client, "weapon_decoy", false, true);
					
					S_teams_items[clientTeam][_:SlotDecoy] = "weapon_decoy";
					for(new h=1; h<=decoy_num; h++)
					{
						GivePlayerItem(client, "weapon_decoy");
					}
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChat(client, "%sYou received %s x%i", TAG_START_WEAPON_CSGO, S_teams_items[clientTeam][_:SlotDecoy], decoy_num);
					}
				}
				
			}
			
			B_client_bot_takeover[client] 	= true;
			B_client_die[client] 			= false;
		}
	}
}

/***********************************************************/
/***************** WHEN PLAYER TAKEOVER BOT ****************/
/***********************************************************/
public Action:Event_PlayerDeathStartWeaponCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(!B_active_start_weapon_csgo)
	{
		new victim 	= GetClientOfUserId(GetEventInt(event, "userid"));
		
		//if(victim)
		{
			if(B_active_start_weapon_csgo_dev)
			{
				PrintToChat(victim, "%sYou die", TAG_START_WEAPON_CSGO);
			}
			
			B_client_bot_takeover[victim] 	= false;
			B_client_die[victim] 			= true;
		}
	}
}

/***********************************************************/
/***************** WHEN PLAYER TAKEOVER BOT ****************/
/***********************************************************/
public Action:Event_BotTakeOverStartWeaponCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_start_weapon_csgo)
	{
		new client 	= GetClientOfUserId(GetEventInt(event, "userid"));
		new bot 	= GetClientOfUserId(GetEventInt(event, "botid"));
		
		decl String:bot_name[32];
		GetClientName(bot, bot_name, sizeof(bot_name));
		
		//if(!B_client_bot_takeover[client] && B_client_die[client])
		{
			if(B_active_start_weapon_csgo_dev)
			{
				PrintToChat(client, "%sYou takeover bot: %s", TAG_START_WEAPON_CSGO, bot_name);
			}
			
			B_client_bot_takeover[client] 	= true;
			B_client_die[client]			= false;
		}
	}
}

/***********************************************************/
/******************** WHEN ROUND START *********************/
/***********************************************************/
public Action:Event_RoundStartStartWeaponCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_start_weapon_csgo)
	{
		for (new i = 0; i < MaxClients; i++)
		{
			if(IsClientValid(i) && IsPlayerAlive(i))
			{
				if(B_active_start_weapon_csgo_dev)
				{
					PrintToChat(i, "%sRound Start: You are alive", TAG_START_WEAPON_CSGO);
				}
				
				new knife = GetPlayerWeaponSlot(i, _:SlotKnife);
				decl String:knife_name[32];
				GetEntityClassname(knife, knife_name, sizeof(knife_name));
				
				
				B_client_bot_takeover[i] 	= true;
				B_client_die[i] 			= false;
				
				if(IsFakeClient(i))
				{
					Client_RemoveWeapon(i, knife_name, true, false);
					GivePlayerItem(i, "weapon_knife");
					
					if(B_active_start_weapon_csgo_dev)
					{
						PrintToChatAll("%sRound Start: Bot detect with wrong knife: %s", TAG_START_WEAPON_CSGO, knife_name);
					}
				}
			}
		}
	}
}

/***********************************************************/
/********************* WHEN ROUND END **********************/
/***********************************************************/
public Action:Event_RoundEndStartWeaponCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_start_weapon_csgo)
	{
		for (new i = 0; i < MaxClients; i++)
		{
			if(IsClientValid(i) && IsPlayerAlive(i))
			{
				B_client_bot_takeover[i] 	= true;
				B_client_die[i] 			= false;
			}
		}
	}
}

/***********************************************************/
/****************** WHEN PLAYER PICKUP BOMB ****************/
/***********************************************************/
public Action:Event_BombPickupStartWeaponCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_start_weapon_csgo)
	{
		new client 				= GetClientOfUserId(GetEventInt(event, "userid"));
		new Teams:clientTeam 	= Teams:GetClientTeam(client);
		
		if(B_start_weapon_remove_objectives || StrEqual(S_teams_items[clientTeam][_:SlotC4], "none", false))
		{
			new c4 	= GetPlayerWeaponSlot(client, _:SlotC4);
			
			StripC4(client);
			
			if(B_active_start_weapon_csgo_dev)
			{
				PrintToChatAll("%sBomb bickup and removed: %i", TAG_START_WEAPON_CSGO, c4);
			}
		}
	}
}
			
/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//*****************************AIO MOD WIN OR DIE****************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_WOR_CSGO "[AIO WOD] - "


//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:cvar_active_wor_csgo;
new Handle:cvar_active_wor_csgo_dev;

//Bool
new bool:B_active_wor_csgo 					= false;
new bool:B_active_wor_csgo_dev				= false;

//String
new static String:M_WOR_MODEL_1[] 				= "models/props_street/garbage_can.mdl";

//Customs
new M_WOR_DECALS_BLOOD[13];

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartWODCsgo()
{
	CreateConVar("aio_win_or_die_version", PLUGIN_VERSION, "Version", CVARS);
	
	cvar_active_wor_csgo 					= CreateConVar("active_wor_csgo",  				"0", 					"Enable/Disable Win or Die", 			DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	cvar_active_wor_csgo_dev				= CreateConVar("active_wor_csgo_dev", 			"0", 					"Enable/Disable Win or Die Dev Mod", 	DEFAULT_FLAGS, 		true, 0.0, 		true, 1.0);
	
	PrecacheModel(M_WOR_MODEL_1, true);
	PrecacheDecalsBloodWOR();
	
	RegAdminCmd("sm_wor",		Command_WOR,		ADMFLAG_CHANGEMAP, 		"Test the Win or Die effect");
	
	HookEventsWODCsgo();
	
	ServerCommand("exec /aio/win_or_die/config.cfg");
}

/***********************************************************/
/******************** WHEN CVAR CHANGED ********************/
/***********************************************************/
HookEventsWODCsgo()
{
	HookConVarChange(cvar_active_wor_csgo, 				Event_CvarChangeWODCsgo);
	HookConVarChange(cvar_active_wor_csgo_dev, 			Event_CvarChangeWODCsgo);
}

/***********************************************************/
/******************** WHEN CVARS CHANGE ********************/
/***********************************************************/
public Event_CvarChangeWODCsgo(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateStateWODCsgo();
}

/***********************************************************/
/*********************** UPDATE STATE **********************/
/***********************************************************/
UpdateStateWODCsgo()
{
	B_active_wor_csgo 			= GetConVarBool(cvar_active_wor_csgo);
	B_active_wor_csgo_dev 		= GetConVarBool(cvar_active_wor_csgo_dev);
	
	PrecacheModel(M_WOR_MODEL_1, true);
	PrecacheDecalsBloodWOR();
}

/***********************************************************/
/******************* WHEN CONFIG EXECUTED ******************/
/***********************************************************/
public OnConfigsExecutedWODCsgo()
{
	//UpdateStateWODCsgo();
}

/***********************************************************/
/********************* WHEN MAP START **********************/
/***********************************************************/
public OnMapStartWODCsgo()
{
	UpdateStateWODCsgo();
}

/***********************************************************/
/********************* WHEN ROUND END **********************/
/***********************************************************/
public Action:Event_RoundEndWODCsgo(Handle:event, const String:name[], bool:dontBroadcast)
{
	if(B_active_wor_csgo)
	{
		new String:S_message_wor_csgo[256];
		new I_winner = GetEventInt(event, "winner");
		new I_reason = GetEventInt(event, "reason");
		new String:S_message[256];
		GetEventString(event, "message", S_message, sizeof(S_message));
		new bool:B_bomb_exploded = false;
		
		
		if(B_active_wor_csgo_dev)
		{
			PrintToChatAll("winner:%d, reason:%d, message:%s", I_winner, I_reason, S_message);
		}
		/* case 8: terro win || case 7:Ct win */
		switch (I_reason)
		{
			//Bomb exploded
			case 0:
			{
				B_bomb_exploded = true;
				S_message_wor_csgo = "Defuse the bomb";
			}
			case 1:
			{
				S_message_wor_csgo = "Kill the VIP";
			}
			case 2:
			{
				S_message_wor_csgo = "Protect the VIP";
			}
			case 3:
			{
				S_message_wor_csgo = "Kill terrorists";
			}
			//bomb defused
			case 6:
			{
				S_message_wor_csgo = "Protect C4";
			}
			case 10:
			{
				S_message_wor_csgo = "Dont let hostages escape";
			}
			//Round end but terro and CT still alive
			case 11:
			{
				S_message_wor_csgo = "Plant the bomb";
			}
			case 12:
			{
				S_message_wor_csgo = "Rescue the hostages";
			}
			case 14:
			{
				S_message_wor_csgo = "VIP should escape";
			}
			default:
			{
				I_winner = 1;
			}
		}
		
		if(I_winner != 1 && !B_bomb_exploded)
		{
			for(new i = 1; i <= MaxClients; ++i)
			{
				if(IsClientValid(i))
				{

					if(GetClientTeam(i) != I_winner && IsPlayerAlive(i))
					{
						CreateGarbage(i);
						CreateTimer(1.0, KillPlayers, i);
					}
				}
			}
		
		}
		
		if(B_bomb_exploded)
		{
			for(new i = 1; i <= MaxClients; ++i)
			{
				if(IsClientValid(i))
				{
					new Teams:clientTeam = Teams:GetClientTeam(i);
					if(clientTeam == TeamCT && IsPlayerAlive(i))
					{
						CreateGarbage(i);
						CreateTimer(1.0, KillPlayers, i);
					}
				}
			}

		}
	}
}

/***********************************************************/
/********************* CREATE GARBAGE **********************/
/***********************************************************/
CreateGarbage(client)
{
	new cage = CreateEntityByName("prop_dynamic_override");
	
	if(IsValidEdict(cage))
	{
		new Float:vec[3];
		GetClientAbsOrigin(client, vec);
		
		//SetEntityModel(cage, M_WOR_MODEL_1);
		if(B_active_wor_csgo_dev)
		{
			PrintToChat(client, "Model:%s", M_WOR_MODEL_1);
		}
		
		DispatchKeyValue(cage, "model", M_WOR_MODEL_1);
		DispatchKeyValue(cage, "StartDisabled", "false");
		DispatchKeyValue(cage, "Solid", "6");

		SetEntProp(cage, Prop_Data, "m_CollisionGroup", 5);
		SetEntProp(cage, Prop_Data, "m_nSolidType", 6);
		//SetEntPropFloat(cage, Prop_Data, "m_flModelScale", 50.0);

		DispatchSpawn(cage);

		AcceptEntityInput(cage, "Enable");
		AcceptEntityInput(cage, "TurnOn");
		AcceptEntityInput(cage, "DisableMotion");
		
		TeleportEntity(cage, vec, NULL_VECTOR, NULL_VECTOR);
		
		CPrintToChat(client, "%t", "WOD useless chat");
		PrintCenterText(client, "%t", "WOD useless alert"); 
    }
	
}

/***********************************************************/
/****************** PRECACHE BLOOD DECALS ******************/
/***********************************************************/
PrecacheDecalsBloodWOR()
{
	M_WOR_DECALS_BLOOD[0] = PrecacheDecal("decals/blood_splatter.vtf");
	M_WOR_DECALS_BLOOD[1] = PrecacheDecal("decals/bloodstain_003.vtf");
	M_WOR_DECALS_BLOOD[2] = PrecacheDecal("decals/bloodstain_101.vtf");
	M_WOR_DECALS_BLOOD[3] = PrecacheDecal("decals/bloodstain_002.vtf");
	M_WOR_DECALS_BLOOD[4] = PrecacheDecal("decals/bloodstain_001.vtf");
	M_WOR_DECALS_BLOOD[5] = PrecacheDecal("decals/blood8.vtf");
	M_WOR_DECALS_BLOOD[6] = PrecacheDecal("decals/blood7.vtf");
	M_WOR_DECALS_BLOOD[7] = PrecacheDecal("decals/blood6.vtf");
	M_WOR_DECALS_BLOOD[8] = PrecacheDecal("decals/blood5.vtf");
	M_WOR_DECALS_BLOOD[9] = PrecacheDecal("decals/blood4.vtf");
	M_WOR_DECALS_BLOOD[10] = PrecacheDecal("decals/blood3.vtf");
	M_WOR_DECALS_BLOOD[11] = PrecacheDecal("decals/blood2.vtf");
	M_WOR_DECALS_BLOOD[12] = PrecacheDecal("decals/blood1.vtf");
}

/***********************************************************/
/******************** TIMER KILL PLAYER ********************/
/***********************************************************/
public Action:KillPlayers(Handle:timer, any:client)
{
	decl Float:pos[3];
	decl Float:ang[3];
	GetClientAbsOrigin(client, pos);
	GetClientAbsAngles(client, ang);
	
	new death = GetEntProp(client, Prop_Data, "m_iDeaths");
	new frags = GetEntProp(client, Prop_Data, "m_iFrags");
	
	new pointHurt = CreateEntityByName("point_hurt");		// Create point_hurt
	DispatchKeyValue(client, "targetname", "hurtme");		// mark client
	DispatchKeyValue(pointHurt, "Damage", "65536");			// No Damage, just HUD display. Does stop Reviving though
	DispatchKeyValue(pointHurt, "DamageTarget", "hurtme");	// client Assignment
	DispatchKeyValue(pointHurt, "DamageType", "2");			// Type of damage
	DispatchSpawn(pointHurt);								// Spawn described point_hurt
	AcceptEntityInput(pointHurt, "Hurt");					// Trigger point_hurt execute
	AcceptEntityInput(pointHurt, "Kill");					// Remove point_hurt
	DispatchKeyValue(client, "targetname",    "cake");		// Clear client's mark
	
	TE_Start("World Decal");
	TE_WriteVector("m_vecOrigin", pos);
	new random = GetRandomInt(0, 12);
	TE_WriteNum("m_nIndex", M_WOR_DECALS_BLOOD[random]);
	TE_SendToAll();
		
	EmitSoundToAll("weapons/c4/c4_explode1.wav", client, 1, 90);
	
	SetEntProp(client, Prop_Data, "m_iFrags", frags);
	SetEntProp(client, Prop_Data, "m_iDeaths", death + 1);
}

/***********************************************************/
/******************* COMMAND KILL TEST *********************/
/***********************************************************/
public Action:Command_WOR(client, args) 
{
	CreateGarbage(client);
	CreateTimer(2.0, KillPlayers, client);
	return Plugin_Handled;
}
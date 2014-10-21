/*        <AIO MOD> (c) by <De Battista Clint - (http://doyou.watch)         */
/*                                                                           */
/*                       <AIO MOD> is licensed under a                       */
/* Creative Commons Attribution-NonCommercial-NoDerivs 4.0 Unported License. */
/*																			 */
/*      You should have received a copy of the license along with this       */
/*  work.  If not, see <http://creativecommons.org/licenses/by-nc-nd/4.0/>.  */
//***************************************************************************//
//***************************************************************************//
//****************************AIO MOD MENU GENERAL***************************//
//***************************************************************************//
//***************************************************************************//

//***********************************//
//*************DEFINE****************//
//***********************************//
#define TAG_MENU_GENERAL_CSGO 				"[AIO MENU] - "

//***********************************//
//***********PARAMETERS**************//
//***********************************//

//Handle
new Handle:B_active_track_bomb_menu_csgo;
new Handle:B_active_show_damage_menu_csgo;
new Handle:B_active_sounds_kill_menu_csgo;

/***********************************************************/
/*********************** PLUGIN START **********************/
/***********************************************************/
public OnPluginStartMenuGeneralCsgo()
{
	B_active_track_bomb_menu_csgo 			= FindConVar("active_track_bomb_csgo");
	B_active_show_damage_menu_csgo 			= FindConVar("active_show_damage_csgo");
	B_active_sounds_kill_menu_csgo 			= FindConVar("active_sounds_kill_csgo");
	
	RegConsoleCmd("sm_aio",		Command_MenuGeneralCsgo, 		"AIO menu general");
}


/***********************************************************/
/******************* COMMANDE TRACK BOMB *******************/
/***********************************************************/
public Action:Command_MenuGeneralCsgo(client, args) 
{
	BuildMenuGeneralCsgo(client);
	return Plugin_Handled;
}

BuildMenuGeneralCsgo(client)
{
	decl String:title[40], String:track_bomb[40], String:show_damage[40], String:sounds_kill[40];
	new Handle:menu = CreateMenu(MenuGeneralCsgo_Menu);
	
	if(B_active_track_bomb_menu_csgo)
	{
		Format(track_bomb, sizeof(track_bomb), "%T", "Track Bomb menu", client);
		AddMenuItem(menu, "M_track_bomb", track_bomb);
	}
	
	if(B_active_show_damage_menu_csgo)
	{
		Format(show_damage, sizeof(show_damage), "%T", "Show Damage menu", client);
		AddMenuItem(menu, "M_show_damage", show_damage);	
	}
	
	if(B_active_sounds_kill_menu_csgo)
	{
		Format(sounds_kill, sizeof(sounds_kill), "%T", "Sounds Kill menu", client);
		AddMenuItem(menu, "M_sounds_kill", sounds_kill);	
	}
	
	Format(title, sizeof(title), "%T", "Menu General", client);
	SetMenuTitle(menu, title);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public MenuGeneralCsgo_Menu(Handle:menu, MenuAction:action, param1, param2)
{
	switch(action)
	{
		case MenuAction_End:
		{
			CloseHandle(menu);	
		}
		case MenuAction_Select:
		{
			new String:menu1[56];
			GetMenuItem(menu, param2, menu1, sizeof(menu1));
			if(StrEqual(menu1, "M_track_bomb"))
			{
				BuildSettingsMenuTrackBombCsgo(param1);
			}
			else if(StrEqual(menu1, "M_show_damage"))
			{
				BuildSettingsMenuShowDamageCsgo(param1);
			}
			else if(StrEqual(menu1, "M_sounds_kill"))
			{
				BuildSettingsMenuSoundsKillCsgo(param1);
			}
		}
	}
}
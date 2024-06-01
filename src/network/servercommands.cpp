// 6733be247db4612b5ac1d0ee86859195
// This file has been automatically generated. Do not edit by hand.
#include "cl_main.h"
#include "servercommands.h"
#include "network.h"
#include "network/netcommand.h"

bool CLIENT_ParseServerCommand( SVC header, BYTESTREAM_s *bytestream )
{
	switch ( header )
	{
	case SVC_HEADER:
		{
			ServerCommands::Header command;
			command.sequence = bytestream->ReadLong();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "Header: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PING:
		{
			ServerCommands::Ping command;
			command.time = bytestream->ReadLong();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "Ping: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_BEGINSNAPSHOT:
		{
			ServerCommands::BeginSnapshot command;
			command.Execute();
		}
		return true;

	case SVC_ENDSNAPSHOT:
		{
			ServerCommands::EndSnapshot command;
			command.Execute();
		}
		return true;

	case SVC_MAPLOAD:
		{
			ServerCommands::MapLoad command;
			command.mapName = bytestream->ReadString();
			command.currentPosition = bytestream->ReadShort();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MapLoad: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MAPNEW:
		{
			ServerCommands::MapNew command;
			command.mapName = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MapNew: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MAPEXIT:
		{
			ServerCommands::MapExit command;
			command.position = bytestream->ReadByte();
			command.nextMap = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MapExit: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MAPAUTHENTICATE:
		{
			ServerCommands::MapAuthenticate command;
			command.mapName = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MapAuthenticate: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPTIME:
		{
			ServerCommands::SetMapTime command;
			command.time = bytestream->ReadLong();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapTime: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPNUMKILLEDMONSTERS:
		{
			ServerCommands::SetMapNumKilledMonsters command;
			command.killedMonsters = bytestream->ReadVariable();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapNumKilledMonsters: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPNUMFOUNDITEMS:
		{
			ServerCommands::SetMapNumFoundItems command;
			command.foundItems = bytestream->ReadVariable();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapNumFoundItems: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPNUMFOUNDSECRETS:
		{
			ServerCommands::SetMapNumFoundSecrets command;
			command.foundSecrets = bytestream->ReadVariable();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapNumFoundSecrets: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPNUMTOTALMONSTERS:
		{
			ServerCommands::SetMapNumTotalMonsters command;
			command.totalMonsters = bytestream->ReadVariable();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapNumTotalMonsters: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPNUMTOTALITEMS:
		{
			ServerCommands::SetMapNumTotalItems command;
			command.totalItems = bytestream->ReadVariable();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapNumTotalItems: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPMUSIC:
		{
			ServerCommands::SetMapMusic command;
			command.music = bytestream->ReadString();
			command.order = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapMusic: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETMAPSKY:
		{
			ServerCommands::SetMapSky command;
			command.sky1 = bytestream->ReadString();
			command.sky2 = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapSky: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSCROLLER:
		{
			ServerCommands::DoScroller command;
				int temp0;
			command.type = bytestream->ReadByte();
			command.x = bytestream->ReadLong();
			command.y = bytestream->ReadLong();
			command.affectee = bytestream->ReadLong();
			command.accel = bytestream->ReadBit();
			command.hasControl = bytestream->ReadBit();
			command.hasPos = bytestream->ReadBit();
			if ( command.ContainsSector() )
			{
				temp0 = bytestream->ReadShort();
				command.sector = CLIENT_FindSectorByID( temp0 );
			}
			if ( command.ContainsPos() )
			{
				command.pos = bytestream->ReadByte();
			}
			if ( command.ContainsSector() )
			{

				if ( command.sector == NULL )
				{
					CLIENT_PrintWarning( "DoScroller: couldn't find sector: %d\n", temp0 );
					return true;
				}


			}
			if ( command.ContainsPos() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoScroller: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNPLAYER:
		{
			ServerCommands::SpawnPlayer command;
				int temp1;
			int temp2;
			command.player = &players[bytestream->ReadByte()];
			command.priorState = bytestream->ReadShortByte( 4 );
			command.isBot = bytestream->ReadBit();
			command.isSpectating = bytestream->ReadBit();
			command.isDeadSpectator = bytestream->ReadBit();
			command.isMorphed = bytestream->ReadBit();
			command.netid = bytestream->ReadShort();
			command.angle = bytestream->ReadLong();
			command.x = bytestream->ReadLong();
			command.y = bytestream->ReadLong();
			command.z = bytestream->ReadLong();
			command.playerClass = bytestream->ReadByte();
			if ( command.CheckIsMorphed() )
			{
				command.morphStyle = bytestream->ReadShort();
				temp1 = bytestream->ReadShort();
				command.morphedClass = NETWORK_GetClassFromIdentification( temp1 );
			}
			temp2 = command.player - players;

			if (( temp2 < 0 ) || ( temp2 >= MAXPLAYERS ))
			{
				CLIENT_PrintWarning( "SpawnPlayer: Invalid player number: %d\n", temp2 );
				return true;
			}

			if ( command.CheckIsMorphed() )
			{

				if ( command.morphedClass == NULL )
				{
					CLIENT_PrintWarning( "SpawnPlayer: unknown class ID for morphedClass: %d\n", temp1 );
					return true;
				}


			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnPlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MOVEPLAYER:
		{
			ServerCommands::MovePlayer command;
			int temp3;
			command.player = &players[bytestream->ReadByte()];
			command.flags = bytestream->ReadByte();
			if ( command.IsVisible() )
			{
				command.x = bytestream->ReadLong();
				command.y = bytestream->ReadLong();
				command.z = bytestream->ReadShort() << FRACBITS;
				command.angle = bytestream->ReadLong();
			}
			if ( command.IsMovingX() )
			{
				command.velx = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.IsMovingY() )
			{
				command.vely = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.IsMovingZ() )
			{
				command.velz = bytestream->ReadShort() << FRACBITS;
			}
			temp3 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp3 ) == false )
			{
				CLIENT_PrintWarning( "MovePlayer: Invalid player number: %d\n", temp3 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( command.IsVisible() )
			{
			}
			if ( command.IsMovingX() )
			{
			}
			if ( command.IsMovingY() )
			{
			}
			if ( command.IsMovingZ() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MovePlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DAMAGEPLAYER:
		{
			ServerCommands::DamagePlayer command;
			int temp4;
			int temp5;
			command.player = &players[bytestream->ReadByte()];
			command.health = bytestream->ReadVariable();
			command.armor = bytestream->ReadVariable();
			temp4 = bytestream->ReadShort();
			temp5 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp5 ) == false )
			{
				CLIENT_PrintWarning( "DamagePlayer: Invalid player number: %d\n", temp5 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( CLIENT_ReadActorFromNetID( temp4, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.attacker ),
											"DamagePlayer", "attacker" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DamagePlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_KILLPLAYER:
		{
			ServerCommands::KillPlayer command;
			int temp6;
			int temp7;
			int temp8;
			int temp9;
			command.player = &players[bytestream->ReadByte()];
			temp6 = bytestream->ReadShort();
			temp7 = bytestream->ReadShort();
			command.health = bytestream->ReadShort();
			command.MOD = bytestream->ReadString();
			command.damageType = bytestream->ReadString();
			temp8 = bytestream->ReadShort();
			command.weaponType = NETWORK_GetClassFromIdentification( temp8 );
			temp9 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp9 ) == false )
			{
				CLIENT_PrintWarning( "KillPlayer: Invalid player number: %d\n", temp9 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( CLIENT_ReadActorFromNetID( temp6, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.source ),
											"KillPlayer", "source" ) == false )
			{
				return true;
			}


			if ( CLIENT_ReadActorFromNetID( temp7, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.inflictor ),
											"KillPlayer", "inflictor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "KillPlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERHEALTH:
		{
			ServerCommands::SetPlayerHealth command;
			int temp10;
			command.player = &players[bytestream->ReadByte()];
			command.health = bytestream->ReadVariable();
			temp10 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp10 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerHealth: Invalid player number: %d\n", temp10 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerHealth: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERARMOR:
		{
			ServerCommands::SetPlayerArmor command;
			int temp11;
			command.player = &players[bytestream->ReadByte()];
			command.armorAmount = bytestream->ReadVariable();
			command.armorIcon = bytestream->ReadString();
			temp11 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp11 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerArmor: Invalid player number: %d\n", temp11 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerArmor: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERSTATE:
		{
			ServerCommands::SetPlayerState command;
			int temp12;
			command.player = &players[bytestream->ReadByte()];
			command.state = bytestream->ReadByte();
			temp12 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp12 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerState: Invalid player number: %d\n", temp12 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerState: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERUSERINFO:
		{
			ServerCommands::SetPlayerUserInfo command;
			unsigned int temp13;
			int temp14;
			command.player = &players[bytestream->ReadByte()];
			temp13 = bytestream->ReadByte();
			command.cvars.Reserve( temp13 );
			for ( unsigned int i = 0; i < temp13; ++i )
			{
				command.cvars[i].name = NETWORK_ReadName( bytestream );
				command.cvars[i].value = bytestream->ReadString();
			}
			temp14 = command.player - players;

			if (( temp14 < 0 ) || ( temp14 >= MAXPLAYERS ))
			{
				CLIENT_PrintWarning( "SetPlayerUserInfo: Invalid player number: %d\n", temp14 );
				return true;
			}

			for ( unsigned int i = 0; i < command.cvars.Size(); ++i )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerUserInfo: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERFRAGS:
		{
			ServerCommands::SetPlayerFrags command;
			int temp15;
			command.player = &players[bytestream->ReadByte()];
			command.fragCount = bytestream->ReadVariable();
			temp15 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp15 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerFrags: Invalid player number: %d\n", temp15 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerFrags: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERPOINTS:
		{
			ServerCommands::SetPlayerPoints command;
			int temp16;
			command.player = &players[bytestream->ReadByte()];
			command.pointCount = bytestream->ReadVariable();
			temp16 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp16 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerPoints: Invalid player number: %d\n", temp16 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerPoints: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERWINS:
		{
			ServerCommands::SetPlayerWins command;
			int temp17;
			command.player = &players[bytestream->ReadByte()];
			command.wins = bytestream->ReadVariable();
			temp17 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp17 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerWins: Invalid player number: %d\n", temp17 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerWins: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERKILLCOUNT:
		{
			ServerCommands::SetPlayerKillCount command;
			int temp18;
			command.player = &players[bytestream->ReadByte()];
			command.killCount = bytestream->ReadVariable();
			temp18 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp18 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerKillCount: Invalid player number: %d\n", temp18 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerKillCount: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERSTATUS:
		{
			ServerCommands::SetPlayerStatus command;
			int temp19;
			command.player = &players[bytestream->ReadByte()];
			command.type = bytestream->ReadShortByte( 7 );
			command.value = bytestream->ReadBit();
			temp19 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp19 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerStatus: Invalid player number: %d\n", temp19 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerStatus: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERTEAM:
		{
			ServerCommands::SetPlayerTeam command;
			int temp20;
			command.player = &players[bytestream->ReadByte()];
			command.team = bytestream->ReadByte();
			temp20 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp20 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerTeam: Invalid player number: %d\n", temp20 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerTeam: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERCAMERA:
		{
			ServerCommands::SetPlayerCamera command;
			int temp21;
			temp21 = bytestream->ReadShort();
			command.revertPlease = bytestream->ReadBit();
			if ( CLIENT_ReadActorFromNetID( temp21, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.camera ),
											"SetPlayerCamera", "camera" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerCamera: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERPOISONCOUNT:
		{
			ServerCommands::SetPlayerPoisonCount command;
			int temp22;
			command.player = &players[bytestream->ReadByte()];
			command.poisonCount = bytestream->ReadShort();
			temp22 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp22 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerPoisonCount: Invalid player number: %d\n", temp22 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerPoisonCount: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERAMMOCAPACITY:
		{
			ServerCommands::SetPlayerAmmoCapacity command;
			int temp23;
			int temp24;
			command.player = &players[bytestream->ReadByte()];
			temp23 = bytestream->ReadShort();
			command.ammoType = NETWORK_GetClassFromIdentification( temp23 );

			if ( command.ammoType->IsDescendantOf( RUNTIME_CLASS( AAmmo )) == false )
				command.ammoType = NULL;


			command.maxAmount = bytestream->ReadLong();
			temp24 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp24 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerAmmoCapacity: Invalid player number: %d\n", temp24 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;


			if ( command.ammoType == NULL )
			{
				CLIENT_PrintWarning( "SetPlayerAmmoCapacity: unknown class ID for ammoType: %d\n", temp23 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerAmmoCapacity: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERCHEATS:
		{
			ServerCommands::SetPlayerCheats command;
			int temp25;
			command.player = &players[bytestream->ReadByte()];
			command.cheats = bytestream->ReadLong();
			temp25 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp25 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerCheats: Invalid player number: %d\n", temp25 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerCheats: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERPENDINGWEAPON:
		{
			ServerCommands::SetPlayerPendingWeapon command;
			int temp26;
			int temp27;
			command.player = &players[bytestream->ReadByte()];
			temp26 = bytestream->ReadShort();
			command.weaponType = NETWORK_GetClassFromIdentification( temp26 );

			if ( command.weaponType->IsDescendantOf( RUNTIME_CLASS( AWeapon )) == false )
				command.weaponType = NULL;


			temp27 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp27 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerPendingWeapon: Invalid player number: %d\n", temp27 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;


			if ( command.weaponType == NULL )
			{
				CLIENT_PrintWarning( "SetPlayerPendingWeapon: unknown class ID for weaponType: %d\n", temp26 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerPendingWeapon: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERPSPRITE:
		{
			ServerCommands::SetPlayerPSprite command;
			int temp28;
			int temp29;
			command.player = &players[bytestream->ReadByte()];
			temp28 = bytestream->ReadShort();
			command.stateOwner = NETWORK_GetClassFromIdentification( temp28 );

			if ( command.stateOwner->IsDescendantOf( RUNTIME_CLASS( AActor )) == false )
				command.stateOwner = NULL;


			command.offset = bytestream->ReadShort();
			command.position = bytestream->ReadByte();
			temp29 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp29 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerPSprite: Invalid player number: %d\n", temp29 );
				return true;
			}


			if ( command.stateOwner == NULL )
			{
				CLIENT_PrintWarning( "SetPlayerPSprite: unknown class ID for stateOwner: %d\n", temp28 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerPSprite: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERBLEND:
		{
			ServerCommands::SetPlayerBlend command;
			int temp30;
			command.player = &players[bytestream->ReadByte()];
			command.blendR = bytestream->ReadFloat();
			command.blendG = bytestream->ReadFloat();
			command.blendB = bytestream->ReadFloat();
			command.blendA = bytestream->ReadFloat();
			temp30 = command.player - players;

			if (( temp30 < 0 ) || ( temp30 >= MAXPLAYERS ))
			{
				CLIENT_PrintWarning( "SetPlayerBlend: Invalid player number: %d\n", temp30 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerBlend: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERMAXHEALTH:
		{
			ServerCommands::SetPlayerMaxHealth command;
			int temp31;
			command.player = &players[bytestream->ReadByte()];
			command.maxHealth = bytestream->ReadLong();
			temp31 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp31 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerMaxHealth: Invalid player number: %d\n", temp31 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerMaxHealth: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETPLAYERLIVESLEFT:
		{
			ServerCommands::SetPlayerLivesLeft command;
			int temp32;
			command.player = &players[bytestream->ReadByte()];
			command.livesLeft = bytestream->ReadByte();
			temp32 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp32 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerLivesLeft: Invalid player number: %d\n", temp32 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerLivesLeft: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_UPDATEPLAYERPING:
		{
			ServerCommands::UpdatePlayerPing command;
			int temp33;
			command.player = &players[bytestream->ReadByte()];
			command.ping = bytestream->ReadShort();
			temp33 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp33 ) == false )
			{
				CLIENT_PrintWarning( "UpdatePlayerPing: Invalid player number: %d\n", temp33 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "UpdatePlayerPing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_UPDATEPLAYEREXTRADATA:
		{
			ServerCommands::UpdatePlayerExtraData command;
			int temp34;
			command.player = &players[bytestream->ReadByte()];
			command.pitch = bytestream->ReadLong();
			command.waterLevel = bytestream->ReadByte();
			command.buttons = bytestream->ReadByte();
			command.viewZ = bytestream->ReadLong();
			command.bob = bytestream->ReadLong();
			temp34 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp34 ) == false )
			{
				CLIENT_PrintWarning( "UpdatePlayerExtraData: Invalid player number: %d\n", temp34 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "UpdatePlayerExtraData: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_UPDATEPLAYERTIME:
		{
			ServerCommands::UpdatePlayerTime command;
			int temp35;
			command.player = &players[bytestream->ReadByte()];
			command.time = bytestream->ReadShort();
			temp35 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp35 ) == false )
			{
				CLIENT_PrintWarning( "UpdatePlayerTime: Invalid player number: %d\n", temp35 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "UpdatePlayerTime: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MOVELOCALPLAYER:
		{
			ServerCommands::MoveLocalPlayer command;
			command.clientTicOnServerEnd = bytestream->ReadLong();
			command.latestServerGametic = bytestream->ReadLong();
			command.x = bytestream->ReadLong();
			command.y = bytestream->ReadLong();
			command.z = bytestream->ReadLong();
			command.velx = bytestream->ReadLong();
			command.vely = bytestream->ReadLong();
			command.velz = bytestream->ReadLong();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MoveLocalPlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DISCONNECTPLAYER:
		{
			ServerCommands::DisconnectPlayer command;
			int temp36;
			command.player = &players[bytestream->ReadByte()];
			temp36 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp36 ) == false )
			{
				CLIENT_PrintWarning( "DisconnectPlayer: Invalid player number: %d\n", temp36 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DisconnectPlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETCONSOLEPLAYER:
		{
			ServerCommands::SetConsolePlayer command;
			command.playerNumber = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetConsolePlayer: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_CONSOLEPLAYERKICKED:
		{
			ServerCommands::ConsolePlayerKicked command;
			command.Execute();
		}
		return true;

	case SVC_GIVEPLAYERMEDAL:
		{
			ServerCommands::GivePlayerMedal command;
			int temp37;
			command.player = &players[bytestream->ReadByte()];
			command.medal = bytestream->ReadByte();
			temp37 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp37 ) == false )
			{
				CLIENT_PrintWarning( "GivePlayerMedal: Invalid player number: %d\n", temp37 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "GivePlayerMedal: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_RESETALLPLAYERSFRAGCOUNT:
		{
			ServerCommands::ResetAllPlayersFragcount command;
			command.Execute();
		}
		return true;

	case SVC_PLAYERISSPECTATOR:
		{
			ServerCommands::PlayerIsSpectator command;
			int temp38;
			command.player = &players[bytestream->ReadByte()];
			command.deadSpectator = bytestream->ReadBit();
			temp38 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp38 ) == false )
			{
				CLIENT_PrintWarning( "PlayerIsSpectator: Invalid player number: %d\n", temp38 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PlayerIsSpectator: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PLAYERSAY:
		{
			ServerCommands::PlayerSay command;
			command.playerNumber = bytestream->ReadByte();
			command.mode = bytestream->ReadByte();
			command.message = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PlayerSay: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PLAYERTAUNT:
		{
			ServerCommands::PlayerTaunt command;
			int temp39;
			command.player = &players[bytestream->ReadByte()];
			temp39 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp39 ) == false )
			{
				CLIENT_PrintWarning( "PlayerTaunt: Invalid player number: %d\n", temp39 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PlayerTaunt: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PLAYERRESPAWNINVULNERABILITY:
		{
			ServerCommands::PlayerRespawnInvulnerability command;
			int temp40;
			command.player = &players[bytestream->ReadByte()];
			temp40 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp40 ) == false )
			{
				CLIENT_PrintWarning( "PlayerRespawnInvulnerability: Invalid player number: %d\n", temp40 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PlayerRespawnInvulnerability: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PLAYERUSEINVENTORY:
		{
			ServerCommands::PlayerUseInventory command;
			int temp41;
			int temp42;
			command.player = &players[bytestream->ReadByte()];
			temp41 = bytestream->ReadShort();
			command.itemType = NETWORK_GetClassFromIdentification( temp41 );

			if ( command.itemType->IsDescendantOf( RUNTIME_CLASS( AInventory )) == false )
				command.itemType = NULL;


			temp42 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp42 ) == false )
			{
				CLIENT_PrintWarning( "PlayerUseInventory: Invalid player number: %d\n", temp42 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;


			if ( command.itemType == NULL )
			{
				CLIENT_PrintWarning( "PlayerUseInventory: unknown class ID for itemType: %d\n", temp41 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PlayerUseInventory: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PLAYERDROPINVENTORY:
		{
			ServerCommands::PlayerDropInventory command;
			int temp43;
			int temp44;
			command.player = &players[bytestream->ReadByte()];
			temp43 = bytestream->ReadShort();
			command.itemType = NETWORK_GetClassFromIdentification( temp43 );

			if ( command.itemType->IsDescendantOf( RUNTIME_CLASS( AInventory )) == false )
				command.itemType = NULL;


			temp44 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp44 ) == false )
			{
				CLIENT_PrintWarning( "PlayerDropInventory: Invalid player number: %d\n", temp44 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;


			if ( command.itemType == NULL )
			{
				CLIENT_PrintWarning( "PlayerDropInventory: unknown class ID for itemType: %d\n", temp43 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PlayerDropInventory: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PRINT:
		{
			ServerCommands::Print command;
			command.printlevel = bytestream->ReadByte();
			command.message = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "Print: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PRINTMID:
		{
			ServerCommands::PrintMid command;
			command.message = bytestream->ReadString();
			command.bold = bytestream->ReadBit();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PrintMid: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PRINTMOTD:
		{
			ServerCommands::PrintMOTD command;
			command.motd = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PrintMOTD: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_PRINTHUDMESSAGE:
		{
			ServerCommands::PrintHUDMessage command;
			command.message = bytestream->ReadString();
			command.x = bytestream->ReadFloat();
			command.y = bytestream->ReadFloat();
			command.type = bytestream->ReadShort();
			command.color = bytestream->ReadShort();
			command.holdTime = bytestream->ReadFloat();
			command.id = bytestream->ReadLong();
			if ( command.ContainsInTime() )
			{
				command.inTime = bytestream->ReadFloat();
			}
			if ( command.ContainsOutTime() )
			{
				command.outTime = bytestream->ReadFloat();
			}
			if ( command.ContainsHUDSize() )
			{
				command.hudWidth = bytestream->ReadShort();
				command.hudHeight = bytestream->ReadShort();
			}
			if ( command.ContainsClippingRectangle() )
			{
				command.clipRectLeft = bytestream->ReadShort();
				command.clipRectTop = bytestream->ReadShort();
				command.clipRectWidth = bytestream->ReadShort();
				command.clipRectHeight = bytestream->ReadShort();
			}
			if ( command.ContainsWrapWidth() )
			{
				command.wrapWidth = bytestream->ReadShort();
			}
			if ( command.ContainsFontName() )
			{
				command.fontName = bytestream->ReadString();
			}
			if ( command.ContainsAlpha() )
			{
				command.alpha = bytestream->ReadLong();
			}
			if ( command.ContainsInTime() )
			{
			}
			if ( command.ContainsOutTime() )
			{
			}
			if ( command.ContainsHUDSize() )
			{
			}
			if ( command.ContainsClippingRectangle() )
			{
			}
			if ( command.ContainsWrapWidth() )
			{
			}
			if ( command.ContainsFontName() )
			{
			}
			if ( command.ContainsAlpha() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "PrintHUDMessage: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNTHING:
		{
			ServerCommands::SpawnThing command;
			int temp45;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp45 = bytestream->ReadShort();
			command.type = NETWORK_GetClassFromIdentification( temp45 );
			command.id = bytestream->ReadShort();

			if ( command.type == NULL )
			{
				CLIENT_PrintWarning( "SpawnThing: unknown class ID for type: %d\n", temp45 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNTHINGNONETID:
		{
			ServerCommands::SpawnThingNoNetID command;
			int temp46;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp46 = bytestream->ReadShort();
			command.type = NETWORK_GetClassFromIdentification( temp46 );

			if ( command.type == NULL )
			{
				CLIENT_PrintWarning( "SpawnThingNoNetID: unknown class ID for type: %d\n", temp46 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnThingNoNetID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNTHINGEXACT:
		{
			ServerCommands::SpawnThingExact command;
			int temp47;
			command.x = bytestream->ReadLong();
			command.y = bytestream->ReadLong();
			command.z = bytestream->ReadLong();
			temp47 = bytestream->ReadShort();
			command.type = NETWORK_GetClassFromIdentification( temp47 );
			command.id = bytestream->ReadShort();

			if ( command.type == NULL )
			{
				CLIENT_PrintWarning( "SpawnThingExact: unknown class ID for type: %d\n", temp47 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnThingExact: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNTHINGEXACTNONETID:
		{
			ServerCommands::SpawnThingExactNoNetID command;
			int temp48;
			command.x = bytestream->ReadLong();
			command.y = bytestream->ReadLong();
			command.z = bytestream->ReadLong();
			temp48 = bytestream->ReadShort();
			command.type = NETWORK_GetClassFromIdentification( temp48 );

			if ( command.type == NULL )
			{
				CLIENT_PrintWarning( "SpawnThingExactNoNetID: unknown class ID for type: %d\n", temp48 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnThingExactNoNetID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MOVETHING:
		{
			ServerCommands::MoveThing command;
			int temp49;
			temp49 = bytestream->ReadShort();
			command.bits = bytestream->ReadShort();
			if ( command.ContainsNewX() )
			{
				command.newX = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsNewY() )
			{
				command.newY = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsNewZ() )
			{
				command.newZ = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsLastX() )
			{
				command.lastX = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsLastY() )
			{
				command.lastY = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsLastZ() )
			{
				command.lastZ = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsAngle() )
			{
				command.angle = bytestream->ReadLong();
			}
			if ( command.ContainsVelX() )
			{
				command.velX = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsVelY() )
			{
				command.velY = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsVelZ() )
			{
				command.velZ = bytestream->ReadShort() << FRACBITS;
			}
			if ( command.ContainsPitch() )
			{
				command.pitch = bytestream->ReadLong();
			}
			if ( command.ContainsMovedir() )
			{
				command.movedir = bytestream->ReadByte();
			}
			if ( CLIENT_ReadActorFromNetID( temp49, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"MoveThing", "actor" ) == false )
			{
				return true;
			}


			if ( command.ContainsNewX() )
			{
			}
			if ( command.ContainsNewY() )
			{
			}
			if ( command.ContainsNewZ() )
			{
			}
			if ( command.ContainsLastX() )
			{
			}
			if ( command.ContainsLastY() )
			{
			}
			if ( command.ContainsLastZ() )
			{
			}
			if ( command.ContainsAngle() )
			{
			}
			if ( command.ContainsVelX() )
			{
			}
			if ( command.ContainsVelY() )
			{
			}
			if ( command.ContainsVelZ() )
			{
			}
			if ( command.ContainsPitch() )
			{
			}
			if ( command.ContainsMovedir() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MoveThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MOVETHINGEXACT:
		{
			ServerCommands::MoveThingExact command;
			int temp50;
			temp50 = bytestream->ReadShort();
			command.bits = bytestream->ReadShort();
			if ( command.ContainsNewX() )
			{
				command.newX = bytestream->ReadLong();
			}
			if ( command.ContainsNewY() )
			{
				command.newY = bytestream->ReadLong();
			}
			if ( command.ContainsNewZ() )
			{
				command.newZ = bytestream->ReadLong();
			}
			if ( command.ContainsLastX() )
			{
				command.lastX = bytestream->ReadLong();
			}
			if ( command.ContainsLastY() )
			{
				command.lastY = bytestream->ReadLong();
			}
			if ( command.ContainsLastZ() )
			{
				command.lastZ = bytestream->ReadLong();
			}
			if ( command.ContainsAngle() )
			{
				command.angle = bytestream->ReadLong();
			}
			if ( command.ContainsVelX() )
			{
				command.velX = bytestream->ReadLong();
			}
			if ( command.ContainsVelY() )
			{
				command.velY = bytestream->ReadLong();
			}
			if ( command.ContainsVelZ() )
			{
				command.velZ = bytestream->ReadLong();
			}
			if ( command.ContainsPitch() )
			{
				command.pitch = bytestream->ReadLong();
			}
			if ( command.ContainsMovedir() )
			{
				command.movedir = bytestream->ReadByte();
			}
			if ( CLIENT_ReadActorFromNetID( temp50, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"MoveThingExact", "actor" ) == false )
			{
				return true;
			}


			if ( command.ContainsNewX() )
			{
			}
			if ( command.ContainsNewY() )
			{
			}
			if ( command.ContainsNewZ() )
			{
			}
			if ( command.ContainsLastX() )
			{
			}
			if ( command.ContainsLastY() )
			{
			}
			if ( command.ContainsLastZ() )
			{
			}
			if ( command.ContainsAngle() )
			{
			}
			if ( command.ContainsVelX() )
			{
			}
			if ( command.ContainsVelY() )
			{
			}
			if ( command.ContainsVelZ() )
			{
			}
			if ( command.ContainsPitch() )
			{
			}
			if ( command.ContainsMovedir() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MoveThingExact: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_KILLTHING:
		{
			ServerCommands::KillThing command;
			int temp51;
			int temp52;
			int temp53;
			temp51 = bytestream->ReadShort();
			command.health = bytestream->ReadVariable();
			command.damageType = bytestream->ReadString();
			temp52 = bytestream->ReadShort();
			temp53 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp51, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.victim ),
											"KillThing", "victim" ) == false )
			{
				return true;
			}


			if ( CLIENT_ReadActorFromNetID( temp52, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.source ),
											"KillThing", "source" ) == false )
			{
				return true;
			}


			if ( CLIENT_ReadActorFromNetID( temp53, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.inflictor ),
											"KillThing", "inflictor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "KillThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGSTATE:
		{
			ServerCommands::SetThingState command;
			int temp54;
			temp54 = bytestream->ReadShort();
			command.state = bytestream->ReadByte();
			if ( CLIENT_ReadActorFromNetID( temp54, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingState", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingState: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGTARGET:
		{
			ServerCommands::SetThingTarget command;
			int temp55;
			int temp56;
			temp55 = bytestream->ReadShort();
			temp56 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp55, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingTarget", "actor" ) == false )
			{
				return true;
			}


			if ( CLIENT_ReadActorFromNetID( temp56, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.target ),
											"SetThingTarget", "target" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingTarget: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DESTROYTHING:
		{
			ServerCommands::DestroyThing command;
			int temp57;
			temp57 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp57, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"DestroyThing", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DestroyThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGANGLE:
		{
			ServerCommands::SetThingAngle command;
			int temp58;
			temp58 = bytestream->ReadShort();
			command.angle = bytestream->ReadShort() << FRACBITS;
			if ( CLIENT_ReadActorFromNetID( temp58, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingAngle", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingAngle: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGANGLEEXACT:
		{
			ServerCommands::SetThingAngleExact command;
			int temp59;
			temp59 = bytestream->ReadShort();
			command.angle = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp59, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingAngleExact", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingAngleExact: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGWATERLEVEL:
		{
			ServerCommands::SetThingWaterLevel command;
			int temp60;
			temp60 = bytestream->ReadShort();
			command.waterlevel = bytestream->ReadByte();
			if ( CLIENT_ReadActorFromNetID( temp60, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingWaterLevel", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingWaterLevel: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGFLAGS:
		{
			ServerCommands::SetThingFlags command;
			int temp61;
			temp61 = bytestream->ReadShort();
			command.flagset = bytestream->ReadByte();
			command.flags = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp61, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingFlags", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingFlags: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGARGUMENTS:
		{
			ServerCommands::SetThingArguments command;
			int temp62;
			temp62 = bytestream->ReadShort();
			command.arg0 = bytestream->ReadLong();
			command.arg1 = bytestream->ReadLong();
			command.arg2 = bytestream->ReadLong();
			command.arg3 = bytestream->ReadLong();
			command.arg4 = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp62, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingArguments", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingArguments: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGTRANSLATION:
		{
			ServerCommands::SetThingTranslation command;
			int temp63;
			temp63 = bytestream->ReadShort();
			command.translation = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp63, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingTranslation", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingTranslation: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGPROPERTY:
		{
			ServerCommands::SetThingProperty command;
			int temp64;
			temp64 = bytestream->ReadShort();
			command.property = bytestream->ReadByte();
			command.value = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp64, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingProperty", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingProperty: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGSOUND:
		{
			ServerCommands::SetThingSound command;
			int temp65;
			temp65 = bytestream->ReadShort();
			command.soundType = bytestream->ReadByte();
			command.sound = bytestream->ReadString();
			if ( CLIENT_ReadActorFromNetID( temp65, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingSound", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingSound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGSPAWNPOINT:
		{
			ServerCommands::SetThingSpawnPoint command;
			int temp66;
			temp66 = bytestream->ReadShort();
			command.spawnPointX = bytestream->ReadLong();
			command.spawnPointY = bytestream->ReadLong();
			command.spawnPointZ = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp66, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingSpawnPoint", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingSpawnPoint: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGSPECIAL1:
		{
			ServerCommands::SetThingSpecial1 command;
			int temp67;
			temp67 = bytestream->ReadShort();
			command.special1 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp67, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingSpecial1", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingSpecial1: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGSPECIAL2:
		{
			ServerCommands::SetThingSpecial2 command;
			int temp68;
			temp68 = bytestream->ReadShort();
			command.special2 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp68, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingSpecial2", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingSpecial2: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGTICS:
		{
			ServerCommands::SetThingTics command;
			int temp69;
			temp69 = bytestream->ReadShort();
			command.tics = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp69, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingTics", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingTics: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGTID:
		{
			ServerCommands::SetThingTID command;
			int temp70;
			temp70 = bytestream->ReadShort();
			command.tid = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp70, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingTID", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingTID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGGRAVITY:
		{
			ServerCommands::SetThingGravity command;
			int temp71;
			temp71 = bytestream->ReadShort();
			command.gravity = bytestream->ReadLong();
			if ( CLIENT_ReadActorFromNetID( temp71, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingGravity", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingGravity: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGFRAME:
		{
			ServerCommands::SetThingFrame command;
			int temp72;
			int temp73;
			temp72 = bytestream->ReadShort();
			temp73 = bytestream->ReadShort();
			command.stateOwner = NETWORK_GetClassFromIdentification( temp73 );

			if ( command.stateOwner->IsDescendantOf( RUNTIME_CLASS( AActor )) == false )
				command.stateOwner = NULL;


			command.offset = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp72, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingFrame", "actor" ) == false )
			{
				return true;
			}



			if ( command.stateOwner == NULL )
			{
				CLIENT_PrintWarning( "SetThingFrame: unknown class ID for stateOwner: %d\n", temp73 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingFrame: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETTHINGFRAMENF:
		{
			ServerCommands::SetThingFrameNF command;
			int temp74;
			int temp75;
			temp74 = bytestream->ReadShort();
			temp75 = bytestream->ReadShort();
			command.stateOwner = NETWORK_GetClassFromIdentification( temp75 );

			if ( command.stateOwner->IsDescendantOf( RUNTIME_CLASS( AActor )) == false )
				command.stateOwner = NULL;


			command.offset = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp74, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingFrameNF", "actor" ) == false )
			{
				return true;
			}



			if ( command.stateOwner == NULL )
			{
				CLIENT_PrintWarning( "SetThingFrameNF: unknown class ID for stateOwner: %d\n", temp75 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingFrameNF: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETWEAPONAMMOGIVE:
		{
			ServerCommands::SetWeaponAmmoGive command;
			int temp76;
			temp76 = bytestream->ReadShort();
			command.ammoGive1 = bytestream->ReadShort();
			command.ammoGive2 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp76, RUNTIME_CLASS( AWeapon ), false,
											reinterpret_cast<AActor *&>( command.weapon ),
											"SetWeaponAmmoGive", "weapon" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetWeaponAmmoGive: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_THINGISCORPSE:
		{
			ServerCommands::ThingIsCorpse command;
			int temp77;
			temp77 = bytestream->ReadShort();
			command.isMonster = bytestream->ReadBit();
			if ( CLIENT_ReadActorFromNetID( temp77, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"ThingIsCorpse", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ThingIsCorpse: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_HIDETHING:
		{
			ServerCommands::HideThing command;
			int temp78;
			temp78 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp78, RUNTIME_CLASS( AInventory ), false,
											reinterpret_cast<AActor *&>( command.item ),
											"HideThing", "item" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "HideThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_TELEPORTTHING:
		{
			ServerCommands::TeleportThing command;
			int temp79;
			temp79 = bytestream->ReadShort();
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			command.momx = bytestream->ReadShort() << FRACBITS;
			command.momy = bytestream->ReadShort() << FRACBITS;
			command.momz = bytestream->ReadShort() << FRACBITS;
			command.reactiontime = bytestream->ReadShort();
			command.angle = bytestream->ReadLong();
			command.sourcefog = bytestream->ReadBit();
			command.destfog = bytestream->ReadBit();
			command.teleportzoom = bytestream->ReadBit();
			if ( CLIENT_ReadActorFromNetID( temp79, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"TeleportThing", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "TeleportThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_THINGACTIVATE:
		{
			ServerCommands::ThingActivate command;
			int temp80;
			int temp81;
			temp80 = bytestream->ReadShort();
			temp81 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp80, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"ThingActivate", "actor" ) == false )
			{
				return true;
			}


			if ( CLIENT_ReadActorFromNetID( temp81, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.activator ),
											"ThingActivate", "activator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ThingActivate: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_THINGDEACTIVATE:
		{
			ServerCommands::ThingDeactivate command;
			int temp82;
			int temp83;
			temp82 = bytestream->ReadShort();
			temp83 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp82, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"ThingDeactivate", "actor" ) == false )
			{
				return true;
			}


			if ( CLIENT_ReadActorFromNetID( temp83, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.activator ),
											"ThingDeactivate", "activator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ThingDeactivate: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_RESPAWNDOOMTHING:
		{
			ServerCommands::RespawnDoomThing command;
			int temp84;
			temp84 = bytestream->ReadShort();
			command.fog = bytestream->ReadBit();
			if ( CLIENT_ReadActorFromNetID( temp84, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"RespawnDoomThing", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "RespawnDoomThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_RESPAWNRAVENTHING:
		{
			ServerCommands::RespawnRavenThing command;
			int temp85;
			temp85 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp85, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"RespawnRavenThing", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "RespawnRavenThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNBLOOD:
		{
			ServerCommands::SpawnBlood command;
			int temp86;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			command.dir = bytestream->ReadShort() << FRACBITS;
			command.damage = bytestream->ReadByte();
			temp86 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp86, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.originator ),
											"SpawnBlood", "originator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnBlood: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNBLOODSPLATTER:
		{
			ServerCommands::SpawnBloodSplatter command;
			int temp87;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp87 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp87, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.originator ),
											"SpawnBloodSplatter", "originator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnBloodSplatter: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNBLOODSPLATTER2:
		{
			ServerCommands::SpawnBloodSplatter2 command;
			int temp88;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp88 = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp88, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.originator ),
											"SpawnBloodSplatter2", "originator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnBloodSplatter2: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNPUFF:
		{
			ServerCommands::SpawnPuff command;
			int temp89;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp89 = bytestream->ReadShort();
			command.pufftype = NETWORK_GetClassFromIdentification( temp89 );
			command.id = bytestream->ReadShort();

			if ( command.pufftype == NULL )
			{
				CLIENT_PrintWarning( "SpawnPuff: unknown class ID for pufftype: %d\n", temp89 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnPuff: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNPUFFNONETID:
		{
			ServerCommands::SpawnPuffNoNetID command;
			int temp90;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp90 = bytestream->ReadShort();
			command.pufftype = NETWORK_GetClassFromIdentification( temp90 );
			command.stateid = bytestream->ReadByte();
			command.receiveTranslation = bytestream->ReadBit();
			if ( command.ContainsTranslation() )
			{
				command.translation = bytestream->ReadLong();
			}

			if ( command.pufftype == NULL )
			{
				CLIENT_PrintWarning( "SpawnPuffNoNetID: unknown class ID for pufftype: %d\n", temp90 );
				return true;
			}


			if ( command.ContainsTranslation() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnPuffNoNetID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORFLOORPLANE:
		{
			ServerCommands::SetSectorFloorPlane command;
			int temp91;
			temp91 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp91 );
			command.height = bytestream->ReadShort() << FRACBITS;

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorFloorPlane: couldn't find sector: %d\n", temp91 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorFloorPlane: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORCEILINGPLANE:
		{
			ServerCommands::SetSectorCeilingPlane command;
			int temp92;
			temp92 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp92 );
			command.height = bytestream->ReadShort() << FRACBITS;

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorCeilingPlane: couldn't find sector: %d\n", temp92 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorCeilingPlane: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORFLOORPLANESLOPE:
		{
			ServerCommands::SetSectorFloorPlaneSlope command;
			int temp93;
			temp93 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp93 );
			command.a = bytestream->ReadShort() << FRACBITS;
			command.b = bytestream->ReadShort() << FRACBITS;
			command.c = bytestream->ReadShort() << FRACBITS;

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorFloorPlaneSlope: couldn't find sector: %d\n", temp93 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorFloorPlaneSlope: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORCEILINGPLANESLOPE:
		{
			ServerCommands::SetSectorCeilingPlaneSlope command;
			int temp94;
			temp94 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp94 );
			command.a = bytestream->ReadShort() << FRACBITS;
			command.b = bytestream->ReadShort() << FRACBITS;
			command.c = bytestream->ReadShort() << FRACBITS;

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorCeilingPlaneSlope: couldn't find sector: %d\n", temp94 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorCeilingPlaneSlope: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORLIGHTLEVEL:
		{
			ServerCommands::SetSectorLightLevel command;
			int temp95;
			temp95 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp95 );
			command.lightLevel = bytestream->ReadShort();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorLightLevel: couldn't find sector: %d\n", temp95 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorLightLevel: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORCOLOR:
		{
			ServerCommands::SetSectorColor command;
			int temp96;
			temp96 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp96 );
			command.red = bytestream->ReadByte();
			command.green = bytestream->ReadByte();
			command.blue = bytestream->ReadByte();
			command.desaturate = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorColor: couldn't find sector: %d\n", temp96 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorColor: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORCOLORBYTAG:
		{
			ServerCommands::SetSectorColorByTag command;
			command.tag = bytestream->ReadShort();
			command.red = bytestream->ReadByte();
			command.green = bytestream->ReadByte();
			command.blue = bytestream->ReadByte();
			command.desaturate = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorColorByTag: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORFADE:
		{
			ServerCommands::SetSectorFade command;
			int temp97;
			temp97 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp97 );
			command.red = bytestream->ReadByte();
			command.green = bytestream->ReadByte();
			command.blue = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorFade: couldn't find sector: %d\n", temp97 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorFade: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORFADEBYTAG:
		{
			ServerCommands::SetSectorFadeByTag command;
			command.tag = bytestream->ReadShort();
			command.red = bytestream->ReadByte();
			command.green = bytestream->ReadByte();
			command.blue = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorFadeByTag: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORFLAT:
		{
			ServerCommands::SetSectorFlat command;
			int temp98;
			temp98 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp98 );
			command.ceilingFlatName = bytestream->ReadString();
			command.floorFlatName = bytestream->ReadString();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorFlat: couldn't find sector: %d\n", temp98 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorFlat: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORPANNING:
		{
			ServerCommands::SetSectorPanning command;
			int temp99;
			temp99 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp99 );
			command.ceilingXOffset = bytestream->ReadLong();
			command.ceilingYOffset = bytestream->ReadLong();
			command.floorXOffset = bytestream->ReadLong();
			command.floorYOffset = bytestream->ReadLong();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorPanning: couldn't find sector: %d\n", temp99 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorPanning: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORROTATION:
		{
			ServerCommands::SetSectorRotation command;
			int temp100;
			temp100 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp100 );
			command.ceilingRotation = bytestream->ReadShort();
			command.floorRotation = bytestream->ReadShort();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorRotation: couldn't find sector: %d\n", temp100 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorRotation: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORROTATIONBYTAG:
		{
			ServerCommands::SetSectorRotationByTag command;
			command.tag = bytestream->ReadShort();
			command.ceilingRotation = bytestream->ReadShort();
			command.floorRotation = bytestream->ReadShort();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorRotationByTag: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORSCALE:
		{
			ServerCommands::SetSectorScale command;
			int temp101;
			temp101 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp101 );
			command.ceilingXScale = bytestream->ReadLong();
			command.ceilingYScale = bytestream->ReadLong();
			command.floorXScale = bytestream->ReadLong();
			command.floorYScale = bytestream->ReadLong();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorScale: couldn't find sector: %d\n", temp101 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorScale: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORSPECIAL:
		{
			ServerCommands::SetSectorSpecial command;
			int temp102;
			temp102 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp102 );
			command.special = bytestream->ReadShort();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorSpecial: couldn't find sector: %d\n", temp102 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorSpecial: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORFRICTION:
		{
			ServerCommands::SetSectorFriction command;
			int temp103;
			temp103 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp103 );
			command.friction = bytestream->ReadLong();
			command.moveFactor = bytestream->ReadLong();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorFriction: couldn't find sector: %d\n", temp103 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorFriction: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORANGLEYOFFSET:
		{
			ServerCommands::SetSectorAngleYOffset command;
			int temp104;
			temp104 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp104 );
			command.ceilingBaseAngle = bytestream->ReadLong();
			command.ceilingBaseYOffset = bytestream->ReadLong();
			command.floorBaseAngle = bytestream->ReadLong();
			command.floorBaseYOffset = bytestream->ReadLong();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorAngleYOffset: couldn't find sector: %d\n", temp104 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorAngleYOffset: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORGRAVITY:
		{
			ServerCommands::SetSectorGravity command;
			int temp105;
			temp105 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp105 );
			command.gravity = bytestream->ReadFloat();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorGravity: couldn't find sector: %d\n", temp105 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorGravity: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORREFLECTION:
		{
			ServerCommands::SetSectorReflection command;
			int temp106;
			temp106 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp106 );
			command.ceilingReflection = bytestream->ReadFloat();
			command.floorReflection = bytestream->ReadFloat();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorReflection: couldn't find sector: %d\n", temp106 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorReflection: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSECTORLINK:
		{
			ServerCommands::SetSectorLink command;
			int temp107;
			temp107 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp107 );
			command.tag = bytestream->ReadShort();
			command.ceiling = bytestream->ReadByte();
			command.moveType = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SetSectorLink: couldn't find sector: %d\n", temp107 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSectorLink: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_STOPSECTORLIGHTEFFECT:
		{
			ServerCommands::StopSectorLightEffect command;
			int temp108;
			temp108 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp108 );

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "StopSectorLightEffect: couldn't find sector: %d\n", temp108 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "StopSectorLightEffect: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DESTROYALLSECTORMOVERS:
		{
			ServerCommands::DestroyAllSectorMovers command;
			command.Execute();
		}
		return true;

	case SVC_STARTSECTORSEQUENCE:
		{
			ServerCommands::StartSectorSequence command;
			int temp109;
			temp109 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp109 );
			command.channel = bytestream->ReadByte();
			command.sequence = bytestream->ReadString();
			command.modeNum = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "StartSectorSequence: couldn't find sector: %d\n", temp109 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "StartSectorSequence: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_STOPSECTORSEQUENCE:
		{
			ServerCommands::StopSectorSequence command;
			int temp110;
			temp110 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp110 );

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "StopSectorSequence: couldn't find sector: %d\n", temp110 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "StopSectorSequence: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTFIREFLICKER:
		{
			ServerCommands::DoSectorLightFireFlicker command;
			int temp111;
			temp111 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp111 );
			command.maxLight = bytestream->ReadShort();
			command.minLight = bytestream->ReadShort();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightFireFlicker: couldn't find sector: %d\n", temp111 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightFireFlicker: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTFLICKER:
		{
			ServerCommands::DoSectorLightFlicker command;
			int temp112;
			temp112 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp112 );
			command.maxLight = bytestream->ReadShort();
			command.minLight = bytestream->ReadShort();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightFlicker: couldn't find sector: %d\n", temp112 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightFlicker: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTLIGHTFLASH:
		{
			ServerCommands::DoSectorLightLightFlash command;
			int temp113;
			temp113 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp113 );
			command.maxLight = bytestream->ReadShort();
			command.minLight = bytestream->ReadShort();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightLightFlash: couldn't find sector: %d\n", temp113 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightLightFlash: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTSTROBE:
		{
			ServerCommands::DoSectorLightStrobe command;
			int temp114;
			temp114 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp114 );
			command.darkTime = bytestream->ReadShort();
			command.brightTime = bytestream->ReadShort();
			command.maxLight = bytestream->ReadShort();
			command.minLight = bytestream->ReadShort();
			command.count = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightStrobe: couldn't find sector: %d\n", temp114 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightStrobe: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTGLOW:
		{
			ServerCommands::DoSectorLightGlow command;
			int temp115;
			temp115 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp115 );

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightGlow: couldn't find sector: %d\n", temp115 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightGlow: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTGLOW2:
		{
			ServerCommands::DoSectorLightGlow2 command;
			int temp116;
			temp116 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp116 );
			command.startLight = bytestream->ReadShort();
			command.endLight = bytestream->ReadShort();
			command.tics = bytestream->ReadShort();
			command.maxTics = bytestream->ReadShort();
			command.oneShot = bytestream->ReadBit();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightGlow2: couldn't find sector: %d\n", temp116 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightGlow2: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_DOSECTORLIGHTPHASED:
		{
			ServerCommands::DoSectorLightPhased command;
			int temp117;
			temp117 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp117 );
			command.baseLevel = bytestream->ReadShort();
			command.phase = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "DoSectorLightPhased: couldn't find sector: %d\n", temp117 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DoSectorLightPhased: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETLINEALPHA:
		{
			ServerCommands::SetLineAlpha command;
			int temp118;
			temp118 = bytestream->ReadShort();
			command.line = CLIENT_FindLineByID( temp118 );
			command.alpha = bytestream->ReadLong();

			if ( command.line == NULL )
			{
				CLIENT_PrintWarning( "SetLineAlpha: couldn't find line: %d\n", temp118 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineAlpha: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETLINETEXTURE:
		{
			ServerCommands::SetLineTexture command;
			int temp119;
			temp119 = bytestream->ReadShort();
			command.line = CLIENT_FindLineByID( temp119 );
			command.textureName = bytestream->ReadString();
			command.side = bytestream->ReadBit();
			command.position = bytestream->ReadByte();

			if ( command.line == NULL )
			{
				CLIENT_PrintWarning( "SetLineTexture: couldn't find line: %d\n", temp119 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineTexture: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETLINETEXTUREBYID:
		{
			ServerCommands::SetLineTextureByID command;
			command.lineID = bytestream->ReadShort();
			command.textureName = bytestream->ReadString();
			command.side = bytestream->ReadBit();
			command.position = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineTextureByID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETLINETEXTUREOFFSET:
		{
			ServerCommands::SetLineTextureOffset command;
			int temp120;
			temp120 = bytestream->ReadShort();
			command.line = CLIENT_FindLineByID( temp120 );
			command.XOffset = bytestream->ReadLong();
			command.YOffset = bytestream->ReadLong();
			command.side = bytestream->ReadBit();
			command.position = bytestream->ReadByte();

			if ( command.line == NULL )
			{
				CLIENT_PrintWarning( "SetLineTextureOffset: couldn't find line: %d\n", temp120 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineTextureOffset: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETLINETEXTURESCALE:
		{
			ServerCommands::SetLineTextureScale command;
			int temp121;
			temp121 = bytestream->ReadShort();
			command.line = CLIENT_FindLineByID( temp121 );
			command.XScale = bytestream->ReadLong();
			command.YScale = bytestream->ReadLong();
			command.side = bytestream->ReadBit();
			command.position = bytestream->ReadByte();

			if ( command.line == NULL )
			{
				CLIENT_PrintWarning( "SetLineTextureScale: couldn't find line: %d\n", temp121 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineTextureScale: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSOMELINEFLAGS:
		{
			ServerCommands::SetSomeLineFlags command;
			int temp122;
			temp122 = bytestream->ReadShort();
			command.line = CLIENT_FindLineByID( temp122 );
			command.blockFlags = bytestream->ReadLong();

			if ( command.line == NULL )
			{
				CLIENT_PrintWarning( "SetSomeLineFlags: couldn't find line: %d\n", temp122 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSomeLineFlags: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SETSIDEFLAGS:
		{
			ServerCommands::SetSideFlags command;
			int temp123;
			temp123 = bytestream->ReadLong();
			command.side = CLIENT_FindSideByID( temp123 );
			command.flags = bytestream->ReadByte();

			if ( command.side == NULL )
			{
				CLIENT_PrintWarning( "SetSideFlags: couldn't find side: %d\n", temp123 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetSideFlags: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SOUND:
		{
			ServerCommands::Sound command;
			command.channel = bytestream->ReadByte();
			command.sound = bytestream->ReadString();
			command.volume = bytestream->ReadByte();
			command.attenuation = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "Sound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SOUNDACTOR:
		{
			ServerCommands::SoundActor command;
			int temp124;
			temp124 = bytestream->ReadShort();
			command.channel = bytestream->ReadShort();
			command.sound = bytestream->ReadString();
			command.volume = bytestream->ReadByte();
			command.attenuation = bytestream->ReadByte();
			if ( CLIENT_ReadActorFromNetID( temp124, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SoundActor", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SoundActor: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SOUNDACTORIFNOTPLAYING:
		{
			ServerCommands::SoundActorIfNotPlaying command;
			int temp125;
			temp125 = bytestream->ReadShort();
			command.channel = bytestream->ReadShort();
			command.sound = bytestream->ReadString();
			command.volume = bytestream->ReadByte();
			command.attenuation = bytestream->ReadByte();
			if ( CLIENT_ReadActorFromNetID( temp125, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SoundActorIfNotPlaying", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SoundActorIfNotPlaying: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SOUNDPOINT:
		{
			ServerCommands::SoundPoint command;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			command.channel = bytestream->ReadByte();
			command.sound = bytestream->ReadString();
			command.volume = bytestream->ReadByte();
			command.attenuation = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SoundPoint: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_ANNOUNCERSOUND:
		{
			ServerCommands::AnnouncerSound command;
			command.sound = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "AnnouncerSound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNMISSILE:
		{
			ServerCommands::SpawnMissile command;
			int temp126;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			command.velX = bytestream->ReadLong();
			command.velY = bytestream->ReadLong();
			command.velZ = bytestream->ReadLong();
			temp126 = bytestream->ReadShort();
			command.missileType = NETWORK_GetClassFromIdentification( temp126 );

			if ( command.missileType->IsDescendantOf( RUNTIME_CLASS( AActor )) == false )
				command.missileType = NULL;


			command.netID = bytestream->ReadShort();
			command.targetNetID = bytestream->ReadShort();

			if ( command.missileType == NULL )
			{
				CLIENT_PrintWarning( "SpawnMissile: unknown class ID for missileType: %d\n", temp126 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnMissile: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_SPAWNMISSILEEXACT:
		{
			ServerCommands::SpawnMissileExact command;
			int temp127;
			command.x = bytestream->ReadLong();
			command.y = bytestream->ReadLong();
			command.z = bytestream->ReadLong();
			command.velX = bytestream->ReadLong();
			command.velY = bytestream->ReadLong();
			command.velZ = bytestream->ReadLong();
			temp127 = bytestream->ReadShort();
			command.missileType = NETWORK_GetClassFromIdentification( temp127 );

			if ( command.missileType->IsDescendantOf( RUNTIME_CLASS( AActor )) == false )
				command.missileType = NULL;


			command.netID = bytestream->ReadShort();
			command.targetNetID = bytestream->ReadShort();

			if ( command.missileType == NULL )
			{
				CLIENT_PrintWarning( "SpawnMissileExact: unknown class ID for missileType: %d\n", temp127 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SpawnMissileExact: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_MISSILEEXPLODE:
		{
			ServerCommands::MissileExplode command;
			int temp128;
			temp128 = bytestream->ReadShort();
			command.lineId = bytestream->ReadShort();
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			if ( CLIENT_ReadActorFromNetID( temp128, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.missile ),
											"MissileExplode", "missile" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "MissileExplode: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_WEAPONSOUND:
		{
			ServerCommands::WeaponSound command;
			int temp129;
			command.player = &players[bytestream->ReadByte()];
			command.sound = bytestream->ReadString();
			temp129 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp129 ) == false )
			{
				CLIENT_PrintWarning( "WeaponSound: Invalid player number: %d\n", temp129 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "WeaponSound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_WEAPONCHANGE:
		{
			ServerCommands::WeaponChange command;
			int temp130;
			int temp131;
			command.player = &players[bytestream->ReadByte()];
			temp130 = bytestream->ReadShort();
			command.weaponType = NETWORK_GetClassFromIdentification( temp130 );

			if ( command.weaponType->IsDescendantOf( RUNTIME_CLASS( AWeapon )) == false )
				command.weaponType = NULL;


			temp131 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp131 ) == false )
			{
				CLIENT_PrintWarning( "WeaponChange: Invalid player number: %d\n", temp131 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;


			if ( command.weaponType == NULL )
			{
				CLIENT_PrintWarning( "WeaponChange: unknown class ID for weaponType: %d\n", temp130 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "WeaponChange: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_WEAPONRAILGUN:
		{
			ServerCommands::WeaponRailgun command;
			int temp132;
				int temp133;
			temp132 = bytestream->ReadShort();
			command.start.X = bytestream->ReadFloat();
			command.start.Y = bytestream->ReadFloat();
			command.start.Z = bytestream->ReadFloat();
			command.end.X = bytestream->ReadFloat();
			command.end.Y = bytestream->ReadFloat();
			command.end.Z = bytestream->ReadFloat();
			command.color1 = bytestream->ReadLong();
			command.color2 = bytestream->ReadLong();
			command.maxdiff = bytestream->ReadFloat();
			command.flags = bytestream->ReadShortByte( 7 );
			command.extended = bytestream->ReadBit();
			if ( command.CheckExtended() )
			{
				command.angleoffset = bytestream->ReadLong();
				temp133 = bytestream->ReadShort();
				command.spawnclass = NETWORK_GetClassFromIdentification( temp133 );
				command.duration = bytestream->ReadShort();
				command.sparsity = bytestream->ReadFloat();
				command.drift = bytestream->ReadFloat();
			}
			if ( CLIENT_ReadActorFromNetID( temp132, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.source ),
											"WeaponRailgun", "source" ) == false )
			{
				return true;
			}


			if ( command.CheckExtended() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "WeaponRailgun: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_ACSSCRIPTEXECUTE:
		{
			ServerCommands::ACSScriptExecute command;
			int temp134;
			command.netid = bytestream->ReadShort();
			temp134 = bytestream->ReadShort();
			command.lineid = bytestream->ReadShort();
			command.levelnum = bytestream->ReadByte();
			command.arg0 = bytestream->ReadVariable();
			command.arg1 = bytestream->ReadVariable();
			command.arg2 = bytestream->ReadVariable();
			command.arg3 = bytestream->ReadVariable();
			command.backSide = bytestream->ReadBit();
			command.always = bytestream->ReadBit();
			if ( CLIENT_ReadActorFromNetID( temp134, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.activator ),
											"ACSScriptExecute", "activator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ACSScriptExecute: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC_REPLACETEXTURES:
		{
			ServerCommands::ReplaceTextures command;
			command.fromTexture = bytestream->ReadString();
			command.toTexture = bytestream->ReadString();
			command.textureFlags = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ReplaceTextures: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	default:
		return false;
	}
}

bool CLIENT_ParseExtendedServerCommand( SVC2 header, BYTESTREAM_s *bytestream )
{
	switch ( header )
	{
	case SVC2_FULLUPDATECOMPLETED:
		{
			ServerCommands::FullUpdateCompleted command;
			command.Execute();
		}
		return true;

	case SVC2_SETMAPNUMTOTALSECRETS:
		{
			ServerCommands::SetMapNumTotalSecrets command;
			command.totalSecrets = bytestream->ReadVariable();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapNumTotalSecrets: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETMAPSKYSCROLLSPEED:
		{
			ServerCommands::SetMapSkyScrollSpeed command;
			command.isSky1 = bytestream->ReadBit();
			command.value = bytestream->ReadFloat();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetMapSkyScrollSpeed: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SECRETFOUND:
		{
			ServerCommands::SecretFound command;
			int temp135;
			temp135 = bytestream->ReadShort();
			command.secretFlags = bytestream->ReadByte();
			if ( CLIENT_ReadActorFromNetID( temp135, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SecretFound", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SecretFound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SECRETMARKSECTORFOUND:
		{
			ServerCommands::SecretMarkSectorFound command;
			int temp136;
			temp136 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp136 );

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SecretMarkSectorFound: couldn't find sector: %d\n", temp136 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SecretMarkSectorFound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_DAMAGEPLAYERWITHTYPE:
		{
			ServerCommands::DamagePlayerWithType command;
			int temp137;
			int temp138;
			command.player = &players[bytestream->ReadByte()];
			command.health = bytestream->ReadVariable();
			command.armor = bytestream->ReadVariable();
			command.damageType = bytestream->ReadString();
			temp137 = bytestream->ReadShort();
			temp138 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp138 ) == false )
			{
				CLIENT_PrintWarning( "DamagePlayerWithType: Invalid player number: %d\n", temp138 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( CLIENT_ReadActorFromNetID( temp137, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.attacker ),
											"DamagePlayerWithType", "attacker" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "DamagePlayerWithType: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETPLAYERCOUNTRY:
		{
			ServerCommands::SetPlayerCountry command;
			int temp139;
			command.player = &players[bytestream->ReadByte()];
			command.country = bytestream->ReadByte();
			temp139 = command.player - players;

			if (( temp139 < 0 ) || ( temp139 >= MAXPLAYERS ))
			{
				CLIENT_PrintWarning( "SetPlayerCountry: Invalid player number: %d\n", temp139 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerCountry: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETPLAYERACCOUNTNAME:
		{
			ServerCommands::SetPlayerAccountName command;
			int temp140;
			command.player = &players[bytestream->ReadByte()];
			command.accountName = bytestream->ReadString();
			temp140 = command.player - players;

			if (( temp140 < 0 ) || ( temp140 >= MAXPLAYERS ))
			{
				CLIENT_PrintWarning( "SetPlayerAccountName: Invalid player number: %d\n", temp140 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerAccountName: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETPLAYERDEATHS:
		{
			ServerCommands::SetPlayerDeaths command;
			int temp141;
			command.player = &players[bytestream->ReadByte()];
			command.deaths = bytestream->ReadVariable();
			temp141 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp141 ) == false )
			{
				CLIENT_PrintWarning( "SetPlayerDeaths: Invalid player number: %d\n", temp141 );
				return true;
			}

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetPlayerDeaths: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETLOCALPLAYERJUMPTICS:
		{
			ServerCommands::SetLocalPlayerJumpTics command;
			command.clientTicOnServerEnd = bytestream->ReadLong();
			command.jumpTics = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLocalPlayerJumpTics: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_GIVEWEAPONHOLDER:
		{
			ServerCommands::GiveWeaponHolder command;
			int temp142;
			int temp143;
			command.player = &players[bytestream->ReadByte()];
			command.pieceMask = bytestream->ReadShort();
			temp142 = bytestream->ReadShort();
			command.pieceWeapon = NETWORK_GetClassFromIdentification( temp142 );
			temp143 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp143 ) == false )
			{
				CLIENT_PrintWarning( "GiveWeaponHolder: Invalid player number: %d\n", temp143 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;


			if ( command.pieceWeapon == NULL )
			{
				CLIENT_PrintWarning( "GiveWeaponHolder: unknown class ID for pieceWeapon: %d\n", temp142 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "GiveWeaponHolder: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETHEXENARMORSLOTS:
		{
			ServerCommands::SetHexenArmorSlots command;
			int temp144;
			command.player = &players[bytestream->ReadByte()];
			command.slot0 = bytestream->ReadLong();
			command.slot1 = bytestream->ReadLong();
			command.slot2 = bytestream->ReadLong();
			command.slot3 = bytestream->ReadLong();
			command.slot4 = bytestream->ReadLong();
			temp144 = command.player - players;

			if ( PLAYER_IsValidPlayer( temp144 ) == false )
			{
				CLIENT_PrintWarning( "SetHexenArmorSlots: Invalid player number: %d\n", temp144 );
				return true;
			}


			if ( command.player->mo == NULL )
				return true;

			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetHexenArmorSlots: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_LEVELSPAWNTHING:
		{
			ServerCommands::LevelSpawnThing command;
			int temp145;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp145 = bytestream->ReadShort();
			command.type = NETWORK_GetClassFromIdentification( temp145 );
			command.id = bytestream->ReadShort();

			if ( command.type == NULL )
			{
				CLIENT_PrintWarning( "LevelSpawnThing: unknown class ID for type: %d\n", temp145 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "LevelSpawnThing: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_LEVELSPAWNTHINGNONETID:
		{
			ServerCommands::LevelSpawnThingNoNetID command;
			int temp146;
			command.x = bytestream->ReadShort() << FRACBITS;
			command.y = bytestream->ReadShort() << FRACBITS;
			command.z = bytestream->ReadShort() << FRACBITS;
			temp146 = bytestream->ReadShort();
			command.type = NETWORK_GetClassFromIdentification( temp146 );

			if ( command.type == NULL )
			{
				CLIENT_PrintWarning( "LevelSpawnThingNoNetID: unknown class ID for type: %d\n", temp146 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "LevelSpawnThingNoNetID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETTHINGSTRINGPROPERTY:
		{
			ServerCommands::SetThingStringProperty command;
			int temp147;
			temp147 = bytestream->ReadShort();
			command.property = bytestream->ReadByte();
			command.value = bytestream->ReadString();
			if ( CLIENT_ReadActorFromNetID( temp147, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingStringProperty", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingStringProperty: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETTHINGREACTIONTIME:
		{
			ServerCommands::SetThingReactionTime command;
			int temp148;
			temp148 = bytestream->ReadShort();
			command.reactiontime = bytestream->ReadShort();
			if ( CLIENT_ReadActorFromNetID( temp148, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingReactionTime", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingReactionTime: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETTHINGSCALE:
		{
			ServerCommands::SetThingScale command;
			int temp149;
			temp149 = bytestream->ReadShort();
			command.scaleflags = bytestream->ReadByte();
			if ( command.ContainsScaleX() )
			{
				command.scaleX = bytestream->ReadLong();
			}
			if ( command.ContainsScaleY() )
			{
				command.scaleY = bytestream->ReadLong();
			}
			if ( CLIENT_ReadActorFromNetID( temp149, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"SetThingScale", "actor" ) == false )
			{
				return true;
			}


			if ( command.ContainsScaleX() )
			{
			}
			if ( command.ContainsScaleY() )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetThingScale: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETLINETEXTUREOFFSETBYID:
		{
			ServerCommands::SetLineTextureOffsetByID command;
			command.lineID = bytestream->ReadShort();
			command.XOffset = bytestream->ReadLong();
			command.YOffset = bytestream->ReadLong();
			command.side = bytestream->ReadBit();
			command.flags = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineTextureOffsetByID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETLINETEXTURESCALEBYID:
		{
			ServerCommands::SetLineTextureScaleByID command;
			command.lineID = bytestream->ReadShort();
			command.XScale = bytestream->ReadLong();
			command.YScale = bytestream->ReadLong();
			command.side = bytestream->ReadBit();
			command.flags = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetLineTextureScaleByID: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SOUNDSECTOR:
		{
			ServerCommands::SoundSector command;
			int temp150;
			temp150 = bytestream->ReadShort();
			command.sector = CLIENT_FindSectorByID( temp150 );
			command.channel = bytestream->ReadShort();
			command.sound = bytestream->ReadString();
			command.volume = bytestream->ReadByte();
			command.attenuation = bytestream->ReadByte();

			if ( command.sector == NULL )
			{
				CLIENT_PrintWarning( "SoundSector: couldn't find sector: %d\n", temp150 );
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SoundSector: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_STOPSOUND:
		{
			ServerCommands::StopSound command;
			int temp151;
			temp151 = bytestream->ReadShort();
			command.channel = bytestream->ReadByte();
			if ( CLIENT_ReadActorFromNetID( temp151, RUNTIME_CLASS( AActor ), false,
											reinterpret_cast<AActor *&>( command.actor ),
											"StopSound", "actor" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "StopSound: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_ACSSENDSTRING:
		{
			ServerCommands::ACSSendString command;
			int temp152;
			command.netid = bytestream->ReadShort();
			temp152 = bytestream->ReadShort();
			command.string = bytestream->ReadString();
			if ( CLIENT_ReadActorFromNetID( temp152, RUNTIME_CLASS( AActor ), true,
											reinterpret_cast<AActor *&>( command.activator ),
											"ACSSendString", "activator" ) == false )
			{
				return true;
			}


			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ACSSendString: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SYNCJOINQUEUE:
		{
			ServerCommands::SyncJoinQueue command;
			unsigned int temp153;
			temp153 = bytestream->ReadByte();
			command.slots.Reserve( temp153 );
			for ( unsigned int i = 0; i < temp153; ++i )
			{
				command.slots[i].player = bytestream->ReadByte();
				command.slots[i].team = bytestream->ReadByte();
			}
			for ( unsigned int i = 0; i < command.slots.Size(); ++i )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SyncJoinQueue: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SYNCMAPROTATION:
		{
			ServerCommands::SyncMapRotation command;
			unsigned int temp154;
			temp154 = bytestream->ReadByte();
			command.entries.Reserve( temp154 );
			for ( unsigned int i = 0; i < temp154; ++i )
			{
				command.entries[i].name = bytestream->ReadString();
				command.entries[i].isUsed = bytestream->ReadByte();
				command.entries[i].minPlayers = bytestream->ReadByte();
				command.entries[i].maxPlayers = bytestream->ReadByte();
			}
			command.currentPosition = bytestream->ReadShort();
			for ( unsigned int i = 0; i < command.entries.Size(); ++i )
			{
			}
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SyncMapRotation: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_SETCUSTOMPLAYERVALUE:
		{
			ServerCommands::SetCustomPlayerValue command;
			command.index = bytestream->ReadByte();
			command.player = bytestream->ReadByte();
			command.value = bytestream->ReadString();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "SetCustomPlayerValue: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	case SVC2_RESETCUSTOMPLAYERVALUE:
		{
			ServerCommands::ResetCustomPlayerValue command;
			command.index = bytestream->ReadByte();
			command.player = bytestream->ReadByte();
			if ( bytestream->pbStream > bytestream->pbStreamEnd )
			{
				CLIENT_PrintWarning( "ResetCustomPlayerValue: Packet contained %td too few bytes\n",
					bytestream->pbStream - bytestream->pbStreamEnd );
				return true;
			}

			command.Execute();
		}
		return true;

	default:
		return false;
	}
}
NetCommand ServerCommands::Header::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: Header::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_HEADER );
	command.addLong( this->sequence );
	return command;
}

void ServerCommands::Header::SetSequence( int value )
{
	this->sequence = value;
	this->_sequenceInitialized = true;
}

NetCommand ServerCommands::Ping::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: Ping::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PING );
	command.setUnreliable( true );
	command.addLong( this->time );
	return command;
}

void ServerCommands::Ping::SetTime( unsigned int value )
{
	this->time = value;
	this->_timeInitialized = true;
}

NetCommand ServerCommands::BeginSnapshot::BuildNetCommand() const
{
	NetCommand command ( SVC_BEGINSNAPSHOT );
	return command;
}

NetCommand ServerCommands::EndSnapshot::BuildNetCommand() const
{
	NetCommand command ( SVC_ENDSNAPSHOT );
	return command;
}

NetCommand ServerCommands::FullUpdateCompleted::BuildNetCommand() const
{
	NetCommand command ( SVC2_FULLUPDATECOMPLETED );
	return command;
}

NetCommand ServerCommands::MapLoad::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MapLoad::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MAPLOAD );
	command.addString( this->mapName );
	command.addShort( this->currentPosition );
	return command;
}

void ServerCommands::MapLoad::SetMapName( const FString & value )
{
	this->mapName = value;
	this->_mapNameInitialized = true;
}

void ServerCommands::MapLoad::SetCurrentPosition( int value )
{
	this->currentPosition = value;
	this->_currentPositionInitialized = true;
}

NetCommand ServerCommands::MapNew::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MapNew::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MAPNEW );
	command.addString( this->mapName );
	return command;
}

void ServerCommands::MapNew::SetMapName( const FString & value )
{
	this->mapName = value;
	this->_mapNameInitialized = true;
}

NetCommand ServerCommands::MapExit::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MapExit::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MAPEXIT );
	command.addByte( this->position );
	command.addString( this->nextMap );
	return command;
}

void ServerCommands::MapExit::SetPosition( int value )
{
	this->position = value;
	this->_positionInitialized = true;
}

void ServerCommands::MapExit::SetNextMap( const FString & value )
{
	this->nextMap = value;
	this->_nextMapInitialized = true;
}

NetCommand ServerCommands::MapAuthenticate::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MapAuthenticate::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MAPAUTHENTICATE );
	command.addString( this->mapName );
	return command;
}

void ServerCommands::MapAuthenticate::SetMapName( const FString & value )
{
	this->mapName = value;
	this->_mapNameInitialized = true;
}

NetCommand ServerCommands::SetMapTime::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapTime::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPTIME );
	command.addLong( this->time );
	return command;
}

void ServerCommands::SetMapTime::SetTime( int value )
{
	this->time = value;
	this->_timeInitialized = true;
}

NetCommand ServerCommands::SetMapNumKilledMonsters::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapNumKilledMonsters::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPNUMKILLEDMONSTERS );
	command.addVariable( this->killedMonsters );
	return command;
}

void ServerCommands::SetMapNumKilledMonsters::SetKilledMonsters( int value )
{
	this->killedMonsters = value;
	this->_killedMonstersInitialized = true;
}

NetCommand ServerCommands::SetMapNumFoundItems::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapNumFoundItems::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPNUMFOUNDITEMS );
	command.addVariable( this->foundItems );
	return command;
}

void ServerCommands::SetMapNumFoundItems::SetFoundItems( int value )
{
	this->foundItems = value;
	this->_foundItemsInitialized = true;
}

NetCommand ServerCommands::SetMapNumFoundSecrets::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapNumFoundSecrets::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPNUMFOUNDSECRETS );
	command.addVariable( this->foundSecrets );
	return command;
}

void ServerCommands::SetMapNumFoundSecrets::SetFoundSecrets( int value )
{
	this->foundSecrets = value;
	this->_foundSecretsInitialized = true;
}

NetCommand ServerCommands::SetMapNumTotalMonsters::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapNumTotalMonsters::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPNUMTOTALMONSTERS );
	command.addVariable( this->totalMonsters );
	return command;
}

void ServerCommands::SetMapNumTotalMonsters::SetTotalMonsters( int value )
{
	this->totalMonsters = value;
	this->_totalMonstersInitialized = true;
}

NetCommand ServerCommands::SetMapNumTotalItems::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapNumTotalItems::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPNUMTOTALITEMS );
	command.addVariable( this->totalItems );
	return command;
}

void ServerCommands::SetMapNumTotalItems::SetTotalItems( int value )
{
	this->totalItems = value;
	this->_totalItemsInitialized = true;
}

NetCommand ServerCommands::SetMapNumTotalSecrets::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapNumTotalSecrets::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETMAPNUMTOTALSECRETS );
	command.addVariable( this->totalSecrets );
	return command;
}

void ServerCommands::SetMapNumTotalSecrets::SetTotalSecrets( int value )
{
	this->totalSecrets = value;
	this->_totalSecretsInitialized = true;
}

NetCommand ServerCommands::SetMapMusic::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapMusic::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPMUSIC );
	command.addString( this->music );
	command.addByte( this->order );
	return command;
}

void ServerCommands::SetMapMusic::SetMusic( const FString & value )
{
	this->music = value;
	this->_musicInitialized = true;
}

void ServerCommands::SetMapMusic::SetOrder( int value )
{
	this->order = value;
	this->_orderInitialized = true;
}

NetCommand ServerCommands::SetMapSky::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapSky::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETMAPSKY );
	command.addString( this->sky1 );
	command.addString( this->sky2 );
	return command;
}

void ServerCommands::SetMapSky::SetSky1( const FString & value )
{
	this->sky1 = value;
	this->_sky1Initialized = true;
}

void ServerCommands::SetMapSky::SetSky2( const FString & value )
{
	this->sky2 = value;
	this->_sky2Initialized = true;
}

NetCommand ServerCommands::SetMapSkyScrollSpeed::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetMapSkyScrollSpeed::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETMAPSKYSCROLLSPEED );
	command.addBit( this->isSky1 );
	command.addFloat( this->value );
	return command;
}

void ServerCommands::SetMapSkyScrollSpeed::SetIsSky1( bool value )
{
	this->isSky1 = value;
	this->_isSky1Initialized = true;
}

void ServerCommands::SetMapSkyScrollSpeed::SetValue( float value )
{
	this->value = value;
	this->_valueInitialized = true;
}

NetCommand ServerCommands::SecretFound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SecretFound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SECRETFOUND );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->secretFlags );
	return command;
}

void ServerCommands::SecretFound::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SecretFound::SetSecretFlags( int value )
{
	this->secretFlags = value;
	this->_secretFlagsInitialized = true;
}

NetCommand ServerCommands::SecretMarkSectorFound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SecretMarkSectorFound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SECRETMARKSECTORFOUND );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	return command;
}

void ServerCommands::SecretMarkSectorFound::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

NetCommand ServerCommands::DoScroller::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoScroller::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSCROLLER );
	command.addByte( this->type );
	command.addLong( this->x );
	command.addLong( this->y );
	command.addLong( this->affectee );
	command.addBit( this->accel );
	command.addBit( this->hasControl );
	command.addBit( this->hasPos );
	if ( ContainsSector() )
	{
		command.addShort( this->sector ? this->sector - sectors : -1 );
	}
	if ( ContainsPos() )
	{
		command.addByte( this->pos );
	}
	return command;
}

void ServerCommands::DoScroller::SetType( int value )
{
	this->type = value;
	this->_typeInitialized = true;
}

void ServerCommands::DoScroller::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::DoScroller::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::DoScroller::SetAffectee( int value )
{
	this->affectee = value;
	this->_affecteeInitialized = true;
}

void ServerCommands::DoScroller::SetAccel( bool value )
{
	this->accel = value;
	this->_accelInitialized = true;
}

void ServerCommands::DoScroller::SetHasControl( bool value )
{
	this->hasControl = value;
	this->_hasControlInitialized = true;
}

void ServerCommands::DoScroller::SetHasPos( bool value )
{
	this->hasPos = value;
	this->_hasPosInitialized = true;
}

void ServerCommands::DoScroller::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoScroller::SetPos( int value )
{
	this->pos = value;
	this->_posInitialized = true;
}

bool ServerCommands::DoScroller::ContainsSector() const
{
	return !!( hasControl );
}

bool ServerCommands::DoScroller::ContainsPos() const
{
	return !!( hasPos );
}

NetCommand ServerCommands::SpawnPlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnPlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNPLAYER );
	command.addByte( this->player - players );
	command.addShortByte( this->priorState, 4 );
	command.addBit( this->isBot );
	command.addBit( this->isSpectating );
	command.addBit( this->isDeadSpectator );
	command.addBit( this->isMorphed );
	command.addShort( this->netid );
	command.addLong( this->angle );
	command.addLong( this->x );
	command.addLong( this->y );
	command.addLong( this->z );
	command.addByte( this->playerClass );
	if ( CheckIsMorphed() )
	{
		command.addShort( this->morphStyle );
		command.addShort( this->morphedClass ? this->morphedClass->getActorNetworkIndex() : -1 );
	}
	return command;
}

void ServerCommands::SpawnPlayer::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SpawnPlayer::SetPriorState( int value )
{
	this->priorState = value;
	this->_priorStateInitialized = true;
}

void ServerCommands::SpawnPlayer::SetIsBot( bool value )
{
	this->isBot = value;
	this->_isBotInitialized = true;
}

void ServerCommands::SpawnPlayer::SetIsSpectating( bool value )
{
	this->isSpectating = value;
	this->_isSpectatingInitialized = true;
}

void ServerCommands::SpawnPlayer::SetIsDeadSpectator( bool value )
{
	this->isDeadSpectator = value;
	this->_isDeadSpectatorInitialized = true;
}

void ServerCommands::SpawnPlayer::SetIsMorphed( bool value )
{
	this->isMorphed = value;
	this->_isMorphedInitialized = true;
}

void ServerCommands::SpawnPlayer::SetNetid( int value )
{
	this->netid = value;
	this->_netidInitialized = true;
}

void ServerCommands::SpawnPlayer::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

void ServerCommands::SpawnPlayer::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnPlayer::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnPlayer::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnPlayer::SetPlayerClass( int value )
{
	this->playerClass = value;
	this->_playerClassInitialized = true;
}

void ServerCommands::SpawnPlayer::SetMorphStyle( int value )
{
	this->morphStyle = value;
	this->_morphStyleInitialized = true;
}

void ServerCommands::SpawnPlayer::SetMorphedClass( const PClass * value )
{
	this->morphedClass = value;
	this->_morphedClassInitialized = true;
}

bool ServerCommands::SpawnPlayer::CheckIsMorphed() const
{
	return !!( isMorphed );
}

NetCommand ServerCommands::MovePlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MovePlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MOVEPLAYER );
	command.setUnreliable( true );
	command.addByte( this->player - players );
	command.addByte( this->flags );
	if ( IsVisible() )
	{
		command.addLong( this->x );
		command.addLong( this->y );
		command.addShort( this->z >> FRACBITS );
		command.addLong( this->angle );
	}
	if ( IsMovingX() )
	{
		command.addShort( this->velx >> FRACBITS );
	}
	if ( IsMovingY() )
	{
		command.addShort( this->vely >> FRACBITS );
	}
	if ( IsMovingZ() )
	{
		command.addShort( this->velz >> FRACBITS );
	}
	return command;
}

void ServerCommands::MovePlayer::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::MovePlayer::SetFlags( int value )
{
	this->flags = value;
	this->_flagsInitialized = true;
}

void ServerCommands::MovePlayer::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::MovePlayer::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::MovePlayer::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::MovePlayer::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

void ServerCommands::MovePlayer::SetVelx( fixed_t value )
{
	this->velx = value;
	this->_velxInitialized = true;
}

void ServerCommands::MovePlayer::SetVely( fixed_t value )
{
	this->vely = value;
	this->_velyInitialized = true;
}

void ServerCommands::MovePlayer::SetVelz( fixed_t value )
{
	this->velz = value;
	this->_velzInitialized = true;
}

bool ServerCommands::MovePlayer::IsMovingX() const
{
	return !!( (flags & PLAYER_VISIBLE) && (flags & PLAYER_SENDVELX) );
}

bool ServerCommands::MovePlayer::IsMovingY() const
{
	return !!( (flags & PLAYER_VISIBLE) && (flags & PLAYER_SENDVELY) );
}

bool ServerCommands::MovePlayer::IsMovingZ() const
{
	return !!( (flags & PLAYER_VISIBLE) && (flags & PLAYER_SENDVELZ) );
}

bool ServerCommands::MovePlayer::IsVisible() const
{
	return !!( flags & PLAYER_VISIBLE );
}

NetCommand ServerCommands::DamagePlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DamagePlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DAMAGEPLAYER );
	command.addByte( this->player - players );
	command.addVariable( this->health );
	command.addVariable( this->armor );
	command.addShort( this->attacker ? this->attacker->NetID : -1 );
	return command;
}

void ServerCommands::DamagePlayer::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::DamagePlayer::SetHealth( int value )
{
	this->health = value;
	this->_healthInitialized = true;
}

void ServerCommands::DamagePlayer::SetArmor( int value )
{
	this->armor = value;
	this->_armorInitialized = true;
}

void ServerCommands::DamagePlayer::SetAttacker( AActor * value )
{
	this->attacker = value;
	this->_attackerInitialized = true;
}

NetCommand ServerCommands::DamagePlayerWithType::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DamagePlayerWithType::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_DAMAGEPLAYERWITHTYPE );
	command.addByte( this->player - players );
	command.addVariable( this->health );
	command.addVariable( this->armor );
	command.addString( this->damageType );
	command.addShort( this->attacker ? this->attacker->NetID : -1 );
	return command;
}

void ServerCommands::DamagePlayerWithType::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::DamagePlayerWithType::SetHealth( int value )
{
	this->health = value;
	this->_healthInitialized = true;
}

void ServerCommands::DamagePlayerWithType::SetArmor( int value )
{
	this->armor = value;
	this->_armorInitialized = true;
}

void ServerCommands::DamagePlayerWithType::SetDamageType( const FString & value )
{
	this->damageType = value;
	this->_damageTypeInitialized = true;
}

void ServerCommands::DamagePlayerWithType::SetAttacker( AActor * value )
{
	this->attacker = value;
	this->_attackerInitialized = true;
}

NetCommand ServerCommands::KillPlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: KillPlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_KILLPLAYER );
	command.addByte( this->player - players );
	command.addShort( this->source ? this->source->NetID : -1 );
	command.addShort( this->inflictor ? this->inflictor->NetID : -1 );
	command.addShort( this->health );
	command.addString( this->MOD );
	command.addString( this->damageType );
	command.addShort( this->weaponType ? this->weaponType->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::KillPlayer::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::KillPlayer::SetSource( AActor * value )
{
	this->source = value;
	this->_sourceInitialized = true;
}

void ServerCommands::KillPlayer::SetInflictor( AActor * value )
{
	this->inflictor = value;
	this->_inflictorInitialized = true;
}

void ServerCommands::KillPlayer::SetHealth( int value )
{
	this->health = value;
	this->_healthInitialized = true;
}

void ServerCommands::KillPlayer::SetMOD( const FString & value )
{
	this->MOD = value;
	this->_MODInitialized = true;
}

void ServerCommands::KillPlayer::SetDamageType( const FString & value )
{
	this->damageType = value;
	this->_damageTypeInitialized = true;
}

void ServerCommands::KillPlayer::SetWeaponType( const PClass * value )
{
	this->weaponType = value;
	this->_weaponTypeInitialized = true;
}

NetCommand ServerCommands::SetPlayerHealth::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerHealth::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERHEALTH );
	command.addByte( this->player - players );
	command.addVariable( this->health );
	return command;
}

void ServerCommands::SetPlayerHealth::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerHealth::SetHealth( int value )
{
	this->health = value;
	this->_healthInitialized = true;
}

NetCommand ServerCommands::SetPlayerArmor::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerArmor::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERARMOR );
	command.addByte( this->player - players );
	command.addVariable( this->armorAmount );
	command.addString( this->armorIcon );
	return command;
}

void ServerCommands::SetPlayerArmor::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerArmor::SetArmorAmount( int value )
{
	this->armorAmount = value;
	this->_armorAmountInitialized = true;
}

void ServerCommands::SetPlayerArmor::SetArmorIcon( const FString & value )
{
	this->armorIcon = value;
	this->_armorIconInitialized = true;
}

NetCommand ServerCommands::SetPlayerState::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerState::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERSTATE );
	command.addByte( this->player - players );
	command.addByte( this->state );
	return command;
}

void ServerCommands::SetPlayerState::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerState::SetState( int value )
{
	this->state = value;
	this->_stateInitialized = true;
}

NetCommand ServerCommands::SetPlayerUserInfo::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerUserInfo::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERUSERINFO );
	command.addByte( this->player - players );
	command.addByte( cvars.Size() );
	for ( unsigned int i = 0; i < cvars.Size(); ++i )
	{
		command.addName( this->cvars[i].name );
		command.addString( this->cvars[i].value );
	}
	return command;
}

void ServerCommands::SetPlayerUserInfo::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerUserInfo::SetCvars( const TArray<struct CVar> & value )
{
	this->cvars = value;
	this->_cvarsInitialized = true;
}

NetCommand ServerCommands::SetPlayerCountry::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerCountry::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETPLAYERCOUNTRY );
	command.addByte( this->player - players );
	command.addByte( this->country );
	return command;
}

void ServerCommands::SetPlayerCountry::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerCountry::SetCountry( int value )
{
	this->country = value;
	this->_countryInitialized = true;
}

NetCommand ServerCommands::SetPlayerAccountName::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerAccountName::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETPLAYERACCOUNTNAME );
	command.addByte( this->player - players );
	command.addString( this->accountName );
	return command;
}

void ServerCommands::SetPlayerAccountName::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerAccountName::SetAccountName( const FString & value )
{
	this->accountName = value;
	this->_accountNameInitialized = true;
}

NetCommand ServerCommands::SetPlayerFrags::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerFrags::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERFRAGS );
	command.addByte( this->player - players );
	command.addVariable( this->fragCount );
	return command;
}

void ServerCommands::SetPlayerFrags::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerFrags::SetFragCount( int value )
{
	this->fragCount = value;
	this->_fragCountInitialized = true;
}

NetCommand ServerCommands::SetPlayerPoints::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerPoints::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERPOINTS );
	command.addByte( this->player - players );
	command.addVariable( this->pointCount );
	return command;
}

void ServerCommands::SetPlayerPoints::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerPoints::SetPointCount( int value )
{
	this->pointCount = value;
	this->_pointCountInitialized = true;
}

NetCommand ServerCommands::SetPlayerWins::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerWins::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERWINS );
	command.addByte( this->player - players );
	command.addVariable( this->wins );
	return command;
}

void ServerCommands::SetPlayerWins::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerWins::SetWins( int value )
{
	this->wins = value;
	this->_winsInitialized = true;
}

NetCommand ServerCommands::SetPlayerDeaths::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerDeaths::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETPLAYERDEATHS );
	command.addByte( this->player - players );
	command.addVariable( this->deaths );
	return command;
}

void ServerCommands::SetPlayerDeaths::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerDeaths::SetDeaths( int value )
{
	this->deaths = value;
	this->_deathsInitialized = true;
}

NetCommand ServerCommands::SetPlayerKillCount::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerKillCount::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERKILLCOUNT );
	command.addByte( this->player - players );
	command.addVariable( this->killCount );
	return command;
}

void ServerCommands::SetPlayerKillCount::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerKillCount::SetKillCount( int value )
{
	this->killCount = value;
	this->_killCountInitialized = true;
}

NetCommand ServerCommands::SetPlayerStatus::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerStatus::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERSTATUS );
	command.addByte( this->player - players );
	command.addShortByte( this->type, 7 );
	command.addBit( this->value );
	return command;
}

void ServerCommands::SetPlayerStatus::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerStatus::SetType( int value )
{
	this->type = value;
	this->_typeInitialized = true;
}

void ServerCommands::SetPlayerStatus::SetValue( bool value )
{
	this->value = value;
	this->_valueInitialized = true;
}

NetCommand ServerCommands::SetPlayerTeam::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerTeam::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERTEAM );
	command.addByte( this->player - players );
	command.addByte( this->team );
	return command;
}

void ServerCommands::SetPlayerTeam::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerTeam::SetTeam( int value )
{
	this->team = value;
	this->_teamInitialized = true;
}

NetCommand ServerCommands::SetPlayerCamera::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerCamera::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERCAMERA );
	command.addShort( this->camera ? this->camera->NetID : -1 );
	command.addBit( this->revertPlease );
	return command;
}

void ServerCommands::SetPlayerCamera::SetCamera( AActor * value )
{
	this->camera = value;
	this->_cameraInitialized = true;
}

void ServerCommands::SetPlayerCamera::SetRevertPlease( bool value )
{
	this->revertPlease = value;
	this->_revertPleaseInitialized = true;
}

NetCommand ServerCommands::SetPlayerPoisonCount::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerPoisonCount::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERPOISONCOUNT );
	command.addByte( this->player - players );
	command.addShort( this->poisonCount );
	return command;
}

void ServerCommands::SetPlayerPoisonCount::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerPoisonCount::SetPoisonCount( int value )
{
	this->poisonCount = value;
	this->_poisonCountInitialized = true;
}

NetCommand ServerCommands::SetPlayerAmmoCapacity::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerAmmoCapacity::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERAMMOCAPACITY );
	command.addByte( this->player - players );
	command.addShort( this->ammoType ? this->ammoType->getActorNetworkIndex() : -1 );
	command.addLong( this->maxAmount );
	return command;
}

void ServerCommands::SetPlayerAmmoCapacity::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerAmmoCapacity::SetAmmoType( const PClass * value )
{
	this->ammoType = value;
	this->_ammoTypeInitialized = true;
}

void ServerCommands::SetPlayerAmmoCapacity::SetMaxAmount( int value )
{
	this->maxAmount = value;
	this->_maxAmountInitialized = true;
}

NetCommand ServerCommands::SetPlayerCheats::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerCheats::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERCHEATS );
	command.addByte( this->player - players );
	command.addLong( this->cheats );
	return command;
}

void ServerCommands::SetPlayerCheats::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerCheats::SetCheats( int value )
{
	this->cheats = value;
	this->_cheatsInitialized = true;
}

NetCommand ServerCommands::SetPlayerPendingWeapon::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerPendingWeapon::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERPENDINGWEAPON );
	command.addByte( this->player - players );
	command.addShort( this->weaponType ? this->weaponType->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::SetPlayerPendingWeapon::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerPendingWeapon::SetWeaponType( const PClass * value )
{
	this->weaponType = value;
	this->_weaponTypeInitialized = true;
}

NetCommand ServerCommands::SetPlayerPSprite::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerPSprite::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERPSPRITE );
	command.addByte( this->player - players );
	command.addShort( this->stateOwner ? this->stateOwner->getActorNetworkIndex() : -1 );
	command.addShort( this->offset );
	command.addByte( this->position );
	return command;
}

void ServerCommands::SetPlayerPSprite::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerPSprite::SetStateOwner( const PClass * value )
{
	this->stateOwner = value;
	this->_stateOwnerInitialized = true;
}

void ServerCommands::SetPlayerPSprite::SetOffset( int value )
{
	this->offset = value;
	this->_offsetInitialized = true;
}

void ServerCommands::SetPlayerPSprite::SetPosition( int value )
{
	this->position = value;
	this->_positionInitialized = true;
}

NetCommand ServerCommands::SetPlayerBlend::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerBlend::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERBLEND );
	command.addByte( this->player - players );
	command.addFloat( this->blendR );
	command.addFloat( this->blendG );
	command.addFloat( this->blendB );
	command.addFloat( this->blendA );
	return command;
}

void ServerCommands::SetPlayerBlend::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerBlend::SetBlendR( float value )
{
	this->blendR = value;
	this->_blendRInitialized = true;
}

void ServerCommands::SetPlayerBlend::SetBlendG( float value )
{
	this->blendG = value;
	this->_blendGInitialized = true;
}

void ServerCommands::SetPlayerBlend::SetBlendB( float value )
{
	this->blendB = value;
	this->_blendBInitialized = true;
}

void ServerCommands::SetPlayerBlend::SetBlendA( float value )
{
	this->blendA = value;
	this->_blendAInitialized = true;
}

NetCommand ServerCommands::SetPlayerMaxHealth::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerMaxHealth::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERMAXHEALTH );
	command.addByte( this->player - players );
	command.addLong( this->maxHealth );
	return command;
}

void ServerCommands::SetPlayerMaxHealth::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerMaxHealth::SetMaxHealth( int value )
{
	this->maxHealth = value;
	this->_maxHealthInitialized = true;
}

NetCommand ServerCommands::SetPlayerLivesLeft::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetPlayerLivesLeft::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETPLAYERLIVESLEFT );
	command.addByte( this->player - players );
	command.addByte( this->livesLeft );
	return command;
}

void ServerCommands::SetPlayerLivesLeft::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetPlayerLivesLeft::SetLivesLeft( int value )
{
	this->livesLeft = value;
	this->_livesLeftInitialized = true;
}

NetCommand ServerCommands::UpdatePlayerPing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: UpdatePlayerPing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_UPDATEPLAYERPING );
	command.setUnreliable( true );
	command.addByte( this->player - players );
	command.addShort( this->ping );
	return command;
}

void ServerCommands::UpdatePlayerPing::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::UpdatePlayerPing::SetPing( unsigned int value )
{
	this->ping = value;
	this->_pingInitialized = true;
}

NetCommand ServerCommands::UpdatePlayerExtraData::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: UpdatePlayerExtraData::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_UPDATEPLAYEREXTRADATA );
	command.setUnreliable( true );
	command.addByte( this->player - players );
	command.addLong( this->pitch );
	command.addByte( this->waterLevel );
	command.addByte( this->buttons );
	command.addLong( this->viewZ );
	command.addLong( this->bob );
	return command;
}

void ServerCommands::UpdatePlayerExtraData::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::UpdatePlayerExtraData::SetPitch( int value )
{
	this->pitch = value;
	this->_pitchInitialized = true;
}

void ServerCommands::UpdatePlayerExtraData::SetWaterLevel( int value )
{
	this->waterLevel = value;
	this->_waterLevelInitialized = true;
}

void ServerCommands::UpdatePlayerExtraData::SetButtons( int value )
{
	this->buttons = value;
	this->_buttonsInitialized = true;
}

void ServerCommands::UpdatePlayerExtraData::SetViewZ( int value )
{
	this->viewZ = value;
	this->_viewZInitialized = true;
}

void ServerCommands::UpdatePlayerExtraData::SetBob( int value )
{
	this->bob = value;
	this->_bobInitialized = true;
}

NetCommand ServerCommands::UpdatePlayerTime::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: UpdatePlayerTime::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_UPDATEPLAYERTIME );
	command.setUnreliable( true );
	command.addByte( this->player - players );
	command.addShort( this->time );
	return command;
}

void ServerCommands::UpdatePlayerTime::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::UpdatePlayerTime::SetTime( unsigned int value )
{
	this->time = value;
	this->_timeInitialized = true;
}

NetCommand ServerCommands::MoveLocalPlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MoveLocalPlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MOVELOCALPLAYER );
	command.setUnreliable( true );
	command.addLong( this->clientTicOnServerEnd );
	command.addLong( this->latestServerGametic );
	command.addLong( this->x );
	command.addLong( this->y );
	command.addLong( this->z );
	command.addLong( this->velx );
	command.addLong( this->vely );
	command.addLong( this->velz );
	return command;
}

void ServerCommands::MoveLocalPlayer::SetClientTicOnServerEnd( unsigned int value )
{
	this->clientTicOnServerEnd = value;
	this->_clientTicOnServerEndInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetLatestServerGametic( int value )
{
	this->latestServerGametic = value;
	this->_latestServerGameticInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetVelx( fixed_t value )
{
	this->velx = value;
	this->_velxInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetVely( fixed_t value )
{
	this->vely = value;
	this->_velyInitialized = true;
}

void ServerCommands::MoveLocalPlayer::SetVelz( fixed_t value )
{
	this->velz = value;
	this->_velzInitialized = true;
}

NetCommand ServerCommands::SetLocalPlayerJumpTics::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLocalPlayerJumpTics::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETLOCALPLAYERJUMPTICS );
	command.setUnreliable( true );
	command.addLong( this->clientTicOnServerEnd );
	command.addByte( this->jumpTics );
	return command;
}

void ServerCommands::SetLocalPlayerJumpTics::SetClientTicOnServerEnd( unsigned int value )
{
	this->clientTicOnServerEnd = value;
	this->_clientTicOnServerEndInitialized = true;
}

void ServerCommands::SetLocalPlayerJumpTics::SetJumpTics( SBYTE value )
{
	this->jumpTics = value;
	this->_jumpTicsInitialized = true;
}

NetCommand ServerCommands::DisconnectPlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DisconnectPlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DISCONNECTPLAYER );
	command.addByte( this->player - players );
	return command;
}

void ServerCommands::DisconnectPlayer::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

NetCommand ServerCommands::SetConsolePlayer::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetConsolePlayer::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETCONSOLEPLAYER );
	command.addByte( this->playerNumber );
	return command;
}

void ServerCommands::SetConsolePlayer::SetPlayerNumber( int value )
{
	this->playerNumber = value;
	this->_playerNumberInitialized = true;
}

NetCommand ServerCommands::ConsolePlayerKicked::BuildNetCommand() const
{
	NetCommand command ( SVC_CONSOLEPLAYERKICKED );
	return command;
}

NetCommand ServerCommands::GivePlayerMedal::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: GivePlayerMedal::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_GIVEPLAYERMEDAL );
	command.addByte( this->player - players );
	command.addByte( this->medal );
	return command;
}

void ServerCommands::GivePlayerMedal::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::GivePlayerMedal::SetMedal( int value )
{
	this->medal = value;
	this->_medalInitialized = true;
}

NetCommand ServerCommands::ResetAllPlayersFragcount::BuildNetCommand() const
{
	NetCommand command ( SVC_RESETALLPLAYERSFRAGCOUNT );
	return command;
}

NetCommand ServerCommands::PlayerIsSpectator::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PlayerIsSpectator::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PLAYERISSPECTATOR );
	command.addByte( this->player - players );
	command.addBit( this->deadSpectator );
	return command;
}

void ServerCommands::PlayerIsSpectator::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::PlayerIsSpectator::SetDeadSpectator( bool value )
{
	this->deadSpectator = value;
	this->_deadSpectatorInitialized = true;
}

NetCommand ServerCommands::PlayerSay::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PlayerSay::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PLAYERSAY );
	command.addByte( this->playerNumber );
	command.addByte( this->mode );
	command.addString( this->message );
	return command;
}

void ServerCommands::PlayerSay::SetPlayerNumber( int value )
{
	this->playerNumber = value;
	this->_playerNumberInitialized = true;
}

void ServerCommands::PlayerSay::SetMode( int value )
{
	this->mode = value;
	this->_modeInitialized = true;
}

void ServerCommands::PlayerSay::SetMessage( const FString & value )
{
	this->message = value;
	this->_messageInitialized = true;
}

NetCommand ServerCommands::PlayerTaunt::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PlayerTaunt::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PLAYERTAUNT );
	command.addByte( this->player - players );
	return command;
}

void ServerCommands::PlayerTaunt::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

NetCommand ServerCommands::PlayerRespawnInvulnerability::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PlayerRespawnInvulnerability::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PLAYERRESPAWNINVULNERABILITY );
	command.addByte( this->player - players );
	return command;
}

void ServerCommands::PlayerRespawnInvulnerability::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

NetCommand ServerCommands::PlayerUseInventory::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PlayerUseInventory::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PLAYERUSEINVENTORY );
	command.addByte( this->player - players );
	command.addShort( this->itemType ? this->itemType->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::PlayerUseInventory::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::PlayerUseInventory::SetItemType( const PClass * value )
{
	this->itemType = value;
	this->_itemTypeInitialized = true;
}

NetCommand ServerCommands::PlayerDropInventory::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PlayerDropInventory::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PLAYERDROPINVENTORY );
	command.addByte( this->player - players );
	command.addShort( this->itemType ? this->itemType->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::PlayerDropInventory::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::PlayerDropInventory::SetItemType( const PClass * value )
{
	this->itemType = value;
	this->_itemTypeInitialized = true;
}

NetCommand ServerCommands::GiveWeaponHolder::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: GiveWeaponHolder::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_GIVEWEAPONHOLDER );
	command.addByte( this->player - players );
	command.addShort( this->pieceMask );
	command.addShort( this->pieceWeapon ? this->pieceWeapon->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::GiveWeaponHolder::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::GiveWeaponHolder::SetPieceMask( int value )
{
	this->pieceMask = value;
	this->_pieceMaskInitialized = true;
}

void ServerCommands::GiveWeaponHolder::SetPieceWeapon( const PClass * value )
{
	this->pieceWeapon = value;
	this->_pieceWeaponInitialized = true;
}

NetCommand ServerCommands::SetHexenArmorSlots::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetHexenArmorSlots::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETHEXENARMORSLOTS );
	command.addByte( this->player - players );
	command.addLong( this->slot0 );
	command.addLong( this->slot1 );
	command.addLong( this->slot2 );
	command.addLong( this->slot3 );
	command.addLong( this->slot4 );
	return command;
}

void ServerCommands::SetHexenArmorSlots::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetHexenArmorSlots::SetSlot0( int value )
{
	this->slot0 = value;
	this->_slot0Initialized = true;
}

void ServerCommands::SetHexenArmorSlots::SetSlot1( int value )
{
	this->slot1 = value;
	this->_slot1Initialized = true;
}

void ServerCommands::SetHexenArmorSlots::SetSlot2( int value )
{
	this->slot2 = value;
	this->_slot2Initialized = true;
}

void ServerCommands::SetHexenArmorSlots::SetSlot3( int value )
{
	this->slot3 = value;
	this->_slot3Initialized = true;
}

void ServerCommands::SetHexenArmorSlots::SetSlot4( int value )
{
	this->slot4 = value;
	this->_slot4Initialized = true;
}

NetCommand ServerCommands::Print::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: Print::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PRINT );
	command.addByte( this->printlevel );
	command.addString( this->message );
	return command;
}

void ServerCommands::Print::SetPrintlevel( int value )
{
	this->printlevel = value;
	this->_printlevelInitialized = true;
}

void ServerCommands::Print::SetMessage( const FString & value )
{
	this->message = value;
	this->_messageInitialized = true;
}

NetCommand ServerCommands::PrintMid::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PrintMid::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PRINTMID );
	command.addString( this->message );
	command.addBit( this->bold );
	return command;
}

void ServerCommands::PrintMid::SetMessage( const FString & value )
{
	this->message = value;
	this->_messageInitialized = true;
}

void ServerCommands::PrintMid::SetBold( bool value )
{
	this->bold = value;
	this->_boldInitialized = true;
}

NetCommand ServerCommands::PrintMOTD::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PrintMOTD::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PRINTMOTD );
	command.addString( this->motd );
	return command;
}

void ServerCommands::PrintMOTD::SetMotd( const FString & value )
{
	this->motd = value;
	this->_motdInitialized = true;
}

NetCommand ServerCommands::PrintHUDMessage::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: PrintHUDMessage::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_PRINTHUDMESSAGE );
	command.addString( this->message );
	command.addFloat( this->x );
	command.addFloat( this->y );
	command.addShort( this->type );
	command.addShort( this->color );
	command.addFloat( this->holdTime );
	command.addLong( this->id );
	if ( ContainsInTime() )
	{
		command.addFloat( this->inTime );
	}
	if ( ContainsOutTime() )
	{
		command.addFloat( this->outTime );
	}
	if ( ContainsHUDSize() )
	{
		command.addShort( this->hudWidth );
		command.addShort( this->hudHeight );
	}
	if ( ContainsClippingRectangle() )
	{
		command.addShort( this->clipRectLeft );
		command.addShort( this->clipRectTop );
		command.addShort( this->clipRectWidth );
		command.addShort( this->clipRectHeight );
	}
	if ( ContainsWrapWidth() )
	{
		command.addShort( this->wrapWidth );
	}
	if ( ContainsFontName() )
	{
		command.addString( this->fontName );
	}
	if ( ContainsAlpha() )
	{
		command.addLong( this->alpha );
	}
	return command;
}

void ServerCommands::PrintHUDMessage::SetMessage( const FString & value )
{
	this->message = value;
	this->_messageInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetX( float value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetY( float value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetType( int value )
{
	this->type = value;
	this->_typeInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetColor( int value )
{
	this->color = value;
	this->_colorInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetHoldTime( float value )
{
	this->holdTime = value;
	this->_holdTimeInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetId( int value )
{
	this->id = value;
	this->_idInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetInTime( float value )
{
	this->inTime = value;
	this->_inTimeInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetOutTime( float value )
{
	this->outTime = value;
	this->_outTimeInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetHudWidth( int value )
{
	this->hudWidth = value;
	this->_hudWidthInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetHudHeight( int value )
{
	this->hudHeight = value;
	this->_hudHeightInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetClipRectLeft( int value )
{
	this->clipRectLeft = value;
	this->_clipRectLeftInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetClipRectTop( int value )
{
	this->clipRectTop = value;
	this->_clipRectTopInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetClipRectWidth( int value )
{
	this->clipRectWidth = value;
	this->_clipRectWidthInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetClipRectHeight( int value )
{
	this->clipRectHeight = value;
	this->_clipRectHeightInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetWrapWidth( int value )
{
	this->wrapWidth = value;
	this->_wrapWidthInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetFontName( const FString & value )
{
	this->fontName = value;
	this->_fontNameInitialized = true;
}

void ServerCommands::PrintHUDMessage::SetAlpha( fixed_t value )
{
	this->alpha = value;
	this->_alphaInitialized = true;
}

bool ServerCommands::PrintHUDMessage::ContainsOutTime() const
{
	return !!( (type & HUDMESSAGETYPE_MASK) != HUDMESSAGETYPE_NORMAL );
}

bool ServerCommands::PrintHUDMessage::ContainsInTime() const
{
	return !!( (type & HUDMESSAGETYPE_MASK) == HUDMESSAGETYPE_TYPEONFADEOUT || (type & HUDMESSAGETYPE_MASK) == HUDMESSAGETYPE_FADEINOUT );
}

bool ServerCommands::PrintHUDMessage::ContainsAlpha() const
{
	return !!( (type & HUDMESSAGE_ACS) && (type & HUDMESSAGE_NET_ALPHA) );
}

bool ServerCommands::PrintHUDMessage::ContainsClippingRectangle() const
{
	return !!( (type & HUDMESSAGE_ACS) && (type & HUDMESSAGE_SEND_CLIPRECT) );
}

bool ServerCommands::PrintHUDMessage::ContainsWrapWidth() const
{
	return !!( (type & HUDMESSAGE_ACS) && (type & HUDMESSAGE_SEND_WRAPWIDTH) );
}

bool ServerCommands::PrintHUDMessage::ContainsFontName() const
{
	return !!( type & HUDMESSAGE_SEND_FONT );
}

bool ServerCommands::PrintHUDMessage::ContainsHUDSize() const
{
	return !!( type & HUDMESSAGE_SEND_HUDSIZE );
}

NetCommand ServerCommands::SpawnThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNTHING );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->type ? this->type->getActorNetworkIndex() : -1 );
	command.addShort( this->id );
	return command;
}

void ServerCommands::SpawnThing::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnThing::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnThing::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnThing::SetType( const PClass * value )
{
	this->type = value;
	this->_typeInitialized = true;
}

void ServerCommands::SpawnThing::SetId( int value )
{
	this->id = value;
	this->_idInitialized = true;
}

NetCommand ServerCommands::SpawnThingNoNetID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnThingNoNetID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNTHINGNONETID );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->type ? this->type->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::SpawnThingNoNetID::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnThingNoNetID::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnThingNoNetID::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnThingNoNetID::SetType( const PClass * value )
{
	this->type = value;
	this->_typeInitialized = true;
}

NetCommand ServerCommands::SpawnThingExact::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnThingExact::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNTHINGEXACT );
	command.addLong( this->x );
	command.addLong( this->y );
	command.addLong( this->z );
	command.addShort( this->type ? this->type->getActorNetworkIndex() : -1 );
	command.addShort( this->id );
	return command;
}

void ServerCommands::SpawnThingExact::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnThingExact::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnThingExact::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnThingExact::SetType( const PClass * value )
{
	this->type = value;
	this->_typeInitialized = true;
}

void ServerCommands::SpawnThingExact::SetId( int value )
{
	this->id = value;
	this->_idInitialized = true;
}

NetCommand ServerCommands::SpawnThingExactNoNetID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnThingExactNoNetID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNTHINGEXACTNONETID );
	command.addLong( this->x );
	command.addLong( this->y );
	command.addLong( this->z );
	command.addShort( this->type ? this->type->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::SpawnThingExactNoNetID::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnThingExactNoNetID::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnThingExactNoNetID::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnThingExactNoNetID::SetType( const PClass * value )
{
	this->type = value;
	this->_typeInitialized = true;
}

NetCommand ServerCommands::LevelSpawnThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: LevelSpawnThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_LEVELSPAWNTHING );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->type ? this->type->getActorNetworkIndex() : -1 );
	command.addShort( this->id );
	return command;
}

void ServerCommands::LevelSpawnThing::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::LevelSpawnThing::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::LevelSpawnThing::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::LevelSpawnThing::SetType( const PClass * value )
{
	this->type = value;
	this->_typeInitialized = true;
}

void ServerCommands::LevelSpawnThing::SetId( int value )
{
	this->id = value;
	this->_idInitialized = true;
}

NetCommand ServerCommands::LevelSpawnThingNoNetID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: LevelSpawnThingNoNetID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_LEVELSPAWNTHINGNONETID );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->type ? this->type->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::LevelSpawnThingNoNetID::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::LevelSpawnThingNoNetID::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::LevelSpawnThingNoNetID::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::LevelSpawnThingNoNetID::SetType( const PClass * value )
{
	this->type = value;
	this->_typeInitialized = true;
}

NetCommand ServerCommands::MoveThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MoveThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MOVETHING );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->bits );
	if ( ContainsNewX() )
	{
		command.addShort( this->newX >> FRACBITS );
	}
	if ( ContainsNewY() )
	{
		command.addShort( this->newY >> FRACBITS );
	}
	if ( ContainsNewZ() )
	{
		command.addShort( this->newZ >> FRACBITS );
	}
	if ( ContainsLastX() )
	{
		command.addShort( this->lastX >> FRACBITS );
	}
	if ( ContainsLastY() )
	{
		command.addShort( this->lastY >> FRACBITS );
	}
	if ( ContainsLastZ() )
	{
		command.addShort( this->lastZ >> FRACBITS );
	}
	if ( ContainsAngle() )
	{
		command.addLong( this->angle );
	}
	if ( ContainsVelX() )
	{
		command.addShort( this->velX >> FRACBITS );
	}
	if ( ContainsVelY() )
	{
		command.addShort( this->velY >> FRACBITS );
	}
	if ( ContainsVelZ() )
	{
		command.addShort( this->velZ >> FRACBITS );
	}
	if ( ContainsPitch() )
	{
		command.addLong( this->pitch );
	}
	if ( ContainsMovedir() )
	{
		command.addByte( this->movedir );
	}
	return command;
}

void ServerCommands::MoveThing::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::MoveThing::SetBits( int value )
{
	this->bits = value;
	this->_bitsInitialized = true;
}

void ServerCommands::MoveThing::SetNewX( fixed_t value )
{
	this->newX = value;
	this->_newXInitialized = true;
}

void ServerCommands::MoveThing::SetNewY( fixed_t value )
{
	this->newY = value;
	this->_newYInitialized = true;
}

void ServerCommands::MoveThing::SetNewZ( fixed_t value )
{
	this->newZ = value;
	this->_newZInitialized = true;
}

void ServerCommands::MoveThing::SetLastX( fixed_t value )
{
	this->lastX = value;
	this->_lastXInitialized = true;
}

void ServerCommands::MoveThing::SetLastY( fixed_t value )
{
	this->lastY = value;
	this->_lastYInitialized = true;
}

void ServerCommands::MoveThing::SetLastZ( fixed_t value )
{
	this->lastZ = value;
	this->_lastZInitialized = true;
}

void ServerCommands::MoveThing::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

void ServerCommands::MoveThing::SetVelX( fixed_t value )
{
	this->velX = value;
	this->_velXInitialized = true;
}

void ServerCommands::MoveThing::SetVelY( fixed_t value )
{
	this->velY = value;
	this->_velYInitialized = true;
}

void ServerCommands::MoveThing::SetVelZ( fixed_t value )
{
	this->velZ = value;
	this->_velZInitialized = true;
}

void ServerCommands::MoveThing::SetPitch( int value )
{
	this->pitch = value;
	this->_pitchInitialized = true;
}

void ServerCommands::MoveThing::SetMovedir( int value )
{
	this->movedir = value;
	this->_movedirInitialized = true;
}

bool ServerCommands::MoveThing::ContainsAngle() const
{
	return !!( bits & CM_ANGLE );
}

bool ServerCommands::MoveThing::ContainsLastX() const
{
	return !!( bits & CM_LAST_X );
}

bool ServerCommands::MoveThing::ContainsLastY() const
{
	return !!( bits & CM_LAST_Y );
}

bool ServerCommands::MoveThing::ContainsLastZ() const
{
	return !!( bits & CM_LAST_Z );
}

bool ServerCommands::MoveThing::ContainsMovedir() const
{
	return !!( bits & CM_MOVEDIR );
}

bool ServerCommands::MoveThing::ContainsPitch() const
{
	return !!( bits & CM_PITCH );
}

bool ServerCommands::MoveThing::ContainsVelX() const
{
	return !!( bits & CM_VELX );
}

bool ServerCommands::MoveThing::ContainsVelY() const
{
	return !!( bits & CM_VELY );
}

bool ServerCommands::MoveThing::ContainsVelZ() const
{
	return !!( bits & CM_VELZ );
}

bool ServerCommands::MoveThing::ContainsNewX() const
{
	return !!( bits & CM_X );
}

bool ServerCommands::MoveThing::ContainsNewY() const
{
	return !!( bits & CM_Y );
}

bool ServerCommands::MoveThing::ContainsNewZ() const
{
	return !!( bits & CM_Z );
}

NetCommand ServerCommands::MoveThingExact::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MoveThingExact::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MOVETHINGEXACT );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->bits );
	if ( ContainsNewX() )
	{
		command.addLong( this->newX );
	}
	if ( ContainsNewY() )
	{
		command.addLong( this->newY );
	}
	if ( ContainsNewZ() )
	{
		command.addLong( this->newZ );
	}
	if ( ContainsLastX() )
	{
		command.addLong( this->lastX );
	}
	if ( ContainsLastY() )
	{
		command.addLong( this->lastY );
	}
	if ( ContainsLastZ() )
	{
		command.addLong( this->lastZ );
	}
	if ( ContainsAngle() )
	{
		command.addLong( this->angle );
	}
	if ( ContainsVelX() )
	{
		command.addLong( this->velX );
	}
	if ( ContainsVelY() )
	{
		command.addLong( this->velY );
	}
	if ( ContainsVelZ() )
	{
		command.addLong( this->velZ );
	}
	if ( ContainsPitch() )
	{
		command.addLong( this->pitch );
	}
	if ( ContainsMovedir() )
	{
		command.addByte( this->movedir );
	}
	return command;
}

void ServerCommands::MoveThingExact::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::MoveThingExact::SetBits( int value )
{
	this->bits = value;
	this->_bitsInitialized = true;
}

void ServerCommands::MoveThingExact::SetNewX( fixed_t value )
{
	this->newX = value;
	this->_newXInitialized = true;
}

void ServerCommands::MoveThingExact::SetNewY( fixed_t value )
{
	this->newY = value;
	this->_newYInitialized = true;
}

void ServerCommands::MoveThingExact::SetNewZ( fixed_t value )
{
	this->newZ = value;
	this->_newZInitialized = true;
}

void ServerCommands::MoveThingExact::SetLastX( fixed_t value )
{
	this->lastX = value;
	this->_lastXInitialized = true;
}

void ServerCommands::MoveThingExact::SetLastY( fixed_t value )
{
	this->lastY = value;
	this->_lastYInitialized = true;
}

void ServerCommands::MoveThingExact::SetLastZ( fixed_t value )
{
	this->lastZ = value;
	this->_lastZInitialized = true;
}

void ServerCommands::MoveThingExact::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

void ServerCommands::MoveThingExact::SetVelX( fixed_t value )
{
	this->velX = value;
	this->_velXInitialized = true;
}

void ServerCommands::MoveThingExact::SetVelY( fixed_t value )
{
	this->velY = value;
	this->_velYInitialized = true;
}

void ServerCommands::MoveThingExact::SetVelZ( fixed_t value )
{
	this->velZ = value;
	this->_velZInitialized = true;
}

void ServerCommands::MoveThingExact::SetPitch( int value )
{
	this->pitch = value;
	this->_pitchInitialized = true;
}

void ServerCommands::MoveThingExact::SetMovedir( int value )
{
	this->movedir = value;
	this->_movedirInitialized = true;
}

bool ServerCommands::MoveThingExact::ContainsAngle() const
{
	return !!( bits & CM_ANGLE );
}

bool ServerCommands::MoveThingExact::ContainsLastX() const
{
	return !!( bits & CM_LAST_X );
}

bool ServerCommands::MoveThingExact::ContainsLastY() const
{
	return !!( bits & CM_LAST_Y );
}

bool ServerCommands::MoveThingExact::ContainsLastZ() const
{
	return !!( bits & CM_LAST_Z );
}

bool ServerCommands::MoveThingExact::ContainsMovedir() const
{
	return !!( bits & CM_MOVEDIR );
}

bool ServerCommands::MoveThingExact::ContainsPitch() const
{
	return !!( bits & CM_PITCH );
}

bool ServerCommands::MoveThingExact::ContainsVelX() const
{
	return !!( bits & CM_VELX );
}

bool ServerCommands::MoveThingExact::ContainsVelY() const
{
	return !!( bits & CM_VELY );
}

bool ServerCommands::MoveThingExact::ContainsVelZ() const
{
	return !!( bits & CM_VELZ );
}

bool ServerCommands::MoveThingExact::ContainsNewX() const
{
	return !!( bits & CM_X );
}

bool ServerCommands::MoveThingExact::ContainsNewY() const
{
	return !!( bits & CM_Y );
}

bool ServerCommands::MoveThingExact::ContainsNewZ() const
{
	return !!( bits & CM_Z );
}

NetCommand ServerCommands::KillThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: KillThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_KILLTHING );
	command.addShort( this->victim ? this->victim->NetID : -1 );
	command.addVariable( this->health );
	command.addString( this->damageType );
	command.addShort( this->source ? this->source->NetID : -1 );
	command.addShort( this->inflictor ? this->inflictor->NetID : -1 );
	return command;
}

void ServerCommands::KillThing::SetVictim( AActor * value )
{
	this->victim = value;
	this->_victimInitialized = true;
}

void ServerCommands::KillThing::SetHealth( int value )
{
	this->health = value;
	this->_healthInitialized = true;
}

void ServerCommands::KillThing::SetDamageType( const FString & value )
{
	this->damageType = value;
	this->_damageTypeInitialized = true;
}

void ServerCommands::KillThing::SetSource( AActor * value )
{
	this->source = value;
	this->_sourceInitialized = true;
}

void ServerCommands::KillThing::SetInflictor( AActor * value )
{
	this->inflictor = value;
	this->_inflictorInitialized = true;
}

NetCommand ServerCommands::SetThingState::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingState::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGSTATE );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->state );
	return command;
}

void ServerCommands::SetThingState::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingState::SetState( int value )
{
	this->state = value;
	this->_stateInitialized = true;
}

NetCommand ServerCommands::SetThingTarget::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingTarget::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGTARGET );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->target ? this->target->NetID : -1 );
	return command;
}

void ServerCommands::SetThingTarget::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingTarget::SetTarget( AActor * value )
{
	this->target = value;
	this->_targetInitialized = true;
}

NetCommand ServerCommands::DestroyThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DestroyThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DESTROYTHING );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	return command;
}

void ServerCommands::DestroyThing::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

NetCommand ServerCommands::SetThingAngle::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingAngle::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGANGLE );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->angle >> FRACBITS );
	return command;
}

void ServerCommands::SetThingAngle::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingAngle::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

NetCommand ServerCommands::SetThingAngleExact::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingAngleExact::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGANGLEEXACT );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addLong( this->angle );
	return command;
}

void ServerCommands::SetThingAngleExact::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingAngleExact::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

NetCommand ServerCommands::SetThingWaterLevel::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingWaterLevel::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGWATERLEVEL );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->waterlevel );
	return command;
}

void ServerCommands::SetThingWaterLevel::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingWaterLevel::SetWaterlevel( int value )
{
	this->waterlevel = value;
	this->_waterlevelInitialized = true;
}

NetCommand ServerCommands::SetThingFlags::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingFlags::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGFLAGS );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->flagset );
	command.addLong( this->flags );
	return command;
}

void ServerCommands::SetThingFlags::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingFlags::SetFlagset( int value )
{
	this->flagset = value;
	this->_flagsetInitialized = true;
}

void ServerCommands::SetThingFlags::SetFlags( unsigned int value )
{
	this->flags = value;
	this->_flagsInitialized = true;
}

NetCommand ServerCommands::SetThingArguments::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingArguments::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGARGUMENTS );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addLong( this->arg0 );
	command.addLong( this->arg1 );
	command.addLong( this->arg2 );
	command.addLong( this->arg3 );
	command.addLong( this->arg4 );
	return command;
}

void ServerCommands::SetThingArguments::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingArguments::SetArg0( int value )
{
	this->arg0 = value;
	this->_arg0Initialized = true;
}

void ServerCommands::SetThingArguments::SetArg1( int value )
{
	this->arg1 = value;
	this->_arg1Initialized = true;
}

void ServerCommands::SetThingArguments::SetArg2( int value )
{
	this->arg2 = value;
	this->_arg2Initialized = true;
}

void ServerCommands::SetThingArguments::SetArg3( int value )
{
	this->arg3 = value;
	this->_arg3Initialized = true;
}

void ServerCommands::SetThingArguments::SetArg4( int value )
{
	this->arg4 = value;
	this->_arg4Initialized = true;
}

NetCommand ServerCommands::SetThingTranslation::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingTranslation::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGTRANSLATION );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addLong( this->translation );
	return command;
}

void ServerCommands::SetThingTranslation::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingTranslation::SetTranslation( int value )
{
	this->translation = value;
	this->_translationInitialized = true;
}

NetCommand ServerCommands::SetThingProperty::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingProperty::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGPROPERTY );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->property );
	command.addLong( this->value );
	return command;
}

void ServerCommands::SetThingProperty::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingProperty::SetProperty( int value )
{
	this->property = value;
	this->_propertyInitialized = true;
}

void ServerCommands::SetThingProperty::SetValue( int value )
{
	this->value = value;
	this->_valueInitialized = true;
}

NetCommand ServerCommands::SetThingStringProperty::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingStringProperty::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETTHINGSTRINGPROPERTY );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->property );
	command.addString( this->value );
	return command;
}

void ServerCommands::SetThingStringProperty::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingStringProperty::SetProperty( int value )
{
	this->property = value;
	this->_propertyInitialized = true;
}

void ServerCommands::SetThingStringProperty::SetValue( const FString & value )
{
	this->value = value;
	this->_valueInitialized = true;
}

NetCommand ServerCommands::SetThingSound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingSound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGSOUND );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->soundType );
	command.addString( this->sound );
	return command;
}

void ServerCommands::SetThingSound::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingSound::SetSoundType( int value )
{
	this->soundType = value;
	this->_soundTypeInitialized = true;
}

void ServerCommands::SetThingSound::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

NetCommand ServerCommands::SetThingSpawnPoint::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingSpawnPoint::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGSPAWNPOINT );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addLong( this->spawnPointX );
	command.addLong( this->spawnPointY );
	command.addLong( this->spawnPointZ );
	return command;
}

void ServerCommands::SetThingSpawnPoint::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingSpawnPoint::SetSpawnPointX( fixed_t value )
{
	this->spawnPointX = value;
	this->_spawnPointXInitialized = true;
}

void ServerCommands::SetThingSpawnPoint::SetSpawnPointY( fixed_t value )
{
	this->spawnPointY = value;
	this->_spawnPointYInitialized = true;
}

void ServerCommands::SetThingSpawnPoint::SetSpawnPointZ( fixed_t value )
{
	this->spawnPointZ = value;
	this->_spawnPointZInitialized = true;
}

NetCommand ServerCommands::SetThingSpecial1::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingSpecial1::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGSPECIAL1 );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->special1 );
	return command;
}

void ServerCommands::SetThingSpecial1::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingSpecial1::SetSpecial1( int value )
{
	this->special1 = value;
	this->_special1Initialized = true;
}

NetCommand ServerCommands::SetThingSpecial2::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingSpecial2::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGSPECIAL2 );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->special2 );
	return command;
}

void ServerCommands::SetThingSpecial2::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingSpecial2::SetSpecial2( int value )
{
	this->special2 = value;
	this->_special2Initialized = true;
}

NetCommand ServerCommands::SetThingTics::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingTics::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGTICS );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->tics );
	return command;
}

void ServerCommands::SetThingTics::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingTics::SetTics( int value )
{
	this->tics = value;
	this->_ticsInitialized = true;
}

NetCommand ServerCommands::SetThingTID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingTID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGTID );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addLong( this->tid );
	return command;
}

void ServerCommands::SetThingTID::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingTID::SetTid( int value )
{
	this->tid = value;
	this->_tidInitialized = true;
}

NetCommand ServerCommands::SetThingReactionTime::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingReactionTime::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETTHINGREACTIONTIME );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->reactiontime );
	return command;
}

void ServerCommands::SetThingReactionTime::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingReactionTime::SetReactiontime( int value )
{
	this->reactiontime = value;
	this->_reactiontimeInitialized = true;
}

NetCommand ServerCommands::SetThingGravity::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingGravity::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGGRAVITY );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addLong( this->gravity );
	return command;
}

void ServerCommands::SetThingGravity::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingGravity::SetGravity( int value )
{
	this->gravity = value;
	this->_gravityInitialized = true;
}

NetCommand ServerCommands::SetThingFrame::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingFrame::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGFRAME );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->stateOwner ? this->stateOwner->getActorNetworkIndex() : -1 );
	command.addShort( this->offset );
	return command;
}

void ServerCommands::SetThingFrame::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingFrame::SetStateOwner( const PClass * value )
{
	this->stateOwner = value;
	this->_stateOwnerInitialized = true;
}

void ServerCommands::SetThingFrame::SetOffset( int value )
{
	this->offset = value;
	this->_offsetInitialized = true;
}

NetCommand ServerCommands::SetThingFrameNF::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingFrameNF::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETTHINGFRAMENF );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->stateOwner ? this->stateOwner->getActorNetworkIndex() : -1 );
	command.addShort( this->offset );
	return command;
}

void ServerCommands::SetThingFrameNF::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingFrameNF::SetStateOwner( const PClass * value )
{
	this->stateOwner = value;
	this->_stateOwnerInitialized = true;
}

void ServerCommands::SetThingFrameNF::SetOffset( int value )
{
	this->offset = value;
	this->_offsetInitialized = true;
}

NetCommand ServerCommands::SetWeaponAmmoGive::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetWeaponAmmoGive::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETWEAPONAMMOGIVE );
	command.addShort( this->weapon ? this->weapon->NetID : -1 );
	command.addShort( this->ammoGive1 );
	command.addShort( this->ammoGive2 );
	return command;
}

void ServerCommands::SetWeaponAmmoGive::SetWeapon( AWeapon * value )
{
	this->weapon = value;
	this->_weaponInitialized = true;
}

void ServerCommands::SetWeaponAmmoGive::SetAmmoGive1( int value )
{
	this->ammoGive1 = value;
	this->_ammoGive1Initialized = true;
}

void ServerCommands::SetWeaponAmmoGive::SetAmmoGive2( int value )
{
	this->ammoGive2 = value;
	this->_ammoGive2Initialized = true;
}

NetCommand ServerCommands::SetThingScale::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetThingScale::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETTHINGSCALE );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->scaleflags );
	if ( ContainsScaleX() )
	{
		command.addLong( this->scaleX );
	}
	if ( ContainsScaleY() )
	{
		command.addLong( this->scaleY );
	}
	return command;
}

void ServerCommands::SetThingScale::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SetThingScale::SetScaleflags( int value )
{
	this->scaleflags = value;
	this->_scaleflagsInitialized = true;
}

void ServerCommands::SetThingScale::SetScaleX( int value )
{
	this->scaleX = value;
	this->_scaleXInitialized = true;
}

void ServerCommands::SetThingScale::SetScaleY( int value )
{
	this->scaleY = value;
	this->_scaleYInitialized = true;
}

bool ServerCommands::SetThingScale::ContainsScaleX() const
{
	return !!( scaleflags & ACTORSCALE_X );
}

bool ServerCommands::SetThingScale::ContainsScaleY() const
{
	return !!( scaleflags & ACTORSCALE_Y );
}

NetCommand ServerCommands::ThingIsCorpse::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ThingIsCorpse::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_THINGISCORPSE );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addBit( this->isMonster );
	return command;
}

void ServerCommands::ThingIsCorpse::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::ThingIsCorpse::SetIsMonster( bool value )
{
	this->isMonster = value;
	this->_isMonsterInitialized = true;
}

NetCommand ServerCommands::HideThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: HideThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_HIDETHING );
	command.addShort( this->item ? this->item->NetID : -1 );
	return command;
}

void ServerCommands::HideThing::SetItem( AInventory * value )
{
	this->item = value;
	this->_itemInitialized = true;
}

NetCommand ServerCommands::TeleportThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: TeleportThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_TELEPORTTHING );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->momx >> FRACBITS );
	command.addShort( this->momy >> FRACBITS );
	command.addShort( this->momz >> FRACBITS );
	command.addShort( this->reactiontime );
	command.addLong( this->angle );
	command.addBit( this->sourcefog );
	command.addBit( this->destfog );
	command.addBit( this->teleportzoom );
	return command;
}

void ServerCommands::TeleportThing::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::TeleportThing::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::TeleportThing::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::TeleportThing::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::TeleportThing::SetMomx( fixed_t value )
{
	this->momx = value;
	this->_momxInitialized = true;
}

void ServerCommands::TeleportThing::SetMomy( fixed_t value )
{
	this->momy = value;
	this->_momyInitialized = true;
}

void ServerCommands::TeleportThing::SetMomz( fixed_t value )
{
	this->momz = value;
	this->_momzInitialized = true;
}

void ServerCommands::TeleportThing::SetReactiontime( int value )
{
	this->reactiontime = value;
	this->_reactiontimeInitialized = true;
}

void ServerCommands::TeleportThing::SetAngle( angle_t value )
{
	this->angle = value;
	this->_angleInitialized = true;
}

void ServerCommands::TeleportThing::SetSourcefog( bool value )
{
	this->sourcefog = value;
	this->_sourcefogInitialized = true;
}

void ServerCommands::TeleportThing::SetDestfog( bool value )
{
	this->destfog = value;
	this->_destfogInitialized = true;
}

void ServerCommands::TeleportThing::SetTeleportzoom( bool value )
{
	this->teleportzoom = value;
	this->_teleportzoomInitialized = true;
}

NetCommand ServerCommands::ThingActivate::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ThingActivate::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_THINGACTIVATE );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->activator ? this->activator->NetID : -1 );
	return command;
}

void ServerCommands::ThingActivate::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::ThingActivate::SetActivator( AActor * value )
{
	this->activator = value;
	this->_activatorInitialized = true;
}

NetCommand ServerCommands::ThingDeactivate::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ThingDeactivate::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_THINGDEACTIVATE );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->activator ? this->activator->NetID : -1 );
	return command;
}

void ServerCommands::ThingDeactivate::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::ThingDeactivate::SetActivator( AActor * value )
{
	this->activator = value;
	this->_activatorInitialized = true;
}

NetCommand ServerCommands::RespawnDoomThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: RespawnDoomThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_RESPAWNDOOMTHING );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addBit( this->fog );
	return command;
}

void ServerCommands::RespawnDoomThing::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::RespawnDoomThing::SetFog( bool value )
{
	this->fog = value;
	this->_fogInitialized = true;
}

NetCommand ServerCommands::RespawnRavenThing::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: RespawnRavenThing::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_RESPAWNRAVENTHING );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	return command;
}

void ServerCommands::RespawnRavenThing::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

NetCommand ServerCommands::SpawnBlood::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnBlood::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNBLOOD );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->dir >> FRACBITS );
	command.addByte( this->damage );
	command.addShort( this->originator ? this->originator->NetID : -1 );
	return command;
}

void ServerCommands::SpawnBlood::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnBlood::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnBlood::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnBlood::SetDir( angle_t value )
{
	this->dir = value;
	this->_dirInitialized = true;
}

void ServerCommands::SpawnBlood::SetDamage( int value )
{
	this->damage = value;
	this->_damageInitialized = true;
}

void ServerCommands::SpawnBlood::SetOriginator( AActor * value )
{
	this->originator = value;
	this->_originatorInitialized = true;
}

NetCommand ServerCommands::SpawnBloodSplatter::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnBloodSplatter::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNBLOODSPLATTER );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->originator ? this->originator->NetID : -1 );
	return command;
}

void ServerCommands::SpawnBloodSplatter::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnBloodSplatter::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnBloodSplatter::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnBloodSplatter::SetOriginator( AActor * value )
{
	this->originator = value;
	this->_originatorInitialized = true;
}

NetCommand ServerCommands::SpawnBloodSplatter2::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnBloodSplatter2::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNBLOODSPLATTER2 );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->originator ? this->originator->NetID : -1 );
	return command;
}

void ServerCommands::SpawnBloodSplatter2::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnBloodSplatter2::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnBloodSplatter2::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnBloodSplatter2::SetOriginator( AActor * value )
{
	this->originator = value;
	this->_originatorInitialized = true;
}

NetCommand ServerCommands::SpawnPuff::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnPuff::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNPUFF );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->pufftype ? this->pufftype->getActorNetworkIndex() : -1 );
	command.addShort( this->id );
	return command;
}

void ServerCommands::SpawnPuff::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnPuff::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnPuff::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnPuff::SetPufftype( const PClass * value )
{
	this->pufftype = value;
	this->_pufftypeInitialized = true;
}

void ServerCommands::SpawnPuff::SetId( int value )
{
	this->id = value;
	this->_idInitialized = true;
}

NetCommand ServerCommands::SpawnPuffNoNetID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnPuffNoNetID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNPUFFNONETID );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addShort( this->pufftype ? this->pufftype->getActorNetworkIndex() : -1 );
	command.addByte( this->stateid );
	command.addBit( this->receiveTranslation );
	if ( ContainsTranslation() )
	{
		command.addLong( this->translation );
	}
	return command;
}

void ServerCommands::SpawnPuffNoNetID::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnPuffNoNetID::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnPuffNoNetID::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnPuffNoNetID::SetPufftype( const PClass * value )
{
	this->pufftype = value;
	this->_pufftypeInitialized = true;
}

void ServerCommands::SpawnPuffNoNetID::SetStateid( int value )
{
	this->stateid = value;
	this->_stateidInitialized = true;
}

void ServerCommands::SpawnPuffNoNetID::SetReceiveTranslation( bool value )
{
	this->receiveTranslation = value;
	this->_receiveTranslationInitialized = true;
}

void ServerCommands::SpawnPuffNoNetID::SetTranslation( int value )
{
	this->translation = value;
	this->_translationInitialized = true;
}

bool ServerCommands::SpawnPuffNoNetID::ContainsTranslation() const
{
	return !!( receiveTranslation );
}

NetCommand ServerCommands::SetSectorFloorPlane::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorFloorPlane::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORFLOORPLANE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->height >> FRACBITS );
	return command;
}

void ServerCommands::SetSectorFloorPlane::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorFloorPlane::SetHeight( fixed_t value )
{
	this->height = value;
	this->_heightInitialized = true;
}

NetCommand ServerCommands::SetSectorCeilingPlane::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorCeilingPlane::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORCEILINGPLANE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->height >> FRACBITS );
	return command;
}

void ServerCommands::SetSectorCeilingPlane::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorCeilingPlane::SetHeight( fixed_t value )
{
	this->height = value;
	this->_heightInitialized = true;
}

NetCommand ServerCommands::SetSectorFloorPlaneSlope::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorFloorPlaneSlope::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORFLOORPLANESLOPE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->a >> FRACBITS );
	command.addShort( this->b >> FRACBITS );
	command.addShort( this->c >> FRACBITS );
	return command;
}

void ServerCommands::SetSectorFloorPlaneSlope::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorFloorPlaneSlope::SetA( fixed_t value )
{
	this->a = value;
	this->_aInitialized = true;
}

void ServerCommands::SetSectorFloorPlaneSlope::SetB( fixed_t value )
{
	this->b = value;
	this->_bInitialized = true;
}

void ServerCommands::SetSectorFloorPlaneSlope::SetC( fixed_t value )
{
	this->c = value;
	this->_cInitialized = true;
}

NetCommand ServerCommands::SetSectorCeilingPlaneSlope::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorCeilingPlaneSlope::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORCEILINGPLANESLOPE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->a >> FRACBITS );
	command.addShort( this->b >> FRACBITS );
	command.addShort( this->c >> FRACBITS );
	return command;
}

void ServerCommands::SetSectorCeilingPlaneSlope::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorCeilingPlaneSlope::SetA( fixed_t value )
{
	this->a = value;
	this->_aInitialized = true;
}

void ServerCommands::SetSectorCeilingPlaneSlope::SetB( fixed_t value )
{
	this->b = value;
	this->_bInitialized = true;
}

void ServerCommands::SetSectorCeilingPlaneSlope::SetC( fixed_t value )
{
	this->c = value;
	this->_cInitialized = true;
}

NetCommand ServerCommands::SetSectorLightLevel::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorLightLevel::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORLIGHTLEVEL );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->lightLevel );
	return command;
}

void ServerCommands::SetSectorLightLevel::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorLightLevel::SetLightLevel( int value )
{
	this->lightLevel = value;
	this->_lightLevelInitialized = true;
}

NetCommand ServerCommands::SetSectorColor::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorColor::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORCOLOR );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addByte( this->red );
	command.addByte( this->green );
	command.addByte( this->blue );
	command.addByte( this->desaturate );
	return command;
}

void ServerCommands::SetSectorColor::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorColor::SetRed( int value )
{
	this->red = value;
	this->_redInitialized = true;
}

void ServerCommands::SetSectorColor::SetGreen( int value )
{
	this->green = value;
	this->_greenInitialized = true;
}

void ServerCommands::SetSectorColor::SetBlue( int value )
{
	this->blue = value;
	this->_blueInitialized = true;
}

void ServerCommands::SetSectorColor::SetDesaturate( int value )
{
	this->desaturate = value;
	this->_desaturateInitialized = true;
}

NetCommand ServerCommands::SetSectorColorByTag::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorColorByTag::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORCOLORBYTAG );
	command.addShort( this->tag );
	command.addByte( this->red );
	command.addByte( this->green );
	command.addByte( this->blue );
	command.addByte( this->desaturate );
	return command;
}

void ServerCommands::SetSectorColorByTag::SetTag( int value )
{
	this->tag = value;
	this->_tagInitialized = true;
}

void ServerCommands::SetSectorColorByTag::SetRed( int value )
{
	this->red = value;
	this->_redInitialized = true;
}

void ServerCommands::SetSectorColorByTag::SetGreen( int value )
{
	this->green = value;
	this->_greenInitialized = true;
}

void ServerCommands::SetSectorColorByTag::SetBlue( int value )
{
	this->blue = value;
	this->_blueInitialized = true;
}

void ServerCommands::SetSectorColorByTag::SetDesaturate( int value )
{
	this->desaturate = value;
	this->_desaturateInitialized = true;
}

NetCommand ServerCommands::SetSectorFade::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorFade::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORFADE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addByte( this->red );
	command.addByte( this->green );
	command.addByte( this->blue );
	return command;
}

void ServerCommands::SetSectorFade::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorFade::SetRed( int value )
{
	this->red = value;
	this->_redInitialized = true;
}

void ServerCommands::SetSectorFade::SetGreen( int value )
{
	this->green = value;
	this->_greenInitialized = true;
}

void ServerCommands::SetSectorFade::SetBlue( int value )
{
	this->blue = value;
	this->_blueInitialized = true;
}

NetCommand ServerCommands::SetSectorFadeByTag::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorFadeByTag::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORFADEBYTAG );
	command.addShort( this->tag );
	command.addByte( this->red );
	command.addByte( this->green );
	command.addByte( this->blue );
	return command;
}

void ServerCommands::SetSectorFadeByTag::SetTag( int value )
{
	this->tag = value;
	this->_tagInitialized = true;
}

void ServerCommands::SetSectorFadeByTag::SetRed( int value )
{
	this->red = value;
	this->_redInitialized = true;
}

void ServerCommands::SetSectorFadeByTag::SetGreen( int value )
{
	this->green = value;
	this->_greenInitialized = true;
}

void ServerCommands::SetSectorFadeByTag::SetBlue( int value )
{
	this->blue = value;
	this->_blueInitialized = true;
}

NetCommand ServerCommands::SetSectorFlat::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorFlat::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORFLAT );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addString( this->ceilingFlatName );
	command.addString( this->floorFlatName );
	return command;
}

void ServerCommands::SetSectorFlat::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorFlat::SetCeilingFlatName( const FString & value )
{
	this->ceilingFlatName = value;
	this->_ceilingFlatNameInitialized = true;
}

void ServerCommands::SetSectorFlat::SetFloorFlatName( const FString & value )
{
	this->floorFlatName = value;
	this->_floorFlatNameInitialized = true;
}

NetCommand ServerCommands::SetSectorPanning::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorPanning::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORPANNING );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addLong( this->ceilingXOffset );
	command.addLong( this->ceilingYOffset );
	command.addLong( this->floorXOffset );
	command.addLong( this->floorYOffset );
	return command;
}

void ServerCommands::SetSectorPanning::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorPanning::SetCeilingXOffset( fixed_t value )
{
	this->ceilingXOffset = value;
	this->_ceilingXOffsetInitialized = true;
}

void ServerCommands::SetSectorPanning::SetCeilingYOffset( fixed_t value )
{
	this->ceilingYOffset = value;
	this->_ceilingYOffsetInitialized = true;
}

void ServerCommands::SetSectorPanning::SetFloorXOffset( fixed_t value )
{
	this->floorXOffset = value;
	this->_floorXOffsetInitialized = true;
}

void ServerCommands::SetSectorPanning::SetFloorYOffset( fixed_t value )
{
	this->floorYOffset = value;
	this->_floorYOffsetInitialized = true;
}

NetCommand ServerCommands::SetSectorRotation::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorRotation::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORROTATION );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->ceilingRotation );
	command.addShort( this->floorRotation );
	return command;
}

void ServerCommands::SetSectorRotation::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorRotation::SetCeilingRotation( int value )
{
	this->ceilingRotation = value;
	this->_ceilingRotationInitialized = true;
}

void ServerCommands::SetSectorRotation::SetFloorRotation( int value )
{
	this->floorRotation = value;
	this->_floorRotationInitialized = true;
}

NetCommand ServerCommands::SetSectorRotationByTag::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorRotationByTag::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORROTATIONBYTAG );
	command.addShort( this->tag );
	command.addShort( this->ceilingRotation );
	command.addShort( this->floorRotation );
	return command;
}

void ServerCommands::SetSectorRotationByTag::SetTag( int value )
{
	this->tag = value;
	this->_tagInitialized = true;
}

void ServerCommands::SetSectorRotationByTag::SetCeilingRotation( int value )
{
	this->ceilingRotation = value;
	this->_ceilingRotationInitialized = true;
}

void ServerCommands::SetSectorRotationByTag::SetFloorRotation( int value )
{
	this->floorRotation = value;
	this->_floorRotationInitialized = true;
}

NetCommand ServerCommands::SetSectorScale::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorScale::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORSCALE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addLong( this->ceilingXScale );
	command.addLong( this->ceilingYScale );
	command.addLong( this->floorXScale );
	command.addLong( this->floorYScale );
	return command;
}

void ServerCommands::SetSectorScale::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorScale::SetCeilingXScale( fixed_t value )
{
	this->ceilingXScale = value;
	this->_ceilingXScaleInitialized = true;
}

void ServerCommands::SetSectorScale::SetCeilingYScale( fixed_t value )
{
	this->ceilingYScale = value;
	this->_ceilingYScaleInitialized = true;
}

void ServerCommands::SetSectorScale::SetFloorXScale( fixed_t value )
{
	this->floorXScale = value;
	this->_floorXScaleInitialized = true;
}

void ServerCommands::SetSectorScale::SetFloorYScale( fixed_t value )
{
	this->floorYScale = value;
	this->_floorYScaleInitialized = true;
}

NetCommand ServerCommands::SetSectorSpecial::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorSpecial::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORSPECIAL );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->special );
	return command;
}

void ServerCommands::SetSectorSpecial::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorSpecial::SetSpecial( int value )
{
	this->special = value;
	this->_specialInitialized = true;
}

NetCommand ServerCommands::SetSectorFriction::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorFriction::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORFRICTION );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addLong( this->friction );
	command.addLong( this->moveFactor );
	return command;
}

void ServerCommands::SetSectorFriction::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorFriction::SetFriction( fixed_t value )
{
	this->friction = value;
	this->_frictionInitialized = true;
}

void ServerCommands::SetSectorFriction::SetMoveFactor( fixed_t value )
{
	this->moveFactor = value;
	this->_moveFactorInitialized = true;
}

NetCommand ServerCommands::SetSectorAngleYOffset::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorAngleYOffset::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORANGLEYOFFSET );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addLong( this->ceilingBaseAngle );
	command.addLong( this->ceilingBaseYOffset );
	command.addLong( this->floorBaseAngle );
	command.addLong( this->floorBaseYOffset );
	return command;
}

void ServerCommands::SetSectorAngleYOffset::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorAngleYOffset::SetCeilingBaseAngle( fixed_t value )
{
	this->ceilingBaseAngle = value;
	this->_ceilingBaseAngleInitialized = true;
}

void ServerCommands::SetSectorAngleYOffset::SetCeilingBaseYOffset( fixed_t value )
{
	this->ceilingBaseYOffset = value;
	this->_ceilingBaseYOffsetInitialized = true;
}

void ServerCommands::SetSectorAngleYOffset::SetFloorBaseAngle( fixed_t value )
{
	this->floorBaseAngle = value;
	this->_floorBaseAngleInitialized = true;
}

void ServerCommands::SetSectorAngleYOffset::SetFloorBaseYOffset( fixed_t value )
{
	this->floorBaseYOffset = value;
	this->_floorBaseYOffsetInitialized = true;
}

NetCommand ServerCommands::SetSectorGravity::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorGravity::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORGRAVITY );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addFloat( this->gravity );
	return command;
}

void ServerCommands::SetSectorGravity::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorGravity::SetGravity( float value )
{
	this->gravity = value;
	this->_gravityInitialized = true;
}

NetCommand ServerCommands::SetSectorReflection::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorReflection::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORREFLECTION );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addFloat( this->ceilingReflection );
	command.addFloat( this->floorReflection );
	return command;
}

void ServerCommands::SetSectorReflection::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorReflection::SetCeilingReflection( float value )
{
	this->ceilingReflection = value;
	this->_ceilingReflectionInitialized = true;
}

void ServerCommands::SetSectorReflection::SetFloorReflection( float value )
{
	this->floorReflection = value;
	this->_floorReflectionInitialized = true;
}

NetCommand ServerCommands::SetSectorLink::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSectorLink::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSECTORLINK );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->tag );
	command.addByte( this->ceiling );
	command.addByte( this->moveType );
	return command;
}

void ServerCommands::SetSectorLink::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SetSectorLink::SetTag( int value )
{
	this->tag = value;
	this->_tagInitialized = true;
}

void ServerCommands::SetSectorLink::SetCeiling( int value )
{
	this->ceiling = value;
	this->_ceilingInitialized = true;
}

void ServerCommands::SetSectorLink::SetMoveType( int value )
{
	this->moveType = value;
	this->_moveTypeInitialized = true;
}

NetCommand ServerCommands::StopSectorLightEffect::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: StopSectorLightEffect::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_STOPSECTORLIGHTEFFECT );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	return command;
}

void ServerCommands::StopSectorLightEffect::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

NetCommand ServerCommands::DestroyAllSectorMovers::BuildNetCommand() const
{
	NetCommand command ( SVC_DESTROYALLSECTORMOVERS );
	return command;
}

NetCommand ServerCommands::StartSectorSequence::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: StartSectorSequence::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_STARTSECTORSEQUENCE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addByte( this->channel );
	command.addString( this->sequence );
	command.addByte( this->modeNum );
	return command;
}

void ServerCommands::StartSectorSequence::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::StartSectorSequence::SetChannel( int value )
{
	this->channel = value;
	this->_channelInitialized = true;
}

void ServerCommands::StartSectorSequence::SetSequence( const FString & value )
{
	this->sequence = value;
	this->_sequenceInitialized = true;
}

void ServerCommands::StartSectorSequence::SetModeNum( int value )
{
	this->modeNum = value;
	this->_modeNumInitialized = true;
}

NetCommand ServerCommands::StopSectorSequence::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: StopSectorSequence::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_STOPSECTORSEQUENCE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	return command;
}

void ServerCommands::StopSectorSequence::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

NetCommand ServerCommands::DoSectorLightFireFlicker::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightFireFlicker::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTFIREFLICKER );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->maxLight );
	command.addShort( this->minLight );
	return command;
}

void ServerCommands::DoSectorLightFireFlicker::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoSectorLightFireFlicker::SetMaxLight( int value )
{
	this->maxLight = value;
	this->_maxLightInitialized = true;
}

void ServerCommands::DoSectorLightFireFlicker::SetMinLight( int value )
{
	this->minLight = value;
	this->_minLightInitialized = true;
}

NetCommand ServerCommands::DoSectorLightFlicker::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightFlicker::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTFLICKER );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->maxLight );
	command.addShort( this->minLight );
	return command;
}

void ServerCommands::DoSectorLightFlicker::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoSectorLightFlicker::SetMaxLight( int value )
{
	this->maxLight = value;
	this->_maxLightInitialized = true;
}

void ServerCommands::DoSectorLightFlicker::SetMinLight( int value )
{
	this->minLight = value;
	this->_minLightInitialized = true;
}

NetCommand ServerCommands::DoSectorLightLightFlash::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightLightFlash::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTLIGHTFLASH );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->maxLight );
	command.addShort( this->minLight );
	return command;
}

void ServerCommands::DoSectorLightLightFlash::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoSectorLightLightFlash::SetMaxLight( int value )
{
	this->maxLight = value;
	this->_maxLightInitialized = true;
}

void ServerCommands::DoSectorLightLightFlash::SetMinLight( int value )
{
	this->minLight = value;
	this->_minLightInitialized = true;
}

NetCommand ServerCommands::DoSectorLightStrobe::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightStrobe::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTSTROBE );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->darkTime );
	command.addShort( this->brightTime );
	command.addShort( this->maxLight );
	command.addShort( this->minLight );
	command.addByte( this->count );
	return command;
}

void ServerCommands::DoSectorLightStrobe::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoSectorLightStrobe::SetDarkTime( int value )
{
	this->darkTime = value;
	this->_darkTimeInitialized = true;
}

void ServerCommands::DoSectorLightStrobe::SetBrightTime( int value )
{
	this->brightTime = value;
	this->_brightTimeInitialized = true;
}

void ServerCommands::DoSectorLightStrobe::SetMaxLight( int value )
{
	this->maxLight = value;
	this->_maxLightInitialized = true;
}

void ServerCommands::DoSectorLightStrobe::SetMinLight( int value )
{
	this->minLight = value;
	this->_minLightInitialized = true;
}

void ServerCommands::DoSectorLightStrobe::SetCount( int value )
{
	this->count = value;
	this->_countInitialized = true;
}

NetCommand ServerCommands::DoSectorLightGlow::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightGlow::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTGLOW );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	return command;
}

void ServerCommands::DoSectorLightGlow::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

NetCommand ServerCommands::DoSectorLightGlow2::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightGlow2::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTGLOW2 );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->startLight );
	command.addShort( this->endLight );
	command.addShort( this->tics );
	command.addShort( this->maxTics );
	command.addBit( this->oneShot );
	return command;
}

void ServerCommands::DoSectorLightGlow2::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoSectorLightGlow2::SetStartLight( int value )
{
	this->startLight = value;
	this->_startLightInitialized = true;
}

void ServerCommands::DoSectorLightGlow2::SetEndLight( int value )
{
	this->endLight = value;
	this->_endLightInitialized = true;
}

void ServerCommands::DoSectorLightGlow2::SetTics( int value )
{
	this->tics = value;
	this->_ticsInitialized = true;
}

void ServerCommands::DoSectorLightGlow2::SetMaxTics( int value )
{
	this->maxTics = value;
	this->_maxTicsInitialized = true;
}

void ServerCommands::DoSectorLightGlow2::SetOneShot( bool value )
{
	this->oneShot = value;
	this->_oneShotInitialized = true;
}

NetCommand ServerCommands::DoSectorLightPhased::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: DoSectorLightPhased::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_DOSECTORLIGHTPHASED );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->baseLevel );
	command.addByte( this->phase );
	return command;
}

void ServerCommands::DoSectorLightPhased::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::DoSectorLightPhased::SetBaseLevel( int value )
{
	this->baseLevel = value;
	this->_baseLevelInitialized = true;
}

void ServerCommands::DoSectorLightPhased::SetPhase( int value )
{
	this->phase = value;
	this->_phaseInitialized = true;
}

NetCommand ServerCommands::SetLineAlpha::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineAlpha::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETLINEALPHA );
	command.addShort( this->line ? this->line - lines : -1 );
	command.addLong( this->alpha );
	return command;
}

void ServerCommands::SetLineAlpha::SetLine( line_t * value )
{
	this->line = value;
	this->_lineInitialized = true;
}

void ServerCommands::SetLineAlpha::SetAlpha( fixed_t value )
{
	this->alpha = value;
	this->_alphaInitialized = true;
}

NetCommand ServerCommands::SetLineTexture::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineTexture::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETLINETEXTURE );
	command.addShort( this->line ? this->line - lines : -1 );
	command.addString( this->textureName );
	command.addBit( this->side );
	command.addByte( this->position );
	return command;
}

void ServerCommands::SetLineTexture::SetLine( line_t * value )
{
	this->line = value;
	this->_lineInitialized = true;
}

void ServerCommands::SetLineTexture::SetTextureName( const FString & value )
{
	this->textureName = value;
	this->_textureNameInitialized = true;
}

void ServerCommands::SetLineTexture::SetSide( bool value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetLineTexture::SetPosition( int value )
{
	this->position = value;
	this->_positionInitialized = true;
}

NetCommand ServerCommands::SetLineTextureByID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineTextureByID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETLINETEXTUREBYID );
	command.addShort( this->lineID );
	command.addString( this->textureName );
	command.addBit( this->side );
	command.addByte( this->position );
	return command;
}

void ServerCommands::SetLineTextureByID::SetLineID( unsigned int value )
{
	this->lineID = value;
	this->_lineIDInitialized = true;
}

void ServerCommands::SetLineTextureByID::SetTextureName( const FString & value )
{
	this->textureName = value;
	this->_textureNameInitialized = true;
}

void ServerCommands::SetLineTextureByID::SetSide( bool value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetLineTextureByID::SetPosition( int value )
{
	this->position = value;
	this->_positionInitialized = true;
}

NetCommand ServerCommands::SetLineTextureOffset::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineTextureOffset::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETLINETEXTUREOFFSET );
	command.addShort( this->line ? this->line - lines : -1 );
	command.addLong( this->XOffset );
	command.addLong( this->YOffset );
	command.addBit( this->side );
	command.addByte( this->position );
	return command;
}

void ServerCommands::SetLineTextureOffset::SetLine( line_t * value )
{
	this->line = value;
	this->_lineInitialized = true;
}

void ServerCommands::SetLineTextureOffset::SetXOffset( fixed_t value )
{
	this->XOffset = value;
	this->_XOffsetInitialized = true;
}

void ServerCommands::SetLineTextureOffset::SetYOffset( fixed_t value )
{
	this->YOffset = value;
	this->_YOffsetInitialized = true;
}

void ServerCommands::SetLineTextureOffset::SetSide( bool value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetLineTextureOffset::SetPosition( int value )
{
	this->position = value;
	this->_positionInitialized = true;
}

NetCommand ServerCommands::SetLineTextureOffsetByID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineTextureOffsetByID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETLINETEXTUREOFFSETBYID );
	command.addShort( this->lineID );
	command.addLong( this->XOffset );
	command.addLong( this->YOffset );
	command.addBit( this->side );
	command.addByte( this->flags );
	return command;
}

void ServerCommands::SetLineTextureOffsetByID::SetLineID( unsigned int value )
{
	this->lineID = value;
	this->_lineIDInitialized = true;
}

void ServerCommands::SetLineTextureOffsetByID::SetXOffset( fixed_t value )
{
	this->XOffset = value;
	this->_XOffsetInitialized = true;
}

void ServerCommands::SetLineTextureOffsetByID::SetYOffset( fixed_t value )
{
	this->YOffset = value;
	this->_YOffsetInitialized = true;
}

void ServerCommands::SetLineTextureOffsetByID::SetSide( bool value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetLineTextureOffsetByID::SetFlags( int value )
{
	this->flags = value;
	this->_flagsInitialized = true;
}

NetCommand ServerCommands::SetLineTextureScale::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineTextureScale::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETLINETEXTURESCALE );
	command.addShort( this->line ? this->line - lines : -1 );
	command.addLong( this->XScale );
	command.addLong( this->YScale );
	command.addBit( this->side );
	command.addByte( this->position );
	return command;
}

void ServerCommands::SetLineTextureScale::SetLine( line_t * value )
{
	this->line = value;
	this->_lineInitialized = true;
}

void ServerCommands::SetLineTextureScale::SetXScale( fixed_t value )
{
	this->XScale = value;
	this->_XScaleInitialized = true;
}

void ServerCommands::SetLineTextureScale::SetYScale( fixed_t value )
{
	this->YScale = value;
	this->_YScaleInitialized = true;
}

void ServerCommands::SetLineTextureScale::SetSide( bool value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetLineTextureScale::SetPosition( int value )
{
	this->position = value;
	this->_positionInitialized = true;
}

NetCommand ServerCommands::SetLineTextureScaleByID::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetLineTextureScaleByID::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETLINETEXTURESCALEBYID );
	command.addShort( this->lineID );
	command.addLong( this->XScale );
	command.addLong( this->YScale );
	command.addBit( this->side );
	command.addByte( this->flags );
	return command;
}

void ServerCommands::SetLineTextureScaleByID::SetLineID( unsigned int value )
{
	this->lineID = value;
	this->_lineIDInitialized = true;
}

void ServerCommands::SetLineTextureScaleByID::SetXScale( fixed_t value )
{
	this->XScale = value;
	this->_XScaleInitialized = true;
}

void ServerCommands::SetLineTextureScaleByID::SetYScale( fixed_t value )
{
	this->YScale = value;
	this->_YScaleInitialized = true;
}

void ServerCommands::SetLineTextureScaleByID::SetSide( bool value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetLineTextureScaleByID::SetFlags( int value )
{
	this->flags = value;
	this->_flagsInitialized = true;
}

NetCommand ServerCommands::SetSomeLineFlags::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSomeLineFlags::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSOMELINEFLAGS );
	command.addShort( this->line ? this->line - lines : -1 );
	command.addLong( this->blockFlags );
	return command;
}

void ServerCommands::SetSomeLineFlags::SetLine( line_t * value )
{
	this->line = value;
	this->_lineInitialized = true;
}

void ServerCommands::SetSomeLineFlags::SetBlockFlags( int value )
{
	this->blockFlags = value;
	this->_blockFlagsInitialized = true;
}

NetCommand ServerCommands::SetSideFlags::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetSideFlags::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SETSIDEFLAGS );
	command.addLong( this->side ? this->side - sides : -1 );
	command.addByte( this->flags );
	return command;
}

void ServerCommands::SetSideFlags::SetSide( side_t * value )
{
	this->side = value;
	this->_sideInitialized = true;
}

void ServerCommands::SetSideFlags::SetFlags( int value )
{
	this->flags = value;
	this->_flagsInitialized = true;
}

NetCommand ServerCommands::Sound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: Sound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SOUND );
	command.addByte( this->channel );
	command.addString( this->sound );
	command.addByte( this->volume );
	command.addByte( this->attenuation );
	return command;
}

void ServerCommands::Sound::SetChannel( int value )
{
	this->channel = value;
	this->_channelInitialized = true;
}

void ServerCommands::Sound::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

void ServerCommands::Sound::SetVolume( int value )
{
	this->volume = value;
	this->_volumeInitialized = true;
}

void ServerCommands::Sound::SetAttenuation( int value )
{
	this->attenuation = value;
	this->_attenuationInitialized = true;
}

NetCommand ServerCommands::SoundActor::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SoundActor::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SOUNDACTOR );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->channel );
	command.addString( this->sound );
	command.addByte( this->volume );
	command.addByte( this->attenuation );
	return command;
}

void ServerCommands::SoundActor::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::SoundActor::SetChannel( int value )
{
	this->channel = value;
	this->_channelInitialized = true;
}

void ServerCommands::SoundActor::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

void ServerCommands::SoundActor::SetVolume( int value )
{
	this->volume = value;
	this->_volumeInitialized = true;
}

void ServerCommands::SoundActor::SetAttenuation( int value )
{
	this->attenuation = value;
	this->_attenuationInitialized = true;
}

NetCommand ServerCommands::SoundActorIfNotPlaying::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SoundActorIfNotPlaying::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SOUNDACTORIFNOTPLAYING );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addShort( this->channel );
	command.addString( this->sound );
	command.addByte( this->volume );
	command.addByte( this->attenuation );
	return command;
}

NetCommand ServerCommands::SoundSector::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SoundSector::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SOUNDSECTOR );
	command.addShort( this->sector ? this->sector - sectors : -1 );
	command.addShort( this->channel );
	command.addString( this->sound );
	command.addByte( this->volume );
	command.addByte( this->attenuation );
	return command;
}

void ServerCommands::SoundSector::SetSector( sector_t * value )
{
	this->sector = value;
	this->_sectorInitialized = true;
}

void ServerCommands::SoundSector::SetChannel( int value )
{
	this->channel = value;
	this->_channelInitialized = true;
}

void ServerCommands::SoundSector::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

void ServerCommands::SoundSector::SetVolume( int value )
{
	this->volume = value;
	this->_volumeInitialized = true;
}

void ServerCommands::SoundSector::SetAttenuation( int value )
{
	this->attenuation = value;
	this->_attenuationInitialized = true;
}

NetCommand ServerCommands::SoundPoint::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SoundPoint::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SOUNDPOINT );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addByte( this->channel );
	command.addString( this->sound );
	command.addByte( this->volume );
	command.addByte( this->attenuation );
	return command;
}

void ServerCommands::SoundPoint::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SoundPoint::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SoundPoint::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SoundPoint::SetChannel( int value )
{
	this->channel = value;
	this->_channelInitialized = true;
}

void ServerCommands::SoundPoint::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

void ServerCommands::SoundPoint::SetVolume( int value )
{
	this->volume = value;
	this->_volumeInitialized = true;
}

void ServerCommands::SoundPoint::SetAttenuation( int value )
{
	this->attenuation = value;
	this->_attenuationInitialized = true;
}

NetCommand ServerCommands::AnnouncerSound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: AnnouncerSound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_ANNOUNCERSOUND );
	command.addString( this->sound );
	return command;
}

void ServerCommands::AnnouncerSound::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

NetCommand ServerCommands::StopSound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: StopSound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_STOPSOUND );
	command.addShort( this->actor ? this->actor->NetID : -1 );
	command.addByte( this->channel );
	return command;
}

void ServerCommands::StopSound::SetActor( AActor * value )
{
	this->actor = value;
	this->_actorInitialized = true;
}

void ServerCommands::StopSound::SetChannel( int value )
{
	this->channel = value;
	this->_channelInitialized = true;
}

NetCommand ServerCommands::SpawnMissile::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnMissile::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNMISSILE );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	command.addLong( this->velX );
	command.addLong( this->velY );
	command.addLong( this->velZ );
	command.addShort( this->missileType ? this->missileType->getActorNetworkIndex() : -1 );
	command.addShort( this->netID );
	command.addShort( this->targetNetID );
	return command;
}

void ServerCommands::SpawnMissile::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnMissile::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnMissile::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnMissile::SetVelX( fixed_t value )
{
	this->velX = value;
	this->_velXInitialized = true;
}

void ServerCommands::SpawnMissile::SetVelY( fixed_t value )
{
	this->velY = value;
	this->_velYInitialized = true;
}

void ServerCommands::SpawnMissile::SetVelZ( fixed_t value )
{
	this->velZ = value;
	this->_velZInitialized = true;
}

void ServerCommands::SpawnMissile::SetMissileType( const PClass * value )
{
	this->missileType = value;
	this->_missileTypeInitialized = true;
}

void ServerCommands::SpawnMissile::SetNetID( int value )
{
	this->netID = value;
	this->_netIDInitialized = true;
}

void ServerCommands::SpawnMissile::SetTargetNetID( int value )
{
	this->targetNetID = value;
	this->_targetNetIDInitialized = true;
}

NetCommand ServerCommands::SpawnMissileExact::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SpawnMissileExact::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_SPAWNMISSILEEXACT );
	command.addLong( this->x );
	command.addLong( this->y );
	command.addLong( this->z );
	command.addLong( this->velX );
	command.addLong( this->velY );
	command.addLong( this->velZ );
	command.addShort( this->missileType ? this->missileType->getActorNetworkIndex() : -1 );
	command.addShort( this->netID );
	command.addShort( this->targetNetID );
	return command;
}

void ServerCommands::SpawnMissileExact::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetVelX( fixed_t value )
{
	this->velX = value;
	this->_velXInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetVelY( fixed_t value )
{
	this->velY = value;
	this->_velYInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetVelZ( fixed_t value )
{
	this->velZ = value;
	this->_velZInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetMissileType( const PClass * value )
{
	this->missileType = value;
	this->_missileTypeInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetNetID( int value )
{
	this->netID = value;
	this->_netIDInitialized = true;
}

void ServerCommands::SpawnMissileExact::SetTargetNetID( int value )
{
	this->targetNetID = value;
	this->_targetNetIDInitialized = true;
}

NetCommand ServerCommands::MissileExplode::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: MissileExplode::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_MISSILEEXPLODE );
	command.addShort( this->missile ? this->missile->NetID : -1 );
	command.addShort( this->lineId );
	command.addShort( this->x >> FRACBITS );
	command.addShort( this->y >> FRACBITS );
	command.addShort( this->z >> FRACBITS );
	return command;
}

void ServerCommands::MissileExplode::SetMissile( AActor * value )
{
	this->missile = value;
	this->_missileInitialized = true;
}

void ServerCommands::MissileExplode::SetLineId( int value )
{
	this->lineId = value;
	this->_lineIdInitialized = true;
}

void ServerCommands::MissileExplode::SetX( fixed_t value )
{
	this->x = value;
	this->_xInitialized = true;
}

void ServerCommands::MissileExplode::SetY( fixed_t value )
{
	this->y = value;
	this->_yInitialized = true;
}

void ServerCommands::MissileExplode::SetZ( fixed_t value )
{
	this->z = value;
	this->_zInitialized = true;
}

NetCommand ServerCommands::WeaponSound::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: WeaponSound::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_WEAPONSOUND );
	command.addByte( this->player - players );
	command.addString( this->sound );
	return command;
}

void ServerCommands::WeaponSound::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::WeaponSound::SetSound( const FString & value )
{
	this->sound = value;
	this->_soundInitialized = true;
}

NetCommand ServerCommands::WeaponChange::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: WeaponChange::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_WEAPONCHANGE );
	command.addByte( this->player - players );
	command.addShort( this->weaponType ? this->weaponType->getActorNetworkIndex() : -1 );
	return command;
}

void ServerCommands::WeaponChange::SetPlayer( player_t * value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::WeaponChange::SetWeaponType( const PClass * value )
{
	this->weaponType = value;
	this->_weaponTypeInitialized = true;
}

NetCommand ServerCommands::WeaponRailgun::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: WeaponRailgun::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_WEAPONRAILGUN );
	command.addShort( this->source ? this->source->NetID : -1 );
	command.addFloat( this->start.X );
	command.addFloat( this->start.Y );
	command.addFloat( this->start.Z );
	command.addFloat( this->end.X );
	command.addFloat( this->end.Y );
	command.addFloat( this->end.Z );
	command.addLong( this->color1 );
	command.addLong( this->color2 );
	command.addFloat( this->maxdiff );
	command.addShortByte( this->flags, 7 );
	command.addBit( this->extended );
	if ( CheckExtended() )
	{
		command.addLong( this->angleoffset );
		command.addShort( this->spawnclass ? this->spawnclass->getActorNetworkIndex() : -1 );
		command.addShort( this->duration );
		command.addFloat( this->sparsity );
		command.addFloat( this->drift );
	}
	return command;
}

void ServerCommands::WeaponRailgun::SetSource( AActor * value )
{
	this->source = value;
	this->_sourceInitialized = true;
}

void ServerCommands::WeaponRailgun::SetStart( const FVector3 & value )
{
	this->start = value;
	this->_startInitialized = true;
}

void ServerCommands::WeaponRailgun::SetEnd( const FVector3 & value )
{
	this->end = value;
	this->_endInitialized = true;
}

void ServerCommands::WeaponRailgun::SetColor1( int value )
{
	this->color1 = value;
	this->_color1Initialized = true;
}

void ServerCommands::WeaponRailgun::SetColor2( int value )
{
	this->color2 = value;
	this->_color2Initialized = true;
}

void ServerCommands::WeaponRailgun::SetMaxdiff( float value )
{
	this->maxdiff = value;
	this->_maxdiffInitialized = true;
}

void ServerCommands::WeaponRailgun::SetFlags( int value )
{
	this->flags = value;
	this->_flagsInitialized = true;
}

void ServerCommands::WeaponRailgun::SetExtended( bool value )
{
	this->extended = value;
	this->_extendedInitialized = true;
}

void ServerCommands::WeaponRailgun::SetAngleoffset( int value )
{
	this->angleoffset = value;
	this->_angleoffsetInitialized = true;
}

void ServerCommands::WeaponRailgun::SetSpawnclass( const PClass * value )
{
	this->spawnclass = value;
	this->_spawnclassInitialized = true;
}

void ServerCommands::WeaponRailgun::SetDuration( int value )
{
	this->duration = value;
	this->_durationInitialized = true;
}

void ServerCommands::WeaponRailgun::SetSparsity( float value )
{
	this->sparsity = value;
	this->_sparsityInitialized = true;
}

void ServerCommands::WeaponRailgun::SetDrift( float value )
{
	this->drift = value;
	this->_driftInitialized = true;
}

bool ServerCommands::WeaponRailgun::CheckExtended() const
{
	return !!( extended );
}

NetCommand ServerCommands::ACSScriptExecute::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ACSScriptExecute::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_ACSSCRIPTEXECUTE );
	command.addShort( this->netid );
	command.addShort( this->activator ? this->activator->NetID : -1 );
	command.addShort( this->lineid );
	command.addByte( this->levelnum );
	command.addVariable( this->arg0 );
	command.addVariable( this->arg1 );
	command.addVariable( this->arg2 );
	command.addVariable( this->arg3 );
	command.addBit( this->backSide );
	command.addBit( this->always );
	return command;
}

void ServerCommands::ACSScriptExecute::SetNetid( int value )
{
	this->netid = value;
	this->_netidInitialized = true;
}

void ServerCommands::ACSScriptExecute::SetActivator( AActor * value )
{
	this->activator = value;
	this->_activatorInitialized = true;
}

void ServerCommands::ACSScriptExecute::SetLineid( int value )
{
	this->lineid = value;
	this->_lineidInitialized = true;
}

void ServerCommands::ACSScriptExecute::SetLevelnum( int value )
{
	this->levelnum = value;
	this->_levelnumInitialized = true;
}

void ServerCommands::ACSScriptExecute::SetArg0( int value )
{
	this->arg0 = value;
	this->_arg0Initialized = true;
}

void ServerCommands::ACSScriptExecute::SetArg1( int value )
{
	this->arg1 = value;
	this->_arg1Initialized = true;
}

void ServerCommands::ACSScriptExecute::SetArg2( int value )
{
	this->arg2 = value;
	this->_arg2Initialized = true;
}

void ServerCommands::ACSScriptExecute::SetArg3( int value )
{
	this->arg3 = value;
	this->_arg3Initialized = true;
}

void ServerCommands::ACSScriptExecute::SetBackSide( bool value )
{
	this->backSide = value;
	this->_backSideInitialized = true;
}

void ServerCommands::ACSScriptExecute::SetAlways( bool value )
{
	this->always = value;
	this->_alwaysInitialized = true;
}

NetCommand ServerCommands::ACSSendString::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ACSSendString::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_ACSSENDSTRING );
	command.addShort( this->netid );
	command.addShort( this->activator ? this->activator->NetID : -1 );
	command.addString( this->string );
	return command;
}

void ServerCommands::ACSSendString::SetNetid( int value )
{
	this->netid = value;
	this->_netidInitialized = true;
}

void ServerCommands::ACSSendString::SetActivator( AActor * value )
{
	this->activator = value;
	this->_activatorInitialized = true;
}

void ServerCommands::ACSSendString::SetString( const FString & value )
{
	this->string = value;
	this->_stringInitialized = true;
}

NetCommand ServerCommands::SyncJoinQueue::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SyncJoinQueue::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SYNCJOINQUEUE );
	command.addByte( slots.Size() );
	for ( unsigned int i = 0; i < slots.Size(); ++i )
	{
		command.addByte( this->slots[i].player );
		command.addByte( this->slots[i].team );
	}
	return command;
}

void ServerCommands::SyncJoinQueue::SetSlots( const TArray<struct JoinSlot> & value )
{
	this->slots = value;
	this->_slotsInitialized = true;
}

NetCommand ServerCommands::SyncMapRotation::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SyncMapRotation::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SYNCMAPROTATION );
	command.addByte( entries.Size() );
	for ( unsigned int i = 0; i < entries.Size(); ++i )
	{
		command.addString( this->entries[i].name );
		command.addByte( this->entries[i].isUsed );
		command.addByte( this->entries[i].minPlayers );
		command.addByte( this->entries[i].maxPlayers );
	}
	command.addShort( this->currentPosition );
	return command;
}

void ServerCommands::SyncMapRotation::SetEntries( const TArray<struct MapRotationEntry> & value )
{
	this->entries = value;
	this->_entriesInitialized = true;
}

void ServerCommands::SyncMapRotation::SetCurrentPosition( int value )
{
	this->currentPosition = value;
	this->_currentPositionInitialized = true;
}

NetCommand ServerCommands::ReplaceTextures::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ReplaceTextures::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC_REPLACETEXTURES );
	command.addString( this->fromTexture );
	command.addString( this->toTexture );
	command.addByte( this->textureFlags );
	return command;
}

void ServerCommands::ReplaceTextures::SetFromTexture( const FString & value )
{
	this->fromTexture = value;
	this->_fromTextureInitialized = true;
}

void ServerCommands::ReplaceTextures::SetToTexture( const FString & value )
{
	this->toTexture = value;
	this->_toTextureInitialized = true;
}

void ServerCommands::ReplaceTextures::SetTextureFlags( int value )
{
	this->textureFlags = value;
	this->_textureFlagsInitialized = true;
}

NetCommand ServerCommands::SetCustomPlayerValue::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: SetCustomPlayerValue::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_SETCUSTOMPLAYERVALUE );
	command.addByte( this->index );
	command.addByte( this->player );
	command.addString( this->value );
	return command;
}

void ServerCommands::SetCustomPlayerValue::SetIndex( int value )
{
	this->index = value;
	this->_indexInitialized = true;
}

void ServerCommands::SetCustomPlayerValue::SetPlayer( int value )
{
	this->player = value;
	this->_playerInitialized = true;
}

void ServerCommands::SetCustomPlayerValue::SetValue( const FString & value )
{
	this->value = value;
	this->_valueInitialized = true;
}

NetCommand ServerCommands::ResetCustomPlayerValue::BuildNetCommand() const
{
	if ( AllParametersInitialized() == false )
	{
		Printf( "WARNING: ResetCustomPlayerValue::BuildNetCommand: not all parameters were initialized:\n" );
		PrintMissingParameters();
	}
	NetCommand command ( SVC2_RESETCUSTOMPLAYERVALUE );
	command.addByte( this->index );
	command.addByte( this->player );
	return command;
}

void ServerCommands::ResetCustomPlayerValue::SetIndex( int value )
{
	this->index = value;
	this->_indexInitialized = true;
}

void ServerCommands::ResetCustomPlayerValue::SetPlayer( int value )
{
	this->player = value;
	this->_playerInitialized = true;
}


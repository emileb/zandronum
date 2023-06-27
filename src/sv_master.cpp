//-----------------------------------------------------------------------------
//
// Skulltag Source
// Copyright (C) 2003 Brad Carney
// Copyright (C) 2007-2012 Skulltag Development Team
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of the Skulltag Development Team nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
// 4. Redistributions in any form must be accompanied by information on how to
//    obtain complete source code for the software and any accompanying
//    software that uses the software. The source code must either be included
//    in the distribution or be available for no more than the cost of
//    distribution plus a nominal fee, and must be freely redistributable
//    under reasonable conditions. For an executable file, complete source
//    code means the source code for all modules it contains. It does not
//    include source code for modules or files that typically accompany the
//    major components of the operating system on which the executable file
//    runs.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// File created:  8/27/03
//
//
// Filename: sv_master.cpp
//
// Description: Server-to-Master and Server-to-Launcher protocol.
//
//-----------------------------------------------------------------------------

#ifndef _WIN32
#include <sys/utsname.h>
#endif
#include "networkheaders.h"
#include "c_dispatch.h"
#include "cooperative.h"
#include "deathmatch.h"
#include "doomstat.h"
#include "d_player.h"
#include "duel.h"
#include "g_game.h"
#include "gamemode.h"
#include "gi.h"
#include "g_level.h"
#include "i_system.h"
#include "lastmanstanding.h"
#include "team.h"
#include "network.h"
#include "sv_main.h"
#include "sv_ban.h"
#include "version.h"
#include "d_dehacked.h"
#include "v_text.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------
//-- VARIABLES -------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------

// Address of master server.
static	NETADDRESS_s		g_AddressMasterServer;

// Message buffer for sending messages to the master server.
static	NETBUFFER_s			g_MasterServerBuffer;

// Port the master server is located on.
static	USHORT				g_usMasterPort;

// List of IP address that this server has been queried by recently.
static	STORED_QUERY_IP_s	g_StoredQueryIPs[MAX_STORED_QUERY_IPS];

static	LONG				g_lStoredQueryIPHead;
static	LONG				g_lStoredQueryIPTail;
static	TArray<int>			g_OptionalWadIndices;

extern	NETADDRESS_s		g_LocalAddress;

FString g_VersionWithOS;

//*****************************************************************************
//	CONSOLE VARIABLES

//--------------------------------------------------------------------------------------------------------------------------------------------------
//-- FUNCTIONS -------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------

//*****************************************************************************
//
void SERVER_MASTER_Construct( void )
{
	const char *pszPort;

	// Setup our message buffer.
	g_MasterServerBuffer.Init( MAX_UDP_PACKET, BUFFERTYPE_WRITE );
	g_MasterServerBuffer.Clear();

	// Allow the user to specify which port the master server is on.
	pszPort = Args->CheckValue( "-masterport" );
    if ( pszPort )
    {
       g_usMasterPort = atoi( pszPort );
       Printf( PRINT_HIGH, "Alternate master server port: %d.\n", g_usMasterPort );
    }
	else 
	   g_usMasterPort = DEFAULT_MASTER_PORT;

	g_lStoredQueryIPHead = 0;
	g_lStoredQueryIPTail = 0;

#ifndef _WIN32
	struct utsname u_name;
	if ( uname(&u_name) < 0 )
		g_VersionWithOS.Format ( "%s", GetVersionStringRev() ); //error, no data
	else
		g_VersionWithOS.Format ( "%s on %s %s", GetVersionStringRev(), u_name.sysname, u_name.release ); // "Linux 2.6.32.5-amd64" or "FreeBSD 9.0-RELEASE" etc
#endif

	// [TP] Which wads will we broadcast as optional to launchers?
	for( unsigned i = 0; i < NETWORK_GetPWADList().Size(); ++i )
	{
		if ( Wads.IsWadOptional( NETWORK_GetPWADList()[i].wadnum ))
			g_OptionalWadIndices.Push( i );
	}

	// Call SERVER_MASTER_Destruct() when Skulltag closes.
	atterm( SERVER_MASTER_Destruct );
}

//*****************************************************************************
//
void SERVER_MASTER_Destruct( void )
{
	// Free our local buffer.
	g_MasterServerBuffer.Free();
}

//*****************************************************************************
//
void SERVER_MASTER_Tick( void )
{
	while (( g_lStoredQueryIPHead != g_lStoredQueryIPTail ) && ( gametic >= g_StoredQueryIPs[g_lStoredQueryIPHead].lNextAllowedGametic ))
	{
		g_lStoredQueryIPHead++;
		g_lStoredQueryIPHead = g_lStoredQueryIPHead % MAX_STORED_QUERY_IPS;
	}

	// Send an update to the master server every 30 seconds.
	if ( gametic % ( TICRATE * 30 ))
		return;

	// User doesn't wish to update the master server.
	if ( sv_updatemaster == false )
		return;

	g_MasterServerBuffer.Clear();

	// [BB] If we can't find the master address, we can't tick the master.
	bool ok = g_AddressMasterServer.LoadFromString( masterhostname );

	if ( ok == false )
	{
		Printf ( "Warning: Can't find masterhostname %s! Either correct masterhostname or set sv_updatemaster to false.\n", *masterhostname );
		return;
	}

	g_AddressMasterServer.SetPort( g_usMasterPort );

	// Write to our packet a challenge to the master server.
	g_MasterServerBuffer.ByteStream.WriteLong( SERVER_MASTER_CHALLENGE );
	// [BB] Also send a string that will allow us to verify that a master banlist was actually sent from the master.
	g_MasterServerBuffer.ByteStream.WriteString( SERVER_GetMasterBanlistVerificationString().GetChars() );
	// [BB] Also tell the master whether we are enforcing its ban list.
	g_MasterServerBuffer.ByteStream.WriteByte( sv_enforcemasterbanlist );
	// [BB] And tell which code revision number the server was built with.
	g_MasterServerBuffer.ByteStream.WriteLong( GetRevisionNumber() );

	// Send the master server our packet.
//	NETWORK_LaunchPacket( &g_MasterServerBuffer, g_AddressMasterServer, true );
	NETWORK_LaunchPacket( &g_MasterServerBuffer, g_AddressMasterServer );
}

//*****************************************************************************
//
void SERVER_MASTER_Broadcast( void )
{
	// Send an update to the master server every second.
	if ( gametic % TICRATE )
		return;

	// User doesn't wish to broadcast this server.
	if (( sv_broadcast == false ) || ( Args->CheckParm( "-nobroadcast" )))
		return;

//	g_MasterServerBuffer.Clear();

	sockaddr_in broadcast_addr;
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;
	broadcast_addr.sin_port = htons( DEFAULT_BROADCAST_PORT );
	NETADDRESS_s AddressBroadcast;
	AddressBroadcast.LoadFromSocketAddress( reinterpret_cast<const sockaddr&> ( broadcast_addr ) );

	// [BB] Under all Windows versions broadcasts to INADDR_BROADCAST seem to work fine
	// while class A broadcasts don't work under Vista/7. So just use INADDR_BROADCAST.
#ifndef _WIN32
	// [BB] Based on the local adress, we find out the class
	// of the network, we are in and set the broadcast address
	// accordingly. Broadcasts to INADDR_BROADCAST = 255.255.255.255
	// should be circumvented if possible and is seem that they
	// aren't	even permitted in the Linux kernel at all.
	// If the server has the ip A.B.C.D depending on the class
	// broadcasts should go to:
	// Class A: A.255.255.255
	// Class B: A. B .255.255
	// Class C: A. B . C .255
	// 
	// Class A comprises networks 1.0.0.0 through 127.0.0.0. The network number is contained in the first octet.
	// Class B contains networks 128.0.0.0 through 191.255.0.0; the network number is in the first two octets.
	// Class C networks range from 192.0.0.0 through 223.255.255.0, with the network number contained in the first three octets.

	int classIndex = 0;

	const int locIP0 = g_LocalAddress.abIP[0];
	if ( (locIP0 >= 1) && (locIP0 <= 127) )
		classIndex = 1;
	else if ( (locIP0 >= 128 ) && (locIP0 <= 191) )
		classIndex = 2;
	else if ( (locIP0 >= 192 ) && (locIP0 <= 223) )
		classIndex = 3;

	for( int i = 0; i < classIndex; i++ )
		AddressBroadcast.abIP[i] = g_LocalAddress.abIP[i];
#endif

	// Broadcast our packet.
	SERVER_MASTER_SendServerInfo( AddressBroadcast, SQF_ALL, 0, SQF2_ALL, true );
//	NETWORK_WriteLong( &g_MasterServerBuffer, MASTER_CHALLENGE );
//	NETWORK_LaunchPacket( g_MasterServerBuffer, AddressBroadcast, true );
}

//*****************************************************************************
//
void SERVER_MASTER_SendServerInfo( NETADDRESS_s Address, ULONG ulFlags, ULONG ulTime, ULONG ulFlags2, bool bBroadcasting )
{
	IPStringArray szAddress;
	ULONG		ulIdx;
	ULONG		ulBits;
	ULONG 		ulBits2;

	// Let's just use the master server buffer! It gets cleared again when we need it anyway!
	g_MasterServerBuffer.Clear();

	if ( bBroadcasting == false )
	{
		// First, check to see if we've been queried by this address recently.
		if ( g_lStoredQueryIPHead != g_lStoredQueryIPTail )
		{
			ulIdx = g_lStoredQueryIPHead;
			while ( ulIdx != (ULONG)g_lStoredQueryIPTail )
			{
				// Check to see if this IP exists in our stored query IP list. If it does, then
				// ignore it, since it queried us less than 10 seconds ago.
				if ( Address.CompareNoPort( g_StoredQueryIPs[ulIdx].Address ))
				{
					// Write our header.
					g_MasterServerBuffer.ByteStream.WriteLong( SERVER_LAUNCHER_IGNORING );

					// Send the time the launcher sent to us.
					g_MasterServerBuffer.ByteStream.WriteLong( ulTime );

					// Send the packet.
	//				NETWORK_LaunchPacket( &g_MasterServerBuffer, Address, true );
					NETWORK_LaunchPacket( &g_MasterServerBuffer, Address );

					if ( sv_showlauncherqueries )
						Printf( "Ignored IP launcher challenge.\n" );

					// Nothing more to do here.
					return;
				}

				ulIdx++;
				ulIdx = ulIdx % MAX_STORED_QUERY_IPS;
			}
		}
	
		// Now, check to see if this IP has been banend from this server.
		szAddress.SetFrom ( Address );
		if ( SERVERBAN_IsIPBanned( szAddress ))
		{
			// Write our header.
			g_MasterServerBuffer.ByteStream.WriteLong( SERVER_LAUNCHER_BANNED );

			// Send the time the launcher sent to us.
			g_MasterServerBuffer.ByteStream.WriteLong( ulTime );

			// Send the packet.
			NETWORK_LaunchPacket( &g_MasterServerBuffer, Address );

			if ( sv_showlauncherqueries )
				Printf( "Denied BANNED IP launcher challenge.\n" );

			// Nothing more to do here.
			return;
		}

		// This IP didn't exist in the list. and it wasn't banned. 
		// So, add it, and keep it there for 10 seconds.
		g_StoredQueryIPs[g_lStoredQueryIPTail].Address = Address;
		g_StoredQueryIPs[g_lStoredQueryIPTail].lNextAllowedGametic = gametic + ( TICRATE * ( sv_queryignoretime ));

		g_lStoredQueryIPTail++;
		g_lStoredQueryIPTail = g_lStoredQueryIPTail % MAX_STORED_QUERY_IPS;
		if ( g_lStoredQueryIPTail == g_lStoredQueryIPHead )
			Printf( "SERVER_MASTER_SendServerInfo: WARNING! g_lStoredQueryIPTail == g_lStoredQueryIPHead\n" );
	}

	// Write our header.
	g_MasterServerBuffer.ByteStream.WriteLong( SERVER_LAUNCHER_CHALLENGE );

	// Send the time the launcher sent to us.
	g_MasterServerBuffer.ByteStream.WriteLong( ulTime );

	// Send our version. [K6] ...with OS
	g_MasterServerBuffer.ByteStream.WriteString( g_VersionWithOS.GetChars() );

	// Send the information about the data that will be sent.
	ulBits = ulFlags;

	// [BB] Remove all unknown flags from our answer.
	ulBits &= SQF_ALL;

	// If the launcher desires to know the team damage, but we're not in a game mode where
	// team damage applies, then don't send back team damage information.
	if (( teamplay || teamgame || teamlms || teampossession || (( deathmatch == false ) && ( teamgame == false ))) == false )
	{
		if ( ulBits & SQF_TEAMDAMAGE )
			ulBits &= ~SQF_TEAMDAMAGE;
	}

	// If the launcher desires to know the team score, but we're not in a game mode where
	// teams have scores, then don't send back team score information.
	if (( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS ) == false )
		ulBits &= ~( SQF_TEAMSCORES | SQF_TEAMINFO_NUMBER | SQF_TEAMINFO_NAME | SQF_TEAMINFO_COLOR | SQF_TEAMINFO_SCORE );

	// If the launcher wants to know player data, then we have to tell them how many players
	// are in the server.
	if ( ulBits & SQF_PLAYERDATA )
		ulBits |= SQF_NUMPLAYERS;

	// [TP] Don't send optional wads if there isn't any.
	if ( g_OptionalWadIndices.Size() == 0 )
		ulBits &= ~SQF_OPTIONAL_WADS;

	// [TP] Don't send deh files if there aren't any.
	if ( D_GetDehFileNames().Size() == 0 )
		ulBits &= ~SQF_DEH;

	// [SB] If the client didn't send any extended flags, don't send any extended info
	if ( ulFlags2 == 0 )
		ulBits &= ~SQF_EXTENDED_INFO;

	g_MasterServerBuffer.ByteStream.WriteLong( ulBits );

	// Send the server name.
	if ( ulBits & SQF_NAME )
	{
		// [AK] Remove any color codes in the server name first.
		FString uncolorizedHostname = sv_hostname.GetGenericRep( CVAR_String ).String;
		V_ColorizeString( uncolorizedHostname );
		V_RemoveColorCodes( uncolorizedHostname );

		g_MasterServerBuffer.ByteStream.WriteString( uncolorizedHostname );
	}

	// Send the website URL.
	if ( ulBits & SQF_URL )
		g_MasterServerBuffer.ByteStream.WriteString( sv_website );

	// Send the host's e-mail address.
	if ( ulBits & SQF_EMAIL )
		g_MasterServerBuffer.ByteStream.WriteString( sv_hostemail );

	if ( ulBits & SQF_MAPNAME )
		g_MasterServerBuffer.ByteStream.WriteString( level.mapname );

	if ( ulBits & SQF_MAXCLIENTS )
		g_MasterServerBuffer.ByteStream.WriteByte( sv_maxclients );

	if ( ulBits & SQF_MAXPLAYERS )
		g_MasterServerBuffer.ByteStream.WriteByte( sv_maxplayers );

	// Send out the PWAD information.
	if ( ulBits & SQF_PWADS )
	{
		g_MasterServerBuffer.ByteStream.WriteByte( NETWORK_GetPWADList().Size( ));

		for ( unsigned i = 0; i < NETWORK_GetPWADList().Size(); ++i )
			g_MasterServerBuffer.ByteStream.WriteString( NETWORK_GetPWADList()[i].name );
	}

	if ( ulBits & SQF_GAMETYPE )
	{
		g_MasterServerBuffer.ByteStream.WriteByte( GAMEMODE_GetCurrentMode( ));
		g_MasterServerBuffer.ByteStream.WriteByte( instagib );
		g_MasterServerBuffer.ByteStream.WriteByte( buckshot );
	}

	if ( ulBits & SQF_GAMENAME )
		g_MasterServerBuffer.ByteStream.WriteString( SERVER_MASTER_GetGameName( ));

	if ( ulBits & SQF_IWAD )
		g_MasterServerBuffer.ByteStream.WriteString( NETWORK_GetIWAD( ));

	if ( ulBits & SQF_FORCEPASSWORD )
		g_MasterServerBuffer.ByteStream.WriteByte( sv_forcepassword );

	if ( ulBits & SQF_FORCEJOINPASSWORD )
		g_MasterServerBuffer.ByteStream.WriteByte( sv_forcejoinpassword );

	if ( ulBits & SQF_GAMESKILL )
		g_MasterServerBuffer.ByteStream.WriteByte( gameskill );

	if ( ulBits & SQF_BOTSKILL )
		g_MasterServerBuffer.ByteStream.WriteByte( botskill );

	if ( ulBits & SQF_DMFLAGS )
	{
		g_MasterServerBuffer.ByteStream.WriteLong( dmflags );
		g_MasterServerBuffer.ByteStream.WriteLong( dmflags2 );
		g_MasterServerBuffer.ByteStream.WriteLong( compatflags );
	}

	if ( ulBits & SQF_LIMITS )
	{
		g_MasterServerBuffer.ByteStream.WriteShort( fraglimit );
		g_MasterServerBuffer.ByteStream.WriteShort( static_cast<SHORT>(timelimit) );
		// [BB] We have to base the decision on whether to send "time left" on the same rounded
		// timelimit value we just sent to the client.
		if ( static_cast<SHORT>(timelimit) )
		{
			LONG	lTimeLeft;

			lTimeLeft = (LONG)( timelimit - ( level.time / ( TICRATE * 60 )));
			if ( lTimeLeft < 0 )
				lTimeLeft = 0;
			g_MasterServerBuffer.ByteStream.WriteShort( lTimeLeft );
		}
		g_MasterServerBuffer.ByteStream.WriteShort( duellimit );
		g_MasterServerBuffer.ByteStream.WriteShort( pointlimit );
		g_MasterServerBuffer.ByteStream.WriteShort( winlimit );
	}

	// Send the team damage scale.
	if ( teamplay || teamgame || teamlms || teampossession || (( deathmatch == false ) && ( teamgame == false )))
	{
		if ( ulBits & SQF_TEAMDAMAGE )
			g_MasterServerBuffer.ByteStream.WriteFloat( teamdamage );
	}

	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS )
	{
		// [CW] This command is now deprecated as there are now more than two teams.
		// Send the team scores.
		if ( ulBits & SQF_TEAMSCORES )
		{
			for ( ulIdx = 0; ulIdx < 2; ulIdx++ )
			{
				if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
					g_MasterServerBuffer.ByteStream.WriteShort( TEAM_GetFragCount( ulIdx ));
				else if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
					g_MasterServerBuffer.ByteStream.WriteShort( TEAM_GetWinCount( ulIdx ));
				else
					g_MasterServerBuffer.ByteStream.WriteShort( TEAM_GetPointCount( ulIdx ));
			}
		}
	}

	if ( ulBits & SQF_NUMPLAYERS )
		g_MasterServerBuffer.ByteStream.WriteByte( SERVER_CountPlayers( true ));

	if ( ulBits & SQF_PLAYERDATA )
	{
		for ( ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
		{
			if ( playeringame[ulIdx] == false )
				continue;

			g_MasterServerBuffer.ByteStream.WriteString( players[ulIdx].userinfo.GetName() );
			if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNPOINTS )
				g_MasterServerBuffer.ByteStream.WriteShort( players[ulIdx].lPointCount );
			else if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
				g_MasterServerBuffer.ByteStream.WriteShort( players[ulIdx].ulWins );
			else if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
				g_MasterServerBuffer.ByteStream.WriteShort( players[ulIdx].fragcount );
			else
				g_MasterServerBuffer.ByteStream.WriteShort( players[ulIdx].killcount );

			g_MasterServerBuffer.ByteStream.WriteShort( players[ulIdx].ulPing );
			g_MasterServerBuffer.ByteStream.WriteByte( PLAYER_IsTrueSpectator( &players[ulIdx] ));
			g_MasterServerBuffer.ByteStream.WriteByte( players[ulIdx].bIsBot );

			if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS )
			{
				if ( players[ulIdx].bOnTeam == false )
					g_MasterServerBuffer.ByteStream.WriteByte( 255 );
				else
					g_MasterServerBuffer.ByteStream.WriteByte( players[ulIdx].Team );
			}

			g_MasterServerBuffer.ByteStream.WriteByte( players[ulIdx].ulTime / ( TICRATE * 60 ));
		}
	}

	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS )
	{
		if ( ulBits & SQF_TEAMINFO_NUMBER )
			g_MasterServerBuffer.ByteStream.WriteByte( TEAM_GetNumAvailableTeams( ));

		if ( ulBits & SQF_TEAMINFO_NAME )
			for ( ulIdx = 0; ulIdx < TEAM_GetNumAvailableTeams( ); ulIdx++ )
				g_MasterServerBuffer.ByteStream.WriteString( TEAM_GetName( ulIdx ));

		if ( ulBits & SQF_TEAMINFO_COLOR )
			for ( ulIdx = 0; ulIdx < TEAM_GetNumAvailableTeams( ); ulIdx++ )
				g_MasterServerBuffer.ByteStream.WriteLong( TEAM_GetColor( ulIdx ));

		if ( ulBits & SQF_TEAMINFO_SCORE )
		{
			for ( ulIdx = 0; ulIdx < TEAM_GetNumAvailableTeams( ); ulIdx++ )
			{
				if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
					g_MasterServerBuffer.ByteStream.WriteShort( TEAM_GetFragCount( ulIdx ));
				else if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
					g_MasterServerBuffer.ByteStream.WriteShort( TEAM_GetWinCount( ulIdx ));
				else
					g_MasterServerBuffer.ByteStream.WriteShort( TEAM_GetPointCount( ulIdx ));
			}
		}
	}

	// [BB] Testing server and what's the binary name?
	if ( ulBits & SQF_TESTING_SERVER )
	{
#if ( BUILD_ID == BUILD_RELEASE )
		g_MasterServerBuffer.ByteStream.WriteByte( 0 );
		g_MasterServerBuffer.ByteStream.WriteString( "" );
#else
		g_MasterServerBuffer.ByteStream.WriteByte( 1 );
		// [BB] Name of the testing binary archive found in http://zandronum.com/
		FString testingBinary;
		testingBinary.Format ( "downloads/testing/%s/ZandroDev%s-%swindows.zip", GAMEVER_STRING, GAMEVER_STRING, GetGitTime() );
		g_MasterServerBuffer.ByteStream.WriteString( testingBinary.GetChars() );
#endif
	}

	// [BB] We don't have a mandatory main data file anymore, so just send an empty string.
	if ( ulBits & SQF_DATA_MD5SUM )
		g_MasterServerBuffer.ByteStream.WriteString( "" );

	// [BB] Send all dmflags and compatflags.
	if ( ulBits & SQF_ALL_DMFLAGS )
	{
		g_MasterServerBuffer.ByteStream.WriteByte( 6 );
		g_MasterServerBuffer.ByteStream.WriteLong( dmflags );
		g_MasterServerBuffer.ByteStream.WriteLong( dmflags2 );
		g_MasterServerBuffer.ByteStream.WriteLong( zadmflags );
		g_MasterServerBuffer.ByteStream.WriteLong( compatflags );
		g_MasterServerBuffer.ByteStream.WriteLong( zacompatflags );
		g_MasterServerBuffer.ByteStream.WriteLong( compatflags2 );
	}

	// [BB] Send special security settings like sv_enforcemasterbanlist.
	if ( ulBits & SQF_SECURITY_SETTINGS )
		g_MasterServerBuffer.ByteStream.WriteByte( sv_enforcemasterbanlist );

	// [TP] Send optional wad indices.
	if ( ulBits & SQF_OPTIONAL_WADS )
	{
		g_MasterServerBuffer.ByteStream.WriteByte( g_OptionalWadIndices.Size() );

		for ( unsigned i = 0; i < g_OptionalWadIndices.Size(); ++i )
			g_MasterServerBuffer.ByteStream.WriteByte( g_OptionalWadIndices[i] );
	}

	// [TP] Send deh patches
	if ( ulBits & SQF_DEH )
	{
		const TArray<FString>& names = D_GetDehFileNames();
		g_MasterServerBuffer.ByteStream.WriteByte( names.Size() );

		for ( unsigned i = 0; i < names.Size(); ++i )
			g_MasterServerBuffer.ByteStream.WriteString( names[i] );
	}

	// [SB] handle extended flags
	if ( ulBits & SQF_EXTENDED_INFO ) 
	{
		ulBits2 = ulFlags2;
		ulBits2 &= SQF2_ALL;

		g_MasterServerBuffer.ByteStream.WriteLong( ulBits2 );

		// [SB] send MD5 hashes of PWADs
		if ( ulBits2 & SQF2_PWAD_HASHES )
		{
			g_MasterServerBuffer.ByteStream.WriteByte( NETWORK_GetPWADList().Size( ) );

			for ( unsigned i = 0; i < NETWORK_GetPWADList().Size(); ++i )
				g_MasterServerBuffer.ByteStream.WriteString( NETWORK_GetPWADList()[i].checksum );
		}

		// [SB] send the server's country code
		if ( ulBits2 & SQF2_COUNTRY )
		{
			// [SB] The value of this field will always be 3 characters, so we can just use and
			// send a char[3]
			const size_t codeSize = 3;
			char code[codeSize];

			FString countryCode = sv_country.GetGenericRep( CVAR_String ).String;
			countryCode.ToUpper();

			// [SB] ISO 3166-1 alpha-3 codes in the range XAA-XZZ will never be allocated to actual
			// countries. Therefore, we use these for our special codes:
			//     XIP  -  launcher should try and use IP geolocation
			//     XUN  -  launcher should display a generic unknown flag

			if ( countryCode.CompareNoCase( "automatic" ) == 0 )
			{
				memcpy( code, "XIP", codeSize );
			}
			// [SB] We assume any 3 character long value is a valid country code, and leave it up
			// the launcher to check if they have a flag for it
			else if ( countryCode.Len() == 3 )
			{
				memcpy( code, countryCode.GetChars(), codeSize );
			}
			// [SB] Any other value results in the "unknown" value
			else
			{
				memcpy( code, "XUN", codeSize );
			}

			g_MasterServerBuffer.ByteStream.WriteBuffer( code, codeSize );
		}
	}

//	NETWORK_LaunchPacket( &g_MasterServerBuffer, Address, true );
	NETWORK_LaunchPacket( &g_MasterServerBuffer, Address );
}

//*****************************************************************************
//
const char *SERVER_MASTER_GetGameName( void )
{	
	switch ( gameinfo.gametype )
	{
	case GAME_Doom:

		if ( !(gameinfo.flags & GI_MAPxx) )
			return ( "DOOM" );
		else
			return ( "DOOM II" );
		break;
	case GAME_Heretic:

		return ( "Heretic" );
		break;
	case GAME_Hexen:

		return ( "Hexen" );
		break;
	default:
		
		return ( "ERROR!" );
		break;
	}
}

//*****************************************************************************
//
NETADDRESS_s SERVER_MASTER_GetMasterAddress( void )
{
	return g_AddressMasterServer;
}

//*****************************************************************************
//
void SERVER_MASTER_HandleVerificationRequest( BYTESTREAM_s *pByteStream  )
{
	LONG lVerificationNumber = pByteStream->ReadLong();

	g_MasterServerBuffer.Clear();
	g_MasterServerBuffer.ByteStream.WriteLong( SERVER_MASTER_VERIFICATION );
	g_MasterServerBuffer.ByteStream.WriteString( SERVER_GetMasterBanlistVerificationString().GetChars() );
	g_MasterServerBuffer.ByteStream.WriteLong( lVerificationNumber );

	// [BB] Send the master server our packet.
	NETWORK_LaunchPacket( &g_MasterServerBuffer, SERVER_MASTER_GetMasterAddress () );
}

//*****************************************************************************
//
void SERVER_MASTER_SendBanlistReceipt ( void )
{
	g_MasterServerBuffer.Clear();
	g_MasterServerBuffer.ByteStream.WriteLong( SERVER_MASTER_BANLIST_RECEIPT );
	g_MasterServerBuffer.ByteStream.WriteString( SERVER_GetMasterBanlistVerificationString().GetChars() );

	// [BB] Send the master server our packet.
	NETWORK_LaunchPacket( &g_MasterServerBuffer, SERVER_MASTER_GetMasterAddress () );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
//-- CONSOLE ---------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------

// [BB] Unless all the declarations of the SERVERCONSOLE_* functions are moved to a platfrom independent header,
// we need to keep those awful declarations everywhere.
void SERVERCONSOLE_UpdateBroadcasting( void );
void SERVERCONSOLE_UpdateTitleString( const char *pszString );
// Should the server inform the master server of its existence?
CUSTOM_CVAR( Bool, sv_updatemaster, true, CVAR_SERVERINFO|CVAR_NOSETBYACS )
{
	SERVERCONSOLE_UpdateBroadcasting( );
}

// Should the server broadcast so LAN clients can hear it?
CUSTOM_CVAR( Bool, sv_broadcast, true, CVAR_ARCHIVE|CVAR_NOSETBYACS )
{
	SERVERCONSOLE_UpdateBroadcasting( );
}

// Name of this server on launchers.
CUSTOM_CVAR( String, sv_hostname, "Unnamed " GAMENAME " server", CVAR_ARCHIVE|CVAR_NOSETBYACS|CVAR_SERVERINFO )
{
	FString tempHostname = self.GetGenericRep( CVAR_String ).String;
	FString cleanedHostname;

	// [AK] Uncolorize the string, just in case, before we clean it up.
	V_UnColorizeString( tempHostname );

	// [AK] Remove any unacceptable characters from the string.
	for ( unsigned int i = 0; i < tempHostname.Len( ); i++ )
	{
		// [AK] Don't accept undisplayable system ASCII.
		if ( tempHostname[i] <= 31 )
			continue;

		// [AK] Don't accept escape codes unless they're used before color codes (e.g. '\c').
		if (( tempHostname[i] == 92 ) && (( i >= tempHostname.Len( ) - 1 ) || ( tempHostname[i+1] != 'c' )))
			continue;

		cleanedHostname += tempHostname[i];
	}

	// [AK] Truncate incredibly long hostnames. Whatever limit that we allow should be more than enough.
	cleanedHostname.Truncate( MAX_HOSTNAME_LENGTH );

	// [AK] Finally, remove any trailing crap from the cleaned hostname string.
	V_RemoveTrailingCrapFromFString( cleanedHostname );

	// [AK] If the string is empty, then there was only crap. Reset sv_hostname back to default.
	// Likewise, if the string is different from the original, set sv_hostname to the cleaned string.
	if ( cleanedHostname.IsEmpty( ))
	{
		self.ResetToDefault( );
		return;
	}
	else if ( tempHostname.Compare( cleanedHostname ) != 0 )
	{
		self = cleanedHostname;
		return;
	}

	SERVERCONSOLE_UpdateTitleString( (const char *)self );

	// [AK] Notify the clients about the new hostname.
	if ( NETWORK_GetState( ) == NETSTATE_SERVER )
		SERVERCOMMANDS_SetCVar( self );
}

// Website that has the wad this server is using, possibly with other info.
CVAR( String, sv_website, "", CVAR_ARCHIVE|CVAR_NOSETBYACS|CVAR_SERVERINFO )

// E-mail address of the person running this server.
CVAR( String, sv_hostemail, "", CVAR_ARCHIVE|CVAR_NOSETBYACS|CVAR_SERVERINFO )

// [SB] The country in which this server is located.
CVAR( String, sv_country, "automatic", CVAR_ARCHIVE|CVAR_NOSETBYACS|CVAR_SERVERINFO )

// IP address of the master server.
// [BB] Client and server use this now, therefore the name doesn't begin with "sv_"
CVAR( String, masterhostname, "master.zandronum.com", CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOSETBYACS )

CCMD( wads )
{
	Printf( "IWAD: %s\n", NETWORK_GetIWAD( ) );
	Printf( "Num PWADs: %d\n", NETWORK_GetPWADList().Size() );

	for ( unsigned int i = 0; i < NETWORK_GetPWADList().Size(); ++i )
	{
		const NetworkPWAD& pwad = NETWORK_GetPWADList()[i];
		Printf( "PWAD: %s - %s%s\n", pwad.name.GetChars(), pwad.checksum.GetChars(),
			( Wads.IsWadOptional( pwad.wadnum ) ? " (optional)" : "" ));
	}
}

//-----------------------------------------------------------------------------
//
// Skulltag Source
// Copyright (C) 2002 Brad Carney
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
//
//
// Filename: scoreboard.cpp
//
// Description: Contains scoreboard routines and globals
//
//-----------------------------------------------------------------------------

#include "a_pickups.h"
#include "c_dispatch.h"
#include "callvote.h"
#include "chat.h"
#include "cl_demo.h"
#include "cooperative.h"
#include "deathmatch.h"
#include "duel.h"
#include "doomtype.h"
#include "d_player.h"
#include "gamemode.h"
#include "gi.h"
#include "invasion.h"
#include "joinqueue.h"
#include "lastmanstanding.h"
#include "network.h"
#include "sbar.h"
#include "scoreboard.h"
#include "st_stuff.h"
#include "team.h"
#include "templates.h"
#include "v_text.h"
#include "v_video.h"
#include "w_wad.h"
#include "c_bind.h"	// [RC] To tell user what key to press to vote.
#include "st_hud.h"
#include "wi_stuff.h"
#include "c_console.h"
#include "g_game.h"

//*****************************************************************************
//	VARIABLES

// Player list according to rank.
static	int		g_iSortedPlayers[MAXPLAYERS];

// [AK] The level we are entering, to be shown on the intermission screen.
static	level_info_t *g_pNextLevel;

// Current position of our "pen".
static	ULONG		g_ulCurYPos;

// How many columns are we using in our scoreboard display?
static	ULONG		g_ulNumColumnsUsed = 0;

// Array that has the type of each column.
static	ULONG		g_aulColumnType[MAX_COLUMNS];

// X position of each column.
static	ULONG		g_aulColumnX[MAX_COLUMNS];

// What font are the column headers using?
static	FFont		*g_pColumnHeaderFont = NULL;

// [AK] Do we need to update the scoreboard before we draw it on the screen?
static	bool		g_bRefreshBeforeRendering = false;

// This is the header for each column type.
static	const char	*g_pszColumnHeaders[NUM_COLUMN_TYPES] =
{
	"",
	"NAME",
	"TIME",
	"PING",
	"FRAGS",
	"POINTS",
	"WINS",
	"KILLS",
	"DEATHS",
	"ASSISTS",
	"SECRETS",
};

//*****************************************************************************
//	PROTOTYPES

static	void			scoreboard_SortPlayers( ULONG ulSortType );
static	int	STACK_ARGS	scoreboard_FragCompareFunc( const void *arg1, const void *arg2 );
static	int	STACK_ARGS	scoreboard_PointsCompareFunc( const void *arg1, const void *arg2 );
static	int	STACK_ARGS	scoreboard_KillsCompareFunc( const void *arg1, const void *arg2 );
static	int	STACK_ARGS	scoreboard_WinsCompareFunc( const void *arg1, const void *arg2 );
static	void			scoreboard_RenderIndividualPlayer( ULONG ulDisplayPlayer, ULONG ulPlayer );
static	void			scoreboard_DrawHeader( ULONG ulPlayer );
static	void			scoreboard_ClearColumns( void );
static	void			scoreboard_Prepare5ColumnDisplay( void );
static	void			scoreboard_Prepare4ColumnDisplay( void );
static	void			scoreboard_Prepare3ColumnDisplay( void );
static	void			scoreboard_DoRankingListPass( ULONG ulPlayer, LONG lSpectators, LONG lDead, LONG lNotPlaying, LONG lNoTeam, LONG lWrongTeam, ULONG ulDesiredTeam );
static	void			scoreboard_DrawRankings( ULONG ulPlayer );
static	void			scoreboard_DrawText( const char *pszString, EColorRange Color, ULONG &ulXPos, ULONG ulOffset, bool bOffsetRight = false );
static	void			scoreboard_DrawIcon( const char *pszPatchName, ULONG &ulXPos, ULONG ulYPos, ULONG ulOffset, bool bOffsetRight = false );

//*****************************************************************************
//	CONSOLE VARIABLES

// [JS] Display the amount of time left on the intermission screen.
CVAR( Bool, cl_intermissiontimer, false, CVAR_ARCHIVE );

//*****************************************************************************
//	FUNCTIONS

//*****************************************************************************
// Checks if the user wants to see the scoreboard and is allowed to.
//
bool SCOREBOARD_ShouldDrawBoard( void )
{
	// [AK] If the user isn't pressing their scoreboard key then return false.
	if ( Button_ShowScores.bDown == false )
		return false;

	// [AK] We generally don't want to draw the scoreboard in singleplayer games unless we're
	// watching a demo. However, we still want to draw it in deathmatch, teamgame, or invasion.
	if (( NETWORK_GetState( ) == NETSTATE_SINGLE ) && ( CLIENTDEMO_IsPlaying( ) == false ) && (( deathmatch || teamgame || invasion ) == false ))
		return false;

	return true;
}

//*****************************************************************************
//
void SCOREBOARD_Render( ULONG ulDisplayPlayer )
{
	// Make sure the display player is valid.
	if ( ulDisplayPlayer >= MAXPLAYERS )
		return;

	// [AK] If we need to update the scoreboard, do so before rendering it.
	if ( g_bRefreshBeforeRendering )
	{
		SCOREBOARD_Refresh( );
		g_bRefreshBeforeRendering = false;
	}

	// [AK] Draw the scoreboard header at the top.
	scoreboard_DrawHeader( ulDisplayPlayer );

	// Draw the headers, list, entries, everything.
	scoreboard_DrawRankings( ulDisplayPlayer );
}

//*****************************************************************************
//
void SCOREBOARD_Refresh( void )
{
	// First, determine how many columns we can use, based on our screen resolution.
	ULONG ulNumIdealColumns = 3;

	if ( HUD_GetWidth( ) >= 600 )
		ulNumIdealColumns = 5;
	else if ( HUD_GetWidth( ) >= 480 )
		ulNumIdealColumns = 4;

	// The 5 column display is only availible for modes that support it.
	if (( ulNumIdealColumns == 5 ) && !( GAMEMODE_GetCurrentFlags( ) & ( GMF_PLAYERSEARNPOINTS | GMF_PLAYERSEARNWINS )))
		ulNumIdealColumns = 4;

	if ( ulNumIdealColumns == 5 )
		scoreboard_Prepare5ColumnDisplay( );
	else if ( ulNumIdealColumns == 4 )
		scoreboard_Prepare4ColumnDisplay( );
	else
		scoreboard_Prepare3ColumnDisplay( );
}

//*****************************************************************************
//
void SCOREBOARD_ShouldRefreshBeforeRendering( void )
{
	if ( NETWORK_GetState( ) == NETSTATE_SERVER )
		return;

	g_bRefreshBeforeRendering = true;
}

//*****************************************************************************
//
LONG SCOREBOARD_GetLeftToLimit( void )
{
	ULONG	ulIdx;

	// If we're not in a level, then clearly there's no need for this.
	if ( gamestate != GS_LEVEL )
		return ( 0 );

	// KILL-based mode. [BB] This works indepently of any players in game.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNKILLS )
	{
		if ( invasion )
			return (LONG) INVASION_GetNumMonstersLeft( );
		else if ( dmflags2 & DF2_KILL_MONSTERS )
		{
			if ( level.total_monsters > 0 )
				return ( 100 * ( level.total_monsters - level.killed_monsters ) / level.total_monsters );
			else
				return 0;
		}
		else
			return ( level.total_monsters - level.killed_monsters );
	}

	// [BB] In a team game with only empty teams or if there are no players at all, just return the appropriate limit.
	if ( ( ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS )
	     && ( TEAM_TeamsWithPlayersOn() == 0 ) )
		 || ( SERVER_CalcNumNonSpectatingPlayers( MAXPLAYERS ) == 0 ) )
	{
		if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
			return winlimit;
		else if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNPOINTS )
			return pointlimit;
		else if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
			return fraglimit;
		else
			return 0;
	}

	// FRAG-based mode.
	if ( fraglimit && GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
	{
		LONG	lHighestFragcount;
				
		if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS )
			lHighestFragcount = TEAM_GetHighestFragCount( );		
		else
		{
			lHighestFragcount = INT_MIN;
			for ( ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
			{
				if ( playeringame[ulIdx] && !players[ulIdx].bSpectating && players[ulIdx].fragcount > lHighestFragcount )
					lHighestFragcount = players[ulIdx].fragcount;
			}
		}

		return ( fraglimit - lHighestFragcount );
	}

	// POINT-based mode.
	else if ( pointlimit && GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNPOINTS )
	{
		if ( teamgame || teampossession )
			return ( pointlimit - TEAM_GetHighestPointCount( ));
		else // Must be possession mode.
		{
			LONG lHighestPointCount = INT_MIN;
			for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
			{
				if ( playeringame[ulIdx] && !players[ulIdx].bSpectating && players[ulIdx].lPointCount > lHighestPointCount )
					lHighestPointCount = players[ulIdx].lPointCount;
			}

			return pointlimit - (ULONG) lHighestPointCount;
		}
	}

	// WIN-based mode (LMS).
	else if ( winlimit && GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
	{
		bool	bFoundPlayer = false;
		LONG	lHighestWincount = 0;

		if ( teamlms )
			lHighestWincount = TEAM_GetHighestWinCount( );
		else
		{
			for ( ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
			{
				if ( playeringame[ulIdx] == false || players[ulIdx].bSpectating )
					continue;

				if ( bFoundPlayer == false )
				{
					lHighestWincount = players[ulIdx].ulWins;
					bFoundPlayer = true;
					continue;
				}
				else if ( players[ulIdx].ulWins > (ULONG)lHighestWincount )
					lHighestWincount = players[ulIdx].ulWins;
			}
		}

		return ( winlimit - lHighestWincount );
	}

	// None of the above.
	return ( -1 );
}

//*****************************************************************************
//*****************************************************************************
//
static void scoreboard_SortPlayers( ULONG ulSortType )
{
	ULONG	ulIdx;

	for ( ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
		g_iSortedPlayers[ulIdx] = ulIdx;

	if ( ulSortType == ST_FRAGCOUNT )
		qsort( g_iSortedPlayers, MAXPLAYERS, sizeof( int ), scoreboard_FragCompareFunc );
	else if ( ulSortType == ST_POINTCOUNT )
		qsort( g_iSortedPlayers, MAXPLAYERS, sizeof( int ), scoreboard_PointsCompareFunc );
	else if ( ulSortType == ST_WINCOUNT )
		qsort( g_iSortedPlayers, MAXPLAYERS, sizeof( int ), scoreboard_WinsCompareFunc );
	else
		qsort( g_iSortedPlayers, MAXPLAYERS, sizeof( int ), scoreboard_KillsCompareFunc );
}

//*****************************************************************************
//
static int STACK_ARGS scoreboard_FragCompareFunc( const void *arg1, const void *arg2 )
{
	return ( players[*(int *)arg2].fragcount - players[*(int *)arg1].fragcount );
}

//*****************************************************************************
//
static int STACK_ARGS scoreboard_PointsCompareFunc( const void *arg1, const void *arg2 )
{
	return ( players[*(int *)arg2].lPointCount - players[*(int *)arg1].lPointCount );
}

//*****************************************************************************
//
static int STACK_ARGS scoreboard_KillsCompareFunc( const void *arg1, const void *arg2 )
{
	return ( players[*(int *)arg2].killcount - players[*(int *)arg1].killcount );
}

//*****************************************************************************
//
static int STACK_ARGS scoreboard_WinsCompareFunc( const void *arg1, const void *arg2 )
{
	return ( players[*(int *)arg2].ulWins - players[*(int *)arg1].ulWins );
}

//*****************************************************************************
//
static void scoreboard_DrawText( const char *pszString, EColorRange Color, ULONG &ulXPos, ULONG ulOffset, bool bOffsetRight )
{
	ulXPos += SmallFont->StringWidth( pszString ) * ( bOffsetRight ? 1 : -1 );
	HUD_DrawText( SmallFont, Color, ulXPos, g_ulCurYPos, pszString, g_bScale );
	ulXPos += ulOffset * ( bOffsetRight ? 1 : -1 );
}

//*****************************************************************************
//
static void scoreboard_DrawIcon( const char *pszPatchName, ULONG &ulXPos, ULONG ulYPos, ULONG ulOffset, bool bOffsetRight )
{
	ulXPos += TexMan[pszPatchName]->GetWidth( ) * ( bOffsetRight ? 1 : -1 );
	ulYPos -= (( TexMan[pszPatchName]->GetHeight( ) - SmallFont->GetHeight( )) >> 1 );

	HUD_DrawTexture( TexMan[pszPatchName], ulXPos, ulYPos, g_bScale );
	ulXPos += ulOffset * ( bOffsetRight ? 1 : -1 );
}

//*****************************************************************************
//
static void scoreboard_RenderIndividualPlayer( ULONG ulDisplayPlayer, ULONG ulPlayer )
{
	ULONG ulColor = CR_GRAY;
	FString patchName;
	FString text;

	// [AK] Change the text color if we're carrying a terminator sphere or on a team.
	if (( terminator ) && ( players[ulPlayer].cheats2 & CF2_TERMINATORARTIFACT ))
		ulColor = CR_RED;
	else if ( players[ulPlayer].bOnTeam )
		ulColor = TEAM_GetTextColor( players[ulPlayer].Team );
	else if ( ulDisplayPlayer == ulPlayer )
		ulColor = demoplayback ? CR_GOLD : CR_GREEN;

	// Draw the data for each column.
	for ( ULONG ulColumn = 0; ulColumn < g_ulNumColumnsUsed; ulColumn++ )
	{
		// [AK] Determine the x-position of the text for this column.
		ULONG ulXPos = static_cast<ULONG>( g_aulColumnX[ulColumn] * g_fXScale );

		// [AK] We need to display icons and some extra text in the name column.
		if ( g_aulColumnType[ulColumn] == COLUMN_NAME )
		{
			// Track where we are to draw multiple icons.
			ULONG ulXPosOffset = ulXPos - SmallFont->StringWidth( "  " );

			// [TP] If this player is in the join queue, display the position.
			int position = JOINQUEUE_GetPositionInLine( ulPlayer );
			if ( position != -1 )
			{
				text.Format( "%d.", position + 1 );
				scoreboard_DrawText( text, position == 0 ? CR_RED : CR_GOLD, ulXPosOffset, 4 );
			}

			// Draw the user's handicap, if any.
			int handicap = players[ulPlayer].userinfo.GetHandicap( );
			if ( handicap > 0 )
			{
				if (( lastmanstanding ) || ( teamlms ))
					text.Format( "(%d)", deh.MaxSoulsphere - handicap < 1 ? 1 : deh.MaxArmor - handicap );
				else
					text.Format( "(%d)", deh.StartHealth - handicap < 1 ? 1 : deh.StartHealth - handicap );

				scoreboard_DrawText( text, static_cast<EColorRange>( ulColor ), ulXPosOffset, 4 );
			}

			// Draw an icon if this player is a ready to go on.
			if ( players[ulPlayer].bReadyToGoOn )
				scoreboard_DrawIcon( "RDYTOGO", ulXPosOffset, g_ulCurYPos, 4 );

			// Draw a bot icon if this player is a bot.
			if ( players[ulPlayer].bIsBot )
			{
				patchName.Format( "BOTSKIL%d", botskill.GetGenericRep( CVAR_Int ).Int );
				scoreboard_DrawIcon( patchName, ulXPosOffset, g_ulCurYPos, 4 );
			}

			// Draw a chat icon if this player is chatting.
			// [Cata] Also shows who's in the console.
			// [AK] Also show who's in the menu.
			if (( players[ulPlayer].bChatting ) || ( players[ulPlayer].bInConsole ) || ( players[ulPlayer].bInMenu ))
			{
				if ( players[ulPlayer].bChatting )
					patchName = "TLKMINI";
				else if ( players[ulPlayer].bInConsole )
					patchName = "CONSMINI";
				else
					patchName = "MENUMINI";

				scoreboard_DrawIcon( patchName, ulXPosOffset, g_ulCurYPos, 4 );
			}

			// [AK] Also show an icon if the player is lagging to the server.
			if (( players[ulPlayer].bLagging ) && ( gamestate == GS_LEVEL ))
				scoreboard_DrawIcon( "LAGMINI", ulXPosOffset, g_ulCurYPos, 4 );

			// Draw text if there's a vote on and this player voted.
			if ( CALLVOTE_GetVoteState( ) == VOTESTATE_INVOTE )
			{
				ULONG ulVoteChoice = CALLVOTE_GetPlayerVoteChoice( ulPlayer );

				// [AK] Check if this player either voted yes or no.
				if ( ulVoteChoice != VOTE_UNDECIDED )
				{
					text.Format( "(%s)", ulVoteChoice == VOTE_YES ? "Yes" : "No" );
					scoreboard_DrawText( text, CALLVOTE_GetVoteCaller( ) == ulPlayer ? CR_RED : CR_GOLD, ulXPosOffset, 4 );
				}
			}

			text = players[ulPlayer].userinfo.GetName( );
		}
		else if ( g_aulColumnType[ulColumn] == COLUMN_TIME )
		{
			text.Format( "%d", static_cast<unsigned int>( players[ulPlayer].ulTime / ( TICRATE * 60 )));
		}
		else if ( g_aulColumnType[ulColumn] == COLUMN_PING )
		{
			text.Format( "%d", static_cast<unsigned int>( players[ulPlayer].ulPing ));
		}
		else if ( g_aulColumnType[ulColumn] == COLUMN_DEATHS )
		{
			text.Format( "%d", static_cast<unsigned int>( players[ulPlayer].ulDeathCount ));
		}
		else
		{
			switch ( g_aulColumnType[ulColumn] )
			{
				case COLUMN_FRAGS:
					text.Format( "%d", players[ulPlayer].fragcount );
					break;

				case COLUMN_POINTS:
					text.Format( "%d", static_cast<int>( players[ulPlayer].lPointCount ));
					break;

				case COLUMN_ASSISTS:
					text.Format( "%d", static_cast<unsigned int>( players[ulPlayer].ulMedalCount[14] ));
					break;

				case COLUMN_WINS:
					text.Format( "%d", static_cast<unsigned int>( players[ulPlayer].ulWins ));
					break;

				case COLUMN_KILLS:
					text.Format( "%d", players[ulPlayer].killcount );
					break;

				case COLUMN_SECRETS:
					text.Format( "%d", players[ulPlayer].secretcount );
					break;
			}

			// If the player isn't really playing, change this.
			if ( PLAYER_IsTrueSpectator( &players[ulPlayer] ))
				text = "Spect";
			else if (( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS ) && ( players[ulPlayer].bOnTeam == false ))
				text = "No Team";
			else if (( GAMEMODE_GetCurrentFlags( ) & GMF_DEADSPECTATORS ) && (( players[ulPlayer].health <= 0 ) || ( players[ulPlayer].bDeadSpectator )) && ( gamestate != GS_INTERMISSION ))
				text = "Dead";
		}

		HUD_DrawText( SmallFont, ulColor, ulXPos, g_ulCurYPos, text, g_bScale );
	}
}

//*****************************************************************************
//
void SCOREBOARD_SetNextLevel( const char *pszMapName )
{
	g_pNextLevel = ( pszMapName != NULL ) ? FindLevelInfo( pszMapName ) : NULL;
}

//*****************************************************************************
//
static void scoreboard_DrawHeader( ULONG ulPlayer )
{
	g_ulCurYPos = 4;

	// Draw the "RANKINGS" text at the top. Don't draw it if we're in the intermission.
	if ( gamestate == GS_LEVEL )
		HUD_DrawTextCentered( BigFont, CR_RED, g_ulCurYPos, "RANKINGS", g_bScale );

	g_ulCurYPos += BigFont->GetHeight( ) + 6;

	if ( gamestate == GS_LEVEL )
	{
		// [AK] Draw the name of the server if we're in an online game.
		if ( NETWORK_InClientMode( ))
		{
			FString hostName = sv_hostname.GetGenericRep( CVAR_String ).String;
			V_ColorizeString( hostName );
			HUD_DrawTextCentered( SmallFont, CR_GREY, g_ulCurYPos, hostName, g_bScale );
			g_ulCurYPos += SmallFont->GetHeight( ) + 1;
		}

		// [AK] Draw the name of the current game mode.
		HUD_DrawTextCentered( SmallFont, CR_GOLD, g_ulCurYPos, GAMEMODE_GetCurrentName( ), g_bScale );
		g_ulCurYPos += SmallFont->GetHeight( ) + 1;

		// Draw the time, frags, points, or kills we have left until the level ends.
		// Generate the limit strings.
		std::list<FString> lines;
		SCOREBOARD_BuildLimitStrings( lines, true );

		// Now, draw them.
		for ( std::list<FString>::iterator i = lines.begin( ); i != lines.end( ); i++ )
		{
			HUD_DrawTextCentered( SmallFont, CR_GREY, g_ulCurYPos, *i, g_bScale );
			g_ulCurYPos += SmallFont->GetHeight( ) + 1;
		}
	}

	// Draw the team scores and their relation (tied, red leads, etc).
	if (( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS ) && (( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNKILLS ) == false ))
	{
		if ( gamestate != GS_LEVEL )
			g_ulCurYPos += SmallFont->GetHeight( ) + 1;

		HUD_DrawTextCentered( SmallFont, CR_GREY, g_ulCurYPos, HUD_BuildPointString( ), g_bScale );
		g_ulCurYPos += SmallFont->GetHeight( ) + 1;
	}
	// Draw my rank and my frags, points, etc. Don't draw it if we're in the intermission.
	else if (( gamestate == GS_LEVEL ) && ( HUD_ShouldDrawRank( ulPlayer )))
	{
		HUD_DrawTextCentered( SmallFont, CR_GREY, g_ulCurYPos, HUD_BuildPlaceString( ulPlayer ), g_bScale );
		g_ulCurYPos += SmallFont->GetHeight( ) + 1;
	}

	// [JS] Intermission countdown display.
	if (( gamestate == GS_INTERMISSION ) && ( NETWORK_GetState( ) == NETSTATE_CLIENT ) && ( cl_intermissiontimer ))
	{
		FString countdownMessage = "Entering ";

		// [AK] Display the name of the level we're entering if possible.
		if ( g_pNextLevel != NULL )
			countdownMessage.AppendFormat( "%s: %s", g_pNextLevel->mapname, g_pNextLevel->LookupLevelName( ).GetChars() );
		else
			countdownMessage += "next map";

		countdownMessage.AppendFormat( " in %d seconds", MAX( static_cast<int>( WI_GetStopWatch( )) / TICRATE + 1, 1 ));
		HUD_DrawTextCentered( SmallFont, CR_GREEN, g_ulCurYPos, countdownMessage, g_bScale );
		g_ulCurYPos += SmallFont->GetHeight( ) + 1;
	}
}

//*****************************************************************************
// [AK] Checks if there's already a limit string on the list, removes it from the list, then
// prepends it to the string we passed into the function.
//
void scoreboard_TryToPrependLimit( std::list<FString> &lines, FString &limit )
{
	// [AK] This shouldn't be done on the server console.
	if (( NETWORK_GetState( ) != NETSTATE_SERVER ) && ( lines.empty( ) == false ))
	{
		FString prevLimitString = lines.back( );
		lines.pop_back( );

		prevLimitString += TEXTCOLOR_DARKGRAY " - " TEXTCOLOR_NORMAL;
		limit.Insert( 0, prevLimitString );
	}
}

//*****************************************************************************
// [RC] Helper method for SCOREBOARD_BuildLimitStrings. Creates a "x things remaining" message.
// [AK] Added the bWantToPrepend parameter.
//
void scoreboard_AddSingleLimit( std::list<FString> &lines, bool condition, int remaining, const char *pszUnitName, bool bWantToPrepend = false )
{
	if ( condition && remaining > 0 )
	{
		FString limitString;
		limitString.Format( "%d %s%s left", static_cast<int>( remaining ), pszUnitName, remaining == 1 ? "" : "s" );

		// [AK] Try to make this string appear on the same line as a previous string if we want to.
		if ( bWantToPrepend )
			scoreboard_TryToPrependLimit( lines, limitString );

		lines.push_back( limitString );
	}
}

//*****************************************************************************
// [AK] Creates the time limit message to be shown on the scoreboard or server console.
//
void scoreboard_AddTimeLimit( std::list<FString> &lines )
{
	FString TimeLeftString;
	GAMEMODE_GetTimeLeftString( TimeLeftString );

	// [AK] Also print "round" when there's more than one duel match to be played.
	FString limitString = (( duel && duellimit > 1 ) || ( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNWINS )) ? "Round" : "Level";
	limitString.AppendFormat( " ends in %s", TimeLeftString.GetChars( ));

	// [AK] Try to put the time limit string on the same line as a previous string.
	scoreboard_TryToPrependLimit( lines, limitString );
	lines.push_back( limitString );
}

//*****************************************************************************
//
// [RC] Builds the series of "x frags left / 3rd match between the two / 15:10 remain" strings. Used here and in serverconsole.cpp
//
void SCOREBOARD_BuildLimitStrings( std::list<FString> &lines, bool bAcceptColors )
{
	if ( gamestate != GS_LEVEL )
		return;

	ULONG ulFlags = GAMEMODE_GetCurrentFlags( );
	LONG lRemaining = SCOREBOARD_GetLeftToLimit( );
	const bool bTimeLimitActive = GAMEMODE_IsTimelimitActive( );
	bool bTimeLimitAdded = false;
	FString text;

	// Build the fraglimit string.
	scoreboard_AddSingleLimit( lines, fraglimit && ( ulFlags & GMF_PLAYERSEARNFRAGS ), lRemaining, "frag" );

	// Build the duellimit and "wins" string.
	if ( duel && duellimit )
	{
		ULONG ulWinner = MAXPLAYERS;
		LONG lHighestFrags = LONG_MIN;
		const bool bInResults = GAMEMODE_IsGameInResultSequence( );
		bool bDraw = true;

		// [AK] If there's a fraglimit and a duellimit string, the timelimit string should be put in-between them
		// on the scoreboard to organize the info better (frags left on the left, duels left on the right).
		if (( bTimeLimitActive ) && ( lines.empty( ) == false ) && ( NETWORK_GetState( ) != NETSTATE_SERVER ))
		{
			scoreboard_AddTimeLimit( lines );
			bTimeLimitAdded = true;
		}

		// [TL] The number of duels left is the maximum number of duels less the number of duels fought.
		// [AK] We already confirmed we're using duel limits, so we can now add this string unconditionally.
		scoreboard_AddSingleLimit( lines, true, duellimit - DUEL_GetNumDuels( ), "duel", true );

		// [AK] If we haven't added the timelimit string yet, make it appear next to the duellimit string.
		if (( bTimeLimitActive ) && ( bTimeLimitAdded == false ) && ( NETWORK_GetState( ) != NETSTATE_SERVER ))
		{
			scoreboard_AddTimeLimit( lines );
			bTimeLimitAdded = true;
		}

		for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
		{
			if (( playeringame[ulIdx] ) && ( players[ulIdx].ulWins > 0 ))
			{
				// [AK] In case both duelers have at least one win during the results sequence the,
				// champion should be the one with the higher frag count.
				if ( bInResults )
				{
					if ( players[ulIdx].fragcount > lHighestFrags )
					{
						ulWinner = ulIdx;
						lHighestFrags = players[ulIdx].fragcount;
					}
				}
				else
				{
					ulWinner = ulIdx;
					break;
				}
			}
		}

		if ( ulWinner == MAXPLAYERS )
		{
			if ( GAME_CountActivePlayers( ) == 2 )
				text = "First match between the two";
			else
				bDraw = false;
		}
		else
		{
			text.Format( "Champion is %s", players[ulWinner].userinfo.GetName( ));
			text.AppendFormat( " with %d win%s", static_cast<unsigned int>( players[ulWinner].ulWins ), players[ulWinner].ulWins == 1 ? "" : "s" );
		}

		if ( bDraw )
		{
			if ( !bAcceptColors )
				V_RemoveColorCodes( text );

			lines.push_back( text );
		}
	}

	// Build the pointlimit, winlimit, and/or wavelimit strings.
	scoreboard_AddSingleLimit( lines, pointlimit && ( ulFlags & GMF_PLAYERSEARNPOINTS ), lRemaining, "point" );
	scoreboard_AddSingleLimit( lines, winlimit && ( ulFlags & GMF_PLAYERSEARNWINS ), lRemaining, "win" );
	scoreboard_AddSingleLimit( lines, invasion && wavelimit, wavelimit - INVASION_GetCurrentWave( ), "wave" );

	// [AK] Build the coop strings.
	if ( ulFlags & GMF_COOPERATIVE )
	{
		ULONG ulNumLimits = 0;

		// Render the number of monsters left in coop.
		// [AK] Unless we're playing invasion, only do this when there are actually monsters on the level.
		if (( ulFlags & GMF_PLAYERSEARNKILLS ) && (( invasion ) || ( level.total_monsters > 0 )))
		{
			if (( invasion ) || (( dmflags2 & DF2_KILL_MONSTERS ) == false ))
				text.Format( "%d monster%s left", static_cast<int>( lRemaining ), lRemaining == 1 ? "" : "s" );
			else
				text.Format( "%d%% monsters left", static_cast<int>( lRemaining ));

			// [AK] Render the number of monsters left on the same line as the number of waves left in invasion.
			if ( invasion && wavelimit )			
				scoreboard_TryToPrependLimit( lines, text );

			lines.push_back( text );
			ulNumLimits++;
		}

		// [AK] If there's monsters and secrets on the current level, the timelimit string should be put in-between
		// them on the scoreboard to organize the info better (monsters left on the left, secrets left on the right).
		if (( bTimeLimitActive ) && ( lines.empty( ) == false ) && ( NETWORK_GetState( ) != NETSTATE_SERVER ))
		{
			scoreboard_AddTimeLimit( lines );
			bTimeLimitAdded = true;
			ulNumLimits++;
 		}

		// [AK] Render the number of secrets left.
		if ( level.total_secrets > 0 )
		{
			lRemaining = level.total_secrets - level.found_secrets;
			text.Format( "%d secret%s left", static_cast<int>( lRemaining ), lRemaining == 1 ? "" : "s" );
			scoreboard_TryToPrependLimit( lines, text );
			lines.push_back( text );
			ulNumLimits++;
		}

		// [AK] If we haven't added the timelimit string yet, make it appear next to the "secrets left" string.
		if (( bTimeLimitActive ) && ( bTimeLimitAdded == false ) && ( NETWORK_GetState( ) != NETSTATE_SERVER ))
		{
			scoreboard_AddTimeLimit( lines );
			bTimeLimitAdded = true;
			ulNumLimits++;
 		}

		// [WS] Show the damage factor.
		if ( sv_coop_damagefactor != 1.0f )
		{
			text.Format( "Damage factor is %.2f", static_cast<float>( sv_coop_damagefactor ));

			// [AK] If there aren't too many limits already, try to make the damage factor appear on the same
			// line as a previous string.
			if ( ulNumLimits == 1 )
				scoreboard_TryToPrependLimit( lines, text );

			lines.push_back( text );
		}
	}

	// Render the timelimit string. - [BB] if the gamemode uses it.
	// [AK] Don't add this if we've already done so.
	if (( bTimeLimitActive ) && ( bTimeLimitAdded == false ))
		scoreboard_AddTimeLimit( lines );
}

//*****************************************************************************
//
static void scoreboard_ClearColumns( void )
{
	ULONG	ulIdx;

	for ( ulIdx = 0; ulIdx < MAX_COLUMNS; ulIdx++ )
		g_aulColumnType[ulIdx] = COLUMN_EMPTY;

	g_ulNumColumnsUsed = 0;
}

//*****************************************************************************
//
static void scoreboard_Prepare5ColumnDisplay( void )
{
	// Set all to empty.
	scoreboard_ClearColumns( );

	g_ulNumColumnsUsed = 5;
	g_pColumnHeaderFont = BigFont;

	// Set up the location of each column.
	g_aulColumnX[0] = 8;
	g_aulColumnX[1] = 56;
	g_aulColumnX[2] = 106;
	g_aulColumnX[3] = 222;
	g_aulColumnX[4] = 286;

	// Build columns for modes in which players try to earn points.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNPOINTS )
	{
		g_aulColumnType[0] = COLUMN_POINTS;
		// [BC] Doesn't look like this is being used right now (at least not properly).
/*
		// Can have assists.
		if ( ctf || skulltag )
			g_aulColumnType[0] = COL_POINTSASSISTS;
*/
		g_aulColumnType[1] = COLUMN_FRAGS;
		g_aulColumnType[2] = COLUMN_NAME;
		g_aulColumnType[3] = COLUMN_DEATHS;
		if ( NETWORK_InClientMode() )
			g_aulColumnType[3] = COLUMN_PING;
		g_aulColumnType[4] = COLUMN_TIME;

		// Sort players based on their pointcount.
		scoreboard_SortPlayers( ST_POINTCOUNT );
	}

	// Build columns for modes in which players try to earn wins.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
	{
		g_aulColumnType[0] = COLUMN_WINS;
		g_aulColumnType[1] = COLUMN_FRAGS;
		g_aulColumnType[2] = COLUMN_NAME;
		g_aulColumnType[3] = COLUMN_DEATHS;
		if ( NETWORK_InClientMode() )
			g_aulColumnType[3] = COLUMN_PING;
		g_aulColumnType[4] = COLUMN_TIME;

		// Sort players based on their pointcount.
		scoreboard_SortPlayers( ST_WINCOUNT );
	}
}

//*****************************************************************************
//
static void scoreboard_SetColumnZeroToKillsAndSortPlayers( void )
{
	if ( zadmflags & ZADF_AWARD_DAMAGE_INSTEAD_KILLS )
	{
		g_aulColumnType[0] = COLUMN_POINTS;

		// Sort players based on their points.
		scoreboard_SortPlayers( ST_POINTCOUNT );
	}
	else
	{
		g_aulColumnType[0] = COLUMN_KILLS;

		// Sort players based on their killcount.
		scoreboard_SortPlayers( ST_KILLCOUNT );
	}
}
//*****************************************************************************
//
static void scoreboard_Prepare4ColumnDisplay( void )
{
	// Set all to empty.
	scoreboard_ClearColumns( );

	g_ulNumColumnsUsed = 4;
	g_pColumnHeaderFont = BigFont;

	// Set up the location of each column.
	g_aulColumnX[0] = 24;
	g_aulColumnX[1] = 84;
	g_aulColumnX[2] = 192;
	g_aulColumnX[3] = 256;
	
	// Build columns for modes in which players try to earn kills.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNKILLS )
	{
		scoreboard_SetColumnZeroToKillsAndSortPlayers();
		g_aulColumnType[1] = COLUMN_NAME;
		g_aulColumnType[2] = COLUMN_DEATHS;
		if ( NETWORK_InClientMode() )
			g_aulColumnType[2] = COLUMN_PING;
		g_aulColumnType[3] = COLUMN_TIME;
	}

	// Build columns for modes in which players try to earn frags.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
	{
		g_aulColumnType[0] = COLUMN_FRAGS;
		g_aulColumnType[1] = COLUMN_NAME;
		g_aulColumnType[2] = COLUMN_DEATHS;
		if ( NETWORK_InClientMode() )
			g_aulColumnType[2] = COLUMN_PING;
		g_aulColumnType[3] = COLUMN_TIME;

		// Sort players based on their fragcount.
		scoreboard_SortPlayers( ST_FRAGCOUNT );
	}
	
	// Build columns for modes in which players try to earn points.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNPOINTS )
	{
//		if ( ctf || skulltag ) // Can have assists
//			g_aulColumnType[0] = COL_POINTSASSISTS;

		g_aulColumnType[0] = COLUMN_POINTS;
		g_aulColumnType[1] = COLUMN_NAME;
		g_aulColumnType[2] = COLUMN_DEATHS;
		if ( NETWORK_InClientMode() )
			g_aulColumnType[2] = COLUMN_PING;
		g_aulColumnType[3] = COLUMN_TIME;

		// Sort players based on their pointcount.
		scoreboard_SortPlayers( ST_POINTCOUNT );
	}

	// Build columns for modes in which players try to earn wins.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
	{
		g_aulColumnType[0] = COLUMN_WINS;
		g_aulColumnType[1] = COLUMN_NAME;
		g_aulColumnType[2] = COLUMN_FRAGS;
		if ( NETWORK_InClientMode() )
			g_aulColumnType[2] = COLUMN_PING;
		g_aulColumnType[3] = COLUMN_TIME;

		// Sort players based on their wincount.
		scoreboard_SortPlayers( ST_WINCOUNT );
	}

}

//*****************************************************************************
//
static void scoreboard_Prepare3ColumnDisplay( void )
{
	// Set all to empty.
	scoreboard_ClearColumns( );

	g_ulNumColumnsUsed = 3;
	g_pColumnHeaderFont = SmallFont;

	// Set up the location of each column.
	g_aulColumnX[0] = 16;
	g_aulColumnX[1] = 96;
	g_aulColumnX[2] = 272;

	// All boards share these two columns. However, you can still deviant on these columns if you want.
	g_aulColumnType[1] = COLUMN_NAME;
	g_aulColumnType[2] = COLUMN_TIME;
	if ( NETWORK_InClientMode() )
		g_aulColumnType[2] = COLUMN_PING;

	// Build columns for modes in which players try to earn kills.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNKILLS )
	{
		scoreboard_SetColumnZeroToKillsAndSortPlayers();
	}

	// Build columns for modes in which players try to earn frags.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNFRAGS )
	{
		g_aulColumnType[0] = COLUMN_FRAGS;

		// Sort players based on their fragcount.
		scoreboard_SortPlayers( ST_FRAGCOUNT );
	}
	
	// Build columns for modes in which players try to earn points.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNPOINTS )
	{
//		if ( ctf || skulltag ) // Can have assists
//			g_aulColumnType[0] = COL_POINTSASSISTS;

		g_aulColumnType[0] = COLUMN_POINTS;

		// Sort players based on their pointcount.
		scoreboard_SortPlayers( ST_POINTCOUNT );
	}

	// Build columns for modes in which players try to earn wins.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSEARNWINS )
	{
		g_aulColumnType[0] = COLUMN_WINS;

		// Sort players based on their wincount.
		scoreboard_SortPlayers( ST_WINCOUNT );
	}

}

//*****************************************************************************
//	These parameters are filters.
//	If 1, players with this trait will be skipped.
//	If 2, players *without* this trait will be skipped.
static void scoreboard_DoRankingListPass( ULONG ulPlayer, LONG lSpectators, LONG lDead, LONG lNotPlaying, LONG lNoTeam, LONG lWrongTeam, ULONG ulDesiredTeam )
{
	ULONG	ulIdx;
	ULONG	ulNumPlayers;

	ulNumPlayers = 0;
	for ( ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
	{
		// Skip or require players not in the game.
		if (((lNotPlaying == 1) && (playeringame[g_iSortedPlayers[ulIdx]] == false )) ||
			((lNotPlaying == 2) && (!playeringame[g_iSortedPlayers[ulIdx]] == false )))
			continue;

		// Skip or require players not on a team.
		 if(((lNoTeam == 1) && (!players[g_iSortedPlayers[ulIdx]].bOnTeam)) ||
			 ((lNoTeam == 2) && (players[g_iSortedPlayers[ulIdx]].bOnTeam)))
			continue;

		// Skip or require spectators.
		if (((lSpectators == 1) && PLAYER_IsTrueSpectator( &players[g_iSortedPlayers[ulIdx]])) ||
			((lSpectators == 2) && !PLAYER_IsTrueSpectator( &players[g_iSortedPlayers[ulIdx]])))
			continue;

		// In LMS, skip or require dead players.
		if( gamestate != GS_INTERMISSION ){
			/*(( lastmanstanding ) && (( LASTMANSTANDING_GetState( ) == LMSS_INPROGRESS ) || ( LASTMANSTANDING_GetState( ) == LMSS_WINSEQUENCE ))) ||
			(( survival ) && (( SURVIVAL_GetState( ) == SURVS_INPROGRESS ) || ( SURVIVAL_GetState( ) == SURVS_MISSIONFAILED )))*/
			
			// If we don't want to draw dead players, and this player is dead, skip this player.
			if (( lDead == 1 ) &&
				(( players[g_iSortedPlayers[ulIdx]].health <= 0 ) || ( players[g_iSortedPlayers[ulIdx]].bDeadSpectator )))
			{
				continue;
			}

			// If we don't want to draw living players, and this player is alive, skip this player.
			if (( lDead == 2 ) &&
				( players[g_iSortedPlayers[ulIdx]].health > 0 ) &&
				( players[g_iSortedPlayers[ulIdx]].bDeadSpectator == false ))
			{
				continue;
			}
		}

		// Skip or require players that aren't on this team.
		if (((lWrongTeam == 1) && (players[g_iSortedPlayers[ulIdx]].Team != ulDesiredTeam)) ||
			((lWrongTeam == 2) && (players[g_iSortedPlayers[ulIdx]].Team == ulDesiredTeam)))
			continue;

		scoreboard_RenderIndividualPlayer( ulPlayer, g_iSortedPlayers[ulIdx] );
		g_ulCurYPos += SmallFont->GetHeight( ) + 1;
		ulNumPlayers++;
	}

	if ( ulNumPlayers )
		g_ulCurYPos += SmallFont->GetHeight( ) + 1;
}

//*****************************************************************************
//
static void scoreboard_DrawRankings( ULONG ulPlayer )
{
	// Nothing to do.
	if ( g_ulNumColumnsUsed < 1 )
		return;

	g_ulCurYPos += 8;

	// Center this a little better in intermission
	if ( gamestate != GS_LEVEL )
		g_ulCurYPos = static_cast<LONG>( 48 * ( g_bScale ? g_fYScale : CleanYfac ));

	// Draw the titles for the columns.
	for ( ULONG ulColumn = 0; ulColumn < g_ulNumColumnsUsed; ulColumn++ )
		HUD_DrawText( g_pColumnHeaderFont, CR_RED, static_cast<LONG>( g_aulColumnX[ulColumn] * g_fXScale ), g_ulCurYPos, g_pszColumnHeaders[g_aulColumnType[ulColumn]] );

	// Draw the player list.
	g_ulCurYPos += 24;

	// Team-based games: Divide up the teams.
	if ( GAMEMODE_GetCurrentFlags() & GMF_PLAYERSONTEAMS )
	{
		// Draw players on teams.
		for ( ULONG ulTeamIdx = 0; ulTeamIdx < teams.Size( ); ulTeamIdx++ )
		{
			// In team LMS, separate the dead players from the living.
			if (( teamlms ) && ( gamestate != GS_INTERMISSION ) && ( LASTMANSTANDING_GetState( ) != LMSS_COUNTDOWN ) && ( LASTMANSTANDING_GetState( ) != LMSS_WAITINGFORPLAYERS ))
			{
				scoreboard_DoRankingListPass( ulPlayer, 1, 1, 1, 1, 1, ulTeamIdx ); // Living in this team
				scoreboard_DoRankingListPass( ulPlayer, 1, 2, 1, 1, 1, ulTeamIdx ); // Dead in this team
			}
			// Otherwise, draw all players all in one group.
			else
				scoreboard_DoRankingListPass( ulPlayer, 1, 0, 1, 1, 1, ulTeamIdx ); 

		}

		// Players that aren't on a team.
		scoreboard_DoRankingListPass( ulPlayer, 1, 1, 1, 2, 0, 0 ); 

		// Spectators are last.
		scoreboard_DoRankingListPass( ulPlayer, 2, 0, 1, 0, 0, 0 );
	}
	// Other modes: Just players and spectators.
	else
	{
		// [WS] Does the gamemode we are in use lives?
		// If so, dead players are drawn after living ones.
		if (( gamestate != GS_INTERMISSION ) && GAMEMODE_AreLivesLimited( ) && GAMEMODE_IsGameInProgress( ) )
		{
			scoreboard_DoRankingListPass( ulPlayer, 1, 1, 1, 0, 0, 0 ); // Living
			scoreboard_DoRankingListPass( ulPlayer, 1, 2, 1, 0, 0, 0 ); // Dead
		}
		// Othrwise, draw all active players in the game together.
		else
			scoreboard_DoRankingListPass( ulPlayer, 1, 0, 1, 0, 0, 0 );

		// Spectators are last.
		scoreboard_DoRankingListPass( ulPlayer, 2, 0, 1, 0, 0, 0 );
	}

	V_SetBorderNeedRefresh();
}

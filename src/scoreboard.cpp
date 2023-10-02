//-----------------------------------------------------------------------------
//
// Skulltag Source
// Copyright (C) 2002 Brad Carney
// Copyright (C) 2021-2023 Adam Kaminski
// Copyright (C) 2007-2023 Skulltag/Zandronum Development Team
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

#include <algorithm>
#include "c_dispatch.h"
#include "callvote.h"
#include "chat.h"
#include "cl_demo.h"
#include "cooperative.h"
#include "deathmatch.h"
#include "gi.h"
#include "joinqueue.h"
#include "scoreboard.h"
#include "team.h"
#include "v_video.h"
#include "st_hud.h"
#include "c_console.h"
#include "d_netinf.h"
#include "v_palette.h"
#include "r_data/r_translate.h"

// [AK] Implement the string table and the conversion functions for the scoreboard enums.
#define GENERATE_ENUM_STRINGS  // Start string generation
#include "scoreboard_enums.h"
#undef GENERATE_ENUM_STRINGS   // Stop string generation

//*****************************************************************************
//	VARIABLES

// [AK] A list of all defined columns.
static	TMap<FName, ScoreColumn *>	g_Columns;

// [AK] The main scoreboard object.
static	Scoreboard	g_Scoreboard;

//*****************************************************************************
//	PROTOTYPES

static	ScoreColumn		*scoreboard_ScanForColumn( FScanner &sc, const bool bMustBeDataColumn );

template <typename ColumnType>
static	bool	scoreboard_TryPushingColumnToList( FScanner &sc, TArray<ColumnType *> &ColumnList, ColumnType *pColumn );

template <typename ColumnType>
static	bool	scoreboard_TryRemovingColumnFromList( FScanner &sc, TArray<ColumnType *> &ColumnList, ColumnType *pColumn );

//*****************************************************************************
//	CONSOLE VARIABLES

// [JS] Display the amount of time left on the intermission screen.
CVAR( Bool, cl_intermissiontimer, false, CVAR_ARCHIVE );

// [AK] Prints everyone's pings in different colours, indicating how severe their connection is.
CVAR( Bool, cl_colorizepings, false, CVAR_ARCHIVE );

// [AK] If true, the country code column will use alpha-3 instead of alpha-2.
CVAR( Bool, cl_usealpha3countrycode, false, CVAR_ARCHIVE );

// [AK] If true, then columns will use their short names in the headers.
CVAR( Bool, cl_useshortcolumnnames, false, CVAR_ARCHIVE );

// [AK] Controls the opacity of the entire scoreboard.
CUSTOM_CVAR( Float, cl_scoreboardalpha, 1.0f, CVAR_ARCHIVE )
{
	float fClampedValue = clamp<float>( self, 0.0f, 1.0f );

	if ( self != fClampedValue )
		self = fClampedValue;
}

//*****************************************************************************
//	PLAYER VALUE SPECIALIZATIONS

// Int data type.
template <> int PlayerValue::RetrieveValue( void ) const { return Int; }
template <> void PlayerValue::ModifyValue( int NewValue ) { Int = NewValue; }
template <> const DATATYPE_e PlayerValue::Trait<int>::DataType = DATATYPE_INT;
template <> const int PlayerValue::Trait<int>::Zero = 0;

// Bool data type.
template <> bool PlayerValue::RetrieveValue( void ) const { return Bool; }
template <> void PlayerValue::ModifyValue( bool NewValue ) { Bool = NewValue; }
template <> const DATATYPE_e PlayerValue::Trait<bool>::DataType = DATATYPE_BOOL;
template <> const bool PlayerValue::Trait<bool>::Zero = false;

// Float data type.
template <> float PlayerValue::RetrieveValue( void ) const { return Float; }
template <> void PlayerValue::ModifyValue( float NewValue ) { Float = NewValue; }
template <> const DATATYPE_e PlayerValue::Trait<float>::DataType = DATATYPE_FLOAT;
template <> const float PlayerValue::Trait<float>::Zero = 0.0f;

// String data type.
template <> const char *PlayerValue::RetrieveValue( void ) const { return String; }
template <> void PlayerValue::ModifyValue( const char *NewValue ) { String = ncopystring( NewValue ); }
template <> const DATATYPE_e PlayerValue::Trait<const char *>::DataType = DATATYPE_STRING;
template <> const char *const PlayerValue::Trait<const char *>::Zero = NULL;

// Color data type.
template <> PalEntry PlayerValue::RetrieveValue( void ) const { return Int; }
template <> void PlayerValue::ModifyValue( PalEntry NewValue ) { Int = NewValue; }
template <> const DATATYPE_e PlayerValue::Trait<PalEntry>::DataType = DATATYPE_COLOR;
template <> const PalEntry PlayerValue::Trait<PalEntry>::Zero = 0;

// Texture data type.
template <> FTexture *PlayerValue::RetrieveValue( void ) const { return Texture; }
template <> void PlayerValue::ModifyValue( FTexture *NewValue ) { Texture = NewValue; }
template <> const DATATYPE_e PlayerValue::Trait<FTexture *>::DataType = DATATYPE_TEXTURE;
template <> FTexture *const PlayerValue::Trait<FTexture *>::Zero = 0;

//*****************************************************************************
//	FUNCTIONS

//*****************************************************************************
//
// [AK] PlayerValue::GetValue
//
// Gets the value that is currently being held. If the input data type doesn't
// match what is currently used, then the "zero" value of that data type is
// returned instead.
//
//*****************************************************************************

template <typename Type> Type PlayerValue::GetValue( void ) const
{
	return ( DataType == Trait<Type>::DataType ) ? RetrieveValue<Type>( ) : Trait<Type>::Zero;
}

// [AK] Explicit specializations of all data types.
template int PlayerValue::GetValue<int>( void ) const;
template bool PlayerValue::GetValue<bool>( void ) const;
template float PlayerValue::GetValue<float>( void ) const;
template const char *PlayerValue::GetValue<const char *>( void ) const;
template PalEntry PlayerValue::GetValue<PalEntry>( void ) const;
template FTexture *PlayerValue::GetValue<FTexture *>( void ) const;

//*****************************************************************************
//
// [AK] PlayerValue::SetValue
//
// Changes the value, and possibly the data type, that's stored.
//
//*****************************************************************************

template <typename Type> void PlayerValue::SetValue( Type NewValue )
{
	if ( DataType == DATATYPE_STRING )
		DeleteString( );

	DataType = Trait<Type>::DataType;
	ModifyValue<Type>( NewValue );
}

// [AK] Explicit specializations of all data types.
template void PlayerValue::SetValue<int>( int NewValue );
template void PlayerValue::SetValue<bool>( bool NewValue );
template void PlayerValue::SetValue<float>( float NewValue );
template void PlayerValue::SetValue<const char *>( const char *NewValue );
template void PlayerValue::SetValue<PalEntry>( PalEntry NewValue );
template void PlayerValue::SetValue<FTexture *>( FTexture *NewValue );

//*****************************************************************************
//
// [AK] PlayerValue::TransferValue
//
// Transfers the value of one object to another.
//
//*****************************************************************************

void PlayerValue::TransferValue ( const PlayerValue &Other )
{
	switch ( Other.GetDataType( ))
	{
		case DATATYPE_INT:
			SetValue<int>( Other.RetrieveValue<int>( ));
			break;

		case DATATYPE_BOOL:
			SetValue<bool>( Other.RetrieveValue<bool>( ));
			break;

		case DATATYPE_FLOAT:
			SetValue<float>( Other.RetrieveValue<float>( ));
			break;

		case DATATYPE_STRING:
			SetValue<const char *>( Other.RetrieveValue<const char *>( ));
			break;

		case DATATYPE_COLOR:
			SetValue<PalEntry>( Other.RetrieveValue<PalEntry>( ));
			break;

		case DATATYPE_TEXTURE:
			SetValue<FTexture *>( Other.RetrieveValue<FTexture *>( ));
			break;

		default:
		{
			if ( DataType == DATATYPE_STRING )
				DeleteString( );

			DataType = DATATYPE_UNKNOWN;
			break;
		}
	}
}

//*****************************************************************************
//
// [AK] PlayerValue::ToString
//
// Returns a string containing the value that's currently stored.
//
//*****************************************************************************

FString PlayerValue::ToString( void ) const
{
	FString Result;

	switch ( DataType )
	{
		case DATATYPE_INT:
			Result.Format( "%d", RetrieveValue<int>( ));
			break;

		case DATATYPE_BOOL:
			Result.Format( "%d", RetrieveValue<bool>( ));
			break;

		case DATATYPE_FLOAT:
			Result.Format( "%f", RetrieveValue<float>( ));
			break;

		case DATATYPE_STRING:
			Result = RetrieveValue<const char *>( );
			break;

		case DATATYPE_COLOR:
			Result.Format( "%d", static_cast<int>( RetrieveValue<PalEntry>( )));
			break;

		case DATATYPE_TEXTURE:
		{
			FTexture *pTexture = RetrieveValue<FTexture *>( );

			if ( pTexture != NULL )
				Result.Format( "%s", pTexture->Name );

			break;
		}

		default:
			break;
	}

	return Result;
}

//*****************************************************************************
//
// [AK] PlayerValue::FromString
//
// Assigns a value and data type using an input string.
//
//*****************************************************************************

void PlayerValue::FromString( const char *pszString, const DATATYPE_e NewDataType )
{
	if ( pszString == NULL )
		return;

	switch ( NewDataType )
	{
		case DATATYPE_INT:
		case DATATYPE_COLOR:
			SetValue<int>( atoi( pszString ));
			break;

		case DATATYPE_BOOL:
		{
			if ( stricmp( pszString, "true" ) == 0 )
				SetValue<bool>( true );
			else if ( stricmp( pszString, "false" ) == 0 )
				SetValue<bool>( false );
			else
				SetValue<bool>( !!atoi( pszString ));

			break;
		}

		case DATATYPE_FLOAT:
			SetValue<float>( static_cast<float>( atof( pszString )));
			break;

		case DATATYPE_STRING:
			SetValue<const char *>( pszString );
			break;

		case DATATYPE_TEXTURE:
			SetValue<FTexture *>( TexMan.FindTexture( pszString ));
			break;

		default:
			break;
	}
}

//*****************************************************************************
//
// [AK] PlayerValue::operator==
//
// Checks if two objects have the same data type and value.
//
//*****************************************************************************

bool PlayerValue::operator== ( const PlayerValue &Other ) const
{
	if ( DataType == Other.GetDataType( ))
	{
		switch ( DataType )
		{
			// [AK] Both objects having no values are technically "equal".
			case DATATYPE_UNKNOWN:
				return true;

			case DATATYPE_INT:
				return ( RetrieveValue<int>( ) == Other.RetrieveValue<int>( ));

			case DATATYPE_BOOL:
				return ( RetrieveValue<bool>( ) == Other.RetrieveValue<bool>( ));

			case DATATYPE_FLOAT:
				return ( RetrieveValue<float>( ) == Other.RetrieveValue<float>( ));

			case DATATYPE_STRING:
			{
				const char *pszString1 = RetrieveValue<const char *>( );
				const char *pszString2 = Other.RetrieveValue<const char *>( );

				// [AK] If one of the strings is NULL, then return either true if
				// both of them are NULL (i.e. pszString1 and pszString2 are equal),
				// or false otherwise.
				if (( pszString1 == NULL ) || ( pszString2 == NULL ))
					return ( pszString1 == pszString2 );

				return ( strcmp( pszString1, pszString2 ) == 0 );
			}

			case DATATYPE_COLOR:
				return ( RetrieveValue<PalEntry>( ) == Other.RetrieveValue<PalEntry>( ));

			case DATATYPE_TEXTURE:
				return ( RetrieveValue<FTexture *>( ) == Other.RetrieveValue<FTexture *>( ));

			default:
				return false;
		}
	}

	return false;
}

//*****************************************************************************
//
// [AK] PlayerValue::DeleteString
//
// If the value is a string that isn't NULL, then it's removed from memory.
//
//*****************************************************************************

void PlayerValue::DeleteString( void )
{
	if (( GetDataType( ) == DATATYPE_STRING ) && ( String != NULL ))
	{
		delete[] String;
		String = NULL;
	}
}

//*****************************************************************************
//
// [AK] PlayerData::PlayerData
//
// Initializes the data type and default value that will be used.
//
//*****************************************************************************

PlayerData::PlayerData( FScanner &sc, BYTE NewIndex ) : Index( NewIndex )
{
	// [AK] Grab the data type first.
	sc.MustGetToken( TK_StringConst );

	if ( sc.StringLen == 0 )
		sc.ScriptError( "Got an empty string for a data type." );

	DataType = static_cast<DATATYPE_e>( sc.MustGetEnumName( "data type", "DATATYPE_", GetValueDATATYPE_e, true ));

	// [AK] Don't accept an "unknown" data type.
	if ( DataType == DATATYPE_UNKNOWN )
		sc.ScriptError( "You can't specify an 'unknown' data type!" );

	sc.MustGetToken( ',' );

	// [AK] Next, grab the default value and store it into a string.
	switch ( DataType )
	{
		case DATATYPE_INT:
		{
			sc.MustGetNumber( );
			DefaultValString.Format( "%d", sc.Number );
			break;
		}

		case DATATYPE_FLOAT:
		{
			sc.MustGetFloat( );
			DefaultValString.Format( "%f", static_cast<float>( sc.Float ));
			break;
		}

		case DATATYPE_BOOL:
		case DATATYPE_STRING:
		case DATATYPE_COLOR:
		case DATATYPE_TEXTURE:
		{
			sc.MustGetString( );

			// [AK] Color values must be saved differently.
			if ( DataType == DATATYPE_COLOR )
			{
				FString ColorString = V_GetColorStringByName( sc.String );
				DefaultValString.Format( "%d", V_GetColorFromString( NULL, ColorString.IsNotEmpty( ) ? ColorString.GetChars( ) : sc.String ));
			}
			else
			{
				DefaultValString = sc.String;
			}

			break;
		}

		default:
			break;
	}
}

//*****************************************************************************
//
// [AK] PlayerData::GetValue
//
// Returns the value associated with a player, or the default value if the
// given player index is invalid.
//
//*****************************************************************************

PlayerValue PlayerData::GetValue( const ULONG ulPlayer ) const
{
	return PLAYER_IsValidPlayer( ulPlayer ) ? Val[ulPlayer] : GetDefaultValue( );
}

//*****************************************************************************
//
// [AK] PlayerData::GetDefaultValue
//
// Returns the default value.
//
//*****************************************************************************

PlayerValue PlayerData::GetDefaultValue( void ) const
{
	PlayerValue DefaultVal;
	DefaultVal.FromString( DefaultValString.GetChars( ), DataType );

	return DefaultVal;
}

//*****************************************************************************
//
// [AK] PlayerData::SetValue
//
// Changes the value of a player.
//
//*****************************************************************************

void PlayerData::SetValue( const ULONG ulPlayer, const PlayerValue &Value )
{
	// [AK] Stop here if the player's invalid, or the new value is equal to the old one.
	if (( PLAYER_IsValidPlayer( ulPlayer ) == false ) || ( GetValue( ulPlayer ) == Value ))
		return;

	// [AK] Only set the value if the data types match. Otherwise, throw a fatal error.
	if ( DataType != Value.GetDataType( ))
		I_Error( "PlayerData::SetValue: data type doesn't match." );

	Val[ulPlayer] = Value;

	// [AK] If we're the server, inform the clients that the value changed.
	if ( NETWORK_GetState( ) == NETSTATE_SERVER )
		SERVERCOMMANDS_SetCustomPlayerValue( *this, ulPlayer );
}

//*****************************************************************************
//
// [AK] PlayerData::ResetToDefault
//
// Resets the value of a single player, or all players, to the default value.
//
//*****************************************************************************

void PlayerData::ResetToDefault( const ULONG ulPlayer, const bool bInformClients )
{
	const PlayerValue DefaultVal = GetDefaultValue( );

	// [AK] Check if we want to restore the default value for all players.
	if ( ulPlayer == MAXPLAYERS )
	{
		for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
			Val[ulIdx] = DefaultVal;
	}
	// [AK] Otherwise, restore it only for the one player.
	else if ( ulPlayer < MAXPLAYERS )
	{
		Val[ulPlayer] = DefaultVal;
	}

	// [AK] If we're the server, tell clients to reset the value(s) to default.
	if (( NETWORK_GetState( ) == NETSTATE_SERVER ) && ( bInformClients ))
		SERVERCOMMANDS_ResetCustomPlayerValue( *this, ulPlayer );
}

//*****************************************************************************
//
// [AK] ScoreColumn::ScoreColumn
//
// Initializes the members of a ScoreColumn object to their default values.
//
//*****************************************************************************

ScoreColumn::ScoreColumn( const char *pszName ) :
	InternalName( pszName ),
	DisplayName( pszName ),
	Alignment( HORIZALIGN_LEFT ),
	pCVar( NULL ),
	ulFlags( 0 ),
	ulSizing( 0 ),
	ulShortestWidth( 0 ),
	ulWidth( 0 ),
	lRelX( 0 ),
	bUsableInCurrentGame( false ),
	bDisabled( false ),
	bUseShortName( false ),
	pScoreboard( NULL )
{
	// [AK] By default, this column is active in all game types and earn types.
	ulGameAndEarnTypeFlags = ( GAMETYPE_MASK | EARNTYPE_MASK );

	// [AK] By default, this column is active in all game modes.
	for ( ULONG ulGameMode = 0; ulGameMode < NUM_GAMEMODES; ulGameMode++ )
		GameModeList.insert( static_cast<GAMEMODE_e>( ulGameMode ));
}

//*****************************************************************************
//
// [AK] ScoreColumn::GetAlignmentPosition
//
// Uses the width of some content (e.g. a string, color box, or texture) and
// determines where the left-most part of that content should start with
// respect to the column's own position, width, and alignment.
//
//*****************************************************************************

LONG ScoreColumn::GetAlignmentPosition( ULONG ulContentWidth ) const
{
	if ( Alignment == HORIZALIGN_LEFT )
		return lRelX;
	else if ( Alignment == HORIZALIGN_CENTER )
		return lRelX + ( ulWidth - ulContentWidth ) / 2;
	else
		return lRelX + ulWidth - ulContentWidth;
}

//*****************************************************************************
//
// [AK] ScoreColumn::Parse
//
// Parses a "column", "compositecolumn", or "customcolumn" block in SCORINFO.
//
//*****************************************************************************

void ScoreColumn::Parse( FScanner &sc )
{
	sc.MustGetToken( '{' );

	while ( sc.CheckToken( '}' ) == false )
	{
		sc.MustGetString( );

		if ( stricmp( sc.String, "addflag" ) == 0 )
		{
			ulFlags |= sc.MustGetEnumName( "column flag", "COLUMNFLAG_", GetValueCOLUMNFLAG_e );
		}
		else if ( stricmp( sc.String, "removeflag" ) == 0 )
		{
			ulFlags &= ~sc.MustGetEnumName( "column flag", "COLUMNFLAG_", GetValueCOLUMNFLAG_e );
		}
		else
		{
			COLUMNCMD_e Command = static_cast<COLUMNCMD_e>( sc.MustGetEnumName( "column command", "COLUMNCMD_", GetValueCOLUMNCMD_e, true ));
			FString CommandName = sc.String;

			sc.MustGetToken( '=' );
			ParseCommand( sc, Command, CommandName );
		}
	}

	// [AK] Unless the ALWAYSUSESHORTESTWIDTH flag is enabled, columns must have a non-zero width.
	if ((( ulFlags & COLUMNFLAG_ALWAYSUSESHORTESTWIDTH ) == false ) && ( ulSizing == 0 ))
		sc.ScriptError( "Column '%s' needs a size that's greater than zero.", GetInternalName( ));

	// [AK] Columns can't be offline-only and online-only at the same, that doesn't make sense.
	if (( ulFlags & COLUMNFLAG_OFFLINEONLY ) && ( ulFlags & COLUMNFLAG_ONLINEONLY ))
		sc.ScriptError( "Column '%s' can't have both the OFFLINEONLY and ONLINEONLY flags enabled at the same time.", GetInternalName( ));

	// [AK] If the short name is longer than the display name, throw a fatal error.
	if ( DisplayName.Len( ) < ShortName.Len( ))
		sc.ScriptError( "Column '%s' has a short name that's greater than its display name.", GetInternalName( ));
}

//*****************************************************************************
//
// [AK] ScoreColumn::ParseCommand
//
// Parses commands that are shared by all (data and composite) columns.
//
//*****************************************************************************

void ScoreColumn::ParseCommand( FScanner &sc, const COLUMNCMD_e Command, const FString CommandName )
{
	switch ( Command )
	{
		case COLUMNCMD_DISPLAYNAME:
		case COLUMNCMD_SHORTNAME:
		{
			sc.MustGetString( );

			// [AK] If the name begins with a '$', look up the string in the LANGUAGE lump.
			const char *pszString = sc.String[0] == '$' ? GStrings[sc.String] : sc.String;

			if ( Command == COLUMNCMD_DISPLAYNAME )
				DisplayName = pszString;
			else
				ShortName = pszString;

			break;
		}

		case COLUMNCMD_ALIGNMENT:
		{
			Alignment = static_cast<HORIZALIGN_e>( sc.MustGetEnumName( "alignment", "HORIZALIGN_", GetValueHORIZALIGN_e ));
			break;
		}

		case COLUMNCMD_SIZE:
		{
			sc.MustGetNumber( );
			ulSizing = MAX( sc.Number, 0 );
			break;
		}

		case COLUMNCMD_GAMEMODE:
		case COLUMNCMD_GAMETYPE:
		case COLUMNCMD_EARNTYPE:
		{
			// [AK] Clear all game modes.
			if ( Command == COLUMNCMD_GAMEMODE )
				GameModeList.clear( );
			// ...or reset all game type flags.
			else if ( Command == COLUMNCMD_GAMETYPE )
				ulGameAndEarnTypeFlags &= ~GAMETYPE_MASK;
			// ...or reset all earn type flags.
			else
				ulGameAndEarnTypeFlags &= ~EARNTYPE_MASK;

			do
			{
				sc.MustGetToken( TK_Identifier );

				if ( Command == COLUMNCMD_GAMEMODE )
				{
					GameModeList.insert( static_cast<GAMEMODE_e>( sc.MustGetEnumName( "game mode", "GAMEMODE_", GetValueGAMEMODE_e, true )));
				}
				else if ( Command == COLUMNCMD_GAMETYPE )
				{
					ULONG ulFlag = sc.MustGetEnumName( "game type", "GMF_", GetValueGMF, true );

					// [AK] Make sure there aren't other constants besides COOPERATIVE, DEATHMATCH, or TEAMGAME.
					if (( ulFlag & GAMETYPE_MASK ) == 0 )
						sc.ScriptError( "A game type list must contain only COOPERATIVE, DEATHMATCH, or TEAMGAME. Using '%s' is invalid.", sc.String );

					ulGameAndEarnTypeFlags |= ulFlag;
				}
				else
				{
					ulGameAndEarnTypeFlags |= sc.MustGetEnumName( "earn type", "GMF_PLAYERSEARN", GetValueGMF, true );
				}
			} while ( sc.CheckToken( ',' ));

			break;
		}

		case COLUMNCMD_CVAR:
		{
			sc.MustGetString( );

			// [AK] Specifying "none" for the CVar clears any CVar being used by the column.
			// This also means that a CVar named "none" (if one actually existed) can never be used.
			if (( stricmp( sc.String, "none" ) == 0 ) && ( pCVar != NULL ))
			{
				pCVar = NULL;
			}
			else
			{
				FBaseCVar *pFoundCVar = FindCVar( sc.String, NULL );

				// [AK] Throw an error if this CVar doesn't exist.
				if ( pFoundCVar == NULL )
					sc.ScriptError( "'%s' is not a CVar.", sc.String );

				// [AK] Throw an error if this CVar isn't a boolean, integer, or flag.
				if (( pFoundCVar->GetRealType( ) != CVAR_Bool ) && ( pFoundCVar->IsFlagCVar( ) == false ))
					sc.ScriptError( "'%s' is not a boolean or flag CVar.", sc.String );

				pCVar = pFoundCVar;
			}

			break;
		}

		default:
			sc.ScriptError( "Couldn't process column command '%s' for column '%s'.", CommandName.GetChars( ), GetInternalName( ));
			break;
	}
}

//*****************************************************************************
//
// [AK] ScoreColumn::CheckIfUsable
//
// Checks if this column works at all in the current game, including whether
// or not the current game mode is compatible, or if the column is allowed in
// offline or online games.
//
//*****************************************************************************

void ScoreColumn::CheckIfUsable( void )
{
	bUsableInCurrentGame = false;

	// [AK] If the column isn't part of a scoreboard, then stop here.
	if ( pScoreboard == NULL )
		return;

	// [AK] If the current game mode isn't allowed for this column, then it can't be active.
	if ( GameModeList.find( GAMEMODE_GetCurrentMode( )) == GameModeList.end( ))
		return;

	const ULONG ulGameModeFlags = GAMEMODE_GetCurrentFlags( );

	// [AK] Check if the current game type won't allow this column to be active.
	if ( ulGameAndEarnTypeFlags & GAMETYPE_MASK )
	{
		if ((( ulGameModeFlags & ulGameAndEarnTypeFlags ) & GAMETYPE_MASK ) == 0 )
			return;
	}

	// [AK] Check if the current game mode's earn type won't allow this column to be active.
	if ( ulGameAndEarnTypeFlags & EARNTYPE_MASK )
	{
		if ((( ulGameModeFlags & ulGameAndEarnTypeFlags ) & EARNTYPE_MASK ) == 0 )
			return;
	}

	ULONG ulRequiredFlags = 0;

	// [AK] We'll check if the column requires the PLAYERONTEAMS, USEMAXLIVES, and USETEAMITEM
	// game mode flags to be enabled. Make sure that the current game mode has all the required flags.
	if ( ulFlags & COLUMNFLAG_REQUIRESTEAMS )
		ulRequiredFlags |= GMF_PLAYERSONTEAMS;
	if ( ulFlags & COLUMNFLAG_REQUIRESLIVES )
		ulRequiredFlags |= GMF_USEMAXLIVES;
	if ( ulFlags & COLUMNFLAG_REQUIRESTEAMITEMS )
		ulRequiredFlags |= GMF_USETEAMITEM;

	if (( ulRequiredFlags != 0 ) && (( ulRequiredFlags & ulGameModeFlags ) != ulRequiredFlags ))
		return;

	ULONG ulForbiddenFlags = 0;

	// [AK] We'll also check if the column requires the aforementioned game mode flags to be disabled.
	// If the current game mode has at least one of the flags that the column forbids, stop here.
	if ( ulFlags & COLUMNFLAG_FORBIDTEAMS )
		ulForbiddenFlags |= GMF_PLAYERSONTEAMS;
	if ( ulFlags & COLUMNFLAG_FORBIDLIVES )
		ulForbiddenFlags |= GMF_USEMAXLIVES;
	if ( ulFlags & COLUMNFLAG_FORBIDTEAMITEMS )
		ulForbiddenFlags |= GMF_USETEAMITEM;

	if ( ulForbiddenFlags & ulGameModeFlags )
		return;

	// [AK] Next, we'll check if the column is only active in offline or online games. A column should
	// be disabled in online games if OFFLINEONLY is enabled, and offline games if ONLINEONLY is enabled.
	if ( NETWORK_InClientMode( ))
	{
		if ( ulFlags & COLUMNFLAG_OFFLINEONLY )
			return;
	}
	else if ( ulFlags & COLUMNFLAG_ONLINEONLY )
	{
		return;
	}

	bUsableInCurrentGame = true;
}

//*****************************************************************************
//
// [AK] ScoreColumn::Refresh
//
// Performs checks to see if a column should be active or disabled.
//
//*****************************************************************************

void ScoreColumn::Refresh( void )
{
	bDisabled = true;

	// [AK] If the column's currently unusable, stop here.
	if ( bUsableInCurrentGame == false )
		return;

	// [AK] If this column has a CVar associated with it, check to see if the column should be
	// active based on the CVar's value. If the conditions fail, stop here.
	if ( pCVar != NULL )
	{
		const bool bValue = pCVar->GetGenericRep( CVAR_Bool ).Bool;

		if ( ulFlags & COLUMNFLAG_CVARMUSTBEZERO )
		{
			if ( bValue != false )
				return;
		}
		else if ( bValue == false )
		{
			return;
		}
	}

	// [AK] Disable this column if it's supposed to be invisible on the intermission screen, or if it's
	// supposed to be invisible in-game.
	if ((( gamestate == GS_INTERMISSION ) && ( ulFlags & COLUMNFLAG_NOINTERMISSION )) ||
		(( gamestate == GS_LEVEL ) && ( ulFlags & COLUMNFLAG_INTERMISSIONONLY )))
	{
		return;
	}

	bDisabled = false;

	// [AK] Should this column use its short or normal display name?
	if (( cl_useshortcolumnnames ) && ( ShortName.Len( ) > 0 ))
		bUseShortName = true;
	else
		bUseShortName = false;
}

//*****************************************************************************
//
// [AK] ScoreColumn::UpdateWidth
//
// Determines what the width of the column should be right now.
//
//*****************************************************************************

void ScoreColumn::UpdateWidth( void )
{
	// [AK] Don't do anything if this column isn't part of a scoreboard.
	if ( pScoreboard == NULL )
		return;

	// [AK] Check if the column must be disabled if its contents are empty.
	if (( ulShortestWidth == 0 ) && ( ulFlags & COLUMNFLAG_DISABLEIFEMPTY ))
	{
		bDisabled = true;
		return;
	}

	ULONG ulHeaderWidth = 0;

	// [AK] If the header is visible on this column, then grab its width.
	if ((( ulFlags & COLUMNFLAG_DONTSHOWHEADER ) == false ) && ( pScoreboard->pHeaderFont != NULL ))
		ulHeaderWidth = pScoreboard->pHeaderFont->StringWidth( bUseShortName ? ShortName.GetChars( ) : DisplayName.GetChars( ));

	ulShortestWidth = MAX( ulShortestWidth, ulHeaderWidth );

	// [AK] Always use the shortest (or header) width if required. In this case,
	// the sizing is added onto the shortest width as padding instead.
	if ( ulFlags & COLUMNFLAG_ALWAYSUSESHORTESTWIDTH )
		ulWidth = ulShortestWidth + ulSizing;
	// [AK] Otherwise, set the column's width to whichever is bigger: the sizing
	// (which becomes the default width of the column), the shortest width, or
	// the header's width.
	else
		ulWidth = MAX( ulSizing, ulShortestWidth );

	// [AK] If the column's width is still zero, just disable it.
	if ( ulWidth == 0 )
		bDisabled = true;
}

//*****************************************************************************
//
// [AK] ScoreColumn::DrawHeader
//
// Draws the column's header with the specified font and color.
//
//*****************************************************************************

void ScoreColumn::DrawHeader( const LONG lYPos, const ULONG ulHeight, const float fAlpha ) const
{
	if (( pScoreboard == NULL ) || ( bDisabled ) || ( ulFlags & COLUMNFLAG_DONTSHOWHEADER ))
		return;

	DrawString( bUseShortName ? ShortName.GetChars( ) : DisplayName.GetChars( ), pScoreboard->pHeaderFont, pScoreboard->HeaderColor, lYPos, ulHeight, fAlpha );
}

//*****************************************************************************
//
// [AK] ScoreColumn::DrawString
//
// Draws a string within the body of the column.
//
//*****************************************************************************

void ScoreColumn::DrawString( const char *pszString, FFont *pFont, const ULONG ulColor, const LONG lYPos, const ULONG ulHeight, const float fAlpha ) const
{
	if (( pszString == NULL ) || ( pFont == NULL ))
		return;

	const ULONG ulLength = strlen( pszString );

	// [AK] Don't bother drawing the string if it's empty.
	if ( ulLength == 0 )
		return;

	LONG lXPos = GetAlignmentPosition( pFont->StringWidth( pszString ));
	ULONG ulLargestCharHeight = 0;

	// [AK] Get the largest character height so the string is aligned within the centre of the specified height.
	for ( unsigned int i = 0; i < ulLength; i++ )
	{
		FTexture *pCharTexture = pFont->GetChar( pszString[i], NULL );

		if ( pCharTexture != NULL )
		{
			const ULONG ulTextureHeight = pCharTexture->GetScaledHeight( );

			if ( ulTextureHeight > ulLargestCharHeight )
				ulLargestCharHeight = ulTextureHeight;
		}
	}

	int clipLeft = lRelX;
	int clipWidth = ulWidth;
	int clipTop = lYPos;
	int clipHeight = ulHeight;

	LONG lNewYPos = lYPos + ( clipHeight - static_cast<LONG>( ulLargestCharHeight )) / 2;

	// [AK] We must take into account the virtual screen's size when setting up the clipping rectangle.
	// Nothing should be drawn outside of this rectangle (i.e. the column's boundaries).
	if ( g_bScale )
		screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidth, clipHeight, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

	screen->DrawText( pFont, ulColor, lXPos, lNewYPos, pszString,
		DTA_UseVirtualScreen, g_bScale,
		DTA_ClipLeft, clipLeft,
		DTA_ClipRight, clipLeft + clipWidth,
		DTA_ClipTop, clipTop,
		DTA_ClipBottom, clipTop + clipHeight,
		DTA_Alpha, FLOAT2FIXED( fAlpha ),
		TAG_DONE );
}

//*****************************************************************************
//
// [AK] ScoreColumn::DrawColor
//
// Draws a hexadecimal color within the body of the column.
//
//*****************************************************************************

void ScoreColumn::DrawColor( const PalEntry color, const LONG lYPos, const ULONG ulHeight, const float fAlpha, const int clipWidth, const int clipHeight ) const
{
	int clipWidthToUse;
	int clipHeightToUse;

	FixClipRectSize( clipWidth, clipHeight, ulHeight, clipWidthToUse, clipHeightToUse );

	int clipLeft = GetAlignmentPosition( clipWidthToUse );
	int clipTop = lYPos + ( static_cast<LONG>( ulHeight ) - clipHeightToUse ) / 2;

	// [AK] We must take into account the virtual screen's size.
	if ( g_bScale )
		screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidthToUse, clipHeightToUse, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

	screen->Dim( color, fAlpha, clipLeft, clipTop, clipWidthToUse, clipHeightToUse );
}

//*****************************************************************************
//
// [AK] ScoreColumn::DrawTexture
//
// Draws a texture within the body of the column.
//
//*****************************************************************************

void ScoreColumn::DrawTexture( FTexture *pTexture, const LONG lYPos, const ULONG ulHeight, const float fAlpha, const int clipWidth, const int clipHeight ) const
{
	int clipWidthToUse;
	int clipHeightToUse;

	if ( pTexture == NULL )
		return;

	LONG lXPos = GetAlignmentPosition( pTexture->GetScaledWidth( ));

	FixClipRectSize( clipWidth, clipHeight, ulHeight, clipWidthToUse, clipHeightToUse );

	int clipLeft = GetAlignmentPosition( clipWidthToUse );
	int clipTop = lYPos + ( ulHeight - clipHeightToUse ) / 2;

	LONG lNewYPos = lYPos + ( static_cast<LONG>( ulHeight ) - pTexture->GetScaledHeight( )) / 2;

	// [AK] We must take into account the virtual screen's size.
	if ( g_bScale )
		screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidthToUse, clipHeightToUse, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

	screen->DrawTexture( pTexture, lXPos, lNewYPos,
		DTA_UseVirtualScreen, g_bScale,
		DTA_ClipLeft, clipLeft,
		DTA_ClipRight, clipLeft + clipWidthToUse,
		DTA_ClipTop, clipTop,
		DTA_ClipBottom, clipTop + clipHeightToUse,
		DTA_Alpha, FLOAT2FIXED( fAlpha ),
		TAG_DONE );
}

//*****************************************************************************
//
// [AK] ScoreColumn::CanDrawForPlayer
//
// Checks if a column can be drawn for a particular player.
//
//*****************************************************************************

bool ScoreColumn::CanDrawForPlayer( const ULONG ulPlayer ) const
{
	// [AK] Don't draw if the column's disabled, or the player's invalid.
	if (( pScoreboard == NULL ) || ( bDisabled ) || ( PLAYER_IsValidPlayer( ulPlayer ) == false ))
		return false;

	// [AK] Don't draw for true spectators if they're meant to be excluded.
	if (( ulFlags & COLUMNFLAG_NOSPECTATORS ) && ( PLAYER_IsTrueSpectator( &players[ulPlayer] )))
		return false;

	return true;
}

//*****************************************************************************
//
// [AK] ScoreColumn::FixClipRectSize
//
// Takes an input width and height for a clipping rectangle and ensures that
// the "fixed" width and height aren't less than zero, or greater than the
// column's width and the height passed into this function respectively.
//
//*****************************************************************************

void ScoreColumn::FixClipRectSize( const int clipWidth, const int clipHeight, const ULONG ulHeight, int &fixedWidth, int &fixedHeight ) const
{
	if (( clipWidth <= 0 ) || ( static_cast<ULONG>( clipWidth ) > ulWidth ))
		fixedWidth = ulWidth;
	else
		fixedWidth = clipWidth;

	// [AK] If the input clip height is negative, subtract it from ulHeight.
	if ( clipHeight < 0 )
	{
		fixedHeight = ulHeight + clipHeight;

		// [AK] If the fixed height is less than zero, just set it to ulHeight.
		if ( fixedHeight <= 0 )
			fixedHeight = ulHeight;
	}
	else if (( clipHeight == 0 ) || ( static_cast<ULONG>( clipHeight ) > ulHeight ))
	{
		fixedHeight = ulHeight;
	}
	else
	{
		fixedHeight = clipHeight;
	}
}

//*****************************************************************************
//
// [AK] DataScoreColumn::GetContentType
//
// Returns the type of content (i.e. either text-based or graphic-based) based
// on the column's data type. If the data type's unknown for any reason, then
// the content type is also unknown.
//
//*****************************************************************************

DATACONTENT_e DataScoreColumn::GetContentType( void ) const
{
	switch ( GetDataType( ))
	{
		case DATATYPE_INT:
		case DATATYPE_BOOL:
		case DATATYPE_FLOAT:
		case DATATYPE_STRING:
			return DATACONTENT_TEXT;

		case DATATYPE_COLOR:
		case DATATYPE_TEXTURE:
			return DATACONTENT_GRAPHIC;

		default:
			return DATACONTENT_UNKNOWN;
	}
}

//*****************************************************************************
//
// [AK] DataScoreColumn::GetDataType
//
// Returns the column's data type based on its native type. For example, frags
// are stored as integers, so the frags column would use the integer data type.
//
//*****************************************************************************

DATATYPE_e DataScoreColumn::GetDataType( void ) const
{
	switch ( NativeType )
	{
		case COLUMNTYPE_INDEX:
		case COLUMNTYPE_TIME:
		case COLUMNTYPE_PING:
		case COLUMNTYPE_FRAGS:
		case COLUMNTYPE_POINTS:
		case COLUMNTYPE_WINS:
		case COLUMNTYPE_KILLS:
		case COLUMNTYPE_DEATHS:
		case COLUMNTYPE_SECRETS:
		case COLUMNTYPE_LIVES:
		case COLUMNTYPE_DAMAGE:
		case COLUMNTYPE_HANDICAP:
		case COLUMNTYPE_JOINQUEUE:
			return DATATYPE_INT;

		case COLUMNTYPE_NAME:
		case COLUMNTYPE_VOTE:
		case COLUMNTYPE_COUNTRYNAME:
		case COLUMNTYPE_COUNTRYCODE:
			return DATATYPE_STRING;

		case COLUMNTYPE_PLAYERCOLOR:
			return DATATYPE_COLOR;

		case COLUMNTYPE_STATUSICON:
		case COLUMNTYPE_READYTOGOICON:
		case COLUMNTYPE_PLAYERICON:
		case COLUMNTYPE_ARTIFACTICON:
		case COLUMNTYPE_BOTSKILLICON:
		case COLUMNTYPE_COUNTRYFLAG:
			return DATATYPE_TEXTURE;

		case COLUMNTYPE_CUSTOM:
		{
			PlayerData *pData = gameinfo.CustomPlayerData.CheckKey( InternalName );

			if ( pData == NULL )
				I_Error( "DataScoreColumn::GetDataType: custom column '%s' has no data.", GetInternalName( ));

			return pData->GetDataType( );
		}

		default:
			return DATATYPE_UNKNOWN;
	}
}

//*****************************************************************************
//
// [AK] DataScoreColumn::GetValueString
//
// This is intended to be used only with text-based column data types like
// integers, boolean, floats, and strings. It takes the input value and formats
// it into a string, while respecting the column's maximum length. Read further
// down to know how some data types are affected by this limit.
//
// Afterwards, it includes the prefix and suffix text, if applicable.
//
//*****************************************************************************

FString DataScoreColumn::GetValueString( const PlayerValue &Value ) const
{
	FString text;

	if ( Value.GetDataType( ) == DATATYPE_INT )
	{
		// [AK] A column's maximum length doesn't apply to integers.
		text.Format( "%d", Value.GetValue<int>( ));
	}
	else if ( Value.GetDataType( ) == DATATYPE_FLOAT )
	{
		// [AK] If the maximum length of a column is non-zero, then the floating point
		// number is rounded to the same number of decimals. Otherwise, the number is
		// left as it is. For example, if only two decimals are allowed, then a value
		// like 3.14159 is rounded to 3.14.
		if ( ulMaxLength == 0 )
			text.Format( "%f", Value.GetValue<float>( ));
		else
			text.Format( "%.*f", static_cast<int>( ulMaxLength ), Value.GetValue<float>( ));
	}
	else if (( Value.GetDataType( ) == DATATYPE_BOOL ) || ( Value.GetDataType( ) == DATATYPE_STRING ))
	{
		// [AK] If the data type is boolean, use the column's true or false text instead.
		if ( Value.GetDataType( ) == DATATYPE_BOOL )
		{
			text = Value.GetValue<bool>( ) ? TrueText : FalseText;

			// [AK] If the true or false text are empty, then use "true" or "false" instead.
			if ( text.IsEmpty( ))
				text = Value.GetValue<bool>( ) ? "True" : "False";
		}
		else
		{
			text = Value.GetValue<const char *>( );
		}

		// [AK] If the number of characters in the passed string exceed the maximum
		// length that's allowed by the column, then the string is truncated to the
		// same length. Any trailing crap is also removed and an ellipsis is added.
		if (( ulMaxLength > 0 ) && ( text.Len( ) > ulMaxLength ))
		{
			text.Truncate( ulMaxLength );
			V_RemoveTrailingCrapFromFString( text );
			text += "...";
		}
	}

	if ( PrefixText.Len( ) > 0 )
		text.Insert( 0, PrefixText.GetChars( ));

	if ( SuffixText.Len( ) > 0 )
		text += SuffixText;

	return text;
}

//*****************************************************************************
//
// [AK] DataScoreColumn::GetValueWidth
//
// Gets the width of a value.
//
//*****************************************************************************

ULONG DataScoreColumn::GetValueWidth( const PlayerValue &Value ) const
{
	// [AK] Make sure that the column is part of a scoreboard.
	if ( pScoreboard != NULL )
	{
		switch ( Value.GetDataType( ))
		{
			case DATATYPE_INT:
			case DATATYPE_BOOL:
			case DATATYPE_FLOAT:
			case DATATYPE_STRING:
			{
				if ( pScoreboard->pRowFont == NULL )
					return 0;

				return pScoreboard->pRowFont->StringWidth( GetValueString( Value ).GetChars( ));
			}

			case DATATYPE_COLOR:
			{
				// [AK] If this column must always use the shortest possible width, then return the
				// clipping rectangle's width, whether it's zero or not.
				if ( ulFlags & COLUMNFLAG_ALWAYSUSESHORTESTWIDTH )
					return lClipRectWidth;

				// [AK] If the clipping rectangle's width is non-zero, return whichever is smaller:
				// the column's size (the default width here) or the clipping rectangle's width.
				return lClipRectWidth > 0 ? MIN<ULONG>( ulSizing, lClipRectWidth ) : ulSizing;
			}

			case DATATYPE_TEXTURE:
			{
				FTexture *pTexture = Value.GetValue<FTexture *>( );

				if ( pTexture == NULL )
					return 0;

				const ULONG ulTextureWidth = pTexture->GetScaledWidth( );
				return lClipRectWidth > 0 ? MIN<ULONG>( ulTextureWidth, lClipRectWidth ) : ulTextureWidth;
			}

			default:
				return 0;
		}
	}

	return 0;
}

//*****************************************************************************
//
// [AK] DataScoreColumn::GetValue
//
// Returns the value associated with a player.
//
//*****************************************************************************

PlayerValue DataScoreColumn::GetValue( const ULONG ulPlayer ) const
{
	// [AK] By default, the result's data type is initialized to DATATYPE_UNKNOWN.
	// If it's still unknown in the end, then no value was retrieved.
	PlayerValue Result;

	if ( PLAYER_IsValidPlayer( ulPlayer ))
	{
		switch ( NativeType )
		{
			case COLUMNTYPE_NAME:
				Result.SetValue<const char *>( players[ulPlayer].userinfo.GetName( ));
				break;

			case COLUMNTYPE_INDEX:
				Result.SetValue<int>( ulPlayer );
				break;

			case COLUMNTYPE_TIME:
				Result.SetValue<int>( players[ulPlayer].ulTime / ( TICRATE * 60 ));
				break;

			case COLUMNTYPE_PING:
				if ( players[ulPlayer].bIsBot )
					Result.SetValue< const char *>( "BOT" );
				else
					Result.SetValue<int>( players[ulPlayer].ulPing );
				break;

			case COLUMNTYPE_FRAGS:
				Result.SetValue<int>( players[ulPlayer].fragcount );
				break;

			case COLUMNTYPE_POINTS:
			case COLUMNTYPE_DAMAGE:
				Result.SetValue<int>( players[ulPlayer].lPointCount );
				break;

			case COLUMNTYPE_WINS:
				Result.SetValue<int>( players[ulPlayer].ulWins );
				break;

			case COLUMNTYPE_KILLS:
				Result.SetValue<int>( players[ulPlayer].killcount );
				break;

			case COLUMNTYPE_DEATHS:
				Result.SetValue<int>( players[ulPlayer].ulDeathCount );
				break;

			case COLUMNTYPE_SECRETS:
				Result.SetValue<int>( players[ulPlayer].secretcount );
				break;

			case COLUMNTYPE_LIVES:
				Result.SetValue<int>( players[ulPlayer].bSpectating ? 0 : players[ulPlayer].ulLivesLeft + 1 );
				break;

			case COLUMNTYPE_HANDICAP:
			{
				int handicap = players[ulPlayer].userinfo.GetHandicap( );

				// [AK] Only show a player's handicap if it's greater than zero.
				if ( handicap > 0 )
				{
					if (( lastmanstanding ) || ( teamlms ))
						Result.SetValue<int>( deh.MaxSoulsphere - handicap < 1 ? 1 : deh.MaxArmor - handicap );
					else
						Result.SetValue<int>( deh.StartHealth - handicap < 1 ? 1 : deh.StartHealth - handicap );
				}

				break;
			}

			case COLUMNTYPE_JOINQUEUE:
			{
				int position = JOINQUEUE_GetPositionInLine( ulPlayer );

				// [AK] Only return the position if the player is in the join queue.
				if ( position != -1 )
					Result.SetValue<int>( position + 1 );

				break;
			}

			case COLUMNTYPE_VOTE:
			{
				ULONG ulVoteChoice = CALLVOTE_GetPlayerVoteChoice( ulPlayer );

				// [AK] Check if this player either voted yes or no.
				if ( ulVoteChoice != VOTE_UNDECIDED )
					Result.SetValue<const char *>( ulVoteChoice == VOTE_YES ? "Yes" : "No" );

				break;
			}

			case COLUMNTYPE_PLAYERCOLOR:
			{
				float h, s, v, r, g, b;
				D_GetPlayerColor( ulPlayer, &h, &s, &v, NULL );
				HSVtoRGB( &r, &g, &b, h, s, v );

				PalEntry color( clamp( static_cast<int>( r * 255.f ), 0, 255 ), clamp( static_cast<int>( g * 255.f ), 0, 255 ), clamp( static_cast<int>( b * 255.f ), 0, 255 ));

				Result.SetValue<PalEntry>( color );
				break;
			}

			case COLUMNTYPE_STATUSICON:
				if (( players[ulPlayer].bLagging ) && ( gamestate == GS_LEVEL ))
					Result.SetValue<FTexture *>( TexMan.FindTexture( "LAGMINI" ));
				else if ( players[ulPlayer].bChatting )
					Result.SetValue<FTexture *>( TexMan.FindTexture( "TLKMINI" ));
				else if ( players[ulPlayer].bInConsole )
					Result.SetValue<FTexture *>( TexMan.FindTexture( "CONSMINI" ));
				else if ( players[ulPlayer].bInMenu )
					Result.SetValue<FTexture *>( TexMan.FindTexture( "MENUMINI" ));
				break;

			case COLUMNTYPE_READYTOGOICON:
				if ( players[ulPlayer].bReadyToGoOn )
					Result.SetValue<FTexture *>( TexMan.FindTexture( "RDYTOGO" ));
				break;

			case COLUMNTYPE_PLAYERICON:
				if (( players[ulPlayer].mo != NULL ) && ( players[ulPlayer].mo->ScoreIcon.GetIndex( ) != 0 ))
					Result.SetValue<FTexture *>( TexMan[players[ulPlayer].mo->ScoreIcon] );

				break;

			case COLUMNTYPE_ARTIFACTICON:
			{
				player_t *pCarrier = NULL;

				// [AK] In one-flag CTF, terminator, or (team) possession, check if this player is
				// carrying the white flag, terminator sphere, or hellstone respectively.
				if (( oneflagctf ) || ( terminator ) || ( possession ) || ( teampossession ))
				{
					pCarrier = GAMEMODE_GetArtifactCarrier( );

					if (( pCarrier != NULL ) && ( static_cast<ULONG>( pCarrier - players ) == ulPlayer ))
					{
						if ( oneflagctf )
							Result.SetValue<FTexture *>( TexMan.FindTexture( "STFLA3" ));
						else if ( terminator )
							Result.SetValue<FTexture *>( TexMan.FindTexture( "TERMINAT" ));
						else
							Result.SetValue<FTexture *>( TexMan.FindTexture( "HELLSTON" ));
					}
				}
				// [AK] In CTF or skulltag, check if this player is carrying an enemy team's item.
				else if (( ctf ) || ( skulltag ))
				{
					for ( ULONG ulTeam = 0; ulTeam < teams.Size( ); ulTeam++ )
					{
						pCarrier = TEAM_GetCarrier( ulTeam );

						if (( pCarrier != NULL ) && ( static_cast<ULONG>( pCarrier - players ) == ulPlayer ))
						{
							Result.SetValue<FTexture *>( TexMan.FindTexture( TEAM_GetSmallHUDIcon( ulTeam )));
							break;
						}
					}
				}

				break;
			}

			case COLUMNTYPE_BOTSKILLICON:
			{
				if ( players[ulPlayer].bIsBot )
				{
					FString IconName;
					IconName.Format( "BOTSKIL%d", botskill.GetGenericRep( CVAR_Int ).Int );

					Result.SetValue<FTexture *>( TexMan.FindTexture( IconName.GetChars( )));
				}

				break;
			}

			case COLUMNTYPE_COUNTRYNAME:
				Result.SetValue<const char *>( NETWORK_GetCountryNameFromIndex( players[ulPlayer].ulCountryIndex ));
				break;

			case COLUMNTYPE_COUNTRYCODE:
				Result.SetValue<const char *>( NETWORK_GetCountryCodeFromIndex( players[ulPlayer].ulCountryIndex, cl_usealpha3countrycode ));
				break;

			case COLUMNTYPE_CUSTOM:
			{
				PlayerData *pData = gameinfo.CustomPlayerData.CheckKey( InternalName );

				if ( pData == NULL )
					I_Error( "DataScoreColumn::GetValue: custom column '%s' has no data.", GetInternalName( ));

				Result = pData->GetValue( ulPlayer );
				break;
			}

			default:
				break;
		}
	}

	return Result;
}

//*****************************************************************************
//
// [AK] DataScoreColumn::Parse
//
// After parsing a "column" or "customcolumn" block in SCORINFO, this checks if
// the data column is inside a composite column, and if it is, ensures that the
// DONTSHOWHEADER flag hasn't been disabled and it's still aligned to the left.
//
//*****************************************************************************

void DataScoreColumn::Parse( FScanner &sc )
{
	ScoreColumn::Parse( sc );

	if ( pCompositeColumn != NULL )
	{
		if (( ulFlags & COLUMNFLAG_DONTSHOWHEADER ) == false )
			sc.ScriptError( "You can't remove the 'DONTSHOWHEADER' flag from column '%s' while it's inside a composite column.", GetInternalName( ));

		if ( Alignment != HORIZALIGN_LEFT )
			sc.ScriptError( "You can't change the alignment of column '%s' while it's inside a composite column.", GetInternalName( ));
	}
}

//*****************************************************************************
//
// [AK] DataScoreColumn::ParseCommand
//
// Parses commands that are only used for data columns.
//
//*****************************************************************************

void DataScoreColumn::ParseCommand( FScanner &sc, const COLUMNCMD_e Command, const FString CommandName )
{
	switch ( Command )
	{
		case COLUMNCMD_MAXLENGTH:
		case COLUMNCMD_PREFIX:
		case COLUMNCMD_SUFFIX:
		{
			// [AK] These commands are only available for text-based columns.
			if ( GetContentType( ) != DATACONTENT_TEXT )
				sc.ScriptError( "Option '%s' is only available for text-based columns.", CommandName.GetChars( ));

			if ( Command == COLUMNCMD_MAXLENGTH )
			{
				// [AK] Since maximum length doesn't apply to integer columns, we should
				// make sure that this command cannot be used for them.
				if ( GetDataType( ) == DATATYPE_INT )
					sc.ScriptError( "Option '%s' cannot be used with integer columns.", CommandName.GetChars( ));

				sc.MustGetNumber( );
				ulMaxLength = MAX( sc.Number, 0 );
			}
			else
			{
				sc.MustGetString( );

				if ( Command == COLUMNCMD_PREFIX )
					PrefixText = sc.String;
				else
					SuffixText = sc.String;
			}

			break;
		}

		case COLUMNCMD_CLIPRECTWIDTH:
		case COLUMNCMD_CLIPRECTHEIGHT:
		{
			// [AK] These commands are only available for graphic-based columns.
			if ( GetContentType( ) != DATACONTENT_GRAPHIC )
				sc.ScriptError( "Option '%s' is only available for graphic-based columns.", CommandName.GetChars( ));

			sc.MustGetNumber( );

			if ( Command == COLUMNCMD_CLIPRECTWIDTH )
				lClipRectWidth = MAX( sc.Number, 0 );
			else
				lClipRectHeight = sc.Number;

			break;
		}

		case COLUMNCMD_TRUETEXT:
		case COLUMNCMD_FALSETEXT:
		{
			// [AK] True and false text are only available for boolean columns.
			if ( GetDataType( ) != DATATYPE_BOOL )
				sc.ScriptError( "Option '%s' is only available for boolean columns.", CommandName.GetChars( ));

			sc.MustGetString( );

			// [AK] If the name begins with a '$', look up the string in the LANGUAGE lump.
			const char *pszString = sc.String[0] == '$' ? GStrings[sc.String] : sc.String;

			if ( Command == COLUMNCMD_TRUETEXT )
				TrueText = pszString;
			else
				FalseText = pszString;

			break;
		}

		// [AK] Parse any generic column commands if we reach here.
		default:
			ScoreColumn::ParseCommand( sc, Command, CommandName );
			break;
	}
}

//*****************************************************************************
//
// [AK] DataScoreColumn::UpdateWidth
//
// Gets the smallest width that will fit the contents in all player rows.
//
//*****************************************************************************

void DataScoreColumn::UpdateWidth( void )
{
	// [AK] Don't update the width of a column that isn't part of a scoreboard.
	if ( pScoreboard == NULL )
		return;

	ulShortestWidth = 0;

	for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
	{
		if ( CanDrawForPlayer( ulIdx ) == false )
			continue;

		PlayerValue Value = GetValue( ulIdx );
		ulShortestWidth = MAX( ulShortestWidth, GetValueWidth( Value ));
	}

	// [AK] Call the superclass's function to finish updating the width.
	ScoreColumn::UpdateWidth( );
}

//*****************************************************************************
//
// [AK] DataScoreColumn::DrawValue
//
// Draws the value of a particular player.
//
//*****************************************************************************

void DataScoreColumn::DrawValue( const ULONG ulPlayer, const ULONG ulColor, const LONG lYPos, const ULONG ulHeight, const float fAlpha ) const
{
	if ( CanDrawForPlayer( ulPlayer ) == false )
		return;

	PlayerValue Value = GetValue( ulPlayer );
	ULONG ulColorToUse;

	// [AK] The text color used in the join queue and vote columns changes depending
	// on whether the player is first in line or the vote caller respectively. Also,
	// the text color in the ping column changes depending on cl_colorizepings.
	if (( NativeType == COLUMNTYPE_PING ) && ( cl_colorizepings ) && ( players[ulPlayer].bIsBot == false ))
	{
		if ( players[ulPlayer].ulPing >= 200 )
			ulColorToUse = CR_RED;
		else if ( players[ulPlayer].ulPing >= 150 )
			ulColorToUse = CR_ORANGE;
		else if ( players[ulPlayer].ulPing >= 100 )
			ulColorToUse = CR_GOLD;
		else
			ulColorToUse = CR_GREEN;
	}
	else if ( NativeType == COLUMNTYPE_JOINQUEUE )
	{
		if ( JOINQUEUE_GetPositionInLine( ulPlayer ) == 0 )
			ulColorToUse = CR_RED;
		else
			ulColorToUse = CR_GOLD;
	}
	else if ( NativeType == COLUMNTYPE_VOTE )
	{
		if ( CALLVOTE_GetVoteCaller( ) == ulPlayer )
			ulColorToUse = CR_RED;
		else
			ulColorToUse = CR_GOLD;
	}
	else
	{
		ulColorToUse = ulColor;
	}

	switch( Value.GetDataType( ))
	{
		case DATATYPE_INT:
		case DATATYPE_BOOL:
		case DATATYPE_FLOAT:
		case DATATYPE_STRING:
			DrawString( GetValueString( Value ).GetChars( ), pScoreboard->pRowFont, ulColorToUse, lYPos, ulHeight, fAlpha );
			break;

		case DATATYPE_COLOR:
			DrawColor( Value.GetValue<PalEntry>( ), lYPos, ulHeight, fAlpha, lClipRectWidth, lClipRectHeight );
			break;

		case DATATYPE_TEXTURE:
			DrawTexture( Value.GetValue<FTexture *>( ), lYPos, ulHeight, fAlpha, lClipRectWidth, lClipRectHeight );
			break;

		default:
			break;
	}
}

//*****************************************************************************
//
// [AK] CountryFlagScoreColumn::CountryFlagScoreColumn
//
// Searches for the "CTRYFLAG" texture and determines the width and height of
// each of the mini flag icons.
//
//*****************************************************************************

CountryFlagScoreColumn::CountryFlagScoreColumn( FScanner &sc, const char *pszName ) : DataScoreColumn( COLUMNTYPE_COUNTRYFLAG, pszName )
{
	pFlagIconSet = TexMan.FindTexture( "CTRYFLAG" );

	// [AK] If "CTRYFLAG" can't be found, then throw a fatal error. We can't use this column without it.
	if ( pFlagIconSet == NULL )
		sc.ScriptError( "Couldn't find texture 'CTRYFLAG'. This lump is required to display country flags." );

	ulFlagWidth = pFlagIconSet->GetScaledWidth( ) / NUM_FLAGS_PER_SIDE;
	ulFlagHeight = pFlagIconSet->GetScaledHeight( ) / NUM_FLAGS_PER_SIDE;

	// [AK] Make sure that all country flags have the same width and height. Otherwise, throw a fatal error.
	if (( ulFlagWidth * NUM_FLAGS_PER_SIDE != static_cast<ULONG>( pFlagIconSet->GetScaledWidth( ))) ||
		( ulFlagHeight * NUM_FLAGS_PER_SIDE != static_cast<ULONG>( pFlagIconSet->GetScaledHeight( ))))
	{
		sc.ScriptError( "The texture 'CTRYFLAG' cannot be accepted. All country flag icons don't have the same width and height." );
	}
}

//*****************************************************************************
//
// [AK] CountryFlagScoreColumn::GetValueWidth
//
// This should always return the width of a mini flag icon, assuming that the
// passed value is a texture set to "CTRYFLAG".
//
//*****************************************************************************

ULONG CountryFlagScoreColumn::GetValueWidth( const PlayerValue &Value ) const
{
	// [AK] Always return zero if this column isn't part of a scoreboard.
	if ( pScoreboard != NULL )
	{
		if (( Value.GetDataType( ) == DATATYPE_TEXTURE ) && ( Value.GetValue<FTexture *>( ) == pFlagIconSet ))
			return ulFlagWidth;

		// [AK] If we somehow end up here, throw a fatal error.
		I_Error( "CountryFlagScoreColumn::GetValueWidth: tried to get the width of a value that isn't 'CTRYFLAG'!" );
	}

	return 0;
}

//*****************************************************************************
//
// [AK] CountryFlagScoreColumn::GetValue
//
// Always returns "CTRYFLAG", so as long as the player is valid.
//
//*****************************************************************************

PlayerValue CountryFlagScoreColumn::GetValue( const ULONG ulPlayer ) const
{
	PlayerValue result;

	if ( PLAYER_IsValidPlayer( ulPlayer ))
		result.SetValue<FTexture *>( pFlagIconSet );

	return result;
}

//*****************************************************************************
//
// [AK] CountryFlagScoreColumn::DrawValue
//
// Draws a mini flag icon of the player's country. To do this, the "CTRYFLAG"
// texture is shifted horizontally and vertically such that, when combined with
// the clipping rectangle, the correct country flag is drawn.
//
//*****************************************************************************

void CountryFlagScoreColumn::DrawValue( const ULONG ulPlayer, const ULONG ulColor, const LONG lYPos, const ULONG ulHeight, const float fAlpha ) const
{
	if ( CanDrawForPlayer( ulPlayer ) == false )
		return;

	if (( pFlagIconSet != NULL ) && ( players[ulPlayer].ulCountryIndex <= COUNTRYINDEX_LAN ))
	{
		const int leftOffset = ( players[ulPlayer].ulCountryIndex % NUM_FLAGS_PER_SIDE ) * ulFlagWidth;
		const int topOffset = ( players[ulPlayer].ulCountryIndex / NUM_FLAGS_PER_SIDE ) * ulFlagHeight;

		LONG lXPos = GetAlignmentPosition( ulFlagWidth );
		LONG lNewYPos = lYPos + ( ulHeight - ulFlagHeight ) / 2;

		int clipLeft = lXPos;
		int clipWidth = ulFlagWidth;
		int clipTop = lNewYPos;
		int clipHeight = ulFlagHeight;

		// [AK] We must take into account the virtual screen's size.
		if ( g_bScale )
			screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidth, clipHeight, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

		screen->DrawTexture( pFlagIconSet, lXPos, lNewYPos,
			DTA_UseVirtualScreen, g_bScale,
			DTA_ClipLeft, clipLeft,
			DTA_ClipRight, clipLeft + clipWidth,
			DTA_ClipTop, clipTop,
			DTA_ClipBottom, clipTop + clipHeight,
			DTA_LeftOffset, leftOffset,
			DTA_TopOffset, topOffset,
			DTA_Alpha, FLOAT2FIXED( fAlpha ),
			TAG_DONE );
	}
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::ParseCommand
//
// Parses commands that are only used for composite columns.
//
//*****************************************************************************

void CompositeScoreColumn::ParseCommand( FScanner &sc, const COLUMNCMD_e Command, const FString CommandName )
{
	switch ( Command )
	{
		case COLUMNCMD_GAPBETWEENCOLUMNS:
		{
			sc.MustGetNumber( );
			ulGapBetweenSubColumns = MAX( sc.Number, 0 );
			break;
		}

		case COLUMNCMD_COLUMNS:
		case COLUMNCMD_ADDTOCOLUMNS:
		{
			if ( Command == COLUMNCMD_COLUMNS )
				ClearSubColumns( );

			do
			{
				// [AK] Make sure that the next column we scan is a data column.
				DataScoreColumn *pDataColumn = static_cast<DataScoreColumn *>( scoreboard_ScanForColumn( sc, true ));
				CompositeScoreColumn *pCompositeColumn = pDataColumn->GetCompositeColumn( );

				// [AK] Don't add a data column that's already inside another composite column.
				if (( pCompositeColumn != NULL ) && ( pCompositeColumn != this ))
					sc.ScriptError( "You can't put column '%s' into composite column '%s' when it's already inside '%s'.", sc.String, GetInternalName( ), pCompositeColumn->GetInternalName( ));

				// [AK] Don't add a data column that's already inside a scoreboard's column order.
				if ( pDataColumn->GetScoreboard( ) != NULL )
					sc.ScriptError( "You can't put column '%s' into composite column '%s' when it's already inside a scoreboard's column order.", sc.String, GetInternalName( ));

				// [AK] All data columns require the DONTSHOWHEADER flag to be enabled to be inside a composite column.
				if (( pDataColumn->GetFlags( ) & COLUMNFLAG_DONTSHOWHEADER ) == false )
					sc.ScriptError( "Column '%s' must have 'DONTSHOWHEADER' enabled before it can be put inside a composite column.", sc.String );

				// [AK] All data columns must be alignment to the left to be inside a composite column.
				if ( pDataColumn->Alignment != HORIZALIGN_LEFT )
					sc.ScriptError( "Column '%s' must be aligned to the left before it can be put inside a composite column.", sc.String );

				if ( scoreboard_TryPushingColumnToList( sc, SubColumns, pDataColumn ))
				{
					pDataColumn->pCompositeColumn = this;

					if ( pScoreboard != NULL )
						pDataColumn->SetScoreboard( pScoreboard );
				}

			} while ( sc.CheckToken( ',' ));

			// [AK] Any data columns no longer in the sub-column list must be removed from the scoreboard's rank order.
			if (( Command == COLUMNCMD_COLUMNS ) && ( pScoreboard != NULL ))
				pScoreboard->RemoveInvalidColumnsInRankOrder( );

			break;
		}

		case COLUMNCMD_REMOVEFROMCOLUMNS:
		{
			do
			{
				DataScoreColumn *pDataColumn = static_cast<DataScoreColumn *>( scoreboard_ScanForColumn( sc, true ));

				if ( scoreboard_TryRemovingColumnFromList( sc, SubColumns, pDataColumn ))
				{
					pDataColumn->pCompositeColumn = NULL;

					if ( pScoreboard != NULL )
						pDataColumn->SetScoreboard( NULL );
				}

			} while ( sc.CheckToken( ',' ));

			// [AK] Any data columns removed from the sub-column list must be removed from the scoreboard's rank order.
			if ( pScoreboard != NULL )
				pScoreboard->RemoveInvalidColumnsInRankOrder( );

			break;
		}

		// [AK] Parse any generic column commands if we reach here.
		default:
			ScoreColumn::ParseCommand( sc, Command, CommandName );
			break;
	}
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::CheckIfUsable
//
// Checks if the composite column and its sub-columns are usable in the current
// game. Refer to ScoreColumn::CheckIfUsable for more information.
//
//*****************************************************************************

void CompositeScoreColumn::CheckIfUsable( void )
{
	// [AK] If the composite column isn't part of a scoreboard, then stop here.
	if ( pScoreboard == NULL )
		return;

	// [AK] Call the superclass's function first.
	ScoreColumn::CheckIfUsable( );

	// [AK] If the composite column is usable, then check the sub-columns too.
	if ( bUsableInCurrentGame )
	{
		for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
			SubColumns[i]->CheckIfUsable( );
	}
	// [AK] Otherwise, mark the sub-columns as unusable too.
	else
	{
		for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
			SubColumns[i]->bUsableInCurrentGame = false;
	}
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::Refresh
//
// Refreshes the composite column and its sub-columns.
//
//*****************************************************************************

void CompositeScoreColumn::Refresh( void )
{
	// [AK] Call the superclass's refresh function first.
	ScoreColumn::Refresh( );

	// [AK] If the composite column isn't disabled, then refresh the sub-columns.
	if ( bDisabled == false )
	{
		for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
			SubColumns[i]->Refresh( );
	}
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::UpdateWidth
//
// Gets the smallest width that can fit the contents of all active sub-columns
// in all player rows.
//
//*****************************************************************************

void CompositeScoreColumn::UpdateWidth( void )
{
	// [AK] Don't update the width of a column that isn't part of a scoreboard.
	if ( pScoreboard == NULL )
		return;

	ulShortestWidth = 0;

	for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
	{
		if ( CanDrawForPlayer( ulIdx ) == false )
			continue;

		ulShortestWidth = MAX( ulShortestWidth, GetRowWidth( ulIdx ));
	}

	// [AK] Call the superclass's function to finish updating the width.
	ScoreColumn::UpdateWidth( );
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::DrawValue
//
// Draws the values of a particular player from all active sub-columns.
//
//*****************************************************************************

void CompositeScoreColumn::DrawValue( const ULONG ulPlayer, const ULONG ulColor, const LONG lYPos, const ULONG ulHeight, const float fAlpha ) const
{
	PlayerValue Value;

	if ( CanDrawForPlayer( ulPlayer ) == false )
		return;

	const bool bIsTrueSpectator = PLAYER_IsTrueSpectator( &players[ulPlayer] );
	const ULONG ulRowWidth = GetRowWidth( ulPlayer );

	// [AK] If this row's width is zero, then there's nothing to draw, so stop here.
	if ( ulRowWidth == 0 )
		return;

	// [AK] Determine at what position we should start drawing the contents.
	LONG lXPos = GetAlignmentPosition( ulRowWidth );

	// [AK] Draw the contents of the sub-columns!
	for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
	{
		if (( SubColumns[i]->IsDisabled( )) || (( SubColumns[i]->GetFlags( ) & COLUMNFLAG_NOSPECTATORS ) && ( bIsTrueSpectator )))
			continue;

		Value = SubColumns[i]->GetValue( ulPlayer );

		if (( Value.GetDataType( ) != DATATYPE_UNKNOWN ) || (( SubColumns[i]->GetFlags( ) & COLUMNFLAG_DISABLEIFEMPTY ) == false ))
		{
			const ULONG ulValueWidth = SubColumns[i]->GetValueWidth( Value );

			// [AK] We didn't update the sub-column's x-position or width since they're part of
			// a composite column, but we need to make sure that the contents appear properly.
			// (i.e. Scoreboard::DrawString and Scoreboard::DrawTexture use these members to
			// form the clipping rectangle's boundaries). What we'll do is temporarily set the
			// members to what they need to be now, draw the value, then set them back to zero.
			if ( Value.GetDataType( ) != DATATYPE_UNKNOWN )
			{
				SubColumns[i]->lRelX = lXPos;
				SubColumns[i]->ulWidth = ulValueWidth;
				SubColumns[i]->DrawValue( ulPlayer, ulColor, lYPos, ulHeight, fAlpha );

				SubColumns[i]->lRelX = SubColumns[i]->ulWidth = 0;
			}

			lXPos += GetSubColumnWidth( i, ulValueWidth ) + ulGapBetweenSubColumns;
		}
	}
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::SetScoreboard
//
// Assigns every data column in the composite column's sub-column list the same
// pointer to the scoreboard that the composite column is setting to.
//
//*****************************************************************************

void CompositeScoreColumn::SetScoreboard( Scoreboard *pScoreboard )
{
	ScoreColumn::SetScoreboard( pScoreboard );

	for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
		SubColumns[i]->SetScoreboard( pScoreboard );
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::ClearSubColumns
//
// Empties the composite column's sub-column list. This also removes any
// references the sub-columns have to the composite column and scoreboard.
//
//*****************************************************************************

void CompositeScoreColumn::ClearSubColumns( void )
{
	for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
	{
		SubColumns[i]->pCompositeColumn = NULL;

		if ( pScoreboard != NULL )
			SubColumns[i]->pScoreboard = NULL;
	}

	SubColumns.Clear( );
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::GetRowWidth
//
// Gets the width of an entire row for a particular player.
//
//*****************************************************************************

ULONG CompositeScoreColumn::GetRowWidth( const ULONG ulPlayer ) const
{
	if (( pScoreboard == NULL ) || ( PLAYER_IsValidPlayer( ulPlayer ) == false ))
		return 0;

	const bool bIsTrueSpectator = PLAYER_IsTrueSpectator( &players[ulPlayer] );
	ULONG ulRowWidth = 0;

	for ( unsigned int i = 0; i < SubColumns.Size( ); i++ )
	{
		// [AK] Ignore sub-columns that are disabled or cannot be shown for true spectators.
		if (( SubColumns[i]->IsDisabled( )) || (( SubColumns[i]->GetFlags( ) & COLUMNFLAG_NOSPECTATORS ) && ( bIsTrueSpectator )))
			continue;

		PlayerValue Value = SubColumns[i]->GetValue( ulPlayer );

		if (( Value.GetDataType( ) != DATATYPE_UNKNOWN ) || (( SubColumns[i]->GetFlags( ) & COLUMNFLAG_DISABLEIFEMPTY ) == false ))
		{
			// [AK] Include the gap between sub-columns if the width is already non-zero.
			if ( ulRowWidth > 0 )
				ulRowWidth += ulGapBetweenSubColumns;

			ulRowWidth += GetSubColumnWidth( i, SubColumns[i]->GetValueWidth( Value ));
		}
	}

	return ulRowWidth;
}

//*****************************************************************************
//
// [AK] CompositeScoreColumn::GetSubColumnWidth
//
// Gets the width of a sub-column. This requires that the width of the value be
// determined first (using DataScoreColumn::GetValueWidth) and passed into this
// function to work.
//
//*****************************************************************************

ULONG CompositeScoreColumn::GetSubColumnWidth( const ULONG ulSubColumn, const ULONG ulValueWidth ) const
{
	if (( pScoreboard == NULL ) || ( ulSubColumn >= SubColumns.Size( )))
		return 0;

	// [AK] If the sub-column always uses its shortest width, then sizing is treated as padding.
	if ( SubColumns[ulSubColumn]->GetFlags( ) & COLUMNFLAG_ALWAYSUSESHORTESTWIDTH )
		return ulValueWidth + SubColumns[ulSubColumn]->GetSizing( );
	// [AK] Otherwise, the sizing is treated as the default width of the sub-column.
	// Use whichever's bigger: the value's width or the sizing.
	else
		return MAX( SubColumns[ulSubColumn]->GetSizing( ), ulValueWidth );
}

//*****************************************************************************
//
// [AK] Scoreboard::Scoreboard
//
// Initializes the members of a Scoreboard object to their default values.
//
//*****************************************************************************

Scoreboard::Scoreboard( void ) :
	lRelX( 0 ),
	lRelY( 0 ),
	ulWidth( 0 ),
	ulHeight( 0 ),
	ulFlags( 0 ),
	pHeaderFont( NULL ),
	pRowFont( NULL ),
	HeaderColor( CR_UNTRANSLATED ),
	RowColor( CR_UNTRANSLATED ),
	LocalRowColors{ CR_UNTRANSLATED },
	pBorderTexture( NULL ),
	BorderColors{ CR_UNTRANSLATED },
	BackgroundColor( 0 ),
	RowBackgroundColors{ 0 },
	TeamRowBackgroundColors{ { 0 } },
	fBackgroundAmount( 0.0f ),
	fRowBackgroundAmount( 0.0f ),
	fDeadRowBackgroundAmount( 0.0f ),
	fDeadTextAlpha( 0.0f ),
	ulBackgroundBorderSize( 0 ),
	ulGapBetweenHeaderAndRows( 0 ),
	ulGapBetweenColumns( 0 ),
	ulGapBetweenRows( 0 ),
	ulColumnPadding( 0 ),
	lHeaderHeight( 0 ),
	lRowHeight( 0 ),
	MainHeader( MARGINTYPE_HEADER_OR_FOOTER, "MainHeader" ),
	TeamHeader( MARGINTYPE_TEAM, "TeamHeader" ),
	SpectatorHeader( MARGINTYPE_SPECTATOR, "SpectatorHeader" ),
	Footer( MARGINTYPE_HEADER_OR_FOOTER, "Footer" ),
	lLastRefreshTick( 0 ) { }

//*****************************************************************************
//
// [AK] Scoreboard::Parse
//
// Parses a "scoreboard" block in a SCORINFO lump.
//
//*****************************************************************************

int scoreboard_GetLuminance( const int r, const int g, const int b )
{
	return static_cast<int>( 0.3f * r + 0.59f * g + 0.11f * b );
}

//*****************************************************************************
//
int scoreboard_GetLuminance( const PalEntry color )
{
	return scoreboard_GetLuminance( color.r, color.g, color.b );
}

//*****************************************************************************
//
void Scoreboard::Parse( FScanner &sc )
{
	sc.MustGetToken( '{' );

	while ( sc.CheckToken( '}' ) == false )
	{
		sc.MustGetString( );

		if ( stricmp( sc.String, "mainheader" ) == 0 )
		{
			MainHeader.Parse( sc );
		}
		else if ( stricmp( sc.String, "teamheader" ) == 0 )
		{
			TeamHeader.Parse( sc );
		}
		else if ( stricmp( sc.String, "spectatorheader" ) == 0 )
		{
			SpectatorHeader.Parse( sc );
		}
		else if ( stricmp( sc.String, "footer" ) == 0 )
		{
			Footer.Parse( sc );
		}
		else if ( stricmp( sc.String, "addflag" ) == 0 )
		{
			ulFlags |= sc.MustGetEnumName( "scoreboard flag", "SCOREBOARDFLAG_", GetValueSCOREBOARDFLAG_e );
		}
		else if ( stricmp( sc.String, "removeflag" ) == 0 )
		{
			ulFlags &= ~sc.MustGetEnumName( "scoreboard flag", "SCOREBOARDFLAG_", GetValueSCOREBOARDFLAG_e );
		}
		else
		{
			SCOREBOARDCMD_e Command = static_cast<SCOREBOARDCMD_e>( sc.MustGetEnumName( "scoreboard command", "SCOREBOARDCMD_", GetValueSCOREBOARDCMD_e, true ));
			FString CommandName = sc.String;

			sc.MustGetToken( '=' );

			switch ( Command )
			{
				case SCOREBOARDCMD_BORDERTEXTURE:
				{
					sc.MustGetString( );
					pBorderTexture = TexMan.FindTexture( sc.String );

					// [AK] If the texture wasn't found, throw a fatal error.
					if ( pBorderTexture == NULL )
						sc.ScriptError( "Couldn't find border texture '%s'.", sc.String );

					break;
				}

				case SCOREBOARDCMD_HEADERFONT:
				case SCOREBOARDCMD_ROWFONT:
				{
					sc.MustGetToken( TK_StringConst );

					// [AK] Throw a fatal error if an empty string was passed.
					if ( sc.StringLen == 0 )
						sc.ScriptError( "Got an empty string for a font name." );

					FFont *pFont = V_GetFont( sc.String );

					// [AK] If the font was invalid, throw a fatal error.
					if ( pFont == NULL )
						sc.ScriptError( "Couldn't find font '%s'.", sc.String );

					if ( Command == SCOREBOARDCMD_HEADERFONT )
						pHeaderFont = pFont;
					else
						pRowFont = pFont;

					break;
				}

				case SCOREBOARDCMD_HEADERCOLOR:
				case SCOREBOARDCMD_ROWCOLOR:
				case SCOREBOARDCMD_LOCALROWCOLOR:
				case SCOREBOARDCMD_LOCALROWDEMOCOLOR:
				{
					sc.MustGetToken( TK_StringConst );
					EColorRange color;

					// [AK] If an empty string was passed, inform the user of the error and switch to untranslated.
					if ( sc.StringLen == 0 )
					{
						sc.ScriptMessage( "Got an empty string for a text color, using untranslated instead." );
						color = CR_UNTRANSLATED;
					}
					else
					{
						color = V_FindFontColor( sc.String );

						// [AK] If the text color name was invalid, let the user know about it.
						if (( color == CR_UNTRANSLATED ) && ( stricmp( sc.String, "untranslated" ) != 0 ))
							sc.ScriptMessage( "'%s' is an unknown text color, using untranslated instead.", sc.String );
					}

					if ( Command == SCOREBOARDCMD_HEADERCOLOR )
						HeaderColor = color;
					else if ( Command == SCOREBOARDCMD_ROWCOLOR )
						RowColor = color;
					else if ( Command == SCOREBOARDCMD_LOCALROWCOLOR )
						LocalRowColors[LOCALROW_COLOR_INGAME] = color;
					else
						LocalRowColors[LOCALROW_COLOR_INDEMO] = color;

					break;
				}

				case SCOREBOARDCMD_DEADPLAYERTEXTALPHA:
				case SCOREBOARDCMD_BACKGROUNDAMOUNT:
				case SCOREBOARDCMD_ROWBACKGROUNDAMOUNT:
				case SCOREBOARDCMD_DEADPLAYERROWBACKGROUNDAMOUNT:
				{
					sc.MustGetFloat( );
					const float fClampedValue = clamp( static_cast<float>( sc.Float ), 0.0f, 1.0f );

					if ( Command == SCOREBOARDCMD_DEADPLAYERTEXTALPHA )
						fDeadTextAlpha = fClampedValue;
					else if ( Command == SCOREBOARDCMD_BACKGROUNDAMOUNT )
						fBackgroundAmount = fClampedValue;
					else if ( Command == SCOREBOARDCMD_ROWBACKGROUNDAMOUNT )
						fRowBackgroundAmount = fClampedValue;
					else
						fDeadRowBackgroundAmount = fClampedValue;

					break;
				}

				case SCOREBOARDCMD_LIGHTBORDERCOLOR:
				case SCOREBOARDCMD_DARKBORDERCOLOR:
				case SCOREBOARDCMD_BACKGROUNDCOLOR:
				case SCOREBOARDCMD_LIGHTROWBACKGROUNDCOLOR:
				case SCOREBOARDCMD_DARKROWBACKGROUNDCOLOR:
				case SCOREBOARDCMD_LOCALROWBACKGROUNDCOLOR:
				{
					sc.MustGetToken( TK_StringConst );

					// [AK] If an empty string was passed, inform the user about the error.
					// This doesn't have to be a fatal error since the color will just be black anyways.
					if ( sc.StringLen == 0 )
						sc.ScriptMessage( "Got an empty string for a color." );

					FString ColorString = V_GetColorStringByName( sc.String );
					PalEntry color = V_GetColorFromString( NULL, ColorString.IsNotEmpty( ) ? ColorString.GetChars( ) : sc.String );

					if ( Command == SCOREBOARDCMD_LIGHTBORDERCOLOR )
						BorderColors[BORDER_COLOR_LIGHT] = color;
					else if ( Command == SCOREBOARDCMD_DARKBORDERCOLOR )
						BorderColors[BORDER_COLOR_DARK] = color;
					else if ( Command == SCOREBOARDCMD_BACKGROUNDCOLOR )
						BackgroundColor = color;
					else if ( Command == SCOREBOARDCMD_LIGHTROWBACKGROUNDCOLOR )
						RowBackgroundColors[ROWBACKGROUND_COLOR_LIGHT] = color;
					else if ( Command == SCOREBOARDCMD_DARKROWBACKGROUNDCOLOR )
						RowBackgroundColors[ROWBACKGROUND_COLOR_DARK] = color;
					else
						RowBackgroundColors[ROWBACKGROUND_COLOR_LOCAL] = color;

					break;
				}

				case SCOREBOARDCMD_BACKGROUNDBORDERSIZE:
				case SCOREBOARDCMD_GAPBETWEENHEADERANDROWS:
				case SCOREBOARDCMD_GAPBETWEENCOLUMNS:
				case SCOREBOARDCMD_GAPBETWEENROWS:
				case SCOREBOARDCMD_COLUMNPADDING:
				{
					sc.MustGetNumber( );
					const ULONG ulCappedValue = MAX( sc.Number, 0 );

					if ( Command == SCOREBOARDCMD_BACKGROUNDBORDERSIZE )
						ulBackgroundBorderSize = ulCappedValue;
					else if ( Command == SCOREBOARDCMD_GAPBETWEENHEADERANDROWS )
						ulGapBetweenHeaderAndRows = ulCappedValue;
					else if ( Command == SCOREBOARDCMD_GAPBETWEENCOLUMNS )
						ulGapBetweenColumns = ulCappedValue;
					else if ( Command == SCOREBOARDCMD_GAPBETWEENROWS )
						ulGapBetweenRows = ulCappedValue;
					else
						ulColumnPadding = ulCappedValue;

					break;
				}

				case SCOREBOARDCMD_HEADERHEIGHT:
				case SCOREBOARDCMD_ROWHEIGHT:
				{
					sc.MustGetNumber( );

					if ( Command == SCOREBOARDCMD_HEADERHEIGHT )
						lHeaderHeight = sc.Number;
					else
						lRowHeight = sc.Number;

					break;
				}

				case SCOREBOARDCMD_COLUMNORDER:
				case SCOREBOARDCMD_ADDTOCOLUMNORDER:
				case SCOREBOARDCMD_RANKORDER:
				case SCOREBOARDCMD_ADDTORANKORDER:
				{
					const bool bAddToRankOrder = (( Command == SCOREBOARDCMD_RANKORDER ) || ( Command == SCOREBOARDCMD_ADDTORANKORDER ));

					// [AK] Clear the list before adding the new columns to it.
					if ( Command == SCOREBOARDCMD_COLUMNORDER )
					{
						TMapIterator<FName, ScoreColumn *> it( g_Columns );
						TMap<FName, ScoreColumn *>::Pair *pair;

						while ( it.NextPair( pair ))
						{
							if ( pair->Value->GetScoreboard( ) == this )
								pair->Value->pScoreboard = NULL;
						}

						ColumnOrder.Clear( );
					}
					else if ( Command == SCOREBOARDCMD_RANKORDER )
					{
						RankOrder.Clear( );
					}

					do
					{
						AddColumnToList( sc, bAddToRankOrder );
					} while ( sc.CheckToken( ',' ));

					// [AK] Any columns that aren't in the column order anymore must be removed from the rank order.
					if ( Command == SCOREBOARDCMD_COLUMNORDER )
						RemoveInvalidColumnsInRankOrder( );

					break;
				}

				case SCOREBOARDCMD_REMOVEFROMCOLUMNORDER:
				case SCOREBOARDCMD_REMOVEFROMRANKORDER:
				{
					const bool bRemoveFromRankOrder = ( Command == SCOREBOARDCMD_REMOVEFROMRANKORDER );

					do
					{
						RemoveColumnFromList( sc, bRemoveFromRankOrder );
					} while ( sc.CheckToken( ',' ));

					// [AK] Any columns removed from the column order must also be removed from the rank order.
					if ( Command == SCOREBOARDCMD_REMOVEFROMCOLUMNORDER )
						RemoveInvalidColumnsInRankOrder( );

					break;
				}

				default:
					sc.ScriptError( "Couldn't process scoreboard command '%s'.", CommandName.GetChars( ));
					break;
			}
		}
	}

	if ( pHeaderFont == NULL )
		sc.ScriptError( "There's no header font for the scoreboard." );

	if ( pRowFont == NULL )
		sc.ScriptError( "There's no row font for the scoreboard." );

	// [AK] A negative header or row height means setting the height with respect to the
	// height of the header or row font's respectively, if valid.
	if ( lHeaderHeight <= 0 )
		lHeaderHeight = pHeaderFont->GetHeight( ) - lHeaderHeight;

	if ( lRowHeight <= 0 )
		lRowHeight = pRowFont->GetHeight( ) - lRowHeight;

	// [AK] Generate row background colors for each team through color blending.
	// This uses the color blend mode explained in section 7.2.4, "Blend Mode", in
	// "PDF Reference" fifth edition, version 1.6.
	for ( ULONG ulTeam = 0; ulTeam < teams.Size( ); ulTeam++ )
	{
		const PalEntry TeamColor = teams[ulTeam].lPlayerColor;

		for ( unsigned int i = 0; i < NUM_ROWBACKGROUND_COLORS; i++ )
		{
			const int delta = scoreboard_GetLuminance( RowBackgroundColors[i] ) - scoreboard_GetLuminance( TeamColor );

			int rgb[3] = { TeamColor.r + delta, TeamColor.g + delta, TeamColor.b + delta };

			const int luminosity = scoreboard_GetLuminance( rgb[0], rgb[1], rgb[2] );
			const int minColor = MIN( MIN( rgb[0], rgb[1] ), rgb[2] );
			const int maxColor = MAX( MAX( rgb[0], rgb[1] ), rgb[2] );

			if ( minColor < 0 )
			{
				for ( unsigned int i = 0; i < 3; i++ )
					rgb[i] = luminosity + ((( rgb[i] - luminosity ) * luminosity ) / ( luminosity - minColor ));
			}

			if ( maxColor > UCHAR_MAX )
			{
				for ( unsigned int i = 0; i < 3; i++ )
					rgb[i] = luminosity + ((( rgb[i] - luminosity ) * ( UCHAR_MAX - luminosity )) / ( maxColor - luminosity ));
			}

			TeamRowBackgroundColors[ulTeam][i] = MAKERGB( rgb[0], rgb[1], rgb[2] );
		}
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::AddColumnToList
//
// This is intended to be called only when parsing scoreboard commands like
// "ColumnOrder", "RankOrder", "AddToColumnOrder", and "AddToRankOrder". It
// scans for a string, finds a column using that string, and adds it to the
// rank or column order lists (depending on the value of bAddToRankOrder).
//
//*****************************************************************************

void Scoreboard::AddColumnToList( FScanner &sc, const bool bAddToRankOrder )
{
	// [AK] Note that if we're adding a column to the rank order, then it must be a data column.
	ScoreColumn *pColumn = scoreboard_ScanForColumn( sc, bAddToRankOrder );
	const char *pszColumnName = pColumn->GetInternalName( );

	CompositeScoreColumn *pCompositeColumn = NULL;

	if ( bAddToRankOrder )
	{
		// [AK] Double-check that this is a data column. Otherwise, throw a fatal error.
		if ( pColumn->GetTemplate( ) != COLUMNTEMPLATE_DATA )
			sc.ScriptError( "Column '%s' is not a data column.", pszColumnName );

		DataScoreColumn *pDataColumn = static_cast<DataScoreColumn *>( pColumn );

		// [AK] Columns must be inside the scoreboard's column order first before they're
		// added to the rank order list. If this column is inside a composite column, then
		// the composite column needs to be in the column order instead.
		if ( pColumn->GetScoreboard( ) != this )
		{
			pCompositeColumn = pDataColumn->GetCompositeColumn( );

			if ( pCompositeColumn == NULL )
				sc.ScriptError( "Column '%s' must be added to the column order before added to the rank order.", pszColumnName );
			else
				sc.ScriptError( "Column '%s' is inside composite column '%s', which must be added to the column order first.", pszColumnName, pCompositeColumn->GetInternalName( ));
		}

		scoreboard_TryPushingColumnToList( sc, RankOrder, pDataColumn );
	}
	else
	{
		// [AK] If this is a data column, make sure that it isn't inside a composite column.
		// The composite column must be added to the list instead.
		if ( pColumn->GetTemplate( ) == COLUMNTEMPLATE_DATA )
		{
			pCompositeColumn = static_cast<DataScoreColumn *>( pColumn )->GetCompositeColumn( );

			if ( pCompositeColumn != NULL )
				sc.ScriptError( "Column '%s' is already inside composite column '%s' and can't be added to the column order.", pszColumnName, pCompositeColumn->GetInternalName( ));
		}

		if ( scoreboard_TryPushingColumnToList( sc, ColumnOrder, pColumn ))
			pColumn->SetScoreboard( this );
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::RemoveColumnFromList
//
// When scoreboard commands like "RemoveFromColumnOrder" or "RemoveFromRankOrder"
// are parsed, this will scan for a string, find a column, and then remove it
// from the rank or column order lists.
//
//*****************************************************************************

void Scoreboard::RemoveColumnFromList( FScanner &sc, const bool bRemoveFromRankOrder )
{
	// [AK] A column must be a data column to be removed from the rank order.
	ScoreColumn *pColumn = scoreboard_ScanForColumn( sc, bRemoveFromRankOrder );

	if ( bRemoveFromRankOrder )
	{
		// [AK] Double-check that this is a data column. Otherwise, throw a fatal error.
		if ( pColumn->GetTemplate( ) != COLUMNTEMPLATE_DATA )
			sc.ScriptError( "Column '%s' is not a data column.", pColumn->GetInternalName( ));

		scoreboard_TryRemovingColumnFromList( sc, RankOrder, static_cast<DataScoreColumn *>( pColumn ));
	}
	else if ( scoreboard_TryRemovingColumnFromList( sc, ColumnOrder, pColumn ))
	{
		pColumn->SetScoreboard( NULL );
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::PlayerComparator
//
// Orders players on the scoreboard, from top to bottom, using the rank order list.
//
//*****************************************************************************

bool Scoreboard::PlayerComparator::operator( )( const int &arg1, const int &arg2 ) const
{
	int result = 0;

	// [AK] Sanity check: make sure that we're pointing to a scoreboard.
	if ( pScoreboard == NULL )
		return false;

	// [AK] Always return false if the first player index is invalid,
	// or true if the second player index is invalid.
	if ( PLAYER_IsValidPlayer( arg1 ) == false )
		return false;
	else if ( PLAYER_IsValidPlayer( arg2 ) == false )
		return true;

	// [AK] Always return false if the first player is a true spectator,
	// or true if the second player is a true spectator.
	if ( PLAYER_IsTrueSpectator( &players[arg1] ))
		return false;
	else if ( PLAYER_IsTrueSpectator( &players[arg2] ))
		return true;

	// [AK] In team-based game modes, order players by team. Players with lower
	// team indices should come before those with higher indices.
	if ( pScoreboard->ShouldSeparateTeams( ))
	{
		result = players[arg1].Team - players[arg2].Team;

		if ( result != 0 )
			return ( result < 0 );
	}

	for ( unsigned int i = 0; i < pScoreboard->RankOrder.Size( ); i++ )
	{
		if ( pScoreboard->RankOrder[i]->IsDisabled( ))
			continue;

		const PlayerValue Value1 = pScoreboard->RankOrder[i]->GetValue( arg1 );
		const PlayerValue Value2 = pScoreboard->RankOrder[i]->GetValue( arg2 );

		// [AK] Always return false if the data type of the first value is unknown.
		// This is also the case when both values have unknown data types.
		if ( Value1.GetDataType( ) == DATATYPE_UNKNOWN )
			return false;

		// [AK] Always return true if the second value is unknown.
		if ( Value2.GetDataType( ) == DATATYPE_UNKNOWN )
			return true;

		switch ( Value1.GetDataType( ))
		{
			case DATATYPE_INT:
				result = Value1.GetValue<int>( ) - Value2.GetValue<int>( );
				break;

			case DATATYPE_BOOL:
				result = static_cast<int>( Value1.GetValue<bool>( ) - Value2.GetValue<bool>( ));
				break;

			case DATATYPE_FLOAT:
				result = static_cast<int>( Value1.GetValue<float>( ) - Value2.GetValue<float>( ));
				break;

			case DATATYPE_STRING:
			{
				FString firstString = Value1.GetValue<const char *>( );
				FString secondString = Value2.GetValue<const char *>( );

				// [AK] Remove color codes from both strings before comparing them.
				V_RemoveColorCodes( firstString );
				V_RemoveColorCodes( secondString );

				result = secondString.Compare( firstString );
				break;
			}

			default:
				break;
		}

		// [AK] If the values for this column aren't the same for both players, return the result.
		if ( result != 0 )
			return ( pScoreboard->RankOrder[i]->GetFlags( ) & COLUMNFLAG_REVERSEORDER ) ? ( result < 0 ) : ( result > 0 );
	}

	return false;
}

//*****************************************************************************
//
// [AK] Scoreboard::Refresh
//
// Updates the scoreboard's width and height, re-positions the columns, and
// sorts the players using the rank order.
//
//*****************************************************************************

void Scoreboard::Refresh( const ULONG ulDisplayPlayer )
{
	// [AK] Refresh all of the scoreboard's columns, then update the widths of any active columns.
	for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
	{
		ColumnOrder[i]->Refresh( );

		if ( ColumnOrder[i]->IsDisabled( ))
			continue;

		ColumnOrder[i]->UpdateWidth( );
	}

	UpdateWidth( );

	// [AK] If the scoreboard's width is zero, then stop here.
	if ( ulWidth == 0 )
		return;

	UpdateHeight( ulDisplayPlayer );

	// [AK] Reset the player list then sort players based on the scoreboard's rank order.
	for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
		ulPlayerList[ulIdx] = ulIdx;

	std::stable_sort( ulPlayerList, ulPlayerList + MAXPLAYERS, PlayerComparator( this ));
}

//*****************************************************************************
//
// [AK] Scoreboard::UpdateWidth
//
// Determines what the width of the scoreboard should be right now, then
// re-positions all of the active columns based on that width.
//
//*****************************************************************************

void Scoreboard::UpdateWidth( void )
{
	const ULONG ulGameModeFlags = GAMEMODE_GetCurrentFlags( );
	ULONG ulNumActiveColumns = 0;
	ULONG ulShortestWidthOfAllColumns = 0;

	ulWidth = 0;

	for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
	{
		if ( ColumnOrder[i]->IsDisabled( ))
			continue;

		ulWidth += ColumnOrder[i]->GetWidth( );
		ulShortestWidthOfAllColumns += ColumnOrder[i]->GetShortestWidth( );
		ulNumActiveColumns++;
	}

	// [AK] If the width is still zero, then no columns are visible, stop here.
	if ( ulWidth == 0 )
		return;

	const ULONG ulExtraSpace = ( ulNumActiveColumns - 1 ) * ulGapBetweenColumns + ( 2 * ulColumnPadding ) * ulNumActiveColumns + 2 * ulBackgroundBorderSize;

	// [AK] Add the gaps between each of the active columns and the background border size to the total width.
	ulWidth += ulExtraSpace;

	// [AK] If the scoreboard is too wide, try shrinking the columns as much as possible.
	if ( ulWidth > static_cast<ULONG>( HUD_GetWidth( )))
	{
		// [AK] Choose whichever's bigger: the shortest combined width of all active columns, or the width of
		// the screen minus the extra space.
		const ULONG ulShortestPossibleWidth = MAX<ULONG>( ulShortestWidthOfAllColumns, HUD_GetWidth( ) - ulExtraSpace );
		const ULONG ulWidthWithoutSpace = ulWidth - ulExtraSpace;

		// [AK] If we're able to shrink down any active columns, then re-adjust their widths as necessary.
		if ( ulShortestPossibleWidth < ulWidthWithoutSpace )
		{
			const ULONG ulMinWidthDiff = ulWidthWithoutSpace - ulShortestPossibleWidth;
			const ULONG ulMaxWidthDiff = ulWidthWithoutSpace - ulShortestWidthOfAllColumns;

			ulWidth = ulExtraSpace;

			for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
			{
				if ( ColumnOrder[i]->IsDisabled( ))
					continue;

				// [AK] Only re-adjust columns that can be shrunken down.
				if ( ColumnOrder[i]->GetShortestWidth( ) < ColumnOrder[i]->GetWidth( ))
				{
					const ULONG ulColumnWidthDiff = ColumnOrder[i]->GetWidth( ) - ColumnOrder[i]->GetShortestWidth( );
					const float fScale = static_cast<float>( ulColumnWidthDiff ) / static_cast<float>( ulMaxWidthDiff );

					ColumnOrder[i]->ulWidth -= static_cast<ULONG>( ulMinWidthDiff * fScale );
				}

				ulWidth += ColumnOrder[i]->GetWidth( );
			}
		}
	}

	lRelX = ( HUD_GetWidth( ) - static_cast<LONG>( ulWidth )) / 2;

	LONG lCurXPos = lRelX + ulBackgroundBorderSize + ulColumnPadding;

	// [AK] We got the width of the scoreboard. Now update the positions of all the columns.
	// Do this here because we already know how many columns are active in this function.
	for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
	{
		if ( ColumnOrder[i]->IsDisabled( ))
			continue;

		ColumnOrder[i]->lRelX = lCurXPos;
		lCurXPos += ColumnOrder[i]->GetWidth( );

		if ( --ulNumActiveColumns > 0 )
			lCurXPos += ulGapBetweenColumns + 2 * ulColumnPadding;
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::UpdateHeight
//
// Determines what the height of the scoreboard should be right now.
//
//*****************************************************************************

void Scoreboard::UpdateHeight( const ULONG ulDisplayPlayer )
{
	const ULONG ulRowYOffset = lRowHeight + ulGapBetweenRows;
	const ULONG ulNumActivePlayers = HUD_GetNumPlayers( );
	const ULONG ulNumSpectators = HUD_GetNumSpectators( );
	const ULONG ulWidthWithoutBorder = ulWidth - 2 * ulBackgroundBorderSize;

	ulHeight = 2 * ulBackgroundBorderSize + lHeaderHeight + ulGapBetweenHeaderAndRows;

	MainHeader.Refresh( ulDisplayPlayer, ulWidthWithoutBorder );
	ulHeight += MainHeader.GetHeight( );

	if (( ulFlags & SCOREBOARDFLAG_DONTDRAWBORDERS ) == false )
	{
		// [AK] The borders are drawn in three places: above and below the column headers, and
		// underneath all player rows. If using textures for the borders, then we must add
		// its height three times.
		if (( ulFlags & SCOREBOARDFLAG_USETEXTUREFORBORDERS ) && ( pBorderTexture != NULL ))
			ulHeight += pBorderTexture->GetScaledHeight( ) * 3;
		// [AK] Otherwise, add 6 pixels for lined borders (each border is 2 pixels tall).
		else
			ulHeight += 6;
	}

	// [AK] Add the total height of all rows for active players.
	if ( ulNumActivePlayers > 0 )
	{
		ulHeight += ulNumActivePlayers * ulRowYOffset;

		if ( ShouldSeparateTeams( ))
		{
			const ULONG ulNumTeamsWithPlayers = TEAM_TeamsWithPlayersOn( );

			if ( ulNumTeamsWithPlayers > 0 )
			{
				// [AK] Refresh and add the heights of all team headers too, if allowed.
				if (( ulFlags & SCOREBOARDFLAG_DONTSHOWTEAMHEADERS ) == false )
				{
					TeamHeader.Refresh( ulDisplayPlayer, ulWidthWithoutBorder );
					ulHeight += TeamHeader.GetHeight( ) * ulNumTeamsWithPlayers;
				}

				ulHeight += lRowHeight * ( ulNumTeamsWithPlayers - 1 );
			}
		}
	}

	// [AK] Do the same for any true spectators.
	if ( ulNumSpectators > 0 )
	{
		if ( ulNumActivePlayers > 0 )
			ulHeight += lRowHeight;

		// [AK] Refresh and add the height of the spectator header too, if allowed.
		if (( ulFlags & SCOREBOARDFLAG_DONTSHOWTEAMHEADERS ) == false )
		{
			SpectatorHeader.Refresh( ulDisplayPlayer, ulWidthWithoutBorder );
			ulHeight += SpectatorHeader.GetHeight( );
		}

		ulHeight += ulNumSpectators * ulRowYOffset;
	}

	Footer.Refresh( ulDisplayPlayer, ulWidthWithoutBorder );
	ulHeight += Footer.GetHeight( );

	lRelY = ( HUD_GetHeight( ) - static_cast<LONG>( ulHeight )) / 2;
}

//*****************************************************************************
//
// [AK] Scoreboard::Render
//
// Draws the scoreboard's background, then everything else.
//
//*****************************************************************************

void Scoreboard::Render( const ULONG ulDisplayPlayer, const float fAlpha )
{
	int clipLeft = lRelX;
	int clipTop = lRelY;
	int clipWidth = ulWidth;
	int clipHeight = ulHeight;

	// [AK] If we need to update the scoreboard, do so before rendering it.
	if ( lLastRefreshTick != gametic )
	{
		Refresh( ulDisplayPlayer );
		lLastRefreshTick = gametic;
	}

	// [AK] We can't draw anything if the width, height, or opacity are zero or less.
	if (( ulWidth == 0 ) || ( ulHeight == 0 ) || ( fAlpha <= 0.0f ))
		return;

	// [AK] We must take into account the virtual screen's size.
	if ( g_bScale )
		screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidth, clipHeight, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

	screen->Dim( BackgroundColor, fBackgroundAmount * fAlpha, clipLeft, clipTop, clipWidth, clipHeight );

	const ULONG ulNumActivePlayers = HUD_GetNumPlayers( );
	const ULONG ulNumTrueSpectators = HUD_GetNumSpectators( );
	LONG lYPos = lRelY + ulBackgroundBorderSize;
	bool bUseLightBackground = true;

	// [AK] Draw the main header first.
	MainHeader.Render( ulDisplayPlayer, ScoreMargin::NO_TEAM, lYPos, fAlpha );

	// [AK] Draw a border above the column headers.
	DrawBorder( HeaderColor, lYPos, fAlpha, false );

	// [AK] Draw all of the column headers.
	for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
		ColumnOrder[i]->DrawHeader( lYPos, lHeaderHeight, fAlpha );

	lYPos += lHeaderHeight;

	// [AK] Draw another border below the headers.
	DrawBorder( HeaderColor, lYPos, fAlpha, true );
	lYPos += ulGapBetweenHeaderAndRows;

	// [AK] Draw rows for all active players.
	for ( ULONG ulIdx = 0; ulIdx < ulNumActivePlayers; ulIdx++ )
	{
		const ULONG ulPlayer = ulPlayerList[ulIdx];
		const ULONG ulTeam = players[ulPlayer].Team;

		// [AK] In team-based game modes, if the previous player is on a different team than
		// the current player, leave a gap between both teams and make the row background light.
		if (( ShouldSeparateTeams( )) && ( players[ulPlayer].bOnTeam ) && (( ulIdx == 0 ) || ( ulTeam != players[ulPlayerList[ulIdx - 1]].Team )))
		{
			if ( ulIdx > 0 )
			{
				lYPos += lRowHeight;
				bUseLightBackground = true;
			}

			// [AK] Draw the header for this team, if allowed.
			if (( ulFlags & SCOREBOARDFLAG_DONTSHOWTEAMHEADERS ) == false )
				TeamHeader.Render( ulDisplayPlayer, ulTeam, lYPos, fAlpha );
		}

		DrawRow( ulPlayer, ulDisplayPlayer, lYPos, fAlpha, bUseLightBackground );
	}

	// [AK] Draw rows for any true spectators.
	if ( ulNumTrueSpectators )
	{
		const ULONG ulTotalPlayers = ulNumActivePlayers + ulNumTrueSpectators;

		// [AK] If there are any active players, leave a gap between them and the true
		// spectators, and make the row background light.
		if ( ulNumActivePlayers > 0 )
		{
			lYPos += lRowHeight;
			bUseLightBackground = true;
		}

		// [AK] Draw the header for spectators, if allowed.
		if (( ulFlags & SCOREBOARDFLAG_DONTSHOWTEAMHEADERS ) == false )
			SpectatorHeader.Render( ulDisplayPlayer, ScoreMargin::NO_TEAM, lYPos, fAlpha );

		// [AK] The index of the first true spectator should be the same as the number of active
		// players. The list is organized such that all active players come before any true spectators.
		for ( ULONG ulIdx = ulNumActivePlayers; ulIdx < ulTotalPlayers; ulIdx++ )
			DrawRow( ulPlayerList[ulIdx], ulDisplayPlayer, lYPos, fAlpha, bUseLightBackground );
	}

	// [AK] Draw a border at the bottom of the scoreboard. We must subtract ulGapBetweenRows here (a bit hacky)
	// because SCOREBOARD_s::DrawPlayerRow adds it every time a row is drawn. This isn't necessary for the last row.
	lYPos += ulGapBetweenHeaderAndRows - ulGapBetweenRows;
	DrawBorder( HeaderColor, lYPos, fAlpha, false );

	// [AK] Finally, draw the footer.
	Footer.Render( ulDisplayPlayer, ScoreMargin::NO_TEAM, lYPos, fAlpha );
}

//*****************************************************************************
//
// [AK] Scoreboard::DrawRow
//
// Draws a player's values and the background of their row on the scoreboard.
//
//*****************************************************************************

void Scoreboard::DrawRow( const ULONG ulPlayer, const ULONG ulDisplayPlayer, LONG &lYPos, const float fAlpha, bool &bUseLightBackground ) const
{
	const bool bIsDisplayPlayer = ( ulPlayer == ulDisplayPlayer );
	const bool bPlayerIsDead = (( gamestate == GS_LEVEL ) && (( players[ulPlayer].playerstate == PST_DEAD ) || ( players[ulPlayer].bDeadSpectator )));
	ULONG ulColor = RowColor;

	// [AK] Change the text color to red if we're carrying a terminator sphere.
	if (( terminator ) && ( players[ulPlayer].cheats2 & CF2_TERMINATORARTIFACT ))
	{
		ulColor = CR_RED;
	}
	// [AK] Change the text color to match the player's team if we should.
	else if ( ulFlags & SCOREBOARDFLAG_USETEAMTEXTCOLOR )
	{
		if ( PLAYER_IsTrueSpectator( &players[ulPlayer] ))
			ulColor = CR_GREY;
		else if ( players[ulPlayer].bOnTeam )
			ulColor = TEAM_GetTextColor( players[ulPlayer].Team );
	}
	// [AK] Change the text color if this is the player we're spying.
	else if ( bIsDisplayPlayer )
	{
		if ( CLIENTDEMO_IsPlaying( ))
			ulColor = LocalRowColors[LOCALROW_COLOR_INDEMO];
		else
			ulColor = LocalRowColors[LOCALROW_COLOR_INGAME];
	}

	const float fBackgroundAlpha = ( bPlayerIsDead ? fDeadRowBackgroundAmount : fRowBackgroundAmount ) * fAlpha;

	// [AK] Draw the background of the row, but only if the alpha is non-zero. In team-based game modes,
	// the color of the background is to be the team's own color.
	if ( fBackgroundAlpha > 0.0f )
	{
		ROWBACKGROUND_COLOR_e RowBackground;

		if (( ulPlayer == ulDisplayPlayer ) && (( ulFlags & SCOREBOARDFLAG_DONTUSELOCALROWBACKGROUNDCOLOR ) == false ))
			RowBackground = ROWBACKGROUND_COLOR_LOCAL;
		else
			RowBackground = bUseLightBackground ? ROWBACKGROUND_COLOR_LIGHT : ROWBACKGROUND_COLOR_DARK;

		// [AK] If the player is on a team, blend the team's colour into the row background.
		if (( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS ) && ( players[ulPlayer].bOnTeam ))
			DrawRowBackground( TeamRowBackgroundColors[players[ulPlayer].Team][RowBackground], lYPos, fBackgroundAlpha );
		// [AK] If the player isn't on a team, use the two background colors that are defined.
		else
			DrawRowBackground( RowBackgroundColors[RowBackground], lYPos, fBackgroundAlpha );
	}

	const float fTextAlpha = ( bPlayerIsDead ? fDeadTextAlpha : 1.0f ) * fAlpha;

	// Draw the data for each column, but only if the text alpha is non-zero.
	if ( fTextAlpha > 0.0f )
	{
		for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
			ColumnOrder[i]->DrawValue( ulPlayer, ulColor, lYPos, lRowHeight, fTextAlpha );
	}

	lYPos += lRowHeight + ulGapBetweenRows;
	bUseLightBackground = !bUseLightBackground;
}

//*****************************************************************************
//
// [AK] Scoreboard::DrawBorder
//
// Draws a border on the scoreboard.
//
//*****************************************************************************

void Scoreboard::DrawBorder( const EColorRange Color, LONG &lYPos, const float fAlpha, const bool bReverse ) const
{
	if ( ulFlags & SCOREBOARDFLAG_DONTDRAWBORDERS )
		return;

	int x = lRelX + ulBackgroundBorderSize;
	int y = lYPos;
	int width = ulWidth - 2 * ulBackgroundBorderSize;
	int height = 0;

	if (( ulFlags & SCOREBOARDFLAG_USETEXTUREFORBORDERS ) && ( pBorderTexture != NULL ))
	{
		LONG lXPos = x;
		const LONG lRight = x + width;

		height = pBorderTexture->GetScaledHeight( );

		if ( g_bScale )
			screen->VirtualToRealCoordsInt( x, y, width, height, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

		while ( lXPos < lRight )
		{
			screen->DrawTexture( pBorderTexture, lXPos, lYPos,
				DTA_UseVirtualScreen, g_bScale,
				DTA_ClipLeft, x,
				DTA_ClipRight, x + width,
				DTA_ClipTop, y,
				DTA_ClipBottom, y + height,
				DTA_Alpha, FLOAT2FIXED( fAlpha ),
				TAG_DONE );

			lXPos += pBorderTexture->GetScaledWidth( );
		}

		lYPos += pBorderTexture->GetScaledHeight( );
	}
	else
	{
		uint32 lightColor, darkColor;
		height = 1;

		if ( g_bScale )
			screen->VirtualToRealCoordsInt( x, y, width, height, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

		// [AK] Do we want to use the font's translation table and text color to colorize the border,
		// or the predetermined hexadecimal colors for the border?
		if ( ulFlags & SCOREBOARDFLAG_USEHEADERCOLORFORBORDERS )
		{
			// [AK] Get the translation table of the (team) header font with its corresponding color.
			const FRemapTable *trans = pHeaderFont->GetColorTranslation( Color );

			// [AK] The light color can be somewhere just past the middle of the remap table.
			lightColor = trans->Palette[trans->NumEntries * 2 / 3];

			// [AK] The dark color should be somewhere at the beginning of the remap table.
			darkColor = trans->Palette[MIN( 1, trans->NumEntries )];
		}
		else
		{
			lightColor = BorderColors[BORDER_COLOR_LIGHT];
			darkColor = BorderColors[BORDER_COLOR_DARK];
		}

		// [AK] The dark color goes above the light one, unless it's reversed.
		screen->Dim( bReverse ? lightColor : darkColor, fAlpha, x, y, width, height );
		screen->Dim( bReverse ? darkColor : lightColor, fAlpha, x, y + height, width, height );
		lYPos += 2;
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::DrawRowBackground
//
// Draws a row's background on the scoreboard.
//
//*****************************************************************************

void Scoreboard::DrawRowBackground( const PalEntry color, int x, int y, int width, int height, const float fAlpha ) const
{
	if (( fAlpha <= 0.0f ) || ( fRowBackgroundAmount <= 0.0f ))
		return;

	if ( g_bScale )
		screen->VirtualToRealCoordsInt( x, y, width, height, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

	screen->Dim( color, fAlpha * fRowBackgroundAmount, x, y, width, height );
}

//*****************************************************************************
//
void Scoreboard::DrawRowBackground( const PalEntry color, const int y, const float fAlpha ) const
{
	if (( fAlpha <= 0.0f ) || ( fRowBackgroundAmount <= 0.0f ))
		return;

	const int height = lRowHeight;

	// [AK] If gaps must be shown in the row's background, then only draw the background where
	// the active columns are. Otherwise, draw a single background across the scoreboard.
	if ( ulFlags & SCOREBOARDFLAG_SHOWGAPSINROWBACKGROUND )
	{
		for ( unsigned int i = 0; i < ColumnOrder.Size( ); i++ )
		{
			if ( ColumnOrder[i]->IsDisabled( ))
				continue;

			DrawRowBackground( color, ColumnOrder[i]->GetRelX( ) - ulColumnPadding, y, ColumnOrder[i]->GetWidth( ) + 2 * ulColumnPadding, height, fAlpha );
		}
	}
	else
	{
		DrawRowBackground( color, lRelX + ulBackgroundBorderSize, y, ulWidth - 2 * ulBackgroundBorderSize, height, fAlpha );
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::RemoveInvalidColumnsInRankOrder
//
// Checks if there are any columns in the scoreboard's rank order that aren't
// actually on the scoreboard. Any invalid entries are removed from the list.
//
//*****************************************************************************

void Scoreboard::RemoveInvalidColumnsInRankOrder( void )
{
	for ( int i = 0; i < static_cast<int>( RankOrder.Size( )); i++ )
	{
		if ( RankOrder[i]->GetScoreboard( ) != this )
			RankOrder.Delete( i-- );
	}
}

//*****************************************************************************
//
// [AK] Scoreboard::ShouldSeparateTeams
//
// Checks if the scoreboard should separate players into their respective teams.
//
//*****************************************************************************

bool Scoreboard::ShouldSeparateTeams( void ) const
{
	return (( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS ) && (( ulFlags & SCOREBOARDFLAG_DONTSEPARATETEAMS ) == false ));
}

//*****************************************************************************
//
// [AK] SCOREBOARD_Construct
//
// Initializes the scoreboard and parses all loaded SCORINFO lumps.
//
//*****************************************************************************

void SCOREBOARD_Construct( void )
{
	if ( Wads.CheckNumForName( "SCORINFO" ) != -1 )
	{
		int currentLump, lastLump = 0;

		Printf( "ParseScorInfo: Loading scoreboard definition.\n" );

		while (( currentLump = Wads.FindLump( "SCORINFO", &lastLump )) != -1 )
		{
			FScanner sc( currentLump );

			while ( sc.GetString( ))
			{
				if ( stricmp( sc.String, "scoreboard" ) == 0 )
				{
					g_Scoreboard.Parse( sc );
				}
				else if (( stricmp( sc.String, "column" ) == 0 ) || ( stricmp( sc.String, "compositecolumn" ) == 0 ))
				{
					const bool bIsCompositeBlock = ( stricmp( sc.String, "compositecolumn" ) == 0 );
					FString ColumnTypeString = "COLUMNTYPE_";

					sc.MustGetToken( TK_StringConst );

					if ( sc.StringLen == 0 )
						sc.ScriptError( "Got an empty string for a column name." );

					FName ColumnName = sc.String;
					ColumnTypeString += ColumnName.GetChars( );
					ColumnTypeString.ToUpper( );

					// [AK] If the column doesn't exist yet, then we must create a new one.
					ScoreColumn *pColumn = SCOREBOARD_GetColumn( ColumnName, false );
					const bool bMustCreateNewColumn = ( pColumn == NULL );

					COLUMNTYPE_e ColumnType = static_cast<COLUMNTYPE_e>( GetValueCOLUMNTYPE_e( ColumnTypeString.GetChars( )));

					if ( bIsCompositeBlock )
					{
						if ( bMustCreateNewColumn )
						{
							// [AK] Don't allow native types (e.g. "frags") to be used as names for composite columns.
							if ( ColumnType != COLUMNTYPE_UNKNOWN )
								sc.ScriptError( "You can't use '%s' as a name for a composite column.", ColumnName.GetChars( ));

							pColumn = new CompositeScoreColumn( ColumnName );
						}

						if ( pColumn->GetTemplate( ) != COLUMNTEMPLATE_COMPOSITE )
							sc.ScriptError( "Column '%s' isn't a composite column.", ColumnName.GetChars( ));
					}
					else
					{
						if ( bMustCreateNewColumn )
						{
							// [AK] If the column isn't using a native type for a name, then it's a custom column.
							// This also implies that "custom" is a valid name for custom columns.
							if ( ColumnType == COLUMNTYPE_UNKNOWN )
								ColumnType = COLUMNTYPE_CUSTOM;

							if ( ColumnType == COLUMNTYPE_COUNTRYFLAG )
							{
								pColumn = new CountryFlagScoreColumn( sc, ColumnName );
							}
							else
							{
								if ( ColumnType == COLUMNTYPE_CUSTOM )
								{
									// [AK] Make sure that this custom column has data already defined.
									if (( gameinfo.CustomPlayerData.CountUsed( ) == 0 ) || ( gameinfo.CustomPlayerData.CheckKey( ColumnName ) == NULL ))
										sc.ScriptError( "Custom column '%s' cannot be created without defining the data first.", ColumnName.GetChars( ));
								}

								pColumn = new DataScoreColumn( ColumnType, ColumnName );
							}
						}

						if ( pColumn->GetTemplate( ) != COLUMNTEMPLATE_DATA )
							sc.ScriptError( "Column '%s' isn't a data column.", ColumnName.GetChars( ));
					}

					// [AK] If a new column was created, insert it to the global list.
					if ( bMustCreateNewColumn )
						g_Columns.Insert( ColumnName, pColumn );

					pColumn->Parse( sc );
				}
				else
				{
					sc.ScriptError( "Unknown option '%s', on line %d in SCORINFO.", sc.String, sc.Line );
				}
			}
		}
	}
}

//*****************************************************************************
//
// [AK] SCOREBOARD_Reset
//
// This should only be executed at the start of a new game. It checks if any
// columns that are part of the scoreboard are usable in the current game.
//
//*****************************************************************************

void SCOREBOARD_Reset( void )
{
	// [AK] Don't do anything if there are no defined columns.
	if ( g_Columns.CountUsed( ) == 0 )
		return;

	TMapIterator<FName, ScoreColumn *> it( g_Columns );
	TMap<FName, ScoreColumn *>::Pair *pair;

	while ( it.NextPair( pair ))
	{
		// [AK] Ignore data columns that are part of a composite column, the latter
		// also checks if their sub-columns are usable.
		if (( pair->Value->GetTemplate( ) != COLUMNTEMPLATE_DATA ) || ( static_cast<DataScoreColumn *>( pair->Value )->GetCompositeColumn( ) == NULL ))
			pair->Value->CheckIfUsable( );
	}
}

//*****************************************************************************
//
// SCOREBOARD_Render
//
// Draws the scoreboard on the screen.
//
//*****************************************************************************

void SCOREBOARD_Render( ULONG ulDisplayPlayer )
{
	// Make sure the display player is valid.
	if ( ulDisplayPlayer >= MAXPLAYERS )
		return;

	g_Scoreboard.Render( ulDisplayPlayer, cl_scoreboardalpha );
}

//*****************************************************************************
//
// SCOREBOARD_ShouldDrawBoard
//
// Checks if the user wants to see the scoreboard and is allowed to.
//
//*****************************************************************************

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
// [AK] SCOREBOARD_GetColumn
//
// Returns a pointer to a column by searching for its name.
//
//*****************************************************************************

ScoreColumn *SCOREBOARD_GetColumn( FName Name, const bool bMustBeUsable )
{
	ScoreColumn **pColumn = g_Columns.CheckKey( Name );

	if (( pColumn != NULL ) && ( *pColumn != NULL ))
		return (( bMustBeUsable == false ) || (( *pColumn )->IsUsableInCurrentGame( ))) ? *pColumn : NULL;

	return NULL;
}

//*****************************************************************************
//
// [AK] scoreboard_ScanForColumn
//
// Scans for a column by name, throwing a fatal error if the column couldn't
// be found. This also has the option of throwing a fatal error if a data
// column must be returned, but the column that was found isn't one.
//
//*****************************************************************************

static ScoreColumn *scoreboard_ScanForColumn( FScanner &sc, const bool bMustBeDataColumn )
{
	sc.MustGetToken( TK_StringConst );

	// [AK] Throw a fatal error if an empty string was passed.
	if ( sc.StringLen == 0 )
		sc.ScriptError( "Got an empty string for a column name." );

	// [AK] Find a column from the main scoreboard object.
	ScoreColumn *pColumn = SCOREBOARD_GetColumn( sc.String, false );

	if ( pColumn == NULL )
		sc.ScriptError( "Column '%s' wasn't found.", sc.String );

	// [AK] Make sure that the pointer is of a DataScoreColumn object
	// (i.e. the template isn't unknown or a composite).
	if (( bMustBeDataColumn ) && ( pColumn->GetTemplate( ) != COLUMNTEMPLATE_DATA ))
		sc.ScriptError( "Column '%s' is not a data column.", sc.String );

	return pColumn;
}

//*****************************************************************************
//
// [AK] scoreboard_TryPushingColumnToList
//
// Tries pushing a pointer to a column object into a list, but only if that
// pointer isn't in the list already. Returns true if successful, or false if
// it wasn't added to the list.
//
//*****************************************************************************

template <typename ColumnType>
static bool scoreboard_TryPushingColumnToList( FScanner &sc, TArray<ColumnType *> &ColumnList, ColumnType *pColumn )
{
	// [AK] Make sure the pointer to the column isn't NULL.
	if ( pColumn == NULL )
		return false;

	// [AK] Make sure that this column isn't already inside this list.
	for ( unsigned int i = 0; i < ColumnList.Size( ); i++ )
	{
		// [AK] Print an error message to let the user know the issue.
		if ( ColumnList[i] == pColumn )
		{
			sc.ScriptMessage( "Tried to put column '%s' into a list more than once.", pColumn->GetInternalName( ));
			return false;
		}
	}

	ColumnList.Push( pColumn );
	return true;
}

//*****************************************************************************
//
// [AK] scoreboard_TryRemovingColumnFromList
//
// Removes a pointer to a column object from a list. Returns true if the column
// was removed successfully, or false if it wasn't in the list to begin with.
//
//*****************************************************************************

template <typename ColumnType>
static bool scoreboard_TryRemovingColumnFromList( FScanner &sc, TArray<ColumnType *> &ColumnList, ColumnType *pColumn )
{
	// [AK] Make sure the pointer to the column isn't NULL.
	if ( pColumn == NULL )
		return false;

	for ( unsigned int i = 0; i < ColumnList.Size( ); i++ )
	{
		if ( ColumnList[i] == pColumn )
		{
			ColumnList.Delete( i );
			return true;
		}
	}

	// [AK] If we get this far, then the column wasn't in the list. Inform the user.
	sc.ScriptMessage( "Couldn't find column '%s' in the list.", pColumn->GetInternalName( ));
	return false;
}

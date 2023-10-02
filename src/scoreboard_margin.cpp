//-----------------------------------------------------------------------------
//
// Zandronum Source
// Copyright (C) 2021-2023 Adam Kaminski
// Copyright (C) 2021-2023 Zandronum Development Team
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
// 3. Neither the name of the Zandronum Development Team nor the names of its
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
// Filename: scoreboard_margin.cpp
//
// Description: Contains everything that controls the scoreboard's margins
// (i.e. the main header, team/spectator headers, and the footer).
//
//-----------------------------------------------------------------------------

#include <map>
#include <tuple>
#include "c_console.h"
#include "scoreboard.h"
#include "team.h"
#include "st_hud.h"
#include "v_video.h"
#include "wi_stuff.h"
#include "cooperative.h"
#include "deathmatch.h"
#include "invasion.h"
#include "duel.h"
#include "lastmanstanding.h"
#include "g_game.h"

EXTERN_CVAR( Int, con_virtualwidth )
EXTERN_CVAR( Int, con_virtualheight )
EXTERN_CVAR( Bool, con_scaletext_usescreenratio )

//*****************************************************************************
//	DEFINITIONS

// What kind of parameter is this?
#define WHICH_PARAMETER			0
// Must this parameter be initialized?
#define MUST_BE_INITIALIZED		1
// Which command(s) can this parameter be used in?
#define USABLE_COMMANDS			2

//*****************************************************************************
//
// [AK] All parameters used by DrawBaseCommand and its derivatives.
//
enum PARAMETER_e
{
	// The value used when drawing the contents.
	PARAMETER_VALUE,
	// How much the contents are offset horizontally.
	PARAMETER_XOFFSET,
	// How much the contents are offset vertically.
	PARAMETER_YOFFSET,
	// How the contents are aligned horizontally (left, center, or right).
	PARAMETER_HORIZALIGN,
	// How the contents are aligned vertically (top, center, or bottom).
	PARAMETER_VERTALIGN,
	// How much room to leave underneath the contents.
	PARAMETER_BOTTOMPADDING,
	// How much room to leave to the right of the contents.
	PARAMETER_RIGHTPADDING,
	// The transparency of the contents.
	PARAMETER_ALPHA,
	// What font to use when drawing a string.
	PARAMETER_FONT,
	// What text color to use when drawing a string.
	PARAMETER_TEXTCOLOR,
	// How large are the gaps between each separate line.
	PARAMETER_GAPSIZE,
	// The width of a color box.
	PARAMETER_WIDTH,
	// The height of a color box.
	PARAMETER_HEIGHT,

	NUM_PARAMETERS
};

//*****************************************************************************
//
// [AK] The command (DrawString, DrawColor, or DrawTexture) a parameter is intended for.
//
enum COMMAND_e
{
	COMMAND_ALL,
	COMMAND_BLOCK,
	COMMAND_STRING,
	COMMAND_COLOR,
	COMMAND_TEXTURE,
};

//*****************************************************************************
//	VARIABLES

// [AK] A map of all of the parameters used by DrawBaseCommand and its derivatives.
static const std::map<FName, std::tuple<PARAMETER_e, bool, std::set<COMMAND_e>>> g_NamedParameters =
{
	{ "value",				{ PARAMETER_VALUE,			true,	{ COMMAND_STRING, COMMAND_COLOR, COMMAND_TEXTURE }}},
	{ "x",					{ PARAMETER_XOFFSET,		false,	{ COMMAND_ALL }}},
	{ "y",					{ PARAMETER_YOFFSET,		false,	{ COMMAND_ALL }}},
	{ "horizontalalign",	{ PARAMETER_HORIZALIGN,		false,	{ COMMAND_ALL }}},
	{ "verticalalign",		{ PARAMETER_VERTALIGN,		false,	{ COMMAND_ALL }}},
	{ "bottompadding",		{ PARAMETER_BOTTOMPADDING,	false,	{ COMMAND_ALL }}},
	{ "rightpadding",		{ PARAMETER_RIGHTPADDING,	false,	{ COMMAND_ALL }}},
	{ "alpha",				{ PARAMETER_ALPHA,			false,	{ COMMAND_ALL }}},
	{ "font",				{ PARAMETER_FONT,			false,	{ COMMAND_STRING }}},
	{ "textcolor",			{ PARAMETER_TEXTCOLOR,		false,	{ COMMAND_STRING }}},
	{ "gapsize",			{ PARAMETER_GAPSIZE,		false,	{ COMMAND_STRING }}},
	{ "width",				{ PARAMETER_WIDTH,			true,	{ COMMAND_COLOR }}},
	{ "height",				{ PARAMETER_HEIGHT,			true,	{ COMMAND_COLOR }}},
};

// [AK] The level we are entering, to be shown on the intermission screen.
static	level_info_t *g_pNextLevel;

//*****************************************************************************
//	CLASSES

//*****************************************************************************
//*****************************************************************************
//
// [AK] ElementBaseCommand
//
// An abstract class for all margin commands that should appear on the screen.
//
//*****************************************************************************
//*****************************************************************************

class ElementBaseCommand : public ScoreMargin::BaseCommand
{
public:
	ElementBaseCommand( ScoreMargin *pMargin, BaseCommand *pParentCommand, COMMAND_e Type ) : BaseCommand( pMargin, pParentCommand ),
		HorizontalAlignment( HORIZALIGN_LEFT ),
		VerticalAlignment( VERTALIGN_TOP ),
		lXOffset( 0 ),
		lYOffset( 0 ),
		ulBottomPadding( 0 ),
		ulRightPadding( 0 ),
		fTranslucency( 1.0f ),
		Command( Type ) { }

	//*************************************************************************
	//
	// [AK] Scans for any parameters until it reaches the end of the command.
	//
	//*************************************************************************

	virtual void Parse( FScanner &sc )
	{
		bool bParameterInitialized[NUM_PARAMETERS] = { false };

		// [AK] If there's already a parenthesis, then don't scan for the parameters.
		if ( sc.CheckToken( ')' ) == false )
		{
			do
			{
				sc.MustGetToken( TK_Identifier );
				auto parameter = g_NamedParameters.find( sc.String );

				// [AK] Make sure that the user entered a valid parameter.
				if ( parameter == g_NamedParameters.end( ))
					sc.ScriptError( "Unknown parameter '%s'.", sc.String );

				const PARAMETER_e ParameterConstant = std::get<WHICH_PARAMETER>( parameter->second );

				// [AK] Make sure that the parameter can be used by this command.
				if ( CanUseParameter( parameter->first ) == false )
					sc.ScriptError( "Parameter '%s' cannot be used inside this command.", sc.String );

				// [AK] Don't allow the same parameter to be initialized more than once.
				if ( bParameterInitialized[ParameterConstant] )
					sc.ScriptError( "Parameter '%s' is already initialized.", sc.String );

				sc.MustGetToken( '=' );
				ParseParameter( sc, parameter->first, ParameterConstant );

				// [AK] This parameter has been initialized now, so mark it.
				bParameterInitialized[ParameterConstant] = true;

			} while ( sc.CheckToken( ',' ));

			sc.MustGetToken( ')' );
		}

		// [AK] Throw an error if there are parameters that were supposed to be initialized, but aren't.
		for ( auto it = g_NamedParameters.begin( ); it != g_NamedParameters.end( ); it++ )
		{
			const PARAMETER_e ParameterConstant = std::get<WHICH_PARAMETER>( it->second );

			// [AK] Skip parameters that aren't associated with this command.
			if ( CanUseParameter( it->first ) == false )
				continue;

			if (( std::get<MUST_BE_INITIALIZED>( it->second )) && ( bParameterInitialized[ParameterConstant] == false ))
				sc.ScriptError( "Parameter '%s' isn't initialized.", it->first.GetChars( ));
		}
	}

	//*************************************************************************
	//
	// [AK] Ensures that the margin can fit the contents (for all teams).
	//
	//*************************************************************************

	virtual void Refresh( const ULONG ulDisplayPlayer );

	//*************************************************************************
	//
	// [AK] Pure virtual functions that return the width/height of the contents.
	//
	//*************************************************************************

	virtual ULONG GetContentWidth( const ULONG ulTeam ) const = 0;
	virtual ULONG GetContentHeight( const ULONG ulTeam ) const = 0;

protected:

	//*************************************************************************
	//
	// [AK] Parses any parameters that every element command can have. Derived
	// classes can handle their own parameters by overriding this function.
	//
	//*************************************************************************

	virtual void ParseParameter( FScanner &sc, const FName ParameterName, const PARAMETER_e Parameter )
	{
		// [AK] Commands nested inside MultiLineBlock or RowBlock commands can't use these parameters.
		if (( pParentCommand != NULL ) && ( pParentCommand->IsBlockElement( )))
		{
			if (( Parameter == PARAMETER_XOFFSET ) || ( Parameter == PARAMETER_YOFFSET ) ||
				( Parameter == PARAMETER_HORIZALIGN ) || ( Parameter == PARAMETER_VERTALIGN ))
			{
				sc.ScriptError( "Parameter '%s' cannot be used by commands that are inside a 'MultiLineBlock' command.", ParameterName.GetChars( ));
			}
		}

		switch ( Parameter )
		{
			case PARAMETER_XOFFSET:
			case PARAMETER_YOFFSET:
			case PARAMETER_BOTTOMPADDING:
			case PARAMETER_RIGHTPADDING:
			{
				const bool bIsNegative = sc.CheckToken( '-' );
				sc.MustGetToken( TK_IntConst );

				const int value = bIsNegative ? -sc.Number : sc.Number;

				if ( Parameter == PARAMETER_XOFFSET )
					lXOffset = value;
				else if ( Parameter == PARAMETER_YOFFSET )
					lYOffset = value;
				else if ( Parameter == PARAMETER_BOTTOMPADDING )
					ulBottomPadding = MAX( value, 0 );
				else
					ulRightPadding = MAX( value, 0 );

				break;
			}

			case PARAMETER_HORIZALIGN:
			case PARAMETER_VERTALIGN:
			{
				sc.MustGetToken( TK_Identifier );

				if ( Parameter == PARAMETER_HORIZALIGN )
					HorizontalAlignment = static_cast<HORIZALIGN_e>( sc.MustGetEnumName( "alignment", "HORIZALIGN_", GetValueHORIZALIGN_e, true ));
				else
					VerticalAlignment = static_cast<VERTALIGN_e>( sc.MustGetEnumName( "alignment", "VERTALIGN_", GetValueVERTALIGN_e, true ));

				break;
			}

			case PARAMETER_ALPHA:
			{
				sc.MustGetToken( TK_FloatConst );
				fTranslucency = clamp( static_cast<float>( sc.Float ), 0.0f, 1.0f );

				break;
			}

			default:
				sc.ScriptError( "Couldn't process parameter '%s'.", ParameterName.GetChars( ));
				break;
		}

		// [AK] Don't offset to the left when aligned to the left, or to the right when aligned to the right.
		if ((( HorizontalAlignment == HORIZALIGN_LEFT ) || ( HorizontalAlignment == HORIZALIGN_RIGHT )) && ( lXOffset < 0 ))
			sc.ScriptError( "Can't have a negative x-offset when aligned to the left or right." );

		// [AK] Don't offset upward when aligned to the top, or downward when aligned to the bottom.
		if ((( VerticalAlignment == VERTALIGN_TOP ) || ( VerticalAlignment == VERTALIGN_BOTTOM )) && ( lYOffset < 0 ))
			sc.ScriptError( "Can't have a negative y-offset when aligned to the top or bottom." );
	}

	//*************************************************************************
	//
	// [AK] If this element command is part of a MultiLineBlock or RowBlock,
	// then return the latter's horizontal alignment (and so on). Otherwise,
	// just return this command's own alignment.
	//
	//*************************************************************************

	HORIZALIGN_e GetHorizontalAlignment( void ) const
	{
		if (( pParentCommand != NULL ) && ( pParentCommand->IsBlockElement( )))
			return static_cast<ElementBaseCommand *>( pParentCommand )->GetHorizontalAlignment( );

		return HorizontalAlignment;
	}

	//*************************************************************************
	//
	// [AK] Determines the position to draw the contents on the screen.
	//
	//*************************************************************************

	TVector2<LONG> GetDrawingPosition( const ULONG ulWidth, const ULONG ulHeight, const LONG lXOffsetBonus = 0 ) const
	{
		const ULONG ulHUDWidth = HUD_GetWidth( );
		const LONG lActualXOffset = lXOffset + lXOffsetBonus;
		TVector2<LONG> result;

		// [AK] Get the x-position based on the horizontal alignment.
		if ( HorizontalAlignment == HORIZALIGN_LEFT )
			result.X = ( ulHUDWidth - pParentMargin->GetWidth( )) / 2 + lActualXOffset;
		else if ( HorizontalAlignment == HORIZALIGN_CENTER )
			result.X = ( ulHUDWidth - ulWidth ) / 2 + lActualXOffset;
		else
			result.X = ( ulHUDWidth + pParentMargin->GetWidth( )) / 2 - ulWidth - lActualXOffset;

		// [AK] Next, get the y-position based on the vertical alignment.
		if ( VerticalAlignment == VERTALIGN_TOP )
			result.Y = lYOffset;
		else if ( VerticalAlignment == VERTALIGN_CENTER )
			result.Y = ( pParentMargin->GetHeight( ) - ulHeight ) / 2 + lYOffset;
		else
			result.Y = pParentMargin->GetHeight( ) - ulHeight - lYOffset;

		return result;
	}

	//*************************************************************************
	//
	// [AK] Increases the margin's height to fit the contents, if necessary.
	//
	//*************************************************************************

	void EnsureContentFitsInMargin( const ULONG ulHeight )
	{
		if ( ulHeight > 0 )
		{
			LONG lAbsoluteOffset = abs( lYOffset );

			// [AK] Double the y-offset if the content is aligned to the center.
			if ( VerticalAlignment == VERTALIGN_CENTER )
				lAbsoluteOffset *= 2;

			const LONG lHeightDiff = lAbsoluteOffset + ulHeight - pParentMargin->GetHeight( );

			if ( lHeightDiff > 0 )
				pParentMargin->IncreaseHeight( lHeightDiff );
		}
	}

	HORIZALIGN_e HorizontalAlignment;
	VERTALIGN_e VerticalAlignment;
	LONG lXOffset;
	LONG lYOffset;
	ULONG ulBottomPadding;
	ULONG ulRightPadding;
	float fTranslucency;

private:

	//*************************************************************************
	//
	// [AK] Checks if this command can use the given parameter.
	//
	//*************************************************************************

	bool CanUseParameter( const FName ParameterName )
	{
		auto parameter = g_NamedParameters.find( ParameterName );

		if ( parameter == g_NamedParameters.end( ))
			return false;

		const std::set<COMMAND_e> &set = std::get<USABLE_COMMANDS>( parameter->second );

		// [AK] If the set contains COMMAND_ALL, then the parameter is usable by all commands.
		if (( set.find( COMMAND_ALL ) != set.end( )) || ( set.find( Command ) != set.end( )))
			return true;

		return false;
	}

	const COMMAND_e Command;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] BlockBaseCommand
//
// An abstract class used for the MultiLineBlock and RowBlock margin commands.
//
//*****************************************************************************
//*****************************************************************************

class BlockBaseCommand : public ElementBaseCommand
{
public:
	BlockBaseCommand( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : ElementBaseCommand( pMargin, pParentCommand, COMMAND_BLOCK ) { }

	//*************************************************************************
	//
	// [AK] Starts a block and parses new margin commands inside it.
	//
	//*************************************************************************

	virtual void Parse( FScanner &sc )
	{
		ElementBaseCommand::Parse( sc );
		Block.ParseCommands( sc, pParentMargin, this );
	}

	//*************************************************************************
	//
	// [AK] Refreshes all the commands inside the block, then makes sure that
	// the margin will fit everything.
	//
	//*************************************************************************

	virtual void Refresh( const ULONG ulDisplayPlayer )
	{
		CommandsToDraw.Clear( );

		Block.Refresh( ulDisplayPlayer );
		ElementBaseCommand::Refresh( ulDisplayPlayer );
	}

	//*************************************************************************
	//
	// [AK] This is a block element, so always return true.
	//
	//*************************************************************************

	virtual bool IsBlockElement( void ) const { return true; }

	//*************************************************************************
	//
	// [AK] Adds a command to the list of commands that will be drawn. This is
	// called by other ElementBaseCommand objects after they've refreshed.
	//
	//*************************************************************************

	void AddToDrawList( ElementBaseCommand *pCommand )
	{
		// [AK] Don't accept commands that aren't part of this block.
		if (( pCommand == NULL ) || ( pCommand->GetParentCommand( ) != this ))
			return;

		CommandsToDraw.Push( pCommand );
	}

protected:

	//*************************************************************************
	//
	// [AK] Helper function that gets the largest width/height of the commands.
	//
	//*************************************************************************

	ULONG GetMaxFromList( const ULONG ulTeam, const ULONG ulPadding, ULONG ( ElementBaseCommand::*pGetterFunc )( const ULONG ) const ) const
	{
		ULONG ulMax = 0;

		for ( unsigned int i = 0; i < CommandsToDraw.Size( ); i++ )
			ulMax = MAX<ULONG>( ulMax, ( CommandsToDraw[i]->*pGetterFunc )( ulTeam ));

		// [AK] Add the padding (bottom/right), but only if the result is non-zero.
		if ( ulMax > 0 )
			ulMax += ulPadding;

		return ulMax;
	}

	//*************************************************************************
	//
	// [AK] Another helper function that gets the total width/height instead.
	//
	//*************************************************************************

	ULONG GetTotalFromList( const ULONG ulTeam, const ULONG ulPadding, ULONG ( ElementBaseCommand::*pGetterFunc )( const ULONG ) const ) const
	{
		ULONG ulTotal = 0;

		for ( unsigned int i = 0; i < CommandsToDraw.Size( ); i++ )
			ulTotal += ( CommandsToDraw[i]->*pGetterFunc )( ulTeam );

		if ( ulTotal > 0 )
			ulTotal += ulPadding;

		return ulTotal;
	}

	ScoreMargin::CommandBlock Block;
	TArray<ElementBaseCommand *> CommandsToDraw;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] MultiLineBlock
//
// Starts a block of lines that consist of strings, colors, or textures.
//
//*****************************************************************************
//*****************************************************************************

class MultiLineBlock : public BlockBaseCommand
{
public:
	MultiLineBlock( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : BlockBaseCommand( pMargin, pParentCommand ) { }

	//*************************************************************************
	//
	// [AK] Draws all commands that can be drawn, from top to bottom.
	//
	//*************************************************************************

	virtual void Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
	{
		if ( CommandsToDraw.Size( ) == 0 )
			return;

		const float fCombinedAlpha = fAlpha * fTranslucency;
		const HORIZALIGN_e AlignmentToUse = GetHorizontalAlignment( );
		const ULONG ulWidth = GetContentWidth( ulTeam );
		TVector2<LONG> Pos = GetDrawingPosition( ulWidth, GetContentHeight( ulTeam ), lXOffsetBonus );

		Pos.X -= ( HUD_GetWidth( ) - pParentMargin->GetWidth( )) / 2;
		Pos.Y += lYPos;

		for ( unsigned int i = 0; i < CommandsToDraw.Size( ); i++ )
		{
			const ULONG ulContentHeight = CommandsToDraw[i]->GetContentHeight( ulTeam );

			// [AK] Skip commands whose heights are zero.
			if ( ulContentHeight == 0 )
				continue;

			const ULONG ulContentWidth = CommandsToDraw[i]->GetContentWidth( ulTeam );
			LONG lActualXOffset = Pos.X;

			if ( AlignmentToUse == HORIZALIGN_CENTER )
				lActualXOffset += ( ulWidth - ulContentWidth ) / 2;
			else if ( AlignmentToUse == HORIZALIGN_RIGHT )
				lActualXOffset += ulWidth - ulContentWidth;

			CommandsToDraw[i]->Draw( ulDisplayPlayer, ulTeam, Pos.Y, fCombinedAlpha, lActualXOffset );

			// [AK] Shift the y-position based on the command's height.
			Pos.Y += ulContentHeight;
		}
	}

	//*************************************************************************
	//
	// [AK] Gets the largest width required by all commands that will be drawn.
	//
	//*************************************************************************

	virtual ULONG GetContentWidth( const ULONG ulTeam ) const
	{
		return GetMaxFromList( ulTeam, ulRightPadding, &ElementBaseCommand::GetContentWidth );
	}

	//*************************************************************************
	//
	// [AK] Gets the total height of all commands that will be drawn.
	//
	//*************************************************************************

	virtual ULONG GetContentHeight( const ULONG ulTeam ) const
	{
		return GetTotalFromList( ulTeam, ulBottomPadding, &ElementBaseCommand::GetContentHeight );
	}
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] RowBlock
//
// Starts a row that consists of strings, colors, or textures.
//
//*****************************************************************************
//*****************************************************************************

class RowBlock : public BlockBaseCommand
{
public:
	RowBlock( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : BlockBaseCommand( pMargin, pParentCommand ) { }

	//*************************************************************************
	//
	// [AK] Draws all commands that can be drawn, from left to right.
	//
	//*************************************************************************

	virtual void Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
	{
		if ( CommandsToDraw.Size( ) == 0 )
			return;

		const float fCombinedAlpha = fAlpha * fTranslucency;
		const ULONG ulWidth = GetContentWidth( ulTeam );
		const ULONG ulHeight = GetContentHeight( ulTeam );
		TVector2<LONG> Pos = GetDrawingPosition( ulWidth, ulHeight, lXOffsetBonus );

		Pos.X -= ( HUD_GetWidth( ) - pParentMargin->GetWidth( )) / 2;
		Pos.Y += lYPos;

		for ( unsigned int i = 0; i < CommandsToDraw.Size( ); i++ )
		{
			const ULONG ulContentWidth = CommandsToDraw[i]->GetContentWidth( ulTeam );

			// [AK] Skip commands whose widths are zero.
			if ( ulContentWidth == 0 )
				continue;

			const LONG lYOffset = ( ulHeight - CommandsToDraw[i]->GetContentHeight( ulTeam )) / 2;
			CommandsToDraw[i]->Draw( ulDisplayPlayer, ulTeam, Pos.Y + lYOffset, fCombinedAlpha, Pos.X );

			// [AK] Shift the x-offset.
			Pos.X += ulContentWidth;
		}
	}

	//*************************************************************************
	//
	// [AK] Gets the total width of all commands that will be drawn.
	//
	//*************************************************************************

	virtual ULONG GetContentWidth( const ULONG ulTeam ) const
	{
		return GetTotalFromList( ulTeam, ulRightPadding, &ElementBaseCommand::GetContentWidth );
	}

	//*************************************************************************
	//
	// [AK] Gets the largest height required by all commands that will be drawn.
	//
	//*************************************************************************

	virtual ULONG GetContentHeight( const ULONG ulTeam ) const
	{
		return GetMaxFromList( ulTeam, ulBottomPadding, &ElementBaseCommand::GetContentHeight );
	}
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] DrawBaseCommand
//
// An abstract class that is shared by all margin commands that are responsible
// for drawing something.
//
//*****************************************************************************
//*****************************************************************************

class DrawBaseCommand : public ElementBaseCommand
{
public:
	DrawBaseCommand( ScoreMargin *pMargin, BaseCommand *pParentCommand, COMMAND_e Type ) : ElementBaseCommand( pMargin, pParentCommand, Type ) { }

protected:
	template <typename EnumType>
	using SpecialValue = std::pair<EnumType, MARGINTYPE_e>;

	template <typename EnumType>
	using SpecialValueList = std::map<FName, SpecialValue<EnumType>, std::less<FName>, std::allocator<std::pair<const FName, SpecialValue<EnumType>>>>;

	//*************************************************************************
	//
	// [AK] Checks for identifiers that correspond to "special" values. These
	// values can only be used in the margins they're intended for, which is
	// also checked. If no identifier was passed, then the value is assumed to
	// be "static", which is parsed in the form of a string.
	//
	//*************************************************************************

	template <typename EnumType>
	EnumType GetSpecialValue( FScanner &sc, const SpecialValueList<EnumType> &ValueList )
	{
		if ( sc.CheckToken( TK_Identifier ))
		{
			auto value = ValueList.find( sc.String );

			if ( value != ValueList.end( ))
			{
				const MARGINTYPE_e MarginType = value->second.second;

				// [AK] Throw an error if this value can't be used in the margin that the commands belongs to.
				if ( MarginType != pParentMargin->GetType( ))
					sc.ScriptError( "Special value '%s' can't be used inside a '%s' margin.", sc.String, pParentMargin->GetName( ));

				// [AK] Return the constant that corresponds to this special value.
				return value->second.first;
			}
			else
			{
				sc.ScriptError( "Unknown special value '%s'.", sc.String );
			}
		}

		sc.MustGetToken( TK_StringConst );

		// [AK] Throw a fatal error if an empty string was passed.
		if ( sc.StringLen == 0 )
			sc.ScriptError( "Got an empty string for a value." );

		// [AK] Return the constant that indicates the value as "static".
		return static_cast<EnumType>( -1 );
	}
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] DrawString
//
// Draws text somewhere in the margin.
//
//*****************************************************************************
//*****************************************************************************

class DrawString : public DrawBaseCommand
{
public:
	DrawString( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : DrawBaseCommand( pMargin, pParentCommand, COMMAND_STRING ),
		pFont( SmallFont ),
		Color( CR_UNTRANSLATED ),
		ulGapSize( 1 ),
		bUsingTeamColor( false ) { }

	//*************************************************************************
	//
	// [AK] Creates the text that will be drawn on the margin beforehand.
	//
	//*************************************************************************

	virtual void Refresh( const ULONG ulDisplayPlayer )
	{
		for ( unsigned int i = 0; i < PreprocessedStrings.Size( ); i++ )
			V_FreeBrokenLines( PreprocessedStrings[i].pLines );

		PreprocessedStrings.Clear( );

		// [AK] If this command belongs in a team header, create a string for each valid team.
		if ( pParentMargin->GetType( ) == MARGINTYPE_TEAM )
		{
			for ( ULONG ulTeam = 0; ulTeam < TEAM_GetNumAvailableTeams( ); ulTeam++ )
				CreateString( ulDisplayPlayer, ulTeam );
		}
		else
		{
			CreateString( ulDisplayPlayer, ScoreMargin::NO_TEAM );
		}

		DrawBaseCommand::Refresh( ulDisplayPlayer );
	}

	//*************************************************************************
	//
	// [AK] Draws the string on the margin.
	//
	//*************************************************************************

	virtual void Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
	{
		const PreprocessedString *pString = RetrieveString( ulTeam );
		const EColorRange TextColorToUse = bUsingTeamColor ? static_cast<EColorRange>( TEAM_GetTextColor( ulTeam )) : Color;
		const HORIZALIGN_e AlignmentToUse = GetHorizontalAlignment( );
		const fixed_t combinedAlpha = FLOAT2FIXED( fAlpha * fTranslucency );
		TVector2<LONG> Pos = GetDrawingPosition( pString->ulMaxWidth, pString->ulTotalHeight, lXOffsetBonus );

		int clipLeft = ( HUD_GetWidth( ) - pParentMargin->GetWidth( )) / 2;
		int clipWidth = pParentMargin->GetWidth( );
		int clipTop = lYPos;
		int clipHeight = pParentMargin->GetHeight( );

		// [AK] We must take into account the virtual screen's size when setting up the clipping rectangle.
		if ( g_bScale )
			screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidth, clipHeight, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

		for ( unsigned int i = 0; pString->pLines[i].Width >= 0; i++ )
		{
			LONG lActualXPos = Pos.X;

			if ( i > 0 )
				Pos.Y += pFont->GetHeight( ) + ulGapSize;

			if ( AlignmentToUse == HORIZALIGN_CENTER )
				lActualXPos += ( pString->ulMaxWidth - pString->pLines[i].Width ) / 2;
			else if ( AlignmentToUse == HORIZALIGN_RIGHT )
				lActualXPos += pString->ulMaxWidth - pString->pLines[i].Width;

			screen->DrawText( pFont, TextColorToUse, lActualXPos, Pos.Y + lYPos, pString->pLines[i].Text.GetChars( ),
				DTA_UseVirtualScreen, g_bScale,
				DTA_ClipLeft, clipLeft,
				DTA_ClipRight, clipLeft + clipWidth,
				DTA_ClipTop, clipTop,
				DTA_ClipBottom, clipTop + clipHeight,
				DTA_Alpha, combinedAlpha,
				TAG_DONE );
		}
	}

	//*************************************************************************
	//
	// [AK] Gets the largest width of a preprocessed string.
	//
	//*************************************************************************

	virtual ULONG GetContentWidth( const ULONG ulTeam ) const
	{
		PreprocessedString *pString = RetrieveString( ulTeam );
		ULONG ulWidth = pString != NULL ? pString->ulMaxWidth : 0;

		// [AK] Include the right padding only when the width isn't zero.
		if ( ulWidth > 0 )
			ulWidth += ulRightPadding;

		return ulWidth;
	}

	//*************************************************************************
	//
	// [AK] Gets the total height of a preprocessed string.
	//
	//*************************************************************************

	virtual ULONG GetContentHeight( const ULONG ulTeam ) const
	{
		PreprocessedString *pString = RetrieveString( ulTeam );
		ULONG ulHeight = pString != NULL ? pString->ulTotalHeight : 0;

		// [AK] Include the bottom padding only when the height isn't zero.
		if ( ulHeight > 0 )
			ulHeight += ulBottomPadding;

		return ulHeight;
	}

protected:
	enum DRAWSTRINGVALUE_e
	{
		// The value of a CVar.
		DRAWSTRING_CVAR,
		// The name of the current game mode.
		DRAWSTRING_GAMEMODE,
		// The name of the current level.
		DRAWSTRING_LEVELNAME,
		// The lump of the current level.
		DRAWSTRING_LEVELLUMP,
		// The name of the next level.
		DRAWSTRING_NEXTLEVELNAME,
		// The lump of the next level.
		DRAWSTRING_NEXTLEVELLUMP,
		// The name of the current skill.
		DRAWSTRING_SKILLNAME,
		// The time, frags, points, wins, or kills left until the level ends.
		DRAWSTRING_LIMITSTRINGS,
		// The team scores and their relation.
		DRAWSTRING_POINTSTRING,
		// The current player's rank and score.
		DRAWSTRING_PLACESTRING,
		// The amount of time that has passed since the level started.
		DRAWSTRING_LEVELTIME,
		// The amount of time left in the round (or level).
		DRAWSTRING_LEVELTIMELEFT,
		// The amount of time left on the intermission screen.
		DRAWSTRING_INTERMISSIONTIMELEFT,
		// The number of players in the server.
		DRAWSTRING_TOTALPLAYERS,
		// The number of players that are in the game.
		DRAWSTRING_PLAYERSINGAME,
		// The name of a team.
		DRAWSTRING_TEAMNAME,
		// The total number of players on a team.
		DRAWSTRING_TEAMPLAYERCOUNT,
		// The number of players still alive on a team.
		DRAWSTRING_TEAMLIVEPLAYERCOUNT,
		// How many frags this team has.
		DRAWSTRING_TEAMFRAGCOUNT,
		// How many points this team has.
		DRAWSTRING_TEAMPOINTCOUNT,
		// How many wins this team has.
		DRAWSTRING_TEAMWINCOUNT,
		// How many deaths this team has.
		DRAWSTRING_TEAMDEATHCOUNT,
		// The number of true spectators.
		DRAWSTRING_SPECTATORCOUNT,

		DRAWSTRING_STATIC = -1
	};

	struct PreprocessedString
	{
		FBrokenLines *pLines;
		ULONG ulMaxWidth;
		ULONG ulTotalHeight;
	};

	//*************************************************************************
	//
	// [AK] Parses the string, font, or text color, or parses the parameters
	// from the DrawBaseCommand class.
	//
	//*************************************************************************

	virtual void ParseParameter( FScanner &sc, const FName ParameterName, const PARAMETER_e Parameter )
	{
		// [AK] All special values supported by the "DrawString" command.
		const SpecialValueList<DRAWSTRINGVALUE_e> SpecialValues
		{
			{ "cvar",					{ DRAWSTRING_CVAR,					MARGINTYPE_HEADER_OR_FOOTER }},
			{ "gamemode",				{ DRAWSTRING_GAMEMODE,				MARGINTYPE_HEADER_OR_FOOTER }},
			{ "levelname",				{ DRAWSTRING_LEVELNAME,				MARGINTYPE_HEADER_OR_FOOTER }},
			{ "levellump",				{ DRAWSTRING_LEVELLUMP,				MARGINTYPE_HEADER_OR_FOOTER }},
			{ "nextlevelname",			{ DRAWSTRING_NEXTLEVELNAME,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "nextlevellump",			{ DRAWSTRING_NEXTLEVELLUMP,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "skillname",				{ DRAWSTRING_SKILLNAME,				MARGINTYPE_HEADER_OR_FOOTER }},
			{ "limitstrings",			{ DRAWSTRING_LIMITSTRINGS,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "pointstring",			{ DRAWSTRING_POINTSTRING,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "placestring",			{ DRAWSTRING_PLACESTRING,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "leveltime",				{ DRAWSTRING_LEVELTIME,				MARGINTYPE_HEADER_OR_FOOTER }},
			{ "leveltimeleft",			{ DRAWSTRING_LEVELTIMELEFT,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "intermissiontimeleft",	{ DRAWSTRING_INTERMISSIONTIMELEFT,	MARGINTYPE_HEADER_OR_FOOTER }},
			{ "totalplayers",			{ DRAWSTRING_TOTALPLAYERS,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "playersingame",			{ DRAWSTRING_PLAYERSINGAME,			MARGINTYPE_HEADER_OR_FOOTER }},
			{ "teamname",				{ DRAWSTRING_TEAMNAME,				MARGINTYPE_TEAM }},
			{ "teamplayercount",		{ DRAWSTRING_TEAMPLAYERCOUNT,		MARGINTYPE_TEAM }},
			{ "teamliveplayercount",	{ DRAWSTRING_TEAMLIVEPLAYERCOUNT,	MARGINTYPE_TEAM }},
			{ "teamfragcount",			{ DRAWSTRING_TEAMFRAGCOUNT,			MARGINTYPE_TEAM }},
			{ "teampointcount",			{ DRAWSTRING_TEAMPOINTCOUNT,		MARGINTYPE_TEAM }},
			{ "teamwincount",			{ DRAWSTRING_TEAMWINCOUNT,			MARGINTYPE_TEAM }},
			{ "teamdeathcount",			{ DRAWSTRING_TEAMDEATHCOUNT,		MARGINTYPE_TEAM }},
			{ "spectatorcount",			{ DRAWSTRING_SPECTATORCOUNT,		MARGINTYPE_SPECTATOR }},
		};

		switch ( Parameter )
		{
			case PARAMETER_VALUE:
			{
				// [AK] Keep processing the string "chunks", each separated by a '+'.
				do
				{
					const DRAWSTRINGVALUE_e SpecialValue = GetSpecialValue( sc, SpecialValues );

					if ( SpecialValue == DRAWSTRING_CVAR )
					{
						sc.MustGetToken( '(' );
						sc.MustGetToken( TK_Identifier );

						FString CVarName = sc.String;
						FBaseCVar *pCVar = FindCVar( CVarName.GetChars( ), NULL );

						if ( pCVar == NULL )
							sc.ScriptError( "'%s' is not a CVar.", CVarName.GetChars( ));

						sc.MustGetToken( ')' );
						StringChunks.Push( { SpecialValue, CVarName } );
					}
					else
					{
						StringChunks.Push( { SpecialValue, ( SpecialValue != DRAWSTRING_STATIC ) ? "" : sc.String } );
					}

				} while ( sc.CheckToken( '+' ));

				break;
			}

			case PARAMETER_FONT:
			{
				sc.MustGetToken( TK_StringConst );

				// [AK] Throw a fatal error if an empty font name was passed.
				if ( sc.StringLen == 0 )
					sc.ScriptError( "Got an empty string for a font name." );

				pFont = V_GetFont( sc.String );

				// [AK] Throw a fatal error if the font wasn't found.
				if ( pFont == NULL )
					sc.ScriptError( "Couldn't find font '%s'.", sc.String );

				break;
			}

			case PARAMETER_TEXTCOLOR:
			{
				if ( sc.CheckToken( TK_Identifier ))
				{
					// [AK] A team's text colour can be used inside a team header.
					if ( stricmp( sc.String, "teamtextcolor" ) == 0 )
					{
						if ( pParentMargin->GetType( ) != MARGINTYPE_TEAM )
							sc.ScriptError( "'teamtextcolor' can't be used inside a '%s' margin.", pParentMargin->GetName( ));

						bUsingTeamColor = true;
					}
					else
					{
						sc.ScriptError( "Unknown identifier '%s'. Did you mean to use 'teamtextcolor'?", sc.String );
					}
				}
				else
				{
					sc.MustGetToken( TK_StringConst );

					// [AK] If an empty string was passed, inform the user of the error and switch to untranslated.
					if ( sc.StringLen == 0 )
					{
						sc.ScriptMessage( "Got an empty string for a text color, using untranslated instead." );
					}
					else
					{
						Color = V_FindFontColor( sc.String );

						// [AK] If the text color name was invalid, let the user know about it.
						if (( Color == CR_UNTRANSLATED ) && ( stricmp( sc.String, "untranslated" ) != 0 ))
							sc.ScriptMessage( "'%s' is an unknown text color, using untranslated instead.", sc.String );
					}
				}

				break;
			}

			case PARAMETER_GAPSIZE:
			{
				sc.MustGetToken( TK_IntConst );
				ulGapSize = MAX( sc.Number, 0 );

				break;
			}

			default:
				DrawBaseCommand::ParseParameter( sc, ParameterName, Parameter );
				break;
		}
	}

	//*************************************************************************
	//
	// [AK] Processes a string.
	//
	//*************************************************************************

	void CreateString( const ULONG ulDisplayPlayer, const ULONG ulTeam )
	{
		PreprocessedString String;
		FString text;

		// [AK] Create the final string using all of the string chunks.
		for ( unsigned int i = 0; i < StringChunks.Size( ); i++ )
		{
			if ( StringChunks[i].first != DRAWSTRING_STATIC )
			{
				const DRAWSTRINGVALUE_e Value = StringChunks[i].first;

				switch ( Value )
				{
					case DRAWSTRING_CVAR:
					{
						FBaseCVar *pCVar = FindCVar( StringChunks[i].second.GetChars( ), NULL );

						if ( pCVar != NULL )
						{
							FString CVarValue = pCVar->GetGenericRep( CVAR_String ).String;
							V_ColorizeString( CVarValue );

							text += CVarValue;
						}

						break;
					}

					case DRAWSTRING_GAMEMODE:
						text += GAMEMODE_GetCurrentName( );
						break;

					case DRAWSTRING_LEVELNAME:
						text += level.LevelName;
						break;

					case DRAWSTRING_LEVELLUMP:
						text += level.mapname;
						break;

					case DRAWSTRING_NEXTLEVELNAME:
					case DRAWSTRING_NEXTLEVELLUMP:
					{
						// [AK] The next level is only available on the intermission screen.
						if (( gamestate == GS_INTERMISSION ) && ( g_pNextLevel != NULL ))
						{
							if ( Value == DRAWSTRING_NEXTLEVELNAME )
								text += g_pNextLevel->LookupLevelName( );
							else
								text += g_pNextLevel->mapname;
						}
						else
						{
							text += "???";
						}

						break;
					}

					case DRAWSTRING_SKILLNAME:
						text += G_SkillName( );
						break;

					case DRAWSTRING_LIMITSTRINGS:
					{
						std::list<FString> lines;
						SCOREBOARD_BuildLimitStrings( lines, true );

						for ( std::list<FString>::iterator it = lines.begin( ); it != lines.end( ); it++ )
						{
							if ( it != lines.begin( ))
								text += '\n';

							text += *it;
						}

						break;
					}

					case DRAWSTRING_POINTSTRING:
						text += HUD_BuildPointString( );
						break;

					case DRAWSTRING_PLACESTRING:
						text += HUD_BuildPlaceString( ulDisplayPlayer );
						break;

					case DRAWSTRING_LEVELTIME:
					case DRAWSTRING_LEVELTIMELEFT:
					{
						if ( Value == DRAWSTRING_LEVELTIME )
						{
							// [AK] The level time is only active while in the level.
							if ( gamestate == GS_LEVEL )
							{
								const int levelTime = level.time / TICRATE;
								text.AppendFormat( "%02d:%02d:%02d", levelTime / 3600, ( levelTime % 3600 ) / 60, levelTime % 60 );

								break;
							}
						}
						else
						{
							// [AK] Make sure that the time limit is active right now.
							if ( GAMEMODE_IsTimelimitActive( ))
							{
								FString TimeLeft;
								GAMEMODE_GetTimeLeftString( TimeLeft );

								text += TimeLeft;
								break;
							}
						}

						text += "00:00:00";
						break;
					}

					case DRAWSTRING_INTERMISSIONTIMELEFT:
						text.AppendFormat( "%ld", ( gamestate == GS_INTERMISSION ) ? WI_GetStopWatch( ) / TICRATE + 1 : 0 );
						break;

					case DRAWSTRING_TOTALPLAYERS:
						text.AppendFormat( "%lu", SERVER_CountPlayers( true ));
						break;

					case DRAWSTRING_PLAYERSINGAME:
						text.AppendFormat( "%lu", HUD_GetNumPlayers( ));
						break;

					case DRAWSTRING_TEAMNAME:
						text += TEAM_GetName( ulTeam );
						break;

					case DRAWSTRING_TEAMPLAYERCOUNT:
						text.AppendFormat( "%lu", TEAM_CountPlayers( ulTeam ));
						break;

					case DRAWSTRING_TEAMLIVEPLAYERCOUNT:
						text.AppendFormat( "%lu", TEAM_CountLivingAndRespawnablePlayers( ulTeam ));
						break;

					case DRAWSTRING_TEAMFRAGCOUNT:
						text.AppendFormat( "%ld", TEAM_GetFragCount( ulTeam ));
						break;

					case DRAWSTRING_TEAMPOINTCOUNT:
						text.AppendFormat( "%ld", TEAM_GetPointCount( ulTeam ));
						break;

					case DRAWSTRING_TEAMWINCOUNT:
						text.AppendFormat( "%ld", TEAM_GetWinCount( ulTeam ));
						break;

					case DRAWSTRING_TEAMDEATHCOUNT:
						text.AppendFormat( "%ld", TEAM_GetDeathCount( ulTeam ));
						break;

					case DRAWSTRING_SPECTATORCOUNT:
						text.AppendFormat( "%lu", HUD_GetNumSpectators( ));
						break;

					default:
						break;
				}
			}
			else
			{
				text += StringChunks[i].second;
			}
		}

		// [AK] The x-offset must be accounted for when determining the largest possible line width.
		// If the string is aligned to the center, then the x-offset must be doubled.
		const ULONG ulMaxWidth = pParentMargin->GetWidth( ) - ( HorizontalAlignment == HORIZALIGN_CENTER ? 2 : 1 ) * abs( lXOffset );
		String.pLines = V_BreakLines( pFont, ulMaxWidth, text.GetChars( ));
		String.ulMaxWidth = 0;
		String.ulTotalHeight = 0;

		// [AK] Determine the total height of the string.
		for ( unsigned int i = 0; String.pLines[i].Width >= 0; i++ )
		{
			if ( i > 0 )
				String.ulTotalHeight += ulGapSize;

			String.ulMaxWidth = MAX<ULONG>( String.ulMaxWidth, String.pLines[i].Width );
			String.ulTotalHeight += pFont->GetHeight( );
		}

		PreprocessedStrings.Push( String );
	}

	//*************************************************************************
	//
	// [AK] Returns a pointer to a preprocessed string belonging to a specific
	// team (or no team). Includes checks to ensure that the string exists.
	//
	//*************************************************************************

	PreprocessedString *RetrieveString( const ULONG ulTeam ) const
	{
		if ( ulTeam == ScoreMargin::NO_TEAM )
		{
			// [AK] If there's no string at all, then something went wrong.
			if ( PreprocessedStrings.Size( ) == 0 )
				I_Error( "DrawString::RetrieveString: there is no string to retrieve." );

			return &PreprocessedStrings[0];
		}
		else
		{
			// [AK] If this team has no string, then something went wrong.
			if ( ulTeam >= PreprocessedStrings.Size( ))
				I_Error( "DrawString::RetrieveString: there is no string to retrieve for team %lu.", ulTeam );

			return &PreprocessedStrings[ulTeam];
		}
	}

	TArray<std::pair<DRAWSTRINGVALUE_e, FString>> StringChunks;
	TArray<PreprocessedString> PreprocessedStrings;
	FFont *pFont;
	EColorRange Color;
	ULONG ulGapSize;
	bool bUsingTeamColor;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] DrawColor
//
// Draws a rectangular box of a color somewhere in the margin.
//
//*****************************************************************************
//*****************************************************************************

class DrawColor : public DrawBaseCommand
{
public:
	DrawColor( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : DrawBaseCommand( pMargin, pParentCommand, COMMAND_COLOR ),
		ValueType( DRAWCOLOR_STATIC ),
		Color( 0 ),
		ulWidth( 0 ),
		ulHeight( 0 ) { }

	//*************************************************************************
	//
	// [AK] Draws the color box on the margin.
	//
	//*************************************************************************

	virtual void Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
	{
		const ULONG ulWidthToUse = MIN( ulWidth, pParentMargin->GetWidth( ) - abs( lXOffset + lXOffsetBonus ));
		const TVector2<LONG> Pos = GetDrawingPosition( ulWidthToUse, ulHeight, lXOffsetBonus );
		const PalEntry ColorToDraw = ( ValueType == DRAWCOLOR_TEAMCOLOR ) ? PalEntry( TEAM_GetColor( ulTeam )) : Color;
		const LONG lMarginLeftXPos = ( HUD_GetWidth( ) - pParentMargin->GetWidth( )) / 2;

		// [AK] The color box can't be drawn past the left or right sides of the margin.
		int clipLeft = MAX<int>( Pos.X, lMarginLeftXPos );
		int clipWidth = MIN<int>( ulWidthToUse, lMarginLeftXPos + pParentMargin->GetWidth( ) - clipLeft );
		int clipTop = Pos.Y + lYPos;
		int clipHeight = ulHeight;

		// [AK] We must take into account the virtual screen's size when setting up the clipping rectangle.
		if ( g_bScale )
			screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidth, clipHeight, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

		screen->Dim( ColorToDraw, fAlpha * fTranslucency, clipLeft, clipTop, clipWidth, clipHeight );
	}

	//*************************************************************************
	//
	// [AK] Returns the width/height of the color box.
	//
	//*************************************************************************

	virtual ULONG GetContentWidth( const ULONG ulTeam ) const { return MIN( ulWidth + ulRightPadding, pParentMargin->GetWidth( )); }
	virtual ULONG GetContentHeight( const ULONG ulTeam ) const { return ulHeight + ulBottomPadding; }

protected:
	enum DRAWCOLORVALUE_e
	{
		// The color of a team.
		DRAWCOLOR_TEAMCOLOR,

		DRAWCOLOR_STATIC = -1
	};

	//*************************************************************************
	//
	// [AK] Parses the color, width, or height, or parses any parameters
	// from the DrawBaseCommand class.
	//
	//*************************************************************************

	virtual void ParseParameter( FScanner &sc, const FName ParameterName, const PARAMETER_e Parameter )
	{
		// [AK] All special values supported by the "DrawColor" command.
		const SpecialValueList<DRAWCOLORVALUE_e> SpecialValues
		{
			{ "teamcolor",	{ DRAWCOLOR_TEAMCOLOR,	MARGINTYPE_TEAM }},
		};

		switch ( Parameter )
		{
			case PARAMETER_VALUE:
			{
				ValueType = GetSpecialValue( sc, SpecialValues );

				if ( ValueType == DRAWCOLOR_STATIC )
				{
					FString ColorString = V_GetColorStringByName( sc.String );
					Color = V_GetColorFromString( NULL, ColorString.IsNotEmpty( ) ? ColorString.GetChars( ) : sc.String );
				}

				break;
			}

			case PARAMETER_WIDTH:
			case PARAMETER_HEIGHT:
			{
				sc.MustGetToken( TK_IntConst );

				if ( Parameter == PARAMETER_WIDTH )
					ulWidth = MAX( sc.Number, 1 );
				else
					ulHeight = MAX( sc.Number, 1 );

				break;
			}

			default:
				DrawBaseCommand::ParseParameter( sc, ParameterName, Parameter );
				break;
		}
	}

	DRAWCOLORVALUE_e ValueType;
	PalEntry Color;
	ULONG ulWidth;
	ULONG ulHeight;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] DrawTexture
//
// Draws a graphic or image somewhere in the margin.
//
//*****************************************************************************
//*****************************************************************************

class DrawTexture : public DrawBaseCommand
{
public:
	DrawTexture( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : DrawBaseCommand( pMargin, pParentCommand, COMMAND_TEXTURE ),
		ValueType( DRAWTEXTURE_STATIC ),
		pTexture( NULL ) { }

	//*************************************************************************
	//
	// [AK] Draws the texture on the margin.
	//
	//*************************************************************************

	virtual void Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
	{
		FTexture *pTextureToDraw = RetrieveTexture( ulTeam );

		// [AK] Stop here if the texture doesn't exist for some reason.
		if ( pTextureToDraw == NULL )
			return;

		const TVector2<LONG> Pos = GetDrawingPosition( pTextureToDraw->GetScaledWidth( ), pTextureToDraw->GetScaledHeight( ), lXOffsetBonus );

		int clipLeft = ( HUD_GetWidth( ) - pParentMargin->GetWidth( )) / 2;
		int clipWidth = pParentMargin->GetWidth( );
		int clipTop = lYPos;
		int clipHeight = pParentMargin->GetHeight( );

		// [AK] We must take into account the virtual screen's size when setting up the clipping rectangle.
		if ( g_bScale )
			screen->VirtualToRealCoordsInt( clipLeft, clipTop, clipWidth, clipHeight, con_virtualwidth, con_virtualheight, false, !con_scaletext_usescreenratio );

		screen->DrawTexture( pTextureToDraw, Pos.X, Pos.Y + lYPos,
			DTA_UseVirtualScreen, g_bScale,
			DTA_ClipLeft, clipLeft,
			DTA_ClipRight, clipLeft + clipWidth,
			DTA_ClipTop, clipTop,
			DTA_ClipBottom, clipTop + clipHeight,
			DTA_Alpha, FLOAT2FIXED( fAlpha * fTranslucency ),
			TAG_DONE );
	}

	//*************************************************************************
	//
	// [AK] Gets the width of a texture (for a team).
	//
	//*************************************************************************

	virtual ULONG GetContentWidth( const ULONG ulTeam ) const
	{
		FTexture *pTexture = RetrieveTexture( ulTeam );
		return pTexture != NULL ? pTexture->GetScaledWidth( ) + ulRightPadding : 0;
	}

	//*************************************************************************
	//
	// [AK] Gets the height of a texture (for a team).
	//
	//*************************************************************************

	virtual ULONG GetContentHeight( const ULONG ulTeam ) const
	{
		FTexture *pTexture = RetrieveTexture( ulTeam );
		return pTexture != NULL ? pTexture->GetScaledHeight( ) + ulBottomPadding : 0;
	}

protected:
	enum DRAWTEXTUREVALUE_e
	{
		// The logo of a team.
		DRAWTEXTURE_TEAMLOGO,

		DRAWTEXTURE_STATIC = -1
	};

	//*************************************************************************
	//
	// [AK] Parses the texture, also making sure that it's valid, or parses
	// any parameters from the DrawBaseCommand class.
	//
	//*************************************************************************

	virtual void ParseParameter( FScanner &sc, const FName ParameterName, const PARAMETER_e Parameter )
	{
		// [AK] All special values supported by the "DrawTexture" command.
		const SpecialValueList<DRAWTEXTUREVALUE_e> SpecialValues
		{
			{ "teamlogo",	{ DRAWTEXTURE_TEAMLOGO,		MARGINTYPE_TEAM }},
		};

		if ( Parameter == PARAMETER_VALUE )
		{
			ValueType = GetSpecialValue( sc, SpecialValues );

			if ( ValueType == DRAWTEXTURE_STATIC )
			{
				pTexture = TexMan.FindTexture( sc.String );

				// [AK] If the texture wasn't found, throw a fatal error.
				if ( pTexture == NULL )
					sc.ScriptError( "Couldn't find texture '%s'.", sc.String );
			}
		}
		else
		{
			DrawBaseCommand::ParseParameter( sc, ParameterName, Parameter );
		}
	}

	//*************************************************************************
	//
	// [AK] Returns a pointer to the texture that should be used.
	//
	//*************************************************************************

	FTexture *RetrieveTexture( const ULONG ulTeam ) const
	{
		if ( ulTeam != ScoreMargin::NO_TEAM )
		{
			if (( ulTeam < teams.Size( )) && ( ValueType == DRAWTEXTURE_TEAMLOGO ))
				return TexMan.FindTexture( teams[ulTeam].Logo );
		}
		else
		{
			return pTexture;
		}

		return NULL;
	}

	DRAWTEXTUREVALUE_e ValueType;
	FTexture *pTexture;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] FlowControlBaseCommand
//
// An abstract class for all margin commands that evaluate a condition, which
// then execute all commands nested inside an "if" block when the condition
// evaluates to true, or an "else" block when it evaluates to false.
//
//*****************************************************************************
//*****************************************************************************

class FlowControlBaseCommand : public ScoreMargin::BaseCommand
{
public:
	FlowControlBaseCommand( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : BaseCommand( pMargin, pParentCommand ), bResult( false ) { }

	//*************************************************************************
	//
	// [AK] Parses new margin commands inside the "if" or "else" blocks.
	//
	//*************************************************************************

	virtual void Parse( FScanner &sc )
	{
		sc.MustGetToken( ')' );
		ParseBlock( sc, true );

		if ( sc.CheckToken( TK_Else ))
			ParseBlock( sc, false );
	}

	//*************************************************************************
	//
	// [AK] Gets the result of the flow control command's condition, and uses
	// that result to determine which block of commands should be refreshed.
	//
	//*************************************************************************

	virtual void Refresh( const ULONG ulDisplayPlayer )
	{
		bResult = EvaluateCondition( ulDisplayPlayer );
		Blocks[bResult].Refresh( ulDisplayPlayer );
	}

	//*************************************************************************
	//
	// [AK] Draws the block of commands that corresponds to the result of the
	// flow control command's condition.
	//
	//*************************************************************************

	virtual void Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
	{
		Blocks[bResult].Draw( ulDisplayPlayer, ulTeam, lYPos, fAlpha, lXOffsetBonus );
	}

protected:
	virtual bool EvaluateCondition( const ULONG ulDisplayPlayer ) = 0;

private:

	//*************************************************************************
	//
	// [AK] Parses the "true" or "false" blocks.
	//
	//*************************************************************************

	void ParseBlock( FScanner &sc, const bool bWhichBlock )
	{
		Blocks[bWhichBlock].ParseCommands( sc, pParentMargin, pParentCommand );

		// [AK] There needs to be at least one command inside the block.
		if ( Blocks[bWhichBlock].HasCommands( ) == false )
			sc.ScriptError( "This flow control command has no commands inside a block!" );
	}

	ScoreMargin::CommandBlock Blocks[2];
	bool bResult;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] TrueOrFalseFlowControl
//
// This class handles these margin commands:
//
// - IfOnlineGame: if the current game is a network game.
// - IfIntermission: if the intermission screen is being shown.
// - IfPlayersOnTeams: if players are supposed to be on teams.
// - IfPlayersHaveLives: if players are supposed to have lives.
// - IfShouldShowRank: if the current player's rank should be shown.
//
// These commands accept one boolean parameter that inverts the condition
// (i.e. the "if" block will be executed when the condition is false).
//
//*****************************************************************************
//*****************************************************************************

class TrueOrFalseFlowControl : public FlowControlBaseCommand
{
public:
	TrueOrFalseFlowControl( ScoreMargin *pMargin, BaseCommand *pParentCommand, MARGINCMD_e Command ) : FlowControlBaseCommand( pMargin, pParentCommand ),
		CommandType( Command ),
		bMustBeTrue( false )
	{
		// [AK] If the command type isn't one of these listed here, throw an error.
		if (( CommandType != MARGINCMD_IFONLINEGAME ) &&
			( CommandType != MARGINCMD_IFINTERMISSION ) &&
			( CommandType != MARGINCMD_IFPLAYERSONTEAMS ) &&
			( CommandType != MARGINCMD_IFPLAYERSHAVELIVES ) &&
			( CommandType != MARGINCMD_IFSHOULDSHOWRANK ))
		{
			if (( CommandType >= 0 ) && ( CommandType < NUM_MARGINCMDS ))
			{
				FString CommandName = GetStringMARGINCMD_e( CommandType ) + strlen( "MARGINCMD_" );
				CommandName.ToLower( );

				I_Error( "TrueOrFalseFlowControlBaseCommand: margin command '%s' cannot be used.", CommandName.GetChars( ));
			}
			else
			{
				I_Error( "TrueOrFalseFlowControlBaseCommand: an unknown margin command was used." );
			}
		}
	}

	//*************************************************************************
	//
	// [AK] Checks if the parameter is "true", "false", or something else.
	//
	//*************************************************************************

	virtual void Parse( FScanner &sc )
	{
		sc.MustGetString( );

		if ( stricmp( sc.String, "true" ) == 0 )
			bMustBeTrue = true;
		else if ( stricmp( sc.String, "false" ) == 0 )
			bMustBeTrue = false;
		else
			bMustBeTrue = !!atoi( sc.String );

		FlowControlBaseCommand::Parse( sc );
	}

protected:

	//*************************************************************************
	//
	// [AK] Checks if the command's condition evaluates to true or false.
	//
	//*************************************************************************

	virtual bool EvaluateCondition( const ULONG ulDisplayPlayer )
	{
		bool bValue = false;

		switch ( CommandType )
		{
			case MARGINCMD_IFONLINEGAME:
				bValue = NETWORK_InClientMode( );
				break;

			case MARGINCMD_IFINTERMISSION:
				bValue = ( gamestate == GS_INTERMISSION );
				break;

			case MARGINCMD_IFPLAYERSONTEAMS:
				bValue = !!( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS );
				break;

			case MARGINCMD_IFPLAYERSHAVELIVES:
				bValue = !!( GAMEMODE_GetCurrentFlags( ) & GMF_USEMAXLIVES );
				break;

			case MARGINCMD_IFSHOULDSHOWRANK:
				bValue = HUD_ShouldDrawRank( ulDisplayPlayer );
				break;

			default:
				break;
		}

		return ( bValue == bMustBeTrue );
	}

	const MARGINCMD_e CommandType;
	bool bMustBeTrue;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] IfGameModeFlowControl
//
// Executes a block when any of the given game modes are being played.
//
//*****************************************************************************
//*****************************************************************************

class IfGameModeFlowControl : public FlowControlBaseCommand
{
public:
	IfGameModeFlowControl( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : FlowControlBaseCommand( pMargin, pParentCommand ) { }

	//*************************************************************************
	//
	// [AK] Parses the game mode list.
	//
	//*************************************************************************

	virtual void Parse( FScanner &sc )
	{
		do
		{
			sc.MustGetToken( TK_Identifier );
			GameModeList.insert( static_cast<GAMEMODE_e>( sc.MustGetEnumName( "game mode", "GAMEMODE_", GetValueGAMEMODE_e, true )));

		} while ( sc.CheckToken( ',' ));

		FlowControlBaseCommand::Parse( sc );
	}

protected:

	//*************************************************************************
	//
	// [AK] Checks if the current game mode is on the list.
	//
	//*************************************************************************

	virtual bool EvaluateCondition( const ULONG ulDisplayPlayer )
	{
		return ( GameModeList.find( GAMEMODE_GetCurrentMode( )) != GameModeList.end( ));
	}

	std::set<GAMEMODE_e> GameModeList;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] IfGameOrEarnTypeFlowControl
//
// This class handles these margin commands:
//
// - IfGameType: if any of the listed game types are being played.
// - IfEarnType: if any of the listed earn types are being played.
//
//*****************************************************************************
//*****************************************************************************

class IfGameOrEarnTypeFlowControl : public FlowControlBaseCommand
{
public:
	IfGameOrEarnTypeFlowControl( ScoreMargin *pMargin, BaseCommand *pParentCommand, const bool bIsGameType ) : FlowControlBaseCommand( pMargin, pParentCommand ),
		bIsGameTypeCommand( bIsGameType ),
		ulFlags( 0 ) { }

	//*************************************************************************
	//
	// [AK] Parses a list of game types or earn types that this command requires.
	//
	//*************************************************************************

	virtual void Parse ( FScanner &sc )
	{
		do
		{
			sc.MustGetToken( TK_Identifier );

			if ( bIsGameTypeCommand )
			{
				ULONG ulFlag = sc.MustGetEnumName( "game type", "GMF_", GetValueGMF, true );

				if (( ulFlag & GAMETYPE_MASK ) == 0 )
					sc.ScriptError( "You must only use COOPERATIVE, DEATHMATCH, or TEAMGAME. Using '%s' is invalid.", sc.String );

				ulFlags |= ulFlag;
			}
			else
			{
				ulFlags |= sc.MustGetEnumName( "earn type", "GMF_PLAYERSEARN", GetValueGMF, true );
			}

		} while ( sc.CheckToken( ',' ));

		FlowControlBaseCommand::Parse( sc );
	}

protected:

	//*************************************************************************
	//
	// [AK] Checks if the current game mode supports the same game type or earn
	// type as required by this command.
	//
	//*************************************************************************

	virtual bool EvaluateCondition( const ULONG ulDisplayPlayer )
	{
		return !!( GAMEMODE_GetCurrentFlags( ) & ulFlags );
	}

	const bool bIsGameTypeCommand;
	ULONG ulFlags;
};

//*****************************************************************************
//*****************************************************************************
//
// [AK] IfCVarFlowControl
//
// Executes a block depending on a CVar's value.
//
//*****************************************************************************
//*****************************************************************************

class IfCVarFlowControl : public FlowControlBaseCommand
{
public:
	IfCVarFlowControl( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : FlowControlBaseCommand( pMargin, pParentCommand ),
		pCVar( NULL ),
		Operator( OPERATOR_EQUAL ) { }

	//*************************************************************************
	//
	// [AK] Gets the CVar, the operator, and the value to compare with.
	//
	//*************************************************************************

	virtual void Parse( FScanner &sc )
	{
		sc.MustGetToken( TK_Identifier );
		pCVar = FindCVar( sc.String, NULL );

		if ( pCVar == NULL )
			sc.ScriptError( "'%s' is not a CVar.", sc.String );

		// [AK] Check which operator to use.
		if ( sc.CheckToken( TK_Eq ))
			Operator = OPERATOR_EQUAL;
		else if ( sc.CheckToken( TK_Neq ))
			Operator = OPERATOR_NOT_EQUAL;
		else if ( sc.CheckToken( '>' ))
			Operator = OPERATOR_GREATER;
		else if ( sc.CheckToken( TK_Geq ))
			Operator = OPERATOR_GREATER_OR_EQUAL;
		else if ( sc.CheckToken( '<' ))
			Operator = OPERATOR_LESS;
		else if ( sc.CheckToken( TK_Leq ))
			Operator = OPERATOR_LESS_OR_EQUAL;
		else
			sc.ScriptError( "Invalid or missing operator." );

		// [AK] Scan the value to be compared, depending on the CVar's data type. To keep
		// everything simple, values for non-string CVars are saved as floats.
		switch ( pCVar->GetRealType( ))
		{
			case CVAR_Int:
				sc.MustGetNumber( );
				Val.SetValue<float>( static_cast<float>( sc.Number ));
				break;

			case CVAR_Bool:
			case CVAR_Dummy:
			{
				bool bValue = false;

				if ( sc.CheckToken( TK_True ))
				{
					bValue = true;
				}
				else if ( sc.CheckToken( TK_False ))
				{
					bValue = false;
				}
				else
				{
					sc.MustGetNumber( );
					bValue = !!sc.Number;
				}

				Val.SetValue<float>( static_cast<float>( bValue ));
				break;
			}

			case CVAR_Float:
				sc.MustGetFloat( );
				Val.SetValue<float>( static_cast<float>( sc.Float ));
				break;

			case CVAR_String:
				sc.MustGetToken( TK_StringConst );
				Val.SetValue<const char *>( sc.String );
				break;

			default:
				sc.ScriptError( "CVar '%s' uses an invalid data type.", pCVar->GetName( ));
				break;
		}

		FlowControlBaseCommand::Parse( sc );
	}

protected:
	enum OPERATOR_TYPE_e
	{
		OPERATOR_EQUAL,
		OPERATOR_NOT_EQUAL,
		OPERATOR_GREATER,
		OPERATOR_GREATER_OR_EQUAL,
		OPERATOR_LESS,
		OPERATOR_LESS_OR_EQUAL,
	};

	//*************************************************************************
	//
	// [AK] Compares the CVar's current value with the other value.
	//
	//*************************************************************************

	virtual bool EvaluateCondition( const ULONG ulDisplayPlayer )
	{
		float fResult = 0.0f;

		// [AK] For all non-string CVars, the values are saved as floats.
		if ( pCVar->GetRealType( ) == CVAR_String )
			fResult = static_cast<float>( strcmp( pCVar->GetGenericRep( CVAR_String ).String, Val.GetValue<const char *>( )));
		else
			fResult = pCVar->GetGenericRep( CVAR_Float ).Float - Val.GetValue<float>( );

		switch ( Operator )
		{
			case OPERATOR_EQUAL:
				return fResult == 0.0f;

			case OPERATOR_NOT_EQUAL:
				return fResult != 0.0f;

			case OPERATOR_GREATER:
				return fResult > 0.0f;

			case OPERATOR_GREATER_OR_EQUAL:
				return fResult >= 0.0f;

			case OPERATOR_LESS:
				return fResult < 0.0f;

			case OPERATOR_LESS_OR_EQUAL:
				return fResult <= 0.0f;
		}

		// [AK] In case we reach here, just return false.
		return false;
	}

	FBaseCVar *pCVar;
	PlayerValue Val;
	OPERATOR_TYPE_e Operator;
};

//*****************************************************************************
//	FUNCTIONS

//*****************************************************************************
//
// [AK] ScoreMargin::BaseCommand::BaseCommand
//
// Initializes a margin command.
//
//*****************************************************************************

ScoreMargin::BaseCommand::BaseCommand( ScoreMargin *pMargin, BaseCommand *pParentCommand ) : pParentMargin( pMargin ), pParentCommand( pParentCommand )
{
	// [AK] This should never happen, but throw a fatal error if it does.
	if ( pParentMargin == NULL )
		I_Error( "ScoreMargin::BaseCommand: parent margin is NULL." );
}

//*****************************************************************************
//
// [AK] ScoreMargin::CommandBlock::ParseCommand
//
// A "factory" function that's starts a block and creates new margin commands.
//
//*****************************************************************************

void ScoreMargin::CommandBlock::ParseCommands( FScanner &sc, ScoreMargin *pMargin, BaseCommand *pParentCommand )
{
	Commands.Clear( );
	sc.MustGetToken( '{' );

	while ( sc.CheckToken( '}' ) == false )
	{
		const MARGINCMD_e Command = static_cast<MARGINCMD_e>( sc.MustGetEnumName( "margin command", "MARGINCMD_", GetValueMARGINCMD_e ));
		BaseCommand *pNewCommand = NULL;

		switch ( Command )
		{
			case MARGINCMD_MULTILINEBLOCK:
				pNewCommand = new MultiLineBlock( pMargin, pParentCommand );
				break;

			case MARGINCMD_ROWBLOCK:
				pNewCommand = new RowBlock( pMargin, pParentCommand );
				break;

			case MARGINCMD_DRAWSTRING:
				pNewCommand = new DrawString( pMargin, pParentCommand );
				break;

			case MARGINCMD_DRAWCOLOR:
				pNewCommand = new DrawColor( pMargin, pParentCommand );
				break;

			case MARGINCMD_DRAWTEXTURE:
				pNewCommand = new DrawTexture( pMargin, pParentCommand );
				break;

			case MARGINCMD_IFONLINEGAME:
			case MARGINCMD_IFINTERMISSION:
			case MARGINCMD_IFPLAYERSONTEAMS:
			case MARGINCMD_IFPLAYERSHAVELIVES:
			case MARGINCMD_IFSHOULDSHOWRANK:
				pNewCommand = new TrueOrFalseFlowControl( pMargin, pParentCommand, Command );
				break;

			case MARGINCMD_IFGAMEMODE:
				pNewCommand = new IfGameModeFlowControl( pMargin, pParentCommand );
				break;

			case MARGINCMD_IFGAMETYPE:
			case MARGINCMD_IFEARNTYPE:
				pNewCommand = new IfGameOrEarnTypeFlowControl( pMargin, pParentCommand, Command == MARGINCMD_IFGAMETYPE );
				break;

			case MARGINCMD_IFCVAR:
				pNewCommand = new IfCVarFlowControl( pMargin, pParentCommand );
				break;

			default:
				sc.ScriptError( "Couldn't create margin command '%s'.", sc.String );
				break;
		}

		// [AK] A command's arguments must always be prepended by a '('.
		sc.MustGetToken( '(' );
		pNewCommand->Parse( sc );

		Commands.Push( pNewCommand );
	}
}

//*****************************************************************************
//
// [AK] ScoreMargin::CommandBlock::Clear
//
// Deletes all of the commands from memory.
//
//*****************************************************************************

void ScoreMargin::CommandBlock::Clear( void )
{
	for ( unsigned int i = 0; i < Commands.Size( ); i++ )
	{
		delete Commands[i];
		Commands[i] = NULL;
	}

	Commands.Clear( );
}

//*****************************************************************************
//
// [AK] ScoreMargin::CommandBlock::Refresh
//
// Invokes the refresh function on all of the listed commands.
//
//*****************************************************************************

void ScoreMargin::CommandBlock::Refresh( const ULONG ulDisplayPlayer )
{
	for ( unsigned int i = 0; i < Commands.Size( ); i++ )
		Commands[i]->Refresh( ulDisplayPlayer );
}

//*****************************************************************************
//
// [AK] ScoreMargin::CommandBlock::Draw
//
// Invokes the draw function on all of the listed commands.
//
//*****************************************************************************

void ScoreMargin::CommandBlock::Draw( const ULONG ulDisplayPlayer, const ULONG ulTeam, const LONG lYPos, const float fAlpha, const LONG lXOffsetBonus ) const
{
	for ( unsigned int i = 0; i < Commands.Size( ); i++ )
		Commands[i]->Draw( ulDisplayPlayer, ulTeam, lYPos, fAlpha, lXOffsetBonus );
}

//*************************************************************************
//
// [AK] ElementBaseCommand::Refresh
//
// Ensures that the margin can fit the contents (for all teams).
//
//*************************************************************************

void ElementBaseCommand::Refresh( const ULONG ulDisplayPlayer )
{
	// [AK] Only do this if the command isn't nested inside a MultiLineBlock or RowBlock command.
	// Otherwise, add this command to the latter's draw list.
	if (( pParentCommand == NULL ) || ( pParentCommand->IsBlockElement( ) == false ))
	{
		if ( pParentMargin->GetType( ) == MARGINTYPE_TEAM )
		{
			for ( ULONG ulTeam = 0; ulTeam < TEAM_GetNumAvailableTeams( ); ulTeam++ )
				EnsureContentFitsInMargin( GetContentHeight( ulTeam ));
		}
		else
		{
			EnsureContentFitsInMargin( GetContentHeight( ScoreMargin::NO_TEAM ));
		}
	}
	else
	{
		static_cast<BlockBaseCommand *>( pParentCommand )->AddToDrawList( this );
	}
}

//*****************************************************************************
//
// [AK] ScoreMargin::ScoreMargin
//
// Initializes a margin's members to their default values.
//
//*****************************************************************************

ScoreMargin::ScoreMargin( MARGINTYPE_e MarginType, const char *pszName ) :
	Type( MarginType ),
	Name( pszName ),
	ulWidth( 0 ),
	ulHeight( 0 ) { }

//*****************************************************************************
//
// [AK] ScoreMargin::Parse
//
// Parses a margin (e.g. "mainheader", "teamheader", "spectatorheader", or
// "footer") block in SCORINFO.
//
//*****************************************************************************

void ScoreMargin::Parse( FScanner &sc )
{
	Block.ParseCommands( sc, this, NULL );
}

//*****************************************************************************
//
// [AK] ScoreMargin::Refresh
//
// Updates the margin's width and height, then refreshes all commands.
//
//*****************************************************************************

void ScoreMargin::Refresh( const ULONG ulDisplayPlayer, const ULONG ulNewWidth )
{
	// [AK] If there's no commands, then don't do anything.
	if ( Block.HasCommands( ) == false )
		return;

	// [AK] Never accept a width of zero, throw a fatal error if this happens.
	if ( ulNewWidth == 0 )
		I_Error( "ScoreMargin::Refresh: tried assigning a width of zero to '%s'.", GetName( ));

	ulWidth = ulNewWidth;
	ulHeight = 0;

	Block.Refresh( ulDisplayPlayer );
}

//*****************************************************************************
//
// [AK] ScoreMargin::Render
//
// Draws all commands that are defined inside the margin.
//
//*****************************************************************************

void ScoreMargin::Render( const ULONG ulDisplayPlayer, const ULONG ulTeam, LONG &lYPos, const float fAlpha ) const
{
	// [AK] If this is supposed to be a team header, then we can't draw for invalid teams!
	if ( Type == MARGINTYPE_TEAM )
	{
		if ( ulTeam == NO_TEAM )
			I_Error( "ScoreMargin::Render: '%s' can't be drawn for no team.", GetName( ));
		else if ( ulTeam >= teams.Size( ))
			I_Error( "ScoreMargin::Render: '%s' can't be drawn for an invalid team (%lu).", GetName( ), ulTeam );
	}
	// [AK] Otherwise, if this is a non-team header, then we can't draw for any specific team!
	else if ( ulTeam != NO_TEAM )
	{
		I_Error( "ScoreMargin::Render: '%s' must not be drawn for any specific team (%lu).", GetName( ), ulTeam );
	}

	// [AK] If there's no commands, or the width or height are zero, then we can't draw anything.
	if (( Block.HasCommands( ) == false ) || ( ulWidth == 0 ) || ( ulHeight == 0 ))
		return;

	Block.Draw( ulDisplayPlayer, ulTeam, lYPos, fAlpha );
	lYPos += ulHeight;
}

//*****************************************************************************
//
// [AK] scoreboard_TryToPrependLimit
//
// Checks if there's already a limit string on the list, removes it from the
// list, then prepends it to the string we passed into the function.
//
//*****************************************************************************

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
//
// [RC] scoreboard_AddSingleLimit
//
// Helper method for SCOREBOARD_BuildLimitStrings. Creates a "x things remaining"
// message. [AK] Added the bWantToPrepend parameter.
//
//*****************************************************************************

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
//
// [AK] scoreboard_AddTimeLimit
//
// Creates the time limit message to be shown on the scoreboard or server console.
//
//*****************************************************************************

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
// [AK] SCOREBOARD_BuildLimitStrings
//
// Builds the series of "x frags left / 3rd match between the two / 15:10 remain"
// strings. Used here and in serverconsole.cpp
//
//*****************************************************************************

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
// [AK] scoreboard_GetScoreLeft
//
// Helper function for SCOREBOARD_GetLeftToLimit that checks which team (in
// team-based game modes) or player has the highest score, then returns the
// difference between it and the desired game limit.
//
//*****************************************************************************

template <typename Type>
LONG scoreboard_GetScoreLeft( FIntCVar &LimitCVar, LONG ( *pTeamGetterFunc )( void ), Type player_t::*pScoreCount )
{
	LONG lHighestScore;

	if ( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS )
	{
		lHighestScore = pTeamGetterFunc( );
	}
	else
	{
		lHighestScore = INT_MIN;

		for ( ULONG ulIdx = 0; ulIdx < MAXPLAYERS; ulIdx++ )
		{
			if (( playeringame[ulIdx] ) && ( players[ulIdx].bSpectating == false ) && ( static_cast<LONG>( players[ulIdx].*pScoreCount ) > lHighestScore ))
				lHighestScore = players[ulIdx].*pScoreCount;
		}
	}

	return LimitCVar - lHighestScore;
}

//*****************************************************************************
//
// SCOREBOARD_GetLeftToLimit
//
// Gets how much score is left to any game limits (e.g. frags, points, or wins).
//
//*****************************************************************************

LONG SCOREBOARD_GetLeftToLimit( void )
{
	// If we're not in a level, then clearly there's no need for this.
	if ( gamestate != GS_LEVEL )
		return 0;

	// KILL-based mode. [BB] This works indepently of any players in game.
	if ( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNKILLS )
	{
		if ( invasion )
			return static_cast<LONG>( INVASION_GetNumMonstersLeft( ));

		if ( dmflags2 & DF2_KILL_MONSTERS )
		{
			if ( level.total_monsters > 0 )
				return ( 100 * ( level.total_monsters - level.killed_monsters ) / level.total_monsters );
			else
				return 0;
		}

		return level.total_monsters - level.killed_monsters;
	}

	// [BB] In a team game with only empty teams or if there are no players at all, just return the appropriate limit.
	if ((( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSONTEAMS ) && ( TEAM_TeamsWithPlayersOn( ) == 0 )) || ( SERVER_CalcNumNonSpectatingPlayers( MAXPLAYERS ) == 0 ))
	{
		if ( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNWINS )
			return winlimit;
		else if ( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNPOINTS )
			return pointlimit;
		else if ( GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNFRAGS )
			return fraglimit;
		else
			return 0;
	}

	// FRAG-based mode.
	if ( fraglimit && GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNFRAGS )
		return scoreboard_GetScoreLeft( fraglimit, TEAM_GetHighestFragCount, &player_t::fragcount );
	// POINT-based mode.
	else if ( pointlimit && GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNPOINTS )
		return scoreboard_GetScoreLeft( pointlimit, TEAM_GetHighestPointCount, &player_t::lPointCount );
	// WIN-based mode.
	else if ( winlimit && GAMEMODE_GetCurrentFlags( ) & GMF_PLAYERSEARNWINS )
		return scoreboard_GetScoreLeft( winlimit, TEAM_GetHighestWinCount, &player_t::ulWins );

	// None of the above.
	return -1;
}

//*****************************************************************************
//
// [AK] SCOREBOARD_SetNextLevel
//
// Finds the level that will be printed on the intermission screen.
//
//*****************************************************************************

void SCOREBOARD_SetNextLevel( const char *pszMapName )
{
	g_pNextLevel = ( pszMapName != NULL ) ? FindLevelInfo( pszMapName ) : NULL;
}

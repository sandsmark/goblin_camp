/* Copyright 2011 Ilkka Halila
This file is part of Goblin Camp.

Goblin Camp is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goblin Camp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License 
along with Goblin Camp. If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <libtcod.hpp>
#include "tileRenderer/TileSetLoader.hpp"
#include "tileRenderer/TileSet.hpp"
#include "tileRenderer/TileSetTexture.hpp"
#include "tileRenderer/NPCSprite.hpp"
#include "tileRenderer/ItemSprite.hpp"
#include "tileRenderer/ConstructionSprite.hpp"
#include "tileRenderer/SpellSpriteSet.hpp"

class TileSetParserV1 : public ITCODParserListener
{
public:
	explicit TileSetParserV1();
	~TileSetParserV1();

	boost::shared_ptr<TileSet> Run(boost::filesystem::path tileSetPath);

	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name);
	bool parserFlag(TCODParser *parser,const char *name);
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value);
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name);
	void error(const char *msg);

private:
	TCODParser parser;

	enum SpriteSet
	{
		SS_NONE,
		SS_NPC,
		SS_ITEM,
		SS_NATURE,
		SS_CONSTRUCTION,
		SS_SPELL
	};

	boost::shared_ptr<TileSet> tileSet;
	bool success;

	boost::filesystem::path tileSetPath;

	std::string tileSetName;
	int tileWidth;
	int tileHeight;
	boost::shared_ptr<TileSetTexture> currentTexture;

	std::vector<int> fireSprites;
	int fireFPS;

	struct TempConstruction {
		std::vector<int> mainSprites;
		std::vector<int> underConstructionSprites;
		bool connectionMapped;
		int width;
		Sprite openDoor;

		TempConstruction() : mainSprites(), underConstructionSprites(), connectionMapped(false), width(1), openDoor() {}

		ConstructionSprite Build(boost::shared_ptr<TileSetTexture> currentTexture);
	};
	TempConstruction tempConstruction;

	struct TempSpell {
		std::vector<int> sprites;
		int fps;

		TempSpell() : sprites(), fps(15) {}

		SpellSpriteSet Build(boost::shared_ptr<TileSetTexture> currentTexture);
	};
	TempSpell tempSpell;

	TileSetParserV1::SpriteSet currentSpriteSet;
	NPCSprite npcSprite;
	NatureObjectSpriteSet natureObjectSpriteSet;
	ItemSprite itemSprite;
	
	static const char * uninitialisedTilesetError;
	
	void SetCursorSprites(CursorType type, TCOD_list_t cursors);
	Sprite ReadConnectionMap(TCOD_list_t indices);
};

class TileSetMetadataParserV1 : public ITCODParserListener {
public:
	TileSetMetadataParserV1();

	TileSetMetadata Run(boost::filesystem::path path);

	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name);
	bool parserFlag(TCODParser *parser,const char *name);
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value);
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name);
	void error(const char *msg);

private:
	TCODParser parser;
	TileSetMetadata metadata;

};
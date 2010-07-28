/* Copyright 2010 Ilkka Halila
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

#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/weak_ptr.hpp>
#include <libtcod.hpp>

#include "Game.hpp"

enum MenuResult {
	MENUHIT,
	NOMENUHIT
};

class MenuChoice {
public:
	MenuChoice(std::string = "", boost::function<void()> = boost::bind(Game::DoNothing));
	std::string label;
	boost::function<void()> callback;
};

class Drawable {
protected:
	int _x, _y, width, height;    
public:
    Drawable(int x, int y, int nwidth, int nheight):
        _x(x), _y(y), width(nwidth), height(nheight) {}
    virtual void Draw(int, int, TCODConsole *) = 0;
	virtual MenuResult Update(int x, int y, bool clicked, TCOD_key_t key) {return NOMENUHIT;}
};

class Label: public Drawable {
private:
    std::string text;
public:
    Label(std::string ntext, int x, int y) :
        Drawable(x, y, 0, 0), text(ntext) {}
    void Draw(int, int, TCODConsole *);
};

class Button: public Drawable {
private:
    std::string text;
    bool selected;
    char shortcut;
    boost::function<void()> callback;
public:
    Button(std::string ntext, boost::function<void()> ncallback, int x, int y, int nwidth, char nshortcut = 0):
        text(ntext), callback(ncallback), shortcut(nshortcut), Drawable(x, y, nwidth, 0), selected(false) {}
    void Draw(int, int, TCODConsole *);
    MenuResult Update(int, int, bool, TCOD_key_t);
};

class Panel: public Drawable {
public:
    Panel(int nwidth, int nheight):
        Drawable(0, 0, nwidth, nheight) {}
    void ShowModal();
};

class Menu: public Panel {
private:
    static std::map<std::string, Menu *> constructionCategoryMenus;
protected:
	std::vector<MenuChoice> choices;
	int _selected;
    std::string title;
	void CalculateSize();
public:
	Menu(std::vector<MenuChoice>, std::string="");
	virtual ~Menu();
	virtual void Draw(int, int, TCODConsole*);
	virtual MenuResult Update(int, int, bool, TCOD_key_t);
	virtual void Open();
	virtual void Close();
	void selected(int);
	void AddChoice(MenuChoice);
	void Callback(unsigned int);

	static Menu* mainMenu;
	static Menu* MainMenu();
	static Menu* constructionMenu;
	static Menu* ConstructionMenu();
	static Menu* basicsMenu;
	static Menu* BasicsMenu();
	static Menu* WorkshopsMenu();
	static Menu* ordersMenu;
	static Menu* OrdersMenu();
	static Menu* FurnitureMenu();
    static Menu* ConstructionCategoryMenu(std::string);

	static void YesNoDialog(std::string text, boost::function<void()> leftAction, boost::function<void()> rightAction,
                            std::string leftButton = "Yes", std::string rightButton = "No");
	static ItemCategory WeaponChoiceDialog();
};

class Dialog: public Panel {
private:
    std::vector<Drawable *> components;
    std::string title;
public:
    Dialog(std::vector<Drawable *> ncomponents, std::string ntitle, int nwidth, int nheight):
        components(ncomponents), title(ntitle), Panel(nwidth, nheight) {}
    ~Dialog();
    void AddComponent(Drawable *component);
    void Draw(int, int, TCODConsole *);
    MenuResult Update(int, int, bool, TCOD_key_t);
};

class JobMenu : public Menu {
private:
	int scroll;
public:
	JobMenu();
	void Draw(int, int, TCODConsole*);
	MenuResult Update(int, int, bool, TCOD_key_t);
	static JobMenu* jobListingMenu;
	static JobMenu* JobListingMenu();
	void ScrollDown();
	void ScrollUp();
};

class AnnounceMenu : public Menu {
private:
	int scroll;
public:
	AnnounceMenu();
	void Draw(int, int, TCODConsole*);
    MenuResult Update(int, int, bool, TCOD_key_t);
	static AnnounceMenu* announcementsMenu;
	static AnnounceMenu* AnnouncementsMenu();
	void ScrollDown();
	void ScrollUp();
};

class NPCMenu : public Menu {
private:
	int scroll;
public:
	NPCMenu();
	void Draw(int, int, TCODConsole*);
    MenuResult Update(int, int, bool, TCOD_key_t);
	static NPCMenu* npcListMenu;
	static NPCMenu* NPCListMenu();
	void ScrollDown();
	void ScrollUp();
};

class ConstructionMenu : public Menu {
private:
	Construction* construct;
	int scroll;
	std::vector<int> productPlacement;
	bool firstTimeDraw;
public:
	ConstructionMenu();
	void Draw(int, int, TCODConsole*);
    MenuResult Update(int, int, bool, TCOD_key_t);
	static ConstructionMenu* constructionInfoMenu;
	static ConstructionMenu* ConstructionInfoMenu(Construction*);
	void Construct(Construction*);
	void ScrollDown();
	void ScrollUp();
	void ClearProductPlacement();
};

class StockManagerMenu : public Menu {
private:
	int scroll;
	std::string filter;
public:
	StockManagerMenu();
	void Draw(int, int, TCODConsole*);
    MenuResult Update(int, int, bool, TCOD_key_t);
	static StockManagerMenu* stocksMenu;
	static StockManagerMenu* StocksMenu();
	void ScrollDown();
	void ScrollUp();
	void Open();
};

class SquadsMenu : public Menu {
private:
	std::string squadName;
	int squadMembers;
	int squadPriority;
	boost::weak_ptr<Squad> chosenSquad;
public:
	SquadsMenu();
	void Draw(int, int, TCODConsole*);
    MenuResult Update(int, int, bool, TCOD_key_t);
	static SquadsMenu* squadMenu;
	static SquadsMenu* SquadMenu();
	void Open();
};
#pragma once
#include <SFML/Graphics.hpp>
#include "../mainCharacter/MainCharacter.h"
#include "../map/GameMap.h"
#include "../npcs/NPC.h"
#include "../npcs/NPCManager.h"
#include "../dialogueSystem/DialogueManager.h"
#include "../menues/Menu.h"
#include "../saveSystem/SaveSystem.h"		
#include "../menues/StartTowerDefenseMenu.h"
#include "../npcs/MiraStanton.h"
#include "../npcs/ElliotMarlowe.h"
#include "../npcs/GarrickStone.h"
#include "../npcs/VincentHale.h"
#include "../npcs/SeraphinaLumeris.h"
#include "../../TimeSystem.h"

class GameManager;

class RPGEngine
{
public:
	RPGEngine(sf::RenderWindow& window, GameManager* gameManager);

	void processEvents();
	void update();
	void render();
	void renderDateTime(sf::RenderWindow& window, sf::Font& font, const std::string& date, const std::string& time);
	void resume(int crystals);

	void saveGame();
	void loadGame();
	void resetSaveGame();

private:
	sf::RenderWindow& mWindow;
	MainCharacter mCharacter;
	GameMap mMap;

	//NPCs
	VincentHale mVincentHale;
	GarrickStone mGarrickStone;
	MiraStanton mMiraStanton;
	ElliotMarlowe mElliotMarlowe;
	SeraphinaLumeris mSeraphinaLumeris;
	//ShopMenu mShopMenu;

	sf::Clock mClock;
	sf::View mView;

	bool mShowMenu;
	bool mShowDialogue;
	bool mShowStartMenu;
	bool mShowShopMenu;
	sf::Text mDialogueText;
	sf::Font mFont;

	GameManager* mGameManager;
	int mCrystals;
	int mCurrentLevel;

	SaveSystem mSaveSystem;
	NPCManager mNPCManager;
	//DialogueManager mDialogueManager;
	SkillTree mSkillTree;
	Inventory mInventory;
	Menu mMenu;
	StartTowerDefenseMenu mStartTowerDefenseMenu;
	std::vector<std::string> mAvailableTowers;

	NPC* mCurrentInteractingNPC;
	TimeSystem mTimeSystem;
};


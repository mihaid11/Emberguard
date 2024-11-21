#include "RPGEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<sf::String> wrapText(const sf::String& text, const sf::Font& font, unsigned int characterSize, float boxWidth) {
	std::vector<sf::String> lines;
	sf::String currentLine;
	std::istringstream wordStream(text.toAnsiString());  // Convert to ANSI string for stream processing
	std::string word;

	while (wordStream >> word) {
		// Measure the size of the current line with the new word appended
		sf::Text tempText(currentLine + (currentLine.isEmpty() ? "" : " ") + word, font, characterSize);
		float lineWidth = tempText.getLocalBounds().width;

		if (lineWidth > boxWidth) {
			// If the current line with the new word exceeds the box width, store the current line and start a new one
			lines.push_back(currentLine);
			currentLine = word;
		}
		else {
			// Otherwise, add the word to the current line
			if (!currentLine.isEmpty()) {
				currentLine += " ";
			}
			currentLine += word;
		}
	}

	// Add the last line to the result
	if (!currentLine.isEmpty()) {
		lines.push_back(currentLine);
	}

	return lines;
}

RPGEngine::RPGEngine(sf::RenderWindow& window, GameManager* gameManager)
	: mWindow(window),
	mMap(),
	mCharacter(sf::Vector2f(400.f, 300.f), mMap),
	mView(sf::Vector2f(400.f, 300.f), sf::Vector2f(1280.f, 720.f)),
	mShowDialogue(false),
	mGameManager(gameManager),
	mSaveSystem("savegame.txt"),
	mMiraStanton(sf::Vector2f(1000.0f, 900.0f)),
	mElliotMarlowe(sf::Vector2f(100.0f, 0.0f)),
	mGarrickStone(sf::Vector2f(-100.0f, 0.0f), gameManager),
	mVincentHale(sf::Vector2f(600.0f, 400.0f)),
	mSeraphinaLumeris(sf::Vector2f(-20.0f, 980.0f)),
	mNPCManager(),
	//mQuestMenu(sf::Vector2f(1080.0f, 360.0f)),
	mCurrentInteractingNPC(nullptr),
	mSkillTree(),
	mMenu(window, mSkillTree, mInventory),
	mShowMenu(false),
	mShowStartMenu(false),
	mShowShopMenu(false),
	mCrystals(100),
	mCurrentLevel(1),
	mAvailableTowers({ "LaserTower", "FlameTurret" }),
	mStartTowerDefenseMenu(window, mAvailableTowers, this, gameManager, mCurrentLevel, mCrystals)
	//,mShopMenu(window, mCrystals) 
	{

	if (!mFont.loadFromFile("gameFont.ttf"))
		std::cout << "Couldn't load font from file" << std::endl;

	mDialogueText.setFont(mFont);
	mDialogueText.setCharacterSize(10);
	mDialogueText.setFillColor(sf::Color::White);
	mDialogueText.setPosition(400.f, 500.f);

	mMap.addBuilding(sf::Vector2f(100.f, 100.f), sf::Vector2f(50.f, 50.f));
	mMap.addBuilding(sf::Vector2f(300.f, 300.f), sf::Vector2f(100.f, 100.f));
	mMap.addBuilding(sf::Vector2f(1500.0f, 1500.0f), sf::Vector2f(200.0f, 200.0f));

	// Add NPCs
	mNPCManager.addNPC(std::make_unique<GarrickStone>(sf::Vector2f(650.0f, 400.0f), gameManager));
	mNPCManager.addNPC(std::make_unique<MiraStanton>(sf::Vector2f(1000.0f, 900.0f)));
	mNPCManager.addNPC(std::make_unique<ElliotMarlowe>(sf::Vector2f(100.0f, 0.0f)));
	mNPCManager.addNPC(std::make_unique<VincentHale>(sf::Vector2f(-100.0f, 0.0f)));
	mNPCManager.addNPC(std::make_unique<SeraphinaLumeris>(sf::Vector2f(-20.0f, 980.0f)));

	//mAvailableTowers.push_back("LaserTower");
	//mAvailableTowers.push_back("FlameTurret");

	//mStartTowerDefenseMenu = StartTowerDefenseMenu(window, mAvailableTowers, this, gameManager, mCurrentLevel, mCrystals);
	loadGame();
}

void RPGEngine::run()
{
	while (mWindow.isOpen()) {
		processEvents();
		update();
		render();
	}
}

void RPGEngine::processEvents() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			saveGame();
			mWindow.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::E) {
				if (!mShowDialogue) {
					mNPCManager.handleInteraction(mCharacter.getPosition(), mShowDialogue, mDialogueText);
					if (mShowDialogue) {
						mCurrentInteractingNPC = mNPCManager.getCurrentNPC();
						mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);
					}
				}
				else if (mCurrentInteractingNPC && mShowDialogue) {
					mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);
				}
				else {
					mShowDialogue = false;
					if (mCurrentInteractingNPC) {
						mCurrentInteractingNPC->resumeMovement();
						mCurrentInteractingNPC->resetDialogue();
						mCurrentInteractingNPC = nullptr;
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Escape) {
				if (mShowDialogue) {
					mShowDialogue = false;
					if (mCurrentInteractingNPC) {
						mCurrentInteractingNPC->resumeMovement();
						mCurrentInteractingNPC->resetDialogue();
						mCurrentInteractingNPC = nullptr;
					}
				}
				else if (mShowStartMenu) {
					mShowStartMenu = false;
				}
				else {
					mShowMenu = !mShowMenu;
				}

				if (mShowShopMenu) {
					mShowShopMenu = false;
				}
			}
			/*else if (event.key.code == sf::Keyboard::O) {
				sf::String original = mDialogueText.getString();
				std::string text1 = original.toAnsiString();
				std::cout << text1 << std::endl;
				std::vector<std::string> lines = wrapText(text1, mFont, 10, 425.0f);
				for (const auto& line : lines) {
					std::cout << line << std::endl;
				}
			}*/
			else if (event.key.code == sf::Keyboard::L) {
				if (!mShowStartMenu) {
					saveGame();
					mShowStartMenu = true;
					if (mShowDialogue) {
						mShowDialogue = false;
						if (mCurrentInteractingNPC) {
							mCurrentInteractingNPC->resetDialogue();
							mCurrentInteractingNPC->resumeMovement();
							mCurrentInteractingNPC = nullptr;
						}
					}

					if (mShowMenu) {
						mShowMenu = false;
					}

					if (mShowShopMenu) {
						mShowShopMenu = false;
					}
				}
				else
					mShowStartMenu = false;
			}
			else if (event.key.code == sf::Keyboard::K) {
				if (!mShowShopMenu) {
					mShowShopMenu = true;
					if (mShowDialogue) {
						mShowDialogue = false;
						if (mCurrentInteractingNPC) {
							mCurrentInteractingNPC->resetDialogue();
							mCurrentInteractingNPC->resumeMovement();
							mCurrentInteractingNPC = nullptr;
						}
					}

					if (mShowMenu)
						mShowMenu = false;
				}
				else
					mShowShopMenu = false;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
				/*if (mQuestMenu.isMenuClicked(static_cast<sf::Vector2f>(mousePos))) {
					mQuestMenu.toggle();
				}*/
				if (mShowMenu) {
					mMenu.handleMouseClick(mousePos);
				}
				if (mShowStartMenu) {
					mStartTowerDefenseMenu.handleMouseClick(mousePos);
				}
			}
		}
	}
}

void RPGEngine::update()
{
	float dt = mClock.restart().asSeconds();

	if (!mShowMenu) {
		if (!mShowStartMenu) {
			sf::Vector2f previousPosition = mCharacter.getPosition();

			if (!mShowDialogue)
				mCharacter.update(dt);

			if (mMap.checkCollision(mCharacter.getBounds()))
				mCharacter.setPosition(previousPosition);

			for (auto& npc : mNPCManager.getNPCs()) {
				if (npc->getBounds().intersects(mCharacter.getBounds())) {
					sf::Vector2f npcPos = npc->getPosition();
					sf::Vector2f direction = previousPosition - npcPos;
					float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

					if (distance < 10.0f) {  // Adjust this threshold for smoothness
						// Push the player away from the NPC smoothly
						direction /= distance;
						previousPosition += direction * 5.0f * dt;
						mCharacter.setPosition(previousPosition);
					}
				}
			}

			mNPCManager.update(dt);
			mView.setCenter(mCharacter.getPosition());

			if (!mNPCManager.playerClose(mCharacter.getPosition())) {
				mShowDialogue = false;
			}

			/*if (mQuestManager.isCurrentStep("The First Trial", 2)) {
				if (mGameManager.switchToTowerDefense(1))
					mQuestManager.advanceQuest("The First Trial");
			}*/
		}
		else {
			sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
			mStartTowerDefenseMenu.updateHover(mousePos);
		}

	}
	else {
		sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
		mMenu.updateHover(mousePos);
		mMenu.update(mCrystals, mInventory, mSkillTree);
	}
}

void RPGEngine::render()
{
	mWindow.clear();

	mWindow.setView(mView);

	mMap.render(mWindow);
	mCharacter.render(mWindow);
	mNPCManager.render(mWindow);
	//mQuestMenu.render(mWindow);

	if (mShowDialogue) {
		mWindow.setView(mWindow.getDefaultView());

		// Create the dialogue box
		sf::RectangleShape dialogueBox(sf::Vector2f(mWindow.getSize().x / 2.8f,
			mWindow.getSize().y / 4.2f));
		dialogueBox.setFillColor(sf::Color(50, 50, 50, 205));
		dialogueBox.setPosition((mWindow.getSize().x - dialogueBox.getSize().x) / 2,
			mWindow.getSize().y - dialogueBox.getSize().y - 35.0f);

		// Create the NPC sprite
		sf::CircleShape npcSprite(17.5f);
		sf::Color npcColor = mCurrentInteractingNPC->getColor();
		npcSprite.setFillColor(npcColor);
		npcSprite.setPosition(dialogueBox.getPosition().x + npcSprite.getRadius(),
			dialogueBox.getPosition().y + npcSprite.getRadius() - 4.75f);

		mWindow.draw(dialogueBox);
		mWindow.draw(npcSprite);

		// Calculate the width available for text
		float textWidth = dialogueBox.getSize().x - npcSprite.getRadius() * 2 - 40.0f;
		std::vector<sf::String> lines = wrapText(mDialogueText.getString(), mFont, mDialogueText.getCharacterSize(), textWidth);

		// Draw each line
		float lineHeight = mDialogueText.getCharacterSize() * 1.35f; // Adjust line height if necessary
		float yOffset = dialogueBox.getPosition().y + 10.0f;
		mDialogueText.setPosition(dialogueBox.getPosition().x + npcSprite.getRadius() * 2 + 35.0f, yOffset);

		for (const auto& line : lines) {
			mDialogueText.setString(line);
			mWindow.draw(mDialogueText);
			yOffset += lineHeight;
			mDialogueText.setPosition(dialogueBox.getPosition().x + npcSprite.getRadius() * 2 + 35.0f, yOffset);
		}
	}

	if (mShowMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mMenu.render(mWindow);
	}

	if (mShowStartMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mStartTowerDefenseMenu.render(mWindow);
	}

	/*if (mShowShopMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mShopMenu.render();
	}*/

	mWindow.display();
}

void RPGEngine::resume(int crystals)
{
	mWindow.setView(mWindow.getDefaultView());
	loadGame();
	mCrystals = crystals;
	if (mShowDialogue) {
		mShowDialogue = false;
		mNPCManager.resumeCurrentNPC();
	}

	if (mShowMenu) {
		mShowMenu = false;
	}

	if (mShowStartMenu) {
		mShowStartMenu = false;
	}
}

void RPGEngine::saveGame() {
	std::vector<sf::Vector2f> npcPositions;
	std::vector<int> npcWaypoints;

	mNPCManager.saveNPCStates(npcPositions, npcWaypoints);

	mSaveSystem.save(mCharacter.getPosition(), npcPositions, npcWaypoints, mCrystals);
}

void RPGEngine::loadGame() {
	sf::Vector2f playerPosition;
	std::vector<sf::Vector2f> npcPositions;
	std::vector<int> npcWaypoints;
	int crystals;

	if (mSaveSystem.load(playerPosition, npcPositions, npcWaypoints, crystals)) {
		std::cout << "Loaded player position: " << playerPosition.x << ", " << playerPosition.y << std::endl;
		mCharacter.setPosition(playerPosition);

		mNPCManager.loadNPCStates(npcPositions, npcWaypoints);
		mCrystals = crystals;
	}
	else {
		std::cerr << "Failed to load game data." << std::endl;
	}
}

void RPGEngine::resetSaveGame() {
	// Open the save file in truncation mode, which clears its contents
	std::ofstream saveFile("savegame.txt", std::ofstream::trunc);

	// Optionally, check if the file was opened successfully
	if (saveFile.is_open()) {
		std::cout << "Save file cleared. Game will start from initial positions." << std::endl;
	}
	else {
		std::cerr << "Failed to clear save file." << std::endl;
	}

	// Close the file
	saveFile.close();

	// Reset the game state to the initial positions
	mCharacter.setPosition(sf::Vector2f(400.f, 300.f));
	mNPCManager.loadNPCStates({}, {}); // Pass empty vectors to reset NPC positions and waypoints
	mCrystals = 100;
}

/*
void RPGEngine::initializeQuests() {

	// Add quest segments (steps) to the quest
	QuestSegment step1(
		"Talk to Elliot to start the quest.",
		[this]() -> bool {
			return mNPCManager.getCurrentNPC() == static_cast<ElliotMarlowe> &&
				mNPCManager.getCurrentNPC().getActiveDialogueKey() == "quest1";
		}
	);

	QuestSegment step2(
		"Talk to Vincent to receive the task.",
		[this]() -> bool {
			return mNPCManager.getCurrentNPC() == "Vincent" &&
				mNPCManager.getCurrentNPC().getActiveDialogueKey() == "task1";
		}
	);

	QuestSegment step3(
		"Complete the first level of the tower defense game.",
		[this]() -> bool {
			return isTowerDefenseLevelCompleted(1);
		}
	);

	// Create QuestInfo object with basic quest information
	QuestInfo questInfo("The First Trial", "First quest to test the player", {step1, step2, step3}, 200);  // reward is 100, modify as needed

	// Create a Quest object using the QuestInfo
	Quest theFirstTrial(questInfo);

	// Add the quest to the QuestManager and QuestMenu
	mQuestManager.addQuest(theFirstTrial);
	mQuestMenu.addQuest(theFirstTrial);
}*/

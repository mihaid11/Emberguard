#include "RPGEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "../../GameManager.h"

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
	mTimeSystem(1.2f),
	mCharacter(sf::Vector2f(400.f, 300.f), mMap),
	mView(sf::Vector2f(400.f, 300.f), sf::Vector2f(1280.f, 720.f)),
	mShowDialogue(false),
	mGameManager(gameManager),
	mSaveSystem("../../savegame.txt"),
	mMiraStanton(sf::Vector2f(1000.0f, 900.0f)),
	mElliotMarlowe(sf::Vector2f(100.0f, 0.0f)),
	mGarrickStone(sf::Vector2f(-100.0f, 0.0f), gameManager),
	mVincentHale(sf::Vector2f(600.0f, 400.0f)),
	mSeraphinaLumeris(sf::Vector2f(-20.0f, 980.0f)),
	mNPCManager(),
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

	if (!mFont.loadFromFile("../assests/fonts/gameFont.ttf"))
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

					if (distance < 10.0f) {  // Push the player away from the npc with a threshold
						direction /= distance;
						previousPosition += direction * 5.0f * dt;
						mCharacter.setPosition(previousPosition);
					}
				}
			}

			mNPCManager.update(dt);
			mView.setCenter(mCharacter.getPosition());

			mTimeSystem.update(dt);

			if (!mNPCManager.playerClose(mCharacter.getPosition())) {
				mShowDialogue = false;
			}
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
	mStartTowerDefenseMenu.update(mCrystals);
}

void RPGEngine::render()
{
	std::string currentDate = mTimeSystem.getDateString();
	std::string currentTime = mTimeSystem.getTimeString();

	mWindow.clear();

	sf::View originalView = mWindow.getView();

	mWindow.setView(mView);

	mMap.render(mWindow);
	mCharacter.render(mWindow);
	mNPCManager.render(mWindow);

	mWindow.setView(mWindow.getDefaultView());

	renderDateTime(mWindow, mFont, currentDate, currentTime);

	if (mShowDialogue) {
		mWindow.setView(mWindow.getDefaultView());

		sf::RectangleShape dialogueBox(sf::Vector2f(mWindow.getSize().x / 2.8f,
			mWindow.getSize().y / 4.2f));
		dialogueBox.setFillColor(sf::Color(50, 50, 50, 205));
		dialogueBox.setPosition((mWindow.getSize().x - dialogueBox.getSize().x) / 2,
			mWindow.getSize().y - dialogueBox.getSize().y - 35.0f);

		sf::CircleShape npcSprite(17.5f);
		sf::Color npcColor = mCurrentInteractingNPC->getColor();
		npcSprite.setFillColor(npcColor);
		npcSprite.setPosition(dialogueBox.getPosition().x + npcSprite.getRadius(),
			dialogueBox.getPosition().y + npcSprite.getRadius() - 4.75f);

		mWindow.draw(dialogueBox);
		mWindow.draw(npcSprite);

		float textWidth = dialogueBox.getSize().x - npcSprite.getRadius() * 2 - 40.0f;
		std::vector<sf::String> lines = wrapText(mDialogueText.getString(), mFont, mDialogueText.getCharacterSize(), textWidth);

		float lineHeight = mDialogueText.getCharacterSize() * 1.35f; 
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

void RPGEngine::renderDateTime(sf::RenderWindow& window, sf::Font& font, const std::string& date, const std::string& time)
{
	sf::RectangleShape background(sf::Vector2f(200, 80));
	background.setFillColor(sf::Color(0, 0, 0, 150));
	background.setPosition(window.getSize().x - 210, 10); 

	sf::Text dateText;
	dateText.setFont(font);
	dateText.setString(date);
	dateText.setCharacterSize(16);
	dateText.setFillColor(sf::Color::White);
	dateText.setPosition(window.getSize().x - 200, 20);

	sf::Text timeText;
	timeText.setFont(font);
	timeText.setString(time);
	timeText.setCharacterSize(16);
	timeText.setFillColor(sf::Color::White);
	timeText.setPosition(window.getSize().x - 200, 50);

	window.draw(background);
	window.draw(dateText);
	window.draw(timeText);
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

	int year = mTimeSystem.getYear();
	int day = mTimeSystem.getDay();
	int hour = mTimeSystem.getHour();
	int minute = mTimeSystem.getMinute();
	
	mNPCManager.saveNPCStates(npcPositions, npcWaypoints);

	mSaveSystem.save(mCharacter.getPosition(), npcPositions, npcWaypoints, mCrystals,
		year, day, hour, minute);
}

void RPGEngine::loadGame() {
	sf::Vector2f playerPosition;
	std::vector<sf::Vector2f> npcPositions;
	std::vector<int> npcWaypoints;
	int crystals, year, day, hour, minute;

	if (mSaveSystem.load(playerPosition, npcPositions, npcWaypoints, crystals,
		year, day, hour, minute)) {
		mCharacter.setPosition(playerPosition);

		mNPCManager.loadNPCStates(npcPositions, npcWaypoints);
		mCrystals = crystals;

		mTimeSystem.setYear(year);
		mTimeSystem.setDay(day);
		mTimeSystem.setHour(hour);
		mTimeSystem.setMinute(minute);
		mTimeSystem.resetTimeAccumulator();
	}
	else {
		std::cerr << "Failed to load game data." << std::endl;
	}
}

void RPGEngine::resetSaveGame() {
	std::ofstream saveFile("savegame.txt", std::ofstream::trunc);

	if (saveFile.is_open()) {
		std::cout << "Save file cleared. Game will start from initial positions." << std::endl;
	}
	else {
		std::cerr << "Failed to clear save file." << std::endl;
	}

	saveFile.close();

	mCharacter.setPosition(sf::Vector2f(400.f, 300.f));
	mNPCManager.loadNPCStates({}, {});
	mCrystals = 100;
}

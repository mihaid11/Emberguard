#include "NPCManager.h"
#include <iostream>

NPCManager::NPCManager() : mCurrentNPC(nullptr), mShowDialogue(false) {

}

void NPCManager::addNPC(std::unique_ptr<NPC> npc) {
	mNPCs.push_back(std::move(npc));
}

void NPCManager::update(float dt) {
	for (auto& npc : mNPCs) {
		if (!mShowDialogue || npc.get() != mCurrentNPC) {
			npc->update(dt);
		}
	}
}

void NPCManager::render(sf::RenderWindow& window) {
	for (auto& npc : mNPCs) {
		npc->render(window);
		//npc->drawIndicator(window);
	}
}

void NPCManager::handleInteraction(const sf::Vector2f& playerPosition, bool& showDialogue, sf::Text& dialogueText) {
	for (auto& npc : mNPCs) {
		if (npc->isPlayerClose(playerPosition)) {
			mCurrentNPC = npc.get();
			showDialogue = true;
			mShowDialogue = true;
			dialogueText.setString(mCurrentNPC->getCurrentDialogue());

			//std::cout << "Interacting with NPC: " << typeid(*mCurrentNPC).name() << std::endl;
			//std::cout << "Current Dialogue: " << mCurrentNPC->getCurrentDialogue() << std::endl;

			break;
		}
	}
}

void NPCManager::interactWithCurrentNPC(bool& showDialogue, sf::Text& dialogueText) {
	if (mCurrentNPC) {
		if (mCurrentNPC->hasMoreDialogue()) {
			stopCurrentNPC();
			dialogueText.setString(mCurrentNPC->getCurrentDialogue());
			std::cout << "Current Dialogue: " << mCurrentNPC->getCurrentDialogue() << std::endl;
			mCurrentNPC->advanceDialogue();
		}
		else {
			mShowDialogue = false;
			showDialogue = false;
			mCurrentNPC->resetDialogue();
			mCurrentNPC->resumeMovement();
			mCurrentNPC = nullptr;
		}
	}
}

void NPCManager::stopCurrentNPC() {
	if (mCurrentNPC) {
		mCurrentNPC->pauseMovement();
	}
}

void NPCManager::resumeCurrentNPC() {
	if (mCurrentNPC) {
		mShowDialogue = false;
		mCurrentNPC->resumeMovement();
		mCurrentNPC->resetDialogue();
		mCurrentNPC = nullptr;
	}
}

void NPCManager::saveNPCStates(std::vector<sf::Vector2f>& npcPositions, std::vector<int>& npcWaypoints) {
	for (auto& npc : mNPCs) {
		npcPositions.push_back(npc->getPosition());
		npcWaypoints.push_back(npc->getCurrentWaypoint());
	}
}

void NPCManager::loadNPCStates(const std::vector<sf::Vector2f>& npcPositions, const std::vector<int>& npcWaypoints) {
	for (size_t i = 0; i < mNPCs.size(); ++i) {
		if (i < npcPositions.size()) {
			mNPCs[i]->setPosition(npcPositions[i]);
			mNPCs[i]->setCurrentWaypoint(npcWaypoints[i]);
		}
	}
}

bool NPCManager::playerClose(const sf::Vector2f& playerPosition)
{
	for (auto& npc : mNPCs) {
		if (npc->isPlayerClose(playerPosition))
			return true;
	}
	return false;
}

const std::vector<std::unique_ptr<NPC>>& NPCManager::getNPCs() const
{
	return mNPCs;
}

void NPCManager::stopAllNPC()
{
	for (auto& npc : mNPCs) {
		npc->pauseMovement();
	}
}

void NPCManager::resumeAllNPC()
{
	for (auto& npc : mNPCs) {
		npc->resumeMovement();
	}
}

NPC* NPCManager::getCurrentNPC()
{
	return mCurrentNPC;
}

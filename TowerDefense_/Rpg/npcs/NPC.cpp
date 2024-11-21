#include "NPC.h"
#include <cstdlib>
#include <iostream>

NPC::NPC(const sf::Vector2f& position)
    : mInteractionRadius(100.0f), mInteractionIndicator(10), mPaused(false), mSpeed(35.0f),
    mCurrentWaypoint(0), mIsPaused(false), mPauseDuration(3.0f), mPauseTimer(0.0f) {

    mShape.setPosition(position);
    mShape.setRadius(10);
    mShape.setFillColor(sf::Color::Green);
    mShape.setOrigin(10, 10);

    mInteractionIndicator.setFillColor(sf::Color::Yellow);
    mInteractionIndicator.setPosition(position.x, position.y - 50);
    mInteractionIndicator.setPointCount(100);
}

void NPC::setDialogue(const std::string& key, const Dialogue& dialogue)
{
    mDialogues[key] = dialogue;
}

void NPC::setActiveDialogue(const std::string& key) {
    auto it = mDialogues.find(key);
    if (it != mDialogues.end()) {
        mActiveDialogueKey = key;
        mDialogueManager.startDialogue(it->second);
    }
    else {
        std::cerr << "Dialogue key not found: " << key << std::endl;
    }
}

Dialogue NPC::getActiveDialogue()
{
    return mDialogues[mActiveDialogueKey];
}

std::string NPC::getCurrentDialogue()
{
    return mDialogueManager.getCurrentDialogueText();
}

bool NPC::hasMoreDialogue() const
{
    return mDialogueManager.isDialogueInProgress();
}

void NPC::resetDialogue()
{
    mDialogueManager.resetDialogue();
}

void NPC::render(sf::RenderWindow& window)
{
    window.draw(mShape);
}

void NPC::drawIndicator(sf::RenderWindow& window)
{
    window.draw(mInteractionIndicator);
}

bool NPC::isPlayerClose(const sf::Vector2f& playerPosition) const {
    float distance = std::sqrt(std::pow(playerPosition.x - mShape.getPosition().x, 2) +
        std::pow(playerPosition.y - mShape.getPosition().y, 2));
    return distance <= mInteractionRadius;
}

void NPC::advanceDialogue()
{
    if (mDialogueManager.isDialogueInProgress()) {
        mDialogueManager.getCurrentDialogue().indexIncrement();
        std::cout << "Dialogue index incremented: " << mDialogueManager.getCurrentDialogue().getCurrentIndex() << std::endl;
    }
}

void NPC::update(float dt) {
    updatePause(dt);

    if (mPaused || mIsPaused) return;

    if (mPath.empty()) return;

    sf::Vector2f target = mPath[mCurrentWaypoint];
    sf::Vector2f direction = target - mShape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        // Move to the next waypoint
        mCurrentWaypoint = (mCurrentWaypoint + 1) % mPath.size();

        if (rand() % 10 < 4)
            startPause(3.0f);
    }
    else {
        // Normalize direction and move
        direction /= distance;
        mShape.move(direction * mSpeed * dt);
    }
}

void NPC::resumeMovement()
{
    mPaused = false;
}

void NPC::pauseMovement()
{
    mPaused = true;
}

sf::Vector2f NPC::getPosition() const
{
    return mShape.getPosition();
}

sf::FloatRect NPC::getBounds() const
{
    return mShape.getGlobalBounds();
}

std::string NPC::getActiveDialogueKey() const
{
    return mActiveDialogueKey;
}

void NPC::setPosition(const sf::Vector2f& position)
{
    mShape.setPosition(position);
}

int NPC::getCurrentWaypoint() const
{
    return mCurrentWaypoint;
}

void NPC::setCurrentWaypoint(int waypoint)
{
    mCurrentWaypoint = waypoint;
}

sf::Color NPC::getColor() const
{
    return  mShape.getFillColor();
}

void NPC::startPause(float duration) {
    mIsPaused = true;
    mPauseDuration = duration;
    mPauseStartTime = std::chrono::steady_clock::now();
}

void NPC::updatePause(float dt) {
    if (mIsPaused) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - mPauseStartTime;
        if (elapsed.count() >= mPauseDuration) {
            mIsPaused = false;
        }
    }
}


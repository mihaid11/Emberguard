#include "BorrowMenu.h"
#include <iostream>

BorrowMenu::BorrowMenu(sf::RenderWindow& window, const sf::Vector2f position,
	const sf::Vector2f size, int& crystals, bool& hasBorrowActive, TimeSystem& timeSystem)
	:m500Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "500"),
	m1000Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "1000"),
	m2000Button(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "2000"),
	mConfirmButton(sf::Vector2f(0, 0), sf::Vector2f(200.0f, 40.0f), "Confirm Loan"),
	mBackButton(sf::Vector2f(0, 0), sf::Vector2f(100.0f, 40.0f), "Back"),
	mConfirmShowing(false), mAmountToBorrow(0), mCrystals(crystals), mAmountToRepay(0),
	mInterest(0), mDaysToRepayment(0), mPenalty(0), mHasBorrowActive(hasBorrowActive),
	mStartDay(0), mRepaymentDay(0)
{
	if (!mFont.loadFromFile("../assests/fonts/gameFont.ttf")) {
		std::cerr << "Failed to load font for BankMenu!" << std::endl;
	}

	mMenuShape.setSize(size);
	mMenuShape.setPosition(position);
	mMenuShape.setFillColor(sf::Color(70, 70, 70, 200));

	mLoanAmount.setFont(mFont);
	mLoanAmount.setCharacterSize(16);
	mLoanAmount.setFillColor(sf::Color::White);
	mLoanAmount.setString("Loan Amount: ");
	mLoanAmount.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60, mMenuShape.getPosition().y + 25));

	mRepayment.setFont(mFont);
	mRepayment.setCharacterSize(16);
	mRepayment.setFillColor(sf::Color::White);
	mRepayment.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60, mMenuShape.getPosition().y + 75));

	mInterestText.setFont(mFont);
	mInterestText.setCharacterSize(16);
	mInterestText.setFillColor(sf::Color::White);
	mInterestText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60, mMenuShape.getPosition().y + 125));

	mTotalRepay.setFont(mFont);
	mTotalRepay.setCharacterSize(16);
	mTotalRepay.setFillColor(sf::Color::White);
	mTotalRepay.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60, mMenuShape.getPosition().y + 175));

	mPenaltyText.setFont(mFont);
	mPenaltyText.setCharacterSize(16);
	mPenaltyText.setFillColor(sf::Color::White);
	mPenaltyText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60, mMenuShape.getPosition().y + 250));

	mBorrowActiveText.setFont(mFont);
	mBorrowActiveText.setCharacterSize(16);
	mBorrowActiveText.setFillColor(sf::Color::White);
	mBorrowActiveText.setString("You can't have more than one borrow active!");
	mBorrowActiveText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 60, mMenuShape.getPosition().y + 25));

	m500Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300,
		mMenuShape.getPosition().y + 25 - 8.75));
	m1000Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300 + 110,
		mMenuShape.getPosition().y + 25 - 8.75));
	m2000Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 300 + 220,
		mMenuShape.getPosition().y + 25 - 8.75));
	mConfirmButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 100, mMenuShape.getPosition().y + 310));
	mBackButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 400, mMenuShape.getPosition().y + 310));

	m500Button.setCallback([&]() {
		if (hasBorrowActive == false) {
			mAmountToBorrow = 500;
			mConfirmShowing = true;
			mInterest = 30;
			mDaysToRepayment = 3;
			mPenalty = 20;
			mAmountToRepay = mAmountToBorrow * (100 + mInterest) / 100;
			mRepaymentDay = mDaysToRepayment + timeSystem.getDay();
		}
		});

	m1000Button.setCallback([&]() {
		if (hasBorrowActive == false) {
			mAmountToBorrow = 1000;
			mConfirmShowing = true;
			mInterest = 25;
			mDaysToRepayment = 5;
			mPenalty = 15;
			mAmountToRepay = mAmountToBorrow * (100 + mInterest) / 100;
			mRepaymentDay = mDaysToRepayment + timeSystem.getDay();
		}
		});

	m2000Button.setCallback([&]() {
		if (hasBorrowActive == false) {
			mAmountToBorrow = 2000;
			mConfirmShowing = true;
			mInterest = 20;
			mDaysToRepayment = 7;
			mPenalty = 10;
			mAmountToRepay = mAmountToBorrow * (100 + mInterest) / 100;
			mRepaymentDay = mDaysToRepayment + timeSystem.getDay();
		}
		});

	mConfirmButton.setCallback([&]() {
		crystals += mAmountToBorrow;
		mAmountToBorrow = 0;
		mConfirmShowing = false;
		hasBorrowActive = true;
		});

	mBackButton.setCallback([&]() {
		mAmountToBorrow = 0;
		mInterest = 0;
		mDaysToRepayment = 0;
		mPenalty = 0;
		mAmountToRepay = 0;
		mRepaymentDay = 0;
		mConfirmShowing = false;
		});
}

void BorrowMenu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);
	window.draw(mRepayment);
	window.draw(mInterestText);
	window.draw(mTotalRepay);
	window.draw(mPenaltyText);

	if (mHasBorrowActive == false) {
		window.draw(mLoanAmount);
		m500Button.render(window);
		m1000Button.render(window);
		m2000Button.render(window);
	}
	else {
		window.draw(mBorrowActiveText);
	}

	if (mConfirmShowing)
	{
		mConfirmButton.render(window);
		mBackButton.render(window);
	}
}

void BorrowMenu::handleClicks(const sf::Vector2f& mousePos)
{
	if (m500Button.isMouseOver(mousePos))
		m500Button.onClick();
	if (m1000Button.isMouseOver(mousePos))
		m1000Button.onClick();
	if (m2000Button.isMouseOver(mousePos))
		m2000Button.onClick();

	if (mConfirmShowing)
	{
		if (mConfirmButton.isMouseOver(mousePos))
			mConfirmButton.onClick();
		if (mBackButton.isMouseOver(mousePos))
			mBackButton.onClick();
	}
}

void BorrowMenu::updateHover(const sf::Vector2f& mousePos)
{
	m500Button.updateHover(mousePos);
	m1000Button.updateHover(mousePos);
	m2000Button.updateHover(mousePos);

	if (mConfirmShowing)
	{
		mConfirmButton.updateHover(mousePos);
		mBackButton.updateHover(mousePos);
	}
}

void BorrowMenu::update()
{
	if (mConfirmShowing) {
		mRepayment.setString("Repayment by: Day " + std::to_string(mRepaymentDay));
		mInterestText.setString("Interest Rate: " + std::to_string(mInterest) + "%");
		mTotalRepay.setString("Total to Repay: " + std::to_string(mAmountToRepay));
		mPenaltyText.setString("Penalty: +" + std::to_string(mPenalty) + "% per late day");
	}
	else {
		mRepayment.setString("");
		mInterestText.setString("");
		mTotalRepay.setString("");
		mPenaltyText.setString("");
	}
}

int BorrowMenu::getPenalty()
{
	return mPenalty;
}

int BorrowMenu::getDaysToRepayment()
{
	return mDaysToRepayment;
}

int BorrowMenu::getInterest()
{
	return mInterest;
}

int BorrowMenu::getAmounToRepay()
{
	return mAmountToRepay;
}

int BorrowMenu::getRepaymentDay()
{
	return mRepaymentDay;
}

bool BorrowMenu::getActiveBorrow()
{
	return mHasBorrowActive;
}

void BorrowMenu::restart()
{
	mConfirmShowing = false;
}

void BorrowMenu::reset()
{
	mAmountToBorrow = 0;
	mAmountToRepay = 0;
	mInterest = 0;
	mDaysToRepayment = 0;
	mRepaymentDay = 0;
	mPenalty = 0;
	mStartDay = 0;
	mHasBorrowActive = false;
}

void BorrowMenu::setStats(bool hasBorrowActive, int penalty, int interest, int amountToRepay, int repayDay)
{
	mHasBorrowActive = hasBorrowActive;
	mPenalty = penalty;
	mInterest = interest;
	mAmountToRepay = amountToRepay;
	mRepaymentDay = repayDay;
}

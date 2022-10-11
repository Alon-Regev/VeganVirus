#include "DesktopAction.h"
#include <cmath>

const std::vector<const wchar_t*> DesktopAction::_messages = {
	L"Eww... Go away,\nmeat eater!",
	L"Are you also trying\nto eat ME?!",
	L"Hate the sin,\nhate the sinner.",
	L"It is the anonymity of\nanimal victims that makes us\ndeaf to their cries.",
	L"Animals are my friends\nand I don't eat my friends.",
	L"Only beautiful animals\nand ugly people wear fur.",
	L"Are you trying to make\na coat out of me??",
};

DesktopAction::DesktopAction(double req, Draw* draw, MouseManager& mouseManager, DesktopManager& desktopManager)
	: Action(req, DESKTOP_ACTION_ICON), _mouseManager(mouseManager), _desktopManager(desktopManager), 
	_speechBubbleBmp(Draw::resizedBitmap(L"speechBubble.png", 250, 200))
{
	_draw = draw;
	_actionTime = 0;
}

DesktopAction::~DesktopAction()
{
	delete this->_speechBubbleBmp;
}

void DesktopAction::start()
{
	_actionTime = DESKTOP_ACTION_TIME;
	this->_iconPositions = std::vector<POINT>();
	int iconCount = _desktopManager.iconCount();
	for (int i = 0; i < iconCount; i++)
	{
		this->_iconPositions.push_back(_desktopManager.getIconPosition(i));
	}
}

void DesktopAction::update(double dt)
{
	if (_actionTime <= 0)
		return;
	_actionTime -= dt;
	Point_t mouseTemp = _mouseManager.getMousePosition();
	POINT mousePos = { mouseTemp.x, mouseTemp.y };
	std::vector<POINT> iconVelocityArr = computeIconInteractionVelocities();
	// going over all icons, calculating their velocity and updating their position
	for (int i = 0; i < _iconPositions.size(); i++)
	{
		POINT position = _iconPositions[i];
		POINT cursorInteraction = velocityBetweeenPoints(position, mousePos, CURSOR_INTERACTION_VELCITY_COEFFICIENT, 1);
		POINT velocity = limitVelocity({ cursorInteraction.x + iconVelocityArr[i].x,
			cursorInteraction.y + iconVelocityArr[i].y });
		POINT newPosition = correctIconOutOfScreen(
			{ (long)(position.x + velocity.x * dt),
			(long)(position.y + velocity.y * dt) }
		);
		_iconPositions[i] = newPosition;
		_desktopManager.setIconPosition(i, newPosition);
	}
	
	if (rand() % SPEECH_BUBBLE_CHANCE_INV == 0)
	{
		SpeechBubble sb = { 0 };
		sb.iconIndex = rand() % _iconPositions.size();
		sb.message = _messages[rand() % _messages.size()];
		sb.timer = SPEECH_BUBBLE_TIMER;
		_speechBubbles.push_back(sb);
	}

	// draw speech bubbles
	for (SpeechBubble& sb : _speechBubbles)
	{
		int x = _iconPositions[sb.iconIndex].x + 10;
		int y = _iconPositions[sb.iconIndex].y - 140;
		_draw->drawImage(_speechBubbleBmp, x, y);
		_draw->drawText(x + 16, y + 16, sb.message);
		sb.timer -= dt;
	}

	_speechBubbles.remove_if([](SpeechBubble& sb)
	{
		return sb.timer <= 0;
	});
}

std::vector<POINT> DesktopAction::computeIconInteractionVelocities()
{
	std::vector<POINT> iconVelocities(_iconPositions.size(), { 0 });
	// looping over all pairs of icons and calculating their interaction. 
	// then summing up all interaction velocities on every icon.
	for (int i = 0; i < _iconPositions.size(); i++)
	{
		for (int j = i + 1; j < _iconPositions.size(); j++)
		{
			POINT velocity = velocityBetweeenPoints(_iconPositions[i], _iconPositions[j], ICON_INTERACTION_VELOCITY_COEFFICIENT, 2);
			iconVelocities[i].x += velocity.x;
			iconVelocities[i].y += velocity.y;
			iconVelocities[j].x -= velocity.x;
			iconVelocities[j].y -= velocity.y;
		}
	}
	return iconVelocities;
}

POINT DesktopAction::limitVelocity(POINT velocity)
{
	double vSqr = velocity.x * velocity.x + velocity.y * velocity.y;
	if (vSqr > MAX_VELOCITY * MAX_VELOCITY)
	{
		double mul = MAX_VELOCITY / sqrt(vSqr);
		return { (long)(velocity.x * mul), (long)(velocity.y * mul) };
	}
	return velocity;
}

POINT DesktopAction::velocityBetweeenPoints(POINT a, POINT b, double coefficient, int power)
{
	POINT d = { a.x - b.x, a.y - b.y };
	double Dpow = pow(pow(d.x, 2) + pow(d.y, 2), (power + 1.) / 2);
	long velocityX = coefficient * d.x / Dpow;
	long velocityY = coefficient * d.y / Dpow;
	return { velocityX, velocityY };
}

POINT DesktopAction::correctIconOutOfScreen(POINT newPos)
{
	// check if icon is out of boundaries and return corrected position
	POINT screenBoundries = _draw->getScreenSize();
	if (newPos.x < SCREEN_MARGIN)
		newPos.x = SCREEN_MARGIN;
	if (newPos.x > screenBoundries.x - SCREEN_MARGIN)
		newPos.x = screenBoundries.x - SCREEN_MARGIN;
	if (newPos.y < SCREEN_MARGIN)
		newPos.y = SCREEN_MARGIN;
	if (newPos.y > screenBoundries.y - SCREEN_MARGIN)
		newPos.y = screenBoundries.y - SCREEN_MARGIN;
	return { newPos.x, newPos.y };
}

#pragma once
class Action
{
public:
	// Action Constructor
	// input: required progress to activate, path to action icon file
	Action(double progressRequirement, const char* iconPath);

	// method called to start the action
	// input: none
	// return: none
	virtual void start() = 0;

	// method called to update the action every frame
	// input: time since last frame
	// return: none
	virtual void update(double dt);

	// method checks whether the action can be activated or not
	// input: none
	// return: true if can be activated
	virtual bool canActivate();

	double getReq() const;
	const char* getIconPath() const;

	bool activeFlag = false;

private:
	double _progressRequirement;
	const char* _iconPath;
};


#pragma once

#include <vector>
#include <string>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

class Menu
{
public:
	Menu();
	~Menu();

	void push_item(wstring string, Font font);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	vector<Text> _arr;
};
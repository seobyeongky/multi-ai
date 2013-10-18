#pragma once

#include <vector>
#include <string>
#include <functional>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

class IItem : public Drawable, public Transformable
{
public:
	virtual void	selected() = 0;
	virtual void	unselected() = 0;
	virtual void	activated() = 0;
};

class Menu : public Drawable, public Transformable
{
public:
	struct delta_t
	{
		Vector2f	delta_pos;
		float		delta_rotation;
		float		delta_scale;
		delta_t(void);
		delta_t(const Vector2f & dpos, float drotation, float dscale);
	};

public:
			Menu(const delta_t & delta, Keyboard::Key done_key);
			~Menu();

	void	pushItem(IItem * item);
	void	clear();

	bool	handleKeyPressed(Event::KeyEvent key);

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	delta_t			_delta;
	vector<IItem *>	_arr;
	size_t			_index;
	View			_view;
	Keyboard::Key	_done_key;
};
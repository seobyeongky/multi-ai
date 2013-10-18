#include "menu.h"

#include <cmath>

Menu::delta_t operator * (const size_t & n, const Menu::delta_t & d)
{
	return Menu::delta_t(	static_cast<float>(n) * d.delta_pos,
							n * d.delta_rotation,
							powf(d.delta_scale, static_cast<float>(n)));
}

Menu::delta_t::delta_t(void)
	: delta_pos(0.f, 0.f), delta_rotation(0.f),
	delta_scale(1.f)
{
}
Menu::delta_t::delta_t(const Vector2f & dpos, float drotation, float dscale)
	: delta_pos(dpos), delta_rotation(drotation),
	delta_scale(dscale)
{
}

Menu::Menu(const delta_t & delta, Keyboard::Key done_key)
	: _delta(delta), _arr(), _index(0U), _view(),
	_done_key(done_key)
{
}

Menu::~Menu()
{
	clear();
}

void Menu::pushItem(IItem * item)
{
	delta_t delta = _arr.size() * _delta;
	item->setPosition(delta.delta_pos);
	item->setRotation(delta.delta_rotation);
	item->setScale(delta.delta_scale, delta.delta_scale);

	_arr.push_back(item);

	if (_arr.size() == 1U)
	{
		// first item is special
		_arr[0]->selected();
	}
}

void Menu::clear()
{
	_index = 0U;
	for (auto it = _arr.begin(); it != _arr.end(); ++it)
		delete *it;
	_arr.clear();
}

bool Menu::handleKeyPressed(Event::KeyEvent key)
{
	if (_arr.empty()) return false;

	if (key.code == _done_key)
	{
		_arr[_index]->activated();
		return true;
	}
	else if (key.code == Keyboard::Up)
	{
		if (_index == 0U) return false;
		_arr[_index]->unselected();
		--_index;
		_arr[_index]->selected();
		return true;
	}
	else if (key.code == Keyboard::Down)
	{
		if (_index >= _arr.size() - 1) return false;
		_arr[_index]->unselected();
		++_index;
		_arr[_index]->selected();
		return true;
	}
	return false;
}

void Menu::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	for (size_t i = 0U; i < _arr.size(); ++i)
	{
		IItem * cur_item = _arr[i];
		target.draw(*cur_item, states);
	}
}
#include "query_scene.h"
#include "asset.h"
#include "director.h"
#include "global.h"
#include "util.h"

const Color grey = Color(100, 100, 100);

////////////////// QueryTextScene ////////////////////

QueryTextScene::QueryTextScene(const wstring & message, callback_t next)
	: _bg(*G.sprite_map[L"menu-bg"]),
	_query_label(message, G.default_font, 45),
	_input_text(G.default_font, 40),
	_next(next)
{
	{
		auto bounds = _bg.getLocalBounds();
		_bg.setScale(G.window.getSize().x/bounds.width,
			G.window.getSize().y/bounds.height);
	}

	{
		_query_label.setColor(Color(50, 50, 50));
		_query_label.setPosition(static_cast<float>(G.window.getSize().x/2),
			static_cast<float>(G.window.getSize().y/2) - 70.f);
		auto bounds = _query_label.getLocalBounds();
		_query_label.setOrigin(bounds.width/2, bounds.height/2);

	}

	{
		_input_text.setTextColor(Color::Black);
		_input_text.setCaretColor(Color::Black);
		_input_text.setPosition(static_cast<float>(G.window.getSize().x/2),
			static_cast<float>(G.window.getSize().y/2));
		UpdateInputTextOrigin();
	}
}

QueryTextScene::~QueryTextScene()
{
}

bool QueryTextScene::HandleIMEUpdated(wchar_t code, bool done)
{
	if (_input_text.handleIMEUpdated(code, done))
		UpdateInputTextOrigin();
	return true;
}

bool QueryTextScene::HandleTextEntered(wchar_t code)
{
	if (_input_text.handleEnteredKey(code))
	{
		UpdateInputTextOrigin();
		return true;
	}
	else return false;
}

bool QueryTextScene::HandleKeyPressed(Event::KeyEvent key)
{
	if (_input_text.handleKeyPress(key))
	{
		UpdateInputTextOrigin();
		return true;
	}
	else
	{
		if (key.code == Keyboard::Return)
		{
			_next(_input_text.getString());
			return true;
		}
		else return false;
	}
}

void QueryTextScene::Go()
{
	G.window.draw(_bg);
	G.window.draw(_query_label);
	G.window.draw(_input_text);
}

void QueryTextScene::UpdateInputTextOrigin()
{
	auto bounds = _input_text.getLocalBounds();
	_input_text.setOrigin(bounds.width/2, 0);
}

///////////////// QueryItemScene /////////////////

QueryItemScene::QueryItemScene(const wstring & message,
								const vector<wstring> & items,
								callback_t next)
	:_bg(*G.sprite_map[L"menu-bg"]),
	_query_label(message, G.default_font, 45),
	_menu(Menu::delta_t(Vector2f(0.f, 50.f), 0.f, 1.f), Keyboard::Return),
	_next(next)
{
	{
		auto bounds = _bg.getLocalBounds();
		_bg.setScale(G.window.getSize().x/bounds.width,
			G.window.getSize().y/bounds.height);
	}

	{
		_query_label.setColor(Color(50, 50, 50));
		_query_label.setPosition(static_cast<float>(G.window.getSize().x/2),
			static_cast<float>(G.window.getSize().y/2) - 70.f);
		auto bounds = _query_label.getLocalBounds();
		_query_label.setOrigin(bounds.width/2, bounds.height/2);
	}

	{
		_menu.setPosition(G.window.getSize().x/2.f, 230.f);

		class MyItem : public IItem
		{
		public:
			MyItem(const wstring & string, function<void()> next)
				: _text(string, G.default_font, 40),
				_rect(*G.sprite_map[L"selected"]),
				_next(next)
			{
				_text.setColor(grey);
				SetMiddleOfLocal(&_text);

				SetScaleToSize(&_rect,
					_text.getLocalBounds().width + 10.f,
					_text.getLocalBounds().height + 10.f);
				auto bounds = _text.getLocalBounds();
				_rect.move(bounds.left, bounds.top);
				SetMiddleOfLocal(&_rect);
			}

			virtual void selected()
			{
				_text.setColor(Color::Black);
				_draw_rect = true;
			}

			virtual void unselected()
			{
				_text.setColor(grey);
				_draw_rect = false;
			}

			virtual void activated()
			{
				_next();
			}

		private:
			virtual void draw(RenderTarget& target, RenderStates states) const
			{
				states.transform *= getTransform();
				target.draw(_text, states);
				if (_draw_rect) target.draw(_rect, states);
			}

		private:
			Text				_text;
			Sprite				_rect;
			bool				_draw_rect;
			function<void()>	_next;
		};

		size_t count = 0U;
		for (auto it = items.begin(); it != items.end(); ++it, ++count)
		{
			auto bound_next = _next;
			auto item = *it;
			_menu.pushItem(new MyItem(item, [count, bound_next]()
				{
					bound_next(count);
				}));
		}
	}
}

QueryItemScene::~QueryItemScene()
{
}

bool QueryItemScene::HandleKeyPressed(Event::KeyEvent key)
{
	return _menu.handleKeyPressed(key);
}

void QueryItemScene::Go()
{
	G.window.draw(_bg);
	G.window.draw(_query_label);
	G.window.draw(_menu);
}
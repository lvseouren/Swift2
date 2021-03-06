#ifndef MOUSEMANAGER_HPP
#define MOUSEMANAGER_HPP

#include <map>
#include <string>
#include <functional>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

namespace swift
{
	class MouseManager
	{
		public:
			void newBinding(const std::string& n, sf::Mouse::Button b, std::function<void(const sf::Vector2i&)> f = [](const sf::Vector2i&){return true;}, bool onPress = false)
			{
				bindings.emplace(std::make_pair(n, ButtonBinding(b, f, onPress)));
			}
			
			void call(const std::string& k, const sf::Vector2i& pos)
			{
				bindings.at(k).call(pos);
			}

			bool operator()(sf::Event& e)
			{
				for(auto &b : bindings)
				{
					if(b.second(e))
						return b.second.call({e.mouseButton.x, e.mouseButton.y});
				}
				
				return false;
			}

		private:
			class ButtonBinding
			{
				public:
					explicit ButtonBinding(sf::Mouse::Button b, std::function<void(const sf::Vector2i&)> f, bool p)
					{
						button = b;
						onPress = p;
						func = f;
					}

					sf::Mouse::Button getButton() const
					{
						return button;
					}

					bool operator()(sf::Event& e)
					{
						return ((e.type == sf::Event::MouseButtonPressed && onPress) || (e.type == sf::Event::MouseButtonReleased && !onPress)) && e.mouseButton.button == button;
					}

					bool call(sf::Vector2i pos)
					{
						if(!func)
							return false;

						func(pos);

						return true;
					}

				private:
					sf::Mouse::Button button;

					std::function<void(const sf::Vector2i&)> func;

					bool onPress;	// if true, means if key is pressed, if false, means if key is released
			};

			std::map<std::string, ButtonBinding> bindings;

	};
}

#endif // MOUSEMANAGER_HPP

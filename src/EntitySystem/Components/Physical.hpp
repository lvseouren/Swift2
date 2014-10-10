#ifndef PHYSICAL_HPP
#define PHYSICAL_HPP

#include "../Component.hpp"

#include <SFML/System/Vector2.hpp>

namespace swift
{
	class Physical : public Component
	{
		public:
			Physical();

			static std::string getType();
			
			virtual std::map<std::string, std::string> serialize() const;
			virtual void unserialize(const std::map<std::string, std::string>& variables);

			sf::Vector2f position;
			sf::Vector2u size;
	};
}

#endif // PHYSICAL_HPP

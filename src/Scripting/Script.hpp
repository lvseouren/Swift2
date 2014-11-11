#ifndef SCRIPT_HPP
#define SCRIPT_HPP

/*
 * Lua is expected to be compiled with float as lua_number!!!
 */

#include "LuaCpp/LuaCpp.hpp"

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "../GUI/Window.hpp"

#include "../Settings/Settings.hpp"

#include "../KeyBindings/KeyboardManager.hpp"

#include "../Logger/Logger.hpp"

/*
 * Each Script object expects two functions to exist in the
 * Lua script:
 *
 * Start()
 * Update()
 *
 * 1 variable is expected to exist:
 * Done
 *
 * 'Done' should be set to false during 'Start'.
 *
 * Start is called during Script construction.
 * This function should only do setup, etc.
 *
 * Update is called every game tick. If the state of the script
 * should ever change, that code goes in here.
 *
 * Finish is called at a game tick that finds 'Done' to be true
 */

namespace swift
{
	class AssetManager;
	class World;
	
	class Script
	{
		public:
			Script();
			~Script();
			
			bool loadFromFile(const std::string& file);

			void start();

			void update();
			
			bool load(const std::string& file);
			bool save(const std::string& file);

			bool toDelete();

			lpp::Selection getVariable(const std::string& name);
			
			// setters for variables that Lua has access to
			static void setWindow(sf::RenderWindow& win);
			static void setAssetManager(AssetManager& am);
			static void setClock(sf::Clock& c);
			static void setSettings(Settings& s);
			void setWorld(World& w);
			void setWorld(std::nullptr_t);
			
			// get world pointer for comparison
			const World* getWorld() const;

		private:
			void addVariables();
			void addClasses();
			void addFunctions();
			
			lpp::State luaState;
			
			bool deleteMe;
			
			// Variables that need to be accessed by Lua
			static sf::RenderWindow* window;
			static AssetManager* assets;
			static sf::Clock* clock;
			static Settings* settings;
			KeyboardManager* keyboard;
			World* world;
	};
}

#endif // SCRIPT_HPP

#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

#include "CppFunction.hpp"

using FunctionsMap = std::unordered_map<std::string, std::unique_ptr<lpp::BaseCppFunction>>;

namespace lpp
{
	class Selection
	{
		public:
			Selection(lua_State* s, const std::string& n, FunctionsMap& funcs);
			// if this Selection is part of a table, idx will not be 0, and will be the index in the stack of the table this Selection is part of
			Selection(lua_State* s, const std::string& n, FunctionsMap& funcs, int idx);
			~Selection();
			
			// lua function call
			template<typename... Args>
			Selection operator()(Args... args);
			
			// get multiple results from a Lua function
			template<typename... Ret>
			std::tuple<Ret...> getMultiReturn();
			
			// assignment operators
			template<typename T>
			void operator =(T& t);
			
			// assigning Lua variable to C++ function
			template<typename Ret, typename... Args>
			void operator =(const std::function<Ret(Args...)>& f);
			
			template<typename Ret, typename... Args>
			void operator =(Ret (*f)(Args... args));
			
			// casting to types
			template<typename T>
			operator T() const;
			
			template<typename T>
			operator std::vector<T>() const;
			
			template<typename K, typename V>
			operator std::map<K, V>() const;
			
			// getting type
			auto getType() const -> decltype(LUA_TNUMBER);
			
			// operator chaining
			Selection operator [](const std::string& n) const;
			Selection operator [](const int i) const;

		private:
			lua_State* state;
			std::string name;
			int index;
			
			FunctionsMap& functions;
	};
	
	// calling Lua function
	template<typename... Args>
	Selection Selection::operator()(Args... args)
	{
		lua_getglobal(state, name.c_str());
		
		detail::distributeArgs(state, args...);
		
		constexpr int nargs = sizeof...(Args);
		
		if(lua_pcall(state, nargs, LUA_MULTRET, 0) != LUA_OK)
		{
			std::size_t size;
			const char* buff = lua_tolstring(state, -1, &size);
			std::string msg = {buff, size};
			luaL_error(state, msg.c_str());
		}
		
		const int nrets = lua_gettop(state);
		
		if(nrets == 0)
			return Selection(state, "", functions);
		else
			return Selection(state, name + " return", functions, -nrets);
	}
	
	// get multiple results from a Lua function
	template<typename... Ret>
	std::tuple<Ret...> Selection::getMultiReturn()
	{
		return detail::getArgs<Ret...>(state);
	}
	
	// pushing
	template<typename T>
	void Selection::operator =(T& t)
	{
		detail::pushValue(state, t);
		lua_setglobal(state, name.c_str());
	}
	
	// assignment of Lua variable to C++ function
	template<typename Ret, typename... Args>
	void Selection::operator =(const std::function<Ret(Args...)>& f)
	{
		functions.emplace(name, std::unique_ptr<BaseCppFunction>(new CppFunction<Ret, Args...>(state, name, f)));
	}
	
	template<typename Ret, typename... Args>
	void Selection::operator =(Ret (*f)(Args... args))
	{
		functions.emplace(name, std::unique_ptr<BaseCppFunction>(new CppFunction<Ret, Args...>(state, name, f)));
	}
	
	// casting
	template<typename T>
	Selection::operator T() const
	{
		T ret;
		
		if(index == 0)
		{
			lua_getglobal(state, name.c_str());
			ret = detail::checkGet(detail::id<T>{}, state);
		}
		else
		{
			ret = detail::checkGet(detail::id<T>{}, state, index);
		}
		
		lua_pop(state, 1);
		return ret;
	}
	
	template<typename T>
	Selection::operator std::vector<T>() const
	{
		std::vector<T> ret;
		
		if(index == 0)
		{
			lua_getglobal(state, name.c_str());
			ret = detail::checkGet(detail::id<std::vector<T>>{}, state);
		}
		else
		{
			ret = detail::checkGet(detail::id<std::vector<T>>{}, state, index);
		}
		
		lua_pop(state, 1);
		return ret;
	}
			
	template<typename K, typename V>
	Selection::operator std::map<K, V>() const
	{
		std::map<K, V> ret;
		
		if(index == 0)
		{
			lua_getglobal(state, name.c_str());
			ret = detail::checkGet(detail::id<std::map<K, V>>{}, state);
		}
		else
		{
			ret = detail::checkGet(detail::id<std::map<K, V>>{}, state, index);
		}
		
		lua_pop(state, 1);
		return ret;
	}
}

#endif // SELECTION_HPP

#ifndef _ZPK_CONFIG_H
#define _ZPK_CONFIG_H
#include <iostream>
#include <fstream>
#include <filesystem>

#include <string>
#include <vector>
#include <iterator>
#include <numeric>
#include <optional>

// TODO: Change std::unordered_map to frozen::unordered_map (much faster)
#include <unordered_map>

#define ZPK_RETURN(condition, ...)\
	if (condition)\
		return __VA_ARGS__;\

#define _ZPK_DISABLE_CURRENT_WARNINGS 4267
#pragma warning(disable: _ZPK_DISABLE_CURRENT_WARNINGS)

#endif // !_ZPK_CONFIG_H

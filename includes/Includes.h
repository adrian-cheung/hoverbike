#ifndef THEGAME_INCLUDES_H
#define THEGAME_INCLUDES_H

// GLOBAL INCLUDES -- these are all accessible by with just `#include "Includes.h"`
#include "raylib-cpp.hpp"
#include <vector>
#include <string>
#include <memory>
#include <ranges>
#include <iostream>
#include <assert.h>

// GLOBAL typedefs/aliases -- shorten lengthy type names for more ergonomic coding :)

// raylib ========
using Vec2 = raylib::Vector2;
using Vec3 = raylib::Vector3;
using Vec4 = raylib::Vector4;

using RectF = raylib::Rectangle;
using RayTexture = raylib::Texture;
using RayColor = raylib::Color;


// GLOBAL using namespaces -- auto-include certain parts of std
using namespace std::views;
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

template <typename T>
using vector = std::vector<T>;

template <typename T>
using shared_ptr = std::shared_ptr<T>;

template <typename T>
using optional = std::optional<T>;

#define MAP(X) transform([&](auto it) X)

#endif //THEGAME_INCLUDES_H

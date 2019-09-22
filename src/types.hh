#pragma once
#include "glm.hh"
#include <string>
#include <unordered_map>
#include <vector>

using v2 = glm::vec2;
using v3 = glm::vec3;
using v4 = glm::vec4;
using mat4 = glm::mat4;
using quat = glm::quat;

using uint = unsigned int;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using string = std::string;

template <typename K, typename V>
using unordered_map = std::unordered_map<K, V>;

template <typename K>
using vector = std::vector<K>;
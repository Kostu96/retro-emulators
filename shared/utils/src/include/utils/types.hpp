/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <cstdint>
#include <cstddef>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using f32 = float;
using f64 = double;
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

template <typename T> inline constexpr u8 to_u8(T val) {
  return static_cast<u8>(val);
}
template <typename T> inline constexpr u16 to_u16(T val) {
  return static_cast<u16>(val);
}
template <typename T> inline constexpr u32 to_u32(T val) {
  return static_cast<u32>(val);
}
template <typename T> inline constexpr u64 to_u64(T val) {
  return static_cast<u64>(val);
}

template <typename T> inline constexpr s8 to_s8(T val) {
  return static_cast<s8>(val);
}
template <typename T> inline constexpr s16 to_s16(T val) {
  return static_cast<s16>(val);
}
template <typename T> inline constexpr s32 to_s32(T val) {
  return static_cast<s32>(val);
}
template <typename T> inline constexpr s64 to_s64(T val) {
  return static_cast<s64>(val);
}

template <typename T> inline constexpr f32 to_f32(T val) {
  return static_cast<f32>(val);
}
template <typename T> inline constexpr f64 to_f64(T val) {
  return static_cast<f64>(val);
}

class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

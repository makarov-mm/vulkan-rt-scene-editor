#pragma once
#include <cstdint>

inline constexpr uint32_t WIDTH  = 800;   // ray-traced viewport
inline constexpr uint32_t HEIGHT = 800;
inline constexpr uint32_t TB_H   = 46;    // toolbar strip above the viewport

inline constexpr uint32_t MAX_INSTANCES = 64;   // floor + light + editable objects
inline constexpr uint32_t FIXED_SLOTS   = 2;    // slot 0 = floor, slot 1 = light

#ifdef NDEBUG
inline constexpr bool kEnableValidation = false;
#else
inline constexpr bool kEnableValidation = true;
#endif

#pragma once
#include <cstdint>

static const uint32_t WIDTH  = 800;   // ray-traced viewport
static const uint32_t HEIGHT = 800;
static const uint32_t TB_H   = 46;    // toolbar strip above the viewport

static const uint32_t MAX_INSTANCES = 64;   // floor + light + editable objects
static const uint32_t FIXED_SLOTS   = 2;    // slot 0 = floor, slot 1 = light

#ifdef NDEBUG
static const bool kEnableValidation = false;
#else
static const bool kEnableValidation = true;
#endif

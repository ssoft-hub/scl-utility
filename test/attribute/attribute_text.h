#pragma once

// The brackets go in before the expansion, so an empty one is still an argument on MSVC.
#define SCL_TEST_TEXT_IMPL(...) #__VA_ARGS__
#define SCL_TEST_TEXT(...) SCL_TEST_TEXT_IMPL([__VA_ARGS__])

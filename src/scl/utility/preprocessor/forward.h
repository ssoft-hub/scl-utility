#pragma once

/**
 * @file
 * @brief Provides a utility macro for forcing the expansion of other macros.
 * @ingroup scl_utility_preprocessor
 * @details Provides small helper macros for token forwarding and macro expansion
 *          in C++ codebases. These macros operate at preprocessing time and do
 *          not introduce runtime overhead.
 */

/**
 * @def SCL_FORWARD(...)
 * @brief Identity macro that forwards variadic arguments unchanged and can force expansion of
 * nested macros.
 * @ingroup scl_utility_preprocessor
 * @details
 * This macro simply expands to its variadic arguments. It is useful when:
 * - You need to forward comma-separated tokens as-is into another macro, function call, or template
 *   parameter list.
 * - You want to force expansion in macro chaining scenarios where another macro yields
 *   comma-separated tokens.
 *
 * @param ... Variadic tokens to forward unchanged.
 *
 * @note
 * - This macro does not add parentheses or alter tokenization; it only forwards tokens.
 * - It is safe to use in both C and C++ code. In C++ contexts, it is commonly used with template
 *   parameter packs and function calls.
 *
 * @code{.cpp}
 * // Example 1: Forwarding arguments to a function call
 * #define CALL(fn, ...) fn(__VA_ARGS__)
 *
 * int add(int a, int b) { return a + b; }
 * int result = CALL(add, SCL_FORWARD(1, 2)); // expands to add(1, 2)
 *
 * // Example 2: Forcing expansion for template parameter lists
 * #define BASIC_TYPES int, long, double
 *
 * template <typename... Ts>
 * struct type_list {};
 *
 * using tl = type_list<SCL_FORWARD(BASIC_TYPES)>; // expands to type_list<int, long, double>
 *
 * // Example 3: Chaining macros that produce comma-separated tokens
 * #define MAKE_PAIR(a, b) a, b
 * auto p = std::pair<SCL_FORWARD(MAKE_PAIR(int, double))>{1, 2.0};
 *
 * // Example 4: Forwarding arguments through another macro layer
 * #define FORWARD_TO_EMPLACE(obj, ...) obj.emplace_back(__VA_ARGS__)
 *
 * std::vector<std::pair<int, std::string>> v;
 * FORWARD_TO_EMPLACE(v, SCL_FORWARD(42, "answer")); // emplace_back(42, "answer")
 * @endcode
 *
 * @warning
 * - Be mindful of operator precedence; if you need grouping, use parentheses in the call sites.
 * - Since macros are global, avoid generic names to reduce collision risk (SCL_ prefix is used for
 *   this reason).
 */
#ifndef SCL_FORWARD
#define SCL_FORWARD(...) __VA_ARGS__
#endif

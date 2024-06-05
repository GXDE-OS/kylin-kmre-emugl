// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ASTC_CODEC_BASE_UTILS_H_
#define ASTC_CODEC_BASE_UTILS_H_

#include <cassert>
#include <cstdio>
#include <cstdlib>

#ifdef NDEBUG
#define UTILS_RELEASE_ASSERT(x)                                        \
  do {                                                                 \
    const bool result = (x);                                           \
    if (!result) {                                                     \
      fprintf(stderr, "Error: UTILS_RELEASE_ASSERT failed: %s\n", #x); \
      abort();                                                         \
    }                                                                  \
  } while (false)
#else
#define UTILS_RELEASE_ASSERT(x) assert(x)
#endif

// In C++11, `assert` can't be used portably within constexpr functions.
// ASTC_CONSTEXPR_ASSERT functions as a runtime assert but works in C++11
// constexpr functions. Example:
//
// constexpr double Divide(double a, double b) {
//   return ASTC_CONSTEXPR_ASSERT(b != 0), a / b;
// }
//
// This macro is based on ABSL_ASSERT.
#ifdef NDEBUG
#define ASTC_CONSTEXPR_ASSERT(expr) \
  (false ? static_cast<void>(expr) : static_cast<void>(0))
#else
#define ASTC_CONSTEXPR_ASSERT(expr) \
  ((expr) ? static_cast<void>(0) : [] { assert(false && #expr); }())
#endif

#endif  // ASTC_CODEC_BASE_UTILS_H_

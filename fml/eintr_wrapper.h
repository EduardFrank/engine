// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FML_EINTR_WRAPPER_H_
#define FLUTTER_FML_EINTR_WRAPPER_H_

#include "flutter/fml/build_config.h"

#include <errno.h>

#if defined(OS_WIN)

// Windows has no concept of EINTR.
#define FML_HANDLE_EINTR(x) (x)
#define FML_IGNORE_EINTR(x) (x)

#else

#if defined(NDEBUG)

#define FML_HANDLE_EINTR(x)                                 \
  ({                                                        \
    decltype(x) eintr_wrapper_result;                       \
    do {                                                    \
      eintr_wrapper_result = (x);                           \
    } while (eintr_wrapper_result == -1 && errno == EINTR); \
    eintr_wrapper_result;                                   \
  })

#else

#define FML_HANDLE_EINTR(x)                                  \
  ({                                                         \
    int eintr_wrapper_counter = 0;                           \
    decltype(x) eintr_wrapper_result;                        \
    do {                                                     \
      eintr_wrapper_result = (x);                            \
    } while (eintr_wrapper_result == -1 && errno == EINTR && \
             eintr_wrapper_counter++ < 100);                 \
    eintr_wrapper_result;                                    \
  })

#endif  // NDEBUG

#define FML_IGNORE_EINTR(x)                               \
  ({                                                      \
    decltype(x) eintr_wrapper_result;                     \
    do {                                                  \
      eintr_wrapper_result = (x);                         \
      if (eintr_wrapper_result == -1 && errno == EINTR) { \
        eintr_wrapper_result = 0;                         \
      }                                                   \
    } while (0);                                          \
    eintr_wrapper_result;                                 \
  })

#endif  // defined(OS_WIN)

#endif  // FLUTTER_FML_EINTR_WRAPPER_H_

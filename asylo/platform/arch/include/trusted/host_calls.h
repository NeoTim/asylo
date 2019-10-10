/*
 *
 * Copyright 2018 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ASYLO_PLATFORM_ARCH_INCLUDE_TRUSTED_HOST_CALLS_H_
#define ASYLO_PLATFORM_ARCH_INCLUDE_TRUSTED_HOST_CALLS_H_

// Defines the C language interface to the untrusted host environment. These
// functions invoke code outside the enclave and secure applications must assume
// an adversarial implementation.

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <poll.h>
#include <pwd.h>
#include <sched.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include "asylo/platform/core/shared_name_kind.h"

#ifdef __cplusplus
extern "C" {
#endif

// Unless otherwise specified, each of the following calls invokes the
// corresponding function on the host.

//////////////////////////////////////////////////
//          Untrusted Heap Allocations          //
//////////////////////////////////////////////////

// Allocates |count| buffers of size |size| on the untrusted heap, returning a
// pointer to an array of buffer pointers.
void **enc_untrusted_allocate_buffers(
    size_t count, size_t size);

// Releases memory on the untrusted heap pointed to by buffer pointers stored in
// |free_list|.
void enc_untrusted_deallocate_free_list(void **free_list, size_t count);

//////////////////////////////////////
//          Error Handling          //
//////////////////////////////////////

// Fetches the value of errno from the untrusted C runtime.
int enc_untrusted_get_errno();

//////////////////////////////////////
//            Threading             //
//////////////////////////////////////

// Exits the enclave and, if the value stored at |futex| equals |expected|,
// suspends the calling thread until it is resumed by a call to
// enc_untrusted_sys_futex_wake. Otherwise returns immediately.
void enc_untrusted_sys_futex_wait(int32_t *futex, int32_t expected);

// Exits the enclave and wakes a suspended thread blocked on |futex|.
void enc_untrusted_sys_futex_wake(int32_t *futex);

//////////////////////////////////////
//            inotify.h             //
//////////////////////////////////////

int enc_untrusted_inotify_read(int fd, size_t count, char **serialized_events,
                               size_t *serialized_events_len);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // ASYLO_PLATFORM_ARCH_INCLUDE_TRUSTED_HOST_CALLS_H_

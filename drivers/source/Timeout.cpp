/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
 */
#include "drivers/Timeout.h"
#include "drivers/LowPowerTimeout.h"
#include "drivers/HighResClock.h"
#include "drivers/LowPowerClock.h"
#include "drivers/RealTimeClock.h"

using namespace std::chrono;

namespace mbed {

void TimeoutBase::handler()
{
    if (_function) {
        if (_lock_deepsleep) {
            sleep_manager_unlock_deep_sleep();
        }
        _function();
        _function = nullptr;
    }
}

Timeout::Timeout() : TimeoutBase(get_us_ticker_data(), true)
{
}

/* A few miscellaneous out-of-line static members from various related classes,
 * just to save them getting needing their own cpp file for one line.
 * (In C++17 could avoid the need for this by making the members inline).
 */
const bool HighResClock::is_steady;
const bool SteadyHighResClock::is_steady;
const bool LowPowerClock::is_steady;
mstd::once_flag SteadyHighResClock::init;
const bool RealTimeClock::is_steady;

#if DEVICE_LPTICKER
LowPowerTimeout::LowPowerTimeout() : TimeoutBase(get_lp_ticker_data(), true)
{
}
#endif

} // namespace mbed

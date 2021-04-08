/*
 * Copyright (C) 2021 William Wold
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EVENT_FILTER_H
#define EVENT_FILTER_H

#include <miral/toolkit_event.h>

namespace tiler
{
class TilerShell;

class EventFilter
{
public:
    EventFilter(TilerShell* const shell);
    ~EventFilter() = default;

    auto filter_event(MirEvent const* event) -> bool;

private:
    EventFilter(EventFilter const&) = delete;
    EventFilter& operator=(EventFilter const&) = delete;

    /// Always outlives this object
    TilerShell* const shell;

    auto filter_input_event(MirInputEvent const* event) -> bool;
    auto filter_touch_event(MirTouchEvent const* event) -> bool;
    auto filter_key_down_event(MirKeyboardEvent const* event) -> bool;
};

}

#endif // EVENT_FILTER_H

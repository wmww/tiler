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

#include "event_filter.h"
#include "tiler_shell.h"
#include <linux/input.h>

using namespace tiler;
using namespace miral::toolkit;

EventFilter::EventFilter(TilerShell* const shell)
    : shell{shell}
{
}

auto EventFilter::filter_event(MirEvent const* event) -> bool
{
    switch (mir_event_get_type(event))
    {
    case mir_event_type_input:
        return filter_input_event(mir_event_get_input_event(event));

    default:
        return false;
    }
}

auto EventFilter::filter_input_event(MirInputEvent const* event) -> bool
{
    switch (mir_input_event_get_type(event))
    {
    case mir_input_event_type_key:
        return filter_keyboard_event(mir_input_event_get_keyboard_event(event));

    case mir_input_event_type_touch:
        return filter_touch_event(mir_input_event_get_touch_event(event));

    default:
        return false;
    }
}

auto EventFilter::filter_touch_event(MirTouchEvent const* event) -> bool
{
    return false;
}

auto EventFilter::filter_keyboard_event(MirKeyboardEvent const* event) -> bool
{
    MirInputEventModifiers const mods = mir_keyboard_event_modifiers(event);

    if ((mods & mir_input_event_modifier_alt) && (mods & mir_input_event_modifier_ctrl))
    {
        switch (mir_keyboard_event_scan_code(event))
        {
        case KEY_BACKSPACE:
            shell->request_stop();
            return true;
        }
    }

    return false;
}

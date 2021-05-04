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
#include "window_manager.h"
#include <miral/external_client.h>
#include <miral/runner.h>
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
    {
        auto const key_ev = mir_input_event_get_keyboard_event(event);
        if (mir_keyboard_event_action(key_ev) == mir_keyboard_action_down)
            return filter_key_down_event(key_ev);
        else
            return false;
    }

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

auto EventFilter::filter_key_down_event(MirKeyboardEvent const* event) -> bool
{
    MirInputEventModifiers const mods = mir_keyboard_event_modifiers(event);

    if ((mods & mir_input_event_modifier_meta) &&
        (mods & mir_input_event_modifier_shift) &&
        (mods & mir_input_event_modifier_ctrl))
    {
        switch (mir_keyboard_event_scan_code(event))
        {
        case KEY_BACKSPACE:
            shell->runner->stop();
            return true;
        }
    }
    else if ((mods & mir_input_event_modifier_ctrl) &&
             (mods & mir_input_event_modifier_alt))
    {
        switch (mir_keyboard_event_scan_code(event))
        {
        case KEY_T:
            launch_terminal();
            return true;
        }
    }
    else if (mods & mir_input_event_modifier_meta)
    {
        switch (mir_keyboard_event_scan_code(event))
        {
        case KEY_SPACE:
            // Synapse, an app launcher
            shell->launcher->launch({"synapse"});
            return true;

        case KEY_ENTER:
            launch_terminal();
            return true;

        case KEY_F:
            // File manager
            shell->launcher->launch({"nemo"});
            return true;

        case KEY_V:
            // Volume control
            shell->launcher->launch({"pavucontrol"});
            return true;

        case KEY_EQUAL:
            // Calculator
            shell->launcher->launch({"gnome-calculator"});
            return true;

        case KEY_Q:
            // Exit the currently active app
            shell->wm->close_active_window();
            return true;
        }
    }

    return false;
}

void EventFilter::launch_terminal()
{
    shell->launcher->launch({"lxterminal"});
}

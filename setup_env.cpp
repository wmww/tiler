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

#include "tiler_shell.h"

#include <iostream>
#include <unistd.h>

// Logic in this file mirrors that in libwayland. See https://github.com/wayland-project/wayland/blob/34306efeb261f8466ad427a61cdae3272b8bae4d/src/wayland-server.c#L1494

/// If $WAYLAND_DISPLAY points to a display that's already in use by another compositor, unset it so Mir will choose a
/// free display instead of failing to start (IMO Mir should do this automatically, but so far I've lost that argument)
static void unset_wayland_display_if_its_taken()
{
    auto const wayland_display_c_str = getenv("WAYLAND_DISPLAY");
    if (wayland_display_c_str)
    {
        std::string wayland_display{wayland_display_c_str};
        // this is how you have to do !wayland_display.starts_with() in C++ <20 lmao
        if (wayland_display.rfind("/", 0) != 0)
        {
            std::string xdg_runtime_dir = "/run/user/1000";
            if (auto const value = getenv("XDG_RUNTIME_DIR"))
            {
                xdg_runtime_dir = value;
            }
            wayland_display = xdg_runtime_dir + "/" + wayland_display;
        }
        if (access(wayland_display.c_str(), F_OK) != -1)
        {
            // File exists
            std::cerr
                << "Wayland display at "
                << wayland_display
                << " exists, unsetting WAYLAND_DISPLAY to let Mir choose another one"
                << std::endl;
            unsetenv("WAYLAND_DISPLAY");
        }
    }
}

void tiler::setup_env()
{
    // enable XWayland support
    setenv("MIR_SERVER_ENABLE_X11", "1", 1);

    unset_wayland_display_if_its_taken();
}

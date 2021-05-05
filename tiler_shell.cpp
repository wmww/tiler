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
#include "event_filter.h"
#include "window_manager/shell_window_manager.h"

#include <miral/runner.h>
#include <miral/x11_support.h>
#include <miral/wayland_extensions.h>
#include <miral/display_configuration.h>
#include <miral/keymap.h>
#include <miral/append_event_filter.h>
#include <miral/external_client.h>

using namespace tiler;
using namespace miral;

TilerShell::TilerShell(int argc, char const* argv[])
    : launcher{std::make_unique<ExternalClientLauncher>()},
      wm{ShellWindowManager::make_impl()},
      runner{std::make_unique<MirRunner>(argc, argv)},
      event_filter{std::make_unique<EventFilter>(this)}
{
}

TilerShell::~TilerShell()
{
}

auto TilerShell::run() -> int
{
    return runner->run_with(
        {
            miral::X11Support{},
            miral::WaylandExtensions{}
                .enable(miral::WaylandExtensions::zwlr_layer_shell_v1)
                .enable(miral::WaylandExtensions::zwlr_foreign_toplevel_manager_v1),
            miral::DisplayConfiguration{*runner},
            miral::Keymap{},
            miral::AppendEventFilter{[this](auto event)
                {
                    return event_filter->filter_event(event);
                }},
            *launcher,
            dynamic_cast<ShellWindowManager*>(wm.get())->runner_option(this),
        });
}

void TilerShell::request_stop()
{
    runner->stop();
}

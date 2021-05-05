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

#include "window_manager.h"
#include "tiler_shell.h"

#include <miral/minimal_window_manager.h>
#include <miral/application_info.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>
#include <miral/set_window_management_policy.h>

#include <mutex>

using namespace tiler;
using namespace miral;
using namespace mir::geometry;

namespace
{
class WindowManagerImpl;

/// Interface to Mir window management functionality. Owned by Mir. The shell, and thus the ShellWindowManager always
/// outlive this object
class WindowManagerImpl : public tiler::WindowManager, public miral::MinimalWindowManager
{
public:
    WindowManagerImpl(miral::WindowManagerTools const& tools, TilerShell* shell, std::function<void()>&& on_destroy)
        : MinimalWindowManager{tools},
          shell{shell},
          on_destroy{std::move(on_destroy)}
    {
    }

    ~WindowManagerImpl()
    {
        on_destroy();
    }

    void close_active_window() override
    {
        tools.ask_client_to_close(tools.active_window());
    }

    TilerShell* const shell;
    std::function<void()> const on_destroy;
    using MinimalWindowManager::tools;
};
}

/// Declared in shell_window_manager.cpp
auto wm_impl_runner_option(
    TilerShell* shell,
    std::function<void(WindowManager*, miral::WindowManagerTools*)> set_window_manager
) -> std::function<void(mir::Server&)>
{
    return SetWindowManagementPolicy{
        [shell, set_window_manager](miral::WindowManagerTools const& tools) -> std::unique_ptr<WindowManagementPolicy>
        {
            auto impl = std::make_unique<WindowManagerImpl>(
                tools,
                shell,
                [set_window_manager]()
                {
                    set_window_manager(nullptr, nullptr);
                });
            set_window_manager(impl.get(), &impl->tools);
            return impl;
        }};
}

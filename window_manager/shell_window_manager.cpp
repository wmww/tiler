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

#include "shell_window_manager.h"
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

/// The runner option for creating a WindowManagerImpl. set_window_manager is called with the wm when it is created
/// and called again with nullptr when it is destroyed. The wm is owned by Mir.
auto wm_impl_runner_option(
    TilerShell* shell,
    std::function<void(WindowManager*, miral::WindowManagerTools*)> set_window_manager
) -> std::function<void(mir::Server&)>;

namespace
{

/// This class wraps a WindowManagerImpl. This class is owned by the shell and should be called into without a window
/// manager lock.
class ShellWindowManagerImpl : public tiler::ShellWindowManager
{
public:
    void close_active_window() override
    {
        with_locked_wm([this](WindowManager* wm)
            {
                wm->close_active_window();
            });
    }

    void focus_next_window() override
    {
        with_locked_wm([this](WindowManager* wm)
            {
                wm->focus_next_window();
            });
    }

    void focus_prev_window() override
    {
        with_locked_wm([this](WindowManager* wm)
            {
                wm->focus_prev_window();
            });
    }

    auto runner_option(TilerShell* shell) -> std::function<void(mir::Server&)> override
    {
        return wm_impl_runner_option(shell, [this](WindowManager* wm, WindowManagerTools* tools)
            {
                std::lock_guard<std::mutex> lock{mutex};
                wm_unsafe = wm;
                tools_unsafe = tools;
            });
    }

    std::mutex mutex;
    /// Should only be used in runner_option() and with_locked_wm()
    WindowManager* wm_unsafe{nullptr};
    WindowManagerTools* tools_unsafe{nullptr};

private:
    void with_locked_wm(std::function<void(WindowManager* wm)> const& callback)
    {
        std::lock_guard<std::mutex> lock{mutex};
        if (wm_unsafe && tools_unsafe)
        {
            tools_unsafe->invoke_under_lock([&]()
                {
                    callback(wm_unsafe);
                });
        }
    }
};

}

auto ShellWindowManager::make_impl() -> std::unique_ptr<ShellWindowManager>
{
    return std::make_unique<ShellWindowManagerImpl>();
}

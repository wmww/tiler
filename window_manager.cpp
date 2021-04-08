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

using namespace tiler;
using namespace miral;
using namespace mir::geometry;

namespace
{

class WindowManagerImpl : public miral::MinimalWindowManager, public tiler::WindowManager
{
public:
    WindowManagerImpl(miral::WindowManagerTools const& tools, TilerShell* const shell)
        : MinimalWindowManager{tools},
          shell{shell}
    {
        shell->wm = this;
    }

    ~WindowManagerImpl()
    {
        shell->wm = &NullWindowManager::instance;
    }

    void close_active_window() override
    {
        tools.ask_client_to_close(tools.active_window());
    }

private:
    TilerShell* const shell;
};

}

auto WindowManager::make_setter_upper(TilerShell* const shell) -> std::function<void(mir::Server&)>
{
    return miral::set_window_management_policy<WindowManagerImpl>(shell);
}

NullWindowManager NullWindowManager::instance;

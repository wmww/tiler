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

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <functional>

namespace mir
{
class Server;
}

namespace miral
{
class WindowManagerTools;
}

namespace tiler
{
class TilerShell;

class WindowManager
{
public:
    WindowManager() = default;
    virtual ~WindowManager() = default;

    virtual void close_active_window() = 0;

    static auto make_setter_upper(TilerShell* const shell) -> std::function<void(mir::Server&)>;

private:
    WindowManager(WindowManager const&) = delete;
    WindowManager& operator=(WindowManager const&) = delete;
};

/// Used when no window manager is available, such as during startup
class NullWindowManager: public WindowManager
{
public:
    void close_active_window() override {};

    static NullWindowManager instance;
};

}

#endif // WINDOW_MANAGER_H

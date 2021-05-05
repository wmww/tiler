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

#ifndef SHELL_WINDOW_MANAGER_H
#define SHELL_WINDOW_MANAGER_H

#include "window_manager.h"

#include <functional>

namespace mir
{
class Server;
}

namespace tiler
{
class TilerShell;

class ShellWindowManager : public WindowManager
{
public:
    static auto make_impl() -> std::unique_ptr<ShellWindowManager>;

    virtual auto runner_option(TilerShell* shell) -> std::function<void(mir::Server&)> = 0;
};

}

#endif // SHELL_WINDOW_MANAGER_H

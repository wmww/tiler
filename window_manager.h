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

#include <miral/minimal_window_manager.h>

using namespace mir::geometry;

namespace tiler
{
class TilerShell;

class WindowManager : public miral::MinimalWindowManager
{
public:
    WindowManager(miral::WindowManagerTools const& tools, TilerShell* const shell);
    ~WindowManager();

private:
    TilerShell* const shell;
};

}

#endif // WINDOW_MANAGER_H

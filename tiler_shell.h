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

#ifndef TILER_SHELL_H
#define TILER_SHELL_H

#include <memory>
#include <optional>

namespace miral
{
class MirRunner;
}

namespace tiler
{
class EventFilter;
class WindowManager;

class TilerShell
{
public:
    TilerShell(int argc, char const* argv[]);
    ~TilerShell();

    /// Returns the return code from the runner
    auto run() -> int;

    /// Requests the runner be stopped
    void request_stop();

private:
    TilerShell(TilerShell const&) = delete;
    TilerShell& operator=(TilerShell const&) = delete;

    std::unique_ptr<miral::MirRunner> const runner;
    std::unique_ptr<EventFilter> const event_filter;
    std::optional<WindowManager*> wm_policy;
};

}

#endif // TILER_SHELL_H

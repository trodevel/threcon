/*

Thread Controller

Copyright (C) 2014 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 1716 $ $Date:: 2015-04-21 #$ $Author: serge $

#ifndef LIB_THRECON_CONTROLLER_H
#define LIB_THRECON_CONTROLLER_H

#include <set>                  // std::set
#include <mutex>                // std::mutex
#include <condition_variable>   // std::condition_variable

#include "namespace_lib.h"      // NAMESPACE_THRECON_START
#include "i_controllable.h"     // IControllable

NAMESPACE_THRECON_START

class Controller
{
public:

    Controller();

    bool register_client( IControllable* );

    void thread_func();

    void send_start();
    void send_stop();
    void send_shutdown();

private:
    void wakeup();

private:
    enum commands_e
    {
        NONE = 0,
        START,
        STOP,
        SHUTDOWN
    };

    typedef std::set<IControllable*>    SetControllable;

private:

    mutable std::mutex              mutex_;
    mutable std::mutex              mutex_cond_;
    mutable std::condition_variable cond_;

    SetControllable             clients_;

    commands_e                  command_;
};

NAMESPACE_THRECON_END

#endif // LIB_THRECON_CONTROLLER_H

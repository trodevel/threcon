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

// $Revision: 1717 $ $Date:: 2015-04-21 #$ $Author: serge $

#include "controller.h"         // self

#include <algorithm>                // std::for_each
#include <functional>               // std::bind

#include "../utils/mutex_helper.h"  // MUTEX_SCOPE_LOCK
#include "../utils/dummy_logger.h"  // dummy_log

#define MODULENAME      "Controller"

NAMESPACE_THRECON_START

Controller::Controller(): command_( commands_e::NONE )
{
}

bool Controller::register_client( IControllable* c )
{
    MUTEX_SCOPE_LOCK( mutex_ );

    bool res = clients_.insert( c ).second;

    if( res == false )
        return false;

    return true;
}

void Controller::thread_func()
{
    dummy_log_info( MODULENAME, "thread_func: started" );

    bool should_run    = true;
    while( should_run )
    {
        {
            std::unique_lock<std::mutex> lock( mutex_cond_ );
            cond_.wait( lock );
        }

        {
            MUTEX_SCOPE_LOCK( mutex_ );

            switch( command_ )
            {
            case START:
                // currently start/stop are disabled, e814
                //std::for_each( clients_.begin(), clients_.end(), std::bind( &IControllable::start, std::placeholders::_1 ));
                break;

            case STOP:
                // currently start/stop are disabled, e814
                //std::for_each( clients_.begin(), clients_.end(), std::bind( &IControllable::stop, std::placeholders::_1 ));
                break;

            case SHUTDOWN:
                std::for_each( clients_.begin(), clients_.end(), std::bind( &IControllable::shutdown, std::placeholders::_1 ));
                should_run  = false;
                break;

            default:
                dummy_log_error( MODULENAME, "unrecognized command: %u", command_ );
                break;
            }

            command_    = NONE;
        }
    }

    dummy_log_info( MODULENAME, "thread_func: ended" );
}

void Controller::send_start()
{
    MUTEX_SCOPE_LOCK( mutex_ );

    command_ = START;

    wakeup();
}

void Controller::send_stop()
{
    MUTEX_SCOPE_LOCK( mutex_ );

    command_ = STOP;

    wakeup();
}

void Controller::send_shutdown()
{
    MUTEX_SCOPE_LOCK( mutex_ );

    command_ = SHUTDOWN;

    wakeup();
}


void Controller::wakeup()
{
    // PRIVATE:

    cond_.notify_one();     // wake-up the thread
}

NAMESPACE_THRECON_END

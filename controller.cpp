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

// $Id: controller.cpp 961 2014-08-14 18:12:04Z serge $

#include "controller.h"         // self

#include "../utils/wrap_mutex.h"    // SCOPE_LOCK
#include "../utils/dummy_logger.h"  // dummy_log

#define MODULENAME      "Controller"

NAMESPACE_THRECON_START

Controller::Controller(): command_( commands_e::NONE )
{
}

bool Controller::register_client( IControllable* c )
{
    SCOPE_LOCK( mutex_ );

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
            cond_.wait( mutex_cond_ );
        }

        {
            SCOPE_LOCK( mutex_ );

            switch( command_ )
            {
            case START:
                // currently start/stop are disabled, e814
                //std::for_each( clients_.begin(), clients_.end(), boost::bind( &IControllable::start, _1 ));
                break;

            case STOP:
                // currently start/stop are disabled, e814
                //std::for_each( clients_.begin(), clients_.end(), boost::bind( &IControllable::stop, _1 ));
                break;

            case SHUTDOWN:
                std::for_each( clients_.begin(), clients_.end(), boost::bind( &IControllable::shutdown, _1 ));
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
    SCOPE_LOCK( mutex_ );

    command_ = START;

    wakeup();
}

void Controller::send_stop()
{
    SCOPE_LOCK( mutex_ );

    command_ = STOP;

    wakeup();
}

void Controller::send_shutdown()
{
    SCOPE_LOCK( mutex_ );

    command_ = SHUTDOWN;

    wakeup();
}


void Controller::wakeup()
{
    // PRIVATE:

    cond_.notify_one();     // wake-up the thread
}

NAMESPACE_THRECON_END

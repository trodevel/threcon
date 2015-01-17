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

// $Revision: 1404 $ $Date:: 2015-01-16 #$ $Author: serge $

#ifndef LIB_THRECON_I_CONTROLLABLE_H
#define LIB_THRECON_I_CONTROLLABLE_H

#include "namespace_lib.h"  // NAMESPACE_THRECON_START

NAMESPACE_THRECON_START

class IControllable
{
public:
    virtual ~IControllable() {}

    // currently start/stop are disabled, e814
    //virtual bool start()        = 0;
    //virtual bool stop()         = 0;
    virtual bool shutdown()     = 0;
};

NAMESPACE_THRECON_END

#endif // LIB_THRECON_I_CONTROLLABLE_H

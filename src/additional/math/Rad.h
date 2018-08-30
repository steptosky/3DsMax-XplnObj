#pragma once

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*
**  Copyright (C) 2017, StepToSky and FlightFactor
**  All rights reserved
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.The name of StepToSky or the name of FlightFactor or the names of its
**    contributors may NOT be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com or www.flightfactor.aero
*/

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#include <limits>

namespace stsff {
namespace math {

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Converts radians to degrees
     */
    constexpr double radToDeg(double inRad) {
        return (inRad * 180.0) / 3.14159265358979323846;
    }

    /*!
     * \details Converts radians to degrees
     */
    constexpr float radToDeg(float inRad) {
        return (inRad * 180.0f) / 3.14159265358979323846f;
    }

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Represents Radians value
     * \note For simple conversion use the global function \link radToDeg \endlink
     */
    template<typename T>
    class Rad {
        static_assert(std::numeric_limits<T>::is_iec559, "This is for the floating types only");
    public:

        typedef T value_type;

        //-------------------------------------------------------------------------
        // @{

        value_type value;

        // @}
        //-------------------------------------------------------------------------
        // @{

        Rad()
            : value(value_type(0)) {}

        explicit Rad(const value_type val)
            : value(val) {}

        ~Rad() = default;

        // @}
        //-------------------------------------------------------------------------
        // @{

        operator value_type() const {
            return value;
        }

        // @}
        //-------------------------------------------------------------------------
        // @{

        value_type toDeg() const {
            return radToDeg(value);
        }

        Rad<float> toFloat() const {
            return Rad<float>(float(value));
        }

        Rad<double> toDouble() const {
            return Rad<double>(double(value));
        }

        // @}
        //-------------------------------------------------------------------------

    };

    /**************************************************************************************************/
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /**************************************************************************************************/
}
}

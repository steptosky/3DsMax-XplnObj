#pragma once

/*
**  Copyright(C) 2017, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
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
**  Contacts: www.steptosky.com
*/

namespace sts {

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Guarantees only one instance in process scope.\n
 * Example of using
 * \code
 * class SingleTest : public sts::Single< SingleTest > {
 *
 *    friend sts::Single< SingleTest >;
 *    SingleTest() = default;
 *    ~SingleTest() = default;
 *    
 *    public:
 *    
 *    //...
 *
 * };
 * \endcode
 */
template<class Type>
class Single {
public:

    /*!
     * \details Gets pointer to instance of the "Type" using constructors with arguments.
     * \note This method will not create new instance if it is already exist.
     * \tparam Args 
     * \param vals 
     * \return Pointer to created instance.
     */
    template<class ... Args>
    static Type * init(Args && ... vals) {
        if (mInstanceCounter == 0) {
            mInstance = new Type(vals ...);
        }
        mInstanceCounter++;
        return mInstance;
    }

    /*!
     * \details Gets pointer to instance of the "Type".
     */
    static Type * instance() {
        if (mInstanceCounter == 0) {
            mInstance = new Type();
        }
        mInstanceCounter++;
        return mInstance;
    }

    /*!
     * \details Checks whether instance is instanced.
     */
    static bool isInstanced() {
        return mInstance != nullptr;
    }

    /*!
     * \details Checks whether instance is instanced.
     * \deprecated Use Single::isInstanced instead.
     */
    [[deprecated("Use Single::isInstanced instead")]]
    static bool isInitialized() {
        return mInstance != nullptr;
    }

    /*!
     * \details Gets internal counter value.
     * This counter shows how many parts of a project code have not called Single::safeFree().
     */
    static unsigned usedCount() {
        return mInstanceCounter;
    }

    /*!
     * \details One decrement, if the counter is 0 then instance will be deleted.
     */
    static void safeFree() {
        if (mInstanceCounter != 0) {
            mInstanceCounter--;
            if (mInstanceCounter == 0) {
                free();
            }
        }
    }

    /*!
     * \details Deletes instance and set counter to 0
     */
    static void free() {
        if (mInstance != nullptr) {
            delete mInstance;
        }
        mInstance = nullptr;
        mInstanceCounter = 0;
    }

protected:

    friend Type;
    Single(const Single &) = delete;
    Single & operator =(const Single &) = delete;

    Single() = default;
    ~Single() = default;

    static Type * mInstance;
    static unsigned int mInstanceCounter;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

template<typename Type>
Type * Single<Type>::mInstance = nullptr;

template<typename Type>
unsigned int Single<Type>::mInstanceCounter = 0;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}

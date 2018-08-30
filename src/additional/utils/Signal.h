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

#include <cassert>
#include <cstdint>
#include <string>
#include <list>

#ifdef STSU_TESTING
#	define Testing(X) X
#	define NoTesting(X)
#else
#	define Testing(X)
#	define NoTesting(X) X
#endif

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

namespace sts {
class HasSlots;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace sts_internal {
class IAutoDeleteDelegate;

class __IDeleteDelegate {
    friend sts::HasSlots;
    virtual void deleteDelegate() = 0;
public:
    virtual ~__IDeleteDelegate() = default;
};
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace sts {
class HasSlots {
    friend sts_internal::IAutoDeleteDelegate;
    Testing(public:)
    std::list<sts_internal::__IDeleteDelegate *> mDelegates;
public:
    virtual ~HasSlots() {
        for (auto & d : mDelegates)
            d->deleteDelegate();
    }
};
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace sts_internal {

class SignalBase {
    friend IAutoDeleteDelegate;
    virtual void _deleteDelegate(IAutoDeleteDelegate * inDelegate) = 0;
public:
    virtual ~SignalBase() = default;
};

class IAutoDeleteDelegate : public __IDeleteDelegate {
    SignalBase * mSignalBase;
    bool mDeleteFromSlotObj = true;

    void deleteDelegate() final {
        mDeleteFromSlotObj = false;
        mSignalBase->_deleteDelegate(this);
    }

protected:

    IAutoDeleteDelegate(sts::HasSlots * inSlotObj, SignalBase * inSign, intptr_t inId)
        : mSignalBase(inSign),
          mSlotObj(inSlotObj),
          mId(inId) {

        if (mSlotObj)
            mSlotObj->mDelegates.push_back(this);
    }

    virtual ~IAutoDeleteDelegate() {
        if (mSlotObj && mDeleteFromSlotObj) {
            for (auto it = mSlotObj->mDelegates.begin(); it != mSlotObj->mDelegates.end(); ++it) {
                if ((*it) == this) {
                    mSlotObj->mDelegates.erase(it);
                    break;
                }
            }
        }
    }

    sts::HasSlots * mSlotObj;
    intptr_t mId;
};

//-------------------------------------------------------------------------

template<class... Args>
class IDelegate : public IAutoDeleteDelegate {
public:

    IDelegate(sts::HasSlots * inSlotObj, SignalBase * inSign, intptr_t inId)
        : IAutoDeleteDelegate(inSlotObj, inSign, inId) { }

    virtual ~IDelegate() = default;
    virtual void invoke(Args ...) = 0;

    bool compare(sts::HasSlots * inSlotObj, intptr_t inId) const {
        return (mSlotObj == inSlotObj && mId == inId);
    }

    bool compare(sts::HasSlots * inSlotObj) const {
        return (mSlotObj == inSlotObj);
    }

};

//-------------------------------------------------------------------------

template<class... Args>
class DelegateStatic : public IDelegate<Args ...> {
    typedef void (*Function)(Args ...);

    // reinterpretation function pointer
    union Convert {
        Convert(Function inM)
            : m(inM) {}

        intptr_t id;
        Function m;
    };

public:

    DelegateStatic(Function inFunc)
        : IDelegate<Args ...>(nullptr, nullptr, Convert(inFunc).id) {
        mFunc = inFunc;
    }

    void invoke(Args ... inArgs) override {
        mFunc(inArgs ...);
    }

private:

    Function mFunc;

};

//-------------------------------------------------------------------------

template<class TObject, class... Args>
class DelegateMethod : public IDelegate<Args ...> {
    typedef void (TObject::*Method)(Args ...);

    // reinterpretation function pointer
    union Convert {
        Convert(Method inM)
            : m(inM) {}

        intptr_t id;
        Method m;
    };

public:

    DelegateMethod(TObject * inObject, Method inMethod, SignalBase * inSignal)
        : IDelegate<Args ...>(inObject, inSignal, Convert(inMethod).id) {
        mMethod = inMethod;
    }

    void invoke(Args ... inArgs) override {
        (static_cast<TObject*>(IAutoDeleteDelegate::mSlotObj)->*mMethod)(inArgs ...);
    }

private:

    Method mMethod;

};
}

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

namespace sts {

/********************************************************************************************************/
//////////////////////////////////////////////* Templates *///////////////////////////////////////////////
/********************************************************************************************************/

/*!
 * \details Implementation of the simple signal/slot mechanism.
 * \pre Class which is having slots must be inherited from the sts::HasSlots for auto-disconnecting when it is deleting.
 * \remark Example:
 * \code
 * 
 * // declaration a signal.
 * typedef sts::Signal<bool, int> TestSignal;
 * TestSignal mTestSignal;
 * // Or
 * sts::Signal<bool, int> mTestEvent;
 * 
 * // Connection slots to the signal.
 * sender.mTestSignal.connect(&SomeClass::staticMethod); // static function
 * sender.mTestSignal.connect(&SomeClassObject, &SomeClass::method); // class function (method)
 * 
 * // invoke signal.
 * mTestSignal(true, 5);
 * 
 * // Disconnection slots from the signal
 * sender.mTestSignal.disconnect(&SomeClass::staticMethod); // static function
 * sender.mTestSignal.disconnect(&SomeClassObject, &SomeClass::method); // class function (method)
 * // or
 * sender.mTestSignal.disconnect(&SomeClassObject); // all class functions (methods)
 * 
 * \endcode
 */
template<class ... Args>
class Signal : public sts_internal::SignalBase {
public:

    typedef std::list<sts_internal::IDelegate<Args ...>*> DelegateList;

    //---------------------------------------------------------------
    // @{

    Signal() = default;
    ~Signal() { disconnectAll(); }

    // @}
    //---------------------------------------------------------------
    // @{

    /*!
     * \details Check whether the Signal have some connections.
     * \return True if the signal has some connection otherwise false.
     */
    bool isEmpty() const {
        return mDelegateList.empty();
    }

    // @}
    //---------------------------------------------------------------
    // @{

    /*!
     * \details Connects a static functions.
     * \code
     * sender.mTestSignal.connect(&AppReceiver::testStaticDelegate);
     * \endcode
     * \param [in] pFunc the pointer to a static function.
     */
    void connect(void (*pFunc)(Args ...)) {
        NoTesting(assert(!_contains(nullptr, *reinterpret_cast<intptr_t*>(&pFunc))));
        mDelegateList.push_back(new sts_internal::DelegateStatic<Args ...>(pFunc));
    }

    /*!
     * \details Connects an object's method.
     * \code
     * sender.mTestSignal.connect(&receiver, &AppReceiver::testMethodDelegate);
     * \endcode
     * \param [in] obj the pointer to an object.
     * \param [in] pMethod the pointer to the object's method.
     */
    template<class ObjType>
    void connect(ObjType * obj, void (ObjType::*pMethod)(Args ...)) {
        NoTesting(assert(!_contains(static_cast<sts::HasSlots*>(obj), *reinterpret_cast<intptr_t*>(&pMethod))));
        mDelegateList.push_back(new sts_internal::DelegateMethod<ObjType, Args ...>(obj, pMethod, this));
    }

    /*!
     * \details Disconnects the static functions.
     * \code
     * sender.mTestSignal.disconnect(&AppReceiver::testStaticDelegate);
     * \endcode
     * \param [in] inFunc the pointer to the static function which must be disconnected.
     */
    void disconnect(void (*inFunc)(Args ...)) {
        typedef void (*Function)(Args ...);
        // reinterpretation function pointer
        union Convert {
            explicit Convert(Function inM)
                : m(inM) { }

            intptr_t id;
            Function m;
        };

        NoTesting(assert(_contains(nullptr, Convert(inFunc).id)));
        _deleteDelegate(nullptr, Convert(inFunc).id);
    }

    /*!
     * \details Disconnects the object's method.
     * \code
     * sender.mTestSignal.disconnect(&receiver, &AppReceiver::testMethodDelegate);
     * \endcode
     * \param [in] obj the pointer to the object.
     * \param [in] inMethod the pointer to the object's method which must be disconnected.
     */
    template<class ObjType>
    void disconnect(ObjType * obj, void (ObjType::*inMethod)(Args ...)) {
        typedef void (ObjType::*Method)(Args ...);
        // reinterpretation function pointer
        union Convert {
            explicit Convert(Method inM)
                : m(inM) { }

            intptr_t id;
            Method m;
        };

        NoTesting(assert(_contains(static_cast<sts::HasSlots*>(obj), Convert(inMethod).id)));
        _deleteDelegate(static_cast<HasSlots*>(obj), Convert(inMethod).id);
    }

    /*!
     * \details Disconnects all methods of the specified object.
     * \code
     * sender.mTestSignal.disconnect(&receiver);
     * \endcode
     * \param [in] obj the pointer to the object whose methods must be disconnected.
     */
    template<class ObjType>
    void disconnect(ObjType * obj) {
        NoTesting(assert(_contains(static_cast<sts::HasSlots*>(obj))));
        _deleteDelegate(static_cast<HasSlots*>(obj));
    }

    /*!
     * \details Disconnects all slots (functions and methods)
     */
    void disconnectAll() {
        for (auto & d : mDelegateList)
            delete d;
        mDelegateList.clear();
    }

    // @}
    //---------------------------------------------------------------
    // @{

    /*!
     * \details Operator invoke
     */
    void operator()(Args ... inArgs) {
        if (!isEmpty()) {
            for (auto & d : mDelegateList)
                d->invoke(inArgs ...);
        }
    }

    // @}
    //---------------------------------------------------------------

private:

    void _deleteDelegate(HasSlots * inSlotObj, intptr_t inId) {
        for (auto it = mDelegateList.begin(); it != mDelegateList.end(); ++it) {
            if ((*it)->compare(inSlotObj, inId)) {
                delete (*it);
                mDelegateList.erase(it);
                break;
            }
        }
    }

    void _deleteDelegate(HasSlots * inSlotObj) {
        for (auto it = mDelegateList.begin(); it != mDelegateList.end();) {
            if ((*it)->compare(inSlotObj)) {
                delete (*it);
                it = mDelegateList.erase(it);
                if (it == mDelegateList.end())
                    break;
            }
            else {
                ++it;
            }
        }
    }

    void _deleteDelegate(sts_internal::IAutoDeleteDelegate * inDelegate) final {
        for (auto it = mDelegateList.begin(); it != mDelegateList.end(); ++it) {
            if ((*it) == inDelegate) {
                delete (*it);
                mDelegateList.erase(it);
                return;
            }
        }
        assert(0);
    }

    bool _contains(HasSlots * inSlotObj, intptr_t inId) {
        for (auto it = mDelegateList.begin(); it != mDelegateList.end(); ++it) {
            if ((*it)->compare(inSlotObj, inId))
                return true;
        }
        return false;
    }

    bool _contains(HasSlots * inSlotObj) {
        for (auto it = mDelegateList.begin(); it != mDelegateList.end(); ++it) {
            if ((*it)->compare(inSlotObj))
                return true;
        }
        return false;
    }

private:

    Testing(public:)

    size_t count() const {
        return mDelegateList.size();
    }

    DelegateList mDelegateList;

};

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

}

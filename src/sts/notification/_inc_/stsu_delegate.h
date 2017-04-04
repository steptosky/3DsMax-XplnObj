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

#pragma once

#include <assert.h>
#include <cstdint>
#include <string>
#include <list>

#ifdef STSU_TESTING
#define Testing(X) X
#define NoTesting(X)
#else
#define Testing(X)
#define NoTesting(X) X
#endif

/* This header is part of the Signal system */

namespace sts {

	class HasSlots;

}

namespace sts_internal {

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	class IAutoDeleteDelegate;

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	class __IDeleteDelegate {
		friend sts::HasSlots;
		virtual void deleteDelegate() = 0;
	public:
		virtual ~__IDeleteDelegate() = default;
	};

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

}

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

namespace sts_internal {

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	class SignalBase {
		friend IAutoDeleteDelegate;
		virtual void _deleteDelegate(IAutoDeleteDelegate * inDelegate) = 0;
	};

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	class IAutoDeleteDelegate : public __IDeleteDelegate {

		SignalBase * mSignalBase;
		bool mDeleteFromSlotObj = true;

		virtual void deleteDelegate() final {
			mDeleteFromSlotObj = false;
			mSignalBase->_deleteDelegate(this);
		}

	protected:

		IAutoDeleteDelegate(sts::HasSlots * inSlotObj, SignalBase * inSign, intptr_t inId)
			:
			mSignalBase(inSign),
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

	/**************************************************************************************************/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**************************************************************************************************/

	template<class... Args>
	class IDelegate : public IAutoDeleteDelegate {
	public:

		IDelegate(sts::HasSlots * inSlotObj, SignalBase * inSign, intptr_t inId)
			:
			IAutoDeleteDelegate(inSlotObj, inSign, inId) { }

		virtual ~IDelegate() = default;

		virtual void invoke(Args ...) = 0;

		inline bool compare(sts::HasSlots * inSlotObj, intptr_t inId) const {
			return (mSlotObj == inSlotObj && mId == inId);
		}

		inline bool compare(sts::HasSlots * inSlotObj) const {
			return (mSlotObj == inSlotObj);
		}

	};

	/********************************************************************************************************/
	//////////////////////////////////////////////* Templates *///////////////////////////////////////////////
	/********************************************************************************************************/

	template<class... Args>
	class DelegateStatic : public IDelegate<Args ...> {

		typedef void (*Function)(Args ...);

		// reinterpretation function pointer
		union Convert {
			Convert(Function inM)
				:
				m(inM) { }

			intptr_t id;
			Function m;
		};

	public:

		DelegateStatic(Function inFunc)
			:
			IDelegate<Args ...>(nullptr, nullptr, Convert(inFunc).id) {

			mFunc = inFunc;
		}

		virtual void invoke(Args ... inArgs) override {
			mFunc(inArgs ...);
		}

	private:

		Function mFunc;

	};

	/********************************************************************************************************/
	//////////////////////////////////////////////* Templates *///////////////////////////////////////////////
	/********************************************************************************************************/

	template<class TObject, class... Args>
	class DelegateMethod : public IDelegate<Args ...> {

		typedef void (TObject::*Method)(Args ...);

		// reinterpretation function pointer
		union Convert {
			Convert(Method inM)
				:
				m(inM) { }

			intptr_t id;
			Method m;
		};

	public:

		DelegateMethod(TObject * inObject, Method inMethod, SignalBase * inSignal)
			:
			IDelegate<Args ...>(inObject, inSignal, Convert(inMethod).id) {

			mMethod = inMethod;
		}

		virtual void invoke(Args ... inArgs) override {
			(static_cast<TObject*>(IAutoDeleteDelegate::mSlotObj) ->* mMethod)(inArgs ...);
		}

	private:

		Method mMethod;

	};

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

}

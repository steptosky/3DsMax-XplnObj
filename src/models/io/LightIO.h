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

/*
* \copyright STEPTOSKY 2009
* http://www.steptosky.com
*
*/

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include "common/Logger.h"
#include "sts/io/DataStream.h"
#include <xpln/obj/ObjLightCustom.h>
#include <xpln/obj/ObjLightNamed.h>
#include <xpln/obj/ObjLightParam.h>
#include <xpln/obj/ObjLightPoint.h>
#include <xpln/obj/ObjLightSpillCust.h>

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class LightIO {

	LightIO() = default;
	~LightIO() = default;

public:

	//-------------------------------------------------------------------------

	class ILightIO {
	public:
		virtual ~ILightIO() = default;
		virtual void gotLight(const xobj::ObjLightCustom & inLight) = 0;
		virtual void gotLight(const xobj::ObjLightNamed & inLight) = 0;
		virtual void gotLight(const xobj::ObjLightParam & inLight) = 0;
		virtual void gotLight(const xobj::ObjLightPoint & inLight) = 0;
		virtual void gotLight(const xobj::ObjLightSpillCust & inLight) = 0;
		virtual void gotNoLight() = 0;
	};

	//-------------------------------------------------------------------------

	static bool canApply(INode * node);
	static bool cloneData(INode * from, INode * to);

	//-------------------------------------------------------------------------

	static bool loadFromNode(INode * inNode, ILightIO * inCallback);
	static void removeFromNode(INode * node);
	static void saveToNode(INode * outNode, const xobj::ObjAbstract * inLight);

	//-------------------------------------------------------------------------

private:

	template<typename T>
	static void saveToNodeInternal(INode * outNode, const T & inLight) {
		if (!outNode) {
			LError << "INode is nullptr";
			return;
		}

		std::stringstream buf;
		sts::DataStream stream(&buf);
		stream.writeFormatVersion();
		stream.setValue<uint8_t>(uint8_t(1)); // node io version
		xobj::eObjectType type = inLight.objType();
		stream.setValue<int32_t>(type);
		save(stream, inLight);
		saveDataToNode(outNode, buf);
	}

	static void save(sts::DataStreamO & stream, const xobj::ObjLightCustom & inLight);
	static void save(sts::DataStreamO & stream, const xobj::ObjLightNamed & inLight);
	static void save(sts::DataStreamO & stream, const xobj::ObjLightParam & inLight);
	static void save(sts::DataStreamO & stream, const xobj::ObjLightPoint & inManip);
	static void save(sts::DataStreamO & stream, const xobj::ObjLightSpillCust & inLight);

	static bool load(INode * node, sts::DataStreamI & stream, xobj::ObjLightCustom & outLight);
	static bool load(INode * node, sts::DataStreamI & stream, xobj::ObjLightNamed & outLight);
	static bool load(INode * node, sts::DataStreamI & stream, xobj::ObjLightParam & outLight);
	static bool load(INode * node, sts::DataStreamI & stream, xobj::ObjLightPoint & outLight);
	static bool load(INode * node, sts::DataStreamI & stream, xobj::ObjLightSpillCust & outLight);

	//-------------------------------------------------------------------------

	static void saveDataToNode(INode * outNode, const std::stringstream & indata);

	//-------------------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

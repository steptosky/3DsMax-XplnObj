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

#pragma warning(push, 0)
#include <max.h>
#include <maxheapdirect.h>
#pragma warning(pop)

#include "gup/ObjCommon.h"

#ifndef TOTEXT
#define TOTEXT(x) #x
#endif // !TOTEXT

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class NodeIO {
public:

	//---------------------------------------------------------------

	static void cloneData(INode * NodeFrom, INode * NodeTo, DWORD index) {
		if (NodeFrom) {
			AppDataChunk * chunkFrom = dataChunk(NodeFrom, index);
			if (chunkFrom) {
				saveData(NodeTo, index, chunkFrom->data, chunkFrom->length);
			}
		}
	}

	static void saveData(INode * outNode, DWORD index, const void * data, DWORD length) {
		if (outNode) {
			AppDataChunk * chunk = outNode->GetAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, index);
			if (chunk) {
				if (chunk->length != length) {
					chunk->length = length;
					chunk->data = MAX_realloc(chunk->data, chunk->length);
				}
				memcpy(chunk->data, data, chunk->length);
			}
			else {
				void * nodeBuf = MAX_malloc(length);
				memcpy(nodeBuf, data, length);
				outNode->AddAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, index, length, nodeBuf);
			}
		}
	}

	static void saveData(INode * outNode, DWORD index, const std::string & inBytes) {
		saveData(outNode, index, reinterpret_cast<const void*>(inBytes.data()), static_cast<DWORD>(inBytes.size()));
	}

	//---------------------------------------------------------------

	static AppDataChunk * dataChunk(INode * node, DWORD index) {
		if (node) {
			return node->GetAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, index);
		}
		return nullptr;
	}

	//---------------------------------------------------------------

	static void removeData(INode * node, DWORD index) {
		if (node) {
			node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, index);
		}
	}

	//---------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

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

#include "CloneNodeChunk.h"
#include "common/Logger.h"
#include "models/MdObjAttr.h"
#include "models/MdManip.h"
#include "models/MdLight.h"
#include "models/MdAnimVis.h"
#include "models/MdAnimTrans.h"
#include "models/MdAnimRot.h"
#include "common/String.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

CloneNodeChunk::CloneNodeChunk() {
    RegisterNotification(slotPostNodesClone, this, NOTIFY_POST_NODES_CLONED);
}

CloneNodeChunk::~CloneNodeChunk() {
    UnRegisterNotification(slotPostNodesClone, this, NOTIFY_POST_NODES_CLONED);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void CloneNodeChunk::slotPostNodesClone(void * /* param */, NotifyInfo * info) {
    struct Data {
        INodeTab * origNodes;
        INodeTab * clonedNodes;
        CloneType cloneType;
    };

    Data * data = reinterpret_cast<Data*>(info->callParam);
    if (data->origNodes->Count() != data->clonedNodes->Count()) {
        LError << "Internal error while the nodes cloning: mismatch count.";
        return;
    }

#if MAX_VERSION_MAJOR > 18 // 2016 = 18 i.e since 2017
    // The problem is the origNodes order and 
    // clonedNodes order are mismatch since 2017 (19) 3dmax version
    //
    // It is not a good solution 
    // but there is no another way to clone AppDataChunk
    // (Application-specific data) for now (19.04.2017).
    data->origNodes->Sort(&CloneNodeChunk::sortFunction);
#endif

    int count = data->origNodes->Count();
    for (int i = 0; i < count; ++i) {
        cloneData((*data->origNodes)[i], (*data->clonedNodes)[i]);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int CloneNodeChunk::sortFunction(const void * item1, const void * item2) {
    INode * i1 = *(INode **)item1;
    INode * i2 = *(INode **)item2;
    return _tcscmp(i1->GetName(), i2->GetName());
}

void CloneNodeChunk::cloneData(INode * origin, INode * clone) {
    MdObjAttr::cloneData(origin, clone);
    MdManip::cloneData(origin, clone);
    MdLight::cloneData(origin, clone);
    MdAnimVis::cloneData(origin, clone);
    MdAnimTrans::cloneData(origin, clone);
    MdAnimRot::cloneData(origin, clone);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

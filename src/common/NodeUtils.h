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

#include <functional>

class INode;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Helper
 */
class NodeUtils {
public:

    //-------------------------------------------------------------------------

    /*!
     * \return false if you want to stop the process otherwise true.
     */
    typedef std::function<bool(INode *)> Function;

    /*!
     * \details Visit all the INode in the scene according to their hierarchy
     *          and call specified function for each one.
     * \param fn function which will receive each INode. 
     * \return false if process was stopped by the function 
     *         otherwise true which means that all the nodes were processed.
     */
    static bool visitAll(const Function & fn) {
        return visitAllOf(GetCOREInterface()->GetRootNode(), fn);
    }

    /*!
     * \details Visit all the INode of the given root one according to their hierarchy
     *          and call specified function for each one.
     * \param root
     * \param fn function which will receive each INode. 
     * \return false if process was stopped by the function 
     *         otherwise true which means that all the nodes were processed.
     */
    static bool visitAllOf(INode * root, const Function & fn);

    /*!
     * \details Visit all the children INode of the given root one according to their order
     *          and call specified function for each one.
     * \param root
     * \param fn function which will receive each INode. 
     * \return false if process was stopped by the function 
     *         otherwise true which means that all the nodes were processed.
     */
    static bool visitChildrenOf(INode * root, const Function & fn);

    //-------------------------------------------------------------------------

    /*!
     * \details Check whether the current scene contains at least one x-plane main object.
     *          It checks only children of the root node and 
     *          does not check children of children.
     * \return True if contains otherwise false.
     */
    static bool isSceneContainMainObj();

    //-------------------------------------------------------------------------
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

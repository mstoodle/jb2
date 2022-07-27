/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/
        
#ifndef VIRTUALMACHINESTATE_INCL
#define VIRTUALMACHINESTATE_INCL

#include "stdint.h"
#include "stddef.h"
#include <map>
#include "CreateLoc.hpp"

namespace OMR {
namespace JitBuilder {

class Builder;

namespace VM {

class BytecodeBuilder;
class VMExtension;

typedef uint64_t VirtualMachineStateID;
const VirtualMachineStateID NoVirtualMachineStateID=0;

typedef uint64_t StateKind;
const StateKind NoStateKind=0;
const StateKind AnyStateKind=1;

class VirtualMachineState {
public:
    static StateKind STATEKIND;
    VirtualMachineState(LOCATION, VMExtension *vme, StateKind kind)
        : _id(nextVirtualMachineStateID++)
        , _createLocation(PASSLOC)
        , _vme(vme)
        , _kind(kind) {

    }

    VirtualMachineStateID id() const { return _id; }
    CreateLocation createLocation() const { return _createLocation; }
    VMExtension *vme() const { return _vme; }

    virtual void Commit(LOCATION, Builder * b) { }
    virtual VirtualMachineState * MakeCopy(LOCATION, Builder *b);
    virtual void MergeInto(LOCATION, VirtualMachineState * vmState, Builder * b) { }
    virtual void Reload(LOCATION, Builder * b) { }

    template<typename T> bool isKind() const {
        return (_kind & T::STATEKIND) != 0;
    }
    template<typename T> const T *refine() const {
        assert(isKind<T>());
        return static_cast<const T *>(this);
    }
    template<typename T> T *refine() {
        assert(isKind<T>());
        return static_cast<T *>(this);
    }

    static StateKind assignStateKind(StateKind baseKind, std::string name);

protected:
    VirtualMachineStateID _id;
    CreateLocation _createLocation;
    VMExtension *_vme;
    StateKind _kind;

    static VirtualMachineStateID nextVirtualMachineStateID;
    static StateKind nextStateKind;
    static std::map<std::string,StateKind> stateKindFromNameMap;
    static std::map<StateKind,std::string> stateNameFromKindMap;
};

} // namespace VM
} // namespace JitBuilder
} // namespace OMR

#endif // !defined(VIRTUALMACHINESTATE_INCL)

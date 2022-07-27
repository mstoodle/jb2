/*******************************************************************************
 * Copyright (c) 2016, 2018 IBM Corp. and others
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

#include "vm/VirtualMachineState.hpp"
#include "Builder.hpp"

namespace OMR {
namespace JitBuilder {
namespace VM {

VirtualMachineStateID VirtualMachineState::nextVirtualMachineStateID = NoVirtualMachineStateID+1;
StateKind VirtualMachineState::STATEKIND = NoStateKind;
std::map<std::string,StateKind> VirtualMachineState::stateKindFromNameMap;
std::map<StateKind,std::string> VirtualMachineState::stateNameFromKindMap;

VirtualMachineState *
VirtualMachineState::MakeCopy(LOCATION, Builder *b) {
    return new VirtualMachineState(PASSLOC, _vme, STATEKIND);
}

static StateKind
getNextStateKind(StateKind k) {
    if (k == NoStateKind) // 0 cannot be shifted
        return AnyStateKind;
    StateKind nextKind = k << 1;
    return nextKind;
}

StateKind VirtualMachineState::nextStateKind=getNextStateKind(AnyStateKind);

StateKind
VirtualMachineState::assignStateKind(StateKind baseKind, std::string name) {
    auto found = stateKindFromNameMap.find(name);
    if (found != stateKindFromNameMap.end())
        return found->second;
            
    StateKind kind = nextStateKind;
    assert(kind != 0); // will eventually need a bit vector
    nextStateKind = getNextStateKind(nextStateKind);

    assert((baseKind & kind) == 0);
    StateKind fullKind = baseKind | kind;
    stateKindFromNameMap.insert({name, fullKind});
    stateNameFromKindMap.insert({fullKind, name});
    return fullKind;
}

} // namespace VM
} // namespace JitBuilder
} // namespace OMR

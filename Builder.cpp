/*******************************************************************************
 * Copyright (c) 2021, 2022 IBM Corp. and others
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

#include <string>
#include <vector>
#include "Builder.hpp"
#include "Compilation.hpp"
#include "Context.hpp"
#include "Location.hpp"
#include "Operation.hpp"
#include "Value.hpp"

namespace OMR {
namespace JitBuilder {

Builder::Builder(Compilation * comp, Context *context, std::string name)
    : _id(comp->getBuilderID())
    , _comp(comp)
    , _name(name)
    , _parent(NULL)
    , _context(context)
    , _successor(NULL)
    , _currentLocation(new Location(comp, "", "", 0))
    , _boundToOperation(NULL)
    , _isTarget(false)
    , _isBound(false)
    , _controlReachesEnd(true) {
}

Builder::Builder(Builder *parent, Context *context, std::string name)
    : _id(parent->_comp->getBuilderID())
    , _comp(parent->_comp)
    , _name(name)
    , _parent(parent)
    , _context(context)
    , _successor(NULL)
    , _currentLocation(parent->location())
    , _boundToOperation(NULL)
    , _isTarget(false)
    , _isBound(false)
    , _controlReachesEnd(true) {
    parent->addChild(this);
}

Builder::Builder(Builder *parent, Operation *boundToOp, std::string name)
    : _id(parent->_comp->getBuilderID())
    , _comp(parent->_comp)
    , _name(name)
    , _parent(parent)
    , _context(parent->context())
    , _successor(NULL)
    , _currentLocation(parent->location())
    , _boundToOperation(boundToOp)
    , _isTarget(false)
    , _isBound(true)
    , _controlReachesEnd(true) {
    parent->addChild(this);
}

Builder::~Builder() {
    for (auto it = OperationsBegin(); it != OperationsEnd(); it++) {
        Operation *op = *it;
        delete op;
    }
}

Builder *
Builder::create(Builder *parent, Context *context, std::string name) {
    return new Builder(parent, context, name);
}

Builder *
Builder::create(Compilation *comp, Context *context, std::string name) {
    return new Builder(comp, context, name);
}

void
Builder::addChild(Builder *child) {
    _children.push_back(child);
}

Builder *
Builder::add(Operation *op) {
    _operations.push_back(op);
    return this;
}

} // namespace JitBuilder
} // namespace OMR

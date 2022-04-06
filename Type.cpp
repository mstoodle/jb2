/*******************************************************************************
 * Copyright (c) 2021, 2021 IBM Corp. and others
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

#include "Compilation.hpp"
#include "Compiler.hpp"
#include "Extension.hpp"
#include "TextWriter.hpp"
#include "Type.hpp"
#include "TypeDictionary.hpp"

namespace OMR {
namespace JitBuilder {

Type::Type(LOCATION, Extension *ext, std::string name, size_t size, const Type *layout)
    : _ext(ext)
    , _createLoc(PASSLOC)
    , _dict(ext->compiler()->dict())
    , _id(ext->compiler()->dict()->getTypeID())
    , _name(name)
    , _size(size)
    , _layout(layout) {

}

Type::Type(LOCATION, TypeDictionary *dict, std::string name, size_t size, const Type *layout)
    : _ext(NULL)
    , _createLoc(PASSLOC)
    , _dict(dict)
    , _id(dict->getTypeID())
    , _name(name)
    , _size(size)
    , _layout(layout) {

}


Literal *
Type::literal(LOCATION, Compilation *comp, const LiteralBytes *value) const {
    return comp->registerLiteral(PASSLOC, this, value);
}

std::string
Type::base_string() const {
    std::string s;
    s.append("type t").append(std::to_string(this->id())).append(" ");
    s.append(std::to_string(this->size())).append(" ");
    s.append(this->name()).append(" ");
    return s;
}

void
Type::writeType(TextWriter &w) const {
    w.indent() << "[ " << this->to_string() ;
    w << " ]" << w.endl();
}

std::string
Type::to_string() const {
    std::string s;
    s.append(base_string()).append("primitiveType");
    if (_layout)
        s.append(" layout t").append(std::to_string(_layout->id())).append(" ").append(_layout->name());
    return s;
}

} // namespace JitBuilder
} // namespace OMR

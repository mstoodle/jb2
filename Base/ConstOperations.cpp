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

#include <stdint.h>
#include "BaseExtension.hpp"
#include "BaseSymbols.hpp"
#include "BaseTypes.hpp"
#include "Builder.hpp"
#include "ConstOperations.hpp"
#include "Function.hpp"
#include "Literal.hpp"
#include "JB1MethodBuilder.hpp"
#include "Location.hpp"
#include "Operation.hpp"
#include "OperationCloner.hpp"
#include "OperationReplacer.hpp"
#include "TextWriter.hpp"
#include "Value.hpp"

namespace OMR {
namespace JitBuilder {
namespace Base {

//
// ConstInt8
//

const std::string Op_ConstInt8::opName="ConstInt8";

Op_ConstInt8::Op_ConstInt8(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt8, Value * result, Literal *lv)
    : OperationR1L1(PASSLOC, aConstInt8, ext, parent, result, lv) {
}

#if 0
void
Op_ConstInt8::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstInt8(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstInt8::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstInt8(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstInt8::write(TextWriter &w) const {
    w << result() << " = " << name() << " " << literal() << w.endl();
}

void
Op_ConstInt8::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstInt8(location(), parent(), result(), literal()->value<const int8_t>());
}

//
// ConstInt16
//

const std::string Op_ConstInt16::opName="ConstInt16";

Op_ConstInt16::Op_ConstInt16(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt16, Value * result, Literal *lv)
    : OperationR1L1(PASSLOC, aConstInt16, ext, parent, result, lv) {
}

#if 0
void
Op_ConstInt16::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstInt16(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstInt16::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstInt16(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstInt16::write(TextWriter &w) const {
    w << result() << " = " << name() << " " << literal() << w.endl();
}

void
Op_ConstInt16::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstInt16(location(), parent(), result(), literal()->value<const int16_t>());
}


//
// ConstInt32
//
const std::string Op_ConstInt32::opName="ConstInt32";

Op_ConstInt32::Op_ConstInt32(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt32, Value * result, Literal *lv)
    : OperationR1L1(PASSLOC, aConstInt32, ext, parent, result, lv) {
}

#if 0
void
Op_ConstInt32::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstInt32(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstInt32::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstInt32(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstInt32::write(TextWriter &w) const {
    w << result() << " = " << name() << " " << literal() << w.endl();
}

void
Op_ConstInt32::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstInt32(location(), parent(), result(), literal()->value<const int32_t>());
}


//
// ConstInt64
//
const std::string Op_ConstInt64::opName="ConstInt64";

Op_ConstInt64::Op_ConstInt64(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt64, Value * result, Literal *lv)
    : OperationR1L1(PASSLOC, aConstInt64, ext, parent, result, lv) {
}

#if 0
void
Op_ConstInt64::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstInt64(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstInt64::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstInt64(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstInt64::write(TextWriter &w) const {
    w << result() << " = " << name() << " " << literal() << w.endl();
}

void
Op_ConstInt64::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstInt64(location(), parent(), result(), literal()->value<const int64_t>());
}


//
// ConstFloat32
//
const std::string Op_ConstFloat32::opName="ConstFloat32";

Op_ConstFloat32::Op_ConstFloat32(LOCATION, Extension *ext, Builder * parent, ActionID aConstFloat32, Value * result, Literal *lv)
    : OperationR1L1(PASSLOC, aConstFloat32, ext, parent, result, lv) {
}

#if 0
void
Op_ConstFloat32::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstFloat32(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstFloat32::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstFloat32(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstFloat32::write(TextWriter &w) const {
    w << result() << " = " << name() <<  " " << literal() << w.endl();
}

void
Op_ConstFloat32::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstFloat(location(), parent(), result(), literal()->value<const float>());
}


//
// ConstFloat64
//
const std::string Op_ConstFloat64::opName="ConstFloat64";

Op_ConstFloat64::Op_ConstFloat64(LOCATION, Extension *ext, Builder * parent, ActionID aConstFloat64, Value * result, Literal *lv)
   : OperationR1L1(PASSLOC, aConstFloat64, ext, parent, result, lv) {
}

#if 0
void
Op_ConstFloat64::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstFloat64(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstFloat64::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstFloat64(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstFloat64::write(TextWriter &w) const {
    w << result() << " = " << name() <<  " " << literal() << w.endl();
}

void
Op_ConstFloat64::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstDouble(location(), parent(), result(), literal()->value<const double>());
}


//
// ConstAddress
//
const std::string Op_ConstAddress::opName="ConstAddress";

Op_ConstAddress::Op_ConstAddress(LOCATION, Extension *ext, Builder * parent, ActionID aConstAddress, Value * result, Literal *lv)
    : OperationR1L1(PASSLOC, aConstAddress, ext, parent, result, lv) {
}

#if 0
void
Op_ConstAddress::cloneTo(LOCATION, Builder *b, ValueMapper **resultMappers, ValueMapper **operandMappers, TypeMapper **typeMappers, LiteralMapper **literalMappers, SymbolMapper **symbolMappers, BuilderMapper **builderMappers) const {
    Operation *op = new Op_ConstAddress(PASSLOC, this->_ext, b, this->action(), resultMappers[0]->next(), literalMappers[0]->next());
    Operation::addToBuilder(this->_ext, b, op);
}
#endif

Operation *
Op_ConstAddress::clone(LOCATION, Builder *b, OperationCloner *cloner) const {
    return new Op_ConstAddress(PASSLOC, this->_ext, b, this->action(), cloner->result(), cloner->literal());
}

void
Op_ConstAddress::write(TextWriter &w) const {
    w << result() << " = " << name() << " " << literal() << w.endl();
}

void
Op_ConstAddress::jbgen(JB1MethodBuilder *j1mb) const {
    j1mb->ConstAddress(location(), parent(), result(), literal()->value<void * const>());
}


} // namespace Base
} // namespace JitBuilder
} // namespace OMR

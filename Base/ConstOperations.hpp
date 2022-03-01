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

#ifndef CONSTOPERATIONS_INCL
#define CONSTOPERATIONS_INCL

#include "Literal.hpp"
#include "Operation.hpp"

namespace OMR {
namespace JitBuilder {

class JB1MethodBuilder;
class TextWriter;

namespace Base {

//
// classes for Const operations that can be directly instantiated
//

class Op_ConstInt8 : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstInt8(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt8, Value *result, Literal *lv);
    static const std::string opName;
    };

class Op_ConstInt16 : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstInt16(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt16, Value * result, Literal *lv);
    static const std::string opName;
    };


class Op_ConstInt32 : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstInt32(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt32, Value * result, Literal *lv);
    static const std::string opName;
    };

class Op_ConstInt64 : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstInt64(LOCATION, Extension *ext, Builder * parent, ActionID aConstInt64, Value * result, Literal *lv);
    static const std::string opName;
    };

class Op_ConstFloat32 : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstFloat32(LOCATION, Extension *ext, Builder * parent, ActionID aConstFloat32, Value * result, Literal *lv);
    static const std::string opName;
    };

class Op_ConstFloat64 : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstFloat64(LOCATION, Extension *ext, Builder * parent, ActionID aConstFloat64, Value * result, Literal *lv);
    static const std::string opName;
    };

class Op_ConstAddress : public OperationR1L1
    {
    friend class BaseExtension;
    public:
    virtual Operation * clone(LOCATION, Builder *b, OperationCloner *cloner) const;
    virtual void write(TextWriter &w) const;
    virtual void jbgen(JB1MethodBuilder *j1mb) const;
    static const std::string & name() { return opName; }
    protected:
    Op_ConstAddress(LOCATION, Extension *ext, Builder * parent, ActionID aConstAddress, Value * result, Literal *lv);
    static const std::string opName;
    };

} // namespace Base
} // namespace JitBuilder
} // namespace OMR

#endif // !defined(CONSTOPERATIONS_INCL)

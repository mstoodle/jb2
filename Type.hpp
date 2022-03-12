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

#ifndef TYPE_INCL
#define TYPE_INCL

#include <stdint.h>
#include <stdarg.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cassert>

#include "typedefs.hpp"
#include "CreateLoc.hpp"
#include "IDs.hpp"
#include "Mapper.hpp"

namespace OMR {
namespace JitBuilder {

class Builder;
class Compilation;
class Compiler;
class Extension;
class JB1MethodBuilder;
class TextWriter;
class Type;
class TypeDictionary;

class Type {
public:
#if 0
    class Builder {
    public:
        Builder *extension(Extension *x) { _ext = x; return this; }
        Builder *dict(TypeDictionary *dict) { _dict = dict; }
        Builder *name(std::string myName) { _name = myName; }
        Builder *size(size_t mySize) { _size = mySize; })
        Builder *layout(Type *layout) { _layout = layout; }

        Type *create(LOCATION) {
            return new Type(PASSLOC, _ext, _name, _size, _layout);
        }
    protected:
        Extension * _ext;
        TypeDictionary * _dict;
        std::string _name;
        size_t _size;
        Type * _layout;
    };
#endif
    friend class Compiler;
    friend class TypeDictionary;
protected:
    typedef void (ValuePrinter)(TextWriter *w, const Type *t, void *p);

public:
    std::string name() const                 { return _name; }
    Extension *ext() const                   { return _ext; }
    TypeDictionary *owningDictionary() const { return _dict; }
    TypeID id() const                        { return _id; }
    size_t size() const                      { return _size; }

    bool operator!=(const Type & other) const {
        return _dict != other._dict || _id != other._id;
    }
    bool operator==(const Type & other) const {
        return _dict == other._dict && _id == other._id;
    }

    // need to remove from Type
    virtual bool isConcrete() const { return false; }
    virtual bool isPointer() const { return false; }
    virtual bool isStruct() const { return false; }
    virtual bool isUnion() const { return false; }
    virtual bool isField() const { return false; }
    virtual bool isFunction() const { return false; }
    virtual bool isDynamic() const { return false; }

    void writeType(TextWriter & w) const;
    virtual void writeSpecificType(TextWriter &w) const;
    virtual void printValue(TextWriter & w, const void *p) const { }
    virtual void printLiteral(TextWriter & w, const Literal *lv) const { }
    virtual bool literalsAreEqual(const LiteralBytes *lv1, const LiteralBytes *lv2) const { return false; }

    // creates a Literal of this Type from the raw LiteralBytes
    Literal * literal(LOCATION, Compilation *comp, const LiteralBytes *value) const;

    // returning NULL from the next function means that values of this Type cannot be broken down further
    virtual const Type *layout() const { return _layout; }

    // for Types with non-NULL layout, converts a literal of Type to the literals of the layout type in the LiteralMapper
    virtual LiteralMapper *explode(Literal *value, LiteralMapper *m=NULL) const { return NULL; }

    // register this Type's corresponding JB1 type(s) in the JB1MethodBuilder
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const { return true; }

protected:
    friend class Extension;

    static Type * create(LOCATION, Extension *ext, std::string name, size_t size, const Type * layout=NULL) {
        return new Type(PASSLOC, ext, name, size, layout);
    }
    static Type * create(LOCATION, TypeDictionary *dict, std::string name, size_t size, const Type * layout=NULL) {
        return new Type(PASSLOC, dict, name, size, layout);
    }

    Type(LOCATION, Extension *ext, std::string name, size_t size, const Type *layout=NULL);
    Type(LOCATION, TypeDictionary *dict, std::string name, size_t size, const Type *layout=NULL);

    Extension *_ext;
    CreateLocation _createLoc;
    TypeDictionary * _dict;
    TypeID _id;
    std::string _name;
    size_t _size;
    const Type * _layout;
};

} // namespace JitBuilder
} // namespace OMR

#endif // defined(TYPE_INCL)

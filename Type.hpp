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
#include "KindService.hpp"
#include "Mapper.hpp"

namespace OMR {
namespace JitBuilder {

class Builder;
class Compilation;
class Compiler;
class Extension;
class JB1MethodBuilder;
class Location;
class TextWriter;
class Type;
class TypeDictionary;
class TypeReplacer;

typedef KindService::Kind TypeKind;

class Type {
    friend class Compiler;
    friend class Extension;
    friend class TypeDictionary;

public:
    std::string name() const                 { return _name; }
    Extension *ext() const                   { return _ext; }
    TypeDictionary *owningDictionary() const { return _dict; }
    TypeID id() const                        { return _id; }
    virtual size_t size() const              { return _size; } // some Types cannot set size at construction

    bool operator!=(const Type & other) const {
        return _dict != other._dict || _id != other._id;
    }
    bool operator==(const Type & other) const {
        return _dict == other._dict && _id == other._id;
    }

    virtual TypeKind kind() const { return TYPEKIND; }
    template<typename T> bool isExactKind() const {
        return kindService.isExactMatch(_kind, T::TYPEKIND);
    }
    template<typename T> bool isKind() const {
        return kindService.isMatch(_kind, T::TYPEKIND);
    }
    template<typename T> const T *refine() const {
        assert(isKind<T>());
        return static_cast<const T *>(this);
    }

    std::string base_string(bool useHeader=false) const;
    virtual std::string to_string(bool useHeader=false) const;
    void writeType(TextWriter & w, bool useHeader=false) const;
    virtual void printValue(TextWriter & w, const void *p) const { }
    virtual void printLiteral(TextWriter & w, const Literal *lv) const { }
    virtual bool literalsAreEqual(const LiteralBytes *lv1, const LiteralBytes *lv2) const { return false; }

    virtual const int64_t getInteger(const Literal *lv) const { return 0; }
    virtual const double getFloatingPoint(const Literal *lv) const { return 0.0; }

    // creates a Literal of this Type from the raw LiteralBytes
    Literal * literal(LOCATION, Compilation *comp, const LiteralBytes *value) const;

    // for Types that can, return a zero or "one" literal (NULL means it doesn't exist for this Type)
    virtual Literal *zero(LOCATION, Compilation *comp) const { return NULL; }
    virtual Literal *identity(LOCATION, Compilation *comp) const { return NULL; }

    // returning NULL from the next function means that values of this Type cannot be broken down further
    virtual const Type *layout() const { return _layout; }

    // for Types with non-NULL layout, converts a literal of Type to the literals of the layout type in the LiteralMapper
    virtual LiteralMapper *explode(Literal *value, LiteralMapper *m=NULL) const { return NULL; }

    // register this Type's corresponding JB1 type(s) in the JB1MethodBuilder
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const { return true; }
    
    // create a JB1 Const operation for a Literal of this Type
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
        assert(0); // default must be to assert TODO convert to CompilationException
    }

    // if this Type should be mapped to another, use repl and create that Type if needed and return it (NULL if not mapped to another Type)
    //virtual const Type * mapIfNeeded(TypeReplacer *repl);

    // return true if this Type can be used as a layout
    virtual bool canBeLayout() const { return false; }

    // if this Type can be a layout, break its parts into the given TypeMapper using individual offsets starting at baseOffset
    virtual void explodeAsLayout(TypeReplacer *repl, size_t baseOffset, TypeMapper *m) const {
        assert(0); // default must be to assert TODO convert to CompilationException
    }

    static TypeKind TYPEKIND;

protected:
    Type(LOCATION, TypeKind kind, Extension *ext, std::string name, size_t size, const Type *layout=NULL);
    Type(LOCATION, TypeKind kind, Extension *ext, TypeDictionary *dict, std::string name, size_t size, const Type *layout=NULL);

    void transformTypeIfNeeded(TypeReplacer *repl, const Type *type) const;

    Extension *_ext;
    CreateLocation _createLoc;
    TypeDictionary * _dict;
    const TypeID _id;
    const TypeKind _kind;
    const std::string _name;
    const size_t _size;
    const Type * _layout;

    static KindService kindService;
};

} // namespace JitBuilder
} // namespace OMR

#endif // defined(TYPE_INCL)


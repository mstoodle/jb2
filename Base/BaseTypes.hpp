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

#ifndef BASETYPES_INCL
#define BASETYPES_INCL

#include <map>
#include "Type.hpp"

namespace OMR {
namespace JitBuilder {

class Extension;
class JB1MethodBuilder;
class Literal;
class TextWriter;

namespace Base {

class BaseExtension;

class NoTypeType : public Type {
    friend class BaseExtension;

    public:
    static NoTypeType * create(LOCATION, Extension *ext) { return new NoTypeType(PASSLOC, ext); }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    NoTypeType(LOCATION, Extension *ext)
        : Type(PASSLOC, ext, "NoType", 0)
        { }
};

class NumericType : public Type {
    friend class BaseExtension;

    protected:
    NumericType(LOCATION, Extension *ext, std::string name, size_t size)
        : Type(PASSLOC, ext, name, size)
        { }
};

class IntegerType : public NumericType {
    friend class BaseExtension;

    protected:
    IntegerType(LOCATION, Extension *ext, std::string name, size_t size)
        : NumericType(PASSLOC, ext, name, size)
        { }
};

class Int8Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 8; }
    Literal *literal(LOCATION, Compilation *comp, const int8_t value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    Int8Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int8", 8) { }
};

class Int16Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 16; }
    Literal *literal(LOCATION, Compilation *comp, const int16_t value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    Int16Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int16", 16) { }
};

class Int32Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 32; }
    Literal *literal(LOCATION, Compilation *comp, const int32_t value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    Int32Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int32", 32) { }
};

class Int64Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 64; }
    Literal *literal(LOCATION, Compilation *comp, const int32_t value) const;
    Literal *literal(LOCATION, Compilation *comp, const int64_t value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    Int64Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int64", 64) { }
};

class FloatingPointType : public NumericType {
    friend class BaseExtension;

    protected:
    FloatingPointType(LOCATION, Extension *ext, std::string name, size_t size)
        : NumericType(PASSLOC, ext, name, size)
        { }
};

class Float32Type : public FloatingPointType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 32; }
    Literal *literal(LOCATION, Compilation *comp, const float value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    Float32Type(LOCATION, Extension *ext) : FloatingPointType(PASSLOC, ext, "Float32", 32) { }
};

class Float64Type : public FloatingPointType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 64; }
    Literal *literal(LOCATION, Compilation *comp, const double value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    Float64Type(LOCATION, Extension *ext) : FloatingPointType(PASSLOC, ext, "Float64", 64) { }
};

class AddressType : public Type {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 64; } // should be platform specific
    Literal *literal(LOCATION, Compilation *comp, const void * value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool mapJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    AddressType(LOCATION, Extension *ext);
    AddressType(LOCATION, Extension *ext, std::string name);
};

class PointerType : public AddressType {
    friend class BaseExtension;
    friend class Type;
    friend class TypeDictionary;

    public:
    virtual bool isPointer() const { return true; }

    Type * BaseType() { return _baseType; }

    virtual void printType(TextWriter &w);
    virtual void printValue(TextWriter &w, const void *p) const;

    protected:
    static PointerType * create(LOCATION, Extension *ext, std::string name, Type * baseType) { return new PointerType(PASSLOC, ext, name, baseType); }
    PointerType(LOCATION, Extension *ext, std::string name, Type * baseType)
        : AddressType(PASSLOC, ext, name) // should really be 32 or 64 from ext->compiler()
        , _baseType(baseType)
        { }
    Type * _baseType;
};

class FieldType;
typedef std::map<std::string, FieldType *>::const_iterator FieldIterator;

class StructType : public Type {
    friend class BaseExtension;
    friend class TypeDictionary;

    public:
    virtual bool isStruct() const { return true; }

    Literal *literal(LOCATION, Compilation *comp, void * structValue);
    virtual bool isConcrete() const { return true; }

    bool closed() const           { return _closed; }
    void setClosed(bool c=true)   { _closed = c; }

    virtual FieldType * addField(LOCATION, Literal *name, Type *type, size_t offset);
    FieldIterator FieldsBegin() const { return _fieldsByName.cbegin(); }
    FieldIterator FieldsEnd() const   { return _fieldsByName.cend(); }
    FieldType *LookupField(std::string fieldName)
        {
        auto it = _fieldsByName.find(fieldName);
        if (it == _fieldsByName.end())
            return NULL;
        return it->second;
        }
    FieldIterator RemoveField(FieldIterator &it);

    virtual void printType(TextWriter &w);
    virtual void printValue(TextWriter &w, const void *p) const;

    protected:
    static StructType * create(LOCATION, Extension *ext, std::string name) {
        return new StructType(PASSLOC, ext, name, 0);
    }

    static StructType * create(LOCATION, Extension *ext, std::string name, size_t size) {
        return new StructType(PASSLOC, ext, name, size); // size in bits, used to be bytes!
    }

    StructType(LOCATION, Extension *ext, std::string name, size_t size)
        : Type(PASSLOC, ext, name, size)
        , _closed(false) {
        if (size > 0)
            _closed = true;
    }

    bool _closed;
    std::map<std::string, FieldType *> _fieldsByName;
    std::multimap<size_t, FieldType *> _fieldsByOffset;
};

class FieldType : public Type {
    friend class BaseExtension;

    public:
    static FieldType * create(LOCATION, Extension *ext, StructType *structType, Literal *fieldName, Type *type, size_t offset) {
        return new FieldType(PASSLOC, ext, structType, fieldName, type, offset);
    }

    StructType *owningStruct() const  { return _structType; }
    Literal *fieldName() const   { return _fieldName; }
    Type *type() const                { return _type; }
    size_t offset() const             { return _offset; }

    virtual bool isField() const      { return true; }

    virtual void printType(TextWriter &w);
    virtual void printValue(TextWriter &w, const void *p) const;

    protected:
    FieldType(LOCATION, Extension *ext, StructType *structType, Literal *fieldName, Type *type, size_t offset);

    StructType *_structType;
    Literal *_fieldName;
    Type *_type;
    size_t _offset;
};

#if 0
class UnionType : public StructType {
    friend class BaseExtension;
    friend class TypeDictionary;

    public:
    virtual bool isUnion() const { return true; }

    Literal *literal(LOCATION, Compilation *comp, void * unionValue);
    virtual bool isConcrete() const { return true; }

    virtual FieldType * addField(LOCATION, Literal *name, Type *type, size_t unused)
        {
        if (type->size() > _size)
            _size = type->size();
        return this->StructType::addField(PASSLOC, name, type, 0);
        }

    virtual void printType(TextWriter *w);
    virtual void printValue(TextWriter *w, const void *p) const;

    protected:
    static UnionType * create(LOCATION, Extension *ext, std::string name) {
        return new UnionType(PASSLOC, ext, name, 0);
    }
    static UnionType * create(LOCATION, Extension *ext, std::string name, size_t size) {
        return new UnionType(PASSLOC, ext, name, size);
    }

    UnionType(LOCATION, Extension *ext, std::string name, size_t size)
        : StructType(PASSLOC, ext, name, size)
        { }
};
#endif

class FunctionType : public Type {
    friend class BaseExtension;
    friend class TypeDictionary;

    public:
    static FunctionType * create(LOCATION, Extension *ext, std::string name, const Type *returnType, int32_t numParms, const Type ** parmTypes);

    ~FunctionType() { delete[] _parmTypes; }

    virtual bool isFunction() const { return true; }

    Literal *literal(LOCATION, Compilation *comp, void * functionValue);
    virtual bool isConcrete() const { return true; }

    const Type *returnType() const { return _returnType; }
    int32_t numParms() const { return _numParms; }
    const Type *parmType(int p) const { return _parmTypes[p]; }
    const Type **parmTypes() const { return _parmTypes; }

    virtual void printType(TextWriter &w);
    virtual void printValue(TextWriter &w, const void *p) const;

    protected:
    FunctionType(LOCATION, Extension *ext, std::string name, const Type *returnType, int32_t numParms, const Type ** parmTypes)
        : Type(PASSLOC, ext, name, 0)
        , _returnType(returnType)
        , _numParms(numParms)
        , _parmTypes(parmTypes)
        {
        }

    const Type *_returnType;
    int32_t _numParms;
    const Type **_parmTypes;
};

} // namespace Base
} // namespace JitBuilder
} // namespace OMR

#endif // !defined(BASETYPES_INCL)
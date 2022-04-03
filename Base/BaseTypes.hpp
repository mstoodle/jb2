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

#include <list>
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
class Function;
class FunctionCompilation;

class NoTypeType : public Type {
    friend class BaseExtension;

    public:
    static NoTypeType * create(LOCATION, Extension *ext) { return new NoTypeType(PASSLOC, ext); }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;

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

    virtual bool isInteger() const { return true; }

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
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 0); }
    Literal *identity(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 1); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;
    virtual const int64_t getInteger(const Literal *lv) const;

    protected:
    Int8Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int8", 8) { }
};

class Int16Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 16; }
    Literal *literal(LOCATION, Compilation *comp, const int16_t value) const;
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 0); }
    Literal *identity(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 1); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;
    virtual const int64_t getInteger(const Literal *lv) const;

    protected:
    Int16Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int16", 16) { }
};

class Int32Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 32; }
    Literal *literal(LOCATION, Compilation *comp, const int32_t value) const;
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 0); }
    Literal *identity(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 1); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;
    virtual const int64_t getInteger(const Literal *lv) const;

    protected:
    Int32Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int32", 32) { }
};

class Int64Type : public IntegerType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 64; }
    Literal *literal(LOCATION, Compilation *comp, const int64_t value) const;
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 0); }
    Literal *identity(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 1); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;
    virtual const int64_t getInteger(const Literal *lv) const;

    protected:
    Int64Type(LOCATION, Extension *ext) : IntegerType(PASSLOC, ext, "Int64", 64) { }
};

class FloatingPointType : public NumericType {
    friend class BaseExtension;

    virtual bool isFloatingPoint() const { return true; }

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
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 0.0); }
    Literal *identity(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 1.0); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;
    virtual const double getFloatingPoint(const Literal *lv) const;

    protected:
    Float32Type(LOCATION, Extension *ext) : FloatingPointType(PASSLOC, ext, "Float32", 32) { }
};

class Float64Type : public FloatingPointType {
    friend class BaseExtension;

    public:
    virtual size_t size() const { return 64; }
    Literal *literal(LOCATION, Compilation *comp, const double value) const;
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 0.0d); }
    Literal *identity(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, 1.0d); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual bool isConcrete() const { return true; }
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;
    virtual const double getFloatingPoint(const Literal *lv) const;

    protected:
    Float64Type(LOCATION, Extension *ext) : FloatingPointType(PASSLOC, ext, "Float64", 64) { }
};

class AddressType : public Type {
    friend class BaseExtension;

    public:
    virtual bool isConcrete() const { return true; }
    virtual size_t size() const { return 64; } // should be platform specific
    Literal *literal(LOCATION, Compilation *comp, const void * value) const;
    Literal *zero(LOCATION, Compilation *comp) const { return literal(PASSLOC, comp, NULL); }
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;
    virtual void createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const;

    protected:
    AddressType(LOCATION, Extension *ext);
    AddressType(LOCATION, Extension *ext, std::string name);
};

class PointerType;
class PointerTypeBuilder;
typedef void (PointerTypeHelper)(PointerType *pType, PointerTypeBuilder *builder);

class PointerTypeBuilder {
public:
    PointerTypeBuilder(BaseExtension *ext, FunctionCompilation *comp);
    PointerTypeBuilder *setBaseType(const Type *type) { _baseType = type; return this; }
    PointerTypeBuilder *setHelper(PointerTypeHelper *helper) { _helper = helper; return this; }

    BaseExtension *extension() const { return _ext; }
    TypeDictionary *dict() const { return _dict; }
    const Type *baseType() const { return _baseType; }
    PointerTypeHelper *helper() const { return _helper; }
    std::string name() const { return std::string("PointerTo(") + _baseType->name() + std::string(")"); }

    const PointerType *create(LOCATION);

protected:
    BaseExtension * _ext;
    FunctionCompilation * _comp;
    TypeDictionary *_dict;
    const Type * _baseType;
    PointerTypeHelper *_helper;
};
    
class PointerType : public Type {
    friend class PointerTypeBuilder;

    public:
    virtual bool isPointer() const { return true; }
    const Type * BaseType() const { return _baseType; }

    Literal *literal(LOCATION, Compilation *comp, const void * value) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual void writeSpecificType(TextWriter &w) const;
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    PointerType(LOCATION, PointerTypeBuilder *builder);
    const Type * _baseType;
};

struct StructType;

class FieldType : public Type {
    friend class StructType;
    #if NEED_UNION
    friend class UnionType; // ?
    #endif

    public:
    const StructType *owningStruct() const  { return _structType; }
    std::string fieldName() const { return _fieldName; }
    const Type *type() const { return _type; }
    size_t offset() const { return _offset; }
    virtual bool isField() const { return true; }

    Literal *literal(LOCATION, Compilation *comp, const LiteralBytes * structValue) const { return NULL; };
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const { return false; }
    virtual void writeSpecificType(TextWriter &w) const;
    virtual void printValue(TextWriter &w, const void *p) const { }
    virtual void printLiteral(TextWriter &w, const Literal *lv) const { }
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;

    protected:
    FieldType(LOCATION, TypeDictionary *dict, const StructType *structType, std::string fieldName, const Type *type, size_t offset);

    const StructType *_structType;
    std::string _fieldName;
    const Type *_type;
    size_t _offset;
};

typedef std::map<std::string, const FieldType *>::const_iterator FieldIterator;

class StructTypeBuilder;
typedef void (StructHelperFunction)(const StructType *sType, StructTypeBuilder *builder);
#if NEED_UNION
class UnionType;
#endif

class StructTypeBuilder {
    // FieldInfo is used to record fields
    struct FieldInfo {
        std::string _name;
        const Type * _type;
        size_t _offset;
        FieldInfo(std::string name, const Type *type, size_t offset)
            : _name(name), _type(type), _offset(offset) {
        }
    };

public:
    StructTypeBuilder(BaseExtension *ext, Function *func);
    #if NEED_UNION
    StructTypeBuilder *setUnion(bool v=false); { _buildUnion = v; return this; }
    #endif
    StructTypeBuilder *setName(std::string n) { _name = n; return this; }
    StructTypeBuilder *setSize(size_t size) { _size = size; return this; }
    StructTypeBuilder *setHelper(StructHelperFunction *helper) { _helper = helper; return this; }
    StructTypeBuilder *addField(std::string name, const Type *fieldType, size_t offset) {
        FieldInfo info(name, fieldType, offset);
        _fields.push_back(info);
        return this;
    }

    BaseExtension *extension() const { return _ext; }
    FunctionCompilation *comp() const { return _comp; }
    TypeDictionary *dict() const { return _dict; }
    std::string name() const { return _name; }
    size_t size() const { return _size; }
    StructHelperFunction *helper() const { return _helper; }

    void createFields(LOCATION, StructType *structType);
    bool verifyFields(const StructType *sType);

    const StructType * create(LOCATION);
    #if NEED_UNION
    const UnionType *createUnion();
    #endif

protected:
    BaseExtension * _ext;
    Function * _func;
    FunctionCompilation * _comp;
    TypeDictionary * _dict;
    std::string _name;
    size_t _size;
    #if NEED_UNION
    bool _buildUnion;
    #endif
    std::list<FieldInfo> _fields;
    StructHelperFunction *_helper;
};

class StructType : public Type {
    friend class StructTypeBuilder;

    public:
    virtual bool isStruct() const { return true; }
    virtual bool isConcrete() const { return true; }

    Literal *literal(LOCATION, Compilation *comp, const LiteralBytes * structValue) const;
    virtual bool literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const;
    virtual void writeSpecificType(TextWriter &w) const;
    virtual void printValue(TextWriter &w, const void *p) const;
    virtual void printLiteral(TextWriter &w, const Literal *lv) const;
    virtual bool registerJB1Type(JB1MethodBuilder *j1mb) const;

    FieldIterator FieldsBegin() const { return _fieldsByName.cbegin(); }
    FieldIterator FieldsEnd() const   { return _fieldsByName.cend(); }
    const FieldType *LookupField(std::string fieldName) const
        {
        auto it = _fieldsByName.find(fieldName);
        if (it == _fieldsByName.end())
            return NULL;
        return it->second;
        }

    protected:
    StructType(LOCATION, StructTypeBuilder *builder);
    virtual const FieldType * addField(LOCATION, TypeDictionary *dict, std::string name, const Type *type, size_t offset);
    void registerAllFields(JB1MethodBuilder *j1mb, std::string structName, std::string fNamePrefix, size_t baseOffset) const;

    std::map<std::string, const FieldType *> _fieldsByName;
    std::multimap<size_t, const FieldType *> _fieldsByOffset;
};

#if NEED_UNION
// why is this class needed?
class UnionType : public StructType {
    friend class StructTypeBuilder;

    public:
    virtual bool isStruct() const { return false; }
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
    UnionType(LOCATION, StructBuilder *builder);
};
#endif

class FunctionType : public Type {
    friend class BaseExtension;
    friend class TypeDictionary;

    public:
    #if 0
    class TypeBuilder {
        struct FieldInfo {
            std::string _name;
            const Type * _type;
            size_t _offset;
            FieldInfo(std::string name, const Type *type, size_t offset)
                : _name(name, _type(type), _offset(offset) {
            }
        };
    public:
        TypeBuilder()
            : _ext(NULL)
            , _name("")
            , _size(0)
            , _helper(NULL) {
        }
        TypeBuilder *setExtension(Extension *ext) { _ext = ext; }
        TypeBuilder *setName(std::string n) { _name = n; }
        TypeBuilder *setSize(size_t size) { _size = size; }
        TypeBuilder *addField(std::string name, const Type *fieldType, size_t offset) {
            FieldInfo info(name, fieldType, offset);
            _fields.push_back(info);
        }
        TypeBuilder *setHelper(StructHelperFuntion *helper) { _helper = helper; }

        Extention *extension() const { return _extension; }
        std::string name() const { return _name; }
        size_t size() const { return _size; }
        StructHelperFunction *helper() const { return _helper; }

        void createFields(LOCATION, StructType *structType) {
            for (auto it = _fields.begin(); it != _fields.end(); it++) {
                FieldInfo info = *it;
                structType->addField(PASSLOC, info._name, info._type, info._offset);
            }
        }

        const StructType *create() {
            return StructType::create(this);
        }

    protected:
        Extension * _ext;
        std::string _name;
        size_t _size;
        std::list<FieldInfo> _fields;
        StructHelperFunction _helper;
    };
    #endif
    
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
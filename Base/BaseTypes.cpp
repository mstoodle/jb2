/*******************************************************************************
 * Copyright (c) 2021, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.acompache.org/licenses/LICENSE-2.0.
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

#include <cstring>
#include "BaseExtension.hpp"
#include "BaseTypes.hpp"
#include "Compiler.hpp"
#include "Function.hpp"
#include "FunctionCompilation.hpp"
#include "JB1MethodBuilder.hpp"
#include "Literal.hpp"
#include "Type.hpp"
#include "TextWriter.hpp"
#include "TypeDictionary.hpp"

namespace OMR {
namespace JitBuilder {
namespace Base {

TypeKind NoTypeType::TYPEKIND = KindService::NoKind;

void
NoTypeType::printValue(TextWriter &w, const void *p) const {
    w << name();
}

bool
NoTypeType::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerNoType(this);
    return true;
}

TypeKind NumericType::TYPEKIND = Type::kindService.assignKind(KindService::AnyKind, "NumericType");

TypeKind IntegerType::TYPEKIND = Type::kindService.assignKind(NumericType::TYPEKIND, "IntegerType");

TypeKind Int8Type::TYPEKIND = Type::kindService.assignKind(IntegerType::TYPEKIND, "Int8");

Literal *
Int8Type::literal(LOCATION, Compilation *comp, const int8_t value) const {
    int8_t *pValue = new int8_t;
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
Int8Type::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<const int8_t *>(l1)) == (*reinterpret_cast<const int8_t *>(l2));
}

void
Int8Type::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << ((int) *reinterpret_cast<const int8_t *>(p));
}

void
Int8Type::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << ((int) lv->value<const int8_t>()) << ")";
}

const int64_t
Int8Type::getInteger(const Literal *lv) const {
    return (const int64_t) (lv->value<const int8_t>());
}

bool
Int8Type::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt8(this);
    return true;
}

void
Int8Type::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstInt8(loc, b, result, lv->value<const int8_t>());
}


TypeKind Int16Type::TYPEKIND = Type::kindService.assignKind(IntegerType::TYPEKIND, "Int16");

Literal *
Int16Type::literal(LOCATION, Compilation *comp, const int16_t value) const {
    int16_t *pValue = new int16_t;
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
Int16Type::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<const int16_t *>(l1)) == (*reinterpret_cast<const int16_t *>(l2));
}

void
Int16Type::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *reinterpret_cast<const int16_t *>(p);
}

void
Int16Type::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<const int16_t>()) << ")";
}

const int64_t
Int16Type::getInteger(const Literal *lv) const {
    return (const int64_t) (lv->value<const int16_t>());
}

bool
Int16Type::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt16(this);
    return true;
}

void
Int16Type::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstInt16(loc, b, result, lv->value<const int16_t>());
}


TypeKind Int32Type::TYPEKIND = Type::kindService.assignKind(IntegerType::TYPEKIND, "Int32");

Literal *
Int32Type::literal(LOCATION, Compilation *comp, const int32_t value) const {
    int32_t *pValue = new int32_t;
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
Int32Type::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<const int32_t *>(l1)) == (*reinterpret_cast<const int32_t *>(l2));
}

void
Int32Type::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *reinterpret_cast<const int32_t *>(p);
}

void
Int32Type::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<const int32_t>()) << ")";
}

const int64_t
Int32Type::getInteger(const Literal *lv) const {
    return (const int64_t) (lv->value<const int32_t>());
}

bool
Int32Type::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt32(this);
    return true;
}

void
Int32Type::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstInt32(loc, b, result, lv->value<const int32_t>());
}


TypeKind Int64Type::TYPEKIND = Type::kindService.assignKind(IntegerType::TYPEKIND, "Int64");

Literal *
Int64Type::literal(LOCATION, Compilation *comp, const int64_t value) const {
    int64_t *pValue = new int64_t;
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
Int64Type::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<const int64_t *>(l1)) == (*reinterpret_cast<const int64_t *>(l2));
}

void
Int64Type::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *reinterpret_cast<const int64_t *>(p);
}

void
Int64Type::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<const int64_t>()) << ")";
}

const int64_t
Int64Type::getInteger(const Literal *lv) const {
    return (const int64_t) (lv->value<const int64_t>());
}

bool
Int64Type::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt64(this);
    return true;
}

void
Int64Type::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstInt64(loc, b, result, lv->value<const int64_t>());
}


TypeKind FloatingPointType::TYPEKIND = Type::kindService.assignKind(NumericType::TYPEKIND, "FloatingPoint");

TypeKind Float32Type::TYPEKIND = Type::kindService.assignKind(FloatingPointType::TYPEKIND, "Float32");

Literal *
Float32Type::literal(LOCATION, Compilation *comp, const float value) const {
    float *pValue = new float[1];
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
Float32Type::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<const float *>(l1)) == (*reinterpret_cast<const float *>(l2));
}

void
Float32Type::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *reinterpret_cast<const float *>(p);
}

void
Float32Type::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<const float>()) << ")";
}

const double
Float32Type::getFloatingPoint(const Literal *lv) const {
    return (const double) (lv->value<const float>());
}

bool
Float32Type::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerFloat(this);
    return true;
}

void
Float32Type::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstFloat(loc, b, result, lv->value<const float>());
}


TypeKind Float64Type::TYPEKIND = Type::kindService.assignKind(FloatingPointType::TYPEKIND, "Float64");

Literal *
Float64Type::literal(LOCATION, Compilation *comp, const double value) const {
    double *pValue = new double[1];
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
Float64Type::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<const double *>(l1)) == (*reinterpret_cast<const double *>(l2));
}

void
Float64Type::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *reinterpret_cast<const double *>(p);
}

void
Float64Type::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<const double>()) << ")";
}

const double
Float64Type::getFloatingPoint(const Literal *lv) const {
    return (const double) (lv->value<const double>());
}

bool
Float64Type::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerDouble(this);
    return true;
}

void
Float64Type::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstDouble(loc, b, result, lv->value<const double>());
}


TypeKind AddressType::TYPEKIND = Type::kindService.assignKind(KindService::AnyKind, "Address");

AddressType::AddressType(LOCATION, Extension *ext)
    : Type(PASSLOC, TYPEKIND, ext, "Address", ext->compiler()->platformWordSize()) {

}

AddressType::AddressType(LOCATION, Extension *ext, std::string name)
    : Type(PASSLOC, TYPEKIND, ext, name, ext->compiler()->platformWordSize() ) {
}

AddressType::AddressType(LOCATION, Extension *ext, TypeDictionary *dict, std::string name)
    : Type(PASSLOC, TYPEKIND, ext, dict, name, dict->compiler()->platformWordSize() ) {
}

AddressType::AddressType(LOCATION, Extension *ext, TypeDictionary *dict, TypeKind kind, std::string name)
    : Type(PASSLOC, kind, ext, dict, name, dict->compiler()->platformWordSize() ) {
}



Literal *
AddressType::literal(LOCATION, Compilation *comp, const void * value) const {
    const void **pValue = new const void *[1];
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}
bool
AddressType::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<void * const *>(l1)) == (*reinterpret_cast<void * const *>(l2));
}

void
AddressType::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *(reinterpret_cast<const void * const *>(p));
}

void
AddressType::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<void * const>()) << ")";
}

bool
AddressType::registerJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerAddress(this);
    return true;
}

void
AddressType::createJB1ConstOp(Location *loc, JB1MethodBuilder *j1mb, Builder *b, Value *result, Literal *lv) const {
    j1mb->ConstAddress(loc, b, result, lv->value<void * const>());
}


PointerTypeBuilder::PointerTypeBuilder(BaseExtension *ext, FunctionCompilation *comp)
    : _ext(ext)
    , _comp(comp)
    , _dict(comp->dict())
    , _baseType(NULL)
    , _helper(NULL) {

}

const PointerType *
PointerTypeBuilder::create(LOCATION) {
    const PointerType *existingType = _comp->pointerTypeFromBaseType(_baseType);
    if (existingType != NULL)
        return existingType;

    const PointerType *newType = new PointerType(PASSLOC, this);
    _dict->registerType(newType);
    return newType;
}


TypeKind PointerType::TYPEKIND = Type::kindService.assignKind(AddressType::TYPEKIND, "Pointer");

PointerType::PointerType(LOCATION, PointerTypeBuilder *builder)
    : AddressType(PASSLOC, builder->extension(), builder->dict(), TYPEKIND, builder->name()) {

    if (builder->helper())
        builder->helper()(this, builder);
    _baseType = builder->baseType();
    assert(_baseType);

    builder->comp()->registerPointerType(this);
}

Literal *
PointerType::literal(LOCATION, Compilation *comp, const void * value) const {
    const void **pValue = new const void *[1];
    *pValue = value;
    return this->Type::literal(PASSLOC, comp, reinterpret_cast<const LiteralBytes *>(pValue));
}

bool
PointerType::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return (*reinterpret_cast<void * const *>(l1)) == (*reinterpret_cast<void * const *>(l2));
}

std::string
PointerType::to_string(bool useHeader) const {
    std::string s = Type::base_string(useHeader);
    return s.append(std::string("pointerType base t")).append(std::to_string(_baseType->id()));
}

void
PointerType::printValue(TextWriter &w, const void *p) const {
    w << name() << " " << *(reinterpret_cast<const void * const *>(p));
}

void
PointerType::printLiteral(TextWriter & w, const Literal *lv) const {
    w << name() << "(" << (lv->value<void * const>()) << ")";
}

bool
PointerType::registerJB1Type(JB1MethodBuilder *j1mb) const {
    if (!j1mb->typeRegistered(_baseType)) // wait until base type is registered
        return false;

    j1mb->registerPointer(this, _baseType);
    return true;
}


TypeKind FieldType::TYPEKIND = Type::kindService.assignKind(KindService::AnyKind, "Field");

FieldType::FieldType(LOCATION, Extension *ext, TypeDictionary *dict, const StructType *structType, std::string name, const Type *type, size_t offset)
    : Type(PASSLOC, TYPEKIND, ext, dict, structType->name() + std::string(".") + name, type->size())
    , _structType(structType)
    , _fieldName(name)
    , _type(type)
    , _offset(offset) {

}

std::string
FieldType::to_string(bool useHeader) const {
    std::string s = Type::base_string(useHeader);
    s.append(std::string("fieldType ")).append(_fieldName);
    s.append(std::string(" size ")).append(std::to_string(_type->size()));
    s.append(std::string(" t")).append(std::to_string(_type->id()));
    s.append(std::string("@")).append(std::to_string(_offset));
    return s;
}

bool
FieldType::registerJB1Type(JB1MethodBuilder *j1mb) const {
    // Fields are registered by the StructType
    //j1mb->registerField(_structType->name(), name(), _type, _offset);
    return true;
}


StructTypeBuilder::StructTypeBuilder(BaseExtension *ext, Function *func)
    : _ext(ext)
    , _func(func)
    , _comp(func->comp())
    , _dict(func->comp()->dict())
    , _size(0)
    , _helper(NULL) {

}

void
StructTypeBuilder::createFields(LOCATION, StructType *structType) {
    for (auto it = _fields.begin(); it != _fields.end(); it++) {
        FieldInfo info = *it;
        structType->addField(PASSLOC, _ext, _dict, info._name, info._type, info._offset);
    }
}

bool
StructTypeBuilder::verifyFields(const StructType *sType) {
    // todo: check fields match!
    return true;
}

const StructType *
StructTypeBuilder::create(LOCATION) {
    const StructType *existingType = _comp->structTypeFromName(_name);
    if (existingType != NULL) {
        if (verifyFields(existingType))
            return existingType;
        // error code?
        return NULL;
    }

    const StructType *newType = new StructType(PASSLOC, this);
    return newType;
}


TypeKind StructType::TYPEKIND = Type::kindService.assignKind(KindService::AnyKind, "Struct");

StructType::StructType(LOCATION, StructTypeBuilder *builder)
    : Type(PASSLOC, TYPEKIND, builder->extension(), builder->dict(), builder->name(), builder->size())
    , _structSize(0) {

    _dict->registerType(this); // proactive: other types may be created before we're done
    if (builder->helper())
        builder->helper()(this, builder);
    builder->createFields(PASSLOC, this);
    builder->comp()->registerStructType(this);
}

const FieldType *
StructType::addField(LOCATION, Extension *ext, TypeDictionary *dict, std::string name, const Type *type, size_t offset) {
    const FieldType *preExistingField = LookupField(name);
    if (preExistingField) {
        if (preExistingField->type() == type && preExistingField->offset() == offset)
            return preExistingField;
        return NULL;
    }

    FieldType *field = new FieldType(PASSLOC, ext, dict, this, name, type, offset);
    _fieldsByName.insert({name, field});
    _fieldsByOffset.insert({offset, field});

    if (_structSize < offset + type->size())
        _structSize = offset + type->size();

    _dict->registerType(field);
    return field;
}

std::string
StructType::to_string(bool useHeader) const {
    std::string s = Type::base_string(useHeader);
    s.append(std::string("structType size ")).append(std::to_string(size()));
    for (auto it = FieldsBegin(); it != FieldsEnd(); it++) {
        auto field = it->second;
        s.append(std::string(" t")).append(std::to_string(field->id()));
        s.append(std::string("@")).append(std::to_string(field->offset()));
    }
    return s;
}

Literal *
StructType::literal(LOCATION, Compilation *comp, const LiteralBytes * structValue) const {
    return this->Type::literal(PASSLOC, comp, structValue);
}

bool
StructType::literalsAreEqual(const LiteralBytes *l1, const LiteralBytes *l2) const {
    return memcmp(l1, l2, size()/8) == 0;
}

void
StructType::printValue(TextWriter &w, const void *p) const {
    // TODO
}

void
StructType::printLiteral(TextWriter & w, const Literal *lv) const {
    // TODO
}

void
StructType::registerAllFields(JB1MethodBuilder *j1mb, std::string structName, std::string fNamePrefix, size_t baseOffset) const {
    for (auto fIt = FieldsBegin(); fIt != FieldsEnd(); fIt++) {
        const FieldType *fType = fIt->second;
        std::string fieldName = fNamePrefix + fType->name();
        size_t fieldOffset = baseOffset + fType->offset();

        if (fType->isKind<StructType>()) {
            // define a "dummy" field corresponding to the struct field itself, so we can ask for its address easily
            // in case this field's struct needs to be passed to anything
            j1mb->registerField(structName, fieldName, static_cast<BaseExtension *>(_ext)->NoType, fieldOffset);
            const StructType *innerStructType = fType->type()->refine<StructType>();
            registerAllFields(j1mb, structName, fieldName + ".", fieldOffset);
        }
        else {
            j1mb->registerField(structName, fieldName, fType->type(), fieldOffset);
        }
    }
}

bool
StructType::registerJB1Type(JB1MethodBuilder *j1mb) const {
    if (!j1mb->typeRegistered(this)) {
        j1mb->registerStruct(this);
        return false; // first pass just creates struct types
    }

    registerAllFields(j1mb, name(), std::string(""), 0); // second pass defines the fields
    j1mb->closeStruct(name());
    return true;
}

#if NEED_UNION
void
UnionType::printValue(TextWriter &w, const void *p) const {
    // TODO
}
#endif


TypeKind FunctionType::TYPEKIND = Type::kindService.assignKind(KindService::AnyKind, "Function");

FunctionType::FunctionType(LOCATION, Extension *ext, TypeDictionary *dict, const Type *returnType, int32_t numParms, const Type ** parmTypes)
    : Type(PASSLOC, TYPEKIND, ext, dict, typeName(returnType, numParms, parmTypes), 0)
    , _returnType(returnType)
    , _numParms(numParms)
    , _parmTypes(parmTypes) {

    dict->registerType(this);
}

std::string
FunctionType::typeName(const Type * returnType, int32_t numParms, const Type **parmTypes) {
    std::string s = std::string("t").append(std::to_string(returnType->id())).append(std::string(" <- ("));
    if (numParms > 0)
        s.append(std::string("0:t")).append(std::to_string(parmTypes[0]->id()));
    for (auto p = 1; p < numParms; p++) {
        const Type *type = parmTypes[p];
        s.append(std::string(" ")).append(std::to_string(p)).append(std::string(":t")).append(std::to_string(type->id()));
    }
    s.append(std::string(")"));
    return s;
}

std::string
FunctionType::to_string(bool useHeader) const {
    std::string s = Type::base_string(useHeader);
    s.append(std::string("functionType"));
    return s;
}

void
FunctionType::printValue(TextWriter &w, const void *p) const {
    // TODO
}

} // namespace Base
} // namespace JitBuilder
} // namespace OMR

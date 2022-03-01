/*******************************************************************************
 * Copyright (c) 2021, 2021 IBM Corp. and others
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

#include "BaseExtension.hpp"
#include "BaseTypes.hpp"
#include "Compiler.hpp"
#include "JB1MethodBuilder.hpp"
#include "Literal.hpp"
#include "Type.hpp"
#include "TextWriter.hpp"

namespace OMR {
namespace JitBuilder {
namespace Base {

void
NoTypeType::printValue(TextWriter &w, const void *p) const {
    w << name();
}

bool
NoTypeType::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerNoType(this);
    return true;
}


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

bool
Int8Type::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt8(this);
    return true;
}

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
Int16Type::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *reinterpret_cast<const int16_t *>(p);
   }

void
Int16Type::printLiteral(TextWriter & w, const Literal *lv) const {
   w << name() << "(" << (lv->value<const int16_t>()) << ")";
}

bool
Int16Type::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt16(this);
    return true;
}


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
Int32Type::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *reinterpret_cast<const int32_t *>(p);
   }

void
Int32Type::printLiteral(TextWriter & w, const Literal *lv) const {
   w << name() << "(" << (lv->value<const int32_t>()) << ")";
}

bool
Int32Type::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt32(this);
    return true;
}


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
Int64Type::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *reinterpret_cast<const int64_t *>(p);
   }

void
Int64Type::printLiteral(TextWriter & w, const Literal *lv) const {
   w << name() << "(" << (lv->value<const int64_t>()) << ")";
}

bool
Int64Type::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerInt64(this);
    return true;
}


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
Float32Type::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *reinterpret_cast<const float *>(p);
   }

void
Float32Type::printLiteral(TextWriter & w, const Literal *lv) const {
   w << name() << "(" << (lv->value<const float>()) << ")";
}

bool
Float32Type::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerFloat(this);
    return true;
}


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
Float64Type::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *reinterpret_cast<const double *>(p);
   }

void
Float64Type::printLiteral(TextWriter & w, const Literal *lv) const {
   w << name() << "(" << (lv->value<const double>()) << ")";
}

bool
Float64Type::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerDouble(this);
    return true;
}


AddressType::AddressType(LOCATION, Extension *ext)
    : Type(PASSLOC, ext, "Address", ext->compiler()->platformWordSize()) {
}

AddressType::AddressType(LOCATION, Extension *ext, std::string name)
    : Type(PASSLOC, ext, name, ext->compiler()->platformWordSize() ) {
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
AddressType::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *(reinterpret_cast<const void * const *>(p));
   }

void
AddressType::printLiteral(TextWriter & w, const Literal *lv) const {
   w << name() << "(" << (lv->value<void * const>()) << ")";
}

bool
AddressType::mapJB1Type(JB1MethodBuilder *j1mb) const {
    j1mb->registerAddress(this);
    return true;
}


void
PointerType::printType(TextWriter &w)
   {
   }

void
PointerType::printValue(TextWriter &w, const void *p) const
   {
   w << name() << " " << *(reinterpret_cast<void * const *>(p));
   }


#if 0
FieldType::FieldType(LOCATION, Extension *ext, StructType *structType, Literal *fieldName, Type *type, size_t offset)
    : Type(PASSLOC, ext, fieldName->get<std::string>(), type->size())
    , _structType(structType)
    , _fieldName(fieldName)
    , _type(type)
    , _offset(offset) {
}
#endif

void
FieldType::printType(TextWriter &w)
   {
   }

void
FieldType::printValue(TextWriter &w, const void *p) const
   {
   }

#if 0
FieldType *
StructType::addField(LOCATION, Literal *name, Type *type, size_t offset)
   {
   assert(name->kind() == T_string || name->kind() == T_typename);
   FieldType *field = LookupField(name->getString());
   if (field)
      return field;

   field = FieldType::create(PASSLOC, ext(), this, name, type, offset);
   _fieldsByName.insert({name->getString(), field});
   _fieldsByOffset.insert({offset, field});

   if (_size < offset + type->size())
      _size = offset + type->size();

   return field;
   }
#endif

FieldIterator
StructType::RemoveField(FieldIterator &it)
   {
   FieldType *fieldType = it->second;

   // iterator is for _fieldsByName, so just erase
   auto it1 = _fieldsByName.erase(it);

   // have to look for it in _fieldsByOffset and erase from it
   auto it2 = _fieldsByOffset.find(fieldType->offset());
   while (it2 != _fieldsByOffset.end())
      {
      // can have multiple fields at an offset so make sure we remove the right one
      FieldType *fType = it2->second;
      if (fType == fieldType)
         {
         _fieldsByOffset.erase(it2);
         break;
         }
      else
         it2++;
      }

   return it1;
   }

void
StructType::printType(TextWriter &w)
   {
   // TODO
   }

void
StructType::printValue(TextWriter &w, const void *p) const
   {
   // TODO
   }

#if 0
void
UnionType::printType(TextWriter &w)
   {
   // TODO
   }

void
UnionType::printValue(TextWriter &w, const void *p) const
   {
   // TODO
   }
#endif

FunctionType *
FunctionType::create(LOCATION,
                     Extension *ext,
                     std::string name,
                     const Type *returnType,
                     int32_t numParms,
                     const Type ** parmTypes)
   {
   return new FunctionType(PASSLOC, ext, name, returnType, numParms, parmTypes);
   }

void
FunctionType::printType(TextWriter &w)
   {
   // TODO
   }

void
FunctionType::printValue(TextWriter &w, const void *p) const
   {
   // TODO
   }

} // namespace Base
} // namespace JitBuilder
} // namespace OMR

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

#include <string>
#include "Compilation.hpp"
#include "Operation.hpp"
#include "Literal.hpp"
#include "LiteralDictionary.hpp"
#include "TextWriter.hpp"
#include "Type.hpp"
#include "Value.hpp"

namespace OMR {
namespace JitBuilder {


LiteralDictionary::LiteralDictionary(Compilation *comp)
    : _id(comp->getLiteralDictionaryID())
    , _comp(comp)
    , _name("")
    , _nextLiteralID(NoLiteral+1)
    , _linkedDictionary(NULL) {
}

LiteralDictionary::LiteralDictionary(Compilation *comp, std::string name)
    : _id(comp->getLiteralDictionaryID())
    , _comp(comp)
    , _name(name)
    , _nextLiteralID(NoLiteral+1)
    , _linkedDictionary(NULL) {
}

// Only accessible to subclasses
LiteralDictionary::LiteralDictionary(Compilation *comp, std::string name, LiteralDictionary * linkedLiterals)
    : _id(comp->getLiteralDictionaryID())
    , _comp(comp)
    , _name(name)
    , _nextLiteralID(linkedLiterals->_nextLiteralID)
    , _linkedDictionary(linkedLiterals) {
    for (LiteralIterator it = linkedLiterals->LiteralsBegin(); it != linkedLiterals->LiteralsEnd(); it++) {
        Literal *literal = *it;
        addNewLiteral(literal);
    }
}

LiteralDictionary::~LiteralDictionary() {
    for (auto it = _ownedLiterals.begin(); it != _ownedLiterals.end(); it++) {
        Literal *literal = *it;
        delete literal;
    }
}

Literal *
LiteralDictionary::LookupLiteral(LiteralID id) {
    for (auto it = LiteralsBegin(); it != LiteralsEnd(); it++) {
        Literal *literal = *it;
        if (literal->id() == id)
            return literal;
    }

    return NULL;
}

void
LiteralDictionary::RemoveLiteral(Literal *literal) {
    // brutal performance; should really collect these and do in one pass
    for (auto it = _literals.begin(); it != _literals.end(); ) {
       if (*it == literal)
          it = _literals.erase(it);
       else
          ++it;
   }
}

void
LiteralDictionary::addNewLiteral(Literal *literal) {
    LiteralVector *typeList = NULL;
    const Type *type = literal->type();
    auto it = _literalsByType.find(type);
    if (it != _literalsByType.end()) {
        typeList = it->second;
    }
    else {
        typeList = new LiteralVector;
        _literalsByType.insert({type, typeList});
    }
    typeList->push_back(literal);
    _literals.push_back(literal);
}

Literal *
LiteralDictionary::registerLiteral(LOCATION, const Type *type, const LiteralBytes *value) {
    LiteralVector *typeList = NULL;
    auto it = _literalsByType.find(type);
    if (it != _literalsByType.end()) {
        typeList = it->second;
        for (auto it = typeList->begin(); it != typeList->end();it++) {
            Literal *other = *it;
            if (type->literalsAreEqual(value, other->value())) {
                delete[] value;
                return other;
            }
        }
    }
    else {
        typeList = new LiteralVector;
        _literalsByType.insert({type, typeList});
    }

    Literal *literal = new Literal(PASSLOC, _comp, type, value);
    typeList->push_back(literal);
    _literals.push_back(literal);
    _ownedLiterals.push_back(literal);
    return literal;
}

void
LiteralDictionary::write(TextWriter &w)
   {
   w.indent() << "[ LiteralDictionary " << this << " \"" << this->name() << "\"" << w.endl();
   w.indentIn();
   if (this->hasLinkedDictionary())
      w.indent() << "[ linkedDictionary " << this->linkedDictionary() << " ]" << w.endl();
   for (LiteralIterator literalIt = this->LiteralsBegin();literalIt != this->LiteralsEnd();literalIt++)
      {
      Literal *literal = *literalIt;
      literal->write(w);
      w << w.endl();
      }
   w.indentOut();
   w.indent() << "]" << w.endl();
   }

#if 0

// Move below to BaseExtension

void
TypeDictionary::createPrimitiveTypes()
   {
   NoType = NoTypeType::create(this);
   addType(NoType);

   Int8 = Int8Type::create(this);
   addType(Int8);

   Int16 = Int16Type::create(this);
   addType(Int16);

   Int32 = Int32Type::create(this);
   addType(Int32);

   Int64 = Int64Type::create(this);
   addType(Int64);

   Float = FloatType::create(this);
   addType(Float);

   Double = DoubleType::create(this);
   addType(Double);

   Address = PointerType::create(this, "Address", Int8);
   addType(Address);

   Word = Int64; // should really be changed based on Config

   //
   // User type handling
   // BEGIN {

   // } END
   // User type handling
   //

   }

PointerType *
TypeDictionary::PointerTo(Type * baseType)
   {
   // don't replicate types
   std::map<Type *,PointerType *>::iterator found = _pointerTypeFromBaseType.find(baseType);
   if (found != _pointerTypeFromBaseType.end())
      {
      PointerType *t = found->second;
      return t;
      }

   // if not found already, then create it
   PointerType * newType = PointerType::create(this, std::string("PointerTo(") + baseType->name()  + std::string(")"), baseType);
   addType(newType);
   _pointerTypeFromBaseType[baseType] = newType;
   registerPointerType(newType);
   return newType;
   }

void
TypeDictionary::RemoveType(Type *type)
   {

   if (type->isField())
      ; // nothing special for fields themselves
   else if (type->isStruct() || type->isUnion())
      {
      StructType *sType = static_cast<StructType *>(type);
      bool fullyRemove = (type->owningDictionary() == this);
      for (auto it = sType->FieldsBegin(); it != sType->FieldsEnd(); )
         {
         FieldType *fType = it->second;
         forgetType(fType);

         if (fullyRemove)
            it = sType->RemoveField(it);
         else
            it++;
         }

      _structTypeFromName.erase(type->name());
      }
   else if (type->isPointer())
      {
      PointerType *ptrType = static_cast<PointerType *>(type);
      _pointerTypeFromBaseType.erase(ptrType->BaseType());
      }
   else if (type->isFunction())
      _functionTypeFromName.erase(type->name());

   forgetType(type);

   // TODO but not *strictly* necessary: _graph->unregister(type);
   }

StructType *
TypeDictionary::LookupStruct(std::string structName)
   {
   std::map<std::string,StructType *>::iterator found = _structTypeFromName.find(structName);
   if (found != _structTypeFromName.end())
      return found->second;

   return NULL;
   }

StructType *
TypeDictionary::DefineStruct(std::string structName, size_t size)
   {
   StructType *existingType = LookupStruct(structName);
   if (existingType)
      {
      assert(existingType->size() == size);
      return existingType;
      }

   StructType * newType = StructType::create(this, structName, size);
   addType(newType);
   _structTypeFromName.insert({structName, newType});
   _graph->registerType(newType);
   _graph->registerType(PointerTo(newType));
   return newType;
   }

UnionType *
TypeDictionary::DefineUnion(std::string unionName)
   {
   StructType *existingType = LookupStruct(unionName);
   if (existingType)
      {
      assert(existingType->isUnion());
      return static_cast<UnionType *>(existingType);
      }

   UnionType * newType = UnionType::create(this, unionName);
   addType(newType);
   _structTypeFromName.insert({unionName, newType});
   _graph->registerType(newType);
   _graph->registerType(PointerTo(newType));
   return newType;
   }

FieldType *
TypeDictionary::DefineField(StructType *structType, Literal *fieldName, Type * fType, size_t offset)
   {
   assert(structType);
   if (structType->closed())
      {
      // TODO: useful error message
      return NULL;
      }

   assert(fieldName->kind() == T_string || fieldName->kind() == T_typename);
   FieldType *fieldType = structType->LookupField(fieldName->getString());
   if (fieldType)
      {
      assert(fieldType->type() == fType && fieldType->offset() == offset);
      return fieldType;
      }

   fieldType = structType->addField(fieldName, fType, offset);
   addType(fieldType);
   _graph->registerValidDirectFieldAccess(fieldType, structType);
   _graph->registerValidIndirectFieldAccess(fieldType, PointerTo(structType));
   return fieldType;
   }

void
TypeDictionary::CloseStruct(Type *type)
   {
   assert(type && type->isStruct());
   StructType *structType = static_cast<StructType *>(type);
   structType->setClosed();
   }

void
TypeDictionary::CloseUnion(Type *type)
   {
   assert(type && type->isUnion());
   UnionType *unionType = static_cast<UnionType *>(type);
   unionType->setClosed();
   }

FunctionType *
TypeDictionary::DefineFunctionType(std::string name,
                                   Type *returnType,
                                   int32_t numParms,
                                   Type **parmTypes)
   {
   // don't replicate types
   std::map<std::string,FunctionType *>::iterator found = _functionTypeFromName.find(name);
   if (found != _functionTypeFromName.end())
      {
      FunctionType *t = found->second;
      assert(t->returnType() == returnType && t->numParms() == numParms);
      for (int p=0;p < numParms;p++)
         assert(t->parmType(p) == parmTypes[p]);
      return t;
      }

   FunctionType * newType = FunctionType::create(this, name, returnType, numParms, parmTypes);
   addType(newType);
   _graph->registerFunctionType(newType);
   return newType;
   }
#endif

} // namespace JitBuilder
} // namespace OMR
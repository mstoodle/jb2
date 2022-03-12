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

#include <stdio.h>
#include <string>
#include "Compiler.hpp"
#include "Operation.hpp"
#include "TextWriter.hpp"
#include "Type.hpp"
#include "TypeDictionary.hpp"
#include "Value.hpp"

namespace OMR {
namespace JitBuilder {


TypeDictionary::TypeDictionary(Compiler *compiler)
    : _id(compiler->getTypeDictionaryID())
    , _compiler(compiler)
    , _name("")
    , _nextTypeID(0)
    , _linkedDictionary(NULL) {
}

TypeDictionary::TypeDictionary(Compiler *compiler, std::string name)
    : _id(compiler->getTypeDictionaryID())
    , _compiler(compiler)
    , _name(name)
    , _nextTypeID(0)
    , _linkedDictionary(NULL) {
}

// Only accessible to subclasses
TypeDictionary::TypeDictionary(Compiler *compiler, std::string name, TypeDictionary * linkedDict)
    : _id(compiler->getTypeDictionaryID())
    , _compiler(compiler)
    , _name(name)
    , _nextTypeID(linkedDict->_nextTypeID)
    , _linkedDictionary(linkedDict) {
    for (TypeIterator typeIt = linkedDict->TypesBegin(); typeIt != linkedDict->TypesEnd(); typeIt++)
        {
        const Type *type = *typeIt;
        internalRegisterType(type);
        }
    _nextTypeID = linkedDict->_nextTypeID;
}

TypeDictionary::~TypeDictionary() {
    for (auto it = _ownedTypes.begin(); it != _ownedTypes.end(); it++) {
        const Type *type = *it;
        delete type;
    }
}

const Type *
TypeDictionary::LookupType(TypeID id) {
    for (auto it = TypesBegin(); it != TypesEnd(); it++) {
        const Type *type = *it;
        if (type->id() == id)
            return type;
    }

    return NULL;
}

void
TypeDictionary::RemoveType(const Type *type) {
    // brutal performance; should really collect these and do in one pass
    for (auto it = _types.begin(); it != _types.end(); ) {
        if (*it == type)
            it = _types.erase(it);
        else
            ++it;
   }
}

void
TypeDictionary::write(TextWriter &w) {
    w.indent() << "[ TypeDictionary " << this << " \"" << this->name() << "\"" << w.endl();
    w.indentIn();
    if (this->hasLinkedDictionary())
        w.indent() << "[ linkedDictionary " << this->linkedDictionary() << " ]" << w.endl();
    for (TypeIterator typeIt = this->TypesBegin();typeIt != this->TypesEnd();typeIt++) {
        const Type *type = *typeIt;
        type->writeType(w);
    }
    w.indentOut();
    w.indent() << "]" << w.endl();
}

void
TypeDictionary::internalRegisterType(const Type *type) {
    _types.push_back(type);
}

void
TypeDictionary::registerType(const Type *type) {
    internalRegisterType(type);
    _ownedTypes.push_back(type);
}

#if 0
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
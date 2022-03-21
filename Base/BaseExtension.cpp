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

#include "ArithmeticOperations.hpp"
#include "BaseExtension.hpp"
#include "BaseOperations.hpp"
#include "BaseSymbols.hpp"
#include "BaseTypes.hpp"
#include "Builder.hpp"
#include "ConstOperations.hpp"
#include "ControlOperations.hpp"
#include "Compilation.hpp"
#include "Compiler.hpp"
#include "JB1CodeGenerator.hpp"
#include "Literal.hpp"
#include "Location.hpp"
#include "MemoryOperations.hpp"
#include "Strategy.hpp"
#include "TextWriter.hpp"
#include "Value.hpp"

namespace OMR {
namespace JitBuilder {
namespace Base {

const SemanticVersion BaseExtension::version(BASEEXT_MAJOR,BASEEXT_MINOR,BASEEXT_PATCH);
const std::string BaseExtension::NAME("base");

extern "C" {
    Extension *create(Compiler *comp) {
        return new BaseExtension(comp);
    }
}

BaseExtension::BaseExtension(Compiler *compiler)
    : Extension(compiler, NAME)
    , NoType(registerType<NoTypeType>(new NoTypeType(LOC, this)))
    , Int8(registerType<Int8Type>(new Int8Type(LOC, this)))
    , Int16(registerType<Int16Type>(new Int16Type(LOC, this)))
    , Int32(registerType<Int32Type>(new Int32Type(LOC, this)))
    , Int64(registerType<Int64Type>(new Int64Type(LOC, this)))
    , Float32(registerType<Float32Type>(new Float32Type(LOC, this)))
    , Float64(registerType<Float64Type>(new Float64Type(LOC, this)))
    , Address(registerType<AddressType>(new AddressType(LOC, this)))
    , Word(compiler->platformWordSize() == 64 ? (IntegerType *)this->Int64 : (IntegerType *)this->Int32)
    , aConstInt8(registerAction(std::string("ConstInt8")))
    , aConstInt16(registerAction(std::string("ConstInt16")))
    , aConstInt32(registerAction(std::string("ConstInt32")))
    , aConstInt64(registerAction(std::string("ConstInt64")))
    , aConstFloat32(registerAction(std::string("ConstFloat32")))
    , aConstFloat64(registerAction(std::string("ConstFloat64")))
    , aConstAddress(registerAction(std::string("ConstAddress")))
    , aAdd(registerAction(std::string("Add")))
    , aMul(registerAction(std::string("Mul")))
    , aSub(registerAction(std::string("Sub")))
    , aLoad(registerAction(std::string("Load")))
    , aStore(registerAction(std::string("Store")))
    , aLoadAt(registerAction(std::string("LoadAt")))
    , aStoreAt(registerAction(std::string("StoreAt")))
    , aLoadField(registerAction(std::string("LoadField")))
    , aStoreField(registerAction(std::string("StoreField")))
    , aLoadFieldAt(registerAction(std::string("LoadFieldAt")))
    , aStoreFieldAt(registerAction(std::string("StoreFieldAt")))
    , aCreateLocalArray(registerAction(std::string("CreateLocalArray")))
    , aCreateLocalStruct(registerAction(std::string("CreateLocalStruct")))
    , aIndexAt(registerAction(std::string("IndexAt")))
    , aReturn(registerAction(std::string("Return"))) {

    Strategy *jb1cgStrategy = new Strategy(compiler, "jb1cg");
    Pass *jb1cg = new JB1CodeGenerator(compiler);
    jb1cgStrategy->addPass(jb1cg);
    _jb1cgStrategyID = jb1cgStrategy->id();
}

BaseExtension::~BaseExtension() {
    delete Address;
    delete Float64;
    delete Float32;
    delete Int64;
    delete Int32;
    delete Int16;
    delete Int8;
    delete NoType;
    // delete pointer types?
}

//
// Const Operations
//

Value *
BaseExtension::ConstInt8(LOCATION, Builder *b, int8_t v)
   {
   Value * result = createValue(b, this->Int8);
   Literal *lv = this->Int8->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstInt8(PASSLOC, this, b, this->aConstInt8, result, lv));
   return result;
   }

Value *
BaseExtension::ConstInt16(LOCATION, Builder *b, int16_t v)
   {
   Value * result = createValue(b, this->Int16);
   Literal *lv = this->Int16->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstInt16(PASSLOC, this, b, this->aConstInt16, result, lv));
   return result;
   }

Value *
BaseExtension::ConstInt32(LOCATION, Builder *b, int32_t v)
   {
   Value * result = createValue(b, this->Int32);
   Literal *lv = this->Int32->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstInt32(PASSLOC, this, b, this->aConstInt32, result, lv));
   return result;
   }

Value *
BaseExtension::ConstInt64(LOCATION, Builder *b, int64_t v)
   {
   Value * result = createValue(b, this->Int64);
   Literal *lv = this->Int64->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstInt64(PASSLOC, this, b, this->aConstInt64, result, lv));
   return result;
   }

Value *
BaseExtension::ConstFloat32(LOCATION, Builder *b, float v)
   {
   Value * result = createValue(b, this->Float32);
   Literal *lv = this->Float32->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstFloat32(PASSLOC, this, b, this->aConstFloat32, result, lv));
   return result;
   }

Value *
BaseExtension::ConstFloat64(LOCATION, Builder *b, double v)
   {
   Value * result = createValue(b, this->Float64);
   Literal *lv = this->Float64->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstFloat64(PASSLOC, this, b, this->aConstFloat64, result, lv));
   return result;
   }

Value *
BaseExtension::ConstAddress(LOCATION, Builder *b, void * v)
   {
   Value * result = createValue(b, this->Address);
   Literal *lv = this->Address->literal(PASSLOC, b->comp(), v);
   addOperation(b, new Op_ConstAddress(PASSLOC, this, b, this->aConstAddress, result, lv));
   return result;
   }


//
// Arithmetic operations
//
Value *
BaseExtension::Add(LOCATION, Builder *b, Value *left, Value *right) {
    bool valid = false;
    const Type *lType = left->type();
    const Type *rType = right->type();
    if (lType == Address)
        valid = (rType == Word);
    else
        valid = (lType == rType);
    #if 0
    if (!valid && haveTypeCheckers())
        valid = checkTypes(aAdd, left, right);
    #endif

    if (!valid)
        assert(0);

    Value *result = createValue(b, left->type());
    addOperation(b, new Op_Add(PASSLOC, this, b, aAdd, result, left, right));
    return result;
}

Value *
BaseExtension::Mul(LOCATION, Builder *b, Value *left, Value *right) {
    bool valid = false;
    const Type *lType = left->type();
    const Type *rType = right->type();
    if (lType == Address)
        valid = (rType == Word);
    else
        valid = (lType == rType);
    #if 0
    if (!valid && haveTypeCheckers())
        valid = checkTypes(aMul, left, right);
    #endif

    if (!valid)
        assert(0);

    Value *result = createValue(b, left->type());
    addOperation(b, new Op_Mul(PASSLOC, this, b, aMul, result, left, right));
    return result;
}

Value *
BaseExtension::Sub(LOCATION, Builder *b, Value *left, Value *right) {
    bool valid = false;
    const Type *lType = left->type();
    const Type *rType = right->type();
    const Type *returnType=NULL;
    if (lType == Address) {
        if (rType == Word) {
            valid = true;
            returnType = Address;
        }
        else if (rType == Address) {
           valid = true;
           returnType = Word;
        }
    }
    else {
        valid = (lType == rType);
        returnType = lType;
    }

    #if 0
    if (!valid && haveTypeCheckers())
        valid = checkTypes(aAdd, left, right);
    #endif

    if (!valid)
        assert(0);

    Value *result = createValue(b, left->type());
    addOperation(b, new Op_Sub(PASSLOC, this, b, aSub, result, left, right));
    return result;
}

//
// Control operations
//

void
BaseExtension::Return(LOCATION, Builder *b) {
    addOperation(b, new Op_Return(PASSLOC, this, b, this->aReturn));
}

void
BaseExtension::Return(LOCATION, Builder *b, Value *v) {
    addOperation(b, new Op_Return(PASSLOC, this, b, this->aReturn, v));
}

//
// Memory operations
//

Value *
BaseExtension::Load(LOCATION, Builder *b, Symbol * sym) {
    Value * result = createValue(b, sym->type());
    addOperation(b, new Op_Load(PASSLOC, this, b, this->aLoad, result, sym));
    return result;
}

void
BaseExtension::Store(LOCATION, Builder *b, Symbol * sym, Value *value) {
    addOperation(b, new Op_Store(PASSLOC, this, b, this->aLoad, sym, value));
}

Value *
BaseExtension::LoadAt(LOCATION, Builder *b, Value *ptrValue) {
    assert(ptrValue->type()->isPointer());
    const Type *baseType = (static_cast<const PointerType *>(ptrValue->type()))->BaseType();
    Value * result = createValue(b, baseType);
    addOperation(b, new Op_LoadAt(PASSLOC, this, b, this->aLoadAt, result, ptrValue));
    return result;
}

void
BaseExtension::StoreAt(LOCATION, Builder *b, Value *ptrValue, Value *value) {
    assert(ptrValue->type()->isPointer());
    const Type *baseType = (static_cast<const PointerType *>(ptrValue->type()))->BaseType();
    assert(baseType == value->type());
    addOperation(b, new Op_StoreAt(PASSLOC, this, b, this->aStoreAt, ptrValue, value));
}

Value *
BaseExtension::LoadField(LOCATION, Builder *b, const FieldType *fieldType, Value *structValue) {
    assert(structValue->type()->isStruct());
    assert(fieldType->owningStruct() == structValue->type());
    Value * result = createValue(b, fieldType->type());
    addOperation(b, new Op_LoadField(PASSLOC, this, b, this->aLoadField, result, fieldType, structValue));
    return result;
}

void
BaseExtension::StoreField(LOCATION, Builder *b, const FieldType *fieldType, Value *structValue, Value *value) {
    assert(structValue->type()->isStruct());
    assert(fieldType->owningStruct() == structValue->type());
    addOperation(b, new Op_StoreField(PASSLOC, this, b, this->aStoreField, fieldType, structValue, value));
}

Value *
BaseExtension::LoadFieldAt(LOCATION, Builder *b, const FieldType *fieldType, Value *pStruct) {
    assert(pStruct->type()->isPointer());
    const Type *structType = (static_cast<const PointerType *>(pStruct->type()))->BaseType();
    assert(fieldType->owningStruct() == structType);
    Value * result = createValue(b, fieldType->type());
    addOperation(b, new Op_LoadFieldAt(PASSLOC, this, b, this->aLoadFieldAt, result, fieldType, pStruct));
    return result;
}

void
BaseExtension::StoreFieldAt(LOCATION, Builder *b, const FieldType *fieldType, Value *pStruct, Value *value) {
    assert(pStruct->type()->isPointer());
    const Type *structType = (static_cast<const PointerType *>(pStruct->type()))->BaseType();
    assert(fieldType->owningStruct() == structType);
    addOperation(b, new Op_StoreFieldAt(PASSLOC, this, b, this->aStoreFieldAt, fieldType, pStruct, value));
}

Value *
BaseExtension::CreateLocalArray(LOCATION, Builder *b, Literal *numElements, const PointerType *pElementType) {
   assert(numElements->type()->isInteger());
   Value * result = createValue(b, pElementType);
   const Type *elementType = pElementType->BaseType();
   // assert concrete type
   addOperation(b, new Op_CreateLocalArray(PASSLOC, this, b, this->aCreateLocalArray, result, numElements, pElementType));
   return result;
}

Value *
BaseExtension::CreateLocalStruct(LOCATION, Builder *b, const PointerType *pStructType) {
    const Type *baseType = pStructType->BaseType();
    assert(baseType->isStruct());
    const StructType *structType = static_cast<const StructType *>(baseType);
    Value * result = createValue(b, pStructType);
    addOperation(b, new Op_CreateLocalStruct(PASSLOC, this, b, this->aCreateLocalStruct, result, structType));
    return result;
}

Value *
BaseExtension::IndexAt(LOCATION, Builder *b, Value *base, Value *index) {
    const Type *pElementType = base->type();
    assert(pElementType->isPointer());
    Value *result = createValue(b, pElementType);
    addOperation(b, new Op_IndexAt(PASSLOC, this, b, aIndexAt, result, base, index));
    return result;
}

//
// Pseudo operations
//

Location *
BaseExtension::SourceLocation(LOCATION, Builder *b, std::string func)
   {
   Location *loc = new Location(b->comp(), func, "");
   b->setLocation(loc);
   return loc;
   }

Location *
BaseExtension::SourceLocation(LOCATION, Builder *b, std::string func, std::string lineNumber)
   {
   Location *loc = new Location(b->comp(), func, lineNumber);
   b->setLocation(loc);
   return loc;
   }

Location *
BaseExtension::SourceLocation(LOCATION, Builder *b, std::string func, std::string lineNumber, int32_t bcIndex)
   {
   Location *loc = new Location(b->comp(), func, lineNumber, bcIndex);
   b->setLocation(loc);
   return loc;
   }

const FunctionType *
BaseExtension::DefineFunctionType(LOCATION, std::string name, const Type *returnType, int32_t numParms, const Type **parmTypes) {
   const FunctionType *f = FunctionType::create(PASSLOC, this, name, returnType, numParms, parmTypes);
   // should register it somewhere!
   return f;
}

CompileResult
BaseExtension::jb1cgCompile(Compilation *comp) {
   return _compiler->compile(comp, _jb1cgStrategyID);
}

#if 0
Value *
BuilderBase::CoercePointer(Type * t, Value * v)
   {
   if (!(v->type()->isPointer() || v->type() == Address) || !t->isPointer())
      creationError(aCoercePointer, "type", t, "value", v);

   Value * result = Value::create(self(), t);
   add(OMR::JitBuilder::CoercePointer::create(self(), result, t, v));
   return result;
   }

Value *
BuilderBase::Add (Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aAdd, left, right);
   if (!returnType)
      creationError(aAdd, "left", left, "right", right);

   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::Add::create(self(), result, left, right));
   return result;
   }

Value *
BuilderBase::Sub (Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aSub, left, right);
   if (!returnType)
      creationError(aSub, "left", left, "right", right);

   Value * result = Value::create(self(), returnType);

   add(OMR::JitBuilder::Sub::create(self(), result, left, right));
   return result;
   }

Value *
BuilderBase::Mul (Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aMul, left, right);
   if (!returnType)
      creationError(aMul, "left", left, "right", right);

   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::Mul::create(self(), result, left, right));
   return result;
   }

Value *
BuilderBase::IndexAt (Type * type, Value * base, Value * index)
   {
   Type *returnType = dict()->producedType(aIndexAt, base, index);
   if (!returnType)
      creationError(aIndexAt, "type", type, "base", base, "index", index);

   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::IndexAt::create(self(), result, type, base, index));
   return result;
   }

Value *
BuilderBase::Load(Symbol *local)
   {
   Type *returnType = local->type();
   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::Load::create(self(), result, local));
   return result;
   }

Value *
BuilderBase::Load (std::string name)
   {
   Symbol *local = _fb->getSymbol(name);
   if (!local)
      creationError(aLoad, "localName", name);
   return Load(local);
   }

Value *
BuilderBase::LoadAt (Type * type, Value * address)
   {
   Type *returnType = dict()->producedType(aLoadAt, address);
   if (!returnType)
      creationError(aLoadAt, "type", type, "address", address);

   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::LoadAt::create(self(), result, type, address));
   return result;
   }

Value *
BuilderBase::LoadField (std::string structName, std::string fieldName, Value * structBase)
   {
   StructType *structType = dict()->LookupStruct(structName);
   FieldType *fieldType = structType->LookupField(fieldName);
   assert(fieldType);
   return LoadField(fieldType, structBase);
   }

Value *
BuilderBase::LoadField (FieldType *fieldType, Value *structBase)
   {
   StructType *structType = fieldType->owningStruct();
   Type *returnType = dict()->producedType(aLoadField, fieldType, structBase);
   if (!returnType)
      creationError(aLoadField, "struct", structType->name(), "field", fieldType->name(), "base", structBase);

   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::LoadField::create(self(), result, fieldType, structBase));
   return result;
   }

Value *
BuilderBase::LoadIndirect (std::string structName, std::string fieldName, Value * pStructBase)
   {
   StructType *structType = dict()->LookupStruct(structName);
   FieldType *fieldType = structType->LookupField(fieldName);
   assert(fieldType);
   return LoadIndirect(fieldType, pStructBase);
   }

Value *
BuilderBase::LoadIndirect (FieldType *fieldType, Value *pStructBase)
   {
   StructType *structType = fieldType->owningStruct();
   Type *returnType = dict()->producedType(aLoadIndirect, fieldType, pStructBase);
   if (!returnType)
      creationError(aLoadIndirect, "struct", structType->name(), "field", fieldType->name(), "basePtr", pStructBase);

   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::LoadIndirect::create(self(), result, fieldType, pStructBase));
   return result;
   }

void
BuilderBase::Store(Symbol * local, Value * value)
   {
   add(OMR::JitBuilder::Store::create(self(), local, value));
   }
   
void
BuilderBase::Store (std::string name, Value * value)
   {
   Symbol *local = fb()->getSymbol(name);
   if (local == NULL)
      {
      fb()->DefineLocal(name, value->type());
      local = fb()->getSymbol(name);
      }
   add(OMR::JitBuilder::Store::create(self(), local, value));
   }

void
BuilderBase::StoreAt (Value * address, Value * value)
   {
   Type *returnType = dict()->producedType(aStoreAt, address, value);
   if (returnType != NoType)
      creationError(aStoreAt, "address", address, "value", value);

   add(OMR::JitBuilder::StoreAt::create(self(), address, value));
   }

void
BuilderBase::StoreField (std::string structName, std::string fieldName, Value * structBase, Value *value)
   {
   StructType *structType = dict()->LookupStruct(structName);
   FieldType *fieldType = structType->LookupField(fieldName);
   assert(fieldType);
   return StoreField(fieldType, structBase, value);
   }

void
BuilderBase::StoreField (FieldType *fieldType, Value *structBase, Value *value)
   {
   StructType *structType = fieldType->owningStruct();
   Type *returnType = dict()->producedType(aStoreField, fieldType, structBase, value);
   if (structType != fieldType->owningStruct() || returnType != NoType)
      creationError(aStoreField, "struct", structType->name(), "field", fieldType->name(), "base", structBase, "value", value);

   add(OMR::JitBuilder::StoreField::create(self(), fieldType, structBase, value));
   }

void
BuilderBase::StoreIndirect (std::string structName, std::string fieldName, Value * pStructBase, Value *value)
   {
   StructType *structType = dict()->LookupStruct(structName);
   FieldType *fieldType = structType->LookupField(fieldName);
   assert(fieldType);
   return StoreIndirect(fieldType, pStructBase, value);
   }

void
BuilderBase::StoreIndirect (FieldType *fieldType, Value *pStructBase, Value *value)
   {
   StructType *structType = fieldType->owningStruct();
   Type *returnType = dict()->producedType(aStoreIndirect, fieldType, pStructBase, value);
   if (structType != fieldType->owningStruct() || returnType != NoType)
      creationError(aStoreIndirect, "struct", structType->name(), "field", fieldType->name(), "basePtr", pStructBase, "value", value);

   add(OMR::JitBuilder::StoreIndirect::create(self(), fieldType, pStructBase, value));
   }

void
BuilderBase::AppendBuilder(Builder * b)
   {
   Operation * appendBuilderOp = OMR::JitBuilder::AppendBuilder::create(self(), b);
   add(appendBuilderOp);
   b->setBoundness(May)
    ->setBound(appendBuilderOp)
    ->setBoundness(Must);
   _controlReachesEnd = true; // AppendBuilder establishes a label so control can now reach the end of this builder even if earlier it could not
   }

Value *
BuilderBase::Call(Value *func, int32_t numArgs, ...)
   {
   assert(func->type()->isFunction());

   va_list args ;
   va_start(args, numArgs);
   FunctionType *function = static_cast<FunctionType *>(func->type());
   Type *returnType = dict()->producedType(function, numArgs, args);
   if (returnType == NULL)
      creationError(aCall, "functionType", func, numArgs, args);
   va_end(args);

   Value * result = NULL;
   if (returnType != NoType)
      result = Value::create(self(), returnType);
   va_list args2 ;
   va_start(args2, numArgs);
   Operation * callOp = OMR::JitBuilder::Call::create(self(), result, func, numArgs, args2);
   add(callOp);
   va_end(args2);
   return result;
   }

Value *
BuilderBase::Call(Value *func, int32_t numArgs, Value **args)
   {
   assert(func->type()->isFunction());
   FunctionType *function = static_cast<FunctionType *>(func->type());
   Type *returnType = dict()->producedType(function, numArgs, args);
   if (returnType == NULL)
      creationError(aCall, "functionType", func, numArgs, args);

   Value * result = NULL;
   if (returnType != NoType)
      result = Value::create(self(), returnType);
   Operation * callOp = OMR::JitBuilder::Call::create(self(), result, func, numArgs, args);
   add(callOp);
   return result;
   }

void
BuilderBase::Goto(Builder * b)
   {
   Operation * gotoOp = OMR::JitBuilder::Goto::create(self(), b);
   add(gotoOp);
   _controlReachesEnd = false; // Goto definitely leaves this builder object
   }

void
BuilderBase::IfCmpGreaterThan(Builder * gtBuilder, Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aIfCmpGreaterThan, left, right);
   if (!returnType || returnType != NoType)
      creationError(aIfCmpGreaterThan, "left", left, "right", right);
   add(OMR::JitBuilder::IfCmpGreaterThan::create(self(), gtBuilder, left, right));
   gtBuilder->setTarget()
            ->setBoundness(Cant);
   }

void
BuilderBase::IfCmpLessThan(Builder * ltBuilder, Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aIfCmpLessThan, left, right);
   if (!returnType || returnType != NoType)
      creationError(aIfCmpLessThan, "left", left, "right", right);
   add(OMR::JitBuilder::IfCmpLessThan::create(self(), ltBuilder, left, right));
   ltBuilder->setTarget()
            ->setBoundness(Cant);
   }

void
BuilderBase::IfCmpGreaterOrEqual(Builder * goeBuilder, Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aIfCmpGreaterOrEqual, left, right);
   if (!returnType || returnType != NoType)
      creationError(aIfCmpGreaterOrEqual, "left", left, "right", right);
   add(OMR::JitBuilder::IfCmpGreaterOrEqual::create(self(), goeBuilder, left, right));
   goeBuilder->setTarget()
             ->setBoundness(Cant);
   }

void
BuilderBase::IfCmpLessOrEqual(Builder * loeBuilder, Value * left, Value * right)
   {
   Type *returnType = dict()->producedType(aIfCmpLessOrEqual, left, right);
   if (!returnType || returnType != NoType)
      creationError(aIfCmpLessOrEqual, "left", left, "right", right);
   add(OMR::JitBuilder::IfCmpLessOrEqual::create(self(), loeBuilder, left, right));
   loeBuilder->setTarget()
             ->setBoundness(Cant);
   }

void
BuilderBase::IfThenElse(Builder * thenB, Builder * elseB, Value * cond)
   {
   if (thenB && thenB->boundness() == Cant)
      creationError(aIfThenElse, "Operation invalid because thenB builder cannot be bound");
   if (elseB && elseB->boundness() == Cant)
      creationError(aIfThenElse, "Operation invalid because elseB builder cannot be bound");

   Operation *ifThenElseOp = OMR::JitBuilder::IfThenElse::create(self(), thenB, elseB, cond);
   add(ifThenElseOp);
   if (thenB)
      thenB->setTarget()
           ->setBoundness(May)
           ->setBound(ifThenElseOp)
           ->setBoundness(Must);
   if (elseB)
      elseB->setTarget()
           ->setBoundness(May)
           ->setBound(ifThenElseOp)
           ->setBoundness(Must);
   }

void
BuilderBase::ForLoopUp(std::string loopVar, Builder * body, Value * initial, Value * end, Value * bump)
   {
   LocalSymbol *loopSym = NULL;
   Symbol *sym = fb()->getSymbol(loopVar);
   if (sym && sym->isLocal())
      loopSym = static_cast<LocalSymbol *>(sym);
   else
      loopSym = fb()->DefineLocal(loopVar, initial->type());
   ForLoopUp(loopSym, body, initial, end, bump);
   }

void
BuilderBase::ForLoopUp(LocalSymbol *loopSym, Builder * body, Value * initial, Value * end, Value * bump)
   {
   Type *returnType = dict()->producedType(aForLoop, initial, end, bump);
   if (!returnType || returnType != NoType)
      creationError(aForLoop, "initial", initial, "end", end, "bump", bump);
   if (body->boundness() == Cant)
      creationError(aIfThenElse, "Operation invalid because body builder cannot be bound");

   Operation * forLoopOp = OMR::JitBuilder::ForLoop::create(self(), true, loopSym, body, initial, end, bump);
   add(forLoopOp);
   }

void
BuilderBase::ForLoop(bool countsUp, std::string loopVar, Builder * loopBody, Builder * loopContinue, Builder * loopBreak, Value * initial, Value * end, Value * bump)
   {
   LocalSymbol *loopSym = NULL;
   Symbol *sym = fb()->getSymbol(loopVar);
   if (sym && sym->isLocal())
      loopSym = static_cast<LocalSymbol *>(sym);
   else
      loopSym = fb()->DefineLocal(loopVar, initial->type());
   ForLoop(countsUp, loopSym, loopBody, loopContinue, loopBreak, initial, end, bump);
   }

void
BuilderBase::ForLoop(bool countsUp, LocalSymbol *loopSym, Builder * loopBody, Builder * loopContinue, Builder * loopBreak, Value * initial, Value * end, Value * bump)
   {
   Type *returnType = dict()->producedType(aForLoop, initial, end, bump);
   if (!returnType || returnType != NoType)
      creationError(aForLoop, "initial", initial, "end", end, "bump", bump);
   if (loopBody->boundness() == Cant)
      creationError(aIfThenElse, "Operation invalid because loopBody builder cannot be bound");
   if (loopContinue && loopContinue->boundness() == Cant)
      creationError(aIfThenElse, "Operation invalid because loopContinue builder cannot be bound");
   if (loopBreak && loopBreak->boundness() == Cant)
      creationError(aIfThenElse, "Operation invalid because loopBreak builder cannot be bound");

   Operation * forLoopOp = (OMR::JitBuilder::ForLoop::create(self(), countsUp, loopSym, loopBody, loopContinue, loopBreak, initial, end, bump));
   add(forLoopOp);
   loopBody->setTarget()
           ->setBoundness(May)
           ->setBound(forLoopOp)
           ->setBoundness(Must);
   if (loopContinue)
      loopContinue->setBoundness(May)
                  ->setBound(forLoopOp)
                  ->setBoundness(Must);
   if (loopBreak)
      loopBreak->setBoundness(May)
               ->setBound(forLoopOp)
               ->setBoundness(Must);
   }

void
BuilderBase::Switch(Value * selector, Builder * defaultBuilder, int numCases, Case ** cases)
   {
   Type *returnType = dict()->producedType(aSwitch, selector);
   if (!returnType || returnType != NoType)
      creationError(aSwitch, "selector", selector);
   Operation *switchOp = OMR::JitBuilder::Switch::create(self(), selector, defaultBuilder, numCases, cases);
   add(switchOp);
   if (defaultBuilder)
      defaultBuilder->setTarget()
                    ->setBoundness(May)
                    ->setBound(switchOp)
                    ->setBoundness(Must);
   for (auto c=0;c < numCases;c++)
      {
      Case *thisCase = cases[c];
      Builder *b = thisCase->builder();
      b->setTarget()
       ->setBoundness(May)
       ->setBound(switchOp)
       ->setBoundness(Must);
      }
   }

void
BuilderBase::Return()
   {
   if (_fb->getReturnType() != NoType)
      creationError(aReturn, "expected type", _fb->getReturnType(), "returned type", NoType);

   add(OMR::JitBuilder::Return::create(self()));
   if (boundness() == Must)
      creationError(aReturn, "Operation invalid because target builder is bound");
   setBoundness(Cant);
   }

void
BuilderBase::Return(Value *v)
   {
   Type *returnType = dict()->producedType(aReturn, v);
   if (v->type() != _fb->getReturnType() || !returnType || returnType != NoType)
      creationError(aReturn, "expected type", _fb->getReturnType(), "returned type", v->type());

   add(OMR::JitBuilder::Return::create(self(), v));
   if (boundness() == Must)
      creationError(aReturn, "Operation invalid because target builder is bound");
   setBoundness(Cant);
   }

Value *
BuilderBase::CreateLocalArray(int32_t numElements, Type *elementType)
   {
   Type *returnType = dict()->PointerTo(elementType);
   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::CreateLocalArray::create(self(), result, numElements, elementType));
   return result;
   }

Value *
BuilderBase::CreateLocalStruct(Type *structType)
   {
   Type *returnType = dict()->PointerTo(structType);
   Value * result = Value::create(self(), returnType);
   add(OMR::JitBuilder::CreateLocalStruct::create(self(), result, structType));
   return result;
   }

#endif

} // namespace Base
} // namespace JitBuilder
} // namespace OMR

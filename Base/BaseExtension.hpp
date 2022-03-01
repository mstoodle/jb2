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

#ifndef BASEEXTENSION_INCL
#define BASEEXTENSION_INCL

#include <stdint.h>
#include "CreateLoc.hpp"
#include "Extension.hpp"
#include "IDs.hpp"
#include "SemanticVersion.hpp"
#include "typedefs.hpp"


namespace OMR {
namespace JitBuilder {

class Compilation;
class FieldType;
class FunctionType;
class Literal;
class Location;
class OperationCloner;
class OperationReplacer;
class PointerType;
class StructType;
class UnionType;
class Value;

namespace Base {

class NoTypeType;
class IntegerType;
class Int8Type;
class Int16Type;
class Int32Type;
class Int64Type;
class Float32Type;
class Float64Type;
class AddressType;
class PointerType;
class FieldType;
class StructType;
class UnionType;
class FunctionType;

class BaseExtension : public Extension {

    public:
    BaseExtension(Compiler *compiler);
    virtual ~BaseExtension();

    static const std::string NAME;
    static const MajorID BASEEXT_MAJOR=0;
    static const MinorID BASEEXT_MINOR=1;
    static const PatchID BASEEXT_PATCH=0;

    // 4 == LocalSymbol, ParameterSymbol, FunctionSymbol, FieldSymbol
    uint32_t numSymbolTypes() const { return 4; }

    virtual const SemanticVersion * semver() const {
        return &version;
    }

    //
    // Types
    //

    const NoTypeType *NoType;
    const Int8Type *Int8;
    const Int16Type *Int16;
    const Int32Type *Int32;
    const Int64Type *Int64;
    const Float32Type *Float32;
    const Float64Type *Float64;
    const AddressType *Address;
    const IntegerType *Word;

    PointerType * PointerTo(LOCATION, Type * baseType);
    StructType *LookupStruct(LOCATION, std::string name);
    StructType * DefineStruct(LOCATION, std::string structName, size_t size=0);
    UnionType * DefineUnion(LOCATION, std::string unionName);
    //FieldType * DefineField(std::string structName, std::string fieldName, Type * fieldType, size_t offset) { // deprecated
    //    return DefineField(LookupStruct(structName), Literal::create(this, fieldName), fieldType, offset);
    //}
    //FieldType * DefineField(StructType *structType, std::string fieldName, Type * fieldType, size_t offset) {
    //    return DefineField(structType, Literal::create(this, fieldName), fieldType, offset);
    //}
    //FieldType * DefineField(LOCATION, StructType *structType, Literal *fieldName, Type * fieldType, size_t offset);
    //void CloseStruct(LOCATION, StructType *structType);
    //void CloseUnion(LOCATION, UnionType *unionType);
    const FunctionType *DefineFunctionType(LOCATION, std::string name, const Type *returnType, int32_t numParms, const Type **parmTypes);

    //
    // Actions
    //

    // Const actions
    const ActionID aConstInt8;
    const ActionID aConstInt16;
    const ActionID aConstInt32;
    const ActionID aConstInt64;
    const ActionID aConstFloat32;
    const ActionID aConstFloat64;
    const ActionID aConstAddress;

    // Control actions
    const ActionID aReturn;

    //
    // Operations
    //

    // Constant operations
    Value * ConstInt8(LOCATION, Builder *b, int8_t v);
    Value * ConstInt16(LOCATION, Builder *b, int16_t v);
    Value * ConstInt32(LOCATION, Builder *b, int32_t v);
    Value * ConstInt64(LOCATION, Builder *b, int64_t v);
    Value * ConstFloat32(LOCATION, Builder *b, float v);
    Value * ConstFloat64(LOCATION, Builder *b, double v);
    Value * ConstAddress(LOCATION, Builder *b, void *v);
    //Deprecated:
    //Value * ConstFloat(LOCATION, Builder *b, float v) { return ConstFloat32(PASSLOC, b, v); } // deprecated
    //Value * ConstDouble(LOCATION, Builder *b, double v) { return ConstFloat64(PASSLOC, b, v); } // deprecated

    // Control operations
    void Return(LOCATION, Builder *b);
    void Return(LOCATION, Builder *b, Value *v=NULL);

    // Pseudo operations
    Location * SourceLocation(LOCATION, Builder *b, std::string func);
    Location * SourceLocation(LOCATION, Builder *b, std::string func, std::string lineNumber);
    Location * SourceLocation(LOCATION, Builder *b, std::string func, std::string lineNumber, int32_t bcIndex);

    CompileResult jb1cgCompile(Compilation *comp);

    protected:
    StrategyID _jb1cgStrategyID;

    static const SemanticVersion version;
};

} // namespace Base
} // namespace JitBuilder
} // namespace OMR

#endif // defined(BASEEXTENSION_INCL)

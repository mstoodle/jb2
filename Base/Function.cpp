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

#include <string>
#include <vector>
#include "Builder.hpp"
//#include "Debugger.hpp"
#include "Compiler.hpp"
#include "Function.hpp"
#include "FunctionCompilation.hpp"
#include "JB1MethodBuilder.hpp"
#include "JitBuilder.hpp"
#include "NativeCallableContext.hpp"
#include "Operation.hpp"
#include "TextWriter.hpp"
#include "TypeDictionary.hpp"
#include "Value.hpp"


namespace OMR {
namespace JitBuilder {
namespace Base {

FunctionSymbolIterator Function::endFunctionIterator;

Function::Function(Compiler *compiler) // , int32_t numEntries)
    : _compiler(compiler)
    , _ext(compiler->lookupExtension<BaseExtension>())
    , _outerFunction(NULL)
    , _dict(new TypeDictionary(compiler, "Function", compiler->dict()))
    , _comp(new FunctionCompilation(compiler, this, _dict))
    , _nativeContext(new NativeCallableContext(_comp))
    , _numEntryPoints(1)
    , _entryPoints(new Builder *[1])
    , _nativeEntryPoints(new void *[1])
    , _debugEntryPoints(new void *[1]) {

    _entryPoints[0] = Builder::create(_comp, _nativeContext); //, "Entry");
    _ext->SourceLocation(LOC, _entryPoints[0], ""); // make sure everything has a location; by default BCIndex is 0
}

Function::Function(Function *outerFunc) // , int32_t numEntries)
    : _compiler(outerFunc->_compiler)
    , _ext(_compiler->lookupExtension<BaseExtension>())
    , _outerFunction(outerFunc)
    , _dict(outerFunc->dict())
    , _comp(outerFunc->comp())
    , _nativeContext(new NativeCallableContext(_comp, outerFunc->_nativeContext))
    , _numEntryPoints(1)
    , _entryPoints(new Builder *[1])
    , _nativeEntryPoints(new void *[1])
    , _debugEntryPoints(new void *[1]) {

    _entryPoints[0] = Builder::create(_comp, _nativeContext); //, "Entry");
    _ext->SourceLocation(LOC, _entryPoints[0], ""); // make sure everything has a location; by default BCIndex is 0
}

Function::~Function() {
    for (auto e=0;e < _numEntryPoints;e++)
        delete _entryPoints[e];
    delete[] _debugEntryPoints;
    delete[] _nativeEntryPoints;
    delete[] _entryPoints;
    delete _nativeContext;
    if (_outerFunction == NULL) {
        delete _comp;
        delete _dict;
    }
}

Config *
Function::config() const {
    return _comp->config();
}

int32_t
Function::numLocals() const {
    return _nativeContext->_locals.size() + _nativeContext->_parameters.size();
}

void
Function::DefineName(std::string name) {
    _givenName = name;
}

void
Function::DefineFile(std::string file) {
    _fileName = file;
}

void
Function::DefineLine(std::string line) {
    _lineNumber = line;
}

ParameterSymbol *
Function::DefineParameter(std::string name, const Type * type) {
    return this->_nativeContext->DefineParameter(name, type);
}

void
Function::DefineReturnType(const Type * type) {
    this->_nativeContext->DefineReturnType(type);
}

LocalSymbol *
Function::DefineLocal(std::string name, const Type * type) {
    return _nativeContext->DefineLocal(name, type);
}

FunctionSymbol *
Function::DefineFunction(LOCATION,
                         std::string name,
                         std::string fileName,
                         std::string lineNumber,
                         void *entryPoint,
                         const Type *returnType,
                         int32_t numParms,
                         ...) {

    const Type **parmTypes = new const Type*[numParms];
    va_list parms;
    va_start(parms, numParms);
   
    for (int32_t p=0;p < numParms;p++)
        parmTypes[p] = (const Type *) va_arg(parms, const Type *);
    va_end(parms);

    return internalDefineFunction(PASSLOC, name, fileName, lineNumber, entryPoint, returnType, numParms, parmTypes);
}

FunctionSymbol *
Function::DefineFunction(LOCATION,
                         std::string name,
                         std::string fileName,
                         std::string lineNumber,
                         void *entryPoint,
                         const Type *returnType,
                         int32_t numParms,
                         const Type **parmTypes) {

    // copy parameter types so don't have to force caller to keep the parmTypes array alive
    const Type **copiedParmTypes = new const Type*[numParms];
    for (int32_t p=0;p < numParms;p++)
        copiedParmTypes[p] = parmTypes[p];

    return internalDefineFunction(PASSLOC, name, fileName, lineNumber, entryPoint, returnType, numParms, copiedParmTypes);
}

// maybe move to Compilation?
FunctionSymbol *
Function::internalDefineFunction(LOCATION,
                                 std::string name,
                                 std::string fileName,
                                 std::string lineNumber,
                                 void *entryPoint,
                                 const Type *returnType,
                                 int32_t numParms,
                                 const Type **parmTypes) {

    const FunctionType *type = _ext->DefineFunctionType(PASSLOC, _comp, returnType, numParms, parmTypes);
    FunctionSymbol *sym = new FunctionSymbol(type, name, fileName, lineNumber, entryPoint);
    _functions.push_back(sym);
    return sym;
}

const PointerType *
Function::PointerTo(LOCATION, const Type *baseType) {
    return this->_ext->PointerTo(PASSLOC, _comp, baseType);
}

LocalSymbolIterator
Function::LocalsBegin() const {
    return _nativeContext->LocalsBegin();
}

LocalSymbolIterator
Function::LocalsEnd() const {
    return _nativeContext->LocalsEnd();
}

ParameterSymbolIterator
Function::ParametersBegin() const {
    return _nativeContext->ParametersBegin();
}

ParameterSymbolIterator
Function::ParametersEnd() const {
    return _nativeContext->ParametersEnd();
}

ParameterSymbolVector
Function::ResetParameters() {
    return _nativeContext->ResetParameters();
}

LocalSymbolVector
Function::ResetLocals() {
    return _nativeContext->ResetLocals();
}

LocalSymbol *
Function::LookupLocal(std::string name) {
    for (LocalSymbolIterator lIt = _nativeContext->LocalsBegin(); lIt != _nativeContext->LocalsEnd(); lIt++) {
        LocalSymbol * local = *lIt;
        if (local->name() == name)
            return local;
    }

    for (ParameterSymbolIterator pIt = _nativeContext->ParametersBegin(); pIt != _nativeContext->ParametersEnd(); pIt++) {
        ParameterSymbol * parameter = *pIt;
        if (parameter->name() == name)
            return parameter;
    }

    return NULL;
}

FunctionSymbolVector
Function::ResetFunctions() {
    FunctionSymbolVector prev = _functions;
    _functions.clear();
    return prev;
}

Symbol *
Function::getSymbol(std::string name) {
    LocalSymbol *localSym = LookupLocal(name);
    if (localSym)
        return localSym;

    for (FunctionSymbolIterator fIt = FunctionsBegin(); fIt != FunctionsEnd(); fIt++) {
        FunctionSymbol * function = *fIt;
        if (function->name() == name)
            return function;
    }

    return NULL;
}

const Type *
Function::returnType() const {
    return _nativeContext->returnType();
}

int32_t
Function::numReturnValues() const {
    return (_nativeContext->returnType() == _ext->NoType) ? 0 : 1; /* 1 for now */
}

void
Function::addInitialBuildersToWorklist(BuilderWorklist & worklist) {
    for (int i=0;i < this->_numEntryPoints;i++)
       worklist.push_back(this->_entryPoints[i]);
}

void
Function::write(TextWriter &w) const {
    w.indent() << "[ Function" /*<< _id*/ << w.endl();
    w.indentIn();

    w.indent() << "[ name " << name() << " ]" << w.endl();
    w.indent() << "[ origin " << fileName() + "::" + lineNumber() << " ]" << w.endl();
    w.indent() << "[ returnType " << returnType() << "]" << w.endl();
    for (ParameterSymbolIterator paramIt = ParametersBegin();paramIt != ParametersEnd(); paramIt++) {
        const ParameterSymbol *parameter = *paramIt;
        w.indent() << "[ parameter " << parameter << " ]" << w.endl();
    }
    for (LocalSymbolIterator localIt = LocalsBegin();localIt != LocalsEnd();localIt++) {
        const LocalSymbol *local = *localIt;
        w.indent() << "[ local " << local << " ]" << w.endl();
    }
    for (FunctionSymbolIterator functionIt = FunctionsBegin();functionIt != FunctionsEnd();functionIt++) {
        const FunctionSymbol *function = *functionIt;
        w.indent() << "[ function " << function << " ]" << w.endl();
    }
    w.indent() << "[ entryPoint " << builderEntry() << " ]" << w.endl();
    w.indentOut();
    w.indent() << "]" << w.endl();
}

void
Function::constructJB1Function(JB1MethodBuilder *j1mb) {
    j1mb->FunctionName(name());
    j1mb->FunctionFile(fileName());
    j1mb->FunctionLine(lineNumber());
    j1mb->FunctionReturnType(returnType());

    for (ParameterSymbolIterator paramIt = ParametersBegin();paramIt != ParametersEnd(); paramIt++) {
        const ParameterSymbol *parameter = *paramIt;
        j1mb->Parameter(parameter->name(), parameter->type());
    }
    for (LocalSymbolIterator localIt = LocalsBegin();localIt != LocalsEnd();localIt++) {
        const LocalSymbol *symbol = *localIt;
        j1mb->Local(symbol->name(), symbol->type());
    }
    for (FunctionSymbolIterator fnIt = FunctionsBegin();fnIt != FunctionsEnd();fnIt++) {
        const FunctionSymbol *fSym = *fnIt;
        const FunctionType *fType = fSym->functionType();
        j1mb->DefineFunction(fSym->name(),
                             fSym->fileName(),
                             fSym->lineNumber(),
                             fSym->entryPoint(),
                             fType->returnType(),
                             fType->numParms(),
                             fType->parmTypes());
     }
}

void
Function::jbgenProlog(JB1MethodBuilder *j1mb) {
    j1mb->EntryPoint(_entryPoints[0]);
}

CompilerReturnCode
Function::Compile(TextWriter *logger, StrategyID strategy) {
    _comp->setLogger(logger);
    if (strategy == NoStrategy)
        return _ext->jb1cgCompile(_comp);
    else
        return _compiler->compile(_comp, strategy);
}

#if 0
void *
Function::internalDebugger(int32_t *returnCode)
   {
   _debuggerObject = new OMR::JitBuilder::Debugger(this);
   _debugEntryPoint = _debuggerObject->createDebugger(returnCode);
   return _debugEntryPoint;
   }
#endif

} // namespace Base
} // namespace JitBuilder
} // namespace OMR


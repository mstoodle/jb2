/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
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

#ifndef FUNCTIONCOMPILATION_INCL
#define FUNCTIONCOMPILATION_INCL

#include <stdint.h>
#include <string>
#include <vector>
#include "Compilation.hpp"
#include "typedefs.hpp"

namespace OMR {
namespace JitBuilder {

class Compiler;
class Config;
class JB1MethodBuilder;
class TextWriter;
class TypeDictionary;

namespace Base {

class Function;

class FunctionCompilation : public Compilation {
public:
    FunctionCompilation(Compiler *compiler, Function *func, TypeDictionary *dict=NULL, Config *localConfig=NULL)
        : Compilation(compiler, dict, localConfig)
        , _func(func) {
    }
    virtual ~FunctionCompilation() { }

    Function *func() const { return _func; }

    virtual void write(TextWriter & w) const;

    virtual void constructJB1Function(JB1MethodBuilder *j1mb);
    virtual void jbgenProlog(JB1MethodBuilder *j1mb);

    virtual bool buildIL();
    virtual bool ilBuilt() const;
    virtual void setNativeEntryPoint(void *entry, int i=0);

    const PointerType * pointerTypeFromBaseType(const Type * baseType);
    const StructType * structTypeFromName(std::string name);

protected:
    virtual void addInitialBuildersToWorklist(BuilderWorklist & worklist);


    Function *_func;
    std::map<const Type *,const PointerType *> _pointerTypeFromBaseType;
    std::map<std::string,const StructType *> _structTypeFromName;
};

} // namespace FunctionCompilation
} // namespace JitBuilder
} // namespace OMR

#endif // !defined(FUNCTIONCOMPILATION_INCL)


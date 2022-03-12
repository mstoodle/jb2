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

#ifndef COMPILER_INCL
#define COMPILER_INCL

#include <list>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>
#include "Flags.hpp"
#include "IDs.hpp"
#include "typedefs.hpp"

namespace OMR
{
namespace JitBuilder
{

class Compilation;
class Config;
class Extension;
class JB1;
class Pass;
class Platform;
class SemanticVersion;
class Strategy;
class Type;
class TypeDictionary;

class Compiler {
    friend class Extension;
    friend class Pass;
    friend class Strategy;

    typedef std::vector<Pass *> PassChain;
    typedef std::map<PassID, PassChain> PassRegistry;

public:
    Compiler(std::string name, Config *config=NULL);
    virtual ~Compiler();

    CompilerID id() const { return _id; }
    std::string name() const { return _name; }
    Config *config() const { return _config; }
    TypeDictionary *dict() const { return _dict; }

    ExtensionID getExtensionID() { return _nextExtensionID++; }
    void addExtension(Extension *ext);
    template<typename T>
    T *loadExtension(std::string name=T::NAME, SemanticVersion *version=NULL) {
        return static_cast<T *>(internalLoadExtension(name, version));
    }
    bool validateExtension(std::string name) const;
    template<typename T>
    T *lookupExtension(std::string name=T::NAME) { return static_cast<T *>(internalLookupExtension(name)); }

    PassID lookupPass(std::string name);
    CompileResult compile(Compilation *comp, StrategyID strategyID);

    TypeDictionaryID getTypeDictionaryID() { return this->_nextTypeDictionaryID++; }

    const std::string actionName(ActionID a) const {
        assert(a < _nextActionID);
        auto found = _actionNames.find(a);
        assert(found != _actionNames.end());
        return found->second;
    }

    uint8_t platformWordSize() { return 64; } // should test _targetPlatform!

protected:
    Extension *internalLookupExtension(std::string name);
    Extension *internalLoadExtension(std::string name, SemanticVersion *version=NULL);
    Strategy * lookupStrategy(StrategyID id);
    StrategyID addStrategy(Strategy *st);
    PassID addPass(Pass *pass);

    CompilerID _id;
    std::string _name;
    JB1 *_jb1;
    Config *_config;
    bool _myConfig;
    bool _myDict;

    ExtensionID _nextExtensionID;
    std::map<std::string, Extension *> _extensions;

    ActionID assignActionID(std::string name);
    ActionID _nextActionID;
    std::map<ActionID, std::string> _actionNames;

    StrategyID _nextStrategyID;
    std::map<StrategyID, Strategy *> _strategies;

    PassID _nextPassID;
    std::map<std::string, PassID> _registeredPassNames;
    PassRegistry _passRegistry;

    TypeID _nextTypeID;
    std::map<TypeID, Type *> _types;

    TypeDictionaryID _nextTypeDictionaryID;

    Platform *_target;
    Platform *_compiler;

    // needs to follow at least _nextTypeDictionaryID
    TypeDictionary *_dict;

    static CompilerID nextCompilerID;
};

} // namespace JitBuilder
} // namespace OMR

#endif // !defined(COMPILER_INCL)

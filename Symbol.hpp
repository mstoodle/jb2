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

#ifndef SYMBOL_INCL
#define SYMBOL_INCL

#include <cassert>
#include <string>
#include <vector>
#include "IDs.hpp"
#include "KindService.hpp"

namespace OMR {
namespace JitBuilder {

class SymbolDictionary;
class TextWriter;
class Type;

typedef KindService::Kind SymbolKind;

class Symbol {
    friend class SymbolDictionary;

public:
    static Symbol *create(std::string name, const Type * type) { return new Symbol(name, type); }

    std::string name() const { return _name; }
    const Type * type() const { return _type; }
    SymbolID id() const { return _id; }
    virtual SymbolKind kind() const { return SYMBOLKIND; }

    template<typename T>
    bool isExactKind() const {
        return (kindService.isExactMatch(_kind, T::SYMBOLKIND));
    }

    template<typename T>
    bool isKind() const {
        return (kindService.isMatch(_kind, T::SYMBOLKIND));
    }

    template<typename T>
    T *refine() {
        assert(isKind<T>());
        return static_cast<T *>(this);
    }

    virtual void write(TextWriter & w) const;

    static SymbolKind assignSymbolKind(SymbolKind baseKind, std::string name);
    static SymbolKind SYMBOLKIND;

protected:
    Symbol(std::string name, const Type * type)
        : _id(NoSymbol)
        , _kind(SYMBOLKIND)
        , _name(name)
        , _type(type) {

    }
    Symbol(SymbolKind kind, std::string name, const Type * type)
        : _id(NoSymbol)
        , _kind(kind)
        , _name(name)
        , _type(type) {

    }

    void assignID(SymbolID id) {
        // TODO convert to CompilationException
        assert(_id == NoSymbol);
       	assert(id != NoSymbol);
       	_id = id;
    }

    SymbolID _id;
    SymbolKind _kind;
    std::string _name;
    const Type * _type;

    static KindService kindService;
};

} // namespace JitBuilder
} // namespace OMR

#endif // defined(SYMBOL_INCL)


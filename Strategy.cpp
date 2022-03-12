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

#include "Compilation.hpp"
#include "Compiler.hpp"
#include "Pass.hpp"
#include "Strategy.hpp"
#include "TextWriter.hpp"

namespace OMR {
namespace JitBuilder {

Strategy::Strategy(Compiler *compiler, std::string name)
    : _id(NoStrategy)
    , _compiler(compiler)
    , _name(name) {
    _id = compiler->addStrategy(this);
}

Strategy *
Strategy::addPass(Pass *pass) {
    assert(pass->_compiler == _compiler);
    _passes.push_back(pass);
    return this;
}

CompileResult
Strategy::perform(Compilation *comp) {
    for (auto it = _passes.begin(); it != _passes.end(); it++) {
        Pass *pass = *it;

        if (comp->logger()) {
            TextWriter &log = *comp->logger();
            log << "IL before pass " << pass->name() << log.endl();
            log.print(comp);
        }

        CompileResult rc = pass->perform(comp);

        if (rc != CompileSuccessful) {
            if (comp->logger()) {
                TextWriter &log = *comp->logger();
                log << "Final IL" << log.endl();
                log.print(comp);
            }
            return rc;
        }
    }
    if (comp->logger()) {
        TextWriter &log = *comp->logger();
        log << "Final IL" << log.endl();
        log.print(comp);
    }
    return CompileSuccessful;
}

} // namespace JitBuilder
} // namespace OMR
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

#include <climits>
#include <sstream>
#include "Builder.hpp"
#include "Compilation.hpp"
#include "Config.hpp"
#include "Operation.hpp"
#include "TextWriter.hpp"
#include "Transformer.hpp"

namespace OMR {
namespace JitBuilder {

void
Transformer::trace(std::string msg) {
    TextWriter *log = _comp->logger(traceEnabled());
    if (log)
        log->indent() << msg << log->endl();
}

bool
Transformer::performTransformation(Operation * op, Builder * transformed, std::string msg) {
    static int64_t lastTransformation = LLONG_MAX;
    int64_t number = _comp->getTransformationID();
    int64_t lastIndex = _comp->config()->lastTransformationIndex();
    bool succeed = (lastIndex < 0 || number < lastIndex);

    if (traceEnabled()) {
        if (succeed) {
            std::ostringstream oss;
            oss << "( " << number << " ) Transformation: " << msg;
            trace(oss.str());
            TextWriter *log= _comp->logger(traceEnabled());
            LOG_INDENT_REGION(log) {
                if (log) log->indentIn();
                if (log) log->print(op);
                if (log) log->indent() << "Replaced with operations from : " << log->endl();
                if (log) log->print(transformed);
                if (log) log->indentOut();
            }
            LOG_OUTDENT
        }
        else
            trace("Transformation not applied: " + msg);
    }

    return succeed;
}

void
Transformer::visitOperations(Builder *b, std::vector<bool> & visited, BuilderWorklist & worklist) {
    TextWriter * log = _comp->logger(traceEnabled());

    for (OperationIterator opIt = b->OperationsBegin(); opIt != b->OperationsEnd(); opIt++) {
        Operation * op = *opIt;

        if (log) {
            log->indent() << std::string("Visit ");
            log->print(op);
        }

        Builder *transformation = transformOperation(op);
        if (transformation != NULL) {
            if (performTransformation(op, transformation)) {
                opIt = b->operations().erase(opIt); // remove the operation we just transformed

                bool replaceWithBuilder=false;
                if (false && replaceWithBuilder) {
                    #ifdef IMPLEMENTED_APPENDBUILDER
                    // replace the current operation with the Builder object containing its transformation
                    //    could also be done immutably by generating a new array of Operations
                    //    and returning new Builder but let's do in place for now
                    opIt = b->operations().insert(opIt, AppendBuilder::create(b, transformation));
                    #endif
                }
                else {
                    // replace the operation with the operations inside the builder
                    // removing the builder object means each operation's parent changes
                    for (OperationIterator it = transformation->OperationsBegin(); it != transformation->OperationsEnd(); it++) {
                        Operation * op = *it;
                        op->setParent(b);
                    }
                    opIt = b->operations().insert(opIt,
                                                  transformation->OperationsBegin(),
                                                  transformation->OperationsEnd());
                    for (OperationIterator it = transformation->OperationsBegin(); it != transformation->OperationsEnd(); it++) {
                        // scan transformed operations for builder objects we need to traverse
                        Operation *op = *it;
                        for (BuilderIterator bIt = op->BuildersBegin(); bIt != op->BuildersEnd(); bIt++) {
                            Builder *inner_b = *bIt;
                            if (inner_b && !visited[inner_b->id()])
                                worklist.push_front(inner_b);
                        }
                        opIt++; // skip over inserted operations
                    }
                    opIt--; // compensate for increment on loop back edge
                }

                // operation has changed, but any internal builders will be found by iterating
                // over the transformed operations we just inserted
            }
        }
        else {
            for (BuilderIterator bIt = op->BuildersBegin(); bIt != op->BuildersEnd(); bIt++) {
                Builder * inner_b = *bIt;
                if (inner_b)
                    worklist.push_front(inner_b);
            }
        }
    }
}

} // namespace JitBuilder
} // namespace OMR


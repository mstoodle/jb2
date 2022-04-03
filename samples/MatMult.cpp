/*******************************************************************************
 * Copyright (c) 2016, 2022 IBM Corp.
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/


#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>

#include "Compiler.hpp"
#include "MatMult.hpp"
#include "TextWriter.hpp"
#include "Base/BaseExtension.hpp"
#include "base/ControlOperations.hpp"
#include "base/Function.hpp"
#include "base/FunctionCompilation.hpp"

#define DO_LOGGING true


MatMult::MatMult(Compiler * compiler, Base::BaseExtension *base)
    : Base::Function(compiler)
    , _base(base)
    , pFloat64(base->PointerTo(LOC, _comp, base->Float64)) {

    DefineLine(LINETOSTR(__LINE__));
    DefineFile(__FILE__);

    DefineName("matmult");

    // C = A * B, all NxN matrices
    _symC = DefineParameter("C", pFloat64);
    _symA = DefineParameter("A", pFloat64);
    _symB = DefineParameter("B", pFloat64);
    _symN = DefineParameter("N", base->Int32);

    DefineReturnType(base->NoType);

    _symSum = DefineLocal("sum", base->Float64);
}

void
MatMult::Store2D(LOCATION,
                 Builder *b,
                 Value *base,
                 Value *first,
                 Value *second,
                 Value *N,
                 Value *value) {
    Value *rowStart = _base->Mul(PASSLOC, b, first, N);
    Value *index = _base->Add(PASSLOC, b, rowStart, second);
    Value *pIndex = _base->IndexAt(PASSLOC, b, base, index);
    _base->StoreAt(PASSLOC, b, pIndex, value);
}

Value *
MatMult::Load2D(LOCATION,
                Builder *b,
                Value *base,
                Value *first,
                Value *second,
                Value *N) {
    Value *rowStart = _base->Mul(PASSLOC, b, first, N);
    Value *index = _base->Add(PASSLOC, b, rowStart, second);
    Value *pIndex = _base->IndexAt(PASSLOC, b, base, index);
    return _base->LoadAt(PASSLOC, b, pIndex);
}

bool
MatMult::buildIL() {
    // marking all locals as defined allows remaining locals to be temps
    // which enables further optimization opportunities particularly for
    //    floating point types
    //AllLocalsHaveBeenDefined();

    Builder *b = builderEntry();

    _base->SourceLocation(LOC, b, "MatMult");
    Value *A = _base->Load(LOC, b, _symA);
    Value *B = _base->Load(LOC, b, _symB);
    Value *C = _base->Load(LOC, b, _symC);
    Value *N = _base->Load(LOC, b, _symN);
    Value *zero = _base->Zero(LOC, _comp, b, _base->Int32);
    Value *one = _base->One(LOC, _comp, b, _base->Int32);

    Base::ForLoopBuilder *iloop=NULL, *jloop=NULL, *kloop=NULL;
    Base::LocalSymbol *symi = DefineLocal("i", _base->Int32);
    iloop = _base->ForLoopUp(LOC, b, symi, zero, N, one); {
        Builder *ibody = iloop->loopBody();
        Value *i = _base->Load(LOC, ibody, iloop->loopVariable());

        Base::LocalSymbol *symj = DefineLocal("j", _base->Int32);
        jloop = _base->ForLoopUp(LOC, ibody, symj, zero, N, one); {
            Builder *jbody = jloop->loopBody();
            Value *j = _base->Load(LOC, jbody, jloop->loopVariable());

            _base->Store(LOC, jbody, _symSum, _base->Zero(LOC, _comp, jbody, _base->Float64));
            Base::LocalSymbol *symk = DefineLocal("k", _base->Int32);
            kloop = _base->ForLoopUp(LOC, jbody, symk, zero, N, one); {
                Builder *kbody = kloop->loopBody();
                Value *k = _base->Load(LOC, kbody, kloop->loopVariable());

                Value *A_ik = Load2D(LOC, kbody, A, i, k, N);
                Value *B_kj = Load2D(LOC, kbody, B, k, j, N);
                Value *prod = _base->Mul(LOC, kbody, A_ik, B_kj);
                Value *oldSum = _base->Load(LOC, kbody, _symSum);
                Value *newSum = _base->Add(LOC, kbody, oldSum, prod);
                _base->Store(LOC, kbody, _symSum, newSum);
            }

            Store2D(LOC, jbody, C, i, j, N, _base->Load(LOC, jbody, _symSum));
        }
    }

    _base->Return(LOC, b);

    return true;
}



void
printMatrix(double *M, int32_t N, const char *name) {
    printf("%s = [\n", name);
    for (int32_t i=0;i < N;i++) {
        printf("      [ %lf", M[i*N]);
        for (int32_t j=1;j < N;j++)
            printf(", %lf", M[i * N + j]);
        printf(" ],\n");
    }
    printf("    ]\n\n");
}

int
main(int argc, char *argv[]) {
    printf("Step 1: Load libjbcore.so\n");
    void *handle = dlopen("libjbcore.so", RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        return -1;
    }

    printf("Step 2: create a Compiler object\n");
    Compiler c("Matrix Multiple Sample Compiler");

    printf("Step 3: Load base extension into Copiler\n");
    Base::BaseExtension *base = c.loadExtension<Base::BaseExtension>("base");
    if (base == NULL) {
        printf("Base extension could not be loaded!\n");
        return -2;
    }

    printf("Step 4: initialize matrices\n");
    const int32_t N=4;
    double A[N*N];
    double B[N*N];
    double C[N*N];
    double D[N*N];
    for (int32_t i=0;i < N;i++) {
        for (int32_t j=0;j < N;j++) {
            A[i*N+j] = 1.0;
            B[i*N+j] = (double)i+(double)j;
            C[i*N+j] = 0.0;
            D[i*N+j] = 0.0;
        }
    }

    printf("Step 5: construct MatMult function\n");
    MatMult func(&c, base);
    //fund.config()
                  //->setReportMemory();
                  //->setTraceBuildIL()
                  //->setTraceReducer()
                  //->setTraceCodeGenerator();

    printf("Step 6: create a Logger and register on the function\n");
    Base::FunctionCompilation *comp = func.comp();
    std::cout.setf(std::ios_base::skipws);
    TextWriter logger(comp, std::cout, std::string("    "));
    TextWriter *log = (DO_LOGGING) ? &logger : NULL;

    printf("Step 7: compile the function\n");
    CompileResult result = func.Compile(log);
    if (result != CompileSuccessful) {
        printf("Compilation was not successful");
        return -3;
    }
    MatMultFunctionType *test = func.nativeEntry<MatMultFunctionType *>(); \

    printf("Step 8: invoke MatMult compiled code\n");
    test(C, A, B, N);

    printMatrix(A, N, "A");
    printMatrix(B, N, "B");
    printMatrix(C, N, "C");

    printf("Compiler freed and JIT unloaded automatically when scope ends!\n");
}
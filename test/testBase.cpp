/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
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

#include <dlfcn.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include "Compiler.hpp"
#include "base/BaseExtension.hpp"
#include "base/Function.hpp"
#include "base/FunctionCompilation.hpp"
#include "TextWriter.hpp"

#if 0
bool internal_initializeJit() {
    return true;
}

void internal_shutdownJit() {
}
#endif

int main(int argc, char** argv) {
    void *handle = dlopen("libjbcore.so", RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        return -1;
    }

   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}


using namespace OMR::JitBuilder;

TEST(BaseExtension, loadExtension) {
    Compiler c("testBase");
    Base::BaseExtension *ext = c.loadExtension<Base::BaseExtension>("base");
    EXPECT_TRUE(ext != NULL) << "Base extension loaded";
}

TEST(BaseExtension, noLoadUnknownExtension) {
    Compiler c("testNotBase");
    Base::BaseExtension *ext = c.loadExtension<Base::BaseExtension>("notbase");
    EXPECT_TRUE(ext == NULL) << "notbase extension correctly could not be loaded";
}

TEST(BaseExtension, checkVersionPass) {
    Compiler c("testBase");
    SemanticVersion v(0,0,0);
    Base::BaseExtension *ext = c.loadExtension<Base::BaseExtension>("base", &v);
    EXPECT_TRUE(ext != NULL) << "Base extension with v(0,0,0) loaded";
}

TEST(BaseExtension, checkVersionFail) {
    Compiler c("testBase");
    SemanticVersion v(1,0,0);
    Base::BaseExtension *ext = c.loadExtension<Base::BaseExtension>("base", &v);
    EXPECT_TRUE(ext == NULL) << "Base extension with v(1,0,0) correctly could not be loaded";
}

#define BASE_FUNC(name,line,file,ext,xtor_code,entry,il_code) \
    class name : public Base::Function { \
    protected: \
        Base::BaseExtension *ext; \
    public: \
        name(Compiler *c, Base::BaseExtension *x) : Base::Function(c), ext(x) { \
            DefineName(#name); \
	    DefineLine(line); \
	    DefineFile(file); \
	    xtor_code \
	} \
        virtual bool buildIL() { \
            Builder *entry = builderEntry(); \
	    il_code \
            return true; \
	} \
    };

BASE_FUNC(TestFunction, "0", "TestFunction.cpp",
    _x, { DefineReturnType(_x->NoType); },
    b, {
        Value *v1 = _x->ConstInt8(LOC, b, 1);
        Value *v2 = _x->ConstInt16(LOC, b, 2);
        Value *v3 = _x->ConstInt32(LOC, b, 3);
        Value *v4 = _x->ConstInt64(LOC, b, 4);
        Value *v5 = _x->ConstFloat32(LOC, b, 5.0);
        Value *v6 = _x->ConstFloat64(LOC, b, 6.0);
        Value *v7 = _x->ConstAddress(LOC, b, NULL);
    })

BASE_FUNC(Return3Function, "0", "Return3.cpp",
    _x, { DefineReturnType(_x->Int32); },
    b, { _x->Return(LOC, b, _x->ConstInt32(LOC, b, 3)); })

TEST(BaseExtension, createTestFunction) {
    Compiler c("testBase");
    Base::BaseExtension *ext = c.loadExtension<Base::BaseExtension>();
    TestFunction func(&c, ext);
    EXPECT_TRUE(func.builderEntry() != NULL) << "Func has a builder entry object";
    EXPECT_EQ(func.numReturnValues(), 0) << "Func has no return value";
    EXPECT_TRUE(func.returnType() == ext->NoType) << "Func has NoTYpe return type";
    Compilation *comp = func.comp();
    EXPECT_TRUE(comp != NULL) << "Func has a compilation object";
    std::cerr.setf(std::ios_base::skipws);
    TextWriter logger(comp, std::cout, std::string("    "));
    comp->setLogger(&logger);
    //func.config()
                 //->setTraceBuildIL()
	         //->setReportMemory()
                 //->setTraceReducer()
                 //->setTraceCodeGenerator()
		 //;
    bool success = func.constructIL();
    EXPECT_TRUE(success) << "TestFunction IL successfully constructed";
    logger.print(comp);

    Return3Function ret3(&c, ext);
    Compilation *comp_ret3 = ret3.comp();
    TextWriter logger_ret3(comp_ret3, std::cout, std::string("    "));
    comp_ret3->setLogger(&logger_ret3);
    comp_ret3->config()->setTraceCodeGenerator();
    bool success_ret3 = ret3.constructIL();
    EXPECT_TRUE(success_ret3) << "Ret3 IL successfully constructed";
    logger_ret3.print(comp_ret3);

    typedef int8_t (RetFuncProto)();
    CompileResult result = ext->jb1cgCompile(comp_ret3);
    EXPECT_EQ((int32_t)result, (int32_t)CompileSuccessful) << "Compiled ret3 ok";
    RetFuncProto *nr3 = ret3.nativeEntry<RetFuncProto *>();
    int8_t r = nr3();
    EXPECT_EQ(r, 3) << "Compiled ret3 returns 3";
}

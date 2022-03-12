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
#include <limits>
#include <stdio.h>
#include "gtest/gtest.h"
#include "Compiler.hpp"
#include "base/BaseExtension.hpp"
#include "base/Function.hpp"
#include "base/FunctionCompilation.hpp"
#include "TextWriter.hpp"


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

TEST(BaseExtension, cannotLoadUnknownExtension) {
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

#define BASE_FUNC(name,line,file,fields,ext,xtor_code,entry,il_code) \
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
        fields; \
    };

#define COMPILE_FUNC(FuncClass, FuncProto, f, DO_LOGGING) \
    Compiler c("testBase"); \
    Base::BaseExtension *ext = c.loadExtension<Base::BaseExtension>(); \
    FuncClass func(&c, ext); \
    Base::FunctionCompilation *comp = func.comp(); \
    TextWriter logger(comp, std::cout, std::string("    ")); \
    TextWriter *log = (DO_LOGGING) ? &logger : NULL; \
    CompileResult result = func.Compile(log); \
    EXPECT_EQ((int)result, (int)CompileSuccessful) << "Compiled function ok"; \
    FuncProto *f = func.nativeEntry<FuncProto *>(); \
    assert(f)


// Test function that returns a constant value
#define CONSTFUNC(type,seq,v) \
    BASE_FUNC(Const ## type ## Function ## seq, "0", #type ".cpp", , \
        _x, { DefineReturnType(_x->type); }, \
        b, { _x->Return(LOC, b, _x->Const ## type(LOC, b, v)); })

#define TESTONECONSTFUNC(type,ctype,seq,v) \
    CONSTFUNC(type, seq, v) \
    TEST(BaseExtension, createConst ## type ## Function ## seq) { \
        typedef ctype (FuncProto)(); \
        COMPILE_FUNC(Const ## type ## Function ## seq, FuncProto, f, false); \
        EXPECT_EQ(f(), v) << "Compiled f() returns " << v; \
    }

#define TESTCONSTFUNC(type,ctype,a,b) \
    TESTONECONSTFUNC(type,ctype,1,a) \
    TESTONECONSTFUNC(type,ctype,2,b) \
    TESTONECONSTFUNC(type,ctype,3,(std::numeric_limits<ctype>::min())) \
    TESTONECONSTFUNC(type,ctype,4,(std::numeric_limits<ctype>::max()))

TESTCONSTFUNC(Int8, int8_t, 3, 0)
TESTCONSTFUNC(Int16, int16_t, 3, 0)
TESTCONSTFUNC(Int32, int32_t, 3, 0)
TESTCONSTFUNC(Int64, int64_t, 3, 0)
TESTCONSTFUNC(Float32, float, 3.0f, 0.0f)
TESTCONSTFUNC(Float64, double, 3.0d, 0.0d)

// Test function that returns the value of its single parameter
#define TYPEFUNC(type) \
    BASE_FUNC(type ## Function, "0", #type ".cpp", , \
        _x, { DefineReturnType(_x->type); DefineParameter("val", _x->type); }, \
        b, { auto parmSym=LookupLocal("val"); _x->Return(LOC, b, _x->Load(LOC, b, parmSym)); })

#define TESTTYPEFUNC(type,ctype,a,b) \
    TYPEFUNC(type) \
    TEST(BaseExtension, create ## type ## Function) { \
        typedef ctype (FuncProto)(ctype); \
        COMPILE_FUNC(type ## Function, FuncProto, f, false); \
        EXPECT_EQ(f(a), a) << "Compiled f(" << a << ") returns " << a; \
        EXPECT_EQ(f(b), b) << "Compiled f(" << b << ") returns " << b; \
        ctype min=std::numeric_limits<ctype>::min(); \
        EXPECT_EQ(f(min), min) << "Compiled f(" << min << ") returns " << min; \
        ctype max=std::numeric_limits<ctype>::max(); \
        EXPECT_EQ(f(max), max) << "Compiled f(" << max << ") returns " << max; \
    }

TESTTYPEFUNC(Int8, int8_t, 3, 0)
TESTTYPEFUNC(Int16, int16_t, 3, 0)
TESTTYPEFUNC(Int32, int32_t, 3, 0)
TESTTYPEFUNC(Int64, int64_t, 3, 0)
TESTTYPEFUNC(Float32, float, 3.0f, 0.0f)
TESTTYPEFUNC(Float64, double, 3.0d, 0.0d)

// Address handled specially
TYPEFUNC(Address)
TEST(BaseExtension, createAddressFunction) {
    typedef void * (FuncProto)(void *);
    COMPILE_FUNC(AddressFunction, FuncProto, f, false);
    void *x = NULL; EXPECT_EQ(f(x), x) << "Compiled f(" << x << ") returns " << x;
    void *y = (void *)&x;EXPECT_EQ(f(y), y) << "Compiled f(" << y << ") returns " << y;
    void *z = (void *)(-1);EXPECT_EQ(f(z), z) << "Compiled f(" << z << ") returns " << z;
}

// Test function that loads parm, stores it into a local variable, then loads and returns the local
#define STORETYPEFUNC(type) \
    BASE_FUNC(Store ## type ## Function, "0", "Store" #type ".cpp", \
        Base::LocalSymbol * _val, \
        _x, { DefineReturnType(_x->type); DefineParameter("parm", _x->type); _val = DefineLocal("val", _x->type); }, \
        b, { auto parm=LookupLocal("parm"); _x->Store(LOC, b, _val, _x->Load(LOC, b, parm)); _x->Return(LOC, b, _x->Load(LOC, b, _val)); })

#define TESTSTORETYPEFUNC(type,ctype,a,b) \
    STORETYPEFUNC(type) \
    TEST(BaseExtension, createStore ## type ## Function) { \
        typedef ctype (FuncProto)(ctype); \
        COMPILE_FUNC(Store ## type ## Function, FuncProto, f, false); \
        EXPECT_EQ(f(a), a) << "Compiled f(" << a << ") returns " << a; \
        EXPECT_EQ(f(b), b) << "Compiled f(" << b << ") returns " << b; \
        ctype min=std::numeric_limits<ctype>::min(); \
        EXPECT_EQ(f(min), min) << "Compiled f(" << min << ") returns " << min; \
        ctype max=std::numeric_limits<ctype>::max(); \
        EXPECT_EQ(f(max), max) << "Compiled f(" << max << ") returns " << max; \
    }

TESTSTORETYPEFUNC(Int8, int8_t, 3, 0)
TESTSTORETYPEFUNC(Int16, int16_t, 3, 0)
TESTSTORETYPEFUNC(Int32, int32_t, 3, 0)
TESTSTORETYPEFUNC(Int64, int64_t, 3, 0)
TESTSTORETYPEFUNC(Float32, float, 3.0f, 0.0f)
TESTSTORETYPEFUNC(Float64, double, 3.0d, 0.0d)

// Address handled specially
STORETYPEFUNC(Address)
TEST(BaseExtension, createStoreAddressFunction) {
    typedef void * (FuncProto)(void *);
    COMPILE_FUNC(StoreAddressFunction, FuncProto, f, false);
    void *x = NULL; EXPECT_EQ(f(x), x) << "Compiled f(" << x << ") returns " << x;
    void *y = (void *)&x;EXPECT_EQ(f(y), y) << "Compiled f(" << y << ") returns " << y;
    void *z = (void *)(-1);EXPECT_EQ(f(z), z) << "Compiled f(" << z << ") returns " << z;
}

// Test function that returns the value pointed to by its single parameter
#define POINTERTOTYPEFUNC(type) \
    BASE_FUNC(PointerTo ## type ## Function, "0", "PointerTo" #type ".cpp", , \
        _x, { DefineReturnType(_x->type); DefineParameter("ptr", PointerTo(LOC, _x->type)); }, \
        b, { auto parmSym = LookupLocal("ptr"); _x->Return(LOC, b, _x->LoadAt(LOC, b, _x->Load(LOC, b, parmSym))); })

#define TESTPOINTERTOTYPEFUNC(type,ctype,a,b) \
    POINTERTOTYPEFUNC(type) \
    TEST(BaseExtension, createPointer ## type ## Function) { \
        typedef ctype (FuncProto)(ctype *); \
        COMPILE_FUNC(PointerTo ## type ## Function, FuncProto, f, false); \
        ctype x=a; EXPECT_EQ(f(&x), a) << "Compiled f(&" << a << ") returns " << a; \
        ctype y=b; EXPECT_EQ(f(&y), b) << "Compiled f(&" << b << ") returns " << b; \
        ctype min=std::numeric_limits<ctype>::min(); \
        EXPECT_EQ(f(&min), min) << "Compiled f(&min) returns " << min; \
        ctype max=std::numeric_limits<ctype>::max(); \
        EXPECT_EQ(f(&max), max) << "Compiled f(&max) returns " << max; \
    }

TESTPOINTERTOTYPEFUNC(Int8, int8_t, 3, 0)
TESTPOINTERTOTYPEFUNC(Int16, int16_t, 3, 0)
TESTPOINTERTOTYPEFUNC(Int32, int32_t, 3, 0)
TESTPOINTERTOTYPEFUNC(Int64, int64_t, 3, 0)
TESTPOINTERTOTYPEFUNC(Float32, float, 3.0f, 0.0f)
TESTPOINTERTOTYPEFUNC(Float64, double, 3.0d, 0.0d)

// Address handled specially
POINTERTOTYPEFUNC(Address)
TEST(BaseExtension, createPointerAddressFunction) {
    typedef void * (FuncProto)(void **);
    COMPILE_FUNC(PointerToAddressFunction, FuncProto, f, false);
    void *a=NULL;
    void *b=(void *)&a; EXPECT_EQ((intptr_t)(f(&b)), (intptr_t)(&a)) << "Compiled f(&" << b << ") returns " << a;
}

// Test function that stores a parameter value through a second pointer parameter
#define STOREPOINTERTOTYPEFUNC(type) \
    BASE_FUNC(StorePointerTo ## type ## Function, "0", "StorePointerTo" #type ".cpp", , \
        _x, { DefineReturnType(_x->NoType); \
              DefineParameter("ptr", PointerTo(LOC, _x->type)); \
              DefineParameter("val", _x->type); }, \
        b, { auto ptrParm = LookupLocal("ptr"); \
             auto valParm = LookupLocal("val"); \
             _x->StoreAt(LOC, b, _x->Load(LOC, b, ptrParm), _x->Load(LOC, b, valParm)); \
             _x->Return(LOC, b); })

#define TESTSTOREPOINTERTOTYPEFUNC(type,ctype,a,b) \
    STOREPOINTERTOTYPEFUNC(type) \
    TEST(BaseExtension, createStorePointer ## type ## Function) { \
        typedef void (FuncProto)(ctype *, ctype); \
        COMPILE_FUNC(StorePointerTo ## type ## Function, FuncProto, f, false); \
        ctype d=0xbb; \
        f(&d, a); EXPECT_EQ(d, a) << "Compiled f(&d," << a << ") stored " << a; \
        f(&d, b); EXPECT_EQ(d, b) << "Compiled f(&d," << b << ") stored " << b; \
        ctype min=std::numeric_limits<ctype>::min(); \
        f(&d, min); EXPECT_EQ(d, min) << "Compiled f(&d,min) stored " << min; \
        ctype max=std::numeric_limits<ctype>::max(); \
        f(&d, max); EXPECT_EQ(d, max) << "Compiled f(&d,max) stored " << max; \
    }

TESTSTOREPOINTERTOTYPEFUNC(Int8, int8_t, 3, 0)
TESTSTOREPOINTERTOTYPEFUNC(Int16, int16_t, 3, 0)
TESTSTOREPOINTERTOTYPEFUNC(Int32, int32_t, 3, 0)
TESTSTOREPOINTERTOTYPEFUNC(Int64, int64_t, 3, 0)
TESTSTOREPOINTERTOTYPEFUNC(Float32, float, 3.0f, 0.0f)
TESTSTOREPOINTERTOTYPEFUNC(Float64, double, 3.0d, 0.0d)

// Address handled specially
STOREPOINTERTOTYPEFUNC(Address)
TEST(BaseExtension, createStorePointerAddressFunction) {
    typedef void (FuncProto)(void **, void *);
    COMPILE_FUNC(StorePointerToAddressFunction, FuncProto, f, false);
    void *a=(void *)(-1);
    f(&a, NULL); EXPECT_EQ((intptr_t)a, (intptr_t)NULL) << "Compiled f(&a, NULL) stores NULL to a";
}

// Test function that loads and returns a field's value from a struct pointer passed as parameter
#define ONEFIELDSTRUCTTYPEFUNC(type) \
    BASE_FUNC(OneFieldStruct ## type ## Function, "0", "OneFieldStruct_" #type ".cpp", \
        Base::ParameterSymbol *_parm; const Base::StructType *_structType; const Base::PointerType *_pStructType;, \
        _x, { Base::StructTypeBuilder stb(_x, this); \
              stb.setName("Struct") \
                 ->addField("field", _x->type, 0); \
              _structType = stb.create(LOC); \
              _pStructType = PointerTo(LOC, _structType); \
              _parm = DefineParameter("parm", _pStructType); \
              DefineReturnType(_x->type); \
            }, \
        b, { Value *base = _x->Load(LOC, b, _parm); \
             const Base::FieldType *field = _structType->LookupField("field"); \
             Value *fieldVal = _x->LoadFieldAt(LOC, b, field, base); \
             _x->Return(LOC, b, fieldVal); })

#define TESTONEFIELDTYPESTRUCT(theType,ctype,a,b) \
    ONEFIELDSTRUCTTYPEFUNC(theType) \
    TEST(BaseExtension, createOneFieldStruct ## theType) { \
        typedef struct { ctype field; } TheStructType; \
        typedef ctype (FuncProto)(TheStructType *); \
        COMPILE_FUNC(OneFieldStruct ## theType ## Function, FuncProto, f, false); \
        const Base::FieldType *fieldType = func._structType->LookupField("field"); \
        EXPECT_EQ(fieldType->name(), std::string("field")); \
        EXPECT_EQ(fieldType->type()->id(), ext->theType->id()); \
        EXPECT_EQ(fieldType->size(), func._structType->size()); \
        TheStructType str; \
        str.field = a; ctype w = f(&str); EXPECT_EQ(w, a); \
        str.field = b; ctype x = f(&str); EXPECT_EQ(x, b); \
        ctype min = str.field = std::numeric_limits<ctype>::min(); \
        ctype y = f(&str); EXPECT_EQ(y, min); \
        ctype max = str.field = std::numeric_limits<ctype>::max(); \
        ctype z = f(&str); EXPECT_EQ(z, max); \
    }

TESTONEFIELDTYPESTRUCT(Int8,int8_t,3,0)
TESTONEFIELDTYPESTRUCT(Int16,int16_t,3,0)
TESTONEFIELDTYPESTRUCT(Int32,int32_t,3,0)
TESTONEFIELDTYPESTRUCT(Int64,int64_t,3,0)
TESTONEFIELDTYPESTRUCT(Float32,float,3.0f,0.0f)
TESTONEFIELDTYPESTRUCT(Float64,double,3.0d,0.0d)

ONEFIELDSTRUCTTYPEFUNC(Address)
TEST(BaseExtension, createOneFieldStructAddress) {
    typedef struct { void * field; } TheStructType;
    typedef void * (FuncProto)(TheStructType *);
    COMPILE_FUNC(OneFieldStructAddressFunction, FuncProto, f, false);
    const Base::FieldType *fieldType = func._structType->LookupField("field");
    EXPECT_EQ(fieldType->name(), std::string("field"));
    EXPECT_EQ(fieldType->type()->id(), ext->Address->id());
    EXPECT_EQ(fieldType->size(), func._structType->size());
    TheStructType str;
    str.field = NULL; void * w = f(&str); EXPECT_EQ((intptr_t)w, (intptr_t)NULL);
    void *ptr = (void *)&str;
    str.field = ptr; void * x = f(&str); EXPECT_EQ((intptr_t)x, (intptr_t)ptr);
}

// Test function that loads and returns the fifth field's value from a struct pointer passed as parameter
#define FIVEFIELDSTRUCTTYPEFUNC(type,ctype) \
    BASE_FUNC(FiveFieldStruct ## type ## Function, "0", "FiveFieldStruct_" #type ".cpp", \
        Base::ParameterSymbol *_parm; const Base::StructType *_structType; const Base::PointerType *_pStructType;, \
        _x, { Base::StructTypeBuilder stb(_x, this); \
              typedef struct { ctype f1; ctype f2; ctype f3; ctype f4; ctype f5; } TheStructType; \
              stb.setName("Struct") \
                 ->addField("f1", _x->type, 8*offsetof(TheStructType,f1)) \
                 ->addField("f2", _x->type, 8*offsetof(TheStructType,f2)) \
                 ->addField("f3", _x->type, 8*offsetof(TheStructType,f3)) \
                 ->addField("f4", _x->type, 8*offsetof(TheStructType,f4)) \
                 ->addField("f5", _x->type, 8*offsetof(TheStructType,f5)); \
              _structType = stb.create(LOC); \
              _pStructType = PointerTo(LOC, _structType); \
              _parm = DefineParameter("parm", _pStructType); \
              DefineReturnType(_x->type); \
            }, \
        b, { Value *base = _x->Load(LOC, b, _parm); \
             const Base::FieldType *field = _structType->LookupField("f5"); \
             Value *fieldVal = _x->LoadFieldAt(LOC, b, field, base); \
             _x->Return(LOC, b, fieldVal); })

#define TESTFIVEFIELDTYPESTRUCT(theType,ctype,a,b) \
    FIVEFIELDSTRUCTTYPEFUNC(theType,ctype) \
    TEST(BaseExtension, createFiveFieldStruct ## theType) { \
        typedef struct { ctype f1; ctype f2; ctype f3; ctype f4; ctype f5; } TheStructType; \
        typedef ctype (FuncProto)(TheStructType *); \
        COMPILE_FUNC(FiveFieldStruct ## theType ## Function, FuncProto, f, false); \
        const Base::FieldType *fieldType = func._structType->LookupField("f5"); \
        EXPECT_EQ(fieldType->name(), std::string("f5")); \
        EXPECT_EQ(fieldType->type()->id(), ext->theType->id()); \
        EXPECT_EQ(fieldType->offset(), 8*offsetof(TheStructType,f5)); \
        TheStructType str; \
        str.f5 = a; ctype w = f(&str); EXPECT_EQ(w, a); \
        str.f5 = b; ctype x = f(&str); EXPECT_EQ(x, b); \
        ctype min = str.f5 = std::numeric_limits<ctype>::min(); \
        ctype y = f(&str); EXPECT_EQ(y, min); \
        ctype max = str.f5 = std::numeric_limits<ctype>::max(); \
        ctype z = f(&str); EXPECT_EQ(z, max); \
    }

TESTFIVEFIELDTYPESTRUCT(Int8,int8_t,3,0)
TESTFIVEFIELDTYPESTRUCT(Int16,int16_t,3,0)
TESTFIVEFIELDTYPESTRUCT(Int32,int32_t,3,0)
TESTFIVEFIELDTYPESTRUCT(Int64,int64_t,3,0)
TESTFIVEFIELDTYPESTRUCT(Float32,float,3.0f,0.0f)
TESTFIVEFIELDTYPESTRUCT(Float64,double,3.0d,0.0d)

FIVEFIELDSTRUCTTYPEFUNC(Address,void*)
TEST(BaseExtension, createFiveFieldStructAddress) {
    typedef struct { void * f1; void * f2; void * f3; void * f4; void * f5; } TheStructType;
    typedef void * (FuncProto)(TheStructType *);
    COMPILE_FUNC(FiveFieldStructAddressFunction, FuncProto, f, false);
    const Base::FieldType *fieldType = func._structType->LookupField("f5");
    EXPECT_EQ(fieldType->name(), std::string("f5"));
    EXPECT_EQ(fieldType->type()->id(), ext->Address->id());
    TheStructType str;
    str.f5 = NULL; void * w = f(&str); EXPECT_EQ((intptr_t)w, (intptr_t)NULL);
    void *ptr = (void *)&str;
    str.f5 = ptr; void * x = f(&str); EXPECT_EQ((intptr_t)x, (intptr_t)ptr);
}

// Test function that stores a parameter to the fifth field's value in a struct pointer also passed as parameter
#define STOREFIVEFIELDSTRUCTTYPEFUNC(type,ctype) \
    BASE_FUNC(StoreFiveFieldStruct ## type ## Function, "0", "StoreFiveFieldStruct_" #type ".cpp", \
        Base::ParameterSymbol *_valParm; Base::ParameterSymbol *_baseParm; const Base::StructType *_structType; const Base::PointerType *_pStructType;, \
        _x, { Base::StructTypeBuilder stb(_x, this); \
              typedef struct { ctype f1; ctype f2; ctype f3; ctype f4; ctype f5; } TheStructType; \
              stb.setName("Struct") \
                 ->addField("f1", _x->type, 8*offsetof(TheStructType,f1)) \
                 ->addField("f2", _x->type, 8*offsetof(TheStructType,f2)) \
                 ->addField("f3", _x->type, 8*offsetof(TheStructType,f3)) \
                 ->addField("f4", _x->type, 8*offsetof(TheStructType,f4)) \
                 ->addField("f5", _x->type, 8*offsetof(TheStructType,f5)); \
              _structType = stb.create(LOC); \
              _pStructType = PointerTo(LOC, _structType); \
              _valParm = DefineParameter("val", _x->type); \
              _baseParm = DefineParameter("pStruct", _pStructType); \
              DefineReturnType(_x->NoType); \
            }, \
        b, { Value *base = _x->Load(LOC, b, _baseParm); \
             const Base::FieldType *field = _structType->LookupField("f5"); \
             Value *val = _x->Load(LOC, b, _valParm); \
             _x->StoreFieldAt(LOC, b, field, base, val); \
             _x->Return(LOC, b); })

#define TESTSTOREFIVEFIELDTYPESTRUCT(theType,ctype,a,b) \
    STOREFIVEFIELDSTRUCTTYPEFUNC(theType,ctype) \
    TEST(BaseExtension, createStoreFiveFieldStruct ## theType) { \
        typedef struct { ctype f1; ctype f2; ctype f3; ctype f4; ctype f5; } TheStructType; \
        typedef void (FuncProto)(ctype, TheStructType *); \
        COMPILE_FUNC(StoreFiveFieldStruct ## theType ## Function, FuncProto, f, false); \
        const Base::FieldType *fieldType = func._structType->LookupField("f5"); \
        EXPECT_EQ(fieldType->name(), std::string("f5")); \
        EXPECT_EQ(fieldType->type()->id(), ext->theType->id()); \
        EXPECT_EQ(fieldType->offset(), 8*offsetof(TheStructType,f5)); \
        TheStructType str; \
        f(a, &str); ctype w=str.f5; EXPECT_EQ(w, a); \
        f(b, &str); ctype x=str.f5; EXPECT_EQ(x, b); \
        ctype min = std::numeric_limits<ctype>::min(); \
        f(min, &str); ctype y = str.f5; EXPECT_EQ(y, min); \
        ctype max = str.f5 = std::numeric_limits<ctype>::max(); \
        f(max, &str); ctype z = str.f5; EXPECT_EQ(z, max); \
    }

TESTSTOREFIVEFIELDTYPESTRUCT(Int8,int8_t,3,0)
TESTSTOREFIVEFIELDTYPESTRUCT(Int16,int16_t,3,0)
TESTSTOREFIVEFIELDTYPESTRUCT(Int32,int32_t,3,0)
TESTSTOREFIVEFIELDTYPESTRUCT(Int64,int64_t,3,0)
TESTSTOREFIVEFIELDTYPESTRUCT(Float32,float,3.0f,0.0f)
TESTSTOREFIVEFIELDTYPESTRUCT(Float64,double,3.0d,0.0d)

STOREFIVEFIELDSTRUCTTYPEFUNC(Address,void*)
TEST(BaseExtension, createStoreFiveFieldStructAddress) {
    typedef struct { void * f1; void * f2; void * f3; void * f4; void * f5; } TheStructType;
    typedef void (FuncProto)(void *, TheStructType *);
    COMPILE_FUNC(StoreFiveFieldStructAddressFunction, FuncProto, f, false);
    const Base::FieldType *fieldType = func._structType->LookupField("f5");
    EXPECT_EQ(fieldType->name(), std::string("f5"));
    EXPECT_EQ(fieldType->type()->id(), ext->Address->id());
    TheStructType str;
    f(NULL, &str); void * w = str.f5; EXPECT_EQ((intptr_t)w, (intptr_t)NULL);
    void *ptr = (void *)&str;
    f(ptr, &str); void * x = str.f5; EXPECT_EQ((intptr_t)x, (intptr_t)ptr);
}
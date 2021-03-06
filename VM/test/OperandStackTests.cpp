/*******************************************************************************
 * Copyright (c) 2016, 2022 IBM Corp. and others
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


#include <cstddef>
#include <cstring>
#include <dlfcn.h>
#include <errno.h>
#include <iostream>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "JBCore.hpp"
#include "Base/Base.hpp"
#include "VM/VM.hpp"
#include "OperandStackTests.hpp"

using std::cout;
using std::cerr;

using namespace OMR::JitBuilder;

typedef struct Thread
   {
   int pad;
   STACKVALUECTYPE *sp;
   } Thread;

class TestState : public VM::VirtualMachineState {
public:
    static VM::StateKind STATEKIND;

    TestState(LOCATION, VM::VMExtension *vme)
        : VM::VirtualMachineState(PASSLOC, vme, STATEKIND),
        _stack(NULL),
        _stackTop(NULL) {

    }

    TestState(LOCATION, VM::VMExtension *vme, VM::VirtualMachineOperandStack *stack, VM::VirtualMachineRegister *stackTop)
        : VM::VirtualMachineState(PASSLOC, vme, STATEKIND),
        _stack(stack),
        _stackTop(stackTop) {

    }

    virtual void Commit(LOCATION, Builder *b) {
        _stack->Commit(PASSLOC, b);
        _stackTop->Commit(PASSLOC, b);
    }

    virtual void Reload(LOCATION, Builder *b) {
        _stack->Reload(PASSLOC, b);
        _stackTop->Reload(PASSLOC, b);
    }

    virtual VM::VirtualMachineState *MakeCopy(LOCATION, Builder *b) {
        TestState *newState = new TestState(PASSLOC, _vme);
        newState->_stack = _stack->MakeCopy(PASSLOC, b)->refine<VM::VirtualMachineOperandStack>();
        newState->_stackTop = _stackTop->MakeCopy(PASSLOC, b)->refine<VM::VirtualMachineRegister>();
        return newState;
    }

    virtual void MergeInto(LOCATION, VM::VirtualMachineState *other, Builder *b) {
        TestState *otherState = other->refine<TestState>();
        _stack->MergeInto(PASSLOC, otherState->_stack, b);
        _stackTop->MergeInto(PASSLOC, otherState->_stackTop, b);
    }

    VM::VirtualMachineOperandStack * _stack;
    VM::VirtualMachineRegister * _stackTop;
};

VM::StateKind TestState::STATEKIND = VirtualMachineState::assignStateKind(VirtualMachineState::STATEKIND, "TestState");

static bool verbose = false;
static int32_t numFailingTests = 0;
static int32_t numPassingTests = 0;
static STACKVALUECTYPE **verifySP = NULL;
static STACKVALUECTYPE expectedResult12Top = -1;
static char * result12Operator;
static Thread thread;
static bool useThreadSP = false;

static void
setupResult12Equals() {
    expectedResult12Top = 11;
    result12Operator = (char *)"==";
}

static void
setupResult12NotEquals() {
    expectedResult12Top = 99;
    result12Operator = (char *)"!=";
}

int
main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--verbose") == 0)
        verbose = true;

    if (verbose) cout << "Step 0: load jbcore.so\n";
    void *handle = dlopen("libjbcore.so", RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        return -1;
    }

    if (verbose) cout << "Step 1: Create a Compiler\n";
    Compiler compiler("OperandStackTests");

    if (verbose) cout << "Step 2: load extensions (Base and VM)\n";
    Base::BaseExtension *base = compiler.loadExtension<Base::BaseExtension>();
    assert(base);
    VM::VMExtension *vme = compiler.loadExtension<VM::VMExtension>();
    assert(vme);

    if (verbose) cout << "Step 3: Create Function object\n";
    OperandStackTestFunction pointerFunction(base, vme);

    if (verbose) cout << "Step 4: Set up logging configuration\n";
    Base::FunctionCompilation *comp = pointerFunction.comp();
    TextWriter logger(comp, std::cout, std::string("    "));
    TextWriter *log = (verbose) ? &logger : NULL;
    
    if (verbose) cout << "Step 5: compile function\n";
    CompilerReturnCode result = pointerFunction.Compile(log);
    
    if (result != compiler.CompileSuccessful) {
        cout << "Compile failed: " << result << "\n";
        exit(-1);
    }

    if (verbose) cout << "Step 6: invoke compiled function and print results\n";
    typedef void (OperandStackTestProto)();
    OperandStackTestProto *ptrTest = pointerFunction.nativeEntry<OperandStackTestProto *>();
    verifySP = pointerFunction.getSPPtr();
    setupResult12Equals();
    ptrTest();

    if (verbose) cout << "Step 7: Set up operand stack tests using a Thread structure\n";
    OperandStackTestUsingStructFunction threadFunction(base, vme);
    comp = threadFunction.comp();
    TextWriter logger2(comp, std::cout, std::string("    "));
    TextWriter *log2 = (verbose) ? &logger : NULL;

    if (verbose) cout << "Step 8: compile function\n";
    result = threadFunction.Compile(log2);
    if (result != compiler.CompileSuccessful) {
        cout << "Compile failed: " << result << "\n";
        exit(-1);
    }

    if (verbose) cout << "Step 9: invoke compiled code and print results\n";
    typedef void (OperandStackTestUsingStructProto)(Thread *thread);
    OperandStackTestUsingStructProto *threadTest = threadFunction.nativeEntry<OperandStackTestUsingStructProto *>();

    useThreadSP = true;
    verifySP = &thread.sp;
    setupResult12NotEquals();
    threadTest(&thread);

    cout << "Number passing tests: " << numPassingTests << "\n";
    cout << "Number failing tests: " << numFailingTests << "\n";

    if (numFailingTests == 0)
        cout << "ALL PASS\n";
    else
        cout << "SOME FAILURES\n";
}


STACKVALUECTYPE *OperandStackTestFunction::_realStack = NULL;
STACKVALUECTYPE *OperandStackTestFunction::_realStackTop = _realStack - 1;
int32_t OperandStackTestFunction::_realStackSize = -1;

void
OperandStackTestFunction::createStack() {
    int32_t stackSizeInBytes = _realStackSize * sizeof(STACKVALUECTYPE);
    _realStack = (STACKVALUECTYPE *) malloc(stackSizeInBytes);
    _realStackTop = _realStack - 1;
    thread.sp = _realStackTop;
    memset(_realStack, 0, stackSizeInBytes);
}

STACKVALUECTYPE *
OperandStackTestFunction::moveStack() {
    int32_t stackSizeInBytes = _realStackSize * sizeof(STACKVALUECTYPE);
    STACKVALUECTYPE *newStack = (STACKVALUECTYPE *) malloc(stackSizeInBytes);
    int32_t delta = 0;
    if (useThreadSP)
        delta = thread.sp - _realStack;
    else
        delta = _realStackTop - _realStack;
    memcpy(newStack, _realStack, stackSizeInBytes);
    memset(_realStack, 0xFF, stackSizeInBytes);
    free(_realStack);
    _realStack = newStack;
    _realStackTop = _realStack + delta;
    thread.sp = _realStackTop;

    return _realStack - 1;
}

void
OperandStackTestFunction::freeStack() {
    memset(_realStack, 0xFF, _realStackSize * sizeof(STACKVALUECTYPE));
    free(_realStack);
    _realStack = NULL;
    _realStackTop = NULL;
    thread.sp = NULL;
}

static void FailingTest() {
    numFailingTests++;
}

static void PassingTest() {
    numPassingTests++;
}

#define REPORT1(c,n,v)         { if (c) { PassingTest(); if (verbose) cout << "Pass\n"; } else { FailingTest(); if (verbose) cout << "Fail: " << (n) << " is " << (v) << "\n"; } }
#define REPORT2(c,n1,v1,n2,v2) { if (c) { PassingTest(); if (verbose) cout << "Pass\n"; } else { FailingTest(); if (verbose) cout << "Fail: " << (n1) << " is " << (v1) << ", " << (n2) << " is " << (v2) << "\n"; } }

// Result 0: empty stack even though Push has happened
void
verifyResult0() {
    if (verbose) cout << "Push(1)  [ no commit ]\n";
    OperandStackTestFunction::verifyStack("0", -1, 0);
}

void
verifyResult1() {
    if (verbose) cout << "Commit(); Top()\n";
    OperandStackTestFunction::verifyStack("1", 0, 1, 1);
}

void
verifyResult2(STACKVALUECTYPE top) {
    if (verbose) cout << "Push(2); Push(3); Top()   [ no commit]\n";
    if (verbose) cout << "\tResult 2: top value == 3: ";
    REPORT1(top == 3, "top", top);

    OperandStackTestFunction::verifyStack("2", 0, 1, 1);
}

void
verifyResult3(STACKVALUECTYPE top) {
    if (verbose) cout << "Commit(); Top()\n";
    if (verbose) cout << "\tResult 3: top value == 3: ";
    REPORT1(top == 3, "top", top);

    OperandStackTestFunction::verifyStack("3", 2, 3, 1, 2, 3);
}

void
verifyResult4(STACKVALUECTYPE popValue) {
    if (verbose) cout << "Pop()    [ no commit]\n";
    if (verbose) cout << "\tResult 4: pop value == 3: ";
    REPORT1(popValue == 3, "popValue", popValue);

    OperandStackTestFunction::verifyStack("4", 2, 3, 1, 2, 3);
}

void
verifyResult5(STACKVALUECTYPE popValue) {
    if (verbose) cout << "Pop()    [ no commit]\n";
    if (verbose) cout << "\tResult 5: pop value == 2: ";
    REPORT1(popValue == 2, "popValue", popValue);

    OperandStackTestFunction::verifyStack("5", 2, 3, 1, 2, 3);
}

void
verifyResult6(STACKVALUECTYPE top) {
    if (verbose) cout << "Push(Add(popValue1, popValue2)); Commit(); Top()\n";
    if (verbose) cout << "\tResult 6: top == 5: ";
    REPORT1(top == 5, "top", top);

    OperandStackTestFunction::verifyStack("6", 2, 2, 1, 5);
}

void
verifyResult7() {
    if (verbose) cout << "Drop(2); Commit(); [ empty stack ]\n";
    OperandStackTestFunction::verifyStack("7", 2, 0);
}

void
verifyResult8(STACKVALUECTYPE pick) {
    if (verbose) cout << "Push(5); Push(4); Push(3); Push(2); Push(1); Commit(); Pick(3)\n";
    if (verbose) cout << "\tResult 8: pick == 4: ";
    REPORT1(pick == 4, "pick", pick);

    OperandStackTestFunction::verifyStack("8", 2, 0);
}

void
verifyResult9(STACKVALUECTYPE top) {
    if (verbose) cout << "Drop(2); Top()\n";
    if (verbose) cout << "\tResult 9: top == 3: ";
    REPORT1(top == 3, "top", top);

    OperandStackTestFunction::verifyStack("9", 2, 0);
}

void
verifyResult10(STACKVALUECTYPE pick) {
    if (verbose) cout << "Dup(); Pick(2)\n";
    if (verbose) cout << "\tResult 10: pick == 4: ";
    REPORT1(pick == 4, "pick", pick);
 
    OperandStackTestFunction::verifyStack("10", 2, 0);
}

void
verifyResult11() {
    if (verbose) cout << "Commit();\n";
    OperandStackTestFunction::verifyStack("11", 3, 4, 5, 4, 3, 3);
}

void
verifyResult12(STACKVALUECTYPE top) {
    if (verbose) cout << "Pop(); Pop(); if (3 " << result12Operator << " 3) { Push(11); } else { Push(99); } Commit(); Top();\n";
    if (verbose) cout << "\tResult 12: top == " << expectedResult12Top << ": ";
    REPORT1(top == expectedResult12Top, "top", top);
    OperandStackTestFunction::verifyStack("11", 3, 3, 5, 4, expectedResult12Top);
}

// used to compare expected values and report fail it not equal
void
verifyValuesEqual(STACKVALUECTYPE v1, STACKVALUECTYPE v2) { 
    REPORT2(v1 == v2, "verifyValuesEqual v1", v1, "verifyValuesEqual v2", v2); 
}

// take the arguments from the stack and modify them
void
modifyTop3Elements(int32_t amountToAdd) {
    if (verbose) cout << "Push();Push();Push() - modify elements passed in real stack and return";
    STACKVALUECTYPE *realSP = *verifySP; 
    REPORT1(realSP[0]== 3, "modifyTop3Elements realSP[0]", realSP[0]); 
    REPORT1(realSP[-1]== 2, "modifyTop3Elements realSP[-1]", realSP[-1]); 
    REPORT1(realSP[-2]== 1, "modifyTop3Elements realSP[-2]", realSP[-2]); 
    realSP[0] += amountToAdd;
    realSP[-1] += amountToAdd;
    realSP[-2] += amountToAdd;  
}




bool
OperandStackTestFunction::verifyUntouched(int32_t maxTouched) {
    for (int32_t i=maxTouched+1;i < _realStackSize;i++) 
        if (_realStack[i] != 0)
            return false;
    return true;
}

void
OperandStackTestFunction::verifyStack(const char *step, int32_t max, int32_t num, ...) {

    STACKVALUECTYPE *realSP = *verifySP;

    if (verbose) cout << "\tResult " << step << ": realSP-_realStack == " << (num-1) << ": ";
    REPORT2((realSP-_realStack) == (num-1), "_realStackTop-_realStack", (realSP-_realStack), "num-1", (num-1));

    va_list args;
    va_start(args, num);
    for (int32_t a=0;a < num;a++) {
        STACKVALUECTYPE val = va_arg(args, STACKVALUECTYPE);
        if (verbose) cout << "\tResult " << step << ": _realStack[" << a << "] == " << val << ": ";
        REPORT2(_realStack[a] == val, "_realStack[a]", _realStack[a], "val", val);
    }
    va_end(args);

    if (verbose) cout << "\tResult " << step << ": upper stack untouched: ";
    REPORT1(verifyUntouched(max), "max", max);
}


OperandStackTestFunction::OperandStackTestFunction(Base::BaseExtension *base, VM::VMExtension *vme)
    : Base::Function(base->compiler())
    , _base(base)
    , _vme(vme) {

    DefineLine(LINETOSTR(__LINE__));
    DefineFile(__FILE__);

    DefineName("OperandStackTest");
    const Type *NoType = base->NoType;
    DefineReturnType(NoType);

    _realStackSize = 32;
    _valueType = base->STACKVALUETYPE;
    const Type *pValueType = _base->PointerTo(LOC, comp(), _valueType);

    _createStack = DefineFunction(LOC, "createStack", "0", "0", (void *)&OperandStackTestFunction::createStack, NoType, 0);
    _moveStack = DefineFunction(LOC, "moveStack", "0", "0", (void *)&OperandStackTestFunction::moveStack, pValueType, 0);
    _freeStack = DefineFunction(LOC, "freeStack", "0", "0", (void *)&OperandStackTestFunction::freeStack, NoType, 0);
    _verifyResult0 = DefineFunction(LOC, "verifyResult0", "0", "0", (void *)&verifyResult0, NoType, 0);
    _verifyResult1 = DefineFunction(LOC, "verifyResult1", "0", "0", (void *)&verifyResult1, NoType, 0);
    _verifyResult2 = DefineFunction(LOC, "verifyResult2", "0", "0", (void *)&verifyResult2, NoType, 1, _valueType);
    _verifyResult3 = DefineFunction(LOC, "verifyResult3", "0", "0", (void *)&verifyResult3, NoType, 1, _valueType);
    _verifyResult4 = DefineFunction(LOC, "verifyResult4", "0", "0", (void *)&verifyResult4, NoType, 1, _valueType);
    _verifyResult5 = DefineFunction(LOC, "verifyResult5", "0", "0", (void *)&verifyResult5, NoType, 1, _valueType);
    _verifyResult6 = DefineFunction(LOC, "verifyResult6", "0", "0", (void *)&verifyResult6, NoType, 1, _valueType);
    _verifyResult7 = DefineFunction(LOC, "verifyResult7", "0", "0", (void *)&verifyResult7, NoType, 0);
    _verifyResult8 = DefineFunction(LOC, "verifyResult8", "0", "0", (void *)&verifyResult8, NoType, 1, _valueType);
    _verifyResult9 = DefineFunction(LOC, "verifyResult9", "0", "0", (void *)&verifyResult9, NoType, 1, _valueType);
    _verifyResult10 = DefineFunction(LOC, "verifyResult10", "0", "0", (void *)&verifyResult10, NoType, 1, _valueType);
    _verifyResult11 = DefineFunction(LOC, "verifyResult11", "0", "0", (void *)&verifyResult11, NoType, 0);
    _verifyResult12 = DefineFunction(LOC, "verifyResult12", "0", "0", (void *)&verifyResult12, NoType, 1, _valueType);
    _verifyValuesEqual = DefineFunction(LOC, "verifyValuesEqual", "0", "0", (void *)&verifyValuesEqual, NoType, 2, _valueType, _valueType);
    _modifyTop3Elements = DefineFunction(LOC, "modifyTop3Elements", "0", "0", (void *)&modifyTop3Elements, NoType, 1, _valueType);
}

// convenience macros
#define STACK(b)           (((b)->vmState()->refine<TestState>())->_stack)
#define STACKTOP(b)        (((b)->vmState()->refine<TestState>())->_stackTop)
#define COMMIT(b)          ((b)->vmState()->Commit(LOC, b))
#define RELOAD(b)          ((b)->vmState()->Reload(LOC, b))
#define UPDATESTACK(b,s)   (STACK(b)->UpdateStack(LOC, b, s))
#define PUSH(b,v)          (STACK(b)->Push(v))
#define POP(b)             (STACK(b)->Pop())
#define TOP(b)             (STACK(b)->Top())
#define DUP(b)             (STACK(b)->Dup())
#define DROP(b,d)          (STACK(b)->Drop(d))
#define PICK(b,d)          (STACK(b)->Pick(d))

VM::BytecodeBuilder *
OperandStackTestFunction::testStack(VM::BytecodeBuilder *b, bool useEqual) {
    STACKVALUECTYPE one=1;
    Literal *lv1 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&one));
    PUSH(b, _base->Const(LOC, b, lv1));
    _base->Call(LOC, b, _verifyResult0);

    COMMIT(b);
    _base->Call(LOC, b, _verifyResult1);

    STACKVALUECTYPE two=2;
    Literal *lv2 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&two));
    PUSH(b, _base->Const(LOC, b, lv2));

    STACKVALUECTYPE three=3;
    Literal *lv3 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&three));
    PUSH(b, _base->Const(LOC, b, lv3));
    _base->Call(LOC, b, _verifyResult2, TOP(b));

    COMMIT(b);
    Value *newStack = _base->Call(LOC, b, _moveStack);
    UPDATESTACK(b, newStack);
    _base->Call(LOC, b, _verifyResult3, TOP(b));

    Value *val1 = POP(b);
    _base->Call(LOC, b, _verifyResult4, val1);

    Value *val2 = POP(b);
    _base->Call(LOC, b, _verifyResult5, val2);

    Value *sum = _base->Add(LOC, b, val1, val2);
    PUSH(b, sum);
    COMMIT(b);
    newStack = _base->Call(LOC, b, _moveStack);
    UPDATESTACK(b, newStack);
    _base->Call(LOC, b, _verifyResult6, TOP(b));

    DROP(b, 2);
    COMMIT(b);
    _base->Call(LOC, b, _verifyResult7);

    STACKVALUECTYPE four=4;
    Literal *lv4 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&four));
    STACKVALUECTYPE five=5;
    Literal *lv5 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&five));

    PUSH(b, _base->Const(LOC, b, lv5));
    PUSH(b, _base->Const(LOC, b, lv4));
    PUSH(b, _base->Const(LOC, b, lv3));
    PUSH(b, _base->Const(LOC, b, lv2));
    PUSH(b, _base->Const(LOC, b, lv1));
    _base->Call(LOC, b, _verifyResult8, PICK(b, 3));

    DROP(b, 2);
    _base->Call(LOC, b, _verifyResult9, TOP(b));

    DUP(b);
    _base->Call(LOC, b, _verifyResult10, PICK(b, 2));

    COMMIT(b);
    newStack = _base->Call(LOC, b, _moveStack);
    UPDATESTACK(b, newStack);
    _base->Call(LOC, b, _verifyResult11);

    VM::BytecodeBuilder *thenBB = _vme->OrphanBytecodeBuilder(comp(), 1, 1, "BCI_then");
    VM::BytecodeBuilder *elseBB = _vme->OrphanBytecodeBuilder(comp(), 2, 1, "BCI_else");
    VM::BytecodeBuilder *mergeBB = _vme->OrphanBytecodeBuilder(comp(), 3, 1, "BCI_merge");

    Value *v1 = POP(b);
    Value *v2 = POP(b);
    if (useEqual)
        _vme->IfCmpEqual(LOC, b, thenBB, v1, v2);
    else
        _vme->IfCmpNotEqual(LOC, b, thenBB, v1, v2);
    _vme->Goto(LOC, b, elseBB);

    STACKVALUECTYPE eleven=11;
    Literal *lv11 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&eleven));
    PUSH(thenBB, _base->Const(LOC, thenBB, lv11));
    _vme->Goto(LOC, thenBB, mergeBB);

    STACKVALUECTYPE ninetynine=99;
    Literal *lv99 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&ninetynine));
    PUSH(elseBB, _base->Const(LOC, elseBB, lv99));
    _vme->Goto(LOC, elseBB, mergeBB);

    COMMIT(mergeBB);
    newStack = _base->Call(LOC, mergeBB, _moveStack);
    UPDATESTACK(mergeBB, newStack);
    _base->Call(LOC, mergeBB, _verifyResult12, TOP(mergeBB));
 
    STACKVALUECTYPE amountToAdd = 10;
    Literal *lvAmount = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&amountToAdd));

    // Reload test. Call a routine that modifies stack elements passed to it. 
    // Test by reloading and test the popped values
    PUSH(mergeBB, _base->Const(LOC, mergeBB, lv1));
    PUSH(mergeBB, _base->Const(LOC, mergeBB, lv2));
    PUSH(mergeBB, _base->Const(LOC, mergeBB, lv3));  
    COMMIT(mergeBB); 
    _base->Call(LOC, mergeBB, _modifyTop3Elements, _base->Const(LOC, mergeBB, lvAmount));  
    RELOAD(mergeBB);

    Value *modifiedStackElement = POP(mergeBB);
    STACKVALUECTYPE amountPlus3 = 3+amountToAdd;
    Literal *lvAmountPlus3 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&amountPlus3));
    Value *expected =  _base->Const(LOC, mergeBB, lvAmountPlus3); 
    _base->Call(LOC, mergeBB, _verifyValuesEqual, modifiedStackElement, expected);  

    modifiedStackElement = POP(mergeBB);
    STACKVALUECTYPE amountPlus2 = 2+amountToAdd;
    Literal *lvAmountPlus2 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&amountPlus2));
    expected = _base->Const(LOC, mergeBB, lvAmountPlus2);
    _base->Call(LOC, mergeBB, _verifyValuesEqual, modifiedStackElement, expected);  

    modifiedStackElement = POP(mergeBB);
    STACKVALUECTYPE amountPlus1 = 1+amountToAdd;
    Literal *lvAmountPlus1 = _valueType->literal(LOC, comp(), reinterpret_cast<LiteralBytes *>(&amountPlus1));
    expected =  _base->Const(LOC, mergeBB, lvAmountPlus1);
    _base->Call(LOC, mergeBB, _verifyValuesEqual, modifiedStackElement, expected);  

    _base->Call(LOC, mergeBB, _freeStack);

    _base->Return(LOC, mergeBB);

    return mergeBB;
}

bool
OperandStackTestFunction::buildIL() {
    const Base::PointerType *pElementType = _base->PointerTo(LOC, comp(), _base->PointerTo(LOC, comp(), _base->STACKVALUETYPE));

    Builder *entry = builderEntry();
    _base->Call(LOC, entry, _createStack);

    Value *realStackTopAddress = _base->ConstPointer(LOC, entry, pElementType, &_realStackTop);
    VM::VirtualMachineRegister *stackTop = new VM::VirtualMachineRegister(LOC, _vme, "SP", this, realStackTopAddress);
    VM::VirtualMachineOperandStack *stack = new VM::VirtualMachineOperandStack(LOC, _vme, this, 1, stackTop, _base->STACKVALUETYPE);

    TestState *vmState = new TestState(LOC, _vme, stack, stackTop);

    VM::BytecodeBuilder *bb = _vme->OrphanBytecodeBuilder(comp(), 0, 1, std::string("entry"));
    bb->setVMState(vmState);
    _base->Goto(LOC, entry, bb);

    testStack(bb, true);

    return true;
}




OperandStackTestUsingStructFunction::OperandStackTestUsingStructFunction(Base::BaseExtension *base, VM::VMExtension *vme)
    : OperandStackTestFunction(base, vme) {

    Base::StructTypeBuilder builder(base, this);
    builder.setName("Thread")
           ->addField("sp", base->PointerTo(LOC, comp(), base->STACKVALUETYPE), 8*offsetof(Thread, sp));
    _threadType = builder.create(LOC);
    _spField = _threadType->LookupField("sp");

    _threadParam = DefineParameter("thread", base->PointerTo(LOC, comp(), _threadType));
}

bool
OperandStackTestUsingStructFunction::buildIL() {
    Builder *entry = builderEntry();

    _base->Call(LOC, entry, _createStack);

    VM::VirtualMachineRegisterInStruct *stackTop = new VM::VirtualMachineRegisterInStruct(LOC, _vme, "SP", this, _spField, _threadParam);
    VM::VirtualMachineOperandStack *stack = new VM::VirtualMachineOperandStack(LOC, _vme, this, 1, stackTop, _base->STACKVALUETYPE);

    TestState *vmState = new TestState(LOC, _vme, stack, stackTop);
    VM::BytecodeBuilder *bb = _vme->OrphanBytecodeBuilder(comp(), 0, 1, std::string("entry"));
    bb->setVMState(vmState);
    _base->Goto(LOC, entry, bb);

    testStack(bb, false);

    return true;
}

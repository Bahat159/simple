
/* 
	Copyright (c) 2017-2018 Azeez Adewale <azeezadewale98@gmail.com"> 
	MIT License Copyright (c) 2017 simple 

*/

/* 
 * File:   simple.h
 * Author: thecarisma
 *
 * Created on July 10, 2017, 1:10 PM
 */


#ifndef simple_vm_h
#define simple_vm_h
/*
**  Data 
**  Stack Size 
*/
#define SIMPLE_VM_STACK_SIZE 256
#define SIMPLE_VM_STACK_CHECKOVERFLOW 253
#define SIMPLE_VM_FREE_STACK_IN_CLASS_REGION_AFTER 100
#define SIMPLE_VM_BC_ITEMS_COUNT 24
typedef struct ByteCode {
	Item *aData[SIMPLE_VM_BC_ITEMS_COUNT]  ;
	char nSize  ;
	List *pList  ;
} ByteCode ;
typedef struct VM {
	int nPC  ;
	List *pCode  ;
	List *pFunctionsMap  ;
	List *pClassesMap  ;
	List *pModulessMap  ;
	int nOPCode  ;
	Item aStack[SIMPLE_VM_STACK_SIZE]  ;
	unsigned char nSP  ;
	List *pMem  ;
	List *pActiveMem  ;
	List *pTempMem  ;
	ByteCode *pByteCode  ;
	ByteCode *pByteCodeIR  ;
	char *cFileName  ;
	int nLineNumber  ;
	int nListStart  ;
	List *pNestedLists  ;
	int nBlockFlag  ;
	List *aPCBlockFlag  ;
	List *pFuncCallList  ;
	int nFuncSP  ;
	int nFuncExecute  ;
	List *pCFunctionsList  ;
	List *pExitMark  ;
	List *pLoopMark  ;
	int nCallMainFunction  ;
	List *pTry  ;
	List *aScopeNewObj  ;
	char nCallMethod  ;
	List *pObjState  ;
	List *pBraceObject  ;
	List *aBraceObjects  ;
	int nVarScope  ;
	int nScopeID  ;
	List *aScopeID  ;
	int nActiveScopeID  ;
	int nActiveCatch  ;
	char nInsideBraceFlag  ;
	char nInClassRegion  ;
	List *aActiveModules  ;
	char nPrivateFlag  ;
	char nGetSetProperty  ;
	void *pGetSetObject  ;
	char nGetSetObjType  ;
	List *aSetProperty  ;
	void *pAssignment  ;
	List *aCPointers  ;
	List *aForStep  ;
	char nFirstAddress  ;
	char nBeforeEqual  ;
	char nNOAssignment  ;
	SimpleState *pSimpleState  ;
	List *aLoadAddressScope  ;
	List *aBeforeObjState  ;
	List *pLoadAddressScope  ;
	int nFuncExecute2  ;
	List *aNewByteCodeItems  ;
	char nEvalCalledFromSimpleCode  ;
	char nDecimals  ;
	char nEvalReallocationFlag  ;
	int nEvalReallocationSize  ;
	int nCFuncParaCount  ;
	char nIgnoreNULL  ;
	int nEvalReturnPC  ;
	char nRetItemRef  ;
	void (*pFuncMutexDestroy)(void *) ;
	void (*pFuncMutexLock)(void *) ;
	void (*pFuncMutexUnlock)(void *) ;
	void *pMutex  ;
	char nIgnoreCPointerTypeCheck  ;
	char nCallClassInit  ;
	char nRetEvalDontDelete  ;
	char *cPrevFileName  ;
	char nRunCode  ;
	char nActiveError  ;
	List *aDynamicSelfItems  ;
	String *pModulesName  ;
	char lTrace  ;
	String *pTrace  ;
	char lTraceActive  ;
	char nTraceEvent  ;
	List *pTraceData  ;
	char nEvalInScope  ;
	char lPassError  ;
	char lHideErrorMsg  ;
	List *aGlobalScopes  ;
	List *aActiveGlobalScopes  ;
	int nCurrentGlobalScope  ;
	char *cFileNameInClassRegion  ;
} VM ;
/*
**  Functions 
**  Main 
*/

VM * simple_vm_new ( SimpleState *pSimpleState ) ;

VM * simple_vm_delete ( VM *vm ) ;

SIMPLE_API void simple_vm_loadcode ( VM *vm ) ;

void simple_vm_start ( SimpleState *pSimpleState,VM *vm ) ;

void simple_vm_fetch ( VM *vm ) ;

void simple_vm_fetch2 ( VM *vm ) ;

void simple_vm_execute ( VM *vm ) ;

SIMPLE_API void simple_vm_error ( VM *vm,const char *cStr ) ;

int simple_vm_eval ( VM *vm,const char *cStr ) ;

void simple_vm_tobytecode ( VM *vm,int x ) ;

void simple_vm_error2 ( VM *vm,const char *cStr,const char *cStr2 ) ;

void simple_vm_newbytecodeitem ( VM *vm,int x ) ;

void simple_vm_mainloop ( VM *vm ) ;

SIMPLE_API void simple_vm_runcode ( VM *vm,const char *cStr ) ;

void simple_vm_init ( SimpleState *pSimpleState ) ;

void simple_vm_printstack ( VM *vm ) ;

SIMPLE_API void simple_vm_showerrormessage ( VM *vm,const char *cStr ) ;

void simple_vm_addglobalvariables ( VM *vm ) ;
/* Stack and Variables */

void simple_vm_pushv ( VM *vm ) ;

void simple_vm_loadaddress ( VM *vm ) ;

void simple_vm_assignment ( VM *vm ) ;

void simple_vm_inc ( VM *vm ) ;

void simple_vm_loadapushv ( VM *vm ) ;

void simple_vm_newline ( VM *vm ) ;

void simple_vm_freestack ( VM *vm ) ;

void simple_vm_setreference ( VM *vm ) ;

void simple_vm_list_copy ( VM *vm,List *pNewList, List *pList ) ;

void simple_vm_list_simpointercopy ( VM *vm,List *pList ) ;

void simple_vm_beforeequallist ( VM *vm,List *pVar,double nNum1 ) ;

void simple_vm_beforeequalitem ( VM *vm,Item *pItem,double nNum1 ) ;

void simple_vm_assignmentpointer ( VM *vm ) ;

void simple_vm_freeloadaddressscope ( VM *vm ) ;

void simple_vm_setfilename ( VM *vm ) ;

void simple_vm_loadaddressfirst ( VM *vm ) ;

void simple_vm_endfuncexec ( VM *vm ) ;
/* Compare */

void simple_vm_equal ( VM *vm ) ;

void simple_vm_lessequal ( VM *vm ) ;

void simple_vm_less ( VM *vm ) ;

void simple_vm_greater ( VM *vm ) ;

void simple_vm_greaterequal ( VM *vm ) ;

void simple_vm_notequal ( VM *vm ) ;
/* Math */

void simple_vm_sum ( VM *vm ) ;

void simple_vm_sub ( VM *vm ) ;

void simple_vm_mul ( VM *vm ) ;

void simple_vm_div ( VM *vm ) ;

void simple_vm_mod ( VM *vm ) ;

void simple_vm_neg ( VM *vm ) ;

char * simple_vm_numtostring ( VM *vm,double nNum1,char *cStr ) ;

double simple_vm_stringtonum ( VM *vm,const char *cStr ) ;

void simple_vm_expr_ppoo ( VM *vm,const char *cStr ) ;

void simple_vm_expr_npoo ( VM *vm,const char *cStr,double nNum1 ) ;

void simple_vm_expr_spoo ( VM *vm,const char *cStr,const char *cStr2,int nSize ) ;

void simple_vm_plusplus ( VM *vm ) ;

void simple_vm_minusminus ( VM *vm ) ;

void simple_vm_addlisttolist ( VM *vm,List *pList,List *pList2 ) ;
/* Logic */

void simple_vm_and ( VM *vm ) ;

void simple_vm_or ( VM *vm ) ;

void simple_vm_not ( VM *vm ) ;
/* Variables */

void simple_vm_newscope ( VM *vm ) ;

int simple_vm_findvar ( VM *vm,const char *cStr ) ;

int simple_vm_findvar2 ( VM *vm,int x,List *pList2,const char *cStr ) ;

void simple_vm_newvar ( VM *vm,const char *cStr ) ;

List * simple_vm_newvar2 ( VM *vm,const char *cStr,List *pParent ) ;

void simple_vm_addnewnumbervar ( VM *vm,const char *cStr,double x ) ;

void simple_vm_addnewstringvar ( VM *vm,const char *cStr,const char *cStr2 ) ;

void simple_vm_deletescope ( VM *vm ) ;

void simple_vm_addnewpointervar ( VM *vm,const char *cStr,void *x,int y ) ;

void simple_vm_newtempvar ( VM *vm,const char *cStr, List *TempList ) ;

void simple_vm_addnewstringvar2 ( VM *vm,const char *cStr,const char *cStr2,int nStrSize ) ;

List * simple_vm_newtempvar2 ( VM *vm,const char *cStr,List *pList3 ) ;

void simple_vm_addnewcpointervar ( VM *vm,const char *cStr,void *pPointer,const char *cStr2 ) ;
/* Jump */

void simple_vm_jump ( VM *vm ) ;

void simple_vm_jumpzero ( VM *vm ) ;

void simple_vm_jumpfor ( VM *vm ) ;

void simple_vm_jumpone ( VM *vm ) ;

void simple_vm_jumpzero2 ( VM *vm ) ;

void simple_vm_jumpone2 ( VM *vm ) ;
/* Lists */

void simple_vm_liststart ( VM *vm ) ;

void simple_vm_listitem ( VM *vm ) ;

void simple_vm_listend ( VM *vm ) ;

void simple_vm_loadindexaddress ( VM *vm ) ;

void simple_vm_listpushv ( VM *vm ) ;

void simple_vm_listassignment ( VM *vm ) ;

void simple_vm_listgetvalue ( VM *vm,List *pVar,const char *cStr ) ;

int simple_vm_strcmpnotcasesensitive ( const char *cStr1,const char *cStr2 ) ;
/* Functions */

int simple_vm_loadfunc ( VM *vm ) ;

int simple_vm_loadfunc2 ( VM *vm,const char *cStr,int nPerformance ) ;

void simple_vm_call ( VM *vm ) ;

void simple_vm_call2 ( VM *vm ) ;

void simple_vm_return ( VM *vm ) ;

void simple_vm_returnnull ( VM *vm ) ;

void simple_vm_returneval ( VM *vm ) ;

void simple_vm_newfunc ( VM *vm ) ;

void simple_vm_blockflag ( VM *vm ) ;

void simple_vm_blockflag2 ( VM *vm,int x ) ;

void simple_vm_removeblockflag ( VM *vm ) ;

void simple_vm_movetoprevscope ( VM *vm ) ;

void simple_vm_createtemplist ( VM *vm ) ;

void simple_vm_saveloadaddressscope ( VM *vm ) ;

void simple_vm_restoreloadaddressscope ( VM *vm ) ;

void simple_vm_anonymous ( VM *vm ) ;

int simple_vm_isstackpointertoobjstate ( VM *vm ) ;

void simple_vm_retitemref ( VM *vm ) ;

void simple_vm_callclassinit ( VM *vm ) ;
/* User Interface */

void simple_vm_see ( VM *vm ) ;

void simple_vm_give ( VM *vm ) ;
/* String As Array */

void simple_vm_stsimple_pushv ( VM *vm ) ;

void simple_vm_stsimple_assignment ( VM *vm ) ;

void simple_vm_stsimple_index ( VM *vm , String *pString , double x ) ;
/* Try Catch Done */

void simple_vm_try ( VM *vm ) ;

void simple_vm_catch ( VM *vm,const char *cError ) ;

void simple_vm_done ( VM *vm ) ;
/* Duplicate and Range */

void simple_vm_dup ( VM *vm ) ;

void simple_vm_range ( VM *vm ) ;

List * simple_vm_range_newlist ( VM *vm ) ;
/* OOP */

void simple_vm_oop_newobj ( VM *vm ) ;

void simple_vm_oop_property ( VM *vm ) ;

int simple_vm_oop_isobject ( List *pList ) ;

List * simple_vm_oop_getobj ( VM *vm ) ;

void simple_vm_oop_loadmethod ( VM *vm ) ;

void simple_vm_oop_aftercallmethod ( VM *vm ) ;

void simple_vm_oop_setscope ( VM *vm ) ;

void simple_vm_oop_printobj ( VM *vm,List *pList ) ;

void simple_vm_oop_parentinit ( VM *vm,List *pList ) ;

void simple_vm_oop_parentmethods ( VM *vm,List *pList ) ;

void simple_vm_oop_newclass ( VM *vm ) ;

void simple_vm_oop_setbraceobj ( VM *vm,List *pList ) ;

void simple_vm_oop_bracestart ( VM *vm ) ;

void simple_vm_oop_braceend ( VM *vm ) ;

void simple_vm_oop_bracestack ( VM *vm ) ;

void simple_vm_oop_newsuperobj ( VM *vm,List *pState,List *pClass ) ;

List * simple_vm_oop_getsuperobj ( VM *vm ) ;

void simple_vm_oop_loadsuperobjmethod ( VM *vm,List *pSuper ) ;

void simple_vm_oop_import ( VM *vm ) ;

List * simple_vm_oop_checkpointertoclassinmodules ( VM *vm,List *pList ) ;

void simple_vm_oop_import2 ( VM *vm,const char *cModules ) ;

void simple_vm_oop_import3 ( VM *vm,List *pList ) ;

int simple_vm_oop_visibleclassescount ( VM *vm ) ;

List * simple_vm_oop_visibleclassitem ( VM *vm,int x ) ;

void simple_vm_oop_pushclassmodules ( VM *vm,List *pList ) ;

void simple_vm_oop_popclassmodules ( VM *vm ) ;

void simple_vm_oop_deletemodulessafter ( VM *vm,int x ) ;

int simple_vm_oop_callmethodinsideclass ( VM *vm ) ;

void simple_vm_oop_setget ( VM *vm,List *pVar ) ;

void simple_vm_oop_setproperty ( VM *vm ) ;

void simple_vm_oop_operatoroverloading ( VM *vm,List *pObj,const char *cStr1,int nType,const char *cStr2,double nNum1,void *pPointer,int nPointerType ) ;

List * simple_vm_oop_objvarfromobjlist ( List *pList ) ;

int simple_vm_oop_objtypefromobjlist ( List *pList ) ;

Item * simple_vm_oop_objitemfromobjlist ( List *pList ) ;

void simple_vm_oop_callmethodfrombrace ( VM *vm ) ;

int simple_vm_oop_ismethod ( VM *vm,List *pList,const char *cStr ) ;

void simple_vm_oop_updateselfpointer ( VM *vm,List *pObj,int nType,void *pContainer ) ;

void simple_vm_oop_movetobeforeobjstate ( VM *vm ) ;

void simple_vm_oop_setthethisvariable ( VM *vm ) ;

void simple_vm_oop_updateselfpointer2 ( VM *vm,List *pObj ) ;
/* For Better Performance */

void simple_vm_pushp ( VM *vm ) ;

void simple_vm_incp ( VM *vm ) ;

void simple_vm_pushpv ( VM *vm ) ;

void simple_vm_incjump ( VM *vm ) ;

void simple_vm_incpjump ( VM *vm ) ;

void simple_vm_jumpvarlenum ( VM *vm ) ;

void simple_vm_jumpvarplenum ( VM *vm ) ;

void simple_vm_loadfuncp ( VM *vm ) ;

void simple_vm_pushplocal ( VM *vm ) ;

void simple_vm_inclpjump ( VM *vm ) ;

void simple_vm_jumpvarlplenum ( VM *vm ) ;

void simple_vm_incpjumpstep1 ( VM *vm ) ;

void simple_vm_jumpvarplenumstep1 ( VM *vm ) ;
/* End Program / Exit from Loop / Loop (Continue) */

void simple_vm_bye ( VM *vm ) ;

void simple_vm_exitmark ( VM *vm ) ;

void simple_vm_popexitmark ( VM *vm ) ;

void simple_vm_exit ( VM *vm,int nType ) ;
/* State */

void simple_vm_savestate ( VM *vm,List *pList ) ;

void simple_vm_restorestate ( VM *vm,List *pList,int nPos,int nFlag ) ;

void simple_vm_backstate ( VM *vm,int x,List *pList ) ;

void simple_vm_savestate2 ( VM *vm,List *pList ) ;

void simple_vm_restorestate2 ( VM *vm,List *pList,int x ) ;

List * simple_vm_savestack ( VM *vm ) ;

void simple_vm_restorestack ( VM *vm,List *pList ) ;
/* Bitwise */

void simple_vm_bitand ( VM *vm ) ;

void simple_vm_bitor ( VM *vm ) ;

void simple_vm_bitxor ( VM *vm ) ;

void simple_vm_bitnot ( VM *vm ) ;

void simple_vm_bitshl ( VM *vm ) ;

void simple_vm_bitshr ( VM *vm ) ;
/* Step Number */

void simple_vm_stepnumber ( VM *vm ) ;

void simple_vm_popstep ( VM *vm ) ;
/* Threads */

SIMPLE_API void simple_vm_mutexfunctions ( VM *vm,void *(*pFunc)(void),void (*pFuncLock)(void *),void (*pFuncUnlock)(void *),void (*pFuncDestroy)(void *) ) ;

SIMPLE_API void simple_vm_mutexlock ( VM *vm ) ;

SIMPLE_API void simple_vm_mutexunlock ( VM *vm ) ;

SIMPLE_API void simple_vm_mutexdestroy ( VM *vm ) ;

SIMPLE_API void simple_vm_runcodefromthread ( VM *vm,const char *cStr ) ;
/* Trace */

void simple_vm_traceevent ( VM *vm,char nEvent ) ;
/* Fast Function Call for Extensions (Without Eval) */

SIMPLE_API void simple_vm_callfunction ( VM *vm,char *cFuncName ) ;
/* Custom Global Scope */

void simple_vm_newglobalscope ( VM *vm ) ;

void simple_vm_endglobalscope ( VM *vm ) ;

void simple_vm_setglobalscope ( VM *vm ) ;

List * simple_vm_getglobalscope ( VM *vm ) ;
/*
**  Macro 
**  Stack 
**  Add 
*/
#define SIMPLE_VM_STACK_PUSHC vm->nSP++ ; simple_itemarray_setstring2(vm->aStack, vm->nSP, simple_stsimple_get(vm->pByteCodeIR->aData[1]->data.pString), simple_stsimple_size(vm->pByteCodeIR->aData[1]->data.pString))
#define SIMPLE_VM_STACK_PUSHN vm->nSP++ ; simple_itemarray_setdouble(vm->aStack, vm->nSP , vm->pByteCodeIR->aData[1]->data.dNumber)
#define SIMPLE_VM_STACK_PUSHP vm->nSP++ ; simple_itemarray_setpointer(vm->aStack, vm->nSP , vm->pByteCodeIR->aData[1]->data.pPointer )
/* Note, use SIMPLE_VM_STACK_OBJTYPE to read/write the pointer type */
#define SIMPLE_VM_STACK_TRUE simple_itemarray_setdouble(vm->aStack,vm->nSP, 1)
#define SIMPLE_VM_STACK_FALSE simple_itemarray_setdouble(vm->aStack,vm->nSP, 0)
#define SIMPLE_VM_STACK_PUSHCVAR simple_itemarray_setstring2(vm->aStack,vm->nSP,simple_list_getstring(pVar,3),simple_list_getstringsize(pVar,3))
#define SIMPLE_VM_STACK_PUSHNVAR simple_itemarray_setdouble(vm->aStack,vm->nSP,simple_list_getdouble(pVar,3))
#define SIMPLE_VM_STACK_PUSHPVALUE(x) vm->nSP++ ; simple_itemarray_setpointer(vm->aStack, vm->nSP , x )
#define SIMPLE_VM_STACK_PUSHCVALUE(x) vm->nSP++ ; simple_itemarray_setstring(vm->aStack, vm->nSP, x)
#define SIMPLE_VM_STACK_PUSHNVALUE(x) vm->nSP++ ; simple_itemarray_setdouble(vm->aStack, vm->nSP , x)
#define SIMPLE_VM_STACK_SETCVALUE(x) simple_itemarray_setstring(vm->aStack, vm->nSP, x)
#define SIMPLE_VM_STACK_SETNVALUE(x) simple_itemarray_setdouble(vm->aStack, vm->nSP , x)
#define SIMPLE_VM_STACK_SETPVALUE(x) simple_itemarray_setpointer(vm->aStack, vm->nSP , x )
#define SIMPLE_VM_STACK_SETCVALUE2(x,y) simple_itemarray_setstring2(vm->aStack, vm->nSP, x,y)
/* Check */
#define SIMPLE_VM_STACK_ISSTRING simple_itemarray_isstring(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_ISNUMBER simple_itemarray_isnumber(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_ISPOINTER simple_itemarray_ispointer(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_ISPOINTERVALUE(x) simple_itemarray_ispointer(vm->aStack,x)
/* Read */
#define SIMPLE_VM_STACK_READC simple_itemarray_getstring(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_STRINGSIZE simple_itemarray_getstringsize(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_READN simple_itemarray_getdouble(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_READP simple_itemarray_getpointer(vm->aStack,vm->nSP)
#define SIMPLE_VM_STACK_OBJTYPE vm->aStack[vm->nSP].nObjectType
#define SIMPLE_VM_STACK_PREVOBJTYPE vm->aStack[vm->nSP-1].nObjectType
/* Delete */
#define SIMPLE_VM_STACK_POP vm->nSP--
/* Objects/Pointer  - Type */
#define SIMPLE_OBJTYPE_VARIABLE 1
#define SIMPLE_OBJTYPE_LISTITEM 2
#define SIMPLE_OBJTYPE_SUBSTRING 3
/* Variable Structure */
#define SIMPLE_VAR_NAME 1
#define SIMPLE_VAR_TYPE 2
#define SIMPLE_VAR_VALUE 3
#define SIMPLE_VAR_PVALUETYPE 4
#define SIMPLE_VAR_PRIVATEFLAG 5
/* Number of global variables defined by the VM like True, False, cErrorMsg */
#define SIMPLE_VM_INTERNALGLOBALSCOUNT 14
#define SIMPLE_VAR_LISTSIZE 5
/* Variable Type */
#define SIMPLE_VM_NULL 0
#define SIMPLE_VM_STRING 1
#define SIMPLE_VM_NUMBER 2
#define SIMPLE_VM_LIST 3
#define SIMPLE_VM_POINTER 4
/* IR (Instruction Register) */
#define SIMPLE_VM_JUMP vm->nPC = vm->pByteCodeIR->aData[1]->data.iNumber
#define SIMPLE_VM_IR_READC simple_stsimple_get(vm->pByteCodeIR->aData[1]->data.pString)
#define SIMPLE_VM_IR_READCVALUE(x) simple_stsimple_get(vm->pByteCodeIR->aData[x]->data.pString)
#define SIMPLE_VM_IR_READP vm->pByteCodeIR->aData[1]->data.pPointer
#define SIMPLE_VM_IR_READPVALUE(x) vm->pByteCodeIR->aData[x]->data.pPointer
#define SIMPLE_VM_IR_READI vm->pByteCodeIR->aData[1]->data.iNumber
#define SIMPLE_VM_IR_READIVALUE(x) vm->pByteCodeIR->aData[x]->data.iNumber
#define SIMPLE_VM_IR_READD vm->pByteCodeIR->aData[1]->data.dNumber
#define SIMPLE_VM_IR_READDVALUE(x) vm->pByteCodeIR->aData[x]->data.dNumber
#define SIMPLE_VM_IR_PARACOUNT vm->pByteCodeIR->nSize
#define SIMPLE_VM_IR_OPCODE vm->pByteCodeIR->aData[0]->data.iNumber
#define SIMPLE_VM_IR_SETCVALUE(x,y) simple_stsimple_set_gc(vm->pSimpleState,vm->pByteCodeIR->aData[x]->data.pString,y)
#define SIMPLE_VM_IR_ITEM(x) vm->pByteCodeIR->aData[x]
#define SIMPLE_VM_IR_LIST vm->pByteCodeIR->pList
#define SIMPLE_VM_IR_LOAD vm->pByteCodeIR = vm->pByteCode + vm->nPC - 1
#define SIMPLE_VM_IR_UNLOAD vm->pByteCodeIR = vm->pByteCode + vm->nPC - 2
/*
**  Calling Functions 
**  Note : When you insert items check performance functions for update too! 
**  pFuncCallList ( Type, Func Name , Position(PC) , Stack Pointer , TempMem, ... 
**  Types 
*/
#define SIMPLE_BLOCKTYPE_SCRIPT 1
#define SIMPLE_BLOCKTYPE_C 2
#define SIMPLE_BLOCKCL_TYPE 1
#define SIMPLE_BLOCKCL_NAME 2
#define SIMPLE_BLOCKCL_PC 3
#define SIMPLE_BLOCKCL_SP 4
#define SIMPLE_BLOCKCL_TEMPMEM 5
#define SIMPLE_BLOCKCL_FILENAME 6
#define SIMPLE_BLOCKCL_NEWFILENAME 6
#define SIMPLE_BLOCKCL_METHODORBLOCK 8
#define SIMPLE_BLOCKCL_LINENUMBER 9
#define SIMPLE_BLOCKCL_CALLERPC 10
#define SIMPLE_BLOCKCL_BLOCKEXE 11
#define SIMPLE_BLOCKCL_LISTSTART 12
#define SIMPLE_BLOCKCL_NESTEDLISTS 13
#define SIMPLE_BLOCKCL_STATE 14
/* pFunctionsMap ( Func Name , Position , File Name, Private Flag) */
#define SIMPLE_BLOCKMAP_NAME 1
#define SIMPLE_BLOCKMAP_PC 2
#define SIMPLE_BLOCKMAP_FILENAME 3
#define SIMPLE_BLOCKMAP_PRIVATEFLAG 4
/* FunctionMap List Size */
#define SIMPLE_BLOCKMAP_NORMALSIZE 4
/* Variable Scope */
#define SIMPLE_VARSCOPE_NOTHING 0
#define SIMPLE_VARSCOPE_LOCAL 1
#define SIMPLE_VARSCOPE_OBJSTATE 2
#define SIMPLE_VARSCOPE_GLOBAL 3
#define SIMPLE_VARSCOPE_NEWOBJSTATE 4
/*
**  OOP 
**  pClassesMap 
*/
#define SIMPLE_CLASSMAP_CLASSNAME 1
#define SIMPLE_CLASSMAP_PC 2
#define SIMPLE_CLASSMAP_PARENTCLASS 3
#define SIMPLE_CLASSMAP_METHODSLIST 4
#define SIMPLE_CLASSMAP_FLAGISPARENTCLASSINFCOLLECTED 5
#define SIMPLE_CLASSMAP_POINTERTOMODULE 6
#define SIMPLE_CLASSMAP_POINTERTOLISTOFCLASSINSIDEMODULE 2
#define SIMPLE_CLASSMAP_POINTERTOFILENAME 3
#define SIMPLE_CLASSMAP_IMPORTEDCLASSSIZE 3
/* Moduless */
#define SIMPLE_MODULENAME 1
#define SIMPLE_CLASSESLIST 2
/* Object */
#define SIMPLE_OBJECT_CLASSPOINTER 1
#define SIMPLE_OBJECT_OBJECTDATA 2
/* pObjState */
#define SIMPLE_OBJSTATE_SCOPE 1
#define SIMPLE_OBJSTATE_METHODS 2
#define SIMPLE_OBJSTATE_CLASS 3
/* Operator Overloading */
#define SIMPLE_OOPARA_STRING 1
#define SIMPLE_OOPARA_NUMBER 2
#define SIMPLE_OOPARA_POINTER 3
/* aBraceObjects */
#define SIMPLE_ABRACEOBJECTS_BRACEOBJECT 1
/* aScopeNewObj */
#define SIMPLE_ASCOPENEWOBJ_PREVSCOPE 1
#define SIMPLE_ASCOPENEWOBJ_LISTSTART 2
#define SIMPLE_ASCOPENEWOBJ_NESTEDLISTS 3
#define SIMPLE_ASCOPENEWOBJ_SP 4
#define SIMPLE_ASCOPENEWOBJ_BLOCKSP 10
/* State */
#define SIMPLE_STATE_TRYCATCH 1
#define SIMPLE_STATE_EXIT 2
#define SIMPLE_STATE_RETURN 3
/* Memory */
#define SIMPLE_MEMORY_GLOBALSCOPE 1
/* List as Hash */
#define SIMPLE_LISTHASH_KEY 1
#define SIMPLE_LISTHASH_VALUE 2
#define SIMPLE_LISTHASH_SIZE 2
/* C Pointer List (inside Variable Value) */
#define SIMPLE_CPOINTER_POINTER 1
#define SIMPLE_CPOINTER_TYPE 2
#define SIMPLE_CPOINTER_STATUS 3
#define SIMPLE_CPOINTER_LISTSIZE 3
/* C Pointer Status */
#define SIMPLE_CPOINTERSTATUS_NOTCOPIED 0
#define SIMPLE_CPOINTERSTATUS_COPIED 1
#define SIMPLE_CPOINTERSTATUS_NOTASSIGNED 2
/* Temp Object */
#define SIMPLE_TEMP_OBJECT "simple_temp_object"
#define SIMPLE_TEMP_VARIABLE "simple_sys_temp"
/* Trace */
#define SIMPLE_VM_TRACEEVENT_NEWLINE 1
#define SIMPLE_VM_TRACEEVENT_NEWBLOCK 2
#define SIMPLE_VM_TRACEEVENT_RETURN 3
#define SIMPLE_VM_TRACEEVENT_ERROR 4
#define SIMPLE_VM_TRACEEVENT_BEFORECBLOCK 5
#define SIMPLE_VM_TRACEEVENT_AFTERCBLOCK 6
/* Runtime Error Messages */
#define SIMPLE_VM_ERROR_DIVIDEBYZERO "Error (R1) : Cann't divide by zero !"
#define SIMPLE_VM_ERROR_INDEXOUTOFRANGE "Error (R2) : Array Access (Index out of range) !"
#define SIMPLE_VM_ERROR_BLOCKNOTFOUND "Error (R3) : Calling Function without definition !"
#define SIMPLE_VM_ERROR_STACKOVERFLOW "Error (R4) : Stack Overflow !"
#define SIMPLE_VM_ERROR_OBJECTISNOTLIST "Error (R5) : Can't access the list item, Object is not list !"
#define SIMPLE_VM_ERROR_NOTVARIABLE "Error (R6) : Variable is required"
#define SIMPLE_VM_ERROR_VALUEMORETHANONECHAR "Error (R7) : Can't assign to a string letter more than one character"
#define SIMPLE_VM_ERROR_VARISNOTSTRING "Error (R8) : Variable is not a string "
#define SIMPLE_VM_ERROR_EXITWITHOUTLOOP "Error (R9) : Using exit command outside loops "
#define SIMPLE_VM_ERROR_EXITNUMBEROUTSIDERANGE "Error (R10) : Using exit command with number outside the range "
#define SIMPLE_VM_ERROR_CLASSNOTFOUND "Error (R11) : error in class name, class not found! "
#define SIMPLE_VM_ERROR_PROPERTYNOTFOUND "Error (R12) : error in property name, property not found! "
#define SIMPLE_VM_ERROR_NOTOBJECT "Error (R13) : Object is required"
#define SIMPLE_VM_ERROR_METHODNOTFOUND "Error (R14) : Calling Method without definition !"
#define SIMPLE_VM_ERROR_PARENTCLASSNOTFOUND "Error (R15) : error in parent class name, class not found! "
#define SIMPLE_VM_ERROR_BRACEWITHOUTOBJECT "Error (R16) : Using braces to access unknown object ! "
#define SIMPLE_VM_ERROR_SUPERCLASSNOTFOUND "Error (R17) : error, using 'Super' without parent class! "
#define SIMPLE_VM_ERROR_NUMERICOVERFLOW "Error (R18) : Numeric Overflow! "
#define SIMPLE_VM_ERROR_LESSPARAMETERSCOUNT "Error (R19) : Calling function with less number of parameters!"
#define SIMPLE_VM_ERROR_EXTRAPARAMETERSCOUNT "Error (R20) : Calling function with extra number of parameters!"
#define SIMPLE_VM_ERROR_BADVALUES "Error (R21) : Using operator with values of incorrect type"
#define SIMPLE_VM_ERROR_LOOPWITHOUTLOOP "Error (R22) : Using loop command outside loops "
#define SIMPLE_VM_ERROR_LOOPNUMBEROUTSIDERANGE "Error (R23) : Using loop command with number outside the range "
#define SIMPLE_VM_ERROR_USINGNULLVARIABLE "RUNTIME ERROR 24 : Using uninitialized variable "
#define SIMPLE_VM_ERROR_MODULENOTFOUND "Error (R25) : Error in modules name, Modules not found! "
#define SIMPLE_VM_ERROR_CALLINGPRIVATEMETHOD "Error (R26) : Calling private method from outside the class "
#define SIMPLE_VM_ERROR_USINGPRIVATEATTRIBUTE "Error (R27) : Using private attribute from outside the class "
#define SIMPLE_VM_ERROR_FORSTEPDATATYPE "Error (R28) : Using bad data type as step value"
#define SIMPLE_VM_ERROR_FORLOOPDATATYPE "Error (R29) : Using bad data type in for loop"
#define SIMPLE_VM_ERROR_PARENTCLASSLIKESUBCLASS "Error (R30) : parent class name is identical to child class name "
#define SIMPLE_VM_ERROR_TRYINGTOMODIFYTHESELFPOINTER "Error (R31) : Trying to destory the object using the self reference "
#define SIMPLE_VM_ERROR_BADCALLPARA "Error (R32) : The CALL command expect a variable contains string!"
#define SIMPLE_VM_ERROR_BADDECIMALNUMBER "Error (R33) : Bad decimals number (correct range >= 0 and <=14) !"
#define SIMPLE_VM_ERROR_ASSIGNNOTVARIABLE "Error (R34) : Variable is required for the assignment operation"
#define SIMPLE_VM_ERROR_CANTOPENFILE "Error (R35) : Can't create/open the file!"
#define SIMPLE_VM_ERROR_BADCOLUMNNUMBER "Error (R36) : The column number is not correct! It's greater than the number of columns in the list"
#define SIMPLE_VM_ERROR_BADCOMMAND "Error (R37) : Sorry, The command is not supported in this context"
#define SIMPLE_VM_ERROR_LIBLOADERROR "Error (R38) : Runtime Error in loading the dynamic library!"
#define SIMPLE_VM_ERROR_TEMPFILENAME "Error (R39) : Error occurred creating unique filename."
/* Extra Size (for codeExecution) */
#define SIMPLE_VM_EXTRASIZE 2
/* Variables Location */
#define SIMPLE_VM_STATICVAR_THIS 12
#endif
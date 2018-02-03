
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


#include "../includes/simple.h"
/*
**  Functions 
**  Main 
*/

VM * simple_vm_new ( SimpleState *pSimpleState )
{
	VM *vm  ;
	int x  ;
	vm = (VM *) simple_state_malloc(pSimpleState,sizeof(VM));
	if ( vm == NULL ) {
		printf( SIMPLE_OOM ) ;
		exit(0);
	}
	/* Simple State */
	vm->pSimpleState = pSimpleState ;
	pSimpleState->vm = vm ;
	vm->nPC = 1 ;
	vm->pCode = NULL ;
	vm->pFunctionsMap = NULL ;
	vm->nOPCode = 0 ;
	vm->nSP = 0 ;
	vm->pMem = simple_list_new_gc(vm->pSimpleState,0);
	vm->pActiveMem = NULL ;
	vm->pTempMem = simple_list_new_gc(vm->pSimpleState,0);
	vm->nLineNumber = 1 ;
	/* Information to test the lifetime of the local scope */
	vm->nScopeID = 0 ;
	vm->aScopeID = simple_list_new_gc(vm->pSimpleState,0);
	simple_vm_newscope(vm);
	for ( x = 0 ; x < SIMPLE_VM_STACK_SIZE ; x++ ) {
		vm->aStack[x].nType = ITEMTYPE_NOTHING ;
		vm->aStack[x].nObjectType = 0 ;
		vm->aStack[x].NumberFlag = ITEM_NUMBERFLAG_NOTHING ;
	}
	/* Add Variables */
	simple_vm_addglobalvariables(vm);
	/* Lists */
	vm->nListStart = 0 ;
	vm->pNestedLists = simple_list_new_gc(vm->pSimpleState,0);
	/* Support for nested Load Instructions */
	vm->nBlockFlag = 0 ;
	vm->aPCBlockFlag = simple_list_new_gc(vm->pSimpleState,0);
	/* Calling Functions */
	vm->pFuncCallList = simple_list_new_gc(vm->pSimpleState,0);
	vm->nFuncSP = 0 ;
	vm->nFuncExecute = 0 ;
	if ( pSimpleState->pSimpleCFunctions == NULL ) {
		pSimpleState->pSimpleCFunctions = simple_list_new_gc(vm->pSimpleState,0);
	}
	vm->pCFunctionsList = pSimpleState->pSimpleCFunctions ;
	vm->nCallMainFunction = 0 ;
	/* Support for Exit/Loop Commands inside For/While loops. */
	vm->pExitMark = simple_list_new_gc(vm->pSimpleState,0);
	vm->pLoopMark = simple_list_new_gc(vm->pSimpleState,0);
	/* Try-Catch-Done */
	vm->pTry = simple_list_new_gc(vm->pSimpleState,0);
	/* Saving scope when creating new objects and calling class init method */
	vm->aScopeNewObj = simple_list_new_gc(vm->pSimpleState,0);
	/* Flag ( 0 = Call Function  1 = Call Method After writing object name using dot ) */
	vm->nCallMethod = 0 ;
	/* List of Lists used like Stack, list structure [Pointer to State , Pointer to Methods] */
	vm->pObjState = simple_list_new_gc(vm->pSimpleState,0);
	/* Support for using Braces to access object state */
	vm->pBraceObject = NULL ;
	vm->aBraceObjects = simple_list_new_gc(vm->pSimpleState,0);
	/* Used by BraceStart, BraceEnd & FreeStack */
	vm->nInsideBraceFlag = 0 ;
	/* Variable scope, where is the varaible (when we use findvar) */
	vm->nVarScope = SIMPLE_VARSCOPE_NOTHING ;
	/* Flag used by Try/Catch to tell C-API that catch happens! */
	vm->nActiveCatch = 0 ;
	/*
	**  Flag ( 0 = check NULL variable in PUSHV  , greater than 0 = Ignore null variable ) 
	**  Class Region (After the Class Name) 
	*/
	vm->nInClassRegion = 0 ;
	vm->pModulessMap = NULL ;
	/* Set the main File Name */
	vm->cFileName = simple_list_getstring(vm->pSimpleState->pSimpleFilesList,1) ;
	vm->cPrevFileName = simple_list_getstring(vm->pSimpleState->pSimpleFilesList,1) ;
	/* We keep information about active modules to access its classes directly with new/from */
	vm->aActiveModules = simple_list_new_gc(vm->pSimpleState,0);
	/* Scope of class attribute ( 0 = public 1 = private ) */
	vm->nPrivateFlag = 0 ;
	/* Set/Get Property */
	vm->nGetSetProperty = 0 ;
	vm->pGetSetObject = NULL ;
	vm->nGetSetObjType = 0 ;
	vm->aSetProperty = simple_list_new_gc(vm->pSimpleState,0);
	/* Assignment Pointer */
	vm->pAssignment = NULL ;
	/* C Pointers List (Copied Pointers Only that are active i.e. Not NULL) */
	vm->aCPointers = simple_list_new_gc(vm->pSimpleState,0) ;
	/* For Loop - Step List */
	vm->aForStep = simple_list_new_gc(vm->pSimpleState,0);
	/* Flag for LoadA , when = 1 , if it's a pointer we get First Var. not the Pointer */
	vm->nFirstAddress = 0 ;
	/* Used to know operator before = like += -= *= /= */
	vm->nBeforeEqual = 0 ;
	/*
	**  NOAssignment used to disable instruction AssignmentPointer 
	**  We uses this when we change assignment to set property 
	**  It's important for ListStart to create Temp List when we use setter 
	**  and try to set attribute value to a list and a function setproperty() will be called 
	**  With this list as parameter stored in temp memory 
	*/
	vm->nNOAssignment = 0 ;
	/* List contains the scope of the result of Load Address */
	vm->aLoadAddressScope = simple_list_new_gc(vm->pSimpleState,0);
	/* List contains what to add  later to pObjState, prepare by loadmethod, add before call */
	vm->aBeforeObjState = simple_list_new_gc(vm->pSimpleState,0) ;
	/* Saving pointers to aLoadAddressScope before func. para. to restore after them */
	vm->pLoadAddressScope = simple_list_new_gc(vm->pSimpleState,0);
	/* Another flag like nFuncExec but not used by see command or return command */
	vm->nFuncExecute2 = 0 ;
	/* Create List for Temp Items (added to ByteCode) inside TempMem */
	vm->aNewByteCodeItems = simple_list_new_gc(vm->pSimpleState,0);
	/* Eval can be called from C code (OOP Set/Get/Operator Overloading) or from ring code using executeCode() */
	vm->nEvalCalledFromSimpleCode = 0 ;
	/* Number of decimals after the point */
	vm->nDecimals = 2 ;
	/*
	**  Do we need to reallocation for executeCode() or not ! 
	**  Size of items that we can use without reallocation (double of the original size) 
	*/
	vm->nEvalReallocationSize = 0 ;
	/* Flag ( 1 = we need space over allocated size so we have to do reallocation ) */
	vm->nEvalReallocationFlag = 0 ;
	/* Parameters Count Passed to C Function */
	vm->nCFuncParaCount = 0 ;
	/*
	**  Flag to Ignore NULL output after calling C Function 
	**  This flag is used by the len() function when we use len(object) 
	**  So operator overloading can return the result from the method 
	*/
	vm->nIgnoreNULL = 0 ;
	/* If vm->nPC <= nEvalReturnPC we end the main loop (used by evalreturn) */
	vm->nEvalReturnPC = 0 ;
	/* Flag to return Item Reference (of object state) */
	vm->nRetItemRef = 0 ;
	/* Mutex Functions Pointers - for threads/lock/unlock */
	vm->pFuncMutexLock = NULL ;
	vm->pFuncMutexUnlock = NULL ;
	vm->pFuncMutexDestroy = NULL ;
	vm->pMutex = NULL ;
	/* Ignore C Pointer Type Check in extension functions */
	vm->nIgnoreCPointerTypeCheck = 0 ;
	/*
	**  Flag when we call class init using new obj() 
	**  Since we call init() using generated braces { } 
	**  setting this flag enable LoadAddress to pass these braces 
	**  To access the class attributes of the caller 
	**  because {} hides the class attributes and we need to avoid that! 
	*/
	vm->nCallClassInit = 0 ;
	/*
	**  Flag to avoid deleteing Byte Code using Return From Eval() 
	**  When we use executeCode() then inside executeCode we uses executeCode for setter/getter/operator overloading 
	**  Then deleting byte code will cause a problem in setter/getter/operator overloading 
	**  In this case we set this flag to avoid the delete operation and solve the problem 
	*/
	vm->nRetEvalDontDelete = 0 ;
	/* Counter to know if we are inside simple_vm_runcode() */
	vm->nRunCode = 0 ;
	/* Flag that we have runtime error to avoid calling the error function again */
	vm->nActiveError = 0 ;
	/* Dynamic List of Self Items and PC */
	vm->aDynamicSelfItems = simple_list_new_gc(vm->pSimpleState,0);
	/* The active modules name (after using import command) */
	vm->pModulesName = simple_stsimple_new_gc(vm->pSimpleState,"");
	/*
	**  Trace Program (After Each Line) 
	**  lTrace = Logical Value (Trace is Active or Not) 
	**  pTrace = String contains the code to be executed (Trace Function) 
	**  lTraceActive = The Trace Function is Active - Don't Call Trace function from Trace Function 
	**  nTraceEvent = The Trace Event (1 = New Line , etc) 
	*/
	vm->lTrace = 0 ;
	vm->pTrace = simple_stsimple_new_gc(vm->pSimpleState,"");
	vm->lTraceActive = 0 ;
	vm->nTraceEvent = 0 ;
	vm->pTraceData = simple_list_new_gc(vm->pSimpleState,0) ;
	/* Eval In Scope function is Active : ringvm_evalinscope() */
	vm->nEvalInScope = 0 ;
	/* Pass error in simple_vm_error() from ringvm_passerror() */
	vm->lPassError = 0 ;
	/* Hide Error message - don't display it in simple_vm_error() */
	vm->lHideErrorMsg = 0 ;
	/* Custom Global Scopes (using load modules) */
	vm->aGlobalScopes = simple_list_new_gc(vm->pSimpleState,0);
	vm->aActiveGlobalScopes = simple_list_new_gc(vm->pSimpleState,0);
	vm->nCurrentGlobalScope = 0 ;
	/* File name in the class region */
	vm->cFileNameInClassRegion = NULL ;
	return vm ;
}

VM * simple_vm_delete ( VM *vm )
{
	int x  ;
	List *pRecord  ;
	Item *pItem  ;
	assert(vm);
	vm->pMem = simple_list_delete_gc(vm->pSimpleState,vm->pMem);
	vm->pNestedLists = simple_list_delete_gc(vm->pSimpleState,vm->pNestedLists);
	vm->pFuncCallList = simple_list_delete_gc(vm->pSimpleState,vm->pFuncCallList);
	vm->aPCBlockFlag = simple_list_delete_gc(vm->pSimpleState,vm->aPCBlockFlag);
	vm->pTempMem = simple_list_delete_gc(vm->pSimpleState,vm->pTempMem);
	vm->pExitMark = simple_list_delete_gc(vm->pSimpleState,vm->pExitMark);
	vm->pLoopMark = simple_list_delete_gc(vm->pSimpleState,vm->pLoopMark);
	vm->pTry = simple_list_delete_gc(vm->pSimpleState,vm->pTry);
	vm->aScopeNewObj = simple_list_delete_gc(vm->pSimpleState,vm->aScopeNewObj);
	vm->pObjState = simple_list_delete_gc(vm->pSimpleState,vm->pObjState);
	vm->aBraceObjects = simple_list_delete_gc(vm->pSimpleState,vm->aBraceObjects);
	vm->aScopeID = simple_list_delete_gc(vm->pSimpleState,vm->aScopeID);
	vm->aActiveModules = simple_list_delete_gc(vm->pSimpleState,vm->aActiveModules);
	vm->aSetProperty = simple_list_delete_gc(vm->pSimpleState,vm->aSetProperty);
	vm->aCPointers = simple_list_delete_gc(vm->pSimpleState,vm->aCPointers);
	vm->aForStep = simple_list_delete_gc(vm->pSimpleState,vm->aForStep);
	vm->aLoadAddressScope = simple_list_delete_gc(vm->pSimpleState,vm->aLoadAddressScope);
	vm->aBeforeObjState = simple_list_delete_gc(vm->pSimpleState,vm->aBeforeObjState);
	vm->pLoadAddressScope = simple_list_delete_gc(vm->pSimpleState,vm->pLoadAddressScope);
	vm->aNewByteCodeItems = simple_list_delete_gc(vm->pSimpleState,vm->aNewByteCodeItems);
	/* Free Stack */
	for ( x = 0 ; x < SIMPLE_VM_STACK_SIZE ; x++ ) {
		simple_item_content_delete(&(vm->aStack[x]));
	}
	simple_state_free(vm->pSimpleState,vm->pByteCode);
	/* Delete Mutex */
	simple_vm_mutexdestroy(vm);
	/*
	**  Remove Dynamic Self Items 
	**  Delete Items 
	*/
	for ( x = 1 ; x <= simple_list_getsize(vm->aDynamicSelfItems) ; x++ ) {
		pRecord = simple_list_getlist(vm->aDynamicSelfItems,x);
		pItem = (Item *) simple_list_getpointer(pRecord,2);
		simple_state_free(vm->pSimpleState,pItem);
	}
	/* Delete List */
	vm->aDynamicSelfItems = simple_list_delete_gc(vm->pSimpleState,vm->aDynamicSelfItems);
	vm->pModulesName = simple_stsimple_delete_gc(vm->pSimpleState,vm->pModulesName);
	vm->pTrace = simple_stsimple_delete_gc(vm->pSimpleState,vm->pTrace);
	vm->pTraceData = simple_list_delete_gc(vm->pSimpleState,vm->pTraceData);
	/* Custom Global Scope (using Load Modules) */
	vm->aGlobalScopes = simple_list_delete_gc(vm->pSimpleState,vm->aGlobalScopes);
	vm->aActiveGlobalScopes = simple_list_delete_gc(vm->pSimpleState,vm->aActiveGlobalScopes);
	vm->pSimpleState->vm = NULL ;
	simple_state_free(vm->pSimpleState,vm);
	vm = NULL ;
	return vm ;
}

SIMPLE_API void simple_vm_loadcode ( VM *vm )
{
	int x,nSize  ;
	/*
	**  We may allocation double of the size that we need to avoid reallocation when we use executeCode() 
	**  executeCode() will check if there is a need to reallocation or not 
	**  This optimization increase the performance of applications that uses executeCode() 
	*/
	nSize = (simple_list_getsize(vm->pCode))*SIMPLE_VM_EXTRASIZE ;
	vm->pByteCode = (ByteCode *) simple_state_calloc(vm->pSimpleState,nSize,sizeof(ByteCode));
	if ( vm->pByteCode == NULL ) {
		printf( SIMPLE_OOM ) ;
		exit(0);
	}
	for ( x = 1 ; x <= simple_list_getsize(vm->pCode) ; x++ ) {
		simple_vm_tobytecode(vm,x);
	}
	vm->nEvalReallocationSize = nSize ;
}

void simple_vm_start ( SimpleState *pSimpleState,VM *vm )
{
	vm->pCode = pSimpleState->pSimpleGenCode ;
	vm->pFunctionsMap = pSimpleState->pSimpleFunctionsMap ;
	vm->pClassesMap = pSimpleState->pSimpleClassesMap ;
	vm->pModulessMap = pSimpleState->pSimpleModulessMap ;
	simple_vm_loadcode(vm);
	simple_vm_loadcfunctions(pSimpleState);
	/* Generate Items Array &  Hash Table */
	simple_list_genarray(pSimpleState->pSimpleCFunctions);
	simple_list_genhashtable2(pSimpleState->pSimpleCFunctions);
	if ( simple_list_getsize(vm->pCode) > 0 ) {
		vm->nPC = 1 ;
		simple_vm_mainloop(vm);
	}
}

void simple_vm_mainloop ( VM *vm )
{
	#if SIMPLE_VMSHOWOPCODE
	/* Preprocessor Allows showing the OPCODE */
	if ( vm->pSimpleState->nPrintInstruction ) {
		do {
			simple_vm_fetch2(vm);
			if ( vm->nPC <= vm->nEvalReturnPC ) {
				vm->nEvalReturnPC = 0 ;
				break ;
			}
		} while (vm->nPC <= simple_list_getsize(vm->pCode))  ;
	}
	else {
		do {
			simple_vm_fetch(vm);
			if ( vm->nPC <= vm->nEvalReturnPC ) {
				vm->nEvalReturnPC = 0 ;
				break ;
			}
		} while (vm->nPC <= simple_list_getsize(vm->pCode))  ;
	}
	#else
	do {
		simple_vm_fetch(vm);
		if ( vm->nPC <= vm->nEvalReturnPC ) {
			vm->nEvalReturnPC = 0 ;
			break ;
		}
	} while (vm->nPC <= simple_list_getsize(vm->pCode))  ;
	#endif
}

void simple_vm_fetch ( VM *vm )
{
	vm->pByteCodeIR = vm->pByteCode + vm->nPC - 1 ;
	vm->nOPCode = SIMPLE_VM_IR_OPCODE ;
	vm->nPC++ ;
	simple_vm_execute(vm);
	if ( vm->nSP > SIMPLE_VM_STACK_CHECKOVERFLOW ) {
		simple_vm_error(vm,SIMPLE_VM_ERROR_STACKOVERFLOW);
	}
}

void simple_vm_fetch2 ( VM *vm )
{
	vm->pByteCodeIR = vm->pByteCode + vm->nPC - 1 ;
	vm->nOPCode = SIMPLE_VM_IR_OPCODE ;
	#if SIMPLE_VMSHOWOPCODE
	if ( vm->pSimpleState->nPrintInstruction ) {
		print_line();
		printf( "\nVM Pointer  : %p  " , (void *) vm ) ;
		printf( "\nOperation  : %s  " , SIMPLE_IC_OP[vm->nOPCode] ) ;
		printf( "\nPC         : %d  " ,vm->nPC ) ;
		printf( "\nLine Number    : %d  , File %s \n " ,vm->nLineNumber,vm->cFileName ) ;
		if ( (vm->nOPCode == ICO_PUSHC) || (vm->nOPCode == ICO_LOADADDRESS) || (vm->nOPCode == ICO_LOADBLOCK) ) {
			printf( "\nData       : %s \n",SIMPLE_VM_IR_READC ) ;
		}
	}
	#endif
	vm->nPC++ ;
	simple_vm_execute(vm);
	#if SIMPLE_VMSHOWOPCODE
	if ( vm->pSimpleState->nPrintInstruction ) {
		printf( "\nSP (After) : %d  - FuncSP : %d \n LineNumber %d \n" , (int) vm->nSP,vm->nFuncSP,vm->nLineNumber ) ;
		print_line();
	}
	#endif
	if ( vm->nSP > SIMPLE_VM_STACK_CHECKOVERFLOW ) {
		simple_vm_error(vm,SIMPLE_VM_ERROR_STACKOVERFLOW);
	}
}

void simple_vm_execute ( VM *vm )
{
	switch ( vm->nOPCode ) {
		/* Stack and Variables */
		case ICO_PUSHC :
			SIMPLE_VM_STACK_PUSHC ;
			break ;
		case ICO_PUSHN :
			SIMPLE_VM_STACK_PUSHN ;
			break ;
		case ICO_PUSHV :
			simple_vm_pushv(vm);
			break ;
		case ICO_LOADADDRESS :
			simple_vm_loadaddress(vm);
			break ;
		case ICO_ASSIGNMENT :
			simple_vm_assignment(vm);
			break ;
		case ICO_INC :
			simple_vm_inc(vm);
			break ;
		case ICO_LOADAPUSHV :
			simple_vm_loadapushv(vm);
			break ;
		case ICO_NEWLINE :
			simple_vm_newline(vm);
			break ;
		case ICO_FREESTACK :
			simple_vm_freestack(vm);
			break ;
		case ICO_FILENAME :
			simple_vm_setfilename(vm);
			break ;
		case ICO_FREELOADASCOPE :
			simple_vm_freeloadaddressscope(vm);
			break ;
		/* Jump */
		case ICO_JUMP :
			simple_vm_jump(vm);
			break ;
		case ICO_JUMPZERO :
			simple_vm_jumpzero(vm);
			break ;
		case ICO_JUMPFOR :
			simple_vm_jumpfor(vm);
			break ;
		case ICO_JUMPONE :
			simple_vm_jumpone(vm);
			break ;
		case ICO_JUMPZERO2 :
			simple_vm_jumpzero2(vm);
			break ;
		case ICO_JUMPONE2 :
			simple_vm_jumpone2(vm);
			break ;
		/* Compare */
		case ICO_LESSEQUAL :
			simple_vm_lessequal(vm);
			break ;
		case ICO_EQUAL :
			simple_vm_equal(vm);
			break ;
		case ICO_LESS :
			simple_vm_less(vm);
			break ;
		case ICO_GREATER :
			simple_vm_greater(vm);
			break ;
		case ICO_GREATEREQUAL :
			simple_vm_greaterequal(vm);
			break ;
		case ICO_NOTEQUAL :
			simple_vm_notequal(vm);
			break ;
		/* Math */
		case ICO_SUM :
			simple_vm_sum(vm);
			break ;
		case ICO_SUB :
			simple_vm_sub(vm);
			break ;
		case ICO_MUL :
			simple_vm_mul(vm);
			break ;
		case ICO_DIV :
			simple_vm_div(vm);
			break ;
		case ICO_MOD :
			simple_vm_mod(vm);
			break ;
		case ICO_NEG :
			simple_vm_neg(vm);
			break ;
		case ICO_PLUSPLUS :
			simple_vm_plusplus(vm);
			break ;
		case ICO_MINUSMINUS :
			simple_vm_minusminus(vm);
			break ;
		/* Logic */
		case ICO_AND :
			simple_vm_and(vm);
			break ;
		case ICO_OR :
			simple_vm_or(vm);
			break ;
		case ICO_NOT :
			simple_vm_not(vm);
			break ;
		/* Lists */
		case ICO_LISTSTART :
			simple_vm_liststart(vm);
			break ;
		case ICO_LISTITEM :
			simple_vm_listitem(vm);
			break ;
		case ICO_LISTEND :
			simple_vm_listend(vm);
			break ;
		case ICO_LOADINDEXADDRESS :
			simple_vm_loadindexaddress(vm);
			break ;
		/* Functions */
		case ICO_LOADBLOCK :
			simple_vm_loadfunc(vm);
			break ;
		case ICO_CALL :
			simple_vm_call(vm);
			break ;
		case ICO_RETURN :
			simple_vm_return(vm);
			break ;
		case ICO_RETNULL :
			simple_vm_returnnull(vm);
			break ;
		case ICO_RETFROMEVAL :
			simple_vm_returneval(vm);
			break ;
		case ICO_RETITEMREF :
			simple_vm_retitemref(vm);
			break ;
		case ICO_NEWBLOCK :
			simple_vm_newfunc(vm);
			break ;
		case ICO_BLOCKFLAG :
			simple_vm_blockflag(vm);
			break ;
		case ICO_BLOCKEXE :
			vm->nFuncExecute++ ;
			break ;
		case ICO_ENDBLOCKEXE :
			simple_vm_endfuncexec(vm);
			break ;
		case ICO_ANONYMOUS :
			simple_vm_anonymous(vm);
			break ;
		/* User Interface */
		case ICO_DISPLAY :
			simple_vm_see(vm);
			break ;
		case ICO_GIVE :
			simple_vm_give(vm);
			break ;
		/* End Program - Exit from Loop - Loop (Continue) */
		case ICO_BYE :
			simple_vm_bye(vm);
			break ;
		case ICO_EXITMARK :
			simple_vm_exitmark(vm);
			break ;
		case ICO_POPEXITMARK :
			simple_vm_popexitmark(vm);
			break ;
		case ICO_EXIT :
			simple_vm_exit(vm,1);
			break ;
		case ICO_LOOP :
			simple_vm_exit(vm,2);
			break ;
		/* For Better Performance */
		case ICO_PUSHP :
			simple_vm_pushp(vm);
			break ;
		case ICO_INCP :
			simple_vm_incp(vm);
			break ;
		case ICO_PUSHPV :
			simple_vm_pushpv(vm);
			break ;
		case ICO_INCJUMP :
			simple_vm_incjump(vm);
			break ;
		case ICO_INCPJUMP :
			simple_vm_incpjump(vm);
			break ;
		case ICO_JUMPVARLENUM :
			simple_vm_jumpvarlenum(vm);
			break ;
		case ICO_JUMPVARPLENUM :
			simple_vm_jumpvarplenum(vm);
			break ;
		case ICO_LOADBLOCKP :
			simple_vm_loadfuncp(vm);
			break ;
		case ICO_PUSHPLOCAL :
			simple_vm_pushplocal(vm);
			break ;
		case ICO_INCLPJUMP :
			simple_vm_inclpjump(vm);
			break ;
		case ICO_JUMPVARLPLENUM :
			simple_vm_jumpvarlplenum(vm);
			break ;
		case ICO_INCPJUMPSTEP1 :
			simple_vm_incpjumpstep1(vm);
			break ;
		case ICO_JUMPVARPLENUMSTEP1 :
			simple_vm_jumpvarplenumstep1(vm);
			break ;
		/* Try-Catch-Done */
		case ICO_TRY :
			simple_vm_try(vm);
			break ;
		case ICO_DONE :
			simple_vm_done(vm);
			break ;
		/* Duplicate and Range */
		case ICO_DUPLICATE :
			simple_vm_dup(vm);
			break ;
		case ICO_RANGE :
			simple_vm_range(vm);
			break ;
		/* OOP */
		case ICO_NEWOBJ :
			simple_vm_oop_newobj(vm);
			break ;
		case ICO_SETSCOPE :
			simple_vm_oop_setscope(vm);
			break ;
		case ICO_LOADSUBADDRESS :
			simple_vm_oop_property(vm);
			break ;
		case ICO_LOADMETHOD :
			simple_vm_oop_loadmethod(vm);
			break ;
		case ICO_AFTERCALLMETHOD :
			simple_vm_oop_aftercallmethod(vm);
			break ;
		case ICO_AFTERCALLMETHOD2 :
			simple_vm_oop_aftercallmethod(vm);
			break ;
		case ICO_NEWCLASS :
			simple_vm_oop_newclass(vm);
			break ;
		case ICO_BRACESTART :
			simple_vm_oop_bracestart(vm);
			break ;
		case ICO_BRACEEND :
			simple_vm_oop_braceend(vm);
			break ;
		case ICO_IMPORT :
			simple_vm_oop_import(vm);
			break ;
		case ICO_PRIVATE :
			vm->nPrivateFlag = 1 ;
			break ;
		case ICO_SETPROPERTY :
			simple_vm_oop_setproperty(vm);
			break ;
		case ICO_CALLCLASSINIT :
			simple_vm_callclassinit(vm);
			break ;
		/* Other */
		case ICO_SETREFERENCE :
			simple_vm_setreference(vm);
			break ;
		case ICO_KILLREFERENCE :
			simple_vm_gc_killreference(vm);
			break ;
		case ICO_ASSIGNMENTPOINTER :
			simple_vm_assignmentpointer(vm);
			break ;
		case ICO_BEFOREEQUAL :
			vm->nBeforeEqual = SIMPLE_VM_IR_READI ;
			break ;
		/* Bitwise Operators */
		case ICO_BITAND :
			simple_vm_bitand(vm);
			break ;
		case ICO_BITOR :
			simple_vm_bitor(vm);
			break ;
		case ICO_BITXOR :
			simple_vm_bitxor(vm);
			break ;
		case ICO_BITNOT :
			simple_vm_bitnot(vm);
			break ;
		case ICO_BITSHL :
			simple_vm_bitshl(vm);
			break ;
		case ICO_BITSHR :
			simple_vm_bitshr(vm);
			break ;
		/* For Step */
		case ICO_STEPNUMBER :
			simple_vm_stepnumber(vm);
			break ;
		case ICO_POPSTEP :
			simple_vm_popstep(vm);
			break ;
		case ICO_LOADAFIRST :
			simple_vm_loadaddressfirst(vm);
			break ;
		/* Custom Global Scope */
		case ICO_NEWGLOBALSCOPE :
			simple_vm_newglobalscope(vm);
			break ;
		case ICO_ENDGLOBALSCOPE :
			simple_vm_endglobalscope(vm);
			break ;
		case ICO_SETGLOBALSCOPE :
			simple_vm_setglobalscope(vm);
			break ;
	}
}

SIMPLE_API void simple_vm_error ( VM *vm,const char *cStr )
{
	List *pList  ;
	/* Check if we have active error */
	if ( vm->nActiveError ) {
		return ;
	}
	vm->nActiveError = 1 ;
	/* Check BraceError() */
	if ( (simple_list_getsize(vm->pObjState) > 0) && (simple_vm_oop_callmethodinsideclass(vm) == 0 ) && (vm->nCallMethod == 0) ) {
		if ( simple_vm_findvar(vm,"self") ) {
			pList = simple_vm_oop_getobj(vm);
			SIMPLE_VM_STACK_POP ;
			if ( simple_vm_oop_isobject(pList) ) {
				if ( simple_vm_oop_ismethod(vm, pList,"braceerror") ) {
					simple_list_setstsimple_gc(vm->pSimpleState,simple_list_getlist(simple_vm_getglobalscope(vm),6),3,cStr);
					simple_vm_runcode(vm,"braceerror()");
					vm->nActiveError = 0 ;
					return ;
				}
			}
		}
	}
	if ( simple_list_getsize(vm->pTry) == 0 ) {
		if ( vm->lHideErrorMsg == 0 ) {
			simple_vm_showerrormessage(vm,cStr);
		}
		/* Trace */
		vm->nActiveError = 0 ;
		simple_vm_traceevent(vm,SIMPLE_VM_TRACEEVENT_ERROR);
		if ( vm->lPassError  == 1 ) {
			vm->lPassError = 0 ;
			return ;
		}
		vm->nActiveError = 1 ;
		exit(0);
	}
	/*
	**  Check Eval In Scope 
	**  When we have ringvm_evalinscope() We don't support try/catch 
	**  We just display the error message and continue 
	*/
	if ( vm->nEvalInScope ) {
		simple_vm_showerrormessage(vm,cStr);
		vm->nActiveError = 0 ;
		simple_vm_freestack(vm);
		return ;
	}
	simple_vm_catch(vm,cStr);
	vm->nActiveError = 0 ;
}

int simple_vm_eval ( VM *vm,const char *cStr )
{
	int nPC,nCont,nLastPC,nRunVM,x,nSize  ;
	Scanner *pScanner  ;
	int aPara[3]  ;
	ByteCode *pByteCode  ;
	nSize = strlen( cStr ) ;
	if ( nSize == 0 ) {
		return 0 ;
	}
	nPC = vm->nPC ;
	/* Add virtual file name */
	simple_list_addstsimple_gc(vm->pSimpleState,vm->pSimpleState->pSimpleFilesList,"executeCode");
	simple_list_addstsimple_gc(vm->pSimpleState,vm->pSimpleState->pSimpleFilesStack,"executeCode");
	pScanner = simple_scanner_new(vm->pSimpleState);
	for ( x = 0 ; x < nSize ; x++ ) {
		simple_scanner_readchar(pScanner,cStr[x]);
	}
	nCont = simple_scanner_checklasttoken(pScanner);
	/* Add Token "End of Line" to the end of any program */
	simple_scanner_endofline(pScanner);
	nLastPC = simple_list_getsize(vm->pCode);
	/* Get Functions/Classes Size before change by parser */
	aPara[0] = nLastPC ;
	aPara[1] = simple_list_getsize(vm->pFunctionsMap) ;
	aPara[2] = simple_list_getsize(vm->pClassesMap) ;
	/* Call Parser */
	if ( nCont == 1 ) {
		vm->pSimpleState->lNoLineNumber = 1 ;
		nRunVM = simple_parser_start(pScanner->Tokens,vm->pSimpleState);
		vm->pSimpleState->lNoLineNumber = 0 ;
	} else {
		simple_vm_error(vm,"Error in executeCode!");
		simple_scanner_delete(pScanner);
		return 0 ;
	}
	if ( nRunVM == 1 ) {
		/*
		**  Generate Code 
		**  Generate  Hash Table 
		*/
		simple_list_genhashtable2(vm->pFunctionsMap);
		if ( vm->nEvalCalledFromSimpleCode ) {
			simple_scanner_addreturn3(vm->pSimpleState,aPara);
		}
		else {
			simple_scanner_addreturn2(vm->pSimpleState);
		}
		simple_vm_blockflag2(vm,nPC);
		vm->nPC = nLastPC+1 ;
		if ( simple_list_getsize(vm->pCode)  > vm->nEvalReallocationSize ) {
			pByteCode = (ByteCode *) simple_state_realloc(vm->pSimpleState,vm->pByteCode , sizeof(ByteCode) * simple_list_getsize(vm->pCode));
			if ( pByteCode == NULL ) {
				printf( SIMPLE_OOM ) ;
				simple_scanner_delete(pScanner);
				exit(0);
			}
			vm->pByteCode = pByteCode ;
			if ( vm->nEvalCalledFromSimpleCode ) {
				/* Here executeCode() function is called from .sim files ( not by the VM for setter/getter/operator overloading) */
				vm->nEvalReallocationFlag = 1 ;
			}
		}
		else {
			vm->nEvalReallocationFlag = 0 ;
		}
		/* Load New Code */
		for ( x = vm->nPC ; x <= simple_list_getsize(vm->pCode) ; x++ ) {
			simple_vm_tobytecode(vm,x);
		}
		/*
		**  The mainloop will be called again 
		**  We do this to execute executeCode instructions directly 
		**  This is necessary when we have GUI library that takes the event loop 
		**  Then an event uses the executeCode() function to execute instructions 
		**  We don't call the main loop here we call it from simple_vm_call() 
		**  We do this to execute the executeCode() instructions in the correct scope 
		**  Because when we call a C Function like executeCode() we have parameters scope 
		**  Before we call the main loop from simple_vm_call the parameters scope will be deleted 
		**  And the local scope will be restored so we can use it from executeCode() 
		**  Update ReallocationSize 
		*/
		vm->nEvalReallocationSize = vm->nEvalReallocationSize - (simple_list_getsize(vm->pCode)-nLastPC) ;
	} else {
		simple_vm_error(vm,"Error in executeCode!");
		simple_scanner_delete(pScanner);
		return 0 ;
	}
	simple_scanner_delete(pScanner);
	simple_list_deletelastitem_gc(vm->pSimpleState,vm->pSimpleState->pSimpleFilesList);
	simple_list_deletelastitem_gc(vm->pSimpleState,vm->pSimpleState->pSimpleFilesStack);
	return nRunVM ;
}

void simple_vm_tobytecode ( VM *vm,int x )
{
	List *pIR  ;
	int x2  ;
	ByteCode *pByteCode  ;
	Item *pItem  ;
	pByteCode = vm->pByteCode + x - 1 ;
	pIR = simple_list_getlist(vm->pCode,x);
	pByteCode->nSize = simple_list_getsize(pIR) ;
	#if SIMPLE_SHOWICFINAL
	pByteCode->pList = pIR ;
	#endif
	/* Check Instruction Size */
	if ( simple_list_getsize(pIR) > SIMPLE_VM_BC_ITEMS_COUNT ) {
		printf( SIMPLE_LONGINSTRUCTION ) ;
		printf( "In File : %s  - Byte-Code PC : %d  ",vm->cFileName,x ) ;
		exit(0);
	}
	for ( x2 = 1 ; x2 <= simple_list_getsize(pIR) ; x2++ ) {
		pItem = simple_list_getitem(pIR,x2) ;
		pByteCode->aData[x2-1] = pItem ;
		/* Avoid Performance Instuctions (Happens when called from New Thread) */
		if ( x2 == 1 ) {
			switch ( pItem->data.iNumber ) {
				case ICO_PUSHPLOCAL :
					pItem->data.iNumber = ICO_LOADADDRESS ;
					break ;
				case ICO_JUMPVARLPLENUM :
					pItem->data.iNumber = ICO_JUMPVARLENUM ;
					break ;
				case ICO_INCLPJUMP :
					pItem->data.iNumber = ICO_INCJUMP ;
					break ;
			}
		}
	}
	/* Clear Other Items */
	for ( x2 = simple_list_getsize(pIR)+1 ; x2 <= SIMPLE_VM_BC_ITEMS_COUNT ; x2++ ) {
		pByteCode->aData[x2-1] = NULL ;
	}
}

void simple_vm_returneval ( VM *vm )
{
	int aPara[3]  ;
	ByteCode *pByteCode  ;
	simple_vm_return(vm);
	aPara[0] = SIMPLE_VM_IR_READIVALUE(1) ;
	aPara[1] = SIMPLE_VM_IR_READIVALUE(2) ;
	aPara[2] = SIMPLE_VM_IR_READIVALUE(3) ;
	if ( ( vm->nRetEvalDontDelete == 0 ) && (aPara[1] == simple_list_getsize(vm->pFunctionsMap)) && (aPara[2] == simple_list_getsize(vm->pClassesMap)) ) {
		/*
		**  The code interpreted by eval doesn't add new functions or new classes 
		**  This means that the code can be deleted without any problems 
		**  We do that to avoid memory leaks 
		*/
		while ( simple_list_getsize(vm->pCode) != aPara[0] ) {
			simple_list_deletelastitem_gc(vm->pSimpleState,vm->pCode);
		}
		if ( vm->nEvalReallocationFlag == 1 ) {
			vm->nEvalReallocationFlag = 0 ;
			pByteCode = (ByteCode *) simple_state_realloc(vm->pSimpleState,vm->pByteCode , sizeof(ByteCode) * simple_list_getsize(vm->pCode));
			if ( pByteCode == NULL ) {
				printf( SIMPLE_OOM ) ;
				exit(0);
			}
			vm->pByteCode = pByteCode ;
		}
	}
	/*
	**  nEvalReturnPC is checked by the simple_vm_mainloop to end the loop 
	**  if the vm->nPC becomes <= vm->nEvalReturnPC the loop will be terminated 
	**  Remember that this is just a sub loop (another main loop) created after using executeCode() 
	**  If we don't terminate the sub main loop , this is just an extra overhead 
	**  Also terminating the sub main loop is a must when we do GUI programming 
	**  Because in GUI programming, the main loop calls the GUI Main Loop 
	**  During GUI main loop when event happens that calls a ring code 
	**  Eval will be used and a sub main loop will be executed 
	**  If we don't terminate the sub main loop, we can't return to the GUI Main Loop 
	**  It's necessary to return to the GUI main loop 
	**  When the GUI Main Loop Ends, we return to the Simple Main Loop 
	*/
	vm->nEvalReturnPC = aPara[0] ;
}

void simple_vm_error2 ( VM *vm,const char *cStr,const char *cStr2 )
{
	String *pError  ;
	pError = simple_stsimple_new_gc(vm->pSimpleState,cStr);
	simple_stsimple_add_gc(vm->pSimpleState,pError,": ");
	simple_stsimple_add_gc(vm->pSimpleState,pError,cStr2);
	simple_vm_error(vm,simple_stsimple_get(pError));
	simple_stsimple_delete_gc(vm->pSimpleState,pError);
}

void simple_vm_newbytecodeitem ( VM *vm,int x )
{
	Item *pItem  ;
	simple_list_addint_gc(vm->pSimpleState,vm->aNewByteCodeItems,0);
	pItem = simple_list_getitem(vm->aNewByteCodeItems,simple_list_getsize(vm->aNewByteCodeItems));
	SIMPLE_VM_IR_ITEM(x) = pItem ;
}

SIMPLE_API void simple_vm_runcode ( VM *vm,const char *cStr )
{
	int nEvalReturnPC,nEvalReallocationFlag,nPC,nRunVM,nSP,nFuncSP,nLineNumber,nRetEvalDontDelete  ;
	List *pStackList  ;
	/* Save state to take in mind nested events execution */
	vm->nRunCode++ ;
	nEvalReturnPC = vm->nEvalReturnPC ;
	nEvalReallocationFlag = vm->nEvalReallocationFlag ;
	nPC = vm->nPC ;
	nSP = vm->nSP ;
	nFuncSP = vm->nFuncSP ;
	pStackList = simple_vm_savestack(vm);
	nLineNumber = vm->nLineNumber ;
	nRetEvalDontDelete = vm->nRetEvalDontDelete ;
	simple_vm_mutexlock(vm);
	vm->nEvalCalledFromSimpleCode = 1 ;
	/* Check removing the new byte code */
	if ( vm->nRunCode != 1 ) {
		/* We have nested events that call this function */
		vm->nRetEvalDontDelete = 1 ;
	}
	nRunVM = simple_vm_eval(vm,cStr);
	vm->nEvalCalledFromSimpleCode = 0 ;
	simple_vm_mutexunlock(vm);
	if ( nRunVM ) {
		vm->nFuncExecute = 0 ;
		vm->nFuncExecute2 = 0 ;
		simple_vm_mainloop(vm);
	}
	/* Restore state to take in mind nested events execution */
	vm->nRunCode-- ;
	vm->nEvalReturnPC = nEvalReturnPC ;
	vm->nEvalReallocationFlag = nEvalReallocationFlag ;
	vm->nPC = nPC ;
	if ( vm->nRunCode != 0 ) {
		/* It's a nested event (Here we don't care about the output and we can restore the stack) */
		simple_vm_restorestack(vm,pStackList);
	}
	/* Here we free the list because, restorestack() don't free it */
	simple_list_delete_gc(vm->pSimpleState,pStackList);
	/* Restore Stack to avoid Stack Overflow */
	vm->nSP = nSP ;
	vm->nFuncSP = nFuncSP ;
	vm->nLineNumber = nLineNumber ;
	vm->nRetEvalDontDelete = nRetEvalDontDelete ;
}

void simple_vm_init ( SimpleState *pSimpleState )
{
	Scanner *pScanner  ;
	VM *vm  ;
	int nRunVM,nFreeFilesList = 0 ;
	/* Check file */
	if ( pSimpleState->pSimpleFilesList == NULL ) {
		pSimpleState->pSimpleFilesList = simple_list_new_gc(pSimpleState,0);
		pSimpleState->pSimpleFilesStack = simple_list_new_gc(pSimpleState,0);
		nFreeFilesList = 1 ;
	}
	simple_list_addstsimple_gc(pSimpleState,pSimpleState->pSimpleFilesList,"Simple_EmbeddedCode");
	simple_list_addstsimple_gc(pSimpleState,pSimpleState->pSimpleFilesStack,"Simple_EmbeddedCode");
	/* Read File */
	pScanner = simple_scanner_new(pSimpleState);
	/* Add Token "End of Line" to the end of any program */
	simple_scanner_endofline(pScanner);
	/* Call Parser */
	nRunVM = simple_parser_start(pScanner->Tokens,pSimpleState);
	simple_scanner_delete(pScanner);
	/* Files List */
	simple_list_deleteitem_gc(pSimpleState,pSimpleState->pSimpleFilesStack,simple_list_getsize(pSimpleState->pSimpleFilesStack));
	if ( nFreeFilesList ) {
		/* Run the Program */
		if ( nRunVM == 1 ) {
			/* Add return to the end of the program */
			simple_scanner_addreturn(pSimpleState);
			vm = simple_vm_new(pSimpleState);
			simple_vm_start(pSimpleState,vm);
			pSimpleState->vm = vm ;
		}
	}
	return ;
}

void simple_vm_retitemref ( VM *vm )
{
	List *pList  ;
	vm->nRetItemRef++ ;
	/* We free the stack to avoid effects on aLoadAddressScope which is used by isstackpointertoobjstate */
	simple_vm_freestack(vm);
	/*
	**  Check if we are in the operator method to increment the counter again 
	**  We do this to avoid another PUSHV on the list item 
	**  The first one after return expression in the operator method 
	**  The second one before return from executeCode() that is used by operator overloading 
	**  This to avoid using & two times like  &  & 
	*/
	if ( simple_list_getsize(vm->pFuncCallList) > 0 ) {
		pList = simple_list_getlist(vm->pFuncCallList,simple_list_getsize(vm->pFuncCallList));
		if ( strcmp(simple_list_getstring(pList,SIMPLE_BLOCKCL_NAME),"operator") == 0 ) {
			vm->nRetItemRef++ ;
		}
	}
}

void simple_vm_printstack ( VM *vm )
{
	int x,nSP  ;
	printf( "\n*****************************************\n" ) ;
	printf( "Stack Size %d \n",vm->nSP ) ;
	nSP = vm->nSP ;
	if ( nSP > 0 ) {
		for ( x = 1 ; x <= nSP ; x++ ) {
			/* Print Values */
			if ( SIMPLE_VM_STACK_ISSTRING ) {
				printf( "(String) : %s  \n",SIMPLE_VM_STACK_READC ) ;
			}
			else if ( SIMPLE_VM_STACK_ISNUMBER ) {
				printf( "(Number) : %f  \n",SIMPLE_VM_STACK_READN ) ;
			}
			else if ( SIMPLE_VM_STACK_ISPOINTER ) {
				printf( "(Pointer) : %p  \n",SIMPLE_VM_STACK_READP ) ;
				if ( SIMPLE_VM_STACK_OBJTYPE == SIMPLE_OBJTYPE_VARIABLE ) {
					printf( "(Pointer Type) : Variable \n" ) ;
					simple_list_print((List *) SIMPLE_VM_STACK_READP);
				}
				else if ( SIMPLE_VM_STACK_OBJTYPE ==SIMPLE_OBJTYPE_LISTITEM ) {
					printf( "(Pointer Type) : ListItem \n" ) ;
					simple_item_print((Item *) SIMPLE_VM_STACK_READP);
				}
			}
			SIMPLE_VM_STACK_POP ;
			printf( "\n*****************************************\n" ) ;
		}
	}
}

void simple_vm_callclassinit ( VM *vm )
{
	if ( SIMPLE_VM_IR_READIVALUE(1) ) {
		vm->nCallClassInit++ ;
	}
	else {
		vm->nCallClassInit-- ;
	}
}

SIMPLE_API void simple_vm_showerrormessage ( VM *vm,const char *cStr )
{
	int x,lFunctionCall  ;
	List *pList  ;
	const char *cFile  ;
	/* CGI Support */
	simple_state_cgiheader(vm->pSimpleState);
	/* Print the Error Message */
	printf( "\nLine %d -> %s \n",vm->nLineNumber,cStr ) ;
	/* Print Calling Information */
	lFunctionCall = 0 ;
	for ( x = simple_list_getsize(vm->pFuncCallList) ; x >= 1 ; x-- ) {
		pList = simple_list_getlist(vm->pFuncCallList,x);
		/*
		**  If we have ICO_LoadFunc but not ICO_CALL then we need to pass 
		**  ICO_LOADBLOCK is executed, but still ICO_CALL is not executed! 
		*/
		if ( simple_list_getsize(pList) < SIMPLE_BLOCKCL_CALLERPC ) {
			continue ;
		}
		if ( simple_list_getint(pList,SIMPLE_BLOCKCL_TYPE) == SIMPLE_BLOCKTYPE_SCRIPT ) {
			/*
			**  Prepare Message 
			**  In 
			*/
			printf( "\tat " ) ;
			/* Method or Function */
			/*if ( simple_list_getint(pList,SIMPLE_BLOCKCL_METHODORBLOCK) ) {
				printf( "method " ) ;
			}
			else {
				printf( "block " ) ;
			}*/
			/* Function Name */
			printf( "%s",simple_list_getstring(pList,SIMPLE_BLOCKCL_NAME) ) ;
			/* Adding () */
			printf( "() in file " ) ;
			/* File Name */
			if ( lFunctionCall == 1 ) {
				cFile = (const char *) simple_list_getpointer(pList,SIMPLE_BLOCKCL_NEWFILENAME) ;
			}
			else {
				if ( vm->nInClassRegion ) {
					cFile = vm->cFileNameInClassRegion ;
				}
				else {
					cFile = vm->cFileName ;
				}
			}
			printf( "%s",file_real_name(cFile) ) ;
			/* Called From */
			printf( "\n\tat line %d ",simple_list_getint(pList,SIMPLE_BLOCKCL_LINENUMBER) ) ;
			lFunctionCall = 1 ;
		}
		else {
			printf( "In %s ",file_real_name(simple_list_getstring(pList,SIMPLE_BLOCKCL_NAME)) ) ;
		}
	}
	if ( lFunctionCall ) {
		printf( "in file %s ",file_real_name(simple_list_getstring(vm->pSimpleState->pSimpleFilesList,1) )) ;
	}
	else {
		if ( vm->nInClassRegion ) {
			cFile = vm->cFileNameInClassRegion ;
		}
		else {
			cFile = file_real_name(vm->cFileName) ;
		}
		printf( "\tin file %s ",cFile ) ;
	}
}

void simple_vm_setfilename ( VM *vm )
{
	if ( vm->nInClassRegion ) {
		/*
		**  We are using special attribute for this region to avoid save/restore file name 
		**  If we used vm->cFileName we could get problem in finding classes and moduless 
		*/
		vm->cFileNameInClassRegion = SIMPLE_VM_IR_READC ;
		return ;
	}
	vm->cPrevFileName = vm->cFileName ;
	vm->cFileName = SIMPLE_VM_IR_READC ;
}

void simple_vm_loadaddressfirst ( VM *vm )
{
	vm->nFirstAddress = 1 ;
	simple_vm_loadaddress(vm);
	vm->nFirstAddress = 0 ;
}

void simple_vm_endfuncexec ( VM *vm )
{
	if ( vm->nFuncExecute > 0 ) {
		vm->nFuncExecute-- ;
	}
}

void simple_vm_addglobalvariables ( VM *vm )
{
	List *pList  ;
	int x  ;
	/*
	**  Add Variables 
	**  We write variable name in lower case because Identifiers is converted to lower by Compiler(Scanner) 
	*/
	simple_vm_addnewnumbervar(vm,"true",1);
	simple_vm_addnewnumbervar(vm,"false",0);
	simple_vm_addnewstringvar(vm,"crlf","\n");
	simple_vm_addnewstringvar(vm,"null","");
	simple_vm_addnewpointervar(vm,"simple_gettemp_var",NULL,0);
	simple_vm_addnewstringvar(vm,"__err__","NULL");
	simple_vm_addnewpointervar(vm,"simple_settemp_var",NULL,0);
	simple_vm_addnewnumbervar(vm,"simple_tempflag_var",0);
	simple_vm_addnewcpointervar(vm,"stdin",stdin,"file");
	simple_vm_addnewcpointervar(vm,"stdout",stdout,"file");
	simple_vm_addnewcpointervar(vm,"stderr",stderr,"file");
	simple_vm_addnewpointervar(vm,"this",NULL,0);
	simple_vm_addnewstringvar(vm,"tab","\t");
	simple_vm_addnewstringvar(vm,"cr","\r");
	/* Add Command Line Parameters */
	pList = simple_vm_newvar2(vm,"sysargv",vm->pActiveMem);
	simple_list_setint_gc(vm->pSimpleState,pList,SIMPLE_VAR_TYPE,SIMPLE_VM_LIST);
	simple_list_setlist_gc(vm->pSimpleState,pList,SIMPLE_VAR_VALUE);
	pList = simple_list_getlist(pList,SIMPLE_VAR_VALUE);
	for ( x = 0 ; x < vm->pSimpleState->argc ; x++ ) {
		simple_list_addstsimple_gc(vm->pSimpleState,pList,vm->pSimpleState->argv[x]);
	}
}
/* Threads */

SIMPLE_API void simple_vm_mutexfunctions ( VM *vm,void *(*pFunc)(void),void (*pFuncLock)(void *),void (*pFuncUnlock)(void *),void (*pFuncDestroy)(void *) )
{
	if ( vm->pMutex == NULL ) {
		vm->pMutex = pFunc() ;
		vm->pFuncMutexLock = pFuncLock ;
		vm->pFuncMutexUnlock = pFuncUnlock ;
		vm->pFuncMutexDestroy = pFuncDestroy ;
	}
}

SIMPLE_API void simple_vm_mutexlock ( VM *vm )
{
	if ( vm->pMutex != NULL ) {
		vm->pFuncMutexLock(vm->pMutex);
	}
}

SIMPLE_API void simple_vm_mutexunlock ( VM *vm )
{
	if ( vm->pMutex != NULL ) {
		vm->pFuncMutexUnlock(vm->pMutex);
	}
}

SIMPLE_API void simple_vm_mutexdestroy ( VM *vm )
{
	if ( vm->pMutex != NULL ) {
		vm->pFuncMutexDestroy(vm->pMutex);
		vm->pMutex = NULL ;
	}
}

SIMPLE_API void simple_vm_runcodefromthread ( VM *vm,const char *cStr )
{
	SimpleState *pState  ;
	List *pList,*pList2,*pList3,*pList4,*pList5  ;
	Item *pItem  ;
	/* Create the SimpleState */
	pState = simple_state_init();
	pState->nPrintInstruction = vm->pSimpleState->nPrintInstruction ;
	/* Share the same Mutex between VMs */
	simple_vm_mutexlock(vm);
	pState->vm->pMutex = vm->pMutex ;
	pState->vm->pFuncMutexDestroy = vm->pFuncMutexDestroy ;
	pState->vm->pFuncMutexLock = vm->pFuncMutexLock ;
	pState->vm->pFuncMutexUnlock = vm->pFuncMutexUnlock ;
	/* Share the global scope between threads */
	pItem = pState->vm->pMem->pFirst->pValue ;
	pState->vm->pMem->pFirst->pValue = vm->pMem->pFirst->pValue ;
	/* Save the state */
	pList = pState->vm->pCode ;
	pList2 = pState->vm->pFunctionsMap ;
	pList3 = pState->vm->pClassesMap ;
	pList4 = pState->vm->pModulessMap ;
	pList5 = pState->vm->pCFunctionsList ;
	/* Share the code between the VMs */
	pState->vm->pFunctionsMap = vm->pSimpleState->pSimpleFunctionsMap ;
	pState->vm->pClassesMap = vm->pSimpleState->pSimpleClassesMap ;
	pState->vm->pModulessMap = vm->pSimpleState->pSimpleModulessMap ;
	pState->vm->pCFunctionsList = vm->pCFunctionsList ;
	pState->pSimpleFunctionsMap = vm->pSimpleState->pSimpleFunctionsMap ;
	pState->pSimpleClassesMap = vm->pSimpleState->pSimpleClassesMap ;
	pState->pSimpleModulessMap = vm->pSimpleState->pSimpleModulessMap ;
	pState->pSimpleCFunctions = vm->pSimpleState->pSimpleCFunctions ;
	/* Get a copy from the byte code List */
	pState->vm->nScopeID = vm->nScopeID + 10000 ;
	pState->vm->pCode = simple_list_new_gc(vm->pSimpleState,0) ;
	simple_list_copy(pState->vm->pCode,vm->pSimpleState->pSimpleGenCode);
	pState->pSimpleGenCode = pState->vm->pCode ;
	simple_vm_loadcode(pState->vm);
	/* Avoid the call to the main block */
	pState->vm->nCallMainFunction = 1 ;
	simple_vm_mutexunlock(vm);
	/* Run the code */
	simple_state_runcode(pState,cStr);
	simple_list_delete_gc(vm->pSimpleState,pState->vm->pCode);
	/* Restore the first scope - global scope */
	pState->vm->pMem->pFirst->pValue = pItem ;
	/* Avoid deleteing the shared code and the Mutex */
	pState->vm->pCode = pList ;
	pState->vm->pFunctionsMap = pList2 ;
	pState->vm->pClassesMap = pList3 ;
	pState->vm->pModulessMap = pList4 ;
	pState->vm->pCFunctionsList = pList5 ;
	pState->pSimpleGenCode = pList ;
	pState->pSimpleFunctionsMap = pList2 ;
	pState->pSimpleClassesMap = pList3 ;
	pState->pSimpleModulessMap = pList4 ;
	pState->pSimpleCFunctions = pList5 ;
	pState->vm->pMutex = NULL ;
	pState->vm->pFuncMutexDestroy = NULL ;
	pState->vm->pFuncMutexLock = NULL ;
	pState->vm->pFuncMutexUnlock = NULL ;
	/* Delete the SimpleState */
	simple_state_delete(pState);
}
/* Fast Function Call for Extensions (Without Eval) */

SIMPLE_API void simple_vm_callfunction ( VM *vm,char *cFuncName )
{
	/* Lower Case and pass () in the end */
	simple_stsimple_lower(cFuncName);
	/* Prepare (Remove effects of the currect function) */
	simple_list_deletelastitem_gc(vm->pSimpleState,vm->pFuncCallList);
	/* Load the function and call it */
	simple_vm_loadfunc2(vm,cFuncName,0);
	simple_vm_call2(vm);
	/* Execute the function */
	simple_vm_mainloop(vm);
	/* Free Stack */
	simple_vm_freestack(vm);
	/* Avoid normal steps after this function, because we deleted the scope in Prepare */
	vm->nActiveCatch = 1 ;
}
/* Trace */

void simple_vm_traceevent ( VM *vm,char nEvent )
{
	List *pList  ;
	if ( (vm->lTrace == 1) && (vm->lTraceActive == 0) ) {
		vm->lTraceActive = 1 ;
		vm->nTraceEvent = nEvent ;
		/* Prepare Trace Data */
		simple_list_deleteallitems_gc(vm->pSimpleState,vm->pTraceData);
		/* Add Line Number */
		simple_list_adddouble_gc(vm->pSimpleState,vm->pTraceData,vm->nLineNumber);
		/* Add File Name */
		simple_list_addstsimple_gc(vm->pSimpleState,vm->pTraceData,vm->cFileName);
		/* Add Function/Method Name */
		if ( simple_list_getsize(vm->pFuncCallList) > 0 ) {
			pList = simple_list_getlist(vm->pFuncCallList,simple_list_getsize(vm->pFuncCallList)) ;
			simple_list_addstsimple_gc(vm->pSimpleState,vm->pTraceData,simple_list_getstring(pList,SIMPLE_BLOCKCL_NAME));
			/* Method of Function */
			simple_list_adddouble_gc(vm->pSimpleState,vm->pTraceData,simple_list_getint(pList,SIMPLE_BLOCKCL_METHODORBLOCK));
		}
		else {
			simple_list_addstsimple_gc(vm->pSimpleState,vm->pTraceData,"");
			/* Method of Function */
			simple_list_adddouble_gc(vm->pSimpleState,vm->pTraceData,0);
		}
		/* Execute Trace Function */
		simple_vm_runcode(vm,simple_stsimple_get(vm->pTrace));
		vm->lTraceActive = 0 ;
		vm->nTraceEvent = 0 ;
	}
}
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkWrap.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * vtkWrap provides useful functions for generating wrapping code.
 */

#ifndef vtkWrap_h
#define vtkWrap_h

#include "vtkParse.h"
#include "vtkParseHierarchy.h"
#include "vtkWrappingToolsModule.h"

/**
 * For use with vtkWrap_DeclareVariable.
 */
/*@{*/
#define VTK_WRAP_RETURN 1
#define VTK_WRAP_ARG 2
#define VTK_WRAP_NOSEMI 4
/*@}*/

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Check for common types.
   * IsPODPointer is for unsized arrays of POD types.
   * IsZeroCopyPointer is for buffers that shouldn't be copied.
   */
  /*@{*/
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsVoid(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsVoidFunction(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsVoidPointer(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsCharPointer(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsPODPointer(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsZeroCopyPointer(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsStdVector(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsVTKObject(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsSpecialObject(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsPythonObject(ValueInfo* val);
  /*@}*/

  /**
   * The basic types, all are mutually exclusive.
   * Note that enums are considered to be objects,
   * bool and char are considered to be numeric.
   */
  /*@{*/
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsObject(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsFunction(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsStream(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsNumeric(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsString(ValueInfo* val);
  /*@}*/

  /**
   * Subcategories of numeric types.  In this categorization,
   * bool and char are not considered to be integers.
   */
  /*@{*/
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsBool(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsChar(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsInteger(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsRealNumber(ValueInfo* val);
  /*@}*/

  /**
   * Arrays and pointers. These are mutually exclusive.
   * IsPointer() does not include pointers to pointers.
   * IsArray() and IsNArray() do not include unsized arrays.
   * Arrays of pointers are not included in any of these.
   */
  /*@{*/
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsScalar(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsPointer(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsArray(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsNArray(ValueInfo* val);
  /*@}*/

  /**
   * Properties that can combine with other properties.
   */
  /*@{*/
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsNonConstRef(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsConstRef(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsRef(ValueInfo* val);
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsConst(ValueInfo* val);
  /*@}*/

  /**
   * Hints.
   * NewInstance objects must be freed by the caller.
   */
  /*@{*/
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsNewInstance(ValueInfo* val);
  /*@}*/

  /**
   * Check whether the class is derived from vtkObjectBase.
   * If "hinfo" is NULL, this just checks that the class
   * name starts with "vtk".
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsVTKObjectBaseType(
    HierarchyInfo* hinfo, const char* classname);

  /**
   * Check whether the class is not derived from vtkObjectBase.
   * If "hinfo" is NULL, it defaults to just checking if
   * the class starts with "vtk" and returns -1 if so.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsSpecialType(HierarchyInfo* hinfo, const char* classname);

  /**
   * Check if the class is derived from superclass.
   * If "hinfo" is NULL, then only an exact match to the
   * superclass will succeed.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsTypeOf(
    HierarchyInfo* hinfo, const char* classname, const char* superclass);

  /**
   * Check if the type of the value is an enum member of the class.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsEnumMember(ClassInfo* data, ValueInfo* arg);

  /**
   * Check whether a class is wrapped.  If "hinfo" is NULL,
   * it just checks that the class starts with "vtk".
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsClassWrapped(HierarchyInfo* hinfo, const char* classname);

  /**
   * Check whether the destructor is public
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_HasPublicDestructor(ClassInfo* data);

  /**
   * Check whether the copy constructor is public
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_HasPublicCopyConstructor(ClassInfo* data);

  /**
   * Expand all typedef types that are used in function arguments.
   * This should be done before any wrapping is done, to make sure
   * that the wrappers see the real types.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_ExpandTypedefs(
    ClassInfo* data, FileInfo* finfo, HierarchyInfo* hinfo);

  /**
   * Apply any using declarations that appear in the class.
   * If any using declarations appear in the class that refer to superclass
   * methods, the superclass header file will be parsed and the used methods
   * will be brought into the class.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_ApplyUsingDeclarations(
    ClassInfo* data, FileInfo* finfo, HierarchyInfo* hinfo);

  /**
   * Merge members of all superclasses into the data structure.
   * The superclass header files will be read and parsed.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_MergeSuperClasses(
    ClassInfo* data, FileInfo* finfo, HierarchyInfo* hinfo);

  /**
   * Apply any hints about array sizes, e.g. hint that the
   * GetNumberOfComponents() method gives the tuple size.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_FindCountHints(
    ClassInfo* data, FileInfo* finfo, HierarchyInfo* hinfo);

  /**
   * Get the size of a fixed-size tuple
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_GetTupleSize(ClassInfo* data, HierarchyInfo* hinfo);

  /**
   * Apply any hints about methods that return a new object instance,
   * i.e. factory methods and the like.  Reference counts must be
   * handled differently for such returned objects.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_FindNewInstanceMethods(
    ClassInfo* data, HierarchyInfo* hinfo);

  /**
   * Get the name of a type.  The name will not include "const".
   */
  VTKWRAPPINGTOOLS_EXPORT const char* vtkWrap_GetTypeName(ValueInfo* val);

  /**
   * True if the method a constructor of the class.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsConstructor(ClassInfo* c, FunctionInfo* f);

  /**
   * True if the method a destructor of the class.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsDestructor(ClassInfo* c, FunctionInfo* f);

  /**
   * True if the method is inherited from a base class.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsInheritedMethod(ClassInfo* c, FunctionInfo* f);

  /**
   * Check if a method is from a SetVector method.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsSetVectorMethod(FunctionInfo* f);

  /**
   * Check if a method is from a GetVector method.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_IsGetVectorMethod(FunctionInfo* f);

  /**
   * Count the number of parameters that are wrapped.
   * This skips the "void *" parameter that follows
   * wrapped function pointer parameters.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_CountWrappedParameters(FunctionInfo* f);

  /**
   * Count the number of args that are required.
   * This counts to the last argument that does not
   * have a default value.  Array args are not allowed
   * to have default values.
   */
  VTKWRAPPINGTOOLS_EXPORT int vtkWrap_CountRequiredArguments(FunctionInfo* f);

  /**
   * Write a variable declaration to a file.
   * Void is automatically ignored, and nothing is written for
   * function pointers
   * Set "idx" to -1 to avoid writing an idx.
   * Set "flags" to VTK_WRAP_RETURN to write a return value,
   * or to VTK_WRAP_ARG to write a temp argument variable.
   * The following rules apply:
   * - if VTK_WRAP_NOSEMI is set, then no semicolon/newline is printed
   * - if VTK_WRAP_RETURN is set, then "&" becomes "*"
   * - if VTK_WRAP_ARG is set, "&" becomes "*" only for object
   *   types, and is removed for all other types.
   * - "const" is removed except for return values with "&" or "*".
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_DeclareVariable(
    FILE* fp, ClassInfo* data, ValueInfo* v, const char* name, int idx, int flags);

  /**
   * Write an "int" size variable for arrays, initialized to
   * the array size if the size is greater than zero.
   * For N-dimensional arrays, write a static array of ints.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_DeclareVariableSize(
    FILE* fp, ValueInfo* v, const char* name, int idx);

  /**
   * Qualify all the unqualified identifiers in the given expression
   * and print the result to the file.
   */
  VTKWRAPPINGTOOLS_EXPORT void vtkWrap_QualifyExpression(
    FILE* fp, ClassInfo* data, const char* text);

  /**
   * Makes a superclass name into a valid identifier. Returns NULL if the given
   * name is valid as-is.
   */
  VTKWRAPPINGTOOLS_EXPORT char* vtkWrap_SafeSuperclassName(const char* name);

#ifdef __cplusplus
}
#endif

#endif
/* VTK-HeaderTest-Exclude: vtkWrap.h */

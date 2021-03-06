##=============================================================================
##
##  Copyright (c) Kitware, Inc.
##  All rights reserved.
##  See LICENSE.txt for details.
##
##  This software is distributed WITHOUT ANY WARRANTY; without even
##  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
##  PURPOSE.  See the above copyright notice for more information.
##
##  Copyright 2012 Sandia Corporation.
##  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
##  the U.S. Government retains certain rights in this software.
##
##=============================================================================

include_directories(${Boost_INCLUDE_DIRS})

set(headers
  CellTag.h
  CellTraits.h
  Extent.h
  Functional.h
  Pair.h
  Types.h
  TypeTraits.h
  VectorTraits.h


  ${Dax_BINARY_DIR}/dax/VectorOperations.h
  )

dax_declare_headers(${headers})

#-----------------------------------------------------------------------------
set(DAX_VECTOR_OPERATIONS_NAMESPACE "dax")
set(DAX_VECTOR_OPERATIONS_NAMESPACE_NAME "dax")
set(DAX_VECTOR_OPERATIONS_NAMESPACE_START "namespace dax {")
set(DAX_VECTOR_OPERATIONS_NAMESPACE_END   "}")
set(DAX_VECTOR_OPERATIONS_EXPORT "DAX_EXEC_CONT_EXPORT")
configure_file(
  ${Dax_SOURCE_DIR}/dax/internal/VectorOperations.h.in
  ${Dax_BINARY_DIR}/dax/VectorOperations.h
  @ONLY
  )

#-----------------------------------------------------------------------------
#first add all the components of dax that are shared between control and exec
add_subdirectory(testing)
add_subdirectory(internal)
add_subdirectory(math)

if (DAX_ENABLE_OPENGL_INTEROP)
  add_subdirectory(opengl)
endif (DAX_ENABLE_OPENGL_INTEROP)


#we add worklets before the exec or cont or any device adapter
#folders so that they can find the worklets for testing
add_subdirectory(worklet)


#-----------------------------------------------------------------------------
#add the control and exec folders
add_subdirectory(cont)
add_subdirectory(exec)


#-----------------------------------------------------------------------------
#now add all our specific device adapters
if (DAX_ENABLE_THRUST)
  add_subdirectory(thrust)
endif (DAX_ENABLE_THRUST)

if (DAX_ENABLE_CUDA)
  add_subdirectory(cuda)
endif (DAX_ENABLE_CUDA)

if (DAX_ENABLE_OPENMP)
  add_subdirectory(openmp)
endif (DAX_ENABLE_OPENMP)

if (DAX_ENABLE_TBB)
  add_subdirectory(tbb)
endif (DAX_ENABLE_TBB)



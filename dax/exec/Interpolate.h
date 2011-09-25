/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

===========================================================================*/

#ifndef __dax_exec_Interpolate_h
#define __dax_exec_Interpolate_h

#include <dax/exec/Cell.h>

#include <dax/exec/internal/InterpolationWeights.h>

namespace dax { namespace exec {

//-----------------------------------------------------------------------------
template<class WorkType, class T>
DAX_EXEC_EXPORT T cellInterpolate(const WorkType &work,
                                  const dax::exec::CellVoxel &cell,
                                  const dax::Vector3 &pcoords,
                                  const dax::exec::FieldPoint<T> &point_field)
{
  const dax::Id numVerts = 8;

  dax::Scalar weights[numVerts];
  dax::exec::internal::interpolationWeightsVoxel(pcoords, weights);

  T result = 0;
  for (dax::Id vertexId = 0; vertexId < numVerts; vertexId++)
    {
    T value = work.GetFieldValue(point_field, vertexId);
    result += value * weights[vertexId];
    }

  return result;
}

}};

#endif //__dax_exec_Interpolate_h
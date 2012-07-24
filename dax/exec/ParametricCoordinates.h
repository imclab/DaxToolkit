//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2012 Sandia Corporation.
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//=============================================================================

#ifndef __dax_exec_ParametricCoordinates_h
#define __dax_exec_ParametricCoordinates_h

#include <dax/Types.h>
#include <dax/exec/Cell.h>
#include <dax/exec/Derivative.h>
#include <dax/exec/Field.h>
#include <dax/exec/Interpolate.h>

#include <dax/exec/math/Matrix.h>
#include <dax/exec/math/Numerical.h>
#include <dax/exec/math/VectorAnalysis.h>

#include <dax/exec/internal/FieldAccess.h>

#include <dax/exec/WorkMapCell.h>
namespace dax {
namespace exec {

//-----------------------------------------------------------------------------
/// Defines the parametric coordinates for special locations in cells.
///
template<class CellType>
struct ParametricCoordinates
#ifdef DAX_DOXYGEN_ONLY
{
  /// The location of parametric center.
  ///
  DAX_EXEC_EXPORT static dax::Vector3 Center();

  /// The location of each vertex.
  ///
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3,CellType::NUM_POINTS> Vertex();
};
#else //DAX_DOXYGEN_ONLY
    ;
#endif

template<>
struct ParametricCoordinates<dax::exec::CellHexahedron>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(0.5, 0.5, 0.5);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 8> Vertex() {
    const dax::Vector3 cellVertexToParametricCoords[8] = {
      dax::make_Vector3(0, 0, 0),
      dax::make_Vector3(1, 0, 0),
      dax::make_Vector3(1, 1, 0),
      dax::make_Vector3(0, 1, 0),
      dax::make_Vector3(0, 0, 1),
      dax::make_Vector3(1, 0, 1),
      dax::make_Vector3(1, 1, 1),
      dax::make_Vector3(0, 1, 1)
    };
    return dax::Tuple<dax::Vector3, 8>(cellVertexToParametricCoords);
  }
};

template<>
struct ParametricCoordinates<dax::exec::CellVoxel>
    : public ParametricCoordinates<dax::exec::CellHexahedron> {  };

template<>
struct ParametricCoordinates<dax::exec::CellTetrahedron>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(0.25, 0.25, 0.25);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 4> Vertex() {
    const dax::Vector3 cellToParametricCoords[4] = {
      dax::make_Vector3(0, 0, 0),
      dax::make_Vector3(1, 0, 0),
      dax::make_Vector3(0, 1, 0),
      dax::make_Vector3(0, 0, 1)
    };
    return dax::Tuple<dax::Vector3, 4>(cellToParametricCoords);
  }
};

template<>
struct ParametricCoordinates<dax::exec::CellWedge>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(1.0/3.0, 1.0/3.0, 0.5);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 6> Vertex() {
    const dax::Vector3 cellToParametricCoords[6] = {
      dax::make_Vector3(0, 0, 0),
      dax::make_Vector3(0, 1, 0),
      dax::make_Vector3(1, 0, 0),
      dax::make_Vector3(0, 0, 1),
      dax::make_Vector3(0, 1, 1),
      dax::make_Vector3(1, 0, 1)
    };
    return dax::Tuple<dax::Vector3, 6>(cellToParametricCoords);
  }
};

template<>
struct ParametricCoordinates<dax::exec::CellTriangle>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(1.0/3.0, 1.0/3.0, 0.0);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 3> Vertex() {
    const dax::Vector3 cellVertexToParametricCoords[3] = {
      dax::make_Vector3(0, 0, 0),
      dax::make_Vector3(1, 0, 0),
      dax::make_Vector3(0, 1, 0)
    };
    return dax::Tuple<dax::Vector3, 3>(cellVertexToParametricCoords);
  }
};

template<>
struct ParametricCoordinates<dax::exec::CellQuadrilateral>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(0.5, 0.5, 0.0);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 4> Vertex() {
    const dax::Vector3 cellVertexToParametricCoords[4] = {
      dax::make_Vector3(0, 0, 0),
      dax::make_Vector3(1, 0, 0),
      dax::make_Vector3(1, 1, 0),
      dax::make_Vector3(0, 1, 0)
    };
    return dax::Tuple<dax::Vector3, 4>(cellVertexToParametricCoords);
  }
};

template<>
struct ParametricCoordinates<dax::exec::CellLine>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(0.5, 0.0, 0.0);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 2> Vertex() {
    const dax::Vector3 cellVertexToParametricCoords[2] = {
      dax::make_Vector3(0, 0, 0),
      dax::make_Vector3(1, 0, 0)
    };
    return dax::Tuple<dax::Vector3, 2>(cellVertexToParametricCoords);
  }
};

template<>
struct ParametricCoordinates<dax::exec::CellVertex>
{
  DAX_EXEC_EXPORT static dax::Vector3 Center() {
    return dax::make_Vector3(0.0, 0.0, 0.0);
  }
  DAX_EXEC_EXPORT static dax::Tuple<dax::Vector3, 1> Vertex() {
    const dax::Vector3 cellVertexToParametricCoords[1] = {
      dax::make_Vector3(0, 0, 0)
    };
    return dax::Tuple<dax::Vector3, 1>(cellVertexToParametricCoords);
  }
};

//-----------------------------------------------------------------------------
template<class WorkType, class CellType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 ParametricCoordinatesToWorldCoordinates(
    const WorkType &work,
    const CellType &cell,
    const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
    const dax::Vector3 pcoords)
{
  return dax::exec::CellInterpolate(work, cell, coordField, pcoords);
}

//-----------------------------------------------------------------------------
template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 ParametricCoordinatesToWorldCoordinates(
  const WorkType &work,
  const dax::exec::CellVoxel &cell,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 pcoords)
{
  dax::Vector3 spacing = cell.GetSpacing();
  dax::Vector3 cellOffset = spacing * pcoords;

  // This is a cheating way to get the coordinate value for index 0.  This is
  // a very special case where you would want just one point coordinate because
  // the rest are implicitly defined.
  dax::Vector3 minCoord = dax::exec::internal::FieldAccess::GetCoordinates(
        coordField, cell.GetPointIndex(0), cell.GetGridTopology(), work);

  return cellOffset + minCoord;
}

template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &work,
  const dax::exec::CellVoxel &cell,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 wcoords)
{
  // This is a cheating way to get the coordinate value for index 0.  This is
  // a very special case where you would want just one point coordinate because
  // the rest are implicitly defined.
  dax::Vector3 minCoord = dax::exec::internal::FieldAccess::GetCoordinates(
        coordField, cell.GetPointIndex(0), cell.GetGridTopology(), work);

  dax::Vector3 cellOffset = wcoords - minCoord;

  dax::Vector3 spacing = cell.GetSpacing();
  return cellOffset / spacing;
}

//-----------------------------------------------------------------------------
namespace detail {

template<class CellType>
class JacobianFunctor3DCell {
  const dax::Tuple<dax::Vector3,CellType::NUM_POINTS>
      &VertexCoordinates;
public:
  DAX_EXEC_EXPORT
  JacobianFunctor3DCell(
      const dax::Tuple<dax::Vector3,CellType::NUM_POINTS>
      &vertexCoords)
    : VertexCoordinates(vertexCoords) {  }
  DAX_EXEC_EXPORT
  dax::exec::math::Matrix3x3 operator()(dax::Vector3 pcoords) const {
    return dax::exec::detail::make_JacobianFor3DCell(
          dax::exec::internal::DerivativeWeights<CellType>(pcoords),
          this->VertexCoordinates);
  }
};

template<class WorkType, class ExecutionAdapter>
class CoodinatesFunctor3DCell {
  const WorkType &Work;
  const typename WorkType::CellType &Cell;
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &CoordField;
public:
  DAX_EXEC_EXPORT
  CoodinatesFunctor3DCell(
      const WorkType &work,
      const typename WorkType::CellType &cell,
      const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField)
    : Work(work), Cell(cell), CoordField(coordField) {  }
  DAX_EXEC_EXPORT
  dax::Vector3 operator()(dax::Vector3 pcoords) const {
    return dax::exec::ParametricCoordinatesToWorldCoordinates(this->Work,
                                                              this->Cell,
                                                              this->CoordField,
                                                              pcoords);
  }
};

} // Namespace detail

template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &work,
  const dax::exec::CellHexahedron &cell,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 wcoords)
{
  dax::Tuple<dax::Vector3,dax::exec::CellHexahedron::NUM_POINTS> vertexCoords =
      work.GetFieldValues(coordField);

  return dax::exec::math::NewtonsMethod(
        detail::JacobianFunctor3DCell<dax::exec::CellHexahedron>(vertexCoords),
        detail::CoodinatesFunctor3DCell<
             WorkType,ExecutionAdapter>(work,cell,coordField),
        wcoords,
        dax::make_Vector3(0.5, 0.5, 0.5));
}

template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &work,
  const dax::exec::CellWedge &cell,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 wcoords)
{
  dax::Tuple<dax::Vector3,dax::exec::CellWedge::NUM_POINTS> vertexCoords =
      work.GetFieldValues(coordField);

  return dax::exec::math::NewtonsMethod(
        detail::JacobianFunctor3DCell<dax::exec::CellWedge>(vertexCoords),
        detail::CoodinatesFunctor3DCell<
             WorkType,ExecutionAdapter>(work,cell,coordField),
        wcoords,
        dax::make_Vector3(0.5, 0.5, 0.5));
}

//-----------------------------------------------------------------------------
template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
    const WorkType &work,
    const dax::exec::CellTetrahedron &,
    const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
    const dax::Vector3 wcoords)
{
  // We solve the world to parametric coordinates problem for tetrahedra
  // similarly to that for triangles. Before understanding this code, you
  // should understand the triangle code. Go ahead. Read it now.
  //
  // The tetrahedron code is an obvious extension of the triangle code by
  // considering the parallelpiped formed by wcoords and p0 of the triangle
  // and the three adjacent faces.  This parallelpiped is equivalent to the
  // axis-aligned cuboid anchored at the origin of parametric space.
  //
  // Just like the triangle, we compute the parametric coordinate for each axis
  // by intersecting a plane with each edge emanating from p0. The plane is
  // defined by the one that goes through wcoords (duh) and is parallel to the
  // plane formed by the other two edges emanating from p0 (as dictated by the
  // aforementioned parallelpiped).
  //
  // In review, by parameterizing the line by fraction of distance the distance
  // from p0 to the adjacent point (which is itself the parametric coordinate
  // we are after), we get the following definition for the intersection.
  //
  // d = dot((wcoords - p0), planeNormal)/dot((p1-p0), planeNormal)
  //

  dax::Vector3 pcoords(dax::Scalar(0));
  dax::Tuple<dax::Vector3, 4> vertexCoords = work.GetFieldValues(coordField);

  const dax::Vector3 vec0 = vertexCoords[1] - vertexCoords[0];
  const dax::Vector3 vec1 = vertexCoords[2] - vertexCoords[0];
  const dax::Vector3 vec2 = vertexCoords[3] - vertexCoords[0];
  const dax::Vector3 coordVec = wcoords - vertexCoords[0];

  dax::Vector3 planeNormal = dax::exec::math::Cross(vec1, vec2);
  pcoords[0] = dax::dot(coordVec, planeNormal)/dax::dot(vec0, planeNormal);

  planeNormal = dax::exec::math::Cross(vec0, vec2);
  pcoords[1] = dax::dot(coordVec, planeNormal)/dax::dot(vec1, planeNormal);

  planeNormal = dax::exec::math::Cross(vec0, vec1);
  pcoords[2] = dax::dot(coordVec, planeNormal)/dax::dot(vec2, planeNormal);

  return pcoords;
}

//-----------------------------------------------------------------------------
template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &work,
  const dax::exec::CellTriangle &daxNotUsed(cell),
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 wcoords)
{
  // We will solve the world to parametric coordinates problem geometrically.
  // Consider the parallelogram formed by wcoords and p0 of the triangle and
  // the two adjacent edges. This parallelogram is equivalent to the
  // axis-aligned rectangle anchored at the origin of parametric space.
  //
  //   p2 |\                 (1,0) |\                                        //
  //      | \                      |  \                                      //
  //      |  \                     |    \                                    //
  //     |    \                    |      \                                  //
  //     |     \                   |        \                                //
  //     |      \                  |    (u,v) \                              //
  //    | ---    \                 |-------*    \                            //
  //    |    ---*wcoords           |       |      \                          //
  //    |       |  \               |       |        \                        //
  // p0 *---    |   \        (0,0) *------------------\ (1,0)                //
  //        ---|     \                                                       //
  //           x--    \                                                      //
  //              ---  \                                                     //
  //                 ---\ p1                                                 //
  //
  // In this diagram, the distance between p0 and the point marked x divided by
  // the length of the edge it is on is equal, by proportionality, to the u
  // parametric coordiante. (The v coordinate follows the other edge
  // accordingly.) Thus, if we can find the interesection at x (or more
  // specifically the distance between p0 and x), then we can find that
  // parametric coordinate.
  //
  // Because the triangle is in 3-space, we are actually going to intersect the
  // edge with a plane that is parallel to the opposite edge of p0 and
  // perpendicular to the triangle. This is partially because it is easy to
  // find the intersection between a plane and a line and partially because the
  // computation will work for points not on the plane. (The result is
  // equivalent to a point projected on the plane.)
  //
  // First, we define an implicit plane as:
  //
  // dot((p - wcoords), planeNormal) = 0
  //
  // where planeNormal is the normal to the plane (easily computed from the
  // triangle), and p is any point in the plane. Next, we define the parametric
  // form of the line:
  //
  // p(d) = (p1 - p0)d + p0
  //
  // Where d is the fraction of distance from p0 toward p1. Note that d is
  // actually equal to the parametric coordinate we are trying to find. Once we
  // compute it, we are done. We can skip the part about finding the actual
  // coordinates of the intersection.
  //
  // Solving for the interesection is as simple as substituting the line's
  // definition of p(d) into p for the plane equation. With some basic algebra
  // you get:
  //
  // d = dot((wcoords - p0), planeNormal)/dot((p1-p0), planeNormal)
  //
  // From here, the u coordiante is simply d. The v coordinate follows
  // similarly.
  //

  dax::Vector3 pcoords(dax::Scalar(0));
  dax::Tuple<dax::Vector3, 3> vertexCoords = work.GetFieldValues(coordField);
  dax::Vector3 triangleNormal =
      dax::exec::math::TriangleNormal(vertexCoords[0],
                                      vertexCoords[1],
                                      vertexCoords[2]);

  for (int dimension = 0; dimension < 2; dimension++)
    {
    const dax::Vector3 &p0 = vertexCoords[0];
    const dax::Vector3 &p1 = vertexCoords[dimension+1];
    const dax::Vector3 &p2 = vertexCoords[2-dimension];
    dax::Vector3 planeNormal = dax::exec::math::Cross(triangleNormal, p2-p0);

    dax::Scalar d =
        dax::dot(wcoords - p0, planeNormal)/dax::dot(p1 - p0, planeNormal);

    pcoords[dimension] = d;
    }

  return pcoords;
}

//-----------------------------------------------------------------------------
namespace detail {

class QuadrilateralJacobianFunctor {
  const dax::Tuple<dax::Vector3,dax::exec::CellQuadrilateral::NUM_POINTS>
      &VertexCoordinates;
  const dax::Id3 &DimensionSwizzle;
public:
  DAX_EXEC_EXPORT
  QuadrilateralJacobianFunctor(
      const dax::Tuple<dax::Vector3,dax::exec::CellQuadrilateral::NUM_POINTS>
      &vertexCoords,
      const dax::Id3 &dimensionSwizzle)
    : VertexCoordinates(vertexCoords), DimensionSwizzle(dimensionSwizzle) {  }
  DAX_EXEC_EXPORT
  dax::exec::math::Matrix2x2 operator()(dax::Vector2 pcoords) const {
    const int NUM_POINTS = dax::exec::CellQuadrilateral::NUM_POINTS;

    const dax::Tuple<dax::Vector3,dax::exec::CellQuadrilateral::NUM_POINTS>
        derivativeWeights =
        dax::exec::internal::DerivativeWeightsQuadrilateral(pcoords);

    // Create the Jacobian matrix of the form
    //
    //   |              |
    //   | da/du  da/dv |
    //   |              |
    //   | db/du  db/dv |
    //   |              |
    //
    // a and b are some arbitrary dimensions choosen from the first two
    // dimension indices in DimensionSwizzle.  (d is partial derivative)

    dax::exec::math::Matrix2x2 jacobian(0);
    for (int pointIndex = 0; pointIndex < NUM_POINTS; pointIndex++)
      {
      const dax::Vector3 &dweight = derivativeWeights[pointIndex];
      const dax::Vector3 &coord = this->VertexCoordinates[pointIndex];

      jacobian(0,0) += coord[this->DimensionSwizzle[0]] * dweight[0];
      jacobian(0,1) += coord[this->DimensionSwizzle[0]] * dweight[1];

      jacobian(1,0) += coord[this->DimensionSwizzle[1]] * dweight[0];
      jacobian(1,1) += coord[this->DimensionSwizzle[1]] * dweight[1];
      }

    return jacobian;
  }
};

template<class WorkType, class ExecutionAdapter>
class QuadrilateralCoodinatesFunctor {
  const WorkType &Work;
  const dax::exec::CellQuadrilateral &Cell;
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &CoordField;
  const dax::Id3 &DimensionSwizzle;
public:
  DAX_EXEC_EXPORT
  QuadrilateralCoodinatesFunctor(
      const WorkType &work,
      const dax::exec::CellQuadrilateral &cell,
      const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
      const dax::Id3 &dimensionSwizzle)
    : Work(work),
      Cell(cell),
      CoordField(coordField),
      DimensionSwizzle(dimensionSwizzle) {  }
  DAX_EXEC_EXPORT
  dax::Vector2 operator()(dax::Vector2 pcoords) const {
    dax::Vector3 pcoords3D(pcoords[0], pcoords[1], 0);
    dax::Vector3 wcoords =
        dax::exec::ParametricCoordinatesToWorldCoordinates(this->Work,
                                                           this->Cell,
                                                           this->CoordField,
                                                           pcoords3D);
    return dax::make_Vector2(wcoords[this->DimensionSwizzle[0]],
                             wcoords[this->DimensionSwizzle[1]]);
  }
};

} // Namespace detail

template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &work,
  const dax::exec::CellQuadrilateral &cell,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 wcoords)
{
  dax::Tuple<dax::Vector3,dax::exec::CellQuadrilateral::NUM_POINTS> vertexCoords
      = work.GetFieldValues(coordField);

  // We have an overconstrained system, so just pick two coordinates to work
  // with (the two most different from the normal).  We will do this by
  // creating a dimension swizzle and using the first two dimensions.

  dax::Id3 dimensionSwizzle;
  dax::Vector3 normal = dax::exec::math::TriangleNormal(vertexCoords[0],
                                                        vertexCoords[1],
                                                        vertexCoords[2]);
  dax::Vector3 absNormal = dax::exec::math::Abs(normal);
  if (absNormal[0] < absNormal[1])
    {
    dimensionSwizzle = dax::make_Id3(0, 1, 2);
    }
  else
    {
    dimensionSwizzle = dax::make_Id3(1, 0, 2);
    }
  if (absNormal[dimensionSwizzle[1]] < absNormal[2])
    {
    // Everything is fine.
    }
  else
    {
    dimensionSwizzle[2] = dimensionSwizzle[1];
    dimensionSwizzle[1] = 2;
    }

  dax::Vector2 pcoords =
      dax::exec::math::NewtonsMethod(
        detail::QuadrilateralJacobianFunctor(vertexCoords, dimensionSwizzle),
        detail::QuadrilateralCoodinatesFunctor<
             WorkType,ExecutionAdapter>(work,cell,coordField, dimensionSwizzle),
        dax::make_Vector2(wcoords[dimensionSwizzle[0]], wcoords[dimensionSwizzle[1]]),
        dax::make_Vector2(0.5, 0.5));

  return dax::make_Vector3(pcoords[0], pcoords[1], 0);
}

//-----------------------------------------------------------------------------
template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &work,
  const dax::exec::CellLine &,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &coordField,
  const dax::Vector3 wcoords)
{
  dax::Tuple<dax::Vector3, 2> vertexCoords = work.GetFieldValues(coordField);

  // Because this is a line, there is only one vaild parametric coordinate. Let
  // vec be the vector from the first point to the second point
  // (vertexCoords[1] - vertexCoords[0]), which is the direction of the line.
  // dot(vec,wcoords-vertexCoords[0])/mag(vec) is the orthoginal projection of
  // wcoords on the line and represents the distance between the orthoginal
  // projection and vertexCoords[0]. The parametric coordinate is the fraction
  // of this over the length of the segment, which is mag(vec). Thus, the
  // parametric coordinate is dot(vec,wcoords-vertexCoords[0])/mag(vec)^2.

  dax::Vector3 vec = vertexCoords[1] - vertexCoords[0];
  dax::Scalar numerator = dax::dot(vec, wcoords - vertexCoords[0]);
  dax::Scalar denominator = dax::exec::math::MagnitudeSquared(vec);

  return dax::make_Vector3(numerator/denominator, 0.0, 0.0);
}

//-----------------------------------------------------------------------------
template<class WorkType, class ExecutionAdapter>
DAX_EXEC_EXPORT dax::Vector3 WorldCoordinatesToParametricCoordinates(
  const WorkType &,
  const dax::exec::CellVertex &,
  const dax::exec::FieldCoordinatesIn<ExecutionAdapter> &,
  const dax::Vector3)
{
  return dax::make_Vector3(0.0, 0.0, 0.0);
}

}
}

#endif //__dax_exec_ParametricCoordinates_h

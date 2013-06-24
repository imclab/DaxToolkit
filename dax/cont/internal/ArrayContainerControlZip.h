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
#ifndef __dax_cont_internal_ArrayContainerControlZip_h
#define __dax_cont_internal_ArrayContainerControlZip_h

#include <dax/Pair.h>
#include <dax/cont/ArrayContainerControlImplicit.h>
#include <dax/cont/ArrayPortal.h>
#include <dax/cont/Assert.h>
#include <dax/cont/ErrorControlInternal.h>
#include <dax/cont/internal/IteratorFromArrayPortal.h>

namespace dax {
namespace cont {
namespace internal {

/// \brief An array portal that implicitly zips two other array portals.
///
template <class P1, class P2>
class ArrayPortalZip
{
public:
  typedef P1 FirstPortalType;
  typedef P2 SecondPortalType;
  typedef typename FirstPortalType::ValueType FirstValueType;
  typedef typename SecondPortalType::ValueType SecondValueType;
  typedef dax::Pair<FirstValueType,SecondValueType> ValueType;

  DAX_EXEC_CONT_EXPORT
  ArrayPortalZip() : FirstPortal(), SecondPortal() {  }

  DAX_EXEC_CONT_EXPORT
  ArrayPortalZip(const FirstPortalType &firstPortal,
                 const SecondPortalType &secondPortal)
    : FirstPortal(firstPortal), SecondPortal(secondPortal) {  }

  /// Copy constructor for any other ArrayPortalZip with an iterator
  /// type that can be copied to this iterator type. This allows us to do any
  /// type casting that the iterators do (like the non-const to const cast).
  ///
  template<class OtherP1, class OtherP2>
  DAX_CONT_EXPORT
  ArrayPortalZip(const ArrayPortalZip<OtherP1,OtherP2> &src)
    : FirstPortal(src.GetFirstPortal()),
      SecondPortal(src.GetSecondPortal())
  {  }

  DAX_EXEC_CONT_EXPORT
  dax::Id GetNumberOfValues() const {
    return this->FirstPortal.GetNumberOfValues();
  }

  DAX_EXEC_CONT_EXPORT
  ValueType Get(dax::Id index) const {
    return dax::make_Pair(this->FirstPortal.Get(index),
                          this->SecondPortal.Get(index));
  }

  DAX_EXEC_CONT_EXPORT
  void Set(dax::Id index, const ValueType &value) const {
    this->FirstPortal.Set(index, value.first);
    this->SecondPortal.Set(index, value.second);
  }

  typedef dax::cont::internal::IteratorFromArrayPortal<
      ArrayPortalZip<FirstPortalType,SecondPortalType> > IteratorType;

  DAX_CONT_EXPORT
  IteratorType GetIteratorBegin() const {
    return IteratorType(*this);
  }

  DAX_CONT_EXPORT
  IteratorType GetIteratorEnd() const {
    return IteratorType(*this, this->GetNumberOfValues());
  }

  DAX_EXEC_CONT_EXPORT
  FirstPortalType &GetFirstPortal() { return this->FirstPortal; }
  DAX_EXEC_CONT_EXPORT
  SecondPortalType &GetSecondPortal() { return this->SecondPortal; }

  DAX_EXEC_CONT_EXPORT
  const FirstPortalType &GetFirstPortal() const { return this->FirstPortal; }
  DAX_EXEC_CONT_EXPORT
  const SecondPortalType &GetSecondPortal() const { return this->SecondPortal; }

private:
  FirstPortalType FirstPortal;
  SecondPortalType SecondPortal;
};

template<class FirstArrayHandleType, class SecondArrayHandleType>
struct ArrayContainerControlTagZip { };

/// This helper struct defines the value type for a zip container containing
/// the given two array handles.
///
template<class FirstArrayHandleType, class SecondArrayHandleType>
struct ArrayContainerControlZipTypes {
  /// The ValueType (a pair containing the value types of the two arrays).
  ///
  typedef dax::Pair<typename FirstArrayHandleType::ValueType,
                    typename SecondArrayHandleType::ValueType> ValueType;

  /// The full type of the internal ArrayContainerControl specialization.
  ///
  typedef ArrayContainerControl<
    ValueType,
    ArrayContainerControlTagZip<FirstArrayHandleType, SecondArrayHandleType> >
      ArrayContainerControlType;

  /// The appropriately templated tag.
  ///
  typedef ArrayContainerControlTagZip<
      FirstArrayHandleType,SecondArrayHandleType> ArrayContainerControlTag;

  /// The portal types used with the zip container.
  ///
  typedef dax::cont::internal::ArrayPortalZip<
      typename FirstArrayHandleType::PortalControl,
      typename SecondArrayHandleType::PortalControl> PortalControl;
  typedef dax::cont::internal::ArrayPortalZip<
      typename FirstArrayHandleType::PortalConstControl,
      typename SecondArrayHandleType::PortalConstControl> PortalConstControl;
};

template<class FirstArrayHandleType, class SecondArrayHandleType>
class ArrayContainerControl<
    dax::Pair<typename FirstArrayHandleType::ValueType,
              typename SecondArrayHandleType::ValueType>,
    ArrayContainerControlTagZip<FirstArrayHandleType, SecondArrayHandleType> >
{
private:
  typedef ArrayContainerControlZipTypes<
      FirstArrayHandleType,SecondArrayHandleType> ZipTypes;
  typedef typename ZipTypes::ArrayContainerControlType ThisType;

public:
  typedef typename ZipTypes::ValueType ValueType;

  typedef typename ZipTypes::PortalControl PortalType;
  typedef typename ZipTypes::PortalConstControl PortalConstType;

public:
  DAX_CONT_EXPORT
  ArrayContainerControl() : Valid(false) {  }

  DAX_CONT_EXPORT
  ArrayContainerControl(const FirstArrayHandleType firstArrayHandle,
                        const SecondArrayHandleType secondArrayHandle)
    : FirstArray(firstArrayHandle), SecondArray(secondArrayHandle), Valid(true)
  {
    // TODO: MPL ASSERT to make sure DeviceAdapter of FirstArrayHandleType
    // and SecondArrayHandleType are the same.
  }

  DAX_CONT_EXPORT
  PortalType GetPortal() {
    DAX_ASSERT_CONT(this->Valid);
    return PortalType(this->FirstArray.GetPortalControl(),
                      this->SecondArray.GetPortalControl());
  }

  DAX_CONT_EXPORT
  PortalConstType GetPortalConst() const {
    DAX_ASSERT_CONT(this->Valid);
    return PortalConstType(this->FirstArray.GetPortalConstControl(),
                           this->SecondArray.GetPortalConstControl());
  }

  DAX_CONT_EXPORT
  dax::Id GetNumberOfValues() const {
    DAX_ASSERT_CONT(this->Valid);
    DAX_ASSERT_CONT(this->FirstArray.GetNumberOfValues()
                    == this->SecondArray.GetNumberOfValues());
    return this->FirstArray.GetNumberOfValues();
  }

  DAX_CONT_EXPORT
  void Allocate(dax::Id daxNotUsed(numberOfValues)) {
    throw dax::cont::ErrorControlInternal(
          "The allocate method for the zip control array container should "
          "never have been called. The allocate is generally only called by "
          "the execution array manager, and the array transfer for the zip "
          "container should prevent the execution array manager from being "
          "directly used.");
  }

  DAX_CONT_EXPORT
  void Shrink(dax::Id numberOfValues) {
    DAX_ASSERT_CONT(this->Valid);
    this->FirstArray.Shrink(numberOfValues);
    this->SecondArray.Shrink(numberOfValues);
  }

  DAX_CONT_EXPORT
  void ReleaseResources() {
    DAX_ASSERT_CONT(this->Valid);
    this->FirstArray.ReleaseResources();
    this->SecondArray.ReleaseResources();
  }

private:
  FirstArrayHandleType FirstArray;
  SecondArrayHandleType SecondArray;
  bool Valid;
};

template<typename T,
         class FirstArrayHandleType,
         class SecondArrayHandleType,
         class DeviceAdapter>
class ArrayTransfer<
    T,
    ArrayContainerControlTagZip<FirstArrayHandleType,SecondArrayHandleType>,
    DeviceAdapter>
{
  // This specialization of ArrayTransfer should never be instantiated, so
  // you should get a compile error about an undefined class element pointing
  // to this class if that happens.  You should be getting the specialization
  // of ArrayTransfer that defines the value type, but an error somewhere,
  // probably using the wrong type, is preventing that.
};

template<class FirstArrayHandleType,
         class SecondArrayHandleType,
         class DeviceAdapter>
class ArrayTransfer<
    typename ArrayContainerControlZipTypes<
        FirstArrayHandleType,SecondArrayHandleType>::ValueType,
    ArrayContainerControlTagZip<FirstArrayHandleType,SecondArrayHandleType>,
    DeviceAdapter>
{
private:
  typedef ArrayContainerControlZipTypes<
      FirstArrayHandleType,SecondArrayHandleType> ZipTypes;
  typedef typename ZipTypes::ArrayContainerControlType ContainerType;

public:
  typedef typename ZipTypes::ValueType ValueType;

  typedef typename ContainerType::PortalType PortalControl;
  typedef typename ContainerType::PortalConstType PortalConstControl;

  typedef ArrayPortalZip<
    typename FirstArrayHandleType::PortalExecution,
    typename SecondArrayHandleType::PortalExecution> PortalExecution;
  typedef ArrayPortalZip<
    typename FirstArrayHandleType::PortalConstExecution,
    typename SecondArrayHandleType::PortalConstExecution> PortalConstExecution;

  DAX_CONT_EXPORT
  ArrayTransfer() :
    ArraysValid(false),
    ExecutionPortalConstValid(false),
    ExecutionPortalValid(false) {
    // TODO: MPL ASSERT to make sure DeviceAdapter of this class is the same
    // as that of the first and second array handles.
  }

  DAX_CONT_EXPORT
  ArrayTransfer(FirstArrayHandleType firstArray,
                SecondArrayHandleType secondArray)
    : FirstArray(firstArray),
      SecondArray(secondArray),
      ArraysValid(true),
      ExecutionPortalConstValid(false),
      ExecutionPortalValid(false)
  {
    // TODO: MPL ASSERT to make sure DeviceAdapter of this class is the same
    // as that of the first and second array handles.
  }

  DAX_CONT_EXPORT
  dax::Id GetNumberOfValues() const {
    DAX_ASSERT_CONT(this->FirstArray.GetNumberOfValues()
                    == this->SecondArray.GetNumberOfValues());
    return this->FirstArray.GetNumberOfValues();
  }

  DAX_CONT_EXPORT
  void LoadDataForInput(PortalConstControl daxNotUsed(portal)) {
    // This method assumes that the given portal is one that is equivalent to
    // a portal zipping the data already in the two arrays. (That is, assumes
    // that a user portal was not given directly the the zipped array handle.)
    // It would be good to check that, but how?
    this->ExecutionPortalConst = PortalConstExecution(
                                   this->FirstArray.PrepareForInput(),
                                   this->SecondArray.PrepareForInput());
    this->ExecutionPortalConstValid = true;
    this->ExecutionPortalValid = false;
  }

  DAX_CONT_EXPORT
  void LoadDataForInPlace(ContainerType &daxNotUsed(controlArray)) {
    // Assuming controlArray uses the same first and second arrays as this.
    this->ExecutionPortal = PortalExecution(
                              this->FirstArray.PrepareForInPlace(),
                              this->SecondArray.PrepareForInPlace());

    // PortalConstExecution &a='a';
    // PortalConstExecution &b='a';
    this->ExecutionPortalConst = this->ExecutionPortal;
    this->ExecutionPortalConstValid = true;
    this->ExecutionPortalValid = true;
  }

  DAX_CONT_EXPORT
  void AllocateArrayForOutput(ContainerType &daxNotUsed(controlArray),
                              dax::Id numberOfValues) {
    // Assuming controlArray uses the same first and second arrays as this.
    this->ExecutionPortal
        = PortalExecution(this->FirstArray.PrepareForOutput(numberOfValues),
                          this->SecondArray.PrepareForOutput(numberOfValues));
    this->ExecutionPortalValid = true;
    this->ExecutionPortalConstValid = false;
  }

  DAX_CONT_EXPORT
  void RetrieveOutputData(ContainerType &daxNotUsed(controlArray)) const {
    // Implementation of this method should be unnecessary. The internal
    // first and second array handles should automatically retrieve the
    // output data as necessary.
  }

  /// I have not implemented this method because at the time of this writing I
  /// do not see a clear use case for it. To do it correctly, you need to
  /// create an iterator wrapper that reads/writes either the first or second
  /// part of the pair.
  ///
  template <class IteratorTypeControl>
  DAX_CONT_EXPORT void CopyInto(IteratorTypeControl dest) const;

  DAX_CONT_EXPORT
  void Shrink(dax::Id numberOfValues) {
    this->FirstArray.Shrink(numberOfValues);
    this->SecondArray.Shrink(numberOfValues);
  }

  DAX_CONT_EXPORT
  PortalExecution GetPortalExecution() {
    DAX_ASSERT_CONT(this->ExecutionPortalValid);
    return this->ExecutionPortal;
  }

  DAX_CONT_EXPORT
  PortalConstExecution GetPortalConstExecution() const {
    DAX_ASSERT_CONT(this->ExecutionPortalConstValid);
    return this->ExecutionPortalConst;
  }

  DAX_CONT_EXPORT
  void ReleaseResources() {
    this->FirstArray.ReleaseResourcesExecution();
    this->SecondArray.ReleaseResourcesExecution();
  }

private:
  FirstArrayHandleType FirstArray;
  SecondArrayHandleType SecondArray;
  bool ArraysValid;
  PortalConstExecution ExecutionPortalConst;
  bool ExecutionPortalConstValid;
  PortalExecution ExecutionPortal;
  bool ExecutionPortalValid;
};

}
}
} // namespace dax::cont::internal

#endif //__dax_cont_internal_ArrayContainerControlZip_h

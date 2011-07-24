/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "daxDataArrayIrregular.h"

#include "DaxDataArray.h"
#include <assert.h>

//-----------------------------------------------------------------------------
template <class T>
daxDataArrayIrregular<T>::daxDataArrayIrregular()
{
}

//-----------------------------------------------------------------------------
template <class T>
daxDataArrayIrregular<T>::~daxDataArrayIrregular()
{
  
}

//-----------------------------------------------------------------------------
template <class T>
DaxDataArray daxDataArrayIrregular<T>::Upload(bool copy_heavy_data/*=false*/)
{
  return copy_heavy_data?
    DaxDataArray::CreateAndCopy(DaxDataArray::IRREGULAR,
      DaxDataArray::type(T()),
      sizeof(T) * this->HeavyData.size(),
      this->HeavyData.data()) :
    DaxDataArray::Create(DaxDataArray::IRREGULAR,
      DaxDataArray::type(T()),
      sizeof(T) * this->HeavyData.size());
}

//-----------------------------------------------------------------------------
template <class T>
bool daxDataArrayIrregular<T>::Download(const DaxDataArray& array)
{
  assert (array.SizeInBytes == sizeof(T) * this->HeavyData.size());
  return array.CopyTo(this->HeavyData.data(), array.SizeInBytes);
}
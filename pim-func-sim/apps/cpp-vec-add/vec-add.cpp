// Test: C++ version of vector add
// Copyright 2024 LavaLab @ University of Virginia. All rights reserved.

#include "libpimsim.h"
#include <iostream>
#include <vector>

int main()
{
  std::cout << "PIM test: Vector add" << std::endl;

  unsigned numCores = 4;
  unsigned numRows = 128;
  unsigned numCols = 256;

  PimStatus status = pimCreateDevice(PIM_FUNCTIONAL, numCores, numRows, numCols);
  if (status != PIM_OK) {
    std::cout << "Abort" << std::endl;
    return 1;
  }

  unsigned numElements = 512;
  unsigned bitsPerElement = 32;

  PimObjId obj1 = pimAlloc(PIM_ALLOC_V1, numElements, bitsPerElement);
  if (obj1 == -1) {
    std::cout << "Abort" << std::endl;
    return 1;
  }
  PimObjId obj2 = pimAllocAssociated(PIM_ALLOC_V1, numElements, bitsPerElement, obj1);
  if (obj2 == -1) {
    std::cout << "Abort" << std::endl;
    return 1;
  }
  PimObjId obj3 = pimAllocAssociated(PIM_ALLOC_V1, numElements, bitsPerElement, obj1);
  if (obj3 == -1) {
    std::cout << "Abort" << std::endl;
    return 1;
  }

  std::vector<int> src1(numElements);
  std::vector<int> src2(numElements);
  std::vector<int> dest(numElements);

  // assign some initial values
  for (unsigned i = 0; i < numElements; ++i) {
    src1[i] = i;
    src2[i] = i * 2 - 10;
  }

  status = pimCopyHostToDevice(PIM_COPY_V, (void*)src1.data(), obj1);
  if (status != PIM_OK) {
    std::cout << "Abort" << std::endl;
    return 1;
  }

  status = pimCopyHostToDevice(PIM_COPY_V, (void*)src2.data(), obj2);
  if (status != PIM_OK) {
    std::cout << "Abort" << std::endl;
    return 1;
  }

  status = pimInt32Add(obj1, obj2, obj3);
  if (status != PIM_OK) {
    std::cout << "Abort" << std::endl;
    return 1;
  }

  status = pimCopyDeviceToHost(PIM_COPY_V, obj3, (void*)dest.data());
  if (status != PIM_OK) {
    std::cout << "Abort" << std::endl;
    return 1;
  }

  // check results
  bool ok = true;
  for (unsigned i = 0; i < numElements; ++i) {
    int result = src1[i] + src2[i];
    if (dest[i] != result) {
      std::cout << "Wrong ansert: " << src1[i] << " + " << src2[i] << " = " << dest[i] << " (expected " << result << ")" << std::endl;
      ok = false;
    }
  }

  if (ok) {
    std::cout << "All correct!" << std::endl;
  }

  return 0;
}

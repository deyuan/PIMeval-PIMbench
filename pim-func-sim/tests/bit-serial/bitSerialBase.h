// Bit-Serial Performance Modeling - Base
// Copyright 2024 LavaLab @ University of Virginia. All rights reserved.

#ifndef BIT_SERIAL_BASE_H
#define BIT_SERIAL_BASE_H

#include "libpimsim.h"
#include <cassert>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <map>
#include <memory>

//! @class  bitSerialBase
//! @brief  Bit-serial perf base class
class bitSerialBase
{
public:
  bitSerialBase() {}
  virtual ~bitSerialBase() {}

  bool runTests(const std::vector<std::string>& testList);

  const std::map<std::string, std::pair<int, int>>& getStats() const { return m_stats; }

protected:

  // virtual: get device type
  virtual PimDeviceEnum getDeviceType() = 0;

  // virtual: high-level APIs to evaluate
  virtual void bitSerialIntAdd(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntSub(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntMul(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntDiv(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntAbs(int numBits, PimObjId src, PimObjId dest) {}
  virtual void bitSerialIntAnd(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntOr(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntXor(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntXnor(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntGT(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntLT(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntEQ(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntMin(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntMax(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialIntPopCount(int numBits, PimObjId src, PimObjId dest) {}
  virtual void bitSerialUIntAdd(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntSub(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntMul(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntDiv(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntAbs(int numBits, PimObjId src, PimObjId dest) {}
  virtual void bitSerialUIntAnd(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntOr(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntXor(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntXnor(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntGT(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntLT(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntEQ(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntMin(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntMax(int numBits, PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialUIntPopCount(int numBits, PimObjId src, PimObjId dest) {}

  virtual void bitSerialFp32Add(PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialFp32Sub(PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialFp32Mul(PimObjId src1, PimObjId src2, PimObjId dest) {}
  virtual void bitSerialFp32Div(PimObjId src1, PimObjId src2, PimObjId dest) {}

  // helper functions
  void createDevice();
  void deleteDevice();

  template <typename T> std::vector<T> getRandInt(uint64_t numElements, T min, T max, bool allowZero = true);
  template <typename T> std::vector<T> getRandFp(uint64_t numElements, T min, T max, bool allowZero = true);
  template <typename T> bool testInt(const std::string& category, PimDataType dataType);
  template <typename T> bool testFp(const std::string& category, PimDataType dataType);

  std::map<std::string, std::pair<int, int>> m_stats;
  std::string m_deviceName;
};


//! @brief  Generate a random vector of integer data type T
template <typename T> std::vector<T>
bitSerialBase::getRandInt(uint64_t numElements, T min, T max, bool allowZero)
{
  std::vector<T> vec(numElements);
  if (min == 0 && max == 0 && !allowZero) {
    return vec;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(min, max);
  for (uint64_t i = 0; i < numElements; ++i) {
    T val = 0;
    do {
      val = dis(gen);
    } while (val == 0 && !allowZero);
    vec[i] = val;
  }
  return vec;
}

//! @brief  Generate a random vector of floating-point data type T
template <typename T> std::vector<T>
bitSerialBase::getRandFp(uint64_t numElements, T min, T max, bool allowZero)
{
  std::vector<T> vec(numElements);
  if (min == 0 && max == 0 && !allowZero) {
    assert(0);
    return vec;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<T> dis(min, max);
  for (uint64_t i = 0; i < numElements; ++i) {
    T val = 0.0;
    do {
      val = dis(gen);
    } while (val == 0.0 && !allowZero);
    vec[i] = val;
  }
  return vec;
}


//! @brief  Test integer bit-serial micro-programs
template <typename T> bool
bitSerialBase::testInt(const std::string& category, PimDataType dataType)
{
  const int numTests = 15;
  int numPassed = 0;
  uint64_t numElements = 4000;
  unsigned numBits = sizeof(T) * 8;
  T maxVal = 0;
  T minVal = 0;
  bool isSigned = true;
  switch (dataType) {
    case PIM_INT8:
        minVal = static_cast<T>(-100);
        maxVal = static_cast<T>(127);
        break;
    case PIM_INT16:
        minVal = static_cast<T>(-10000);
        maxVal = static_cast<T>(10000);
        break;
    case PIM_INT32:
        minVal = static_cast<T>(-100000);
        maxVal = static_cast<T>(100000);
        break;
    case PIM_INT64:
        minVal = static_cast<T>(-40000000000LL);
        maxVal = static_cast<T>(40000000000LL);
        break;
    case PIM_UINT8:
        isSigned = false;
        maxVal = static_cast<T>(255);
        break;
    case PIM_UINT16:
        isSigned = false;
        maxVal = static_cast<T>(40000);
        break;
    case PIM_UINT32:
        isSigned = false;
        maxVal = static_cast<T>(100000);
        break;
    case PIM_UINT64:
        isSigned = false;
        maxVal = static_cast<T>(40000000000LL);
        break;
    default:
      std::cout << "Error: Unsupported data type." << std::endl;
      return false;
  };

  std::cout << "================================================================" << std::endl;
  std::cout << "INFO: Evaluating bit-serial micro-programs for [" << m_deviceName << ":" << category << "]" << std::endl;
  std::cout << "================================================================" << std::endl;

  // allocate host vectors
  std::vector<T> vecSrc1 = getRandInt<T>(numElements, minVal, maxVal);
  std::vector<T> vecSrc2 = getRandInt<T>(numElements, minVal, maxVal);
  std::vector<T> vecSrcNonZero = getRandInt<T>(numElements, minVal, maxVal, false);
  std::vector<T> vecSrc3 = getRandInt<T>(numElements, minVal, maxVal);
  std::vector<T> vecDest(numElements);
  std::vector<T> vecSrc1Verify(numElements);
  std::vector<T> vecSrc2Verify(numElements);
  std::vector<T> vecSrcNonZeroVerify(numElements);
  std::vector<T> vecSrc3Verify(numElements);
  std::vector<T> vecDestVerify(numElements);
  // create EQ cases
  vecSrc2[100] = vecSrc1[100];
  vecSrc2[3000] = vecSrc1[3000];

  // allocate PIM objects
  PimObjId src1 = pimAlloc(PIM_ALLOC_V1, numElements, numBits, dataType);
  PimObjId src2 = pimAllocAssociated(numBits, src1, dataType);
  PimObjId srcNonZero = pimAllocAssociated(numBits, src1, dataType);
  PimObjId dest1 = pimAllocAssociated(numBits, src1, dataType);
  PimObjId dest2 = pimAllocAssociated(numBits, src1, dataType);
  PimObjId src3 = pimAllocAssociated(numBits, src1, dataType);  // alloc at last for oob check

  // for printing. keep in sync with switch case below
  const std::vector<std::string> testNames = {
    "add", "sub", "mul", "div", "abs",
    "and", "or", "xor", "xnor",
    "gt", "lt", "eq",
    "min", "max",
    "popcount"
  };

  for (int testId = 0; testId < numTests; ++testId) {
    std::string tag = "[" + m_deviceName + ":" + category + ":" + testNames[testId] + ":" + std::to_string(testId) + "]";
    bool ok = true;
    std::cout << tag << " Start" << std::endl;

    pimCopyHostToDevice((void *)vecSrc1.data(), src1);
    pimCopyHostToDevice((void *)vecSrc2.data(), src2);
    pimCopyHostToDevice((void *)vecSrcNonZero.data(), srcNonZero);
    pimCopyHostToDevice((void *)vecSrc3.data(), src3);

    switch (testId) {
    case 0: pimAdd(src1, src2, dest1); break;
    case 1: pimSub(src1, src2, dest1); break;
    case 2: pimMul(src1, src2, dest1); break;
    case 3: pimDiv(src1, srcNonZero, dest1); break;
    case 4: pimAbs(src1, dest1); break;
    case 5: pimAnd(src1, src2, dest1); break;
    case 6: pimOr(src1, src2, dest1); break;
    case 7: pimXor(src1, src2, dest1); break;
    case 8: pimXnor(src1, src2, dest1); break;
    case 9: pimGT(src1, src2, dest1); break;
    case 10: pimLT(src1, src2, dest1); break;
    case 11: pimEQ(src1, src2, dest1); break;
    case 12: pimMin(src1, src2, dest1); break;
    case 13: pimMax(src1, src2, dest1); break;
    case 14: pimPopCount(src1, dest1); break;
    default:
      std::cout << tag << " Error: Test ID not supported" << std::endl;
      return false;
    }

    pimResetStats();

    if (isSigned) {
      switch (testId) {
      case 0: bitSerialIntAdd(numBits, src1, src2, dest2); break;
      case 1: bitSerialIntSub(numBits, src1, src2, dest2); break;
      case 2: bitSerialIntMul(numBits, src1, src2, dest2); break;
      case 3: bitSerialIntDiv(numBits, src1, srcNonZero, dest2); break;
      case 4: bitSerialIntAbs(numBits, src1, dest2); break;
      case 5: bitSerialIntAnd(numBits, src1, src2, dest2); break;
      case 6: bitSerialIntOr(numBits, src1, src2, dest2); break;
      case 7: bitSerialIntXor(numBits, src1, src2, dest2); break;
      case 8: bitSerialIntXnor(numBits, src1, src2, dest2); break;
      case 9: bitSerialIntGT(numBits, src1, src2, dest2); break;
      case 10: bitSerialIntLT(numBits, src1, src2, dest2); break;
      case 11: bitSerialIntEQ(numBits, src1, src2, dest2); break;
      case 12: bitSerialIntMin(numBits, src1, src2, dest2); break;
      case 13: bitSerialIntMax(numBits, src1, src2, dest2); break;
      case 14: bitSerialIntPopCount(numBits, src1, dest2); break;
      default:
        std::cout << tag << " Error: Test ID not supported" << std::endl;
        return false;
      }
    } else {
      switch (testId) {
      case 0: bitSerialUIntAdd(numBits, src1, src2, dest2); break;
      case 1: bitSerialUIntSub(numBits, src1, src2, dest2); break;
      case 2: bitSerialUIntMul(numBits, src1, src2, dest2); break;
      case 3: bitSerialUIntDiv(numBits, src1, srcNonZero, dest2); break;
      case 4: bitSerialUIntAbs(numBits, src1, dest2); break;
      case 5: bitSerialUIntAnd(numBits, src1, src2, dest2); break;
      case 6: bitSerialUIntOr(numBits, src1, src2, dest2); break;
      case 7: bitSerialUIntXor(numBits, src1, src2, dest2); break;
      case 8: bitSerialUIntXnor(numBits, src1, src2, dest2); break;
      case 9: bitSerialUIntGT(numBits, src1, src2, dest2); break;
      case 10: bitSerialUIntLT(numBits, src1, src2, dest2); break;
      case 11: bitSerialUIntEQ(numBits, src1, src2, dest2); break;
      case 12: bitSerialUIntMin(numBits, src1, src2, dest2); break;
      case 13: bitSerialUIntMax(numBits, src1, src2, dest2); break;
      case 14: bitSerialUIntPopCount(numBits, src1, dest2); break;
      default:
        std::cout << tag << " Error: Test ID not supported" << std::endl;
        return false;
      }
    }

    pimShowStats();

    pimCopyDeviceToHost(dest1, (void*)vecDestVerify.data());
    pimCopyDeviceToHost(dest2, (void*)vecDest.data());
    if (vecDest != vecDestVerify) {
      ok = false;
      std::cout << tag << " Error: Incorrect results !!!!!" << std::endl;

      if (1) { // debug
        uint64_t numFailed = 0;
        for (uint64_t i = 0; i < numElements; ++i) {
          if (vecDest[i] != vecDestVerify[i]) {
            numFailed++;
            if (numFailed < 3) {
              T val1 = vecSrc1[i];
              T val2 = (testId == 3 ? vecSrcNonZero[i] : vecSrc2[i]);
              std::cout << "  Idx " << i << " Operand1 " << val1 << " Operand2 " << val2
                        << " Result " << vecDest[i] << " Expected " << vecDestVerify[i] << std::endl;
            }
          }
        }
        std::cout << "  Total " << numFailed << " out of " << numElements
                  << " failed" << std::endl;
      }
    }

    pimCopyDeviceToHost(src1, (void*)vecSrc1Verify.data());
    pimCopyDeviceToHost(src2, (void*)vecSrc2Verify.data());
    pimCopyDeviceToHost(srcNonZero, (void*)vecSrcNonZeroVerify.data());
    pimCopyDeviceToHost(src3, (void*)vecSrc3Verify.data());
    if (vecSrc1 != vecSrc1Verify || vecSrc2 != vecSrc2Verify || vecSrcNonZero != vecSrcNonZeroVerify || vecSrc3 != vecSrc3Verify) {
      ok = false;
      std::cout << tag << " Error: Input modified !!!!!" << std::endl;
    }

    std::cout << tag << " End " << (ok ? " -- Succeeded" : " -- Failed!") << std::endl;
    if (ok) {
      numPassed++;
    }
  }

  pimFree(src3);
  pimFree(dest2);
  pimFree(dest1);
  pimFree(srcNonZero);
  pimFree(src2);
  pimFree(src1);

  std::cout << "INFO: Bit-serial micro-programs for [" << m_deviceName << ":" << category << "] " << numPassed << " / " << numTests << " passed" << std::endl;
  m_stats[category] = std::make_pair(numPassed, numTests);
  return numPassed == numTests;
}

//! @brief  Test floating-point bit-serial micro-programs
template <typename T> bool
bitSerialBase::testFp(const std::string& category, PimDataType dataType)
{
  const int numTests = 4;
  int numPassed = 0;
  uint64_t numElements = 4000;
  unsigned numBits = sizeof(T) * 8;
  T maxVal = 0;
  T minVal = 0;
  switch (dataType) {
    case PIM_FP32:
        minVal = -10000.0;
        maxVal = 10000;
        break;
    default:
      std::cout << "Error: Unsupported data type." << std::endl;
      return false;
  };

  std::cout << "================================================================" << std::endl;
  std::cout << "INFO: Evaluating bit-serial micro-programs for [" << m_deviceName << ":" << category << "]" << std::endl;
  std::cout << "================================================================" << std::endl;

  // allocate host vectors
  std::vector<T> vecSrc1 = getRandFp<T>(numElements, minVal, maxVal);
  std::vector<T> vecSrc2 = getRandFp<T>(numElements, minVal, maxVal);
  std::vector<T> vecSrcNonZero = getRandFp<T>(numElements, minVal, maxVal, false);
  std::vector<T> vecSrc3 = getRandFp<T>(numElements, minVal, maxVal);
  std::vector<T> vecDest(numElements);
  std::vector<T> vecSrc1Verify(numElements);
  std::vector<T> vecSrc2Verify(numElements);
  std::vector<T> vecSrcNonZeroVerify(numElements);
  std::vector<T> vecSrc3Verify(numElements);
  std::vector<T> vecDestVerify(numElements);
  // create EQ cases
  vecSrc2[100] = vecSrc1[100];
  vecSrc2[3000] = vecSrc1[3000];

  // allocate PIM objects
  PimObjId src1 = pimAlloc(PIM_ALLOC_V1, numElements, numBits, dataType);
  PimObjId src2 = pimAllocAssociated(numBits, src1, dataType);
  PimObjId srcNonZero = pimAllocAssociated(numBits, src1, dataType);
  PimObjId dest1 = pimAllocAssociated(numBits, src1, dataType);
  PimObjId dest2 = pimAllocAssociated(numBits, src1, dataType);
  PimObjId src3 = pimAllocAssociated(numBits, src1, dataType);  // alloc at last for oob check

  // for printing. keep in sync with switch case below
  const std::vector<std::string> testNames = {
    "add", "sub", "mul", "div",
  };

  for (int testId = 0; testId < numTests; ++testId) {
    std::string tag = "[" + m_deviceName + ":" + category + ":" + testNames[testId] + ":" + std::to_string(testId) + "]";
    bool ok = true;
    std::cout << tag << " Start" << std::endl;

    pimCopyHostToDevice((void *)vecSrc1.data(), src1);
    pimCopyHostToDevice((void *)vecSrc2.data(), src2);
    pimCopyHostToDevice((void *)vecSrcNonZero.data(), srcNonZero);
    pimCopyHostToDevice((void *)vecSrc3.data(), src3);

    switch (testId) {
    case 0: pimAdd(src1, src2, dest1); break;
    case 1: pimSub(src1, src2, dest1); break;
    case 2: pimMul(src1, src2, dest1); break;
    case 3: pimDiv(src1, srcNonZero, dest1); break;
    default:
      std::cout << tag << " Error: Test ID not supported" << std::endl;
      return false;
    }

    pimResetStats();

    if (numBits == 32) {
      switch (testId) {
      case 0: bitSerialFp32Add(src1, src2, dest2); break;
      case 1: bitSerialFp32Sub(src1, src2, dest2); break;
      case 2: bitSerialFp32Mul(src1, src2, dest2); break;
      case 3: bitSerialFp32Div(src1, srcNonZero, dest2); break;
      default:
        std::cout << tag << " Error: Test ID not supported" << std::endl;
        return false;
      }
    }

    pimShowStats();

    pimCopyDeviceToHost(dest1, (void*)vecDestVerify.data());
    pimCopyDeviceToHost(dest2, (void*)vecDest.data());
    if (vecDest != vecDestVerify) {
      ok = false;
      std::cout << tag << " Error: Incorrect results !!!!!" << std::endl;

      if (1) { // debug
        uint64_t numFailed = 0;
        for (uint64_t i = 0; i < numElements; ++i) {
          if (vecDest[i] != vecDestVerify[i]) {
            numFailed++;
            if (numFailed < 3) {
              T val1 = vecSrc1[i];
              T val2 = (testId == 3 ? vecSrcNonZero[i] : vecSrc2[i]);
              std::cout << "  Idx " << i << " Operand1 " << val1 << " Operand2 " << val2
                        << " Result " << vecDest[i] << " Expected " << vecDestVerify[i] << std::endl;
            }
          }
        }
        std::cout << "  Total " << numFailed << " out of " << numElements
                  << " failed" << std::endl;
      }
    }

    pimCopyDeviceToHost(src1, (void*)vecSrc1Verify.data());
    pimCopyDeviceToHost(src2, (void*)vecSrc2Verify.data());
    pimCopyDeviceToHost(srcNonZero, (void*)vecSrcNonZeroVerify.data());
    pimCopyDeviceToHost(src3, (void*)vecSrc3Verify.data());
    if (vecSrc1 != vecSrc1Verify || vecSrc2 != vecSrc2Verify || vecSrcNonZero != vecSrcNonZeroVerify || vecSrc3 != vecSrc3Verify) {
      ok = false;
      std::cout << tag << " Error: Input modified !!!!!" << std::endl;
    }

    std::cout << tag << " End " << (ok ? " -- Succeeded" : " -- Failed!") << std::endl;
    if (ok) {
      numPassed++;
    }
  }

  pimFree(src3);
  pimFree(dest2);
  pimFree(dest1);
  pimFree(srcNonZero);
  pimFree(src2);
  pimFree(src1);

  std::cout << "INFO: Bit-serial micro-programs for [" << m_deviceName << ":" << category << "] " << numPassed << " / " << numTests << " passed" << std::endl;
  m_stats[category] = std::make_pair(numPassed, numTests);
  return numPassed == numTests;
}


#endif


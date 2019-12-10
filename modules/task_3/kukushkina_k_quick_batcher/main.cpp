// Copyright 2019 Kukushkina Ksenia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./quick_batcher.h"

TEST(Quick_Batcher_Sort, Quick_Sort) {
  std::vector<int> vec(10);
  generateRand(vec, 10);
  quickSort(vec, 0, 9);
  ASSERT_EQ(true, isSorted(vec, 10));
}

TEST(Quick_Batcher_Sort, Any_Size_No_Throw) {
  std::vector<int> vec(100);
  generateRand(vec, 100);
  ASSERT_NO_THROW(quickBatcherMPI(vec, 0, 99));
}

TEST(Quick_Batcher_Sort, S_No_Throw) {
  std::vector<int> p(6), q(6);
  for (int i = 0; i < 6; i++) {
    p[i] = i;
    q[i] = i * 5;
  }
  ASSERT_NO_THROW(S(p, q));
}

TEST(Quick_Batcher_Sort, B_No_Throw) {
  std::vector<int> p(6);
  for (int i = 0; i < 6; i++)
    p[i] = i;
  ASSERT_NO_THROW(B(p));
}

TEST(Quick_Batcher_Sort, Quick_Batcher_Sort) {
  std::vector<int> vec(10);
  generateRand(vec, 10);
  quickBatcherMPI(vec, 0, 9);
  ASSERT_EQ(true, isSorted(vec, 10));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
}

// Copyright 2019 Kukushkina Ksenia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./quick_batcher.h"

TEST(Quick_Batcher_Sort, Random_Generation) {
  std::vector<int> vec(100000);
  ASSERT_NO_THROW(generateRand(&vec));
}

TEST(Quick_Batcher_Sort, Empty_Vector) {
  std::vector<int> vec(0);
  ASSERT_ANY_THROW(quickBatcher(&vec));
}

TEST(Quick_Batcher_Sort, Odd_Size) {
  std::vector<int> vec(1001);
  generateRand(&vec);
  ASSERT_NO_THROW(quickBatcher(&vec));
}

TEST(Quick_Batcher_Sort, Pow_2_Size) {
  std::vector<int> vec(256);
  generateRand(&vec);
  ASSERT_NO_THROW(quickBatcher(&vec));
}

TEST(Quick_Batcher_Sort, Quick_Sort) {
  std::vector<int> vec(100);
  generateRand(&vec);
  quickSort(&vec, 0, 99);
  ASSERT_EQ(isSorted(vec, 99), true);
}

TEST(Quick_Batcher_Sort, Batcher_Sort) {
  std::vector<int> vec(1000);
  generateRand(&vec);
  quickBatcher(&vec);
  ASSERT_EQ(isSorted(vec, 999), true);
}

TEST(Quick_Batcher_Sort, Equal_Result) {
  std::vector<int> vec(1000), vec1(1000);
  generateRand(&vec);
  vec1 = vec;
  quickSort(&vec, 0, 999);
  quickBatcher(&vec1);
  bool eq = true;
  for (int i = 0; i < 1000; i++)
    if (vec1[i] != vec[i])
      eq = false;
  ASSERT_EQ(eq, true);
}

/* TEST(Quick_Batcher_Sort, Efficiency) {
  std::vector<int> vec(100000), vec1(100000);
  double t1, t2, finish;
  generateRand(vec);
  vec1 = vec;
  double start = MPI_Wtime();
  quickSort(vec, 0, 99999);
  finish = MPI_Wtime();
  t1 = finish - start;
  start = MPI_Wtime();
  quickBatcher(&vec1);
  finish = MPI_Wtime();
  t2 = finish - start;
  std::cout << "Qsort: " << t1 << "s\nBatcher: " << t2 << "s\n";
  ASSERT_EQ(t1 > t2, true);
}  */

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

// Copyright 2019 Kukushkina Ksenia
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "../../../modules/task_2/kukushkina_k_simple_iterations/simple_iterations.h"

TEST(Simple_Iterations, Vector_Random_Generation) {
  ASSERT_NO_THROW(GenerateVector(1000000000));
}

TEST(Simple_Iterations, Incorrect_Size_Vector_Random_Generation) {
  ASSERT_ANY_THROW(GenerateVector(-13));
}

TEST(Simple_Iterations, Matrix_Random_Generation) {
  ASSERT_NO_THROW(GenerateMatrix(10000));
}

TEST(Simple_Iterations, Incorrect_Size_Matrix_Random_Generation) {
  ASSERT_ANY_THROW(GenerateMatrix(-13));
}

TEST(Simple_Iterations, Non_Equal_A_and_b_Sizez) {
  std::vector<double> A = GenerateMatrix(10);
  std::vector<double> b = GenerateVector(11);
}

TEST(Simple_Iterations, Zero_Element) {
  std::vector<double> A = { 0, 1, 1, 0 };
  std::vector<double> b = { 1, 1 };
  ASSERT_ANY_THROW(SimpleIterations(A, b, 0.0001));
}

TEST(Simple_Iterations, No_Diagonal_Prevalence) {
  std::vector<double> A = { 1, 2, 1, 2, 1, 1, 3, 1, 1 };
  std::vector<double> b = { 1, 1, 1 };
  ASSERT_ANY_THROW(SimpleIterations(A, b, 0.0001));
}

TEST(Simple_Iterations, SLAE_1) {
  std::vector<double> A = { 1, 2, 3, 3, 1, 2, 3, 3, 1 };
  std::vector<double> b = { 14, 11, 12 };
  std::vector<double> x = SimpleIterations(A, b, 0.0001);
  double sum = x[0] + x[1] + x[2];
  ASSERT_NEAR(sum, 6, 0.0003);  // (1, 2, 3), 3 components -> precision*=3
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

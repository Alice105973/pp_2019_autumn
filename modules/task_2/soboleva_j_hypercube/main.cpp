// Copyright 2019 Soboleva Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/soboleva_j_hypercube/hypercube.h"


TEST(Parallel_Operations_MPI, Get_Dimension_Is_Correct) {
  EXPECT_EQ(getDimension(63), 6);
}

TEST(Parallel_Operations_MPI, Get_Dimension_Throws) {
  ASSERT_ANY_THROW(getDimension(-7));
}

TEST(Parallel_Operations_MPI, Get_Count_Vertex_Is_Correct) {
  EXPECT_EQ(getCountVertex(12), 4096);
}

TEST(Parallel_Operations_MPI, Get_Count_Vertex_Throws) {
  ASSERT_ANY_THROW(getCountVertex(-7));
}

TEST(Parallel_Operations_MPI, Can_Get_Hypercube) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int dim = getDimension(size);

  if ((size == getCountVertex(dim)) && (dim > 0)) {
    MPI_Comm cube = getHypercube(dim);
    int realDim;
    int status;
    int* dimension = new int[dim];
    int* period = new int[dim];
    int* coord = new int[dim];

    MPI_Cartdim_get(cube, &realDim);
    EXPECT_EQ(dim, realDim);

    MPI_Cart_get(cube, dim, dimension, period, coord);
    for (int i = 0; i < dim; i++) {
      EXPECT_EQ(period[i], 1);
      EXPECT_EQ(dimension[i], 2);
    }

    MPI_Topo_test(cube, &status);
    EXPECT_EQ(status, MPI_CART);
  }
}

TEST(Parallel_Operations_MPI, Get_Hypercube_throws) {
  ASSERT_ANY_THROW(getHypercube(-7));
}

TEST(Parallel_Operations_MPI, Can_Transfer_Data) {
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int dim = getDimension(size);

  if ((size == getCountVertex(dim)) && (dim > 0)) {
    MPI_Comm cube = getHypercube(dim);

    int tmp = -1;
    int left, right;
    for (int i = 0; i < dim; i++) {
      MPI_Cart_shift(cube, i, 1, &left, &right);
      if ((left == MPI_PROC_NULL) || (right == MPI_PROC_NULL)) {
        tmp = 17;
      }
    }

    if (rank == 0) {
      tmp = 17;
    }

    for (int i = 0; i < size; i++)
      MPI_Bcast(&tmp, 1, MPI_INT, 0, cube);

    ASSERT_EQ(tmp, 17);
  }
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

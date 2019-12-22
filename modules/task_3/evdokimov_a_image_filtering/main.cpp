// Copyright 2019 Evdokimov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./image_filtering.h"

TEST(Filtering_Image_MPI, Transpose_Correct_Work) {
    // Arrange
    int columns = 3;
    int rows = 2;
    std::vector<int> result = {1, 2, 3, 4, 5, 6};
    // Act
    result = transpose(result, rows, columns);
    // Result
    std::vector <int> expectedResult = {1, 4, 2, 5, 3, 6};
    // Assert
    ASSERT_EQ(result , expectedResult);
}

TEST(Filtering_Image_MPI, Image_Filtering_Correct_Work) {
    // Arrange
    int columns = 2;
    int rows = 2;
    // Act
    std::vector <int> result = imageFiltering(getTempImage({1, 2, 3, 4}, rows, columns), cernel(), rows, columns);
    // Result
    std::vector <int> expectedResult = {-8, -1, 6, 13};
    // Assert
    ASSERT_EQ(result , expectedResult);
}

TEST(Filtering_Image_MPI, Image_Filtering_MPI_Correct_Work) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int columns = 3;
    int rows = 3;
    std::vector<int> result = getTempImage(generateImage(rows, columns), rows, columns);
    // Result
    std::vector <int> expectedResult = imageFiltering(result, cernel(), rows, columns);
    // Act
    result = imageFilteringMPI(result, cernel(), rows, columns);
    // Assert
    if (rank == 0) {
        ASSERT_EQ(result, expectedResult);
    }
}

TEST(Filtering_Image_MPI, Image_Filtering_MPI_With_Not_Equals_Attributes) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int columns = 5;
    int rows = 2;
    std::vector<int> result = getTempImage(generateImage(rows, columns), rows, columns);
    // Result
    std::vector <int> expectedResult = imageFiltering(result, cernel(), rows, columns);
    // Act
    result = imageFilteringMPI(result, cernel(), rows, columns);
    // Assert
    if (rank == 0) {
        ASSERT_EQ(result, expectedResult);
    }
}

TEST(Filtering_Image_MPI, Exception_In_Transpose_With_Not_Equals_Attributes) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int columns = 0;
    int rows = -1;
    // Assert
    if (rank == 0) {
        ASSERT_ANY_THROW(transpose({1, 2, 3, 4} , rows, columns));
    }
}

TEST(Filtering_Image_MPI, Exception_In_Generate_Image_With_Not_Equals_Attributes) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int columns = 0;
    int rows = -1;
    // Assert
    if (rank == 0) {
        ASSERT_ANY_THROW(generateImage(rows, columns));
    }
}

TEST(Filtering_Image_MPI, Exception_In_Temp_Image_With_Not_Equals_Attributes) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int columns = 0;
    int rows = -1;
    // Assert
    if (rank == 0) {
        ASSERT_ANY_THROW(getTempImage({1, 2, 3, 4} , rows, columns));
    }
}

TEST(Filtering_Image_MPI, Exception_In_Filtering_With_Not_Equals_Attributes) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int columns = 0;
    int rows = -1;
    // Assert
    if (rank == 0) {
        ASSERT_ANY_THROW(imageFiltering({1, 2, 3, 4}, cernel(), rows, columns));
    }
}

TEST(Filtering_Image_MPI, Exception_In_FilteringMPI_With_Not_Equals_Attributes) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int columns = 0;
    int rows = -1;
    // Assert
    if (rank == 0) {
        ASSERT_ANY_THROW(imageFilteringMPI({1, 2, 3, 4}, cernel(), rows, columns));
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

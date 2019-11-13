// Copyright 2019 Denisov Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cstring>
#include "./count_frequency_char_in_str.h"

TEST(Count_frequency_char_in_str, count_a_character_in_string) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char str[] = "aababas";
    char ch = 'a';

    int count = getCountFreqCharInStr(str, ch);

    if (rank == 0) {
        int seqCount = countFreqSeq(str, ch);
        ASSERT_EQ(seqCount, count);
    }
}

TEST(Count_frequency_char_in_str, count_a_missing_character_in_string) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char str[] = "aababas";
    char ch = 'z';

    int count = getCountFreqCharInStr(str, ch);

    if (rank == 0) {
        int seqCount = countFreqSeq(str, ch);
        ASSERT_EQ(seqCount, count);
    }
}

TEST(Count_frequency_char_in_str, count_character_in_string_containing_only_that_character) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char str[] = "aaaaaaa";
    char ch = 'a';

    int count = getCountFreqCharInStr(str, ch);
    if (rank == 0) {
        int seqCount = countFreqSeq(str, ch);
        ASSERT_EQ(seqCount, count);
    }
}

TEST(Count_frequency_char_in_str, can_create_random_string) {
    int size = 10;
    char *str = new char[size];
    ASSERT_NO_THROW(getRandomStr(str, size));
}

TEST(Count_frequency_char_in_str, throw_when_create_random_string_with_negative_size) {
    int size = -5;
    char *str = new char[5];
    ASSERT_ANY_THROW(getRandomStr(str, size));
}

TEST(Count_frequency_char_in_str, count_character_in_random_string) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10;
    char *str = new char[size + 1];
    char ch = 'a';

    getRandomStr(str, size);

    int count = getCountFreqCharInStr(str, ch);

    if (rank == 0) {
        int seqCount = countFreqSeq(str, ch);
        ASSERT_EQ(seqCount, count);
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

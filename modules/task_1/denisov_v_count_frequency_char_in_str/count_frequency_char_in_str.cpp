// Copyright 2019 Denisov Vladislav
#include <mpi.h>
#include <random>
#include <cstring>
#include <iostream>
#include "../../../modules/task_1/denisov_v_count_frequency_char_in_str/count_frequency_char_in_str.h"

void getRandomStr(char *str, int strSize) {
    const char arr[] = "abcdefghijklmnopqrstuvwxyz";
    if (strSize <= 0)
        throw "Error size str";
    for (int i = 0; i < strSize; i++) {
        str[i] = arr[rand() % (strlen(arr) - 1)];
    }

    str[strSize] = 0;
}

int countFreqSeq(char *str, char ch) {
    int count = 0;
    size_t length = strlen(str);
    for (size_t i = 0; i < length; i++) {
        if (str[i] == ch)
            count++;
    }
    return count;
}

int getCountFreqCharInStr(char* str, char ch) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = strlen(str) / size;
    int remainder = strlen(str) % size;

    int globalCount = 0;
    int localCount = 0;

    char* strLocal = new char[delta + 1];
    char chLocal;

    if (rank == 0) {
        if (delta != 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&str[0] + proc * delta + remainder, delta, MPI_CHAR, proc, 1, MPI_COMM_WORLD);
                MPI_Send(&ch, 1, MPI_CHAR, proc, 2, MPI_COMM_WORLD);
            }
        }
        delete[] strLocal;
        strLocal = new char[delta + remainder + 1];
    }

    if (rank == 0) {
        for (int i = 0; i < delta + remainder; i++) {
            strLocal[i] = str[i];
        }
        strLocal[delta + remainder] = 0;
        chLocal = ch;
    } else {
        MPI_Status status;
        if (delta != 0) {
            MPI_Recv(&strLocal[0], delta, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&chLocal, 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);
            strLocal[delta] = 0;
        }
    }

    localCount = countFreqSeq(strLocal, chLocal);

    MPI_Barrier(MPI_COMM_WORLD);

    if (delta != 0) {
        MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    } else {
        globalCount = localCount;
    }
    delete[] strLocal;

    return globalCount;
}

// Copyright 2019 Lembrikov Stepan
#ifndef MODULES_TASK_1_LEMBRIKOV_S_MIN_ELEM_VECTOR_MIN_ELEM_VECTOR_H_
#define MODULES_TASK_1_LEMBRIKOV_S_MIN_ELEM_VECTOR_MIN_ELEM_VECTOR_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> getRandomVector(int n);
std::vector<int> getIdentityVector(int n);
std::vector<int> getConstVector(int n, int c);
std::vector<int> getNegativeVector(int n);
int MinOfVector(const std::vector <int> a, int n);

#endif  // MODULES_TASK_1_LEMBRIKOV_S_MIN_ELEM_VECTOR_MIN_ELEM_VECTOR_H_

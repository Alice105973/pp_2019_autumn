// Copyright 2019 Kukushkina Ksenia
#ifndef MODULES_TASK_2_KUKUSHKINA_K_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_
#define MODULES_TASK_2_KUKUSHKINA_K_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_

#include <vector>

double preprocess(std::vector<double> A, std::vector<double> b);
std::vector<double> GenerateVector(int n);
std::vector<double> GenerateMatrix(int n);
std::vector<double> SimpleIterations(std::vector<double> A, std::vector<double> b, double precision);

#endif  // MODULES_TASK_2_KUKUSHKINA_K_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_

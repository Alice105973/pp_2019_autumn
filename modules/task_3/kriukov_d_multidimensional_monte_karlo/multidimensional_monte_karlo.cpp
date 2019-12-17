// Copyright 2019 Kriukov Dmitry
#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/kriukov_d_multidimensional_monte_karlo/multidimensional_monte_karlo.h"

double multidimensionalIntegrationSequential(std::vector<double> start_point, double side,
    double(*pfunc)(std::vector<double>), bool(*parea)(std::vector<double>), unsigned int dimension, int point_count) {
    std::mt19937 gen;
    time_t curr_time = static_cast<time_t>(0);
    gen.seed(static_cast<unsigned int>(time(&curr_time)));
    std::uniform_real_distribution<> urd(0, 1);
    int delta = point_count;

    std::vector<double> points(dimension * delta);
    for (unsigned int i = 0; i < points.size(); i++) {
        points[i] = start_point[i%dimension] + urd(gen)*side;
    }

    int num_inside = 0;
    double mean = 0;

    for (int i = 0; i < delta; ++i) {
        std::vector<double> cpoint;
        for (unsigned int j = 0; j < dimension; ++j) {
            cpoint.push_back(points[dimension * i + j]);
        }
        if (parea(cpoint)) {
            num_inside++;
            mean += pfunc(cpoint);
        }
    }

    mean = mean / num_inside;
    double area = std::pow(side, dimension) * num_inside / point_count;

    return area * mean;
}


double multidimensionalIntegration(std::vector<double> start_point, double side, double(*pfunc)(std::vector<double>),
                                   bool(*parea)(std::vector<double>), unsigned int dimension, int point_count) {
    if (start_point.size() != dimension)
        throw(1);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::mt19937 gen;
    time_t curr_time = static_cast<time_t>(rank);
    gen.seed(static_cast<unsigned int>(time(&curr_time)));
    std::uniform_real_distribution<> urd(0, 1);
    int delta = point_count / size;

    std::vector<double> points(dimension * delta);
    for (unsigned int i = 0; i < points.size(); i++) {
        points[i] = start_point[i%dimension] + urd(gen)*side;
    }

    int local_num_inside = 0;
    int num_inside = 0;
    double local_mean = 0;
    double mean = 0;

    for (int i = 0; i < delta; ++i) {
        std::vector<double> cpoint;
        for (unsigned int j = 0; j < dimension; ++j) {
            cpoint.push_back(points[dimension * i + j]);
        }
        if (parea(cpoint)) {
            local_num_inside++;
            local_mean += pfunc(cpoint);
        }
    }
    MPI_Reduce(&local_num_inside, &num_inside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_mean, &mean, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    mean = mean / num_inside;
    double area = std::pow(side, dimension) * num_inside / point_count;

    return area * mean;
}

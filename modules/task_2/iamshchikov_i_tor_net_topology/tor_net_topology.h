// Copyright 2019 Iamshchikov Ivan
#ifndef MODULES_TASK_2_IAMSHCHIKOV_I_TOR_NET_TOPOLOGY_TOR_NET_TOPOLOGY_H_
#define MODULES_TASK_2_IAMSHCHIKOV_I_TOR_NET_TOPOLOGY_TOR_NET_TOPOLOGY_H_

#include <mpi.h>
#include <vector>

int getNodenumInNetworkTopology(const std::vector<int> dims);
MPI_Comm createTorusTopology(const std::vector<int> dims,
                             const MPI_Comm comm_base = MPI_COMM_WORLD);
bool isNeighbor(MPI_Comm comm_cart, std::vector<int> dims, int ndims,
                int root, int dest);
void send(const std::vector<int> data, int count, MPI_Datatype datatype,
          int root, int dest, std::vector<int> dims, MPI_Comm comm_cart);

#endif  // MODULES_TASK_2_IAMSHCHIKOV_I_TOR_NET_TOPOLOGY_TOR_NET_TOPOLOGY_H_

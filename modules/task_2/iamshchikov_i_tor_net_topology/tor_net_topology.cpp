// Copyright 2019 Iamshchikov Ivan
#include <vector>
#include "../../../modules/task_2/iamshchikov_i_tor_net_topology/tor_net_topology.h"

int getNodenumInNetworkTopology(const std::vector<int> dims) {
    if (dims.size() == 0) throw -1;
    int nodenum = 1;
    for (size_t i = 0; i < dims.size(); ++i)
        nodenum *= dims[i];
    return nodenum;
}

MPI_Comm createTorusTopology(const std::vector<int> dims,
                             const MPI_Comm comm_base) {
    int procnum_in_base_comm;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    int nodenum_in_network_topology = getNodenumInNetworkTopology(dims);
    if (nodenum_in_network_topology > procnum_in_base_comm) {
    throw -1;
    } else {
        MPI_Comm comm_cart = MPI_COMM_NULL;
        int ndims = dims.size();
        std::vector<int> periods(ndims);
        periods.assign(ndims, 1);
        MPI_Cart_create(comm_base, ndims, dims.data(), periods.data(),
                        false, &comm_cart);
        return comm_cart;
    }
}

bool isNeighbor(MPI_Comm comm_cart, std::vector<int> dims, int ndims,
                int root, int dest) {
    if (root == dest) {
        return false;
    } else {
        std::vector<int> coord_root(ndims), coord_dest(ndims), dims(ndims);
        MPI_Cart_coords(comm_cart, root, ndims, coord_root.data());
        MPI_Cart_coords(comm_cart, dest, ndims, coord_dest.data());
        for (int i = 0; i < ndims; ++i)
            if (coord_root[i] - coord_dest[i] != 1
                || coord_root[i] - coord_dest[i] != -1
                || coord_root[i] - coord_dest[i] != dims[i]-1
                || coord_root[i] - coord_dest[i] != 1-dims[i])
                return false;
        return true;
    }
}

void send(const std::vector<int> data, int count, MPI_Datatype datatype,
          int root, int dest, std::vector<int> dims, MPI_Comm comm_cart) {
    int procnum, ndims;
    std::vector<int> neighbors_ranks;
    if (comm_cart == MPI_COMM_NULL) throw -1;
    MPI_Comm_size(comm_cart, &procnum);
    if (root >= procnum || dest >= procnum) throw -1;
    MPI_Cartdim_get(comm_cart, &ndims);
    if (!isNeighbor(comm_cart, dims, ndims, root, dest)) throw -1;
}

#include "CXHDF5.hh"

#include <vector>
#include <string>
#include <sstream>

void CX::CXHDF5::initialize(const std::vector<int> & dims)
{
    dims_.clear();
    dims_.resize(dims.size());
    for ( unsigned i=0; i != dims_.size(); i++)
        dims_[i] = dims[i];
}

void CX::CXHDF5::open_file(const std::string & filename )
{
    file_ = H5Fcreate(filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
    dataspace_ = H5Screate_simple(dims_.size(), &dims_[0], NULL);

    hsize_t primarydims[1] = {1};
    primaryspace_ = H5Screate_simple(1, primarydims, NULL);

    datagrp_ = H5Gcreate(file_, "/data", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    primgrp_ = H5Gcreate(file_, "/primary", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

}

void CX::CXHDF5::close_file()
{
    H5Sclose(dataspace_);
    H5Fclose(file_);
}


void CX::CXHDF5::write_event(unsigned id, void * data)
{
    std::stringstream dataname;
    dataname << "/data/" << id;
    auto dataset = H5Dcreate(file_, dataname.str().c_str(), H5T_NATIVE_FLOAT, dataspace_, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    H5Dclose(dataset);
}

void CX::CXHDF5::write_primary(unsigned id, void * data)
{
    std::stringstream dataname;
    dataname << "/primary/" << id;
    auto dataset = H5Dcreate(file_, dataname.str().c_str(), H5T_NATIVE_FLOAT, primaryspace_, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    H5Dclose(dataset);
}

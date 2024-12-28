#ifndef CXHDF_HH
#define CXHDF_HH

#include <vector>
#include <string>

#include "hdf5.h"

namespace CX {

class CXHDF5 {
public:

    CXHDF5():dims_(1.,0) { }
    ~CXHDF5() { }
    
    ///
    /// Initialize the data space, set the 
    /// dimensions of the stored arrays.
    void initialize(const std::vector<int> & dims);

    ///
    /// Open a new file
    void open_file(const std::string & filename);

    ///
    /// Close file
    void close_file();

    ///
    /// write event data to file.
    void write_event(unsigned, void *);

    ///
    /// write primary info to file
    void write_primary(unsigned, void *);

    ///
    /// return dimensions
    const std::vector<hsize_t> & get_dims() const { return dims_; }

    ///
    /// return size
    unsigned size() const;


private:

    hid_t file_;
    hid_t dataspace_;
    hid_t primaryspace_;

    hid_t datagrp_;
    hid_t primgrp_;
   
    std::vector<hsize_t> dims_; 

};

inline unsigned CXHDF5::size() const  {
    unsigned ss = dims_[0];
    unsigned ndim = dims_.size();
    for ( unsigned i=1; i != ndim; i++ )
        ss *= dims_[i];
    return ss;
}

}

#endif

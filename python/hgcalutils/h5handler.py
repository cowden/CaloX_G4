"""Handle hdf5 files from calox-hgcal G4 simulation"""

import os
import numpy as np
from operator import itemgetter
import h5py

from typing import List, Optional, Tuple

def _check_top_level_keys(h5file: h5py.File) -> bool:
    """Check top level hdf5 keys.

    Return false if the group structure is incompatible"""

    keys = h5file.keys()
    if "primary" not in keys:
        return False
    elif "data" not in keys:
        return False
    elif len(h5file["primary"]) == 0:
        return False
    elif len(h5file["data"]) == 0:
        return False
    elif len(h5file["primary"]) != len(h5file["data"]):
        return False

    return True

def list_data_files(pth: Optional[str] = None) -> List[str]:
    """List calox-hgcal hdf5 files"""
    
    if not pth:
        pth = "."

    flst = [fl for fl in os.listdir(pth) if fl[-3:] == ".h5"]

    data_files = []
    for fl in flst:
        name = os.path.join(pth, fl)
        h5file = h5py.File(name,"r")
        if _check_top_level_keys(h5file):
            data_files.append(name)
        h5file.close()

    return data_files

def extract_numpy_data(h5file: h5py.File) -> Tuple[np.ndarray, np.ndarray]:
    """Return numpy arrays of data"""

    assert sorted(h5file["primary"].keys()) == sorted(h5file["data"].keys())
    y = [(int(k),h5file["primary"][k][:]) for k in h5file["primary"].keys()]
    X = [(int(k),h5file["data"][k][:]) for k in h5file["data"].keys()]

    y = sorted(y, key=itemgetter(0))
    X = sorted(X, key=itemgetter(0))

    y = np.array([p[1] for p in y])
    X = np.array([d[1] for d in X])

    return y, X


def merge_data(data_list: List[Tuple[np.ndarray, np.ndarray]]) -> Tuple[np.ndarray, np.ndarray]:
    """Extract and merge data into numpy arrays"""

    y = np.concatenate([p[0] for p in data_list])
    X = np.concatenate([p[1] for p in data_list])

    return y, X

def collect_and_merge(file_list: List[str]) -> Tuple[np.ndarray, np.ndarray]:
    """return merged data from a list of files"""

    data = []
    for fpath in file_list:
        fl = h5py.File(fpath,"r")
        data.append( extract_numpy_data(fl) )
        fl.close()

    y, X = merge_data(data)

    return y, X



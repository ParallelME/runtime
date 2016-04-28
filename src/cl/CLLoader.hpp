/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef CL_CLLOADER_HPP
#define CL_CLLOADER_HPP

#include "../Loader.hpp"
#include "dynloader/dynLoader.h"
#include <Device.hpp>


/**
 * OpenCL loader. This class dynamically loads OpenCL at the runtime through
 * dlfcn.h's functions and prepares the OpenCL's devices for execution.
 */
class CLLoader : public Loader {
    DeviceVector _devices;

    /// Returns the device type of a given device.
    DeviceType findType(cl_device_id id);

public:
    CLLoader(JavaVM *jvm);

    ~CLLoader();

    inline const DeviceVector &devices() {
        return _devices;
    }
};

#endif // !CL_CLLOADER_HPP

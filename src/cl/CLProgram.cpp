/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <cl/CLProgram.hpp>
#include <Runtime.hpp>
#include "CLProgramObject.hpp"
#include "CLDevice.hpp"

CLProgram::CLProgram(std::shared_ptr<Runtime> runtime, const char *source,
        const char *compilerFlags) {
    for(auto &device : runtime->devices()) {
        if(device->platform() != OpenCL)
            continue;

        auto obj = std::make_shared<CLProgramObject>(device, source, compilerFlags);

        // Only add the program object if it compiled successfully.
        if(obj->valid())
            _programObjects.insert(ProgramObjectPair(device->id(), obj));
    }
}

CLProgram::~CLProgram() = default;

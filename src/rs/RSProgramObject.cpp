/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include "RSProgramObject.hpp"
#include "RSKernelObject.hpp"
#include "RSDevice.hpp"

RSProgramObject::RSProgramObject(std::shared_ptr<Device> device, jobject localScriptc)
        : ProgramObject(device), _scriptc(nullptr) {
    auto env = device->jenv();

    _scriptc = env->NewGlobalRef(localScriptc);
    if(!_scriptc)
        throw std::runtime_error("Failed to create a global ref.");
}

RSProgramObject::~RSProgramObject() {
    auto env = device()->jenv();

    if(_scriptc) {
        env->DeleteGlobalRef(_scriptc);
        _scriptc = nullptr;
    }
}

std::unique_ptr<KernelObject> RSProgramObject::createKernelObject(
        const std::string &name) {
    return std::make_unique<RSKernelObject>(name, _device, _scriptc);
}

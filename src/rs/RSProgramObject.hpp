/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef RS_RSPROGRAMOBJECT_HPP
#define RS_RSPROGRAMOBJECT_HPP

#include "../ProgramObject.hpp"
#include <jni.h>

/**
 * This class implements the ProgramObject interface for RenderScript.
 */
class RSProgramObject : public ProgramObject {
    jobject _scriptc;

public:
    /**
     * Constructs the ProgramObject.
     * Needs a RenderScript device and the ScriptC object.
     */
    RSProgramObject(std::shared_ptr<Device> device, jobject scriptc);

    ~RSProgramObject();

    inline bool valid() {
         return _scriptc != nullptr;
    }

    std::unique_ptr<KernelObject> createKernelObject(const std::string &name);
};

#endif // !RS_RSPROGRAMOBJECT_HPP

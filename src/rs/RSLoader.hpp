/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef RS_RSLOADER_HPP
#define RS_RSLOADER_HPP

#include "../Loader.hpp"
#include <Device.hpp>

class RSLoader : public Loader {
    DeviceVector _devices;

public:
    /**
     * To load RenderScript, the JavaVM and the android context are needed.
     * @see Runtime
     */
    RSLoader(JavaVM *jvm, jobject androidContext);
    ~RSLoader();

    inline const DeviceVector &devices() {
        return _devices;
    }
};

#endif // !RS_RSLOADER_HPP

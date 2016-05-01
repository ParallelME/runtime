/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef PARALLELME_BUFFER_HPP
#define PARALLELME_BUFFER_HPP

#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <jni.h>

struct _cl_mem;

namespace parallelme {
class Device;
class Worker;

/**
 * Exception thrown if the buffer failed to construct and initialize.
 * The error message can be accessed through the what() function.
 */
class BufferConstructionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Exception thrown if the buffer failed to copy data to or from the device.
 * The error message can be accessed through the what() function.
 */
class BufferCopyError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * This class represents a buffer that can be used to send and receive data
 * to/from the kernels.
 * Please pay attention to the saveCopyFrom functions: they don't make a
 * copy on the spot, instead saving a reference to the array, buffer or pointer
 * and only copying right before execution.
 */
class Buffer {

public:
    /**
     * Creates a memory buffer.
     * @param size The size in bytes of the buffer.
     */
    Buffer(size_t size);

    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    ~Buffer();

    /**
     * Helper method to copy size() bytes from a jarray.
     * Important! If copying from a Java array, don't forget
     * that inside the configFunction only global references survive, local
     * references will have already been cleaned up by the JNI. Because of this,
     * create a global reference for the jarray before entering the
     * configFunction.
     * Also note that the device where the memory object will be created should
     * be the parameter of the configFunction.
     * @see Task::setConfigFunction
     */
    void copyFrom(std::shared_ptr<Device> device, jarray array);

    /**
     * Helper method to copy size() bytes from an Android bitmap.
     * Important! If copying from an Android bitmap, don't forget
     * that inside the configFunction only global references survive, local
     * references will have already been cleaned up by the JNI. Because of this,
     * create a global reference for the Android bitmap before entering the
     * configFunction.
     * Also note that the device where the memory object will be created should
     * be the parameter of the configFunction.
     * @see Task::setConfigFunction
     */
    void copyFrom(std::shared_ptr<Device> device, jobject bitmap);

    /**
     * Copies size() bytes from the host pointer to the internal memory. Please
     * note that if this not called inside a Task's configFunction, the copy
     * will not work and will probably cause a segfault.
     * Also note that the device where the memory object will be created should
     * be the parameter of the configFunction.
     * @see Task::setConfigFunction
     */
    void copyFrom(std::shared_ptr<Device> device, void *host);

    /**
     * Helper method to copy size() bytes to a jarray. If the buffer doesn't
     * have any data this will cause a segfault.
     */
    void copyTo(JNIEnv *env, jarray array);

    /**
     * Helper method to copy size() bytes to an Android bitmap. If the buffer
     * doesn't have any data this will cause a segfault.
     */
    void copyTo(JNIEnv *env, jobject bitmap);

    /**
     * Copies size() bytes to an host pointer. If the buffer doesn't have any
     * data this will cause a segfault.
     */
    void copyTo(void *host);

    /**
     * Returns the size of the buffer in bytes.
     */
    inline size_t size() {
        return _size;
    }

    /**
     * @internal
     * Returns the OpenCL memory object. Creates it if it wasn't created yet.
     * This should only be called by the Kernel class.
     */
    _cl_mem *clMem(std::shared_ptr<Device> device);

private:
    /**
     * Creates an OpenCL memory object on the given device or makes a copy to
     * a memory object on the new device if one already exists and the flag
     * is set.
     */
    void createMemoryObject(std::shared_ptr<Device> newDevice, bool copyOld);

    size_t _size;                       /// Size of the buffer.
    _cl_mem *_mem;                      /// Pointer to the buffer.
    std::shared_ptr<Device> _device;    /// Device of the buffer.
};

}

#endif // !PARALLELME_BUFFER_HPP

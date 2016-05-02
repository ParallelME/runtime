/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <parallelme/Buffer.hpp>
#include <parallelme/Device.hpp>
#include <string>
#include <android/bitmap.h>
#include <jni.h>
#include "dynloader/dynLoader.h"
using namespace parallelme;

Buffer::Buffer(size_t size) : _size(size), _mem(nullptr), _device(nullptr) {

}

Buffer::~Buffer() {
    if(_mem) {
        clReleaseMemObject(_mem);
        _mem = nullptr;
    }
}

void Buffer::copyFrom(std::shared_ptr<Device> device, jarray array) {
    JNIEnv *env = device->JNIEnv();

    void *ptr = env->GetPrimitiveArrayCritical(array, nullptr);
    if(!ptr)
        throw BufferCopyError("Failed to get primitive array.");

    copyFrom(device, ptr);

    env->ReleasePrimitiveArrayCritical(array, ptr, 0);
}

void Buffer::copyFrom(std::shared_ptr<Device> device, jobject bitmap) {
    JNIEnv *env = device->JNIEnv();

    void *ptr;
    int err = AndroidBitmap_lockPixels(env, bitmap, &ptr);
    if(err < 0)
        throw BufferCopyError("Failed to lock android bitmap's pixels.");

    copyFrom(device, ptr);

    AndroidBitmap_unlockPixels(env, bitmap);
}

void Buffer::copyFrom(std::shared_ptr<Device> device, void *host) {
    auto mem = clMem(device, false);
    int err;

    void *data = clEnqueueMapBuffer(_device->clQueue(), mem, CL_TRUE,
            CL_MAP_WRITE, 0, _size, 0, nullptr, nullptr, &err);
    if(err < 0)
        throw BufferCopyError(std::to_string(err));

    memcpy(data, host, _size);
    clEnqueueUnmapMemObject(_device->clQueue(), mem, data, 0, nullptr, nullptr);
}

void Buffer::copyTo(JNIEnv *env, jarray array) {
    void *ptr = env->GetPrimitiveArrayCritical(array, nullptr);
    if(!ptr)
        throw BufferCopyError("Failed to get primitive array.");

    copyTo(ptr);

    env->ReleasePrimitiveArrayCritical(array, ptr, 0);
}

void Buffer::copyTo(JNIEnv *env, jobject bitmap) {
    void *ptr;
    int err = AndroidBitmap_lockPixels(env, bitmap, &ptr);
    if(err < 0)
        throw BufferCopyError(std::to_string(err));

    copyTo(ptr);

    AndroidBitmap_unlockPixels(env, bitmap);
}

void Buffer::copyTo(void *host) {
    auto mem = clMem(_device);
    int err;

    void *data = clEnqueueMapBuffer(_device->clQueue(), mem, CL_TRUE,
            CL_MAP_READ, 0, _size, 0, nullptr, nullptr, &err);
    if(err < 0)
        throw BufferCopyError(std::to_string(err));

    memcpy(host, data, _size);
    clEnqueueUnmapMemObject(_device->clQueue(), mem, data, 0, nullptr, nullptr);
}

_cl_mem *Buffer::clMem(std::shared_ptr<Device> device, bool copyOld) {
    if(_device != device)
        createMemoryObject(device, copyOld);

    return _mem;
}

void Buffer::createMemoryObject(std::shared_ptr<Device> newDevice, bool copyOld) {
    int err;
    auto newMem = clCreateBuffer(newDevice->clContext(), CL_MEM_READ_WRITE,
            _size, nullptr, &err);
    if(err < 0)
        throw BufferConstructionError(std::to_string(err));

    // If there is a memory object already, do a copy and delete the old mem.
    if(copyOld && _mem) {
        void *oldData = clEnqueueMapBuffer(_device->clQueue(), _mem, CL_TRUE,
                CL_MAP_READ, 0, _size, 0, nullptr, nullptr, &err);
        if(err < 0)
            throw BufferCopyError(std::to_string(err));
        void *newData = clEnqueueMapBuffer(newDevice->clQueue(), newMem, CL_TRUE,
                CL_MAP_WRITE, 0, _size, 0, nullptr, nullptr, &err);
        if(err < 0)
            throw BufferCopyError(std::to_string(err));

        memcpy(newData, oldData, _size);

        clEnqueueUnmapMemObject(_device->clQueue(), _mem, oldData, 0, nullptr, nullptr);
        _device->finish();
        clEnqueueUnmapMemObject(newDevice->clQueue(), newMem, newData, 0, nullptr, nullptr);

        clReleaseMemObject(_mem);
    }

    _mem = newMem;
    _device = newDevice;
}


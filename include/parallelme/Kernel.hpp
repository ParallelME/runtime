/**                                               _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef PARALLELME_KERNEL_HPP
#define PARALLELME_KERNEL_HPP

#include <cstdlib>
#include <string>
#include <stdexcept>

struct _cl_kernel;

namespace parallelme {
class Buffer;
class Device;
class Program;
class Task;

/**
 * Exception thrown if the kernel failed execute.
 * The error message can be accessed through the what() function.
 */
class KernelExecutionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Exception thrown if the kernel failed to be created.
 * The error message can be accessed through the what() function.
 */
class KernelConstructionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Exception thrown if the kernel failed to configure the argument.
 * The error message can be accessed through the what() function.
 */
class KernelArgError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * This class represents a Task's kernel.
 */
class Kernel {
    friend class Task;

    /**
     * Executes the kernel with the given work size.
     */
    void run();

    /**
     * Constructs the Kernel object. Only the Task class can do it.
     * To create a kernel to be used with a task, call the Task::addKernel()
     * function.
     * @see Task::addKernel
     */
    Kernel(const std::string &name, std::shared_ptr<Device> device, Program &program);

public:
    Kernel(const Kernel &) = delete;
    Kernel &operator=(const Kernel &) = delete;

    ~Kernel();

    /**
     * Sets the range of execution.
     * @param workSize The number of work items.
     */
    inline Kernel *setWorkSize(size_t workSize) {
        _workSize = workSize;

        return this;
    }

    /**
     * Sets a buffer as the argument with the given id.
     * Must only be called inside a ConfigFunction.
     */
    Kernel *setArg(unsigned id, std::shared_ptr<Buffer> buffer);

    /**
     * Sets a primitive type as the argument with the given id.
     * Must only be called inside a ConfigFunction.
     * @see Task::ConfigFunction
     */
    template<typename T>
    Kernel *setArg(unsigned id, T primitive) {
        return setPrimitiveArg(id, sizeof(primitive), &primitive);
    }

    /// Returns the the kernel work size.
    inline size_t workSize() {
        return _workSize;
    }

private:
    /**
     * Sets a PRIMITIVE type as the argument to the kernel. Trying to use this
     * function with other types will cause a segfault.
     */
    Kernel *setPrimitiveArg(unsigned id, size_t size, void *host);

    std::shared_ptr<Device> _device;
    _cl_kernel *_clKernel;
    size_t _workSize;
};

}

#endif // !PARALLELME_KERNEL_HPP

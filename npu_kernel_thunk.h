//
// Created by wehu on 18-4-11.
//

#ifndef TENSORFLOW_NPU_KERNEL_THUNK_H
#define TENSORFLOW_NPU_KERNEL_THUNK_H

#include "npu_buffer_allocations.h"
#include "npu_thunk.h"

#include <memory>
#include <string>
#include <vector>

#include "tensorflow/compiler/xla/service/buffer_assignment.h"
#include "tensorflow/compiler/xla/service/hlo_instruction.h"
#include "tensorflow/compiler/xla/types.h"
#include "tensorflow/compiler/xla/service/cpu/simple_orc_jit.h"
#include "tensorflow/core/lib/gtl/array_slice.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/stream_executor_no_cuda.h"
#include "tensorflow/core/platform/thread_annotations.h"

namespace xla {
    namespace npu {

        class NpuExecutable;

        class NpuKernelThunk : public NpuThunk {
        public:
            // Constructs a thunk for the given kernel.
            //
            // `hlo_instruction` is as in Thunk. Other arguments are as the class members.
            NpuKernelThunk(tensorflow::gtl::ArraySlice<const BufferAllocation *> args,
                           const string &kernel_name,
                           const HloInstruction *hlo_instruction,
                           xla::cpu::SimpleOrcJIT *jit);

            ~NpuKernelThunk() override = default;

            const string &kernel_name() const { return kernel_name_; }

            // Executes the kernel for the thunk on "stream", which must be non-null.
            tensorflow::Status ExecuteOnStream(
                    const NpuBufferAllocations &buffer_allocations,
                    perftools::gputools::Stream *stream) override;

        private:
            // Buffers passed to the kernel as arguments.
            const std::vector<const BufferAllocation *> args_;

            // Entry kernel name for the computation.
            const string kernel_name_;

            xla::cpu::SimpleOrcJIT *jit_;

            SE_DISALLOW_COPY_AND_ASSIGN(NpuKernelThunk);
        };

    }  // namespace npu

} // namespace xla

#endif //TENSORFLOW_NPU_KERNEL_THUNK_H

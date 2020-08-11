/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_CORE_TPU_KERNELS_TRANSFER_OPS_H_
#define TENSORFLOW_CORE_TPU_KERNELS_TRANSFER_OPS_H_

#include "tensorflow/compiler/jit/xla_device.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/util/stream_executor_util.h"
#include "tensorflow/stream_executor/tpu/tpu_transfer_manager_interface.h"

namespace tensorflow {

// Base class providing common functionality for async ops that transfer from
// host to TPU.
class TpuTransferAsyncOpKernel : public AsyncOpKernel {
 public:
  explicit TpuTransferAsyncOpKernel(OpKernelConstruction* ctx,
                                    const string& transfer_type,
                                    int number_of_threads);

  void ComputeAsync(OpKernelContext* ctx, DoneCallback done) override;

 protected:
  virtual Status DoWork(OpKernelContext* context,
                        xla::TpuTransferManagerInterface* transfer_manager,
                        stream_executor::StreamExecutor* stream_executor) = 0;

 private:
  Status RunTransfer(OpKernelContext* ctx);
  void Cancel();

  std::unique_ptr<thread::ThreadPool> thread_pool_;
  int device_ordinal_;
  mutex mu_;

  // TpuTransferAsyncOpKernel is neither copyable nor movable.
  TpuTransferAsyncOpKernel(const TpuTransferAsyncOpKernel&) = delete;
  TpuTransferAsyncOpKernel& operator=(const TpuTransferAsyncOpKernel&) = delete;
};

}  // namespace tensorflow

#endif  // TENSORFLOW_CORE_TPU_KERNELS_TRANSFER_OPS_H_

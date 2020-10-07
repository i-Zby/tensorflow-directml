/* Copyright (c) Microsoft Corporation.

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

#pragma once

#include "dml_common.h"
#include "dml_device_removal_handler.h"
#include "dml_event_queue.h"
#include "dml_execution_context.h"
#include "dml_pooled_heap.h"

namespace tensorflow {
class DmlExecutionContext;

// Performs non-blocking readback from GPU resources. This class is thread-safe.
class DmlReadbackHeap : public DmlPooledHeap, public DmlDeviceRemovalHandler {
 public:
  DmlReadbackHeap(ID3D12Device* device, DmlExecutionContext* execution_context,
                  DmlEventQueue* event_queue,
                  DmlDeviceRemovedStatus* device_removed_status);

  // Copies data from the specified GPU resource into CPU memory pointed-to by
  // the span. This is non-blocking; the copy is not complete until the returned
  // event becomes signaled. Both the dst buffer and src resource must stay
  // alive until the copy is complete.
  StatusOr<DmlGpuEvent> ReadbackFromGpu(absl::Span<uint8_t> dst,
                                        ID3D12Resource* src,
                                        uint64_t src_offset,
                                        D3D12_RESOURCE_STATES src_state);

  void HandleDeviceRemoval() final;

 private:
  std::mutex mutex_;
  DmlExecutionContext* execution_context_;  // weak; owned by DmlDeviceState
  DmlEventQueue* event_queue_;              // weak; owned by DmlDeviceState

  // We maintain a completion event independent of the execution context,
  // because the execution context's completion event only tells you when the
  // copy to the readback heap has completed, whereas what the caller cares
  // about is whether the copy to the `dst` buffer is complete.
  DmlGpuEvent current_completion_event_;

  ID3D12Device* d3d12_device_;
};

}  // namespace tensorflow

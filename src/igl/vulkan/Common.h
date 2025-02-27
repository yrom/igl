/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cassert>

// set to 1 to see very verbose debug console logs with Vulkan commands
#define IGL_VULKAN_PRINT_COMMANDS 0

#if !defined(VK_NO_PROTOTYPES)
#define VK_NO_PROTOTYPES 1
#endif // !defined(VK_NO_PROTOTYPES)

#include <igl/Macros.h>
#include <volk.h>
#if IGL_PLATFORM_MACOS
#include <vulkan/vulkan_metal.h>
#endif

#include <igl/Common.h>
#include <igl/DepthStencilState.h>
#include <igl/Texture.h>
#include <igl/vulkan/VulkanHelpers.h>

// libc++'s implementation of std::format has a large binary size impact
// (https://github.com/llvm/llvm-project/issues/64180), so avoid it on Android.
#if defined(__cpp_lib_format) && !defined(__ANDROID__)
#include <format>
#define IGL_FORMAT std::format
#else
#include <fmt/core.h>
#define IGL_FORMAT fmt::format
#endif // __cpp_lib_format

// Enable to use VulkanMemoryAllocator (VMA)
#define IGL_VULKAN_USE_VMA 1

#define VK_ASSERT(func)                                            \
  {                                                                \
    const VkResult vk_assert_result = func;                        \
    if (vk_assert_result != VK_SUCCESS) {                          \
      IGL_LOG_ERROR("Vulkan API call failed: %s:%i\n  %s\n  %s\n", \
                    __FILE__,                                      \
                    __LINE__,                                      \
                    #func,                                         \
                    ivkGetVulkanResultString(vk_assert_result));   \
      assert(false);                                               \
    }                                                              \
  }

#define VK_ASSERT_RETURN(func)                                     \
  {                                                                \
    const VkResult vk_assert_result = func;                        \
    if (vk_assert_result != VK_SUCCESS) {                          \
      IGL_LOG_ERROR("Vulkan API call failed: %s:%i\n  %s\n  %s\n", \
                    __FILE__,                                      \
                    __LINE__,                                      \
                    #func,                                         \
                    ivkGetVulkanResultString(vk_assert_result));   \
      assert(false);                                               \
      return getResultFromVkResult(vk_assert_result);              \
    }                                                              \
  }

namespace igl::vulkan {

Result getResultFromVkResult(VkResult result);
void setResultFrom(Result* outResult, VkResult result);
VkFormat textureFormatToVkFormat(igl::TextureFormat format);
igl::TextureFormat vkFormatToTextureFormat(VkFormat format);
igl::ColorSpace vkColorSpaceToColorSpace(VkColorSpaceKHR colorSpace);
VkMemoryPropertyFlags resourceStorageToVkMemoryPropertyFlags(igl::ResourceStorage resourceStorage);
VkCompareOp compareFunctionToVkCompareOp(igl::CompareFunction func);
VkSampleCountFlagBits getVulkanSampleCountFlags(size_t numSamples);
VkSurfaceFormatKHR colorSpaceToVkSurfaceFormat(igl::ColorSpace colorSpace, bool isBGR = false);

} // namespace igl::vulkan

#include <iostream>
#include <iomanip>
#include <VX/vx.h>

// 在OpenVX中，就概念上而言，所有的对象都是继承自 vx_reference，因此所有的对象都能转换为vx_reference
// 除 vx_context 之外，所有的对象都包含一个指向 vx_context 的reference

void get_properties(vx_context context)
{
    // 获取属性（VX_CONTEXT_VENDOR_ID）
    vx_uint16 vendor_id;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_VENDOR_ID, &vendor_id, sizeof(vx_uint16));
    std::cout << "vendor_id : " << vendor_id << std::endl;

    // 获取属性（VX_CONTEXT_VERSION）
    vx_uint16 version;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_VERSION, &version, sizeof(vx_uint16));
    std::cout << "version : " << version << std::endl;

    // 获取属性（VX_CONTEXT_UNIQUE_KERNELS），当前实现的 kernel 数量
    vx_uint32 unique_kernels;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_UNIQUE_KERNELS, &unique_kernels, sizeof(vx_uint32));
    std::cout << "unique_kernels : " << unique_kernels << std::endl;

    // 获取属性（VX_CONTEXT_MODULES）， 激活模块的数量
    vx_uint32 modules;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_MODULES, &modules, sizeof(vx_uint32));
    std::cout << "modules : " << modules << std::endl;

    // 获取属性（VX_CONTEXT_REFERENCES），激活的引用数量
    vx_uint32 references;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_REFERENCES, &references, sizeof(vx_uint32));
    std::cout << "references : " << references << std::endl;

    // 获取属性（VX_CONTEXT_IMPLEMENTATION），查询实现的名称，该名称的长度（包含结果的\0）不能超过 VX_MAX_IMPLEMENTATION_NAME
    vx_char implementation[VX_MAX_IMPLEMENTATION_NAME];
    vxQueryContext(context, (vx_enum)VX_CONTEXT_IMPLEMENTATION, &implementation, sizeof(vx_uint32));
    std::cout << "implementation : " << implementation << std::endl;

    // 获取属性（VX_CONTEXT_EXTENSIONS_SIZE），扩展字符串的长度，包含结尾的\0
    vx_size extension_size;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_EXTENSIONS_SIZE, &extension_size, sizeof(vx_size));
    std::cout << "extension_size : " << extension_size << std::endl;

    // 获取属性（VX_CONTEXT_EXTENSIONS）, 扩展字符串
    vx_char *extensions = new vx_char[extension_size];
    vxQueryContext(context, (vx_enum)VX_CONTEXT_EXTENSIONS, extensions, extension_size);
    for (size_t i = 0; i < extension_size; i++)
    {
        std::cout << "extensions : " << (int)extensions[i] << std::endl;
    }
    delete[] extensions;

    // 获取属性（VX_CONTEXT_CONVOLUTION_MAX_DIMENSION）， 卷积的最大尺寸
    vx_size convolution_max_dimension;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &convolution_max_dimension, sizeof(vx_size));
    std::cout << "convolution_max_dimension : " << convolution_max_dimension << std::endl;

    // 获取属性（VX_CONTEXT_OPTICAL_FLOW_MAX_WINDOW_DIMENSION），光流操作核 OpticalFlowPyrLK kernel 支持的最大窗口尺寸
    vx_size optical_flow_max_window_dimension;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_OPTICAL_FLOW_MAX_WINDOW_DIMENSION, &optical_flow_max_window_dimension, sizeof(vx_size));
    std::cout << "optical_flow_max_window_dimension : " << optical_flow_max_window_dimension << std::endl;

    // 获取属性（VX_CONTEXT_IMMEDIATE_BORDER），立即模式中的边界模式。图模式不受此参数的影响
    vx_border_t immediate_border;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_IMMEDIATE_BORDER, &immediate_border, sizeof(vx_border_t));
    std::cout << "immediate_border : " << immediate_border.constant_value.S32 << std::endl;
    std::cout << "immediate_border : " << immediate_border.mode << std::endl;

    // 获取属性（VX_CONTEXT_UNIQUE_KERNEL_TABLE）, 当前上下文中存在的所有 Kernel 组成的表
    vx_kernel_info_t *unique_kernel_table = new vx_kernel_info_t[unique_kernels];
    vxQueryContext(context, (vx_enum)VX_CONTEXT_UNIQUE_KERNEL_TABLE, &unique_kernel_table, sizeof(vx_kernel_info_t));
    for (int i = 0; i < unique_kernels; ++i) {
        std::cout << "unique_kernel_table - kernel enum : " << unique_kernel_table[i].enumeration << std::endl;
        std::cout << "unique_kernel_table - kernel name : " << unique_kernel_table[i].name << std::endl;
    }
    free(unique_kernel_table);

    // 获取属性（VX_CONTEXT_IMMEDIATE_BORDER_POLICY）, 立即模式中不支持的边界模式。图模式不受影响
    vx_border_policy_e immediate_border_policy;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_IMMEDIATE_BORDER_POLICY, &immediate_border_policy, sizeof(vx_border_policy_e));
    std::cout << "immediate_border_policy : " << immediate_border_policy << std::endl;

    // 获取属性（VX_CONTEXT_NONLINEAR_MAX_DIMENSION），非线性滤波支持的最大尺寸
    vx_size nonlinear_max_dimension;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_NONLINEAR_MAX_DIMENSION, &nonlinear_max_dimension, sizeof(vx_size));
    std::cout << "nonlinear_max_dimension : " << nonlinear_max_dimension << std::endl;

    // 获取属性（VX_CONTEXT_MAX_TENSOR_DIMS）
    vx_size max_tensor_dims;
    vxQueryContext(context, (vx_enum)VX_CONTEXT_MAX_TENSOR_DIMS, &max_tensor_dims, sizeof(vx_size));
    std::cout << "max_tensor_dims : " << max_tensor_dims << std::endl;
}

void set_properties(vx_context context)
{
    vx_status status = VX_SUCCESS;

    // 获取属性（VX_CONTEXT_IMMEDIATE_BORDER）
    vx_pixel_value_t pixel_value{.U8 = 128u};
    vx_border_t immediate_border{.mode = VX_BORDER_UNDEFINED, .constant_value = pixel_value};
    status = vxSetContextAttribute(context, (vx_enum)VX_CONTEXT_IMMEDIATE_BORDER, &immediate_border, sizeof(vx_border_t));
    std::cout << "status         : " << status << std::endl;
    std::cout << "mode           : " << immediate_border.mode << std::endl;
    std::cout << "constant_value : " << immediate_border.constant_value.S32 << std::endl;

    // 获取属性（VX_CONTEXT_IMMEDIATE_BORDER_POLICY）, 立即模式中不支持的边界模式。图模式不受影响
    vx_border_policy_e immediate_border_policy = VX_BORDER_POLICY_DEFAULT_TO_UNDEFINED;
    status = vxSetContextAttribute(context, (vx_enum)VX_CONTEXT_IMMEDIATE_BORDER_POLICY, &immediate_border_policy, sizeof(vx_border_policy_e));
    std::cout << "status                  : " << status << std::endl;
    std::cout << "immediate_border_policy : " << immediate_border_policy << std::endl;
}

int main(int argc, char *argv[])
{
    vx_context context = vxCreateContext();
    printf("The address of context is %p\n", context);

    get_properties(context);

    set_properties(context);

    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

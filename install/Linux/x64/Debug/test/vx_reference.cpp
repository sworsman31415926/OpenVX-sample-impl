#include <iostream>
#include <VX/vx.h>

// 在OpenVX中，就概念上而言，所有的对象都是继承自 vx_reference，因此所有的对象都能转换为vx_reference
// 除 vx_context 之外，所有的对象都包含一个指向 vx_context 的reference
// 这些对象包含一下类型
// Object: Array
// Object: Context
// Object: Convolution
// Object: Distribution
// Object: Graph
// Object: Image
// Object: LUT
// Object: Matrix
// Object: Node
// Object: ObjectArray
// Object: Tensor
// Object: Pyramid
// Object: Reference
// Object: Remap
// Object: Scalar
// Object: Threshold

int main(int argc, char *argv[]) {
    vx_context context = vxCreateContext();
    printf("The address of context is %p\n", context);

    // 创建引用
    vx_graph graph = vxCreateGraph(context);
    vx_reference graph_reference = (vx_reference) graph;

    // 获取绑定上下文
    printf("The address of context that got from reference is %p\n", vxGetContext(graph_reference));

    // 获取状态
    std::cout << "vxGetStatus : " << vxGetStatus(graph_reference) << std::endl;

    // 设置名称
    vxSetReferenceName(graph_reference, "graph_reference");

    // 获取属性之引用数量
    vx_uint32 reference_count;
    vxQueryReference(graph_reference, (vx_enum) VX_REFERENCE_COUNT, &reference_count, sizeof(vx_uint32));
    std::cout << "reference_count : " << reference_count << std::endl;

    // 获取属性之引用类型
    vx_type_e reference_type;
    vxQueryReference(graph_reference, (vx_enum) VX_REFERENCE_TYPE, &reference_type, sizeof(vx_type_e));
    std::cout << "reference_type : " << reference_type << std::endl;

    // 获取属性之引用名称，名称的长度（包含结尾的\0）不能超过 VX_MAX_REFERENCE_NAME，NULL表示引用是未命名的
    vx_char *reference_name;
    vxQueryReference(graph_reference, (vx_enum) VX_REFERENCE_NAME, &reference_name, sizeof(vx_char *));
    std::cout << "reference_name : " << reference_name << std::endl;

    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    for (size_t i = 0; i < 5; i++) {
        // 对参数指定的引用增加一次引用计数
        vxRetainReference(graph_reference);

        vx_uint32 count;
        vxQueryReference(graph_reference, VX_REFERENCE_COUNT, &count, sizeof(vx_uint32));
        std::cout << "count : " << count << std::endl;
    }

    std::cout << "--------------------------------------------------" << std::endl;
    for (size_t i = 0; i < 5; i++) {
        if (graph_reference != nullptr) {
            // 释放参数所表示的那个引用，且引用计数减一
            // 注意：vxRetainReference 与 vxReleaseReference 并非完全相反的功能
            //      vxRetainReference  调用一次该函数，将对参数指定的引用计数加一
            //      vxReleaseReference 仅释放参数所表示的那个引用，多次调用不起作用
            vxReleaseReference(&graph_reference);

            if (graph_reference != nullptr) {
                vx_uint32 count;
                vxQueryReference(graph_reference, VX_REFERENCE_COUNT, &count, sizeof(vx_uint32));
                std::cout << "count : " << count << std::endl;
            } else {
                std::cout << "graph reference is NULL (vxQueryReference)" << std::endl;
            }
        } else {
            std::cout << "graph reference is NULL (vxReleaseReference)" << std::endl;
        }
    }

    vxReleaseGraph(&graph);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

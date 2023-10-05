#include <VX/vx.h>
#include <stdio.h>

// 回调函数定义
vx_action VX_CALLBACK analyze_brightness(vx_node node) {
    // 获取标量参数的值
    vx_uint8 lmin = 0, lmax = 0;
    vx_uint32 minCount = 0, maxCount = 0;

    vx_scalar scalar;

    scalar = (vx_scalar)vxGetParameterByIndex(node, 0);
    vxCopyScalar(scalar, &lmin, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

    scalar = (vx_scalar)vxGetParameterByIndex(node, 1);
    vxCopyScalar(scalar, &lmax, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

    scalar = (vx_scalar)vxGetParameterByIndex(node, 2);
    vxCopyScalar(scalar, &minCount, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

    scalar = (vx_scalar)vxGetParameterByIndex(node, 3);
    vxCopyScalar(scalar, &maxCount, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

    // 打印结果
    printf("Min Value: %d, Max Value: %d, Min Count: %u, Max Count: %u\n", lmin, lmax, minCount, maxCount);

    return VX_ACTION_CONTINUE;
}

int main() {
    // 创建OpenVX上下文
    vx_context context = vxCreateContext();

    // 创建输入图像和图
    // vx_image input = vxCreateImage(context, /*图像格式参数*/);
    vx_image input = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);

    // 创建图
    vx_graph graph = vxCreateGraph(context);

    // 创建标量
    vx_uint8 lmin = 0, lmax = 0;
    vx_uint32 minCount = 0, maxCount = 0;
    vx_scalar scalars[] = {
        vxCreateScalar(context, VX_TYPE_UINT8, &lmin),
        vxCreateScalar(context, VX_TYPE_UINT8, &lmax),
        vxCreateScalar(context, VX_TYPE_UINT32, &minCount),
        vxCreateScalar(context, VX_TYPE_UINT32, &maxCount)
    };

    // 创建数组
    vx_array arrays[] = {
        vxCreateArray(context, VX_TYPE_COORDINATES2D, 1),
        vxCreateArray(context, VX_TYPE_COORDINATES2D, 1)
    };

    // 创建节点
    vx_node nodes[] = {
        vxMinMaxLocNode(graph, input, scalars[0], scalars[1], arrays[0], arrays[1], scalars[2], scalars[3])
        // 添加其他节点
    };

    // 分配回调函数给节点
    vxAssignNodeCallback(nodes[0], &analyze_brightness);

    // 执行图
    vxProcessGraph(graph);

    // 释放资源
    vxReleaseContext(&context);
    vxReleaseImage(&input);
    vxReleaseGraph(&graph);
    for (int i = 0; i < 4; i++) {
        vxReleaseScalar(&scalars[i]);
    }
    for (int i = 0; i < 2; i++) {
        vxReleaseArray(&arrays[i]);
    }

    return 0;
}
/*
 g++ -I include test/wanshan.cpp bin/libopenvx.so bin/libvxu.so -o test/wanshan
 ./test/wanshan
 export LD_LIBRARY_PATH=bin
 ./test/wanshan
Min Value: 0, Max Value: 0, Min Count: 0, Max Count: 0
*/
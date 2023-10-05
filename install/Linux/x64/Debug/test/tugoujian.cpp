#include <VX/vx.h>
#include <stdio.h>

int main() {
    vx_uint32 width = 1024, height = 760;

    // 创建OpenVX上下文和图
    vx_context context = vxCreateContext();
    vx_graph graph = vxCreateGraph(context);

    // 创建输入图像 img0 和输出图像 img1
    vx_image img0 = vxCreateImage(context, width, height, VX_DF_IMAGE_RGB);
    vx_image img1 = vxCreateImage(context, width, height, VX_DF_IMAGE_NV12);

    // 创建中间图像 img2
    vx_image img2 = vxCreateImage(context, width, height, VX_DF_IMAGE_U8);

    // 创建节点 n0：颜色转换
    vx_node n0 = vxColorConvertNode(graph, img0, img1);

    // 创建节点 n1：通道提取
    vx_node n1 = vxChannelExtractNode(graph, img1, 0, img2);

    // 验证图
    vx_status status = vxVerifyGraph(graph);
    if (status == VX_SUCCESS) {
        printf("Graph verification succeeded.\n");

        // 这里可以添加图像处理代码

        // 执行图
        status = vxProcessGraph(graph);
        if (status == VX_SUCCESS) {
            printf("Graph execution succeeded.\n");
        } else {
            printf("Error executing graph.\n");
        }
    } else {
        printf("Error verifying graph.\n");
    }

    // 释放资源
    vxReleaseContext(&context);
    vxReleaseGraph(&graph);
    vxReleaseImage(&img0);
    vxReleaseImage(&img1);
    vxReleaseImage(&img2);

    return 0;
}
/*
g++ -I include test/tugoujian.cpp bin/libopenvx.so bin/libvxu.so -o test/tugoujian
./test/tugoujian
Error verifying graph.

*/
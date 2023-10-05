#include <VX/vx.h>
#include <stdio.h>
int main()
{
    // 创建OpenVX上下文
    vx_context context = vxCreateContext();

    // 创建输入图像
    vx_image inputImage = vxCreateImage(context, 640, 480, VX_DF_IMAGE_U8);

    // 创建输出图像
    vx_image outputImage = vxCreateImage(context, 640, 480, VX_DF_IMAGE_U8);

    // 创建模糊卷积核
    vx_uint8 kernel[3][3] = {{1, 1, 1},
                             {1, 1, 1},
                             {1, 1, 1}};
    vx_convolution convolution = vxCreateConvolution(context, 3, 3);

    // 将卷积核设置为模糊核
    vxCopyConvolutionCoefficients(convolution, (vx_uint8 *)kernel, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    // 创建图
    vx_graph graph = vxCreateGraph(context);

    // 创建节点
    vx_node node1 = vxGaussian3x3Node(graph, inputImage, outputImage);
    vx_node node2 = vxConvolveNode(graph, inputImage, convolution, outputImage);

    // 链接节点
    vx_status status = vxVerifyGraph(graph);

    if (status == VX_SUCCESS)
    {
        // 执行图
        status = vxProcessGraph(graph);
        printf("Error verifying graph.\n");
    }
    printf("1 Error verifying graph.\n");
    // 释放资源
    vxReleaseContext(&context);
    vxReleaseImage(&inputImage);
    vxReleaseImage(&outputImage);
    vxReleaseConvolution(&convolution);
    vxReleaseGraph(&graph);

    return 0;
}
/*
g++ -I include test/pipline.cpp bin/libopenvx.so bin/libvxu.so -o test/pipline
./test/pipline
1 Error verifying graph.
*/

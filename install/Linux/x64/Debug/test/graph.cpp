#include <VX/vx.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// 自定义节点 F1，这里只是示例，你需要根据实际情况实现节点逻辑
vx_node vxF1Node(vx_graph graph, vx_image input, vx_image output) {
    return vxColorConvertNode(graph, input, output);
}

// 自定义节点 F2，这里只是示例，你需要根据实际情况实现节点逻辑
vx_node vxF2Node(vx_graph graph, vx_image input, vx_image output) {
    return vxChannelExtractNode(graph, input, 0, output);
}

int main() {
    // 创建OpenVX上下文
    vx_context context = vxCreateContext();

    // 创建输入和输出图像
    vx_uint32 width = 640;
    vx_uint32 height = 480;
    vx_image input = vxCreateImage(context, width, height, VX_DF_IMAGE_U8);
    vx_image output = vxCreateImage(context, width, height, VX_DF_IMAGE_U8);

    // 创建图
    vx_graph graph = vxCreateGraph(context);

    // 创建节点 F1
    vx_node f1Node = vxF1Node(graph, input, output);

    // 创建节点 F2
    vx_node f2Node = vxF2Node(graph, input, output);
    std::cout <<"00000" <<std::endl;
    // 验证图
    if (vxVerifyGraph(graph) == VX_SUCCESS) 
    {
        int numIterations = 10; // 设置迭代次数
        std::cout <<"11111" <<std::endl;
        for (int iteration = 0; iteration < numIterations; iteration++)
        {
            // 在这里写入输入图像数据
            // ...

            // 执行图
            vxProcessGraph(graph); // 或者使用 vxScheduleGraph(graph);

            // 在这里读取输出图像数据
            // ...
        }
    }

    // 释放资源
    vxReleaseContext(&context);
    vxReleaseImage(&input);
    vxReleaseImage(&output);
    vxReleaseGraph(&graph);

    return 0;
}
/*g++ -I include test/graph.cpp bin/libopenvx.so bin/libvxu.so -o test/graph
./test/graph
00000
*/

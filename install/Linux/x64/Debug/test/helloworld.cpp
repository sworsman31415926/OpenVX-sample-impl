#include <iostream>
#include <VX/vx.h>
// #include "vxF1Node.h"

// init
int main()
{
    vx_context context = vxCreateContext();//创建openvx上下文
    vx_graph graph = vxCreateGraph( context );//基于上下文创建图
    
    vx_image input = vxCreateImage( context, 640, 480, VX_DF_IMAGE_U8 );//基于上下文创建image变量
    vx_image output = vxCreateImage( context, 640, 480, VX_DF_IMAGE_U8 );
    vx_image intermediate = vxCreateVirtualImage( graph, 640, 480, VX_DF_IMAGE_U8 );//在图中创建虚拟image变量
    // vx_node F1 = vxF1Node( graph, input, intermediate );
    // vx_node F2 = vxF2Node( graph, intermediate, output );
    
    vxVerifyGraph( graph );//校验graph中的node是否正常
    vxProcessGraph( graph );//运行graph，这里会将刚才链接进来的所有Node的func执行一次
    std::cout <<"helloworld"<<std::endl;

    return 0;
}
/*
user@user-ThinkPad-T14-Gen-1:~/Downloads/openvx_second/OpenVX-sample-impl/install/Linux/x64/Debug/test/build$ ./helloworld 
helloworld
*/
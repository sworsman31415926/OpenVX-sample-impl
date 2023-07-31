#include <iostream>
#include <VX/vx.h>

int main()
{
    vx_uint32 width = 1024, height = 760;
    vx_context context = vxCreateContext();
    vx_graph graph = vxCreateGraph();
    vx_image img0 = vxCreateImage(context, width, height, VX_DF_IMAGE_RGB);
    vx_image img1 = vxCreateImage(context, width, height, VX_DF_IMAGE_NV12);
    vx_image img2 = vxCreateImage(context, width, height, VX_DF_IMAGE_U8 );

    vx_node n0 = vxColorConvertNode(graph, img0, img1);
    vx_node n1 = vxChannelExtractNode(graph, img1, 0, img2);

    vxVerifyGraph(graph);
    
    returb 0;
}
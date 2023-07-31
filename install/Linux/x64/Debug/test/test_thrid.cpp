#include <iostream>
#include <VX/vx.h>
// #include <NVX/nvx.h> // Include the VisionWorks header

int main()
{
    /* create graph */
    vx_context context = vxCreateContext();
    vx_image input = vxCreateImage(context, 640, 480, VX_DF_IMAGE_U8);
    vx_image output = vxCreateImage(context, 640, 480, VX_DF_IMAGE_U8);
    vx_graph graph = vxCreateGraph(context);
    vx_image intermediate = vxCreateVirtualImage(graph, 640, 480, VX_DF_IMAGE_U8);
    vx_node F1 = vxF1Node(graph, input, intermediate);
    vx_node F2 = vxF2Node(graph, intermediate, output);

    /* verify graph */
    if (vxVerifyGraph(graph) == VX_SUCCESS)
    {
        while (/* ... condition for loop ... */)
        {
            // ... write to input image ...

            /* execute graph */
            vxProcessGraph(graph); // or vxScheduleGraph(graph);

            // ... read from output image ...
        }
    }

    /* release resources */
    vxReleaseNode(&F2);
    vxReleaseNode(&F1);
    vxReleaseImage(&intermediate);
    vxReleaseGraph(&graph);
    vxReleaseImage(&output);
    vxReleaseImage(&input);
    vxReleaseContext(&context);

    return 0;
}

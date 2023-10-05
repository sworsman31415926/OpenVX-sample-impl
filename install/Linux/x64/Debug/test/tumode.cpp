#include <stdio.h>
#include <stdlib.h>
#include "VX/vx.h"

void errorCheck(vx_context *context_p, vx_status status, const char *message)
{
    if (status)
    {
      puts("ERROR! ");
      puts(message);
      vxReleaseContext(context_p);
      exit(1);
    }
}

vx_image makeInputImage(vx_context context, vx_uint32 width, vx_uint32 height)
{
  vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  if (width > 48)
      width = 48;
  if (height > 48)
      height = 48;
  vx_rectangle_t rect = {
    .start_x = 50 - width, .start_y = 50 - height, .end_x = 50 + width, .end_y = 50 + height
  };

  if (VX_SUCCESS == vxGetStatus((vx_reference)image))
  {
    vx_image roi = vxCreateImageFromROI(image, &rect);
    vx_pixel_value_t pixel_white, pixel_black;
    pixel_white.U8 = 255;
    pixel_black.U8 = 0;
    if (VX_SUCCESS == vxGetStatus((vx_reference)roi) &&
        VX_SUCCESS == vxSetImagePixelValues(image, &pixel_black) &&
        VX_SUCCESS == vxSetImagePixelValues(roi, &pixel_white))
      vxReleaseImage(&roi);
    else
      vxReleaseImage(&image);
  }
  return image;
}

vx_graph makeTestGraph(vx_context context)
{
    vx_graph graph = vxCreateGraph(context);
    int i;
    vx_image imagesU8[5], imagesS16[3];
    vx_image input = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);

	//节点所需的虚拟图像对象
    for (i = 0; i < 5; ++i)
      imagesU8[i] = vxCreateVirtualImage(graph, 100, 100, VX_DF_IMAGE_U8);
    for (i = 0; i < 3; ++i)
        imagesS16[i] = vxCreateVirtualImage(graph, 0, 0, VX_DF_IMAGE_VIRT);
	//所需参数创建
    vx_matrix warp_matrix = vxCreateMatrix(context, VX_TYPE_FLOAT32, 2U, 3U);
    vx_float32 matrix_values[6] = {0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };       /* Rotate through 90 degrees */
    vx_float32 strength_thresh_value = 128.0;
    vx_scalar strength_thresh = vxCreateScalar(context, VX_TYPE_FLOAT32, &strength_thresh_value);
    vx_array corners = vxCreateArray(context, VX_TYPE_KEYPOINT, 100);
    vx_size num_corners_value = 0;
    vx_int32 shift_value = 1;
    vx_scalar num_corners = vxCreateScalar(context, VX_TYPE_SIZE, &num_corners_value);
    vx_scalar shift = vxCreateScalar(context, VX_TYPE_INT32, &shift_value);

    vxCopyMatrix(warp_matrix, matrix_values, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
	//节点连接：节点的连接方式是以数据为支撑
    /* Create the nodes to do the processing, order of creation is not important */
    vx_node last_node = vxFastCornersNode(graph, imagesU8[4], strength_thresh, vx_true_e, corners, num_corners);
    vxDilate3x3Node(graph, imagesU8[3], imagesU8[4]);
    vxConvertDepthNode(graph, imagesS16[2], imagesU8[3], VX_CONVERT_POLICY_SATURATE, shift);
    vxMagnitudeNode(graph, imagesS16[0], imagesS16[1], imagesS16[2]);
    vxSobel3x3Node(graph, imagesU8[2], imagesS16[0], imagesS16[1]);
    vxOrNode(graph, imagesU8[0], imagesU8[1], imagesU8[2]);
    vxWarpAffineNode(graph, imagesU8[0], warp_matrix, VX_INTERPOLATION_NEAREST_NEIGHBOR, imagesU8[1]);

	//设置grpah 输入，Index:节点的第几个参数
    /* Setup input parameter using a Copy node */
    vxAddParameterToGraph(graph, vxGetParameterByIndex(vxCopyNode(graph, (vx_reference)input, (vx_reference)imagesU8[0]), 0));
	//设置grpah 输出，Index:节点的第几个参数
    /* Setup the output parameters from the last node */
    vxAddParameterToGraph(graph, vxGetParameterByIndex(last_node, 3));    /* array of corners */
    vxAddParameterToGraph(graph, vxGetParameterByIndex(last_node, 4));    /* number of corners */

    /* Release resources */
    vxReleaseImage(&input);
    for (i = 0; i < 5; ++i)
        vxReleaseImage(&imagesU8[i]);
    for (i = 0; i < 3; ++i)
        vxReleaseImage(&imagesS16[i]);
    vxReleaseMatrix(&warp_matrix);
    vxReleaseScalar(&strength_thresh);
    vxReleaseScalar(&num_corners);
    vxReleaseScalar(&shift);
    vxReleaseArray(&corners);

    return graph;
}
//获取Graph的参数
vx_reference getGraphParameter(vx_graph graph, vx_uint32 index)
{
    vx_parameter p = vxGetGraphParameterByIndex(graph, index);
    vx_reference ref = NULL;
    vxQueryParameter(p, VX_PARAMETER_REF, &ref, sizeof(ref));
    vxReleaseParameter(&p);
    return ref;
}

void showResults(vx_graph graph, vx_image image, const char * message)
{
	vx_status status;
	//创建上下文
    vx_context context = vxGetContext((vx_reference)graph);
    puts(message);
	//设置Graph的输出参数，Index:0 第0个参数
    vxSetGraphParameterByIndex(graph, 0, (vx_reference)image);
	//graph 认证
	errorCheck(&context,vxVerifyGraph(graph),"Graph verify fail!");
	//graph 执行
	status = vxProcessGraph(graph);	
    if (VX_SUCCESS == status)
    {
        vx_size num_corners_value = 0;
        vx_keypoint_t *kp = (vx_keypoint_t*)calloc( 100, sizeof(vx_keypoint_t));
		//执行成功后，将scaler参数拷贝值num_corners_value
        errorCheck(&context, vxCopyScalar((vx_scalar)getGraphParameter(graph, 2), &num_corners_value,
                                          VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyScalar failed");
        printf("Found %zu corners with non-max suppression\n", num_corners_value);

        /* Array can only hold 100 values */
        if (num_corners_value > 100)
            num_corners_value = 100;
		//执行成功后，将scaler参数拷贝值vx_keypoint_t 数组
        errorCheck(&context, vxCopyArrayRange((vx_array)getGraphParameter(graph, 1), 0,
                                              num_corners_value, sizeof(vx_keypoint_t), kp,
                                              VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyArrayRange failed");
        for (int i=0; i<num_corners_value; ++i)
        {
            printf("Entry %3d: x = %d, y = %d\n", i, kp[i].x, kp[i].y);
        }

        free(kp);
    }
    else
    {
        printf("Graph processing failed[%d]!\n",status);
    }
}

int main(void)
{
    vx_context context = vxCreateContext();
    errorCheck(&context, vxGetStatus((vx_reference)context), "Could not create a vx_context\n");

    vx_graph graph = makeTestGraph(context);

    vx_image image1 = makeInputImage(context, 30, 10);
    vx_image image2 = makeInputImage(context, 25, 25);

    showResults(graph, image1, "Results for Image 1");
    showResults(graph, image2, "Results for Image 2");

    vxReleaseContext(&context);
    return 0;
}

/*
g++ -I include test/tumode.cpp bin/libopenvx.so bin/libvxu.so -o test/tumode
tugoujian  tumode
h./test/tumode
Results for Image 1
Found 24 corners with non-max suppression
Entry   0: x = 36, y = 18
Entry   1: x = 59, y = 18
Entry   2: x = 40, y = 22
Entry   3: x = 55, y = 22
Entry   4: x = 35, y = 35
Entry   5: x = 62, y = 35
Entry   6: x = 16, y = 38
Entry   7: x = 79, y = 38
Entry   8: x = 38, y = 39
Entry   9: x = 58, y = 39
Entry  10: x = 20, y = 42
Entry  11: x = 75, y = 42
Entry  12: x = 22, y = 55
Entry  13: x = 75, y = 55
Entry  14: x = 39, y = 58
Entry  15: x = 56, y = 58
Entry  16: x = 18, y = 59
Entry  17: x = 78, y = 59
Entry  18: x = 35, y = 62
Entry  19: x = 60, y = 62
Entry  20: x = 42, y = 75
Entry  21: x = 55, y = 75
Entry  22: x = 38, y = 79
Entry  23: x = 58, y = 79
Results for Image 2
Found 8 corners with non-max suppression
Entry   0: x = 21, y = 23
Entry   1: x = 74, y = 23
Entry   2: x = 25, y = 27
Entry   3: x = 70, y = 27
Entry   4: x = 27, y = 70
Entry   5: x = 70, y = 70
Entry   6: x = 23, y = 74
Entry   7: x = 73, y = 74

*/

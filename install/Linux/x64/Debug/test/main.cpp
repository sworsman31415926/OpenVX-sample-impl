#include <stdio.h>
#include <stdlib.h>
#include "VX/vx.h"
#include "VX/vxu.h"
#include "openvx_utils.h"
 
void errorCheck(vx_context context, vx_status status, const char *message)
{
    if (status)
    {
        puts("ERROR! ");
        puts(message);
        vxReleaseContext(&context);
        exit(EXIT_FAILURE);
    }
}
 
vx_reference getGraphParameter(vx_graph graph, vx_uint32 index)
{
    vx_parameter p = vxGetGraphParameterByIndex(graph, index);
    vx_reference ref = NULL;
    vxQueryParameter(p, VX_PARAMETER_REF, &ref, sizeof(ref));
    vxReleaseParameter(&p);
    return ref;
}
 
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
 
    vx_context context = vxCreateContext();
 
    // 创建图
    vx_graph graph;
    {
        graph = vxCreateGraph(context);
        vx_image input_image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
 
        vx_float32 strength_thresh_value = 128.0f;
        vx_scalar strength_thresh = vxCreateScalar(context, VX_TYPE_FLOAT32, &strength_thresh_value);
 
        vx_array corners = vxCreateArray(context, VX_TYPE_KEYPOINT, 100);
 
        vx_size num_corners_value = 0;
        vx_scalar num_corners = vxCreateScalar(context, VX_TYPE_SIZE, &num_corners_value);
 
        vx_node node = vxFastCornersNode(graph, input_image, strength_thresh, vx_true_e, corners, num_corners);
 
        // 创建图参数
        vxAddParameterToGraph(graph, vxGetParameterByIndex(node, 0));
        vxAddParameterToGraph(graph, vxGetParameterByIndex(node, 3));
        vxAddParameterToGraph(graph, vxGetParameterByIndex(node, 4));
 
        vxReleaseImage(&input_image);
        vxReleaseArray(&corners);
        vxReleaseScalar(&num_corners);
        vxReleaseScalar(&strength_thresh);
    }
 
    // 图校验
    {
        errorCheck(context, vxVerifyGraph(graph), "Verify graph is fail.");
 
        vx_uint32 num_params;
        vxQueryGraph(graph, VX_GRAPH_NUMPARAMETERS, &num_params, sizeof(vx_uint32));
        std::cout << "num_params: " << num_params << std::endl;
 
        vx_graph_state_e graph_state;
        vxQueryGraph(graph, VX_GRAPH_STATE, &graph_state, sizeof(vx_graph_state_e));
        std::cout << "graph_state: " << graph_state << std::endl;
    }
 
    // 设置输入参数——图像
    vx_image image;
    {
        // 创建输入图像
        {
            image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
            errorCheck(context, vxGetStatus((vx_reference)image), "Create image is fail.");
 
            vx_rectangle_t rect = {.start_x = 20, .start_y = 40, .end_x = 80, .end_y = 60};
            vx_image roi = vxCreateImageFromROI(image, &rect);
            errorCheck(context, vxGetStatus((vx_reference)roi), "Create roi is fail.");
 
            vx_pixel_value_t pixel_white, pixel_black;
            pixel_white.U8 = 255;
            pixel_black.U8 = 0;
 
            errorCheck(context, vxSetImagePixelValues(image, &pixel_black), "Set pixel value is fail.");
            errorCheck(context, vxSetImagePixelValues(roi, &pixel_white), "Set pixel value is fail.");
        }
 
        vx_status status = vxSetGraphParameterByIndex(graph, 0, (vx_reference)image);
        if (status != VX_SUCCESS)
        {
            if (status == VX_ERROR_INVALID_REFERENCE)
                std::cout << "VX_ERROR_INVALID_REFERENCE" << std::endl;
 
            if (status == VX_ERROR_INVALID_PARAMETERS)
                std::cout << "VX_ERROR_INVALID_PARAMETERS" << std::endl;
 
            printf("status = %d\n", status);
            vxReleaseContext(&context);
            return EXIT_FAILURE;
        }
    }
 
    // 图执行
    errorCheck(context, vxProcessGraph(graph), "Process graph is fail.");
 
    // 获取图输出
    {
        vx_size num_corners_value = 0;
        vxCopyScalar((vx_scalar)getGraphParameter(graph, 2), &num_corners_value, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
        std::cout << "num_corners_value: " << num_corners_value << std::endl;
        
        vx_keypoint_t *kp = (vx_keypoint_t *)calloc(100, sizeof(vx_keypoint_t));
        vxCopyArrayRange((vx_array)getGraphParameter(graph, 1), 0, num_corners_value, sizeof(vx_keypoint_t), kp, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
 
        cv::Mat cv_image;
        int ret = vxVxImageToCvMat(image, cv_image);
        if (0 == ret)
        {
            cv::Mat cv_image_color;
            cv::cvtColor(cv_image, cv_image_color, cv::COLOR_GRAY2BGR);
            for (int i = 0; i < num_corners_value; ++i)
            {
                cv::circle(cv_image_color, cv::Point(kp[i].x, kp[i].y), 3, cv::Scalar(0, 255, 0), -1);
            }
            DisplayImage(cv_image_color, "KeyPoints");
        }
 
        free(kp);
    }
 
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

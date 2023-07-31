#include <iostream>
#include <iomanip>
#include <VX/vx.h>

vx_reference getGraphParameter(vx_graph graph, vx_uint32 index)
{
    vx_parameter p = vxGetGraphParameterByIndex(graph, index);
    vx_reference ref = nullptr;
    vxQueryParameter(p, VX_PARAMETER_REF, &ref, sizeof(vx_reference));
    vxReleaseParameter(&p);
    return ref;
}

vx_image make_image(vx_context context, vx_int32 half_w, vx_int32 half_h)
{
    vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);

    vx_rectangle_t rect = {.start_x = 50U - half_w, .start_y = 50U - half_h, .end_x = 0U + half_w, .end_y = 50U + half_h};
    vx_image roi = vxCreateImageFromROI(image, &rect);

    vx_pixel_value_t pixel_white, pixel_black;
    pixel_white.U8 = 255;
    pixel_black.U8 = 0;

    vxSetImagePixelValues(roi, &pixel_white);

    return image;
}

int main(int argc, char *argv[]) {
    vx_status status = VX_SUCCESS;

    vx_context context = vxCreateContext();

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

    // 注册延迟对象
    vx_delay delay;
    {
        vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
        delay = vxCreateDelay(context, (vx_reference)image, 3);
        vxRegisterAutoAging(graph, delay);
        vxReleaseImage(&image);
    }

    // 判断图是否被验证
    std::cout << "vxIsGraphVerified : " << vxIsGraphVerified(graph) << std::endl;

    // 图验证
    status = vxVerifyGraph(graph);
    std::cout << "vxVerifyGraph : " << status << std::endl;

    // 判断图是否被验证
    std::cout << "vxIsGraphVerified : " << vxIsGraphVerified(graph) << std::endl;

    // 查询图参数
    {
        vx_uint32 num_modules;
        vxQueryGraph(graph, VX_GRAPH_NUMNODES, &num_modules, sizeof(vx_uint32));
        std::cout << "num_modules: " << num_modules << std::endl;

        vx_perf_t performance;
        vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &performance, sizeof(vx_uint32));
        std::cout << "performance tmp: " << performance.tmp << std::endl;
        std::cout << "performance beg: " << performance.beg << std::endl;
        std::cout << "performance end: " << performance.end << std::endl;
        std::cout << "performance sum: " << performance.sum << std::endl;
        std::cout << "performance avg: " << performance.avg << std::endl;
        std::cout << "performance min: " << performance.min << std::endl;
        std::cout << "performance num: " << performance.num << std::endl;
        std::cout << "performance max: " << performance.max << std::endl;

        vx_uint32 num_params;
        vxQueryGraph(graph, VX_GRAPH_NUMPARAMETERS, &num_params, sizeof(vx_uint32));
        std::cout << "num_params: " << num_params << std::endl;

        vx_graph_state_e graph_state;
        vxQueryGraph(graph, VX_GRAPH_STATE, &graph_state, sizeof(vx_graph_state_e));
        std::cout << "graph_state: " << graph_state << std::endl;
    }

    // 设置输入参数——图像
    vx_image image = make_image(context, 20, 10);
    status = vxSetGraphParameterByIndex(graph, 0, (vx_reference) image);
    std::cout << "vxSetGraphParameterByIndex : " << status << std::endl;

    // 图执行
    {
        // 同步
        status = vxProcessGraph(graph);
        std::cout << "vxProcessGraph : " << status << std::endl;

        // 异步
        status = vxScheduleGraph(graph);
        std::cout << "vxScheduleGraph : " << status << std::endl;

        status = vxWaitGraph(graph);
        std::cout << "vxWaitGraph : " << status << std::endl;
    }


    // 获取图输出
    {
        vx_size num_corners_value = 0;
        vxCopyScalar((vx_scalar)getGraphParameter(graph, 2), &num_corners_value, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
        std::cout << "num_corners_value: " << num_corners_value << std::endl;

        vx_keypoint_t *kp = (vx_keypoint_t *)calloc(100, sizeof(vx_keypoint_t));
        vxCopyArrayRange((vx_array)getGraphParameter(graph, 1), 0, num_corners_value, sizeof(vx_keypoint_t), kp, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

        for (int i = 0; i < num_corners_value; ++i) {
            std::cout << "index                     : " << i << std::endl;
            std::cout << "\t" << "error             : " << kp[i].error << std::endl;
            std::cout << "\t" << "orientation       : " << kp[i].orientation << std::endl;
            std::cout << "\t" << "scale             : " << kp[i].scale << std::endl;
            std::cout << "\t" << "strength          : " << kp[i].strength << std::endl;
            std::cout << "\t" << "tracking_status   : " << kp[i].tracking_status << std::endl;
            std::cout << "\t" << "x                 : " << kp[i].x << std::endl;
            std::cout << "\t" << "y                 : " << kp[i].y << std::endl;
        }

        free(kp);
    }

    vxReleaseGraph(&graph);
    vxReleaseDelay(&delay);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

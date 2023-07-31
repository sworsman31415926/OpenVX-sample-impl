#include <iostream>
#include <iomanip>
#include <VX/vx.h>

vx_image make_image(vx_context context, vx_int32 half_w, vx_int32 half_h)
{
    std::cout << "make_image start" << std::endl;
    vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);

    vx_rectangle_t rect = {.start_x = 50U - half_w, .start_y = 50U - half_h, .end_x = 0U + half_w, .end_y = 50U + half_h};
    std::cout << "make_image rect.start_x:" << rect.start_x << std::endl;
    std::cout << "make_image rect.start_y:" << rect.start_y << std::endl;
    std::cout << "make_image rect.end_x:" << rect.end_x << std::endl;
    std::cout << "make_image rect.end_y:" << rect.end_y << std::endl;
    vx_image roi = vxCreateImageFromROI(image, &rect);

    vx_pixel_value_t pixel_white, pixel_black;
    pixel_white.U8 = 255;
    pixel_black.U8 = 0;

    vxSetImagePixelValues(roi, &pixel_white);

    return image;
}

int main(int argc, char *argv[])
{
    vx_status status = VX_SUCCESS;

    vx_context context = vxCreateContext();
    vx_graph graph = vxCreateGraph(context);

    vx_image input_image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);

    vx_float32 strength_thresh_value = 128.0f;
    vx_scalar strength_thresh = vxCreateScalar(context, VX_TYPE_FLOAT32, &strength_thresh_value);

    vx_array corners = vxCreateArray(context, VX_TYPE_KEYPOINT, 100);

    vx_size num_corners_value = 0;
    vx_scalar num_corners = vxCreateScalar(context, VX_TYPE_SIZE, &num_corners_value);

    vx_node node = vxFastCornersNode(graph, input_image, strength_thresh, vx_true_e, corners, num_corners);
    make_image(context, 1, 1);

    // 查询属性
    {
        vx_status node_state;
        vxQueryNode(node, (vx_enum)VX_NODE_STATUS, &node_state, sizeof(node_state));

        vx_directive_e performance;
        vxQueryNode(node, (vx_enum)VX_NODE_PERFORMANCE, &performance, sizeof(performance));

        vx_border_t border_mode;
        vxQueryNode(node, (vx_enum)VX_NODE_BORDER, &border_mode, sizeof(border_mode));

        // 指示kernel中局部内存的大小
        vx_size data_size;
        vxQueryNode(node, (vx_enum)VX_NODE_LOCAL_DATA_SIZE, &data_size, sizeof(data_size));

        // 指示kernel中局部内存的地址
        void *data_ptr;
        vxQueryNode(node, (vx_enum)VX_NODE_LOCAL_DATA_PTR, &data_ptr, sizeof(data_ptr));

        vx_uint32 num_params;
        vxQueryNode(node, (vx_enum)VX_NODE_PARAMETERS, &num_params, sizeof(num_params));

        vx_bool replicated;
        vxQueryNode(node, (vx_enum)VX_NODE_IS_REPLICATED, &replicated, sizeof(replicated));

        vx_bool *replicated_flags;
        vxQueryNode(node, (vx_enum)VX_NODE_REPLICATE_FLAGS, &replicated_flags, sizeof(replicated_flags));

        vx_bool rect_reset;
        vxQueryNode(node, (vx_enum)VX_NODE_VALID_RECT_RESET, &rect_reset, sizeof(rect_reset));
    }

    // 设置属性
    {
        vx_pixel_value_t pixel_value{.U8 = 128U};
        vx_border_t border_mode{.mode = VX_BORDER_CONSTANT, .constant_value = pixel_value};
        vxSetNodeAttribute(node, (vx_enum)VX_NODE_BORDER, &border_mode, sizeof(border_mode));
    }

    // 复制节点，用于处理存储在 vx_pyramid 或者 vx_object_array 中的对象集合
    // 指定节点 first_node 需要具有能够处理 vx_pyramid 第0层的参数或者 vx_object_array 中索引为 0 的对象的参数。
    // 复制的节点不能通过任何方式进行访问。
    // 当移除 first_node 时，所有的复制节点都被移除
    // first_node 节点的参数或属性发生变化，复制节点同步变化。
    // vxVerifyGraph 强制所有的复制节点具有相同的参数
    {
        vx_bool replicate[5]{vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e};
        vxReplicateNode(graph, node, replicate, 5);
    }

    // 设置节点目标。运行成功后，使得节点所属的图失效。目前不知如何使用
    // vx_status vxSetNodeTarget(vx_node node, vx_enum target_enum, const char *target_string);

    // 从 graph 中移除节点，同时释放节点
    vxRemoveNode(&node);

    if (node != NULL)
        vxReleaseNode(&node);
    vxReleaseImage(&input_image);
    vxReleaseArray(&corners);
    vxReleaseScalar(&num_corners);
    vxReleaseScalar(&strength_thresh);

    vxReleaseGraph(&graph);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}
/*
make_image start
make_image rect.start_x:49
make_image rect.start_y:49
make_image rect.end_x:1
make_image rect.end_y:51
*/

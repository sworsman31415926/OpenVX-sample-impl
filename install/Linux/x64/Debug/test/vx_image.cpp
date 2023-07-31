#include <iostream>
#include <VX/vx.h>


vx_df_image_e df_images[]{
        VX_DF_IMAGE_VIRT,
        VX_DF_IMAGE_RGB,
        VX_DF_IMAGE_RGBX,
        VX_DF_IMAGE_NV12,
        VX_DF_IMAGE_NV21,
        VX_DF_IMAGE_UYVY,
        VX_DF_IMAGE_YUYV,
        VX_DF_IMAGE_IYUV,
        VX_DF_IMAGE_YUV4,
        VX_DF_IMAGE_U8,
        VX_DF_IMAGE_U16,
        VX_DF_IMAGE_S16,
        VX_DF_IMAGE_U32,
        VX_DF_IMAGE_S32,
        VX_DF_IMAGE_U1};

std::string get_string_vx_df_image_e(vx_df_image_e format) {
    switch (format) {
        case VX_DF_IMAGE_VIRT:
            return std::string("VX_DF_IMAGE_VIRT");
        case VX_DF_IMAGE_RGB:
            return std::string("VX_DF_IMAGE_RGB");
        case VX_DF_IMAGE_RGBX:
            return std::string("VX_DF_IMAGE_RGBX");
        case VX_DF_IMAGE_NV12:
            return std::string("VX_DF_IMAGE_NV12");
        case VX_DF_IMAGE_NV21:
            return std::string("VX_DF_IMAGE_NV21");
        case VX_DF_IMAGE_UYVY:
            return std::string("VX_DF_IMAGE_UYVY");
        case VX_DF_IMAGE_YUYV:
            return std::string("VX_DF_IMAGE_YUYV");
        case VX_DF_IMAGE_IYUV:
            return std::string("VX_DF_IMAGE_IYUV");
        case VX_DF_IMAGE_U8:
            return std::string("VX_DF_IMAGE_U8");
        case VX_DF_IMAGE_U16:
            return std::string("VX_DF_IMAGE_U16");
        case VX_DF_IMAGE_S16:
            return std::string("VX_DF_IMAGE_S16");
        case VX_DF_IMAGE_U32:
            return std::string("VX_DF_IMAGE_U32");
        case VX_DF_IMAGE_S32:
            return std::string("VX_DF_IMAGE_S32");
        case VX_DF_IMAGE_YUV4:
            return std::string("VX_DF_IMAGE_YUV4");
        case VX_DF_IMAGE_U1:
            return std::string("VX_DF_IMAGE_U1");
    }

    return std::string("");
}

std::string get_string_vx_color_space_e(vx_color_space_e color_space) {
    switch (color_space) {
        case VX_COLOR_SPACE_NONE:
            return std::string("VX_COLOR_SPACE_NONE");
        case VX_COLOR_SPACE_BT601_525:
            return std::string("VX_COLOR_SPACE_BT601_525");
        case VX_COLOR_SPACE_BT601_625:
            return std::string("VX_COLOR_SPACE_BT601_625");
        case VX_COLOR_SPACE_BT709:
            return std::string("VX_COLOR_SPACE_BT709");
    }
    return std::string("");
}

std::string get_string_vx_channel_range_e(vx_channel_range_e channel_range) {
    switch (channel_range) {
        case VX_CHANNEL_RANGE_FULL:
            return std::string("VX_CHANNEL_RANGE_FULL");
        case VX_CHANNEL_RANGE_RESTRICTED:
            return std::string("VX_CHANNEL_RANGE_RESTRICTED");
    }
    return std::string("");
}

std::string get_string_vx_memory_type_e(vx_memory_type_e memory_type) {
    switch (memory_type) {
        case VX_MEMORY_TYPE_NONE:
            return std::string("VX_MEMORY_TYPE_NONE");
        case VX_MEMORY_TYPE_HOST:
            return std::string("VX_MEMORY_TYPE_HOST");
    }
    return std::string("");
}

void print_image(vx_image image, const char *message) {
    vx_status status;

    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;

    vx_uint32 img_width;
    vxQueryImage(image, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));

    vx_uint32 img_height;
    vxQueryImage(image, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

    vx_df_image_e format;
    vxQueryImage(image, VX_IMAGE_FORMAT, &format, sizeof(vx_df_image));

    vx_size planes;
    vxQueryImage(image, VX_IMAGE_PLANES, &planes, sizeof(vx_size));

    vx_color_space_e color_space;
    vxQueryImage(image, VX_IMAGE_SPACE, &color_space, sizeof(vx_color_space_e));

    vx_channel_range_e channel_range;
    vxQueryImage(image, VX_IMAGE_RANGE, &channel_range, sizeof(vx_channel_range_e));

    vx_memory_type_e memory_type;
    vxQueryImage(image, VX_IMAGE_MEMORY_TYPE, &memory_type, sizeof(vx_memory_type_e));

    vx_bool is_uniform;
    vx_pixel_value_t uniform_value;
    status = vxQueryImage(image, VX_IMAGE_IS_UNIFORM, &is_uniform, sizeof(is_uniform));
    if (status != VX_SUCCESS)
        is_uniform = false, uniform_value.S32 = -32768;

    if (is_uniform == vx_true_e) {
        status = vxQueryImage(image, VX_IMAGE_UNIFORM_VALUE, &uniform_value, sizeof(uniform_value));
        if (status != VX_SUCCESS)
            uniform_value.S32 = -32768;
    }

    std::cout << "\t" << "format        : " << get_string_vx_df_image_e(format) << std::endl;
    std::cout << "\t" << "img_width     : " << img_width << std::endl;
    std::cout << "\t" << "img_height    : " << img_height << std::endl;
    std::cout << "\t" << "planes        : " << planes << std::endl;
    std::cout << "\t" << "color_space   : " << get_string_vx_color_space_e(color_space) << std::endl;
    std::cout << "\t" << "channel_range : " << get_string_vx_channel_range_e(channel_range) << std::endl;
    std::cout << "\t" << "memory_type   : " << get_string_vx_memory_type_e(memory_type) << std::endl;
    std::cout << "\t" << "is_uniform    : " << is_uniform << std::endl;
    if (is_uniform)
        std::cout << "\t" << "uniform_value : " << uniform_value.S32 << std::endl;

    std::cout << "\t" << "valid region  : " << std::endl;
    vx_rectangle_t patch;
    vxGetValidRegionImage(image, &patch);
    std::cout << "\t" << "\t" << "start_x   : " << patch.start_x << std::endl;
    std::cout << "\t" << "\t" << "start_y   : " << patch.start_y << std::endl;
    std::cout << "\t" << "\t" << "end_x     : " << patch.end_x << std::endl;
    std::cout << "\t" << "\t" << "end_y     : " << patch.end_y << std::endl;

    std::cout << "\t" << "image patch addressing   : " << std::endl;
    for (size_t i = 0; i < planes; i++) {
        vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;

        vx_map_id map_id;
        unsigned char *ptr;
        vxMapImagePatch(image, &patch, i, &map_id, &addr, (void **) &ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST,
                        VX_NOGAP_X);
        std::cout << "\t" << "\t" << "plane        : " << i << std::endl;
        std::cout << "\t" << "\t" << "addr.dim_x   : " << addr.dim_x << std::endl;
        std::cout << "\t" << "\t" << "addr.dim_y   : " << addr.dim_y << std::endl;
        std::cout << "\t" << "\t" << "addr.stride_x: " << addr.stride_x << std::endl;
        std::cout << "\t" << "\t" << "addr.stride_y: " << addr.stride_y << std::endl;
        std::cout << "\t" << "\t" << "addr.scale_x : " << addr.scale_x << std::endl;
        std::cout << "\t" << "\t" << "addr.scale_y : " << addr.scale_y << std::endl;
        std::cout << "\t" << "\t" << "addr.step_x  : " << addr.step_x << std::endl;
        std::cout << "\t" << "\t" << "addr.step_y  : " << addr.step_y << std::endl;
        std::cout << std::endl;
        vxUnmapImagePatch(image, map_id);
    }
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    vx_status status;
    vx_context context = vxCreateContext();

//    for (auto &df_image : df_images) {
//        vx_image image_created = vxCreateImage(context, 400, 300, df_image);
//        print_image(image_created, "created");
//        vxReleaseImage(&image_created);
//    }

    // 创建图像
    vx_image image_created = vxCreateImage(context, 400, 300, VX_DF_IMAGE_S32);
    print_image(image_created, "created");

    // 设置图像属性
    {
        vx_color_space_e color_space = VX_COLOR_SPACE_DEFAULT;
        vxSetImageAttribute(image_created, (vx_enum) VX_IMAGE_SPACE, &color_space, sizeof(color_space));
        print_image(image_created, "update attributes");
    }

    // 设置像素值
    {
        vx_pixel_value_t pixel_value{.S32=-32768};
        vxSetImagePixelValues(image_created, &pixel_value);
        print_image(image_created, "set pixel value");
    }

    // 设置合法区域
    {
        vx_rectangle_t valid_rect{.start_x=50, .start_y=50, .end_x=250, .end_y=250};
        vxSetImageValidRectangle(image_created, &valid_rect);
        print_image(image_created, "set valid rect");
    }

    // 由通道数据创建图像
    {
        vx_image image_created_from_channel = vxCreateImageFromChannel(image_created, 0);
        print_image(image_created_from_channel, "image_created_from_channel");
        vxReleaseImage(&image_created_from_channel);
    }

    // 由ROI创建图像
    {
        vx_rectangle_t roi{.start_x=100, .start_y=100, .end_x=200, .end_y=200};
        vx_image image_created_from_roi = vxCreateImageFromROI(image_created, &roi);
        print_image(image_created_from_roi, "image_created_from_roi");
        vxReleaseImage(&image_created_from_roi);
    }

    // 创建制定像素值的图像
    {
        vx_pixel_value_t pixel_value{.S32=200};
        vx_image image_created_uniform = vxCreateUniformImage(context, 400, 300, VX_DF_IMAGE_S32, &pixel_value);
        print_image(image_created_uniform, "image_created_uniform");
        vxReleaseImage(&image_created_uniform);
    }

    // 根据已有数据创建图像
    vx_image image_created_from_handle;
    vx_uint8 *pixel_data[]{new vx_uint8[10 * 8]};
    {
        vx_imagepatch_addressing_t addr[]{{.dim_x=10, .dim_y=8, .stride_x=1, .stride_y=10}};
        image_created_from_handle = vxCreateImageFromHandle(context, VX_DF_IMAGE_U8, addr, (void **) pixel_data,VX_MEMORY_TYPE_HOST);
        print_image(image_created_from_handle, "image_created_from_handle");
    }

    // 图像数据地址交换
    vx_uint8 *pixel_data_new[]{new vx_uint8[10 * 8]};
    {
        for (int i = 0; i < 80; ++i) {
            pixel_data_new[0][i] = i;
        }
        vxSwapImageHandle(image_created_from_handle, (void **) pixel_data_new, nullptr, 1);
        print_image(image_created_from_handle, "swap image handle");
    }

    // 使用一维地址获取像素值
    {
        std::cout << "===============================" << std::endl;
        std::cout << "address 1d" << std::endl;
        vx_rectangle_t patch;
        vxGetValidRegionImage(image_created_from_handle, &patch);

        vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;

        vx_map_id map_id;
        unsigned char *ptr;
        vxMapImagePatch(image_created_from_handle, &patch, 0, &map_id, &addr, (void **) &ptr, VX_READ_ONLY,VX_MEMORY_TYPE_HOST, VX_NOGAP_X);

        for (int row = 0; row < addr.dim_y; ++row) {
            std::cout << "\t";
            for (int col = 0; col < addr.dim_x; ++col) {
                std::cout << (int) *((vx_uint8 *) vxFormatImagePatchAddress1d(ptr, row * addr.dim_x + col, &addr)) << "\t";
            }
            std::cout << std::endl;
        }
        vxUnmapImagePatch(image_created_from_handle, map_id);
    }

    // 使用二维地址获取像素值
    {
        std::cout << "===============================" << std::endl;
        std::cout << "address 2d" << std::endl;
        vx_rectangle_t patch;
        vxGetValidRegionImage(image_created_from_handle, &patch);

        vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;

        vx_map_id map_id;
        unsigned char *ptr;
        vxMapImagePatch(image_created_from_handle, &patch, 0, &map_id, &addr, (void **) &ptr, VX_READ_ONLY,VX_MEMORY_TYPE_HOST, VX_NOGAP_X);

        for (int row = 0; row < addr.dim_y; ++row) {
            std::cout << "\t";
            for (int col = 0; col < addr.dim_x; ++col) {
                std::cout << (int) *((vx_uint8 *) vxFormatImagePatchAddress2d(ptr, col, row, &addr)) << "\t";
            }
            std::cout << std::endl;
        }
        vxUnmapImagePatch(image_created_from_handle, map_id);
    }

    delete[] pixel_data_new[0];
    delete[] pixel_data[0];
    vxReleaseImage(&image_created);
    vxReleaseImage(&image_created_from_handle);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

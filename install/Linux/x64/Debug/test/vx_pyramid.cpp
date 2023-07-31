#include <iostream>
#include <VX/vx.h>

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

    std::cout << "\t" << "format        : " << format << std::endl;
    std::cout << "\t" << "img_width     : " << img_width << std::endl;
    std::cout << "\t" << "img_height    : " << img_height << std::endl;
    std::cout << "\t" << "planes        : " << planes << std::endl;
    std::cout << "\t" << "color_space   : " << color_space << std::endl;
    std::cout << "\t" << "channel_range : " << channel_range << std::endl;
    std::cout << "\t" << "memory_type   : " << memory_type << std::endl;
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
        vxMapImagePatch(image, &patch, i, &map_id, &addr, (void **) &ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST,VX_NOGAP_X);
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

void print_pyramid(vx_pyramid pyramid, const char *message)
{
    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;

    vx_size level;
    vxQueryPyramid(pyramid, (vx_enum)VX_PYRAMID_LEVELS, &level, sizeof(level));

    vx_float32 scale;
    vxQueryPyramid(pyramid, (vx_enum)VX_PYRAMID_SCALE, &scale, sizeof(scale));

    vx_uint32 width;
    vxQueryPyramid(pyramid, (vx_enum)VX_PYRAMID_WIDTH, &width, sizeof(width));

    vx_uint32 height;
    vxQueryPyramid(pyramid, (vx_enum)VX_PYRAMID_HEIGHT, &height, sizeof(height));

    vx_df_image format;
    vxQueryPyramid(pyramid, (vx_enum)VX_PYRAMID_FORMAT, &format, sizeof(format));

    std::cout << "level   : " << level << std::endl;
    std::cout << "scale   : " << scale << std::endl;
    std::cout << "width   : " << width << std::endl;
    std::cout << "height  : " << height << std::endl;
    std::cout << "format  : " << format << std::endl;

    for (int i = 0; i <level; ++i) {
        vx_image image = vxGetPyramidLevel(pyramid, i);
        char msg[120]{0};
        sprintf(msg, "level: %d", i);
        print_image(image, msg);
    }
}



int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    vx_context context = vxCreateContext();

    vx_pyramid pyramid = vxCreatePyramid(context, 3, VX_SCALE_PYRAMID_HALF, 10, 10, VX_DF_IMAGE_U8);
    print_pyramid(pyramid, "create");

    vxReleasePyramid(&pyramid);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}
/*
user@user-ThinkPad-T14-Gen-1:~/Downloads/openvx_second/OpenVX-sample-impl/install/Linux/x64/Debug/test/build$ ./vx_pyramid 
===============================
create
level   : 3
scale   : 0.5
width   : 10
height  : 10
format  : 942682197
===============================
level: 0
	format        : 942682197
	img_width     : 10
	img_height    : 10
	planes        : 1
	color_space   : 24576
	channel_range : 28672
	memory_type   : 57344
	is_uniform    : 0
	valid region  : 
		start_x   : 0
		start_y   : 0
		end_x     : 10
		end_y     : 10
	image patch addressing   : 
		plane        : 0
		addr.dim_x   : 10
		addr.dim_y   : 10
		addr.stride_x: 1
		addr.stride_y: 10
		addr.scale_x : 1024
		addr.scale_y : 1024
		addr.step_x  : 1
		addr.step_y  : 1

===============================
level: 1
	format        : 942682197
	img_width     : 5
	img_height    : 5
	planes        : 1
	color_space   : 24576
	channel_range : 28672
	memory_type   : 57344
	is_uniform    : 0
	valid region  : 
		start_x   : 0
		start_y   : 0
		end_x     : 5
		end_y     : 5
	image patch addressing   : 
		plane        : 0
		addr.dim_x   : 5
		addr.dim_y   : 5
		addr.stride_x: 1
		addr.stride_y: 5
		addr.scale_x : 1024
		addr.scale_y : 1024
		addr.step_x  : 1
		addr.step_y  : 1

===============================
level: 2
	format        : 942682197
	img_width     : 3
	img_height    : 3
	planes        : 1
	color_space   : 24576
	channel_range : 28672
	memory_type   : 57344
	is_uniform    : 0
	valid region  : 
		start_x   : 0
		start_y   : 0
		end_x     : 3
		end_y     : 3
	image patch addressing   : 
		plane        : 0
		addr.dim_x   : 3
		addr.dim_y   : 3
		addr.stride_x: 1
		addr.stride_y: 3
		addr.scale_x : 1024
		addr.scale_y : 1024
		addr.step_x  : 1
		addr.step_y  : 1


*/

#include <stdio.h>
#include <stdlib.h>
#include <VX/vx.h>

int main()
{
    // 创建OpenVX上下文
    vx_context context = vxCreateContext();

    // 创建图像内存对象的描述
    vx_uint32 width = 640;
    vx_uint32 height = 480;
    vx_df_image format = VX_DF_IMAGE_U8;

    // 分配内存并创建图像内存对象
    vx_uint8 *data = (vx_uint8 *)malloc(width * height * sizeof(vx_uint8));
    vx_imagepatch_addressing_t addr;
    addr.dim_x = width;
    addr.dim_y = height;
    addr.stride_x = sizeof(vx_uint8);
    addr.stride_y = width * sizeof(vx_uint8);

    vx_image image = vxCreateImageFromHandle(context, format, &addr, (void**)&data, VX_MEMORY_TYPE_HOST);

    // 验证图像内存对象是否创建成功
    vx_status status = vxGetStatus((vx_reference)image);
    if (status != VX_SUCCESS)
    {
        printf("Failed to create image memory object!\n");
        free(data);
        vxReleaseContext(&context);
        return -1;
    }

    // 写入一些数据到图像内存对象
    for (vx_uint32 y = 0; y < height; y++)
    {
        for (vx_uint32 x = 0; x < width; x++)
        {
            data[y * addr.stride_y + x * addr.stride_x] = (vx_uint8)(x % 256);
        }
    }

    // 从图像内存对象读取并验证值
    vx_rectangle_t rect;
    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = width;
    rect.end_y = height;

    // vx_imagepatch_addressing_t read_addr;
    // vx_map_id read_addr;
    // vx_imagepatch_addressing_t ptr;
    // status = vxMapImagePatch(image, &rect, 0, &read_addr, &ptr, nullptr, VX_MEMORY_TYPE_HOST, 0);
    vx_imagepatch_addressing_t read_addr;
    void *ptr = NULL;
    status = vxMapImagePatch(image, &rect, 0, (vx_map_id*)&read_addr, (vx_imagepatch_addressing_t*)ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);
    if (status == VX_SUCCESS)
    {
        vx_uint8 *read_data = reinterpret_cast<vx_uint8*>(&ptr);
        vx_uint8 pixelValue = read_data[0];
        printf("Pixel value at (0, 0): %d\n", pixelValue);

        // 解除映射图像内存对象
        vxUnmapImagePatch(image, (vx_map_id*)&rect, 0, ptr);
    }

    // 释放资源
    free(data);
    vxReleaseImage(&image);
    vxReleaseContext(&context);

    return 0;
}

#include <VX/vx.h>
#include <stdio.h>

int main() {
    // 创建OpenVX上下文
    vx_context context = vxCreateContext();

    // 创建一个 vx_image 对象，这里只是示例，你可以替换为你需要的对象类型
    // vx_image img = vxCreateImage(context, /* 参数 */);
    vx_uint32 width = 1024, height = 760;
    vx_image img = vxCreateImage(context, width, height, VX_DF_IMAGE_RGB);
    // 获取对象的状态
    vx_status status = vxGetStatus((vx_reference)img);

    if (status == VX_SUCCESS) {
        // 对象创建成功
        printf("Object creation succeeded.\n");

        // 可以在这里使用对象进行处理

        // 释放对象
        vxReleaseImage(&img);
    } else {
        // 对象创建失败，处理错误
        printf("Object creation failed with status %d.\n", status);
    }

    // 释放OpenVX上下文
    vxReleaseContext(&context);

    return 0;
}
/*
 g++ -I include test/all_test.cpp bin/libopenvx.so bin/libvxu.so -o test/all_test
./test/all_test
Object creation succeeded.
*/
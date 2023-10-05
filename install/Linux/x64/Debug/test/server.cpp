#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <VX/vx.h>

#define SHM_KEY 12345

int main()
{
    // 创建OpenVX上下文
    vx_context context = vxCreateContext();

    // 创建共享内存段
    int shmid = shmget(SHM_KEY, 640 * 480, 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }

    // 附加共享内存
    vx_uint8 *sharedData = (vx_uint8 *)shmat(shmid, NULL, 0);

    // 创建OpenVX图像
    vx_image image = vxCreateImage(context, 640, 480, VX_DF_IMAGE_U8);

    // 将共享内存数据拷贝到OpenVX图像
    vx_imagepatch_addressing_t addr;
    addr.dim_x = 640;
    addr.dim_y = 480;
    addr.stride_x = 1;
    addr.stride_y = 640;
    vx_uint8 *ptr;
    vx_imagepatch_addressing_t *addr_ptr = &addr;
    vx_rectangle_t rect;
    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = 640;
    rect.end_y = 480;
    vxCopyImagePatch(image, &rect, 0, addr_ptr, sharedData, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

    // 在这里进行图像处理操作，这里只是示例，您可以根据需求进行处理

    // 等待其他进程生成新的图像，然后清理资源
    printf("Press Enter to exit...");
    getchar();

    // 释放资源
    shmdt(sharedData);
    vxReleaseImage(&image);
    vxReleaseContext(&context);

    return 0;
}
/*
g++ -I include test/client.cpp bin/libopenvx.so bin/libvxu.so -o test/client
~/Downloads/openvx_second/OpenVX-sample-impl/install/Linux/x64/Debug$ g++ -I include test/server.cpp bin/libopenvx.so bin/libvxu.so -o test/server
~/Downloads/openvx_second/OpenVX-sample-impl/install/Linux/x64/Debug$ ./test/client
Press Enter to exit...
*/

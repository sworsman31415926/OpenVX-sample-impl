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

    // 创建图像
    vx_image image = vxCreateImage(context, 640, 480, VX_DF_IMAGE_U8);

    // 在这里生成图像数据，这里只是示例，您可以用实际图像数据替换它
    vx_uint8 *imageData = (vx_uint8 *)malloc(640 * 480);
    memset(imageData, 128, 640 * 480); // 填充为灰色

    // 将图像数据拷贝到OpenVX图像
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
    vxCopyImagePatch(image, &rect, 0, addr_ptr, imageData, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    // 创建共享内存段
    int shmid = shmget(SHM_KEY, 640 * 480, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }

    // 附加共享内存
    vx_uint8 *sharedData = (vx_uint8 *)shmat(shmid, NULL, 0);

    // 拷贝图像数据到共享内存
    memcpy(sharedData, imageData, 640 * 480);

    // 等待其他进程处理图像，然后清理资源
    printf("Press Enter to exit...");
    getchar();

    // 释放资源
    shmdt(sharedData);
    shmctl(shmid, IPC_RMID, NULL);
    free(imageData);
    vxReleaseImage(&image);
    vxReleaseContext(&context);

    return 0;
}

#include <iostream>
#include <VX/vx.h>

void print_distribution(vx_distribution distribution, const char *message)
{
    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;

    vx_size dimensions;
    vxQueryDistribution(distribution, (vx_enum)VX_DISTRIBUTION_DIMENSIONS, &dimensions, sizeof(dimensions));

    vx_int32 offset;
    vxQueryDistribution(distribution, (vx_enum)VX_DISTRIBUTION_OFFSET, &offset, sizeof(offset));

    vx_uint32 range; // 需要为bins的倍数
    vxQueryDistribution(distribution, (vx_enum)VX_DISTRIBUTION_RANGE, &range, sizeof(range));

    vx_size bins; // BIN的数量
    vxQueryDistribution(distribution, (vx_enum)VX_DISTRIBUTION_BINS, &bins, sizeof(bins));

    vx_uint32 window; // BIN的宽度 window=RANGE/BINS
    vxQueryDistribution(distribution, (vx_enum)VX_DISTRIBUTION_WINDOW, &window, sizeof(window));

    vx_size size; // 总字节数
    vxQueryDistribution(distribution, (vx_enum)VX_DISTRIBUTION_SIZE, &size, sizeof(size));

    std::cout << "dimensions    : " << dimensions << std::endl;
    std::cout << "offset        : " << offset << std::endl;
    std::cout << "range         : " << range << std::endl;
    std::cout << "bins          : " << bins << std::endl;
    std::cout << "window        : " << window << std::endl;
    std::cout << "size          : " << size << std::endl;

    vx_map_id map_id;
    void* ptr;
    // 输出的地址是 vx_uint32 的数组，数量为 VX_DISTRIBUTION_BINS 的返回值
    std::cout << "item          : " << std::endl;
    vxMapDistribution(distribution, &map_id, &ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);
    for (int i = 0; i < bins; ++i) {
        std::cout << "\t" << "index: " << i << ", value: " << ((vx_uint32*)ptr)[i] << std::endl;
    }
    vxUnmapDistribution(distribution, map_id);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    vx_status status;
    vx_context context = vxCreateContext();

    vx_size bins = 64;
    vx_uint32 offset = 0U;
    vx_uint32 range = 256U;
    vx_distribution distribution = vxCreateDistribution(context, bins, offset, range);
    print_distribution(distribution, "create");

    vx_uint32* array = new vx_uint32 [bins];
    for (int i = 0; i < bins; ++i) {
        array[i] = i;
    }
    vxCopyDistribution(distribution, array, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    print_distribution(distribution, "update");
    delete[] array;

    vxReleaseDistribution(&distribution);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

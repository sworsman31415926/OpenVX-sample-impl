#include <iostream>
#include <VX/vx.h>

void print_lut(vx_lut lut, const char *message)
{
    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;

    vx_enum type;
    vxQueryLUT(lut, (vx_enum)VX_LUT_TYPE, &type, sizeof(type));

    vx_size count;
    vxQueryLUT(lut, (vx_enum)VX_LUT_COUNT, &count, sizeof(count));

    vx_size size;
    vxQueryLUT(lut, (vx_enum)VX_LUT_SIZE, &size, sizeof(size));

    vx_uint32 offset;
    vxQueryLUT(lut, (vx_enum)VX_LUT_OFFSET, &offset, sizeof(offset));

    std::cout << "type    : " << type << std::endl;
    std::cout << "count   : " << count << std::endl;
    std::cout << "size    : " << size << std::endl;
    std::cout << "offset  : " << offset << std::endl;

    vx_map_id map_id;
    void* ptr;
    std::cout << "item    : " << std::endl;
    vxMapLUT(lut, &map_id, &ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);
    for (int i = 0; i < count; ++i) {
        std::cout << "\t" << "index: " << i << ", value: " << (int)((vx_uint8*)ptr)[i] << std::endl;
    }
    vxUnmapLUT(lut, map_id);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    vx_context context = vxCreateContext();

    vx_lut lut = vxCreateLUT(context, VX_TYPE_UINT8, 10);
    print_lut(lut, "create");

    vx_uint8 *array = new vx_uint8 [10];
    for (int i = 0; i < 10; ++i)
        array[i] = i;
    vxCopyLUT(lut, array, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    print_lut(lut, "update");

    delete[] array;
    vxReleaseLUT(&lut);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

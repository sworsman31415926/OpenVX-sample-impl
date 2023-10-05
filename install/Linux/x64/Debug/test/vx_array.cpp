#include <iostream>
#include <VX/vx.h>
 
// Array 是一个强类型的容器。该容器提供相同速度的随机访问。
// 它对自己的元素使用值语义并保存数据副本。以下是一个数组遍历
 
void print_array(vx_array array, char* message)
{
    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;
 
    vx_size num;
    vxQueryArray(array, VX_ARRAY_NUMITEMS, &num, sizeof(num));
    
    vx_enum type;
    vxQueryArray(array, VX_ARRAY_ITEMTYPE, &type, sizeof(type));
    
    vx_size capacity;
    vxQueryArray(array, VX_ARRAY_CAPACITY, &capacity, sizeof(capacity));
 
    vx_size item_size;
    vxQueryArray(array, VX_ARRAY_ITEMSIZE, &item_size, sizeof(item_size));
 
    std::cout << "num      : " << num << std::endl;
    std::cout << "type     : " << type << std::endl;
    std::cout << "capacity : " << capacity << std::endl;
    std::cout << "item_size: " << item_size << std::endl;
 
    vx_map_id map_id;
    vx_size size;
    void *ptr;
 
    vxMapArrayRange(array, 0, 5, &map_id, &size, &ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, VX_NOGAP_X);
    std::cout << "ptr      : " << ptr << std::endl;
    for (size_t i = 0; i < num; i++)
    {
        //std::cout << "index: " << i << ", value: " << (int)((uint8_t*)ptr)[i] << std::endl;
        std::cout << "index: " << i << ", value: " << (int)vxArrayItem(uint8_t, ptr, i, item_size) << std::endl;
    }
    vxUnmapArrayRange(array, map_id);
}
 
int main(int argc, char *argv[])
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
 
    // 创建数组，此时数组仅有容量，未有数据
    vx_array array = vxCreateArray(context, (vx_enum)VX_TYPE_INT8, 5);
    print_array(array, (char*)"create");
 
    // 添加元素
    uint8_t uint8_new_arr[]{5, 6, 7, 8, 9};
    vxAddArrayItems(array, 5, uint8_new_arr, sizeof(uint8_t));
    print_array(array, (char*)"add item");
 
    // 更新元素
    uint8_t uint8_init_arr[]{0, 1, 2, 3, 4};
    vxCopyArrayRange(array, 0, 5, sizeof(uint8_t), uint8_init_arr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
 
    print_array(array, (char*)"update item");
 
    // 再次增加元素，默认情况下，该函数不会重新分配内存。如果在添加元素之前，数组已满，或者剩余空间不足以存放新数据，返回VX_FAILURE
    uint8_t uint8_new_arr_again[]{5, 6, 7, 8, 9};
    status = vxAddArrayItems(array, 5, uint8_new_arr_again, sizeof(uint8_t));
    std::cout << "vxAddArrayItems: " << status << std::endl;
    print_array(array, (char*)"add items again");
 
    // 截断数组，重新分配数据内存。截断后的数据为随机值
    status = vxTruncateArray(array, 3);
    std::cout << "vxTruncateArray: " << status << std::endl;
    print_array(array, (char*)"truncate array");
 
    vxReleaseArray(&array);
    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}

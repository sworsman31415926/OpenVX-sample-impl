#include <iostream>
#include <VX/vx.h>

int main()
{
// 创建capture节点，获取图像
status = app_create_graph_capture(obj->graph, &obj->captureObj);
// 创建ldc节点，进行摄像头校正和格式转换
status = app_create_graph_ldc(obj->graph, &obj->ldcObj, obj->captureObj.raw_image_arr[0]);
// 创建mosaic节点，将四个摄像头图像拼成田字格
status = app_create_graph_img_mosaic(obj->graph, &obj->imgMosaicObj);
// 创建display节点，显示输出
status = app_create_graph_display(obj->graph, &obj->displayObj, display_in_image);
// 设置Graph参数(输入和输出)
graph_parameter_index = 0;
// 设置Graph参数
add_graph_parameter_by_node_index(obj->graph, obj->captureObj.node, 1);// Graph中图像输入的节点，图像输出在node中的参数位置
obj->captureObj.graph_parameter_index = graph_parameter_index;
graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH; // 设置节点缓冲区个数
graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->captureObj.raw_image_arr[0];// 输出的图像
graph_parameter_index++;
// 设置Graph运行模式
status = vxSetGraphScheduleConfig(obj->graph,
                VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                graph_parameter_index,
                graph_parameters_queue_params_list);
status = tivxSetGraphPipelineDepth(obj->graph, APP_PIPELINE_DEPTH);

// 图像入队 次数等于缓冲区个数
status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&obj->captureObj.raw_image_arr[obj->enqueueCnt], 1);

for(;;)
{
    // capture节点输出一帧图像
    status = vxGraphParameterDequeueDoneRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&capture_input_arr, 1, &num_refs);
	status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&capture_input_arr, 1);

}

return 0;
}
          

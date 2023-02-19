# Real_responsitory
寒假修改主要如下：<br>
1.armor_detection 的数字识别那部分的结构改了很多，数字识别速度会更快，只算数字识别dnn部署推理网络部分只需要0.2毫秒左右<br>
![image](https://user-images.githubusercontent.com/84974759/219947253-0c37e9e1-d8e7-4cca-ab0f-c888c783c64b.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219947269-b91f131d-13e6-4f4d-9c41-995f7ee4398f.png)<br>


2.opencv的图像拷贝操作，clone实测会比copyTo慢上一截，所以所有的clone都被我替换成了copyTo，这是对比图：（前两张是copyTo，后两张是clone）<br>
![image](https://user-images.githubusercontent.com/84974759/219947346-ea34263b-cc2f-440e-91c0-4cd45e1eba2e.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219947354-5a995b87-0373-47c2-9d2e-b16bddb05d85.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219947403-b1483dfd-a9a6-40f8-9382-462b6e76331d.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219947406-1a32f4d0-caaf-43e3-bf69-61bf43327c20.png)<br>


3.目前修改了上述两点之后，只算装甲板检测部分只需要6毫秒左右（这是包含有图像显示的时间计算在内的时间开销），如果没有图像显示和时间计算的开销可以达到4毫秒左右，效果图（其中run_time是装甲板检测的时间，choose_time是筛选装甲板部分的时间，light_time是选择灯条部分的时间，set_time是处理图像部分（即拷贝图像并二值化）的时间，可以看到主要的耗时的地方是这三个）<br>
以下是有包含图像显示和时间计算的：<br>
![image](https://user-images.githubusercontent.com/84974759/219947714-000dabcd-4563-4cdc-a92e-7773973943b2.png)<br>
以下是没有包含的：<br>
![image](https://user-images.githubusercontent.com/84974759/219947965-10779ace-2854-4be9-ace5-6624c8081021.png)<br>


4.另外std::chrono库用来计算时间虽然和getTickcount差不多，但精度确实高一些，所以计算时间就用std::chrono库吧，我定义了三个对应单位的时间的类型声明，这样可以避免在用这个库计算时间的时候写一大堆，如下：<br>
![image](https://user-images.githubusercontent.com/84974759/219948116-bb4c6933-d974-4cab-a073-81a13b1486d5.png)<br>


5.我们的数字识别模型已经很稳定了（不是佛山的时候那种稳定，是真的很稳定），现在我自己视频测试的时候给的置信度阈值都是0，完全不需要调阈值，如下：<br>
![image](https://user-images.githubusercontent.com/84974759/219948264-5297a0ab-b756-41d8-93b0-2c56bbf02239.png)<br>
即使是在置信度阈值为0的情况下，他依然不会出现误识别<br>
![image](https://user-images.githubusercontent.com/84974759/219948478-d2eab7fe-1cf9-47d6-a69d-615fc8fec313.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219948491-f553c0b7-669d-4cec-9af9-2ab9875eda60.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219948558-0ee46e3f-efc4-457d-a940-1385564caaf0.png)<br>
同时哨兵装甲板也很准确<br>
![image](https://user-images.githubusercontent.com/84974759/219948626-a68ece31-d066-4eaf-a800-f23f069f846e.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219948652-122f47df-2e7a-4009-9b16-11fb8d4f6188.png)<br>
![image](https://user-images.githubusercontent.com/84974759/219948688-1bb7b808-e1e7-47b4-b0e1-d17411bee9de.png)<br>


6.另外基于Windows和Linux两个系统的静态动态库文件不同，但平常两种系统都会用来调试，所以我给cmakelist.txt加了一些东西，可以方便两种系统的转换，如图：<br>
![image](https://user-images.githubusercontent.com/84974759/219948839-27324bd9-385d-4283-8e79-791178ed8c51.png)<br>


7.最近OpenVINO和TensorRT两个神经网络加速工具（前者用于CPU推理加速，后者用于GPU推理加速）我都完善了一套可以我们使用的代码出来，具体看我的github仓库，yolov5代码在看了<br>
![image](https://user-images.githubusercontent.com/84974759/219949135-c8ee88ef-e409-412c-a0bb-38e6d0f07215.png)<br>

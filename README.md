# Real_responsitory
迭代修改主要如下：<br>
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


7.最近OpenVINO和TensorRT两个神经网络加速工具（前者用于CPU推理加速，后者用于GPU推理加速）我都完善了一套可以我们使用的代码出来，具体看我的github仓库<br>
![image](https://user-images.githubusercontent.com/84974759/219949135-c8ee88ef-e409-412c-a0bb-38e6d0f07215.png)<br>


8.串口接收增加了四元数接收，现在一次需要接收32个字节的数据，四元数的接收用于转为旋转矩阵用于世界坐标系的转换，详见最新的get_Model_new函数，而四元数转旋转矩阵相关算法看第二张图（已经过验证是对的，即四元数转为的旋转矩阵对应获得的角度是正确的）如图：<br>
![image](https://user-images.githubusercontent.com/84974759/220103873-090b8160-c06d-49ba-b5be-dc31eeb71238.png)<br>
![image](https://user-images.githubusercontent.com/84974759/220571910-13f39d09-7229-4cf6-8506-3e669aac70bd.png)<br>


9.颜色接收成为过去式，现在颜色的判定用yaml文件写入参数来解决，在保留之前的EumeColor和大多数地方的代码不变下的改变，参数写在detector的yaml里：<br>
![image](https://user-images.githubusercontent.com/84974759/220104796-0a8c82d0-2ad3-4e14-931d-2197f2546d37.png)<br>


10.Singer模型新增滤波算法，防止Singer算出的预测值过于离谱，且防止抖动，滤波表达式灵感来源于tanh激活函数<br>
![image](https://user-images.githubusercontent.com/84974759/222332122-b0f02749-1274-454e-9e06-668c3fa03517.png)<br>


11.坐标系变换最后不需要转到camera坐标系来算角度，在world坐标系下算出角度，这样可以保证给电控的转角是对于云台转轴中心的，而不是对于相机转轴中心的<br>
![image](https://user-images.githubusercontent.com/84974759/222332503-3b313749-dd4a-496b-9e25-2062b6eb2a1e.png)<br>


12.串口传输使用ttyACM通讯设备，可以不用usb转ttl，用c板上的虚拟串口，接线使用microusb线，不用易断不稳定的杜邦线，而且新的串口收发的内存对齐，压包数据的做法很大程度减少CRC错误率，！注：ttyACM begin之前的版本是都不包含ttyACM功能的版本，ttyACM begin及之后的版本才包含ttyACM功能，新增的ttyACM相关代码部分文件为serial_device.cpp/.h，serial_main.cpp/.h，protocol.h，具体调用ttyACM的方式参考ttyACM begin那一版本的main_debug.cpp未注释部分。<br>













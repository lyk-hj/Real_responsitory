#include "thread.h"

using namespace cv;
//using namespace robot_detection;
using namespace std;

//SerialPort port;
SerialMain serial;

//one2two
form _send_data;
Mat src;

//two2three
//vector<double> vision_send(3);


void* Sample(void* PARAM)
{
    int first_get/* = true*/;
	chrono_time time_temp;
	Mat get_src;
    auto camera_warper = new Camera;
	printf("camera_open-\n");
    // chrono
//    auto start = chrono::high_resolution_clock::now();
	if (camera_warper->init())
	{
		//printf("1-real\n");
		while (is_continue && (waitKey(1) != 27))
		{
            // chrono
//            auto end = chrono::high_resolution_clock::now();    //结束时间
			if (camera_warper->read_frame_rgb())
			{
				get_src = camera_warper->src.clone();

//				lin_is_get = port.get_Mode1_new(mode_temp, lin[0], lin[1], lin[2],quat);
				first_get = serial.ReceiverMain();
				time_temp = chrono::high_resolution_clock::now();
                pthread_mutex_lock(&mutex_new);
                {
                    get_src.copyTo(src);
					_send_data = {0x21,first_get, //! 0x21应该换成serial.vision_msg_.mode赋值，方便调试默认给的0x21
								 {serial.vision_msg_.pitch,
								  		serial.vision_msg_.yaw,
								  		serial.vision_msg_.shoot},
								 {serial.vision_msg_.quaternion[0],
								  		serial.vision_msg_.quaternion[1],
								  		serial.vision_msg_.quaternion[2],
								  		serial.vision_msg_.quaternion[3]},
								 vector<Armor>(),time_temp};
                    is_start = true;
                    pthread_cond_signal(&cond_new);
                    pthread_mutex_unlock(&mutex_new);
                    imshow("src",src);
                    camera_warper->release_data();
                }

			}
			else
			{
				src = cv::Mat();
			}
//			start = end;
		}
		camera_warper->~Camera();
		pthread_mutex_unlock(&mutex_new);
		is_continue = false;
	}
	else
	{
		printf("No camera!!\n");
		is_continue = false;
	}
}

void* Implement(void* PARAM)
{
    //!< Task 1:Detection
    ArmorDetector Detect;
    std::vector<Armor> Targets;
	Mat src_copy;
	chrono_time time_temp;
	int mode_temp;
	int second_get;

	//!< Task 2:Track and Predict
	vector<double> vdata(3);
	ArmorTracker Track;

	//!< 感觉可以不需要sleep，可以用一个Peterson's Algorithm来进行临界区互斥同步（猜想）
	sleep(2);
	printf("Armor_open\n");
	while (is_continue)
	{
		pthread_mutex_lock(&mutex_new);

		while (!is_start) {

			pthread_cond_wait(&cond_new, &mutex_new);
		}
		is_start = false;

		src.copyTo(src_copy);
        Detect.updateData(_send_data.data,_send_data.quat);
        Track.AS.updateData(_send_data.data,_send_data.quat);
        mode_temp = _send_data.mode;
        second_get = _send_data.dat_is_get;
        time_temp = _send_data.tim;
		pthread_mutex_unlock(&mutex_new);
		if(second_get && mode_temp == 0x21)
        {
        	printf("[mode_temp]:    |%x\n",mode_temp);
            Targets = Detect.autoAim(src_copy);
//                    Track.AS.bullet_speed = 28.0;
            if (Track.locateEnemy(src_copy,Targets,time_temp))
            {
                vdata = { Track.pitch, Track.yaw, 0x31 };
                Track.show();
            }
            else
            {
                //    原数据，无自瞄
                vdata = { Track.AS.ab_pitch, Track.AS.ab_yaw, 0x32 };
            }
            serial.SenderMain(vdata);
//            pthread_mutex_lock(&mutex_send);
//            vision_send = vdata;
//            is_send = true;
//            pthread_cond_signal(&cond_send);
//            pthread_mutex_unlock(&mutex_send);
        }


	}
}

//void* Send(void* PARAM)
//{
//	vector<double> vdata(3);
//	int mode_temp;
//	int data_get;
//
//    sleep(3);
//    printf("send_open\n");
//	while (is_continue)
//	{
//		pthread_mutex_lock(&mutex_send);
//
//		while (!is_send) {
//
//			pthread_cond_wait(&cond_send, &mutex_send);
//		}
//
//		is_send = false;
//		vdata = vision_send;
//        pthread_mutex_unlock(&mutex_send);
//        if(data_get && mode_temp == 0x21)
//		{
//				serial.SenderMain(vdata);
//		}
//	}
//}



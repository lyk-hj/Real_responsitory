#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "armor_detection.h"
#include "armor_track.h"
#include "camera.h"
#include <stdlib.h>
#include <time.h>
#include "serialport.h"
using namespace cv;
//using namespace robot_detection;
SerialPort port("/dev/ttyUSB0");

//#define COLOR RED

int main()
{
    auto camera_warper = new Camera;
//    float data[4] = {0,0,0,28};
    robot_state robot;
//    robot.updateData(data);
    ArmorDetector autoShoot;
//    autoShoot.clone(robot);
    ArmorTracker autoTrack;
//    autoTrack.AS.clone(robot);
    Skalman Singer;
    std::vector<Armor> autoTargets;
    Eigen::Vector3d predicted_position;
    Mat src;
    int lost_count = 0;
    int mode_temp;
    float lin[3];
    float quat[4];
    if (camera_warper->init())
    {
        while(true)
        {
            if(camera_warper->read_frame_rgb())
            {
                //            	std::cout<<"init success"<<std::endl;
                src = camera_warper->src.clone();
                camera_warper->release_data();
            }
            bool dataGet = port.get_Mode1_new(mode_temp, lin[0], lin[1], lin[2],quat);
            autoTrack.AS.updateData(lin,quat);
            Eigen::Matrix3d quatRx = autoTrack.AS.quaternionToRotationMatrix();


//            autoTargets = autoShoot.autoAim(src);
//            if (!autoTargets.empty())
//            {
//                Mat src_copy;
//                printf("---------------main get target!!!---------------\n");
//                double now_time = (double)getTickCount();
//                src.copyTo(src_copy);
//                if(autoTrack.locateEnemy(src_copy,autoTargets,now_time))
//                {
//                    //todo:show state
//                    std::cout<<"track!!!"<<autoTrack.tracker_state<<"  id: "<<autoTrack.tracking_id<<std::endl;
//                }
//                else
//                {
//                    std::cout<<"loss!!!"<<std::endl;
//                }
//            }
//            else
//            {
//                autoTrack.Singer.Reset();
//                lost_count++;
//                printf("----------------no target---------------\n");
//            }
            imshow("src",src);
            if (waitKey(1) == 27)
            {
                camera_warper->~Camera();
                break;
            }
        }
    }
    printf("lost_count:%d\n",lost_count);
    return 0;
}

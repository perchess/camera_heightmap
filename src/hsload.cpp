//
// Created by allen on 2020/11/20.
//
#include <iostream>
#include <ros/ros.h>
#include <fstream>
//
#include <lcm/lcm-cpp.hpp>
#include "traversability_float_t.hpp"
#include "heightnew_t.hpp"
#include "heightmap_t.hpp"

#define DATA_DIR "/home/den/catkin_workspaces/visionMPC/src/camera_heightmap/data"

using namespace std;
int
main(int argc, char **argv)
{   float tmp_h;
    ros::init(argc, argv, "publisher");
    ros::NodeHandle nh;
    lcm::LCM lcm;
    heightmap_t heightnew_lcm;//高度地图矩阵
    traversability_float_t trav_lcm;
    if (!lcm.good())
        return 1;

//    ifstream fin("/home/allen/catkin_ws/src/my_pcl/data/heightmaprough.txt", ios::in);
//    ifstream fin("/home/allen/catkin_ws/src/my_pcl/data/heightmap18.txt", ios::in);
    ifstream fin(DATA_DIR"/heightmap11.txt", ios::in);
    if(!fin)
    {
        std::cout << "The file" <<  "DATA_DIR" "/heightmap11.txt" << " is not exist!" << std::endl;
        return -1;
    }
    for (int i(0); i<101;++i){
        for (int j = 0; j < 101; ++j) {
            fin >> tmp_h;
            if (tmp_h-0.04<0)
                heightnew_lcm.map[i][j]=0.0;
            else heightnew_lcm.map[i][j]=tmp_h-0.04;
//            heightnew_lcm.map[i][j]=tmp_h;
        }
    }
    fin.close();
//    ifstream fin2("/home/allen/catkin_ws/src/my_pcl/data/scoremaprough.txt", ios::in);
//    ifstream fin2("/home/allen/catkin_ws/src/my_pcl/data/scoremap18.txt", ios::in);
    ifstream fin2(DATA_DIR"/scoremap11.txt", ios::in);
    if(!fin2)
    {
        printf("The file 2 is not exist!");
        return -1;
    }
    for (int i(0); i<100;++i){
        for (int j = 0; j < 100; ++j) {
            fin2 >> trav_lcm.map[i][j];
        }
    }
    fin2.close();
    long iters;
    while (ros::ok()) {
        lcm.publish("local_heightmap", &heightnew_lcm);
//        ros::Duration(0.1).sleep();//更新频率
        lcm.publish("traversability_float", &trav_lcm);

        iters ++;
        if(iters%100==0)
            printf("Send successfully!\n");
    }

}
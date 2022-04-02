#include <iostream>
#include <ros/ros.h>
#include <ros/package.h>
#include <fstream>
//
#include <lcm/lcm-cpp.hpp>
#include "traversability_float_t.hpp"
#include "heightnew_t.hpp"
#include "heightmap_t.hpp"

//! @brief Шаблоннная функция для чтения параметров
template <typename T>
void readParam(const std::string param_name, T& param_value,
               const T default_value)
{
  if (!ros::param::get(param_name, param_value)) {
    ROS_WARN_STREAM("Parameter \""
                    << param_name << "\" didn' find in Parameter Server."
                    << "\nSetting default value: " << default_value);
    param_value = default_value;
  }
}

using namespace std;
int main(int argc, char **argv)
{
  float tmp_h;
  ros::init(argc, argv, "hsload");
  ros::NodeHandle nh;
  std::string datadir = ros::package::getPath("camera_heightmap") + "/data/";
  std::string heightmap_name;
  std::string scoremap_name;
  readParam("heightmap_name", heightmap_name, std::string("heightmap22_scaled.txt"));
  readParam("scoremap_name", scoremap_name, std::string("scoremap22.txt"));
  lcm::LCM lcm;
  heightnew_t heightnew_lcm;
  traversability_float_t trav_lcm;

  if (!lcm.good())
  {
    ROS_ERROR("Lcm is bad");
    return 1;
  }

  ifstream fin(datadir + "heightmap22_scaled.txt", ios::in);
  if(!fin)
  {
    std::cout << "The file" <<  datadir + heightmap_name<< " is not exist!" << std::endl;
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

  ifstream fin2(datadir + "scoremap22.txt", ios::in);
  if(!fin2)
  {
    std::cout << "The file" <<  datadir + scoremap_name << " is not exist!" << std::endl;
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
    lcm.publish("heightmapnew", &heightnew_lcm);
    ros::Rate(25).sleep();
    lcm.publish("traversability_float", &trav_lcm);

    iters ++;
    if(iters%100==0)
      printf("Send successfully!\n");
  }
  return 0;

}

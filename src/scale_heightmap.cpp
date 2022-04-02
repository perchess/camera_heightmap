#include <iostream>
#include <ros/ros.h>
#include <fstream>
#include <ros/package.h>

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
  double tmp_h = 0;
  double scale = 0;
  std::string type = ".txt";
  std::string name ("heightmap22");
  std::string dir(ros::package::getPath("camera_heightmap") + "/data/");
  ros::init(argc, argv, "heightmap_scaler");
  ros::NodeHandle nh;
  readParam("~scale", scale, 2.0);
  readParam("~in_filename", name, string("heightmap22"));
  readParam("~dir", dir, ros::package::getPath("camera_heightmap") + "/data/");


  ifstream fin(dir + name + type, ios::in);
  if(!fin)
  {
    std::cout << "The file" <<  dir << " does not exist!" << std::endl;
    return -1;
  }
  // Create new file
  std::string out_name(name + "_scaled");
  std::ofstream outfile (dir + out_name + type);
  for (int i(0); i<101;++i){
    for (int j = 0; j < 101; ++j) {
      fin >> tmp_h;
      outfile << tmp_h * scale << std::endl;
    }
  }
  fin.close();
  outfile.close();
  ROS_INFO_STREAM("New file location is :" << dir + out_name + type);
  while (ros::ok())
  {
    ros::Rate(25).sleep();
  }
  return 0;

}

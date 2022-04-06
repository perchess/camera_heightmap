#include <fstream>
#include <iostream>
#include <ros/package.h>
#include <ros/ros.h>

//! @brief Шаблоннная функция для чтения параметров
template<typename T>
void readParam(const std::string param_name,
               T& param_value,
               const T default_value)
{
  if (!ros::param::get(param_name, param_value))
  {
    ROS_WARN_STREAM("Parameter \""
                    << param_name << "\" didn' find in Parameter Server."
                    << "\nSetting default value: " << default_value);
    param_value = default_value;
  }
}

using namespace std;
int main(int argc, char** argv)
{
  double tmp_h = 0;
  double scale = 0;
  std::string type = ".txt";
  std::string input_name("heightmap22");
  std::string out_name(input_name + "_scaled");
  std::string dir(ros::package::getPath("camera_heightmap") + "/data/");

  ros::init(argc, argv, "heightmap_scaler");
  ros::NodeHandle nh;
  readParam("~scale", scale, 2.0);
  readParam("~in_filename", input_name, string("heightmap22"));
  readParam("~out_filename", out_name, input_name + "_scaled");
  readParam("~dir", dir, ros::package::getPath("camera_heightmap") + "/data/");

  ifstream fin(dir + input_name + type, ios::in);
  if (!fin)
  {
    std::cout << "The file" << dir << " does not exist!" << std::endl;
    return -1;
  }
  // Create new file
  std::ofstream outfile(dir + out_name + type);
  for (int i(0); i < 101; ++i)
  {
    for (int j = 0; j < 101; ++j)
    {
      fin >> tmp_h;
      //      if (tmp_h > 1e-3)
      //      {
      //        tmp_h = 2;
      //      }
      //      outfile << tmp_h << std::endl;
      outfile << tmp_h * scale << std::endl;
    }
  }
  fin.close();
  outfile.close();
  ROS_INFO_STREAM("New file location is :" << dir + out_name + type);

  return 0;
}

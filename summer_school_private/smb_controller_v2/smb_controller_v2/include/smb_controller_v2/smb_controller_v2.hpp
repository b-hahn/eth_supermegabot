#pragma once

#include <chrono>
#include <mutex>

#include <boost/bind.hpp>
#include <geometry_msgs/Twist.h>
#include <glog/logging.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>

#include "SmbModes.hpp"
#include <smb_driver/SmbController.h>


namespace smb_controller_v2 {

class SmbControllerV2 {
public:
  SmbControllerV2();
  ~SmbControllerV2() = default;

  // init smb driver
  bool init(std::string port, ros::NodeHandle &nh, bool commandSmb);
  void run();
  void cleanup();
  void preCleanup();

  void
  wheelVelocityCallback(const std_msgs::Float32ConstPtr &right_wheel_velocity,
                        const std_msgs::Float32ConstPtr &left_wheel_velocity);
  void wheelVelocitiesCallback(
      const std_msgs::Float32MultiArrayConstPtr &wheel_velocities);
  void twistCallback(const geometry_msgs::TwistConstPtr& twist);
  void joystickCallback(const sensor_msgs::JoyConstPtr& joystick_input);

protected:
  // subscriber to SmbCommands
  // publisher for SmbReadings

  // SMB driver
  std::shared_ptr<smb_driver::SmbController> smb_;
  smb_common::SmbModeType lastControlMode_;

private:
    const double wheel_velocity_factor_ = 1000.0;
    std::mutex smbDriverMutex_;
    ros::Subscriber wheel_velocity_sub;
    ros::Subscriber wheel_twist_sub;
    ros::Subscriber joystick_sub;

};
} // namespace smb_controller_v2
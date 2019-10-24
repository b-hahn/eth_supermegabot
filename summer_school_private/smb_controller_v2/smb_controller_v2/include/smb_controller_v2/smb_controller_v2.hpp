#pragma once

#include <chrono>
#include <mutex>

// #include <smb_msgs/SmbCommands.h>
#include <glog/logging.h>
#include <ros/ros.h>

#include <boost/bind.hpp>
#include <geometry_msgs/Twist.h>

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

  void twistCallback(const geometry_msgs::TwistConstPtr& twist);

protected:
  // subscriber to SmbCommands
  // publisher for SmbReadings

  // SMB driver
  std::shared_ptr<smb_driver::SmbController> smb_;
  smb_common::SmbModeType lastControlMode_;

private:

    std::mutex smbDriverMutex_;

};
} // namespace smb_controller_v2
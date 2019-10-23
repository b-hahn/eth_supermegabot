#include <glog/logging.h>

// smb lowlevel controller
// #include "smb_lowlevel_controller/SmbLowLevelController.hpp"
#include <mutex>

// smb_common
// #include <smb_common/SmbCommands.hpp>
// #include <smb_common/SmbReadings.hpp>

// #include <smb_description/SmbDescription.hpp>
// #include <smb_model/SmbModel.hpp>
// #include <smb_msgs/SmbCommands.h>
// #include <smb_msgs/SmbReadings.h>
// // #include <any_measurements_ros/any_measurements_ros.hpp>
// #include <smb_driver/SmbController.h>

#include "smb_controller_v2/smb_controller_v2.hpp"


int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Starting smb controller v2...";

    std::string port = "/dev/ttySMB";

    ros::init(argc, argv, "smb_controller_v2");
    ros::NodeHandle nh;

    bool commandSmb;
    nh.param<bool>("command_smb",commandSmb, true);

    smb_controller_v2::SmbControllerV2 smb_controller;

    if (smb_controller.init(port, nh, commandSmb)) {
      smb_controller.run();
    }
    smb_controller.cleanup();

//   any_node::Nodewrap<smb_lowlevel_controller::SmbLowLevelController> node(argc, argv, "smb_lowlevel_controller", 2); // use 2 spinner thread
//   node.execute(); // update thread won't be executed
  return 0;
}

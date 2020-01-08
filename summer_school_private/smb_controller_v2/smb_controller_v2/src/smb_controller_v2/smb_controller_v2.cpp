#include "smb_controller_v2/smb_controller_v2.hpp"

namespace smb_controller_v2 {

SmbControllerV2::SmbControllerV2() {
    // do something?
}

bool SmbControllerV2::init(std::string port, ros::NodeHandle &nh,
                           bool commandSmb) {
  std::unique_lock<std::mutex> lock(smbDriverMutex_);

  lastControlMode_ = smb_common::SmbMode::FREEZE;

  try {
    // Wheel velocity message subscriber
    // TODO(ben): add sync for left & right msgs

    // message_filters::Subscriber<std_msgs::Float32> right_wheel_velocity_sub(nh, "right_wheel_velocity", 1);
    // message_filters::Subscriber<std_msgs::Float32> left_wheel_velocity_sub(nh, "left_wheel_velocity", 1);
    // message_filters::TimeSynchronizer<std_msgs::Float32, std_msgs::Float32>
    //     sync(right_wheel_velocity_sub, left_wheel_velocity_sub, 10);
    // sync.registerCallback(boost::bind(&SmbControllerV2::wheelVelocityCallback, this, _1, _2));

    boost::function<void(const std_msgs::Float32MultiArrayConstPtr &)> wheel_velocities_callback =
        boost::bind(&SmbControllerV2::wheelVelocitiesCallback, this, _1);
    wheel_velocity_sub = nh.subscribe("/wheel_velocities", 100, wheel_velocities_callback);

    // Twist message subscriber
    boost::function<void(const geometry_msgs::TwistConstPtr &)> twist_command_callback =
        boost::bind(&SmbControllerV2::twistCallback, this, _1);
    wheel_twist_sub = nh.subscribe("/wheelTwist", 100, twist_command_callback);
    
    // Joystick message subscriber
    boost::function<void(const sensor_msgs::JoyConstPtr &)> joystick_callback =
        boost::bind(&SmbControllerV2::joystickCallback, this, _1);
    joystick_sub = nh.subscribe("/joy", 100, joystick_callback);

    // Initializes communication to the smb
    smb_ =
        std::make_shared<smb_driver::SmbController>(port, nh, 10, commandSmb);
    smb_->startAcquisition();
    smb_->setMode(smb_driver::CLOSED_LOOP_SPEED);
    lastControlMode_ = smb_common::SmbMode::MODE_WHEEL_VELOCITY;


    LOG(INFO) << "[SmbControllerInterfaceInterface]: Initialized "
                 "successfully.";
  } catch (...) {
    LOG(WARNING)
        << "[SmbControllerInterface]: Caught an unspecified exception.";
    return false;
  }

  return true;
}

void SmbControllerV2::run()
{
    // std::chrono::time_point<std::chrono::steady_clock> initial_start, period_start;
    // std::chrono::seconds t_period(1l);
    // period_start = std::chrono::steady_clock::now();
    // initial_start = period_start;

    // while (true) {
    //   if (std::chrono::steady_clock::now() - period_start > t_period) {
    //     VLOG(4) << "[SmbControllerV2] run";
    //     double speed_l, speed_r, battery_voltage;
    //     smb_->getWheelSpeeds(speed_l, speed_r, 500);
    //     smb_->getBatteryVoltage(battery_voltage, 500);
    //     VLOG(2) << "Wheel speed l: " << speed_l << ", speed r: " << speed_r
    //             << ", battery voltage: " << battery_voltage << ".";
    //     period_start = std::chrono::steady_clock::now();
    //   }
    //   if (period_start - initial_start > std::chrono::seconds(3l)) {
    //     VLOG(4) << "Time since initial_start: "
    //             << std::chrono::duration_cast<std::chrono::microseconds>(
    //                    period_start - initial_start)
    //                    .count()
    //             << ". ";
    //     break;
    //   }
    // }

    // smb_->setVelocity(50);
    // sleepms(3000);
    // smb_->setVelocity(0);


    // VLOG(4) << "Finished while loop.";
    VLOG(4) << "starting spin...";
    ros::spin();
    VLOG(4) << "ending spin...";
}

// callback for twist messages that send a velocity command to wheels
void SmbControllerV2::wheelVelocityCallback(
    const std_msgs::Float32ConstPtr &right_wheel_velocity,
    const std_msgs::Float32ConstPtr &left_wheel_velocity) {
  VLOG(4) << "wheelVelocityCallback - right_wheel_velolcity: "
          << *right_wheel_velocity
          << ", left_wheel_velocity: " << *left_wheel_velocity;

  // 0: BOTH_MOTORS, 1: FIRST_MOTOR, 2: SECOND_MOTOR
  smb_->setVelocity(right_wheel_velocity->data * wheel_velocity_factor_, 1);
  smb_->setVelocity(left_wheel_velocity->data * wheel_velocity_factor_, 2);

}

// callback for twist messages that send a velocity command to wheels
void SmbControllerV2::wheelVelocitiesCallback(
    const std_msgs::Float32MultiArrayConstPtr &wheel_velocities) {
  VLOG(4) << "wheelVelocityCallback - right_wheel_velolcity: "
          << wheel_velocities->data[0]
          << ", left_wheel_velocity: " << wheel_velocities->data[1];

  // 0: BOTH_MOTORS, 1: FIRST_MOTOR, 2: SECOND_MOTOR
  // TODO(ben): limit speed of motors?
  smb_->setVelocity(wheel_velocities->data[0] * wheel_velocity_factor_, 1);
  smb_->setVelocity(wheel_velocities->data[0] * wheel_velocity_factor_, 2);

}

// callback for twist messages that send a velocity command to wheels
void SmbControllerV2::twistCallback(const geometry_msgs::TwistConstPtr& twist) {
  VLOG(4) << "twistCallback - currently not ACTIVE!";

  // TODO(ben): figure out correct way to convert ROS Twist messages to a
  // angular velocity. Not sure what the -1k - 1k range stands for. rad/s?

  // TODO(ben): read in robot parameters from urdf file
  // constexpr double ws = 0.265 * 2;
  // const double lwr = 0.19;

  //   // Compute wheels velocities:
  //   const double vel_left  = (curr_cmd.lin - curr_cmd.ang * ws / 2.0)/lwr;
  //   const double vel_right = (curr_cmd.lin + curr_cmd.ang * ws / 2.0)/rwr;
  
  // //TODO(ben): check how 0/1 motor numbers map to left and right. 
  // smb_->setVelocity(vel_left, 0);
  // smb_->setVelocity(vel_right, 1);
  // sleepms(5000);
  // smb_->setVelocity(0);
}

// callback for joy messages that send a velocity command to wheels
void SmbControllerV2::joystickCallback(const sensor_msgs::JoyConstPtr& joystick_input) {
  VLOG(4) << "joystick Callback!";

  // TODO(ben): figure out correct way to convert ROS Twist messages to a
  // angular velocity. Not sure what the -1k - 1k range stands for. rad/s?
  double joystick_to_rad_pers_s_factor = 50.0;
  smb_->setVelocity(joystick_input->axes[1] * joystick_to_rad_pers_s_factor);
  // sleepms(5000);
  // smb_->setVelocity(0);
}

void SmbControllerV2::preCleanup()
{
    VLOG(4) << "[SmbControllerV2] pre-cleanup";
//   SmbControllerV2Interface_.preCleanup();

//   stopUpdating_ = true;
//   cvUpdate_.notify_all();
//   syncMaster_->stop(true);
}

void SmbControllerV2::cleanup()
{
    VLOG(4) << "[SmbControllerV2] cleanup";
//   SmbControllerV2Interface_.cleanup();

//   try
//   {
//     signal_logger::logger->cleanup();

//     MELO_INFO_STREAM("[SmbControllerV2]: Cleaned up successfully.");
//   }
//   catch (message_logger::log::melo_fatal& exception)
//   {
//     MELO_ERROR_STREAM("[SmbControllerV2]: Caught a message logger exception: " << exception.what());
//   }
//   catch (std::exception& exception)
//   {
//     MELO_ERROR_STREAM("[SmbControllerV2]: Caught a standard exception: " << exception.what());
//   }
//   catch (...)
//   {
//     MELO_ERROR_STREAM("[SmbControllerV2]: Caught an unspecified exception.");
//   }
}


}
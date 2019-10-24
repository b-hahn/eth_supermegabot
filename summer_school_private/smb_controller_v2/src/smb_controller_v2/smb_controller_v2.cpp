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
    std::chrono::time_point<std::chrono::steady_clock> initial_start, period_start;
    std::chrono::seconds t_period(1l);
    period_start = std::chrono::steady_clock::now();
    initial_start = period_start;

    while (true) {
      if (std::chrono::steady_clock::now() - period_start > t_period) {
        VLOG(4) << "[SmbControllerV2] run";
        double speed_l, speed_r, battery_voltage;
        smb_->getWheelSpeeds(speed_l, speed_r, 500000);
        smb_->getBatteryVoltage(battery_voltage, 500000);
        VLOG(2) << "Wheel speed l: " << speed_l << ", speed r: " << speed_r
                << ", battery voltage: " << battery_voltage << ".";
        period_start = std::chrono::steady_clock::now();
      }
      if (period_start - initial_start > std::chrono::seconds(10l)) {
        VLOG(4) << "Time since initial_start: "
                << std::chrono::duration_cast<std::chrono::microseconds>(
                       period_start - initial_start)
                       .count()
                << ". ";
        break;
      }
    }
    VLOG(4) << "Finished while loop.";
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
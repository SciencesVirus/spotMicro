// Declaration file 

#pragma once //designed to include the current source file only once in a single compilation.
#ifndef SPOT_MICRO_MOTION_CMD //usd for conditional compiling.
#define SPOT_MICRO_MOTION_CMD
#include <ros/ros.h>
#include "std_msgs/Bool.h"

#include "i2cpwm_board/Servo.h"
#include "i2cpwm_board/ServoArray.h"

#include "command.h"
#include "spot_micro_kinematics/spot_micro_kinematics.h"
#include "spot_micro_state.h"

// Define a configuration struct
// To hold configuration parameters from parameter server/config file
struct SpotMicroNodeConfig {
  smk::SpotMicroConfig smc;
  float default_stand_height;
  float lie_down_height;
  int num_servos;
  float servo_max_angle_deg;
  std::map<std::string, std::map<std::string, float>> servo_config;
};


/* defining the class */
class SpotMicroMotionCmd
{

 public:
  SpotMicroMotionCmd(ros::NodeHandle &nh, ros::NodeHandle &pnh); //constructor method
  ~SpotMicroMotionCmd(); // distructor method
  void runOnce(); // runOnce method to control the flow of program

  // Publish a servo configuration message
  bool publishServoConfiguration();

  // Set servo proprotional message data
  void setServoCommandMessageData();

  // Publishes a servo proportional command message
  void publishServoProportionalCommand(); 

  // Publishes a servo absolute command message with all servos set to a command
  // value of 0. This effectively disables the servos (stops them from holding
  // position, should just freewheel)
  void publishZeroServoAbsoluteCommand();

 private:
  // Declare SpotMicroState a friend so it can access and modify private
  // members of this class
  friend class SpotMicroState;

  // Pointer to state object
  std::unique_ptr<SpotMicroState> state_;

  // Command object for encapsulating external commands
  Command cmd_; // Command object, encapsulate commands

  // Spot Micro Kinematics object. Holds kinematic state of robot, and holds
  // kinematics operations for setting position/orientation of the robot
  smk::SpotMicroKinematics sm_; 

  // Spot Micro Node Config object
  SpotMicroNodeConfig smnc_;

  // Holds the body state to be commanded: feet position, body position and
  // angles
  smk::BodyState body_state_cmd_; 

  // Map to hold servo command values in radians
  std::map<std::string, float> servo_cmds_rad_ = { {"RF_3", 0.0f}, {"RF_2", 0.0f}, {"RF_1", 0.0f},
                                                   {"RB_3", 0.0f}, {"RB_2", 0.0f}, {"RB_1", 0.0f},
                                                   {"LB_3", 0.0f}, {"LB_2", 0.0f}, {"LB_1", 0.0f},
                                                   {"LF_3", 0.0f}, {"LF_2", 0.0f}, {"LF_1", 0.0f} };

  // Reads parameters from parameter server to initialize spot micro node config
  // struct
  void readInConfigParameters();

  // Servo array message for servo proportional command
  i2cpwm_board::ServoArray servo_array_;

  // Servo array message for servo absolute command
  i2cpwm_board::ServoArray servo_array_absolute_;


  // Ros publisher and subscriber handles
  ros::NodeHandle nh_; // Defining the ros NodeHandle variable for registrating the same with the master
  ros::NodeHandle pnh_; // Private version of node handle
  ros::Subscriber stand_sub_; // ros subscriber handle for stand_cmd topic
  ros::Subscriber idle_sub_; // ros subscriber handle for idle_cmd topic
  ros::Subscriber walk_sub_;
  ros::Subscriber speed_cmd_sub_;
  ros::Subscriber position_cmd_sub_;
  ros::Subscriber body_rate_cmd_sub_;
  ros::Subscriber body_angle_cmd_sub_;
  ros::Publisher servos_absolute_pub_;
  ros::Publisher servos_proportional_pub_; 
  ros::ServiceClient servos_config_client_;

  // Callback method for stand command
  void standCommandCallback(const std_msgs::Bool::ConstPtr& msg);

  // Callback method for idle command
  void idleCommandCallback(const std_msgs::Bool::ConstPtr& msg);

  // Callback method for walk command
  void walkCommandCallback(const std_msgs::Bool::ConstPtr& msg);

  // Resets all events if they were true
  void resetEventCommands();
  

  // State Machine Related Methods
  // Handle input commands, delegate to state machine
  void handleInputCommands();

  // Changes state of the state machine
  void changeState(std::unique_ptr<SpotMicroState> sms);

};
#endif  

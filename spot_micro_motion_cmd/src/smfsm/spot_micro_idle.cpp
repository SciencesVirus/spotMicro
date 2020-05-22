#include "spot_micro_idle.h"
#include "spot_micro_motion_cmd.h"
#include "spot_micro_transition_stand.h"

SpotMicroIdleState::SpotMicroIdleState() {
  // Construcotr, doesn't need to do anything, for now...
  std::cout << "SpotMicroIdleState Ctor" << std::endl;
}

SpotMicroIdleState::~SpotMicroIdleState() {
  std::cout << "SpotMicroIdleState Dtor" << std::endl;
}

void SpotMicroIdleState::handleInputCommands(SpotMicroMotionCmd* smmc,
                                             const smk::BodyState& body_state,
                                             const SpotMicroNodeConfig& smnc,
                                             const Command& cmd) {
  std::cout << "In Spot Micro Idle State" << std::endl;
  
  // Check if stand command issued, if so, transition to stand state
  if (cmd.getStandCmd() == true) {
    changeState(smmc, std::make_unique<SpotMicroTransitionStandState>());
  
  } else {
    // Otherwise, just command idle servo commands
    smmc->publishZeroServoAbsoluteCommand();
  }

}


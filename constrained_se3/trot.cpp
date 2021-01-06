#include "trot.h"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <array>
#include <iostream>
#include <tuple>

#include "collision.h"
#include "eigen_helpers.h"
#include "estimation/estimation.h"
#include "force_allocator_3d.h"
#include "geometry.h"
#include "hive/logger/macros.h"
#include "lemo.h"
#include "lemo_consts.h"
#include "scm_config_reader.h"
#include "stream_helpers.h"
#include "time_capsule.h"
#include "utils.h"
#include "virtual_model_control.h"

namespace {
constexpr double MAX_RATE = 0.25;
const std::string GAIT_NAME = "trot";
}  // namespace

// A \NOTE on frame usage in the Trot behavior:
//
//  (1) World/Inertial frame (denoted as 'w') - This is the traditional world frame as meaured by
//      the IMU/Odometry.
//  (2) Body frame (denoted as 'b') - This is the traditional world frame, a body-fixed reference
//      frame located at the origin of the base link and oriented with the base link of the robot.
//  (3) Desired yaw frame (denoted as 'yaw_d') - This is a frame that is located coincident with the
//      body frame, but with the z-axis aligned with gravity and the yaw of the frame equal to the
//      desired yaw of the base of the robot.
//
Trot::Trot() {
  char const* robot_model_path = getenv("ROBOT_MODEL_PATH");
  if (robot_model_path == nullptr) {
    // Print to std::cout as well as to the log so the message isn't missed by the user.
    ROBOHIVE_LOG_CRITICAL("ROBOT_MODEL_PATH not set");
    std::cout << "ROBOT_MODEL_PATH not set" << std::endl;
    std::terminate();
  }

  char const* robot_type = getenv("ROBOT_TYPE");
  if (robot_type == nullptr) {
    // Print to std::cout as well as to the log so the message isn't missed by the user.
    ROBOHIVE_LOG_CRITICAL("ROBOT_TYPE not set");
    std::cout << "ROBOT_TYPE not set" << std::endl;
    std::terminate();
  }

  std::string urdf_filename = std::string(robot_model_path) + "/" + robot_type + ".urdf";
  fk_d_ = std::make_unique<lemo::kinematics::FKMath>(urdf_filename);

  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    leg_ik_[leg] =
        std::make_unique<lemo::kinematics::LegIK>(lemo::kinematics::LegIKParams(*fk_d_, leg));
  }

  lemo::Lumberjack<lemo::LOG_BUFFER_SIZE>& lj = lemo::lemoLoggers[lemo::LOG500HZ];
  lj.registerColumn(GAIT_NAME + ".height_d", &height_d_);
  lj.registerColumn(GAIT_NAME + ".p_com", &p_com_, lemo::log_helpers::cartesian);
  for (size_t i = 0; i < 3; ++i) {
    lj.registerColumn(GAIT_NAME + ".dp_body_cmd." + lemo::log_helpers::cartesian[i],
                      &dp_body_cmd_[i]);
    lj.registerColumn(GAIT_NAME + ".dp_body_d." + lemo::log_helpers::cartesian[i],
                      &dp_body_des_[i]);
  }
  lj.registerColumn(GAIT_NAME + ".yaw_d", &yaw_d_);
  lj.registerColumn(GAIT_NAME + ".yaw_rate_cmd", &yaw_rate_cmd_);
  lj.registerColumn(GAIT_NAME + ".yaw_rate_d", &yaw_rate_d_);

  lj.registerColumn(GAIT_NAME + ".t_stance_elapsed", &t_stance_elapsed_);
  lj.registerColumn(GAIT_NAME + ".stance_phase", &stance_phase_);
  lj.registerColumn(GAIT_NAME + ".p_virtual_foot", &p_virtual_foot_rt_b_ewrt_yaw_d_,
                    lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".p_stepout_offset", &p_stepout_offset_,
                    lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".p_stepout_hip_offset", &p_stepout_hip_offset_);

  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    const auto lname = GAIT_NAME + "." + lemo::K::leg_names[leg];
    lj.registerColumn(lname + ".f_ft_ewrt_b_d", &f_ft_ewrt_b_d_[leg], lemo::log_helpers::cartesian);

    for (size_t i = 0; i < 3; ++i) {
      lj.registerColumn(lname + ".p_ft_final_rt_b_ewrt_yaw_d." + lemo::log_helpers::cartesian[i],
                        &p_swing_final_rt_b_ewrt_yaw_d_[leg][i]);
    }

    lj.registerColumn(lname + ".possible_collision", &possible_collision_[leg]);
    lj.registerColumn(lname + ".ik_status", &leg_ik_status_[leg]);
  }

  lj.registerColumn(GAIT_NAME + ".vmc.pos", &vmc_input_.pos, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".vmc.pos_d", &vmc_input_.pos_d, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".vmc.vel", &vmc_input_.vel, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".vmc.vel_d", &vmc_input_.vel_d, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".vmc.F_ff", &vmc_input_.force_ff, lemo::log_helpers::cartesian);

  // TODO(rose@)(2020/05/20) Add a `registerColumn` specialization to Lumberjack for quaternions.
  lj.registerColumn(GAIT_NAME + ".vmc.quat", &vmc_input_.quat.coeffs(), lemo::log_helpers::quat);
  lj.registerColumn(GAIT_NAME + ".vmc.quat_d", &vmc_input_.quat_d.coeffs(),
                    lemo::log_helpers::quat);
  lj.registerColumn(GAIT_NAME + ".vmc.omega", &vmc_input_.omega, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".vmc.omega_d", &vmc_input_.omega_d, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".vmc.M_ff", &vmc_input_.moment_ff, lemo::log_helpers::cartesian);

  lj.registerColumn(GAIT_NAME + ".body_force", &body_force_, lemo::log_helpers::cartesian);
  lj.registerColumn(GAIT_NAME + ".body_moment", &body_moment_, lemo::log_helpers::cartesian);

  fa3d_.addVarsToLog(GAIT_NAME + ".fa3d", lj);

  lj.registerColumn(GAIT_NAME + ".v_lip_final", &v_lip_final_, lemo::log_helpers::cartesian);
}

void Trot::init() {
  EIGEN_MALLOC_NOT_ALLOWED
  getConfig();

  // Calculate the value of mid_hip_dist. We use fk_d_ here because it's available and should
  // provide the same results as the FKMath object in the RobotData object.
  // Front mid hip point
  mid_hip_dist_[0] = 0.5 * (fk_d_->jointOffsetFromParent(lemo::K::FL, lemo::K::HX) +
                            fk_d_->jointOffsetFromParent(lemo::K::FR, lemo::K::HX))
                               .x();
  // Rear mid hip point
  mid_hip_dist_[1] = 0.5 * (fk_d_->jointOffsetFromParent(lemo::K::HL, lemo::K::HX) +
                            fk_d_->jointOffsetFromParent(lemo::K::HR, lemo::K::HX))
                               .x();
}

void Trot::setArg(size_t group, size_t idx, double value) {
  switch (group) {
    case CMD_LIN_VEL:
      if (idx >= 0 && idx < 2) {
        dp_body_cmd_[idx] = value;
      }
      break;
    case CMD_ANG_VEL:
      if (idx == 2) {
        yaw_rate_cmd_ = value;
      }
      break;
    case CMD_HEIGHT:
      height_d_ = value;
      break;
    default:
      ROBOHIVE_LOG_WARN("Received unexpected user command - group: " << group << ", idx: " << idx
                                                                     << ", value: " << value);
  }
}

auto Trot::enter(LemoWrapper& w) -> bool {
  ROBOHIVE_LOG_NOTE("ENTER " << GAIT_NAME);

  Eigen::Quaterniond w_R_b = lemo::utils::eigenVectorToQuaternion(w.data_.q.segment(3, 4));

  // Initialize the desired yaw to the measured yaw.
  yaw_d_ = lemo::utils::yawFromQuaternion(w_R_b);
  ROBOHIVE_LOG_DEBUG("Desired yaw: " << yaw_d_);

  dp_body_des_.fill(0);

  // Initialise first swing legs (a diagonal set of leg pairs)
  is_swing_leg_[lemo::K::FR] = was_swinging_[lemo::K::FR] = true;
  is_swing_leg_[lemo::K::HL] = was_swinging_[lemo::K::HL] = true;

  // Setup swing leg objects
  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    dp_swing_init_rt_b_ewrt_yaw_d_[leg] = { -dp_body_des_[0], -dp_body_des_[1], -dp_body_des_[2] };
    ddp_swing_init_rt_b_ewrt_yaw_d_[leg].fill(0);
    // Setup final arrays
    dp_swing_final_rt_b_ewrt_yaw_d_[leg] = { -dp_body_des_[0], -dp_body_des_[1], ground_seek_vel_ };
    ddp_swing_final_rt_b_ewrt_yaw_d_[leg].fill(0);

    swing_legs_[leg].setSwingHeight(swing_height_);
    swing_legs_[leg].setSwingDuration(swing_duration_);
  }

  // Setup weights for the allocator:
  fa3d_.setForceWeight(fa3d_Q_force_);
  fa3d_.setMomentWeight(fa3d_Q_moment_);
  fa3d_.setHipTorqueWeight(fa3d_Q_min_torque_);

  // TODO(rose@): Remove this line once the update ordering bug has been fixed in LemoWrapper.
  w.bypass = true;

  return true;
}

void Trot::update(float delta_time, const lemo::RobotState& state, const lemo::RobotData& rbt_data,
                  lemo::RobotCommand& cmd, ControllerData& ctrl_data) {
  SO3<World, Base> wRb{ rbt_data.pos_acc_observer.getPose().orientation };
  const Eigen::Quaterniond yaw_d_Q_w = lemo::utils::quaternionFromYaw(-yaw_d_);
  const Eigen::Quaterniond yaw_d_Q_b = yaw_d_Q_w * w_Q_b;

  // Update the Z position to be height above the ground.

  P<Base> p_com_b{ rbt_data.forward_kinematics->getBodyRelativeCOMPosition() };
  P<World> w_p_com_b = wRb * p_com_b;
  double ground_height = lemo::estimation::estimateRelativeGroundHeight(
      w_Q_b, *rbt_data.forward_kinematics, rbt_data.contact_state);
  p_com_[2] = w_p_com_b[2] - ground_height;

  // \TODO(rose@)(2020/05/18) Move this to 'enter' once the RobotData is available there.
  if (needs_init_) {
    t_stance_start_ = ctrl_data.t;
    for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
      // Setup init arrays
      P<Base> p_ft = rbt_data.forward_kinematics->getBodyRelativeFootPosition(leg);
      const auto& p_ft_yaw = yaw_d_Q_b * p_ft;
      dp_ft_b = rbt_data.forward_kinematics->getBodyRelativeFootVelocity(leg);
      const auto& dp_ft_yaw = yaw_d_Q_b*dp_ft_b;
      for (size_t i = 0; i < 3; ++i) {
        // Initialise reference arrays as initial leg position
        ctrl_data.p_ft_rt_b_d[leg][i] =
            rbt_data.forward_kinematics->getBodyRelativeFootPosition(leg)[i];
        ctrl_data.dp_ft_rt_b_d[leg][i] =
            rbt_data.forward_kinematics->getBodyRelativeFootVelocity(leg)[i];
      }
      ctrl_data.ddp_ft_rt_b_d[leg].fill(0);
      initializeSwingLeg(leg, ctrl_data.t, p_ft_yaw, dp_ft_yaw);
    }
    ctrl_data.p_com_d = p_com_;
    dp_body_des_.fill(0);
    needs_init_ = false;
  }
  t_stance_elapsed_ = ctrl_data.t - t_stance_start_;
  stance_phase_ = t_stance_elapsed_ / swing_duration_;  // use 'swing_duration_' because
                                                        // swing/stance durations are equivalant.

  handleUserInputs(delta_time, ctrl_data);

  calculateVirtualFootTarget(rbt_data);

  // Do this before or after `is_swing_leg_` is used, but never in the middle.
  checkForStateChange(rbt_data);

  // Iterate over all legs and compute a touchdown position for each foot from the virtual foot
  // position
  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    const bool front = lemo::K::isFront(leg);
    const bool left = lemo::K::isLeft(leg);
    // Setup specific leg stance position
    const double width = stance_width_ * (left ? 1 : -1);
    const double length =
        front ? (mid_hip_dist_[0] + x_foot_offset_) : (mid_hip_dist_[1] - x_foot_offset_);

    // Check if this leg is in a swing phase
    if (is_swing_leg_[leg]) {
      // Set final foot position depending on calculated virtual foot and desired stance
      // positions
      p_swing_final[leg][0] = p_virtual_foot[0] + length;
      p_swing_final[leg][1] =
          p_virtual_foot[1] + width + length * p_stepout_hip_offset_;

      // Set final z position to ground height
      // NOTE: Ground height is from the body to the bottom of the foot (accounting for foot
      // radius). The foot frame for the swing trajectories is at the foot center, so we need to
      // offset the desired final height by the foot radius.
      p_swing_final[leg][2] = rbt_data.pz_ground_rt_b_ewrt_w;

      // Set the desired touchdown velocity.
      dp_swing_final_rt_b_ewrt_yaw_d_[leg] = { -dp_body_des_[0], -dp_body_des_[1],
                                               ground_seek_vel_ };
    }
  }

  // If necessary, modify 'p_swing_final' for the swing legs in order to avoid
  // foot-foot collisions.
  avoidSwingStanceFootCollisions(yaw_d_Q_b, rbt_data);

  // Iterate over legs and update reference positions, do IK, or update initial swing positions for
  // current stance legs.
  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    if (is_swing_leg_[leg]) {
      // Call set destination to final position
      swing_legs_[leg].setDestination(p_swing_final_rt_b_ewrt_yaw_d_[leg],
                                      dp_swing_final_rt_b_ewrt_yaw_d_[leg],
                                      ddp_swing_final_rt_b_ewrt_yaw_d_[leg], ctrl_data.t);

      // Compute leg references
      swing_legs_[leg].compute(ctrl_data.t);

      // Only update the swing leg if the trajectory hasn't finished.
      // TODO(rose@): Handle the case where the swing trajectory has elapsed but the foot hasn't
      //              touched down yet.
      if (!swing_legs_[leg].finished()) {
        doSwingLegIK(yaw_d_Q_b, leg, ctrl_data);
      }
    } else {
      // Continue to update swing trajectory object for next phase to make sure initial positions
      // are always up to date
      const auto p_ft_rt_b_ewrt_yw =
          yaw_d_Q_b * rbt_data.forward_kinematics->getBodyRelativeFootPosition(leg);
      const auto dp_ft_rt_b_ewrt_yw =
          yaw_d_Q_b * rbt_data.forward_kinematics->getBodyRelativeFootVelocity(leg);
      initializeSwingLeg(leg, ctrl_data.t, p_ft_rt_b_ewrt_yw, dp_ft_rt_b_ewrt_yw);
    }
  }

  runAllocator(rbt_data, ctrl_data);

  // \TODO(rose@)(2020/05/18) This is only here so that behaviors after this behavior will work.
  // We use the `p_foot_rt_w_` variables to set the desired foot reference positions so that any
  // TSID base behavior transitioned into after this behavior will work properly. Remove this once
  // other behaviors setup their own state appropriately.
  for (int leg = 0; leg < lemo::K::N_LEG; ++leg) {
    for (size_t i = 0; i < 3; ++i) {
      ctrl_data.p_ft_rt_w_d[leg][i] = ctrl_data.p_ft_rt_w[leg][i];
    }
    ctrl_data.dp_ft_rt_w_d[leg].fill(0);
    ctrl_data.ddp_ft_rt_w_d[leg].fill(0);
  }

  // Here we'll calculate the measured force and moment on the body:
  body_force_ = Eigen::Vector3d::Zero();
  body_moment_ = Eigen::Vector3d::Zero();
  for (int leg = 0; leg < lemo::K::N_LEG; ++leg) {
    const Eigen::Vector3d f_ft_ewrt_yaw_d =
        yaw_d_Q_b * rbt_data.foot_force_est.getFootForceEstimateEwrtBody(leg);
    body_force_ += f_ft_ewrt_yaw_d;
    const Eigen::Vector3d p_ft_rt_b_ewrt_yaw_d =
        yaw_d_Q_b * rbt_data.forward_kinematics->getBodyRelativeFootPosition(leg);
    body_moment_ += p_ft_rt_b_ewrt_yaw_d.cross(f_ft_ewrt_yaw_d);
  }

  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    for (size_t ldof = 0; ldof < lemo::K::N_LDOF; ++ldof) {
      const auto dof = lemo::K::dofFromLegDof(leg, ldof);
      // Check if swing leg or stance leg so that we can use position or torque control
      if (is_swing_leg_[leg]) {
        // Position control
        cmd.joint[dof].q_d = joint_d_[dof].q;
        cmd.joint[dof].dq_d = joint_d_[dof].dq;
        cmd.joint[dof].tau_d = tau_d_[dof];
        cmd.gains[dof].k_q = Kp_joint_swing_[ldof];
        cmd.gains[dof].k_dq = Kd_joint_swing_[ldof];
      } else {
        // Torque control
        cmd.joint[dof].q_d = state.joint[dof].q;
        // \note(rose): Setting the desired joint velocity to zero in combination with a small Kd
        // gain provides a bit of damping to the joints (but doesn't seem strictly necessary).
        cmd.joint[dof].dq_d = 0;
        cmd.joint[dof].tau_d = tau_d_[dof];
        cmd.gains[dof].k_q = Kp_joint_stance_[ldof];
        cmd.gains[dof].k_dq = Kd_joint_stance_[ldof];
      }
    }
  }
  cmd.is_valid = true;
}

auto Trot::update(LemoWrapper& w) -> bool {
  // Set the joint targets
  w.bypass = true;

  return false;
}

void Trot::getConfig() {
  // These are more generic parameters and not associated with a behavior:
  lemo::scm_config_reader::read("desired_height", height_d_);
  lemo::scm_config_reader::read("swing_height", swing_height_);
  dp_body_cmd_ = lemo::scm_config_reader::read<std::array<double, 3>>("dp_body_cmd");
  lemo::scm_config_reader::read("yaw_rate_cmd", yaw_rate_cmd_);

  // These parameters are behavior specific:
  lemo::scm_config_reader::read(GAIT_NAME + ".com_vel_max", com_vel_max_);
  lemo::scm_config_reader::read(GAIT_NAME + ".swing_duration", swing_duration_);
  lemo::scm_config_reader::read(GAIT_NAME + ".K_stepout", K_stepout_);
  lemo::scm_config_reader::read(GAIT_NAME + ".K_body_vel_i", K_body_vel_i_);
  lemo::scm_config_reader::read(GAIT_NAME + ".stance_width", stance_width_);
  lemo::scm_config_reader::read(GAIT_NAME + ".x_foot_offset", x_foot_offset_);

  lemo::scm_config_reader::read(GAIT_NAME + ".ground_seek_velocity", ground_seek_vel_);
  if (ground_seek_vel_ > 0) {
    ROBOHIVE_LOG_WARN("'ground_seek_velocity' expected to be <= 0.");
    ground_seek_vel_ = 0;
  }
  lemo::scm_config_reader::read(GAIT_NAME + ".minimum_liftoff_velocity", min_liftoff_vel_);
  if (min_liftoff_vel_ < 0) {
    ROBOHIVE_LOG_WARN("'minimum_liftoff_velocity' expected to be >= 0.");
    min_liftoff_vel_ = 0;
  }

  lemo::scm_config_reader::read(GAIT_NAME + ".contact_detection_blackout_phase",
                                td_detect_phase_delay_);

  lemo::scm_config_reader::read(GAIT_NAME + ".body_velocity_rate_limit", body_velocity_rate_limit_);
  lemo::scm_config_reader::read(GAIT_NAME + ".xd_max", dp_body_cmd_limit_[0].max);
  lemo::scm_config_reader::read(GAIT_NAME + ".xd_min", dp_body_cmd_limit_[0].min);
  const auto y_vel_limit = lemo::scm_config_reader::read<double>(GAIT_NAME + ".yd_max");
  dp_body_cmd_limit_[1] = lemo::utils::Limits(y_vel_limit);
  const auto yaw_rate_limit = lemo::scm_config_reader::read<double>(GAIT_NAME + ".yaw_rate_max");
  yaw_rate_cmd_limit_ = lemo::utils::Limits(yaw_rate_limit);

  lemo::scm_config_reader::read(GAIT_NAME + ".use_pos_acc_obs", use_pos_acc_obs_);
  lemo::scm_config_reader::read(GAIT_NAME + ".use_pos_acc_obs_for_stepout",
                                use_pos_acc_obs_for_stepout_);
  lemo::scm_config_reader::read(GAIT_NAME + ".use_measured_foot_velocity_for_liftoff",
                                use_measured_foot_velocity_for_liftoff_);

  {
    const std::string prefix = GAIT_NAME + ".fa3d";
    auto Q_force = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Q_force");
    fa3d_Q_force_ = Eigen::Map<Eigen::Vector3d>(Q_force.data());
    auto Q_moment = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Q_moment");
    fa3d_Q_moment_ = Eigen::Map<Eigen::Vector3d>(Q_moment.data());
    auto Q_min_torque = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Q_min_"
                                                                                      "torque");
    fa3d_Q_min_torque_ = Eigen::Map<Eigen::Vector3d>(Q_min_torque.data());
  }
  {
    const std::string prefix = GAIT_NAME + ".vmc";
    auto Kp_lin_vec = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Kp_lin");
    auto Kd_lin_vec = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Kd_lin");
    auto Kp_ang_vec = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Kp_ang");
    auto Kd_ang_vec = lemo::scm_config_reader::read<std::array<double, 3>>(prefix + ".Kd_ang");
    for (size_t i = 0; i < 3; ++i) {
      Kp_lin_[i] = Kp_lin_vec[i];
      Kd_lin_[i] = Kd_lin_vec[i];
      Kp_ang_[i] = Kp_ang_vec[i];
      Kd_ang_[i] = Kd_ang_vec[i];
    }
  }

  {
    const std::string prefix = GAIT_NAME + ".joint_swing_gains";
    lemo::scm_config_reader::read(prefix + ".Kp", Kp_joint_swing_);
    lemo::scm_config_reader::read(prefix + ".Kd", Kd_joint_swing_);
  }
  {
    const std::string prefix = GAIT_NAME + ".joint_stance_gains";
    lemo::scm_config_reader::read(prefix + ".Kp", Kp_joint_stance_);
    lemo::scm_config_reader::read(prefix + ".Kd", Kd_joint_stance_);
  }

  {
    const std::string prefix = GAIT_NAME + ".collision_buffer";
    lemo::scm_config_reader::read(prefix + ".longitudinal", foot_buffer_lon_);
    lemo::scm_config_reader::read(prefix + ".lateral", foot_buffer_lat_);
  }
}

void Trot::handleUserInputs(double delta_time, ControllerData& ctrl_data) {
  // We don't care about the absolute position in x & y, so set them equal to the measured values:
  ctrl_data.p_com_d.head(2) = ctrl_data.p_com.head(2);
  // Plan a trajectory from the current desired COM height to the target COM height
  std::tie(ctrl_data.p_com_d.z(), ctrl_data.dp_com_d.z()) =
      lemo::utils::slewRateLimit(ctrl_data.p_com_d.z(), height_d_, delta_time, com_vel_max_);
  // Rate limit the change in desired x & y velocity:
  for (int i = 0; i < 2; ++i) {
    dp_body_cmd_[i] = dp_body_cmd_limit_[i].clamp(dp_body_cmd_[i]);
    std::tie(dp_body_des_[i], ctrl_data.ddp_com_d[i]) = lemo::utils::slewRateLimit(
        dp_body_des_[i], dp_body_cmd_[i], delta_time, body_velocity_rate_limit_);

    ctrl_data.dp_com_d[i] = dp_body_des_[i];
  }
  // Zero out this unused value.
  ctrl_data.ddp_com_d.z() = 0;

  // Rate limit the commanded yaw rate into a desired yaw rate, then integrate the desired yaw rate
  // into the desired yaw.
  yaw_rate_cmd_ = yaw_rate_cmd_limit_.clamp(yaw_rate_cmd_);
  double ddyaw_d{ 0 };  // Unused but required for std::tie below.
  std::tie(yaw_rate_d_, ddyaw_d) =
      lemo::utils::slewRateLimit(yaw_rate_d_, yaw_rate_cmd_, delta_time, body_velocity_rate_limit_);
  yaw_d_ += yaw_rate_d_ * delta_time;
}

void Trot::calculateVirtualFootTarget(const lemo::RobotData& rbt_data) {
  // Calculate virtual foot position
  // \note(rose): Add a separate flag here for using the pos_acc_observer (vs kin_odom) rather than
  // switching on the `use_pos_acc_obs_` flag since the pos_acc_observer seems to have smoother x &
  // y velocity signals.
  const auto* pose_est =
      use_pos_acc_obs_for_stepout_ ?
          static_cast<const lemo::estimation::PoseEstimatorI*>(&rbt_data.pos_acc_observer) :
          &rbt_data.kin_pose_est;

  const Eigen::Quaterniond& w_Q_b = pose_est->getPose().orientation;
  const Eigen::Quaterniond yaw_d_Q_w = lemo::utils::quaternionFromYaw(-yaw_d_);
  const Eigen::Quaterniond yaw_d_Q_b = yaw_d_Q_w * w_Q_b;

  const Eigen::Vector3d& v_body_ewrt_yw_d = yaw_d_Q_w * pose_est->getTwist().linear;
  const Eigen::Vector3d v_body_d_ewrt_yw_d = Eigen::Map<Eigen::Vector3d>(dp_body_des_.data());
  p_virtual_foot =
      0.5 * swing_duration_ * v_body_ewrt_yw_d +
      K_stepout_ * ((sqrt(height_d_ / lemo::GRAVITY)) * (v_body_ewrt_yw_d - v_body_d_ewrt_yw_d)) +
      yaw_d_Q_b * rbt_data.forward_kinematics->getBodyRelativeCOMPosition();

  p_stepout_offset_.head(2) += K_body_vel_i_ * TimeCapsule::getInstance()->getDt() *
                               (v_body_ewrt_yw_d - v_body_d_ewrt_yw_d).head(2);
  p_virtual_foot += p_stepout_offset_;
  // Account for differential hip velocity due to yaw-rate command when computing the stepout
  // location.
  const Eigen::Vector3d omega_ewrt_w = w_Q_b * pose_est->getTwist().angular;
  p_stepout_hip_offset_ =
      0.5 * swing_duration_ * omega_ewrt_w[2] -
      K_stepout_ * sqrt(height_d_ / lemo::GRAVITY) * (yaw_rate_d_ - omega_ewrt_w[2]);
}

void Trot::checkForStateChange(const lemo::RobotData& rbt_data) {
  // Iterate over legs and check for state changes.
  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    // Update scheduling array if appropriate leg is finished
    if (is_swing_leg_[leg] && (swing_legs_[leg].finished() ||
                               // Don't allow touchdown detection to cause a state change for a
                               // brief period after the previous state change.
                               (stance_phase_ > td_detect_phase_delay_ &&
                                rbt_data.contact_state[leg].touchdownDetected()))) {
      is_swing_leg_[leg] = false;
    }
  }
  // If all feet are finished (none left in swing) switch stance feet to swing.
  const int sum = std::count(is_swing_leg_.begin(), is_swing_leg_.end(), true);
  if (sum == 0) {
    for (size_t i = 0; i < was_swinging_.size(); i++) {
      if (was_swinging_[i] == false) {
        is_swing_leg_[i] = true;
        was_swinging_[i] = true;
      } else {
        was_swinging_[i] = false;
      }
    }
    // The start of a new stance phase.
    t_stance_start_ = TimeCapsule::getInstance()->getRobotTime();
  }
}

void Trot::avoidSwingStanceFootCollisions(const Eigen::Quaterniond& yaw_d_Q_b,
                                          const lemo::RobotData& rbt_data) {
  const auto& fk = *rbt_data.forward_kinematics;
  possible_collision_.fill(0);  // reset logged values in the case of no collisions.

  Eigen::Vector3d p_body_delta = Eigen::Vector3d::Zero();
  {  // Predict the position of the stance feet at the end of swing
    const Eigen::Quaterniond& w_Q_b = rbt_data.kin_pose_est.getPose().orientation;
    const Eigen::Quaterniond yaw_d_Q_w = yaw_d_Q_b * w_Q_b.inverse();
    // predict the LIP dynamics forward
    const Eigen::Vector3d p_b_rt_cop_ewrt_w = -rbt_data.p_cop_rt_b_ewrt_w;
    const Eigen::Vector3d p_b_rt_cop_ewrt_yaw_d = yaw_d_Q_w * p_b_rt_cop_ewrt_w;

    const Eigen::Vector3d& v_body_ewrt_yw_d =
        yaw_d_Q_w * rbt_data.pos_acc_observer.getTwist().linear;

    const double t_stance_rem = swing_duration_ - t_stance_elapsed_;
    const auto [p_b_rt_cop_final, v_lip_final] = lemo::estimation::predictLIPDynamics(
        p_b_rt_cop_ewrt_yaw_d.head(2), v_body_ewrt_yw_d.head(2), height_d_, t_stance_rem);

    v_lip_final_.head(2) = v_lip_final;

    p_body_delta.head(2) = p_b_rt_cop_final - p_b_rt_cop_ewrt_yaw_d.head(2);
  }

  // Iterate over all possible leg pairs
  for (size_t leg1 = 0; leg1 < lemo::K::N_LEG; ++leg1) {
    for (size_t leg2 = leg1 + 1; leg2 < lemo::K::N_LEG; ++leg2) {
      const bool both_swing = is_swing_leg_[leg1] && is_swing_leg_[leg2];
      const bool both_stance = !(is_swing_leg_[leg1] || is_swing_leg_[leg2]);
      if (both_swing || both_stance) {
        // If both legs are in swing, assume they can't both be in the same bounding region (for
        // now). If both legs are in stance, nothing to do.
        continue;
      }
      // Figure out which of the two legs is the swing leg and which is the stance leg.
      const auto swing_leg = (is_swing_leg_[leg1] ? leg1 : leg2);
      const auto stance_leg = (!is_swing_leg_[leg1] ? leg1 : leg2);
      // We want to check the final foot position of the swing leg to see if it is in the bounding
      // region of the predicted position of the stance leg at the end of stance.
      const Eigen::Vector3d p_stance_leg =
          yaw_d_Q_b * fk.getBodyRelativeFootPosition(stance_leg) - p_body_delta;

      const auto stance_bb = lemo::collision::createFootBoundingBox(
          p_stance_leg, lemo::K::isLeft(stance_leg), foot_buffer_lon_, foot_buffer_lat_);
      const Eigen::Vector3d p_swing_final =
          Eigen::Vector3d(p_swing_final_rt_b_ewrt_yaw_d_[swing_leg].data());
      const bool in_bb = lemo::geometry::pointInPoly(p_swing_final.head(2), stance_bb);

      if (in_bb) {
        possible_collision_[swing_leg] = 1;
        // A collision was detected. Let's correct it
        const Eigen::Vector2d safe_step_loc =
            lemo::geometry::nearestPointOnPoly(p_swing_final.head(2), stance_bb);
        p_swing_final_rt_b_ewrt_yaw_d_[swing_leg][0] = safe_step_loc.x();
        p_swing_final_rt_b_ewrt_yaw_d_[swing_leg][1] = safe_step_loc.y();

        // Offset the diagonal leg (the other swing leg) by the same amount in order to preserve the
        // virtual foot location.
        const auto& diagonal_leg = lemo::K::diagonalLeg(swing_leg);
        p_swing_final_rt_b_ewrt_yaw_d_[diagonal_leg][0] -= (safe_step_loc.x() - p_swing_final.x());
        p_swing_final_rt_b_ewrt_yaw_d_[diagonal_leg][1] -= (safe_step_loc.y() - p_swing_final.y());
      }
    }
  }
}

void Trot::runAllocator(const lemo::RobotData& rbt_data, const ControllerData& ctrl_data) {
  const auto* pose_est =
      use_pos_acc_obs_ ?
          static_cast<const lemo::estimation::PoseEstimatorI*>(&rbt_data.pos_acc_observer) :
          &rbt_data.kin_pose_est;
  const Eigen::Quaterniond& w_Q_b = pose_est->getPose().orientation;
  const Eigen::Quaterniond yaw_d_Q_w = lemo::utils::quaternionFromYaw(-yaw_d_);
  const Eigen::Quaterniond yaw_d_Q_b = yaw_d_Q_w * w_Q_b;

  const lemo::kinematics::FKMath& fk = *rbt_data.forward_kinematics;

  // Set up the linear VMC inputs:
  vmc_input_.pos = yaw_d_Q_w * p_com_;
  vmc_input_.pos_d = yaw_d_Q_w * ctrl_data.p_com_d;
  vmc_input_.vel = yaw_d_Q_w * pose_est->getTwist().linear;
  vmc_input_.vel_d = yaw_d_Q_w * ctrl_data.dp_com_d;
  vmc_input_.force_ff = Eigen::Vector3d::UnitZ() * fk.getTotalMass() * lemo::GRAVITY;

  // Calculate desired body wrench:
  Eigen::Vector3d F_d_ewrt_yaw_d =
      lemo::control::VMC::computeLinAcc(vmc_input_.pos_d, vmc_input_.pos, vmc_input_.vel_d,
                                        vmc_input_.vel, Kp_lin_, Kd_lin_) +
      vmc_input_.force_ff;

  // Set up the angular VMC inputs:
  vmc_input_.quat = yaw_d_Q_b;
  vmc_input_.quat_d = Eigen::Quaterniond::Identity();
  vmc_input_.omega = yaw_d_Q_b * pose_est->getTwist().angular;
  vmc_input_.omega_d = Eigen::Vector3d(0, 0, yaw_rate_d_);
  vmc_input_.moment_ff = Eigen::Vector3d::Zero();

  Eigen::Vector3d M_d_ewrt_yaw_d =
      lemo::control::VMC::computeAngAcc(vmc_input_.quat_d, vmc_input_.quat, vmc_input_.omega_d,
                                        vmc_input_.omega, Kp_ang_, Kd_ang_) +
      vmc_input_.moment_ff;

  fa3d_.setDesiredWrench(F_d_ewrt_yaw_d, M_d_ewrt_yaw_d);

  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    fa3d_.setLegState(leg, yaw_d_Q_b * fk.getBodyRelativeFootPosition(leg),
                      yaw_d_Q_b * fk.getBodyRelativeJointPosition(leg, lemo::K::HX),
                      !is_swing_leg_[leg]);
  }
  fa3d_.allocate();

  // Convert the desired forces into joint torques.
  const Eigen::Quaterniond b_Q_yaw_d = yaw_d_Q_b.inverse();
  for (size_t leg = 0; leg < lemo::K::N_LEG; ++leg) {
    // Rotate output from world frame to body frame.
    f_ft_ewrt_b_d_[leg] = b_Q_yaw_d * fa3d_.getFootForce(leg);
    // Convert desired foot forces into desired joint torques.
    const Eigen::Vector3d tau_leg =
        -fk.calculateFootTranslationalJacobian(leg).transpose() * f_ft_ewrt_b_d_[leg];
    // Copy desired joint torques into torque array (used in native update call).
    for (size_t ldof = 0; ldof < lemo::K::N_LDOF; ++ldof) {
      tau_d_.at(lemo::K::dofFromLegDof(leg, ldof)) = tau_leg[ldof];
    }
  }
}

void Trot::doSwingLegIK(const Eigen::Quaterniond& yaw_d_Q_b, int leg, ControllerData& ctrl_data) {
  // Get swing leg trajectory references.
  const auto p_ft_rt_b_ewrt_yaw_d = swing_legs_[leg].getPosReference();
  const auto dp_ft_rt_b_ewrt_yaw_d = swing_legs_[leg].getVelReference();
  const auto ddp_ft_rt_b_ewrt_yaw_d = swing_legs_[leg].getAccReference();

  // Convert all references from the 'yaw_d' frame to the 'body' frame.
  const Eigen::Quaterniond b_Q_yaw_d = yaw_d_Q_b.inverse();
  const Eigen::Vector3d p_ft_rt_b_d =
      b_Q_yaw_d * Eigen::Map<const Eigen::Vector3d>(p_ft_rt_b_ewrt_yaw_d.data());
  const Eigen::Vector3d dp_ft_rt_b_d =
      b_Q_yaw_d * Eigen::Map<const Eigen::Vector3d>(dp_ft_rt_b_ewrt_yaw_d.data());
  const Eigen::Vector3d ddp_ft_rt_b_d =
      b_Q_yaw_d * Eigen::Map<const Eigen::Vector3d>(ddp_ft_rt_b_ewrt_yaw_d.data());

  // Copy the foot references expressed in the body frame into ctrl_data for logging.
  for (size_t i = 0; i < 3; ++i) {
    ctrl_data.p_ft_rt_b_d[leg][i] = p_ft_rt_b_d[i];
    ctrl_data.dp_ft_rt_b_d[leg][i] = dp_ft_rt_b_d[i];
    ctrl_data.ddp_ft_rt_b_d[leg][i] = ddp_ft_rt_b_d[i];
  }

  // Solve the IK problem to get joint angles.
  std::array<double, lemo::K::N_LDOF> q_d_out = { 0 };
  leg_ik_status_[leg] = leg_ik_[leg]->solve(p_ft_rt_b_d, q_d_out);
  ROBOHIVE_LOG_WARN_IF(leg_ik_status_[leg] != 0, "Bad IK status ("
                                                     << leg_ik_status_[leg] << ") for "
                                                     << lemo::K::leg_names[leg] << "leg.");

  // Setup "input" structure for the FKMath object with the joint angles from the IK output. We only
  // care about one leg here so we can get the Jacobian for that leg.
  // TODO(rose@): Solve the IK for all swing legs, then update the forward kinematics in order to
  // improve efficiency
  if (leg_ik_status_[leg] == 0) {
    for (size_t ldof = 0; ldof < lemo::K::N_LDOF; ++ldof) {
      const auto dof = lemo::K::dofFromLegDof(leg, ldof);
      input_.q[dof] = q_d_out[ldof];
    }
    fk_d_->update(input_);

    // Use Jacobian to map from desired foot velocity to desired joint velocities
    const Eigen::Vector3d dq_d =
        fk_d_->calculateFootTranslationalJacobian(leg).inverse() * dp_ft_rt_b_d;

    for (size_t ldof = 0; ldof < lemo::K::N_LDOF; ++ldof) {
      const auto dof = lemo::K::dofFromLegDof(leg, ldof);
      joint_d_[dof].q = q_d_out[ldof];
      joint_d_[dof].dq = dq_d[ldof];
    }
  }
}

void Trot::initializeSwingLeg(const int& leg, double t, const Eigen::Vector3d& p_ft_rt_b_ewrt_yaw_d,
                              const Eigen::Vector3d& dp_ft_rt_b_ewrt_yaw_d) {
  for (size_t i = 0; i < 3; ++i) {
    p_swing_init_rt_b_ewrt_yaw_d_[leg][i] = p_ft_rt_b_ewrt_yaw_d[i];
    dp_swing_init_rt_b_ewrt_yaw_d_[leg][i] =
        use_measured_foot_velocity_for_liftoff_ ? dp_ft_rt_b_ewrt_yaw_d[i] : -dp_body_des_[i];
  }
  // Ensure that the initial foot velocity isn't trying to drive the foot back into the ground.
  dp_swing_init_rt_b_ewrt_yaw_d_[leg][2] =
      std::max(dp_swing_init_rt_b_ewrt_yaw_d_[leg][2], min_liftoff_vel_);
  ddp_swing_init_rt_b_ewrt_yaw_d_[leg].fill(0);
  swing_legs_[leg].setInitialFootProperties(p_swing_init_rt_b_ewrt_yaw_d_[leg],
                                            dp_swing_init_rt_b_ewrt_yaw_d_[leg],
                                            ddp_swing_init_rt_b_ewrt_yaw_d_[leg], t);
}

auto makeTrot() -> LemoScenario* {
  return new Trot();
}

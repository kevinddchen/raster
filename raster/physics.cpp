#include <raster/physics.hpp>


namespace raster
{

Kinetics::Kinetics(
    float pos_friction,
    float ang_friction,
    const Eigen::Vector3f& init_pos_velocity,
    const Eigen::Vector3f& init_ang_velocity)
    : pos_friction(pos_friction),
      ang_friction(ang_friction),
      pos_velocity(init_pos_velocity),
      ang_velocity(init_ang_velocity)
{
}


Eigen::Affine3f Kinetics::update(const Eigen::Vector3f& delta_pos_velocity, const Eigen::Vector3f& delta_ang_velocity)
{
    const float ang_vel_angle = ang_velocity.norm();
    const Eigen::Vector3f ang_vel_axis = ang_velocity.normalized();

    Eigen::Affine3f pose;
    pose.translation() = pos_velocity;
    pose.linear() = Eigen::Matrix3f(Eigen::AngleAxis(ang_vel_angle, ang_vel_axis));

    // update velocities
    pos_velocity = pos_friction * pos_velocity + delta_pos_velocity;
    ang_velocity = ang_friction * ang_velocity + delta_ang_velocity;

    return pose;
}

}  // namespace raster

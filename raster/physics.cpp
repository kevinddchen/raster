#include <raster/physics.hpp>


namespace raster
{

Inertial::Inertial(
    float pos_friction, float ang_friction, const Eigen::Vector3f& velocity, const Eigen::Vector3f& angular_velocity)
    : pos_velocity(velocity),
      ang_velocity(angular_velocity),
      pos_friction(pos_friction),
      ang_friction(ang_friction)
{
}


Eigen::Affine3f Inertial::update(const Eigen::Vector3f& delta_velocity, const Eigen::Vector3f& delta_angular_velocity)
{
    const float angle = ang_velocity.norm();
    const Eigen::Vector3f axis = ang_velocity.normalized();

    Eigen::Affine3f pose;
    pose.translation() = pos_velocity;
    pose.linear() = Eigen::Matrix3f(Eigen::AngleAxis(angle, axis));

    // update velocities
    pos_velocity = pos_friction * pos_velocity + delta_velocity;
    ang_velocity = ang_friction * ang_velocity + delta_angular_velocity;

    return pose;
}

}  // namespace raster

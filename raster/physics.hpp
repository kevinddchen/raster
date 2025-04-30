#pragma once

#include <Eigen/Dense>


namespace raster
{

/**
 * Representation of a physical object's rigid motion.
 */
class Inertial
{
public:
    /**
     * Create new `Inertial`.
     *
     * @param pos_friction Friction for positional velocity. 1.0 means no friction.
     * @param ang_friction Friction for angular velocity. 1.0 means no friction.
     * @param init_pos_velocity Initial positional velocity.
     * @param init_ang_velocity Initial angular velocity.
     */
    Inertial(
        float pos_friction = 1.f,
        float ang_friction = 1.f,
        const Eigen::Vector3f& init_pos_velocity = Eigen::Vector3f::Zero(),
        const Eigen::Vector3f& init_ang_velocity = Eigen::Vector3f::Zero());

    /**
     * Step forward one unit in time and return the update to the object's pose due to its positional and angular
     * velocities. Can also be provided updates to the positional and angular velocities themselves, which will affect
     * the object's motion in future steps.
     *
     * @param delta_velocity If not null, will update the object's velocity AFTER the pose correction is returned.
     * @param delta_angular_velocity If not null, will update the object's angular velocity AFTER the pose correction is
     *     returned.
     * @returns A pose correction as an affine (i.e. rigid) transformation.
     */
    Eigen::Affine3f update(const Eigen::Vector3f& delta_velocity, const Eigen::Vector3f& delta_angular_velocity);

private:
    const float pos_friction;
    const float ang_friction;

    Eigen::Vector3f pos_velocity;
    Eigen::Vector3f ang_velocity;
};

}  // namespace raster

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Redshift
{
    [RequireComponent(typeof(CharacterController))]
    public class PlayerMotor : MonoBehaviour
    {
        [Header("Jumping")]
        /// <summary>
        /// Duration in seconds, after the player hits the ground, for which the player moves as if it is still in 
        /// the air. 
        /// 
        /// This is used to prevent the player from decelerating due to friction when hitting the ground after 
        /// jumping; allowing the player to reach higher velocities more easily while bunny hopping.
        /// </summary>
        ///
        /// <remarks>
        /// This does not necessarily apply for jumps only, therefore if the player has fell of an edge, and touches
        /// the ground, this window will still work.
        /// </remarks>
        [Tooltip("Duration after which deceleration (friction) is applied when the player touches the ground in s.")]
        public float JumpPostWindow = 0.1f;

        /// <summary>
        /// Duration in seconds, during jumps are "queued" up before the player touches the ground. 
        /// 
        /// This is used to allow the user to hit bunny hops without perfectly timing jump inputs; allowing the player
        /// to reach higher velocities more easily while bunny hopping.
        /// </summary>
        [Tooltip("Duration during which jumps input are handled before the player touches the ground in s.")]
        public float JumpPreWindow = 0.1f;

        /// <summary>
        /// Initial speed in units per second of jump motions.
        /// 
        /// This value is applied to the Y-component in the positive direction, of the velocity of the player whenever
        /// the jumps are served.
        /// </summary>
        [Tooltip("Initial vertical speed of jump motion in us-1.")]
        public float JumpSpeed = 4f;

        [Header("Accelerations")]
        /// <summary>
        /// Magnitude in units per second squared of the downwards acceleration acting as gravity.
        /// 
        /// This acceleration is applied to the Y-component in the negative direction, of the velocity of the player
        /// whenever it leaves the ground (is in the air, that is).
        /// </summary>
        [Tooltip("Magnitude of downwards acceleration acting as gravity in us-2.")]
        public float GravityMagnitude = 9.8f;

        /// <summary>
        /// Magnitude in units per second squared of the acceleration of the player when its on the ground.
        /// </summary>
        [Tooltip("Magnitude of acceleration of motion in us-2.")]
        public float AccelerationMagnitude = 90.0f;

        /// <summary>
        /// Magnitude in units per second squared of the deceleration of the player when its on the ground.
        /// </summary>
        [Tooltip("Magnitude of deceleration of motion in us-2.")]
        public float DecelerationMagnitude = 90.0f;

        [Header("Acceleration Coefficients")]
        /// <summary>
        /// Coefficient with which <see cref="AccelerationMagnitude"/> is multiplied with to give the acceleration
        /// of the player when in air.
        /// </summary>
        [Tooltip("Coefficient to multiply the acceleration with when in air.")]
        public float AirAccelerationCoefficient = 0.33f;

        /// <summary>
        /// Coefficient with which <see cref="DecelerationMagnitude"/> is multiplied with to give the deceleration
        /// of the player when in air.
        /// </summary>
        [Tooltip("Coefficient to multiply the deceleration with when in air.")]
        public float AirDecelerationCoefficient = 0.01f;

        [Header("Speeds")]
        /// <summary>
        /// Maximum speed in units per second when the player is on the ground.
        /// </summary>
        [Tooltip("Maximum moving speed of motion in us-1.")]
        public float MaxSpeed = 6.0f;

        /// <summary>
        /// Maximum speed in units per second when the player is in the air.
        /// </summary>
        [Tooltip("Maximum moving speed of motion when in air in us-1.")]
        public float MaxAirSpeed = 10.0f;

        /// <summary>
        /// Attached character controller.
        /// </summary>
        private CharacterController _controller;

        /* 
         * Duration in s for which the player is contact with the ground. This resets to 0 everytime the player
         * jumps.
         *
         * Basically a timer, which starts after the player hits the ground.
         */
        private float _contactTime;

        /*
         * Duration in s for which the player has last jumped. This resets to 0 everytime the playe jumps.
         * 
         * Basically a timer, which starts after the player jumps, that is when doing Move(direction, true).
         */
        private float _jumpedTime;

        /* Next motion to apply to the _controller in Move */
        private Vector3 _motion;

        /* Applied acceleration to _motion */
        private Vector3 _acceleration;

        private void Awake()
        {
            _controller = GetComponent<CharacterController>();
        }

        private void OnGUI()
        {
            /* TODO: Implement toggleable thingy. */
            GUI.Label(new Rect(10, 10, Screen.width - 10, 30),
                "isGrounded: " + _controller.isGrounded);
            GUI.Label(new Rect(10, 30, Screen.width - 10, 30),
                "motion: " + _motion + " -> " + _motion.magnitude + "unit/s");
            GUI.Label(new Rect(10, 50, Screen.width - 10, 30),
                "velocity: " + _controller.velocity + " -> " + _controller.velocity.magnitude + "unit/s");
            GUI.Label(new Rect(10, 70, Screen.width - 10, 30),
                "acceleration: " + _acceleration + " -> " + _acceleration.magnitude + "unit/s^2");
        }

        public void Move(Vector3 direction, bool jumped)
        {
            _jumpedTime = jumped ? 0f : _jumpedTime + Time.fixedDeltaTime;

            Vector3 inputWorldDirection = transform.TransformDirection(direction.x, 0, direction.z); // Direction the character is facing...
            inputWorldDirection = inputWorldDirection.normalized;

            if (_controller.isGrounded)
            {
                HandleOnGround(inputWorldDirection, jumped);
            }
            else
            {
                HandleInAir(inputWorldDirection);
            }

            _controller.Move(_motion * Time.fixedDeltaTime);
        }

        private void HandleInAir(Vector3 direction)
        {
            _contactTime = 0;

            /* If no user input, apply friction and decelerate. */
            if (direction == Vector3.zero)
            {
                ApplyFriction(DecelerationMagnitude * AirDecelerationCoefficient);
            }
            else
            {
                MoveAir(direction);
            }

            /* 
             * Apply gravitational force/acceleration when we're in the air. Set the Y-component of the acceleration to
             * gravitation acceleration (-gravityMagnitude) set.
             */
            _acceleration = new Vector3(_acceleration.x, -GravityMagnitude, _acceleration.z);

            // Primative ceiling detection
            if ((_controller.collisionFlags & CollisionFlags.Above) != 0)
            {
                _motion = new Vector3(_motion.x, -3.81f, _motion.z);
            }

            ApplyAcceleration();
        }

        private void MoveAir(Vector3 direction)
        {
            Vector3 _velocity = _controller.velocity;

            /* Limit air speed using quake's way of limiting speed. */
            float projection = Vector3.Dot(_velocity, direction);

            float finalAccel = AccelerationMagnitude * AirAccelerationCoefficient;
            if (finalAccel + projection > MaxAirSpeed)
            {
                finalAccel = MaxAirSpeed - projection;
            }

            _acceleration = direction * finalAccel;
            ApplyAcceleration();
        }

        private void HandleOnGround(Vector3 direction, bool jumped)
        {
            _contactTime += Time.fixedDeltaTime;

            /* If no user input, apply friction and decelerate. */
            if (direction == Vector3.zero)
            {
                ApplyFriction(DecelerationMagnitude);
            }
            else if (_contactTime > JumpPostWindow)
            {
                MoveGround(direction);
            }

            if (jumped || _jumpedTime <= JumpPreWindow)
            {
                /*
                 * Set the Y-component of the velocity to the initial jump speed.
                 */
                _motion = new Vector3(_motion.x, JumpSpeed, _motion.z);
            }
            else
            {
                /*
                 * When we're on the ground, we set the Y-component of the motion to something negative which makes 
                 * the CharacterController collide with the ground and therefore sets CharacterController.isGrounded
                 * to true.
                 */
                _motion = new Vector3(_motion.x, -0.1f, _motion.z);
            }
        }

        private void MoveGround(Vector3 direction)
        {
            _acceleration = direction * AccelerationMagnitude;
            ApplyAcceleration();

            /* Limit ground speed using a hard vector magnitude limit/clamp. */
            _motion = Vector3.ClampMagnitude(_motion, MaxSpeed);
        }

        private void ApplyAcceleration()
        {
            _motion += _acceleration * Time.fixedDeltaTime;
        }

        private void ApplyFriction(float magnitude)
        {
            Vector3 _velocity = _controller.velocity;
            /* 
             * Velocity on the XZ plane (no Y-velocity/vertical velocity).
             *
             * TODO: Consider friction on slopes
             */
            var xzVelocity = new Vector3(_velocity.x, 0, _velocity.z);
            if (xzVelocity != Vector3.zero)
            {
                _acceleration = -xzVelocity.normalized * magnitude;

                /* Prevent deceleration to cause acceleration in opposite direction. */
                var accelerationFrame = _acceleration * Time.fixedDeltaTime;
                _motion = accelerationFrame.sqrMagnitude >= _motion.sqrMagnitude ?
                            Vector3.zero : _motion + accelerationFrame;
            }
            else
            {
                _acceleration = Vector3.zero;
            }
        }

        public void SuperJump(float height)
        {
            _motion = new Vector3(_motion.x, height, _motion.z);
            _controller.Move(_motion * Time.fixedDeltaTime);
        }

        private void OnControllerColliderHit(ControllerColliderHit hit)
        {
            if (!_controller.isGrounded)
            {
                float projection = Vector3.Dot(hit.normal, _motion);

                if (projection < 0)
                {
                    _motion -= hit.normal * projection;
                }
            }
        }
    }
}
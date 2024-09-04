using UnityEngine;
using System.Collections;

namespace Redshift
{
    public class PlayerView : MonoBehaviour
    {
        /// <summary>
        /// The camera object of the player.
        /// </summary>
        public Transform PlayerCamera;

        /// <summary>
        /// Player camera offset.
        /// </summary>
        public Vector3 PlayerViewOffset = new Vector3(0, 0.6f, 0);

        // Camera rotations.
        private float rotX = 0.0f;
        private float rotY = 0.0f;

        public float xMouseSensitivity = 20;
        public float yMouseSensitivity = 20;

        private void Update()
        {
            MouseLook();
        }

        private void MouseLook()
        {
            // Camera rotation stuff, mouse controls this*/
            rotX -= Input.GetAxis("Mouse Y") * xMouseSensitivity;
            rotY += Input.GetAxis("Mouse X") * yMouseSensitivity;

            // Clamp the X rotation
            if (rotX < -90)
            {
                rotX = -90;
            }
            else if (rotX > 90)
            {
                rotX = 90;
            }

            this.transform.rotation = Quaternion.Euler(0, rotY, 0); // Rotates the collider
            PlayerCamera.rotation = Quaternion.Euler(rotX, rotY, 0); // Rotates the camera
            PlayerCamera.position = new Vector3(
               transform.position.x + PlayerViewOffset.x,
               transform.position.y + PlayerViewOffset.y,
               transform.position.z + PlayerViewOffset.z);
        }
    }
}
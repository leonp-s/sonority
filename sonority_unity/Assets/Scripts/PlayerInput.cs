using UnityEngine;

namespace Redshift
{
    public class PlayerInput : MonoBehaviour
    {
        public PlayerMotor playerMotor;

        Vector3 input;
        bool jumped;

        private void Start()
        {
            playerMotor = transform.GetComponent<PlayerMotor>();

            // Purely used for testing, will be managed by some other mechanism soon...
            Cursor.visible = false;
            Cursor.lockState = CursorLockMode.Locked;
        }

        private void Update()
        {
            // Poll Input in Update, consume in FixedUpdate
            input = new Vector3(Input.GetAxisRaw("Horizontal"), 0, Input.GetAxisRaw("Vertical"));
            jumped = Input.GetKeyDown(KeyCode.Space) ? true : jumped;
        }

        private void FixedUpdate()
        {
            playerMotor.Move(input, jumped);

            // Consume the jump...
            jumped = false;
        }
    }
}
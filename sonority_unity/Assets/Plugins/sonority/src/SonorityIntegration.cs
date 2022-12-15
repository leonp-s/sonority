using System.Runtime.InteropServices;
using UnityEngine;

public class SonorityIntegration : MonoBehaviour
{
    [DllImport("sonority_engine", EntryPoint="Init")]
    public static extern void Init();

    void Start()
    {
        Init();
    }
}

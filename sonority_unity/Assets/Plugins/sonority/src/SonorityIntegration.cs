using System.Runtime.InteropServices;
using UnityEngine;

public class SonorityIntegration : MonoBehaviour
{
    [DllImport("sonority_engine", EntryPoint="Init")]
    public static extern void Init();

    [DllImport("sonority_engine", EntryPoint="Deinit")]
    public static extern void Deinit();

    void Start()
    {
        Init();
    }

    void OnDestroy()
    {
        Deinit();
    }
    
}

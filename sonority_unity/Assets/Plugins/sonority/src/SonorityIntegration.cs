using System.Runtime.InteropServices;
using UnityEngine;

public class SonorityIntegration : MonoBehaviour
{
    [DllImport("sonority_engine", EntryPoint="Init")]
    public static extern void Init();

    [DllImport("sonority_engine", EntryPoint="Deinit")]
    public static extern void Deinit();
    
    [DllImport("sonority_engine", EntryPoint="SetPlayingNoise")]
    public static extern void SetPlayingNoise(bool is_playing_noise);


    void Start()
    {
        Init();
    }
    
    

    void OnDestroy()
    {
        Deinit();
    }
    
}

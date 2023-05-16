using System;
using System.Runtime.InteropServices;
using UnityEngine;

public static class SonorityInternal
{
    [DllImport("sonority_engine", EntryPoint="Internal_CreateSonority")]
    public static extern IntPtr CreateSonority();
    
    [DllImport("sonority_engine", EntryPoint="Internal_DestroySonority")]
    public static extern void DestroySonority(IntPtr sonority);
        
    [DllImport("sonority_engine", EntryPoint="Internal_SonorityPrepare")]
    public static extern void SonorityPrepare(IntPtr sonority);
        
    [DllImport("sonority_engine", EntryPoint="Internal_SonorityRelease")]
    public static extern void SonorityRelease(IntPtr sonority);
        
    [DllImport("sonority_engine", EntryPoint="Internal_SonoritySetPlayingNoise")]
    public static extern void SonoritySetPlayingNoise(IntPtr sonority, bool isPlayingNoise);
}

public class SonorityEngine
{
    private readonly IntPtr _sonorityEngine;
    
    public SonorityEngine()
    {
        _sonorityEngine = SonorityInternal.CreateSonority();
    }

    ~SonorityEngine()
    {
        SonorityInternal.DestroySonority(_sonorityEngine);
    }

    public void Prepare()
    {
        SonorityInternal.SonorityPrepare(_sonorityEngine);
    }

    public void Release()
    {
        SonorityInternal.SonorityRelease(_sonorityEngine);
    }

    public void SetPlayingNoise(bool isPlayingNoise)
    {
        SonorityInternal.SonoritySetPlayingNoise(_sonorityEngine, isPlayingNoise);
    }
};

public class SonorityIntegration : MonoBehaviour
{
    private SonorityEngine _sonorityEngine;

    void Start()
    {
        _sonorityEngine = new SonorityEngine();
        _sonorityEngine.Prepare();
        _sonorityEngine.SetPlayingNoise(true);
    }

    void OnDestroy()
    {
        _sonorityEngine.Release();
    }
}

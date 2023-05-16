using System;
using System.Runtime.InteropServices;
using fts;
using UnityEngine;

[PluginAttr("libsonority_engine")]
public static class SonorityInternal
{
    public delegate IntPtr CreateSonorityDelegate();
    public delegate void DestroySonorityDelegate(IntPtr sonority);
    public delegate void SonorityPrepareDelegate(IntPtr sonority);
    public delegate void SonorityReleaseDelegate(IntPtr sonority);
    public delegate void SonoritySetPlayingNoiseDelegate(IntPtr sonority, bool isPlayingNoise);

    [PluginFunctionAttr("Internal_CreateSonority")] 
    public static readonly CreateSonorityDelegate CreateSonority = null;
    
    [PluginFunctionAttr("Internal_DestroySonority")] 
    public static readonly DestroySonorityDelegate DestroySonority = null;
    
    [PluginFunctionAttr("Internal_SonorityPrepare")] 
    public static readonly SonorityPrepareDelegate SonorityPrepare = null;
    
    [PluginFunctionAttr("Internal_SonorityRelease")] 
    public static readonly SonorityReleaseDelegate SonorityRelease = null;
    
    [PluginFunctionAttr("Internal_SonoritySetPlayingNoise")] 
    public static readonly SonoritySetPlayingNoiseDelegate SonoritySetPlayingNoise = null;
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

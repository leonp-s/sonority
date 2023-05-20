using System;
using System.Runtime.InteropServices;
using fts;
using UnityEngine;

[PluginAttr("libsonority_engine")]
public static class SonorityInternal
{
    public delegate void SetLoggerDelegate(IntPtr debugCallback);
    public delegate IntPtr CreateSonorityDelegate();
    public delegate void DestroySonorityDelegate(IntPtr sonority);
    public delegate void SonorityPrepareDelegate(IntPtr sonority);
    public delegate void SonorityReleaseDelegate(IntPtr sonority);
    public delegate void SonoritySetPlayingNoiseDelegate(IntPtr sonority, bool isPlayingNoise);
    public delegate void SonorityPlayWavFileDelegate(IntPtr sonority);
    public delegate void SonoritySetSphericalCoordinatesDelegate(IntPtr sonority, float azimuth, float elevation);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void SonorityNativeDebugCallbackDelegate(string str);

    [PluginFunctionAttr("Internal_SetLogger")] 
    public static readonly SetLoggerDelegate SetLogger = null;
    
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

    [PluginFunctionAttr("Internal_SonorityPlayWavFile")]
    public static readonly SonorityPlayWavFileDelegate SonorityPlayWavFile = null;
    
    [PluginFunctionAttr("Internal_SonoritySetSphericalCoordinates")]
    public static readonly SonoritySetSphericalCoordinatesDelegate SonoritySetSphericalCoordinates = null;
}

public class SonorityEngine
{
    private readonly IntPtr _sonorityEngine;
    
    public SonorityEngine()
    {
        SonorityInternal.SonorityNativeDebugCallbackDelegate debugCallbackDelegate = new SonorityInternal.SonorityNativeDebugCallbackDelegate(SonorityNativeDebugCallback);
        IntPtr callbackFuncPtr = 
            Marshal.GetFunctionPointerForDelegate(debugCallbackDelegate);
        SonorityInternal.SetLogger(callbackFuncPtr);
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

    public void PlayWavFile()
    {
        SonorityInternal.SonorityPlayWavFile(_sonorityEngine);
    }

    public void SetSphericalCoordinates(float azimuth, float elevation)
    {
        SonorityInternal.SonoritySetSphericalCoordinates(_sonorityEngine, azimuth, elevation);
    }
    
    private static void SonorityNativeDebugCallback(string str)
    {
        Debug.Log("::Sonority Native:: " + str);
    }
};

public class SonorityIntegration : MonoBehaviour
{
    private SonorityEngine _sonorityEngine;

    void Start()
    {
        _sonorityEngine = new SonorityEngine();
        _sonorityEngine.Prepare();

    }

    void OnDestroy()
    {
        _sonorityEngine.Release();
    }

    
    public void SetPlayingNoise(bool isNoisePlaying)
    {
        _sonorityEngine.SetPlayingNoise(isNoisePlaying);
    }

    public void PlayWavFile()
    {
        _sonorityEngine.PlayWavFile();
    }

    public void SetSphericalCoordinates(float azimuth, float elevation)
    {
        _sonorityEngine.SetSphericalCoordinates(azimuth, elevation);
    }
}

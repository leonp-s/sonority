using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
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
    public delegate void SonorityRequestCreateSourceDelegate(IntPtr sonority, StringBuilder sourceBuilder);
    public delegate void SonorityDeleteSourceDelegate(IntPtr sonority, string source);
    public delegate void SonoritySourceDidUpdateDelegate(IntPtr sonority, string source, bool isPlaying, float volume, string filePath, float x, float y, float z);
    
    
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
    
    [PluginFunctionAttr("Internal_RequestCreateSource")] 
    public static readonly SonorityRequestCreateSourceDelegate SonorityRequestCreateSource = null;
    
    [PluginFunctionAttr("Internal_DeleteSource")] 
    public static readonly SonorityDeleteSourceDelegate SonorityDeleteSource = null;

    [PluginFunctionAttr("Internal_SourceDidUpdate")] 
    public static readonly SonoritySourceDidUpdateDelegate SonoritySourceDidUpdate = null;
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
    
    private static void SonorityNativeDebugCallback(string str)
    {
        Debug.Log("::Sonority Native:: " + str);
    }

    public void Prepare()
    {
        SonorityInternal.SonorityPrepare(_sonorityEngine);
    }

    public void Release()
    {
        SonorityInternal.SonorityRelease(_sonorityEngine);
    }

    public string RequestCreateSource()
    {
        StringBuilder sourceBuilder = new StringBuilder(100);
        SonorityInternal.SonorityRequestCreateSource(_sonorityEngine, sourceBuilder);
        return sourceBuilder.ToString();
    }

    public void DeleteSource(string source)
    {
        SonorityInternal.SonorityDeleteSource(_sonorityEngine, source);
    }

    public void SourceDidUpdate(string source, bool isPlaying, float volume, string filePath, float x, float y, float z)
    {
        SonorityInternal.SonoritySourceDidUpdate(_sonorityEngine, source, isPlaying, volume, filePath, x, y, z);
    }
};

public delegate void OnIntegrationLoadedDelegate();

public class SonorityIntegration : MonoBehaviour
{
    private SonorityEngine _sonorityEngine;
    
    public event OnIntegrationLoadedDelegate OnIntegrationLoaded;
    public Transform ListenerTransform;

    void Start()
    {
        _sonorityEngine = new SonorityEngine();
        _sonorityEngine.Prepare();

        OnIntegrationLoaded?.Invoke();

        //var source = _sonorityEngine.RequestCreateSource();
        //_sonorityEngine.SourceDidUpdate(source, true, 1.0f, "/Users/LeonPS/Documents/Development/sonority/sonority_engine/vocdemo.wav");
    }

    void OnDestroy()
    {
        _sonorityEngine.Release();
    }

    public string RegisterSource()
    {
        return _sonorityEngine.RequestCreateSource();
    }

    public void SourceDidUpdate(SonorityAudioSource sonorityAudioSource)
    {
        var cartesian = sonorityAudioSource.GetCartesianRelativeToListener(ListenerTransform);
        _sonorityEngine.SourceDidUpdate(sonorityAudioSource.SourceId, sonorityAudioSource.IsPlaying, sonorityAudioSource.Volume, sonorityAudioSource.GetAudioFilePath(), cartesian.x, cartesian.y, cartesian.z);
    }
}

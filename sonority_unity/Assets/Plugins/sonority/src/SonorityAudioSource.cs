using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class SonorityAudioSource : MonoBehaviour
{
    private SonorityIntegration _sonorityIntegration;

    public string SourceId { get; private set; }
    
    public string AudioFileName;
    public bool IsPlaying = true;
    
    [Range(0,1)]
    public float Volume = 1.0f;

    public float UpdateInterval = 0.2f;
    
    private float _elapsedUpdateTime = 0.0f;

    void Awake()
    {
        _sonorityIntegration = FindFirstObjectByType<SonorityIntegration>();
        if(_sonorityIntegration == null)
            Debug.LogError("No integration found in scene!");

        _sonorityIntegration.OnIntegrationLoaded += () =>
        {
            SourceId = _sonorityIntegration.RegisterSource();
        };
    }

    public string GetAudioFilePath()
    {
        return Path.Combine(Application.streamingAssetsPath, AudioFileName);
    }

    private void FixedUpdate()
    {
        if (!String.IsNullOrEmpty(SourceId))
        {
            _elapsedUpdateTime += Time.fixedDeltaTime;
            if (_elapsedUpdateTime > UpdateInterval)
            {
                _elapsedUpdateTime = 0;
                _sonorityIntegration.SourceDidUpdate(this);
            }
        }
    }

    public Vector3 GetCartesianRelativeToListener(Transform listenerTransform)
    {
        var diff = transform.position - listenerTransform.position;
        // Debug.Log("Diff: " + diff);
        diff = listenerTransform.rotation * diff;
        // Debug.Log("Rotated: " + diff);
        return diff;
    }
}

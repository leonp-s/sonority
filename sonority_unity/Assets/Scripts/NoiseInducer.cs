using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;



public class NoiseInducer : MonoBehaviour
{
    public Button playNoiseButton;
    public Button playWavFileButton;
    private bool isNoisePlaying = false;
    public SonorityIntegration SonorityIntegration;
    
    private void Awake()
    {
        playNoiseButton.onClick.AddListener(() =>
        {
            isNoisePlaying = !isNoisePlaying;
            SonorityIntegration.SetPlayingNoise(isNoisePlaying);
        });
        
        playWavFileButton.onClick.AddListener(() =>
        {

           SonorityIntegration.PlayWavFile();
        });
    }
}

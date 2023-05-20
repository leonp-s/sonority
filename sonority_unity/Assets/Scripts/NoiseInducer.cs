using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;



public class NoiseInducer : MonoBehaviour
{
    public Button PlayNoiseButton;
    public Button PlayWavFileButton;
    public Slider AzimuthSlider;
    public Slider ElevationSlider;
    
    private bool isNoisePlaying = false;
    public SonorityIntegration SonorityIntegration;
    
    private void Awake()
    {
        PlayNoiseButton.onClick.AddListener(() =>
        {
            isNoisePlaying = !isNoisePlaying;
            SonorityIntegration.SetPlayingNoise(isNoisePlaying);
        });
        
        PlayWavFileButton.onClick.AddListener(() =>
        {

           SonorityIntegration.PlayWavFile();
        });
        
        AzimuthSlider.onValueChanged.AddListener((float value) =>
        {
            UpdateSphericalCoordinates();
        });
        
        ElevationSlider.onValueChanged.AddListener((float value) =>
        {
            UpdateSphericalCoordinates();
        });
    }

    private void UpdateSphericalCoordinates()
    {
        SonorityIntegration.SetSphericalCoordinates(AzimuthSlider.value * -180, ElevationSlider.value * 90);
    }
}

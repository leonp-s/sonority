using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;



public class NoiseInducer : MonoBehaviour
{
    public Button playNoiseButton;
    private bool isNoisePlaying = false;
    private void Awake()
    {
        playNoiseButton.onClick.AddListener(() =>
        {
            isNoisePlaying = !isNoisePlaying;
            //SonorityEngine.SetPlayingNoise(isNoisePlaying);
        });
    }
}

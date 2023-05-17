using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEditor;

public class SonorityEditor : MonoBehaviour
{
    [MenuItem("Sonority Dev/Reopen Project")]
    public static void ReopenProject()
    {
        EditorApplication.OpenProject(Directory.GetCurrentDirectory());
    }
}

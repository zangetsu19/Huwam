using UnrealBuildTool;
using System.Collections.Generic;

public class HuwamEditorTarget : TargetRules
{
    public HuwamEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Huwam");
    }
}

using UnrealBuildTool;
using System.Collections.Generic;

public class HuwamTarget : TargetRules
{
    public HuwamTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Huwam");
    }
}

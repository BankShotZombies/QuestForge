using UnrealBuildTool;

public class QuestForgeEditor : ModuleRules
{
	public QuestForgeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"QuestForgeRuntime"
		});
        
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"EditorFramework",
			"EditorStyle",
			"AssetTools"
		});
	}
}
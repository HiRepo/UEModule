using UnrealBuildTool;

public class _CommonEditor : ModuleRules
{
	public _CommonEditor(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		// Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "_Common"
        });

		PrivateDependencyModuleNames.AddRange(
			new string[] {
                "PropertyEditor",
                "EditorStyle",
            }
		);

//	                "EditorWidgets",
//	                "Persona",

//			PrivateDependencyModuleNames.AddRange( 
//				new string[] {
//					"EditorStyle",
//	                "KismetCompiler",
//	                "GraphEditor",
//	                "Kismet",
//	                "PropertyEditor",
//				}
//			);


		PrivateDependencyModuleNames.AddRange(
			new string[] {
                "ShaderCore",
				"RenderCore",
				"RHI",
				"UMG",
			}
		);

//			CircularlyReferencedDependentModules.AddRange(
//	            new string[] {
//	                "KismetCompiler",
//	                "UnrealEd",
//	                "GraphEditor",
//	                "Kismet",
//	            }
//			); 


		PublicIncludePaths.AddRange(
			new string[] {
               	"Runtime/UMG/Public",			 		// UI
			}
		);
		PrivateIncludePaths.AddRange(
			new string[] {
               	"Runtime/UMG/Private",			 		// UI
			}
		);
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
				"ImageWrapper",
			}
        );
        PublicDependencyModuleNames.AddRange(
            new string[] {
				"HTTP",
				"MovieScene",
                "MovieSceneTracks",
			}
        );

		if (Target.Type != TargetRules.TargetType.Server)
		{
			PrivateIncludePathModuleNames.AddRange(
				new string[] {
					"SlateRHIRenderer",
				}
			);
            DynamicallyLoadedModuleNames.AddRange(
                new string[] {
				    "ImageWrapper",
				    "SlateRHIRenderer",
			    }
            );
		};


//--------------------------------------------------------------------
		PrivateIncludePaths.AddRange(
			new string[] {
				"Editor/BlueprintGraph/Private",
				"Editor/KismetCompiler/Public",
			}
		);

		PrivateDependencyModuleNames.AddRange( new string[] 
		{
			"UnrealEd", 
			"BlueprintGraph",
			"KismetCompiler",
		});
//				PrivateDependencyModuleNames.Add();
    }
}


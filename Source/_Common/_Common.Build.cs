// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class _Common : ModuleRules
{
	public _Common(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		// AI
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", });

		// Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "GameplayTasks" });


//			PrivateDependencyModuleNames.AddRange( 
//				new string[] {
//					"EditorStyle",
//	                "KismetCompiler",
//	                "GraphEditor",
//	                "Kismet",
//	                "PropertyEditor",
//				}
//			);

//			CircularlyReferencedDependentModules.AddRange(
//	            new string[] {
//	                "KismetCompiler",
//	                "UnrealEd",
//	                "GraphEditor",
//	                "Kismet",
//	            }
//			); 


//	Runtime/UMG/Public/Components/SlateWrapperTypes.h
//				PublicIncludePaths.AddRange(
//					new string[] {
//	                	"Runtime/UMG/Public", 					// UI
//					}
//				);


		PrivateDependencyModuleNames.AddRange(
			new string[] {
                "ShaderCore",
				"RenderCore",
				"RHI",
				"UMG",
			}
		);
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
        // "UnrealEd"  	for Editor	
		if( UEBuildConfiguration.bBuildEditor == true )
		{
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

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }
    }
}


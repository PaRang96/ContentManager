#include <istream>
#include <vector>

#include "JsonParser.h"
#include "TargetContent.h"

static std::vector<TargetContent*> ManagedContents;
static bool bShouldMinimizeSelf = false;
static std::string TouchPlayerPath = "";
static std::vector<rapidjson::GenericObject<false, rapidjson::Value>> ContentCandidates;

constexpr int	CHECKDELAY = 100;
constexpr char	TOUCHPLAYERPATH[] = "TouchPlayerPath";
constexpr char	TARGETPROGRAMS[] = "TargetPrograms";
constexpr char	PATH[] = "path";
constexpr char	LAUNCHDELAY[] = "launch_delay";
constexpr char	LAUNCHPARAMS[] = "launch_params";

int main()
{
	// generate json parser and setup paths for touch player
	static JsonParser* MasterJsonParser = new JsonParser();
	MasterJsonParser->ReadJsonFile(MasterJsonParser->GetFilePath());
	MasterJsonParser->GetStringField(TOUCHPLAYERPATH, TouchPlayerPath);

	// start reading target contents
	MasterJsonParser->GetObjectArrayField(TARGETPROGRAMS, ContentCandidates);

	for (const auto& ContentCandidate : ContentCandidates)
	{
		auto path = ContentCandidate[PATH].GetString();
		auto launchDelay = ContentCandidate[LAUNCHDELAY].GetInt();
		auto launchParamsCandidates = ContentCandidate[LAUNCHPARAMS].GetArray();
		std::vector<std::string> launchParams;
		if (launchParamsCandidates.Size() > 0)
		{
			for (auto& launchParam : launchParamsCandidates)
			{
				launchParams.push_back(launchParam.GetString());
			}
		}

		TargetContent* NewContent = new TargetContent(path, launchDelay, launchParams);

		ManagedContents.push_back(NewContent);
	}

	// boot up managed contents, apply delay if needed
	for (auto& ManagedContent : ManagedContents)
	{
		if (ManagedContent->IsTargetToe())
		{
			ManagedContent->BootToeWithDelay(TouchPlayerPath, ManagedContent->GetLaunchDelay());
		}
		else
		{
			ManagedContent->BootWithDelay(ManagedContent->GetLaunchDelay());
		}
		std::cout << "Process ID : " << ManagedContent->GetProcessID() <<
					 " HWND: "		 << ManagedContent->GetHWND()	   <<
				  std::endl;
	}

	// check up things & if target is not running, reboot without delay
	while (true)
	{
		for (auto& ManagedContent : ManagedContents)
		{
			if (ManagedContent->IsRunning())
			{
				continue;
			}
			else
			{
				if (ManagedContent->IsTargetToe())
				{
					ManagedContent->BootToe(TouchPlayerPath);
				}
				else
				{
					ManagedContent->Boot();
				}
				std::cout << "Process ID : " << ManagedContent->GetProcessID() <<
							 " HWND: "		 << ManagedContent->GetHWND()	   <<
						  std::endl;
			}
			Sleep(CHECKDELAY);
		}
	}

	// return all the memory back to OS
	for (auto& ManagedContent : ManagedContents)
	{
		ManagedContent->Close();
		delete ManagedContent;
		ManagedContent = nullptr;
	}

	delete MasterJsonParser;
	MasterJsonParser = nullptr;

	system("pause");

	return 0;
}
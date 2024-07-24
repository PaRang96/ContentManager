#include <istream>
#include <vector>

#include "JsonParser.h"
#include "TargetContent.h"

static std::vector<TargetContent*> ManagedContents;
static bool bShouldMinimizeSelf = false;
static std::string TouchPlayerPath = "";
static std::vector<rapidjson::GenericObject<false, rapidjson::Value>> ContentCandidates;

constexpr int	CHECKDELAY			= 100;
constexpr char	TOUCHPLAYERPATH[]	= "TouchPlayerPath";
constexpr char	TARGETPROGRAMS[]	= "TargetPrograms";
constexpr char	PATH[]				= "path";
constexpr char	LAUNCHDELAY[]		= "launch_delay";
constexpr char	LAUNCHPARAMS[]		= "launch_params";

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
		// ready needed params for target content
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

		// generate target content
		TargetContent* NewContent = new TargetContent(path, launchDelay, launchParams);

		ManagedContents.push_back(NewContent);
	}

	// boot up managed contents, apply delay if needed
	for (const auto& ManagedContent : ManagedContents)
	{
		ManagedContent->Boot(ManagedContent->GetLaunchDelay(), TouchPlayerPath);
		std::cout << "Process ID : " << ManagedContent->GetProcessID() <<
					 " HWND: "		 << ManagedContent->GetHWND()	   <<
					 '\n'			 << std::endl;
	}
	
	// check up things & if target is not running, reboot without delay
	// main loop
	while (true)
	{
		for (const auto& ManagedContent : ManagedContents)
		{
			if (ManagedContent->IsRunning())
			{
				continue;
			}
			else
			{
				int noDelay = 0;
				ManagedContent->Boot(noDelay, TouchPlayerPath);
				std::cout << "Process ID : " << ManagedContent->GetProcessID() <<
							 " HWND: "		 << ManagedContent->GetHWND()	   <<
							 '\n'			 << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(CHECKDELAY));
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
#include "TargetContent.h"

TargetContent::TargetContent()
{

}

TargetContent::TargetContent(const std::string& path, const int& launchDelay,
	const std::vector<std::string>& launchParams)
{
	std::cout << "Path: " << path << '\n';
	std::cout << "Launch Delay: " << launchDelay << '\n';
	std::cout << "Param Length: " << launchParams.size() << '\n';
	for (auto param : launchParams)
	{
		std::cout << "param : " << param << '\n';
	}
	std::cout << "END OF TARGET CONTENT INFO\n" << std::endl;

	Path = path;
	LaunchDelay = launchDelay;
	ExecutionArguments = launchParams;

	bIsToeFile = IsTargetToe(Path);
}

TargetContent::~TargetContent()
{
}

BOOL CALLBACK TargetContent::EnumWindows_FindTargetHwnd(HWND hwnd, LPARAM lParam)
{
	EnumWindowsData* Data = reinterpret_cast<EnumWindowsData*>(lParam);
	DWORD windowProcessID;
	GetWindowThreadProcessId(hwnd, &windowProcessID);

	if (windowProcessID == Data->ProcessID)
	{
		Data->hwnd = hwnd;
		std::cout << "Found HWND: " << hwnd << std::endl;
		return FALSE;
	}
	return TRUE;
}

HWND TargetContent::GetWindowHandleForProcess(DWORD ProcessID)
{
	WindowsData.ProcessID = ProcessID;
	WindowsData.hwnd = nullptr;

	for (int i = 0; i < MaxTrials; i++)
	{
		EnumWindows(EnumWindows_FindTargetHwnd, reinterpret_cast<LPARAM>(&WindowsData));
		if (WindowsData.hwnd != nullptr)
		{
			return WindowsData.hwnd;
		}
		Sleep(Delay);
	}
	return nullptr;
}

bool TargetContent::IsTargetToe(const std::string& path)
{
	return path.find("toe") != std::string::npos;
}

HWND TargetContent::Boot(int delay, std::string& TouchPlayerPath)
{
	if (!IsTargetToe())
	{
		return BootWithDelay(delay);
	}
	else
	{
		return BootToeWithDelay(TouchPlayerPath, delay);
	}
}

HWND TargetContent::Boot_Internal()
{
	STARTUPINFOA StartupInfo = { sizeof(StartupInfo) };
	PROCESS_INFORMATION ProcessInformation;

	std::string TempPath = Path;

	if (ExecutionArguments.size() > 0)
	{
		for (unsigned int i = 0; i < ExecutionArguments.size(); i++)
		{
			TempPath = Path + " " + ExecutionArguments[i];
		}
	}

	if (!CreateProcessA(TempPath.c_str(), NULL, NULL, NULL, FALSE,
		0, NULL, NULL, &StartupInfo, &ProcessInformation))
	{
		return NULL;
	}

	HWND hwnd = GetWindowHandleForProcess(ProcessInformation.dwProcessId);
	if (hwnd)
	{
		std::cout << "Window Handle: " << hwnd << std::endl;
		CloseHandle(ProcessInformation.hProcess);
		CloseHandle(ProcessInformation.hThread);
		return hwnd;
	}
	else
	{
		std::cout << "No window found for the process" << std::endl;
		CloseHandle(ProcessInformation.hProcess);
		CloseHandle(ProcessInformation.hThread);
		return NULL;
	}
}

HWND TargetContent::BootWithDelay(int delay)
{
	std::promise<HWND> promise;
	std::future<HWND> future = promise.get_future();

	std::thread([delay, this, promise = std::move(promise)]() mutable {
		std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		HWND bootHwnd = Boot_Internal();
		promise.set_value(bootHwnd);
		}).detach();

	return future.get();
}

HWND TargetContent::BootToe_Internal(std::string& TouchPlayerPath)
{
	STARTUPINFOA StartupInfo = { sizeof(StartupInfo) };
	PROCESS_INFORMATION ProcessInformation;

	std::string TempPath = TouchPlayerPath + " " + Path;

	if (!CreateProcessA(NULL, &TempPath[0], NULL, NULL, FALSE,
		0, NULL, NULL, &StartupInfo, &ProcessInformation))
	{
		return NULL;
	}
	
	HWND hwnd = GetWindowHandleForProcess(ProcessInformation.dwProcessId);
	if (hwnd)
	{
		std::cout << "Window Handle: " << hwnd << std::endl;
		CloseHandle(ProcessInformation.hProcess);
		CloseHandle(ProcessInformation.hThread);
		return hwnd;
	}
	else
	{
		std::cout << "No window found for the process" << std::endl;
		CloseHandle(ProcessInformation.hProcess);
		CloseHandle(ProcessInformation.hThread);
		return NULL;
	}
}

HWND TargetContent::BootToeWithDelay(std::string& TouchPlayerPath, int delay)
{
	std::promise<HWND> promise;
	std::future<HWND> future = promise.get_future();

	std::thread([delay, this, &TouchPlayerPath, promise = std::move(promise)]() mutable {
		std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		HWND bootHwnd = BootToe_Internal(TouchPlayerPath);
		promise.set_value(bootHwnd);
		}).detach();

	return future.get();
}

void TargetContent::Close()
{
	auto target = OpenProcess(PROCESS_TERMINATE, FALSE, WindowsData.ProcessID);
	TerminateProcess(target, 1);
	CloseHandle(target);

	WindowsData.hwnd = NULL;
	WindowsData.ProcessID = NULL;
}

DWORD TargetContent::GetProcessID()
{
	return WindowsData.ProcessID;
}

HWND TargetContent::GetHWND()
{
	return WindowsData.hwnd;
}

bool TargetContent::IsRunning()
{
	bRunning = IsWindow(WindowsData.hwnd);
	return bRunning;
}
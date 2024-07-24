#pragma once

#include <rapidjson/document.h>

#include <iostream>
#include <Windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <future>

const int MaxTrials = 50;
const int Delay = 100;

struct EnumWindowsData
{
	DWORD ProcessID		= NULL;
	HWND hwnd			= NULL;
};

class TargetContent
{
public:
	/// <summary>
	/// default constructor
	/// </summary>
	TargetContent();

	/// <summary>
	/// constructor with needed parameters
	/// </summary>
	/// <param name="path"> target content's path (url), recommend using absolute path </param>
	/// <param name="launchDelay"> launch delay </param>
	/// <param name="launchParams"> execution arguments </param>
	TargetContent(const std::string& path, const int& launchDelay,
		const std::vector<std::string>& launchParams);
	virtual ~TargetContent();
	
private:
	EnumWindowsData WindowsData;

	static BOOL CALLBACK EnumWindows_FindTargetHwnd(HWND hwnd, LPARAM lParam);
	HWND GetWindowHandleForProcess(DWORD ProcesID);

	/// <summary>
	/// Distinguish whether target content is touch designer file or not
	/// </summary>
	/// <param name="path"> Target content's path </param>
	/// <returns> If target content contains "toe" in path, returns true </returns>
	bool IsTargetToe(const std::string& path);

	int LaunchDelay = 0;
	bool bRunning = false;
	bool bIsToeFile = false;
	std::vector<std::string> ExecutionArguments;
	std::string Path = "Path/To/Your/Target/Exe.exe";

protected:
	
public:
	/// <summary>
	/// Boot up target content, wrapper for all the boots
	/// </summary>
	/// <param name="delay"> Delay in milisecond </param>
	/// <param name="TouchPlayerPath"></param>
	/// <returns></returns>
	HWND Boot(int delay, std::string& TouchPlayerPath);

private:
public:
	/// <summary>
	/// Boot up target exe
	/// CAN deal with execution arguments
	/// </summary>
	/// <returns></returns>
	HWND Boot_Internal();

	/// <summary>
	/// Boot up target exe with delay
	/// </summary>
	/// <param name="delay"> Delay in milisecond </param>
	/// <returns></returns>
	HWND BootWithDelay(int delay);
	
	/// <summary>
	/// Boot up target toe
	/// CANNOT deal with execution arguments
	/// </summary>
	/// <param name="TouchPlayerPath"></param>
	/// <returns></returns>
	HWND BootToe_Internal(std::string& TouchPlayerPath);

	/// <summary>
	/// Boot up target toe with delay
	/// </summary>
	/// <param name="TouchPlayerPath"></param>
	/// <param name="delay"> Delay in milisecond </param>
	/// <returns></returns>
	HWND BootToeWithDelay(std::string& TouchPlayerPath, int delay);

public:
	/// <summary>
	/// Close target exe
	/// </summary>
	void Close();

	/// <summary>
	/// Get target content's process id
	/// </summary>
	/// <returns> process id of target content </returns>
	DWORD GetProcessID();

	/// <summary>
	/// Get target content's hwnd
	/// </summary>
	/// <returns> hwnd of target content </returns>
	HWND GetHWND();

	/// <summary>
	/// Check whether target content is running or not
	/// </summary>
	/// <returns> true if target content is running </returns>
	bool IsRunning();

	/// <summary>
	/// Check whether target content is toe or not
	/// </summary>
	/// <returns> True if target content is toe </returns>
	inline bool IsTargetToe() { return bIsToeFile; };

	/// <summary>
	/// Check target contnet's initial launch delay
	/// </summary>
	/// <returns> Return target content's launch delay, interpreted as milisecond </returns>
	inline int& GetLaunchDelay() { return LaunchDelay; };
};
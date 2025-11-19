#pragma once
#include <functional>

namespace imgui {

	struct DebugView
	{
		bool open = false;
		bool& isPaused;
		std::function<void()> stepCallback = nullptr;
		std::function<void()> cpuStatusCallback = nullptr;

		void updateWindow();

		DebugView(bool& paused) : isPaused{ paused } {}
	};

} // namespace imgui

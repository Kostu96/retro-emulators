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

		DebugView(bool& isPaused) : isPaused{ isPaused } {}
	};

} // namespace imgui

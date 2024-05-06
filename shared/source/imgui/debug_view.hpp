#pragma once
#include <functional>

namespace imgui {

	struct DebugView
	{
		bool open = false;
		std::function<void()> cpuStepCallback = nullptr;
		std::function<void()> cpuStatusCallback = nullptr;

		void updateWindow();
	};

} // namespace imgui

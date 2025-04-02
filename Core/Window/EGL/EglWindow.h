#pragma once
#include <Window/Window.h>
namespace flora
{
	class XEglWindow final : public XWindow
	{
	public:
		XEglWindow() = default;
		XEglWindow(const XWindowCreateDesc& InDesc);
		virtual ~XEglWindow() = default;
	public:
		void Terminate() final;

		void SwapBuffers() final;
		void PollEvents() final;

		bool IsWindowShouldClose() final;

		void SetSwapInterval(int) final;

		void* GetProcAddressCallbackFunc() final;

		double GetTime() final;
		virtual void MakeContextCurrent() final;
		virtual int GetKey(int) final;
		virtual int GetMouseButton(int) final;
		virtual void GetCursorPos(double*, double*) final;
		virtual void* GetHandle() final;
	public:
		virtual EWindowType GetType()const override;
	protected:
		void* Handle = nullptr;
	};
}

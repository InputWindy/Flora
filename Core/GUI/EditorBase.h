#pragma once
namespace XVerse
{
	enum class EEditorType
	{
		ET_ImGui,

		ET_MAX_COUNT
	};

	class IEditor
	{
	protected:
		IEditor() = default;
	public:
		virtual ~IEditor() = default;

		template<EEditorType Type>
		static std::shared_ptr<IEditor> InitEditor(void* InWindowHandle);
	public:
		virtual void Terminate() = 0;
		virtual void BeginRender() = 0;
		virtual void Render() = 0;
		virtual void EndRender() = 0;
	};
}
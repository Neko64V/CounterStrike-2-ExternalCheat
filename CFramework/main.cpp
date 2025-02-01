#include "Cheat/FrameCore.h"
#include "Framework/Overlay/Overlay.h"
#include "Framework/ImGui/Fonts/RobotoRegular.h"

Overlay*	overlay = new Overlay;
CFramework* cheat = new CFramework;

void Memory::GetBaseAddress()
{
	// ベースアドレスを取得
	m_gClientBaseAddr = GetModuleBase("client.dll");
	//m_gEngineBaseAddr = GetModuleBase("engine.dll");
}

void Overlay::OverlayUserInit()
{
	// ImGui io setting
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	// Load Font
	io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, 13.f, nullptr);

	// Load Icon
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.GlyphOffset.y = 2.f;
	cheat->icon = io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, 16.f, &icons_config, icons_ranges);
	io.Fonts->Build();
}

void Overlay::OverlayUserFunction()
{
	cheat->MiscAll();

	cheat->RenderInfo();

	if (g.g_ESP)
		cheat->RenderESP();

	if (g.g_ShowMenu)
		cheat->RenderMenu();
}

// DEBUG時にはコンソールウィンドウを表示する
#if _DEBUG
int main()
#else 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// プロセスに接続する
	if (!m.AttachProcess("cs2.exe", InitializeMode::PROCESS)) // 詳細は Framework/Utils/Memory/Memory.h を参照
		return 1;

	// ベースアドレスを取得する
	m.GetBaseAddress();

	// Overlay
	if (!overlay->InitOverlay("cs2.exe", InitializeMode::PROCESS)) // MemoryInitModeと同様
		return 2;

	// スレッドを作成
	std::thread([&]() { cheat->UpdateList(); }).detach(); // ESP/AIM用にプレイヤーのデータをキャッシュする

	timeBeginPeriod(1);
	overlay->OverlayLoop();
	overlay->DestroyOverlay();
	m.DetachProcess();
	timeEndPeriod(1);
	g.g_Run = false;
	delete cheat, overlay;

	return 0;
}
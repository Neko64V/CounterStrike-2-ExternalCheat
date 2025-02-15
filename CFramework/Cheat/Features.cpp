#include "FrameCore.h"
#pragma comment(lib, "WinMM.lib")

const int ReadCount = 1024;

void CFramework::UpdateList()
{
    if (!Game->InitOffset()) {
        g.process_active = false;
        return;
    }

    while (g.process_active)
    {
        std::vector<CEntity> ent_list;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        // Read EntityList
        auto list_addr = m.Read<uintptr_t>(m.m_gClientBaseAddr + Game->dwEntityList);

        if (list_addr == NULL)
            continue;

        // Local
        pLocal->address = m.Read<uintptr_t>(m.m_gClientBaseAddr + Game->dwLocalPlayerController);

        if (!pLocal->UpdateStatic(list_addr))
            continue;
        else if (!pLocal->Update())
            continue;

        for (int i = 0; i < ReadCount; i++)
        {
            uintptr_t entity_entry = m.Read<uintptr_t>(list_addr + (0x8 * (i & 0x7FFF) >> 9) + 0x10);

            if (entity_entry == NULL)
                continue;

            CEntity p = CEntity();
            p.address = m.Read<uintptr_t>(entity_entry + 120 * (i & 0x1FF));

            uintptr_t classNamePtr = m.ReadChain(p.address, { 0x10, 0x20 });
            std::string class_name = m.ReadStringA(classNamePtr);

            if (class_name.size() > 0) {

                if (entity_entry == pLocal->address)
                    continue;

                // Player
                if (!class_name.compare("cs_player_controller")) 
                {
                    if (!p.UpdateStatic(list_addr))
                        continue;
                    else if (!p.Update())
                        continue;

                    p.m_nameClass = class_name;
                    ent_list.push_back(p);
                }
            }
        }

        EntityList = ent_list;
        ent_list.clear();
    }
}

void CFramework::MiscAll()
{
    if (pLocal->IsDead())
        return;

    // bHop, NoRecoil and more...

}
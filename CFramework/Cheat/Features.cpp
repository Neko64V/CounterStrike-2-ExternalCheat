#include "FrameCore.h"
#pragma comment(lib, "WinMM.lib")

const int ReadCount = 128;

void CFramework::UpdateList()
{
    while (g.g_Run)
    {
        std::vector<CEntity> ent_list;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        // Read EntityList
        auto list_addr = m.Read<uintptr_t>(m.m_gClientBaseAddr + offset::dwEntityList);

        if (list_addr == NULL)
            continue;

        // Local
        pLocal->address = m.Read<uintptr_t>(m.m_gClientBaseAddr + offset::dwLocalPlayerController);

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

            if (entity_entry == pLocal->address)
                continue;

            // get CSPlayerPawn and some data
            if (!p.UpdateStatic(list_addr))
                continue;
            else if (!p.Update())
                continue;

            ent_list.push_back(p);
        }

        EntityList = ent_list;
        ent_list.clear();
    }
}

void CFramework::MiscAll()
{
    if (!pLocal->Update())
        return;

    // NoRecoil
    if (g.g_NoRecoil)
    {
        /*
        static Vector3 OldPunch{};

        Vector3 PunchAngle = pLocal->GetWeaponPunchAngle();

        if (!Vec3_Empty(PunchAngle)) {
            Vector3 Delta = pLocal->GetViewAngle() + ((OldPunch - PunchAngle) * g.g_NoRecoilVal);
            NormalizeAngles(Delta);

            if (!Vec3_Empty(Delta))
                m.Write<Vector3>(pLocal->address + offset::m_ViewAngle, Delta);

            OldPunch = PunchAngle;
        }
        */
    }

    // bHop
    if (g.g_BunnyHop)
    {
        /*
        if (GetAsyncKeyState(VK_SPACE))
        {
            int flag = pLocal->GetFlag();

            if (flag != 64) {
                m.Write<uint32_t>(m.m_gProcessBaseAddr + offset::in_jump + 0x8, 5);
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                m.Write<uint32_t>(m.m_gProcessBaseAddr + offset::in_jump + 0x8, 4);
            }
        }
        */
    }
}
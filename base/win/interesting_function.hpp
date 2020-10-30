#pragma once

#include <windows.h>

#include <array>
#include <cmath>

namespace BeepMusic
{
    constexpr std::array<std::pair<int, float>, 32> notes{{
        {84, 1 / 2.f}, {84, 1 / 4.f}, {86, 1 / 8.f}, {84, 1 / 4.f},
        {82, 1 / 4.f}, {81, 1 / 4.f}, {77, 1 / 8.f}, {79, 1 / 8.f},
        {72, 1 / 8.f}, {77, 1 / 2.f}, {76, 1 / 8.f}, {77, 1 / 8.f},
        {79, 1 / 8.f}, {81, 1 / 4.f}, {79, 1 / 4.f}, {77, 1 / 4.f},
        {79, 1 / 4.f}, {81, 1 / 8.f}, {84, 1 / 2.f}, {84, 1 / 4.f},
        {86, 1 / 8.f}, {84, 1 / 4.f}, {82, 1 / 4.f}, {81, 1 / 4.f},
        {77, 1 / 8.f}, {79, 1 / 8.f}, {72, 1 / 8.f}, {77, 1 / 2.f},
        {76, 1 / 8.f}, {77, 1 / 8.f}, {76, 1 / 8.f}, {74, 1 / 2.f},
    }};

    void playOrange()
    {
        constexpr auto get_frequency = [](const int note) {
            if (note < 0 || note > 119) return -1.0f;
            return 440.0f *
                   std::pow(2.0f, static_cast<float>(note - 57) / 12.0f);
        };

        constexpr auto get_duration = [](const float duration) {
            return 1600 * duration;
        };

        for (const auto& [note, duration] : notes)
        {
            ::Beep(static_cast<DWORD>(get_frequency(note)),
                   static_cast<DWORD>(get_duration(duration)));
        }
    }
    }

}  // namespace BeepMusic

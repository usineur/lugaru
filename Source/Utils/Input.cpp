/*
Copyright (C) 2003, 2010 - Wolfire Games
Copyright (C) 2010-2017 - Lugaru contributors (see AUTHORS file)

This file is part of Lugaru.

Lugaru is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Lugaru is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Lugaru.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Utils/Input.hpp"

static bool keyDown[SDL_NUM_SCANCODES + numMouseButtons + SDL_CONTROLLER_BUTTON_MAX];
static bool keyPressed[SDL_NUM_SCANCODES + numMouseButtons + SDL_CONTROLLER_BUTTON_MAX];
static SDL_GameController* gameController = nullptr;

void Input::Init() {
    const int joystickCount = SDL_NumJoysticks();
    for (int i = 0; i < joystickCount; i++) {
        if (!SDL_IsGameController(i) || (gameController = SDL_GameControllerOpen(i)) == nullptr) {
            continue;
        }

        if (const char* const name = SDL_GameControllerName(gameController)) {
            fprintf(stderr, "Opened game controller %s\n", name);
        }
        else {
            fprintf(stderr, "Opened unknown game controller\n");
        }
        return;
    }

    fprintf(stderr, "No game controller found\n");
}

void Input::Tick()
{
    SDL_PumpEvents();
    int numkeys;
    const Uint8* keyState = SDL_GetKeyboardState(&numkeys);
    for (int i = 0; i < numkeys; i++) {
        keyPressed[i] = !keyDown[i] && keyState[i];
        keyDown[i] = keyState[i];
    }
    Uint8 mb = SDL_GetMouseState(nullptr, nullptr);
    for (int i = 1; i < numMouseButtons; i++) {
        keyPressed[SDL_NUM_SCANCODES + i] = !keyDown[SDL_NUM_SCANCODES + i] && (mb & SDL_BUTTON(i));
        keyDown[SDL_NUM_SCANCODES + i] = (mb & SDL_BUTTON(i));
    }
    if (gameController != nullptr) {
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
            const int index = SDL_NUM_SCANCODES + numMouseButtons + i;
            const bool isDown = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton(i));
            keyPressed[index] = !keyDown[index] && isDown;
            keyDown[index] = isDown;
        }
    }
}

bool Input::isKeyDown(int k)
{
    if (k >= SDL_NUM_SCANCODES + numMouseButtons + SDL_CONTROLLER_BUTTON_MAX) {
        return false;
    }
    return keyDown[k];
}

bool Input::isKeyPressed(int k)
{
    if (k >= SDL_NUM_SCANCODES + numMouseButtons + SDL_CONTROLLER_BUTTON_MAX) {
        return false;
    }
    return keyPressed[k];
}

float Input::GetAxis(SDL_GameControllerAxis axis)
{
    if (gameController == nullptr) {
        return 0.f;
    }
    const float result = SDL_GameControllerGetAxis(gameController, axis) / 32768.f;
    if (std::abs(result) < 0.1f) {
        return 0.f;
    }
    return result;
}

std::string Input::keyToChar(unsigned short i)
{
    if (i < SDL_NUM_SCANCODES) {
        return SDL_GetKeyName(SDL_GetKeyFromScancode(SDL_Scancode(i)));
    } else if (i == MOUSEBUTTON_LEFT) {
        return "mouse left button";
    } else if (i == MOUSEBUTTON_RIGHT) {
        return "mouse right button";
    } else if (i == MOUSEBUTTON_MIDDLE) {
        return "mouse middle button";
    } else if (i == MOUSEBUTTON_X1) {
        return "mouse button 4";
    } else if (i == MOUSEBUTTON_X2) {
        return "mouse button 5";
    } else if (SDL_NUM_SCANCODES + numMouseButtons <= i && i < SDL_NUM_SCANCODES + numMouseButtons + SDL_CONTROLLER_BUTTON_MAX) {
        return std::string(SDL_GameControllerGetStringForButton(SDL_GameControllerButton(i - SDL_NUM_SCANCODES - numMouseButtons))) + " (joystick)";
    } else {
        return "unknown";
    }
}

bool Input::MouseClicked()
{
#ifdef __SWITCH__
    return isKeyPressed(MOUSEBUTTON_LEFT) || isKeyPressed(519); // Ugly hack: press A as mouse click
#else
    return isKeyPressed(MOUSEBUTTON_LEFT);
#endif
}

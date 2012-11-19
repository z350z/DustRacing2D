// This file is part of Dust Racing 2D.
// Copyright (C) 2012 Jussi Lind <jussi.lind@iki.fi>
//
// Dust Racing 2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Dust Racing 2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Dust Racing 2D. If not, see <http://www.gnu.org/licenses/>.

#include "eventhandler.hpp"
#include "keycodes.hpp"
#include "statemachine.hpp"

#include <MenuManager>

EventHandler::EventHandler(InputHandler & inputHandler)
: m_inputHandler(inputHandler)
{
    m_keyToActionMap[KeyCodes::LSHIFT] = {1, InputHandler::IA_UP};
    m_keyToActionMap[KeyCodes::RSHIFT] = {0, InputHandler::IA_UP};
    m_keyToActionMap[KeyCodes::LCTRL]  = {1, InputHandler::IA_DOWN};
    m_keyToActionMap[KeyCodes::RCTRL]  = {0, InputHandler::IA_DOWN};
    m_keyToActionMap[Qt::Key_Left]     = {0, InputHandler::IA_LEFT};
    m_keyToActionMap[Qt::Key_Right]    = {0, InputHandler::IA_RIGHT};
    m_keyToActionMap[Qt::Key_A]        = {1, InputHandler::IA_LEFT};
    m_keyToActionMap[Qt::Key_D]        = {1, InputHandler::IA_RIGHT};
    m_keyToActionMap[Qt::Key_Up]       = {0, InputHandler::IA_UP};
    m_keyToActionMap[Qt::Key_Down]     = {0, InputHandler::IA_DOWN};
    m_keyToActionMap[Qt::Key_W]        = {1, InputHandler::IA_UP};
    m_keyToActionMap[Qt::Key_S]        = {1, InputHandler::IA_DOWN};
}

bool EventHandler::handleKeyPressEvent(QKeyEvent * event)
{
    if (StateMachine::instance().state() != StateMachine::Menu)
    {
        return handleGameKeyPressEvent(event);
    }
    else
    {
        return handleMenuKeyPressEvent(event);
    }

    return false;
}

bool EventHandler::handleKeyReleaseEvent(QKeyEvent * event)
{
    if (StateMachine::instance().state() != StateMachine::Menu)
    {
        return handleGameKeyReleaseEvent(event);
    }

    return false;
}

bool EventHandler::handleMenuKeyPressEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        MTFH::MenuManager::instance().left();
        break;
    case Qt::Key_Right:
        MTFH::MenuManager::instance().right();
        break;
    case Qt::Key_Up:
        MTFH::MenuManager::instance().up();
        break;
    case Qt::Key_Down:
        MTFH::MenuManager::instance().down();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        MTFH::MenuManager::instance().selectCurrentItem();
        break;
    case Qt::Key_Escape:
    case Qt::Key_Q:
        MTFH::MenuManager::instance().popMenu();
        if (MTFH::MenuManager::instance().done())
        {
            emit gameExited();
        }
        break;
    default:
        return false;
    }

    return true;
}

bool EventHandler::handleGameKeyPressEvent(QKeyEvent * event)
{
    return findMatch(event, true);
}

bool EventHandler::handleGameKeyReleaseEvent(QKeyEvent * event)
{
    return findMatch(event, false);
}

bool EventHandler::findMatch(QKeyEvent * event, bool press)
{
    if (!event->isAutoRepeat())
    {
        if (m_keyToActionMap.find(event->nativeScanCode()) != m_keyToActionMap.end())
        {
            const unsigned int player = m_keyToActionMap[event->nativeScanCode()].player;
            const InputHandler::InputAction action =
                m_keyToActionMap[event->nativeScanCode()].action;
            m_inputHandler.setActionState(player, action, press);
            return true;
        }

        if (m_keyToActionMap.find(event->key()) != m_keyToActionMap.end())
        {
            const unsigned int player = m_keyToActionMap[event->key()].player;
            const InputHandler::InputAction action =
                m_keyToActionMap[event->key()].action;
            m_inputHandler.setActionState(player, action, press);
            return true;
        }

        if (press)
        {
            switch (event->key())
            {
            case Qt::Key_Escape:
            case Qt::Key_Q:
                StateMachine::instance().quit();
                return true;
            case Qt::Key_P:
                emit pauseToggled();
                return true;
            default:
                break;
            }
        }
    }

    return false;
}


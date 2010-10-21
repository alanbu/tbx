/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "application.h"
#include "eventrouter.h"
#include "autocreatelistener.h"
#include "prequitlistener.h"
#include "quitlistener.h"
#include "command.h"
#include "commandrouter.h"
#include "reporterror.h"
#include "swixcheck.h"
#include "sprite.h"
#include <swis.h>

// Unix filename processing
#include "unixlib/local.h"
int __riscosify_control = __RISCOSIFY_NO_PROCESS;


using namespace tbx;

Application *Application::_instance = 0;

static int all[1] = {0};

const int Application::MIN_WIMP_VERSION = 310;
const int *Application::NO_MESSAGES = (int *)0;
const int *Application::ALL_MESSAGES = all;
const int *Application::NO_EVENTS = (int *)0;;
const int *Application::ALL_EVENTS = all;

/**
 * Initialise the application.
 *
 * If the application can not be initialised this will
 * report the error and then exit the program.
 *
 * @param task_directory The path to application directory
 * @param wimp_version The minimum WIMP version supported by the application.
 *        Defaults to MIN_WIMP_VERSION (310).
 * @param deliver_messages Integer array of message IDs that will be delivered
 *        to the application. Defaults to deliver all messages. Use:
 *        - ALL_MESSAGES - to deliver all messages.
 *        - NO_MESSAGES - to deliver no messages
 *
 * @param deliver_events  Integer array of Toolbox event IDs that will be
 *        delivered to the application. Defaults to deliver all messages.
 *        Use:
 *        - ALL_EVENTS - to deliver all toolbox events.
 *        - NO_EVENTS - to deliver no toolbox events
 */
Application::Application(const char *task_directory,
	    int wimp_version /* = MIN_WIMP_VERSION*/,
	    const int *deliver_messages /*= ALL_MESSAGES*/,
	    const int *deliver_events /*= ALL_EVENTS*/)
{
	_running = false;
	_instance = this;

	// By default TBX applications do not do unix filename processing
	__riscosify_control = __RISCOSIFY_NO_PROCESS;

	_kernel_swi_regs regs;

	regs.r[0] = 0; // flags
	regs.r[1] = wimp_version; // Minimum wimp version
	regs.r[2] = reinterpret_cast<int> (deliver_messages); // Message to allow return
	regs.r[3] = reinterpret_cast<int> (deliver_events); // toolbox events to allow
	regs.r[4] = reinterpret_cast<int> (task_directory);
	regs.r[5] = reinterpret_cast<int> (&_messagesFD); // Message trans descriptor
	regs.r[6] = reinterpret_cast<int> (&(event_router()->_id_block));

	// Initialise the toolbox
	_kernel_oserror *err = _kernel_swi(0x44ECF, &regs, &regs);
	if (err) {
		report_error(err);
		exit(-1);
	}
	_wimp_version = regs.r[0];
	_task_handle = regs.r[1];
	OsSpriteAreaPtr os_sprite_area = reinterpret_cast<OsSpriteAreaPtr>(regs.r[2]);

	if (os_sprite_area) _sprite_area = new SpriteArea(os_sprite_area, false);
	else _sprite_area = 0;
}

/**
 * Run the main polling loop for the application.
 *
 * Once the application has been initialised the program should
 * call this routine to process the events.
 *
 * The routine will not return until the quit method is called
 * from an event listener or command or the event id 0x82a91
 * (This is the id of the Quit event from the quit dialog) is
 * received.
 *
 * @see quit
 */
void Application::run()
{
	EventRouter *router = event_router();
	_running = true;

	while (_running)
	{
		router->poll();
	}
}

/**
 * Set the AutoCreateListener to be called when an object with the specified
 * template is created.
 *
 * This function is used to capture the object handle of an auto created
 * object so it can be used to add event handlers or stored for later
 * use.
 *
 * Objects are auto created when the toolbox resources are loaded if the
 * auto create flag was set in the toolbox resource file.
 *
 * @param template_name The name of the object template.
 * @param listener The auto create listener to call when an object for the
 *        specified template is called.
 */
void Application::set_autocreate_listener(std::string template_name, AutoCreateListener *listener)
{
	event_router()->set_autocreate_listener(template_name, listener);
}

/**
 * Clear the autocreate listener for the specified template.
 *
 * @param template_name The name of the template.
 */
void Application::clear_autocreate_listener(std::string template_name)
{
	event_router()->clear_autocreate_listener(template_name);
}

/**
 * Add a command to be run for the specified event ID.
 *
 * Multiple commands can be added for the same event ID.
 *
 * For more details on command IDs, events and commands see
 * the topic
 * @subpage events
 *
 * @param command_id The ID for the command
 * @param command The command to run.
  */
void Application::add_command(int command_id, Command *command)
{
	event_router()->add_object_listener(NULL_ObjectId,NULL_ComponentId, command_id, command, command_router);
}

/**
 * Remove a command.
 *
 * The event is only removed if the command ID and the command
 * match a previously added command.
 *
 * @param command_id The command ID used for this command
 * @param command The command that was originally added
 */
void Application::remove_command(int command_id, Command *command)
{
	event_router()->remove_object_listener(NULL_ObjectId,NULL_ComponentId, command_id, command);
}

/**
 * Add a command to be run when no events are being received from the desktop.
 *
 * @param Command to add
 */
void Application::add_idle_command(Command *command)
{
	event_router()->add_null_event_command(command);
}

/**
 * Remove a command to be run when no events are being received from the desktop.
 *
 * @param Command to remove
 */

void Application::remove_idle_command(Command *command)
{
	event_router()->remove_null_event_command(command);
}

/**
 * Add a listener for the given wimp message.
 *
 * @param message code to listen for.
 * @param listener to execute.
 */
void Application::add_message_listener(int message_id, WimpMessageListener *listener)
{
	event_router()->add_message_listener(message_id, listener);
}

/**
 * Remove a listener for the given wimp message.
 *
 * @param message code to remove
 * @param listener to remove.
 */
void Application::remove_message_listener(int message_id, WimpMessageListener *listener)
{
	event_router()->remove_message_listener(message_id, listener);
}

/**
 * Add a listener for the pre quit message sent (usually) by the desktop.
 *
 * This message gives the application a chance to refuse to quit if
 * it has unsaved documents.
 */
void Application::add_prequit_listener(PreQuitListener *listener)
{
	if (PreQuitManager::instance() == 0)
	{
		event_router()->add_message_listener(8, new PreQuitManager());
	}
	PreQuitManager::instance()->add_listener(listener);
}

/**
 * Remove a prequit listener
 */
void Application::remove_prequit_listener(PreQuitListener *listener)
{
	if (PreQuitManager::instance() != 0)
		PreQuitManager::instance()->remove_listener(listener);
}

/**
 * Add a listener to process the Quit message.
 *
 * The quit message is sent when the desktop wants the application to close.
 * At this point the application does not have a choice and must close.
 *
 * The application class will drop out of its run method after these messages
 * have been processed and the program should exit.
 *
 * Use a prequit listener to object to the desktop closing down the application.
 *
 * @param quit listener to add
 */
void Application::add_quit_listener(QuitListener *listener)
{
	event_router()->add_message_listener(0, &(listener->_message_listener));
}

/**
 * Remove a quit listener
 *
 * @param listener to remove
 */
void Application::remove_quit_listener(QuitListener *listener)
{
	event_router()->remove_message_listener(0, &(listener->_message_listener));
}

/**
 * Get a handle to a toolbox template.
 *
 * The returned handle can be used to create an object
 * for the template, copied into a ResObject derived
 * class to make a modified version of the template,
 * used to extract a ResGadget from a window or a
 * ResMenuItem from a menu.
 *
 * The return value should not be saved as it is not
 * guaranteed to be the same for the lifetime of the
 * application.
 */
ResHandle *Application::resource(std::string template_name)
{
    ResHandle *res = 0;
    swix_check(_swix(0x44EFB, _INR(0,1)|_OUT(0),
        0, reinterpret_cast<int>(template_name.c_str()),
        &res));

    return res;
}

/**
 * Return the name of the directory that was used to initialised this
 * application.
 */
std::string Application::directory() const
{
	int len;
	// Toolbox_GetSysInfo to get the directory size
	swix_check(_swix(0x44ECE, _INR(0,2)|_OUT(2), 2, 0, 0, &len));

    std::string dir;
    if (len)
    {
    	char buffer[len];
        swix_check(_swix(0x44ECE, _INR(0,2), 2,
    		   reinterpret_cast<int>(buffer),
    		   len));
        dir = buffer;
    }

    return dir;
}

/**
 * Turn on or off UnixLib's automatic translation of unix
 * style file names.
 *
 * By default TBX applications have the unix filename translation
 * turned off.
 *
 * When turned on this uses the default processing.
 * @see unix_filename_control to tune how the details of the translation
 *
 * @param on true to turn on the filename translation, false to turn it off
 *
 */
void Application::unix_filename_translation(bool on)
{
	if (on) __riscosify_control = 0;
	else __riscosify_control = __RISCOSIFY_NO_PROCESS;
}

/**
 * Check if some unix filename translations are being used
 * @return true if any unix filename translation is in force.
 */
bool Application::unix_filename_translation() const
{
	return ((__riscosify_control & __RISCOSIFY_NO_PROCESS) == 0);
}

/**
 * Set flags to control the unix filename translation process.
 *
 * @param flags See the unixlib/local.h UnixLib header file RISCOSIFY flags.
 */
void Application::unix_filename_control(int flags)
{
	__riscosify_control = flags;
}

/**
 * Get the flags that control the unix filename translation process
 */
int Application::unix_filename_control() const
{
	return __riscosify_control;
}
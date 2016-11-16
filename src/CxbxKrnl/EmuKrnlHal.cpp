// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;;
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Win32->CxbxKrnl->EmuKrnlHal.cpp
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Bostom, MA 02111-1307, USA.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *  (c) 2016 Patrick van Logchem <pvanlogchem@gmail.com>
// *
// *  All rights reserved
// *
// ******************************************************************
#define _CXBXKRNL_INTERNAL
#define _XBOXKRNL_DEFEXTRN_

// prevent name collisions
namespace xboxkrnl
{
#include <xboxkrnl/xboxkrnl.h> // For HalReadSMCTrayState, etc.
};

#include "Logging.h" // For LOG_FUNC()
#include "CxbxKrnl.h" // For CxbxKrnlCleanup
#include "Emu.h" // For EmuWarning()

using namespace xboxkrnl;

// ******************************************************************
// * 0x0009 HalReadSMCTrayState
// ******************************************************************
XBSYSAPI EXPORTNUM(9) VOID NTAPI xboxkrnl::HalReadSMCTrayState
(
	DWORD*	State,
	DWORD*	Count
)
{
	LOG_FUNC_BEGIN
		LOG_FUNC_ARG(State)
		LOG_FUNC_ARG(Count)
		LOG_FUNC_END;

#define TRAY_CLOSED_MEDIA_PRESENT 96
#define TRAY_CLOSED_NO_MEDIA 64
#define TRAY_OPEN 16

	// TODO: Make this configurable?
	// TODO: What is the count parameter for??

	// Pretend the tray is open
	// TRAY_CLOSED_NO_MEDIA causes Dashboard to call DeviceIoControl, which we do not implement
	// TRAY_CLOSED_MEDIA_PRESENT causes Dashboard to attempt to launch media, causing errors.
	*State = TRAY_OPEN;
	//	*Count = 1;


}

// This enables Partition3..7  Source:OpenXDK  TODO : Make this configurable
XBSYSAPI EXPORTNUM(40) xboxkrnl::DWORD xboxkrnl::HalDiskCachePartitionCount = 4; // Was 3
																				 // Source:OpenXDK  TODO : Fill this with something sensible
XBSYSAPI EXPORTNUM(41) xboxkrnl::PANSI_STRING xboxkrnl::HalDiskModelNumber = 0;

// Source:OpenXDK  TODO : Fill this with something sensible
XBSYSAPI EXPORTNUM(42) xboxkrnl::PANSI_STRING xboxkrnl::HalDiskSerialNumber = 0;

// ******************************************************************
// * HalGetInterruptVector
// ******************************************************************
XBSYSAPI EXPORTNUM(44) xboxkrnl::ULONG  NTAPI xboxkrnl::HalGetInterruptVector
(
	IN ULONG   InterruptLevel,
	OUT CHAR*  Irql
)
{
	LOG_FUNC_BEGIN
		LOG_FUNC_ARG(InterruptLevel)
		LOG_FUNC_ARG_OUT(Irql)
		LOG_FUNC_END;

	// I'm only adding this for Virtua Cop 3 (Chihiro). Xbox games need not emulate this.

	EmuWarning("HalGetInterruptVector(): If this is NOT a Chihiro game, tell blueshogun!");

	return 1;
}

// ******************************************************************
// * 0x002D - HalReadSMBusValue
// ******************************************************************
XBSYSAPI EXPORTNUM(45) xboxkrnl::NTSTATUS NTAPI xboxkrnl::HalReadSMBusValue
(
	IN  UCHAR               Address,
	IN  UCHAR               Command,
	IN  BOOLEAN             ReadWord,
	OUT PULONG              DataValue
)
{
	LOG_FUNC_BEGIN
		LOG_FUNC_ARG(Address)
		LOG_FUNC_ARG(Command)
		LOG_FUNC_ARG(ReadWord)
		LOG_FUNC_ARG_OUT(DataValue)
		LOG_FUNC_END;

	if (ReadWord) {
		// Write UCHAR
	}
	else {
		// Write BYTE
	}


	return STATUS_SUCCESS;
}

// ******************************************************************
// * 0x002F - HalRegisterShutdownNotification
// ******************************************************************
XBSYSAPI EXPORTNUM(47) VOID xboxkrnl::HalRegisterShutdownNotification
(
	IN PHAL_SHUTDOWN_REGISTRATION ShutdownRegistration,
	IN BOOLEAN Register
)
{
	LOG_FUNC_BEGIN
		LOG_FUNC_ARG(ShutdownRegistration)
		LOG_FUNC_ARG(Register)
		LOG_FUNC_END;

		UNIMPLEMENTED();

	return;
}


// ******************************************************************
// * 0x0031 - HalReturnToFirmware
// ******************************************************************
XBSYSAPI EXPORTNUM(49) VOID DECLSPEC_NORETURN xboxkrnl::HalReturnToFirmware
(
	RETURN_FIRMWARE Routine
)
{
	LOG_FUNC_ONE_ARG(Routine);

	// Prevent the dashboard from rebooting due to unimplemented crypto routines
	if ((uint32_t)Routine != 4) {
		CxbxKrnlCleanup("Xbe has rebooted : HalReturnToFirmware(%d)", Routine);
	}

	UNIMPLEMENTED();
}

// ******************************************************************
// * 0x0032 - HalWriteSMBusValue
// ******************************************************************
XBSYSAPI EXPORTNUM(50) xboxkrnl::NTSTATUS NTAPI xboxkrnl::HalWriteSMBusValue
(
	IN  UCHAR               Address,
	IN  UCHAR               Command,
	IN  BOOLEAN             WriteWord,
	IN  ULONG               DataValue
)
{
	LOG_FUNC_BEGIN
		LOG_FUNC_ARG(Address)
		LOG_FUNC_ARG(Command)
		LOG_FUNC_ARG(WriteWord)
		LOG_FUNC_ARG(DataValue)
		LOG_FUNC_END;

	// TODO: Later.

	return STATUS_SUCCESS;
}

// ******************************************************************
// * HalBootSMCVideoMode
// ******************************************************************
// TODO: Verify this!
XBSYSAPI EXPORTNUM(356) xboxkrnl::DWORD xboxkrnl::HalBootSMCVideoMode = 1;

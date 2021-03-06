//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
//$Revision: 1.26 $
//$Author: jogybl $
//$Date: 2007-05-11 15:29:19 $
//
//  Implements TMciWaveAudio
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_MCI_H)
# include <owl/mci.h>
#endif


__OWL_BEGIN_NAMESPACE

OWL_DIAGINFO;

//
// Initialize the structure for the waveaudio device.
//
static void InitParms(MCI_OPEN_PARMS& parms, uint32& command, LPCTSTR deviceName,
  LPCTSTR elementName = 0, uint16 id = 0)
{
  command = (id == 0) ? MCI_OPEN_TYPE : MCI_OPEN_TYPE_ID;
  if (elementName != 0)  {
    command |= MCI_OPEN_ELEMENT;
    parms.lpstrElementName = elementName;
  }

  parms.lpstrDeviceType = (id == 0) ? deviceName : 
                                      (LPTSTR)MkUint32((uint16)(uint)deviceName, id);
}

//
// Constructs an MCI waveaudio (.WAV) device.
//
TMciWaveAudio::TMciWaveAudio(LPCTSTR elementName, LPCTSTR deviceName, uint16 id)
{
  MCI_OPEN_PARMS parms;
  uint32 command;
  InitParms(parms, command, deviceName == 0 ? _T("waveaudio") : deviceName,
            elementName, id);
  Open(parms, command);
}

//
// Plays the file on the waveaudio device.
//
uint32
TMciWaveAudio::Play(uint32 flags, uint32 from, uint32 to)
{
  MCI_PLAY_PARMS parms;

  if (flags & MCI_NOTIFY)
    parms.dwCallback = GetCallback();

  flags |= MCI_FROM;
  parms.dwFrom = from;

  if (to != 0) {
    flags |= MCI_TO;
    parms.dwTo = to;
  }

  return TMci::Play(parms, flags);
}

__OWL_END_NAMESPACE
/* ========================================================================== */


Name "ScopedVolumeWheel"

!ifndef x86_64
	OutFile "ScopedVolumeWheel-x86-setup.exe"
!else
	OutFile "ScopedVolumeWheel-x86_64-setup.exe"
!endif


InstallDir "$LOCALAPPDATA\ScopedVolumeWheel"
InstallDirRegKey HKCU "Software\ScopedVolumeWheel" "Install_Dir"
RequestExecutionLevel user
SetCompress auto

LicenseData "..\..\LICENSE.txt"

; MUI
!include "MUI2.nsh"
!define MUI_ICON "..\..\ScopedVolumeWheel\icon.ico"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"


Section
	SetOutPath $INSTDIR

	!ifndef x86_64
		File "..\..\Release\ScopedVolumeWheel.exe"
	!else
		File "..\..\x64\Release\ScopedVolumeWheel.exe"
	!endif

	; Desktop shortcut
	CreateShortCut "$DESKTOP\ScopedVolumeWheel.lnk" "$INSTDIR\ScopedVolumeWheel.exe"

	; Start menu shortcuts
	CreateDirectory "$SMPROGRAMS\ScopedVolumeWheel"
	CreateShortCut "$SMPROGRAMS\ScopedVolumeWheel\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	CreateShortCut "$SMPROGRAMS\ScopedVolumeWheel\ScopedVolumeWheel.lnk" "$INSTDIR\ScopedVolumeWheel.exe"

	; Run at startup
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "ScopedVolumeWheel" "$INSTDIR\ScopedVolumeWheel.exe"

	; Registry uninstall information
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScopedVolumeWheel" "DisplayName" "ScopedVolumeWheel (remove only)"
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScopedVolumeWheel" "UninstallString" "$INSTDIR\Uninstall.exe"

	WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "Uninstall"
	
	ExecWait "TaskKill /IM ScopedVolumeWheel.exe /F"

	RMDir /r "$INSTDIR"
	RMDir /r "$APPDATA\ScopedVolumeWheel"

	DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "ScopedVolumeWheel"

	DeleteRegKey HKCU "Software\ScopedVolumeWheel"
	DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScopedVolumeWheel"

SectionEnd

; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Eggnapped"
#define MyAppVersion "1.2"
#define MyAppPublisher "DigiPen Institute of Technology"
#define MyAppURL "http://www.digipen.edu/"
#define MyAppExeName "Eggnapped.exe"
#define MyAppIcoName "Eggnapped.ico"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{66D973E4-F87B-4933-A233-C90C5DE80FB2}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}/DigiPen/{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=DigiPen_EULA.txt
OutputBaseFilename=EggnappedInstaller
SetupIconFile={#MyAppIcoName}
Compression=lzma
SolidCompression=yes
AlwaysRestart=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; \
  Flags: unchecked
Name: "msvc2017redist"; Description: "Install Microsoft VS2017 Redistributable"; Flags: 

[Files]
Source: "Redist/VC_redist.x86.exe"; DestDir: {tmp}; Flags: deleteafterinstall
Source: "../Release/TetraiderEngine.exe"; DestDir: "{app}"; DestName: "Eggnapped.exe"; Flags: ignoreversion
; Source: "{#MyAppIcoName}"; DestDir: "{app}"
Source: "fmod.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "fmodL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "fmodstudioL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "glew32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "imgui.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libjpeg-9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libtiff-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libwebp-4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2_mixer.lib"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL2_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "config.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "Assets\*"; DestDir: "{app}\Assets"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; \
  IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; \
  IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; \
  IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon
Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; \
  IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon

[Run]
; add the Parameters, WorkingDir and StatusMsg as you wish, just keep here
; the conditional installation Check
Filename: "{tmp}\VC_redist.x86.exe"; Parameters: /silent; \
  StatusMsg: "Installing Microsoft VS2017 Redistributable"; Tasks: msvc2017redist;
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; \
  Flags: nowait postinstall skipifsilent
Filename: "{app}\README.txt"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent


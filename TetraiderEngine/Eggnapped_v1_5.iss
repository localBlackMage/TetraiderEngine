#define MyAppName "Eggnapped"
#define MyAppVersion "1.4"
#define MyAppPublisher "DigiPen Institute of Technology"
#define MyAppURL "http://www.digipen.edu/"
#define MyAppExeName "Eggnapped.exe"
#define MyAppIcoName "Eggnapped.ico"
#define MyAdditionToolsFolder "REDIST"
#define MyOutputDir "INSTALLER"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{12F00B93-978A-49ED-A619-CDB2268529FB}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\DigiPen/{#MyAppName}
DefaultGroupName=DigiPen/{#MyAppName}
LicenseFile=EULA\DigiPen_EULA.txt
OutputBaseFilename={#MyAppName}_Setup
SetupIconFile={#MyAppIcoName}
OutputDir={#MyOutputDir}/
Compression=lzma
SolidCompression=yes
AllowNoIcons=yes
PrivilegesRequired=lowest
; Prevents the installer asking for a reboot if certian Windows files are modified.
RestartIfNeededByRun=no
AlwaysRestart=no

; DO NOT CHANGE THIS SECTION
[Code]
#ifdef UNICODE
  #define AW "W"
#else
  #define AW "A"
#endif
type
  HINSTANCE = THandle;

procedure ExitProcess(uExitCode: UINT);
  external 'ExitProcess@kernel32.dll stdcall';
function ShellExecute(hwnd: HWND; lpOperation: string; lpFile: string;
  lpParameters: string; lpDirectory: string; nShowCmd: Integer): HINSTANCE;
  external 'ShellExecute{#AW}@shell32.dll stdcall';

var
  Elevated: Boolean;
  PagesSkipped: Boolean;

function CmdLineParamExists(const Value: string): Boolean;
var
  I: Integer;  
begin
  Result := False;
  for I := 1 to ParamCount do
    if CompareText(ParamStr(I), Value) = 0 then
    begin
      Result := True;
      Break;
    end;
end;

procedure InitializeWizard;
begin
  // initialize our helper variables
  Elevated := CmdLineParamExists('/ELEVATE');
  PagesSkipped := False;
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  // if we've executed this instance as elevated, skip pages unless we're
  // on the directory selection page
  Result := not PagesSkipped and Elevated and (PageID <> wpSelectDir);
  // if we've reached the directory selection page, set our flag variable
  if not Result then
    PagesSkipped := True;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
  Params: string;
  RetVal: HINSTANCE;
  TestCreatable: bool;
  Compara: bool;
begin
  Result := True;


  

  if (CurPageID = wpSelectDir) then
  begin
    TestCreatable := SaveStringToFile(ExpandConstant('{app}'),  ' ', false);
    if TestCreatable then
    begin
      DeleteFile(ExpandConstant('{app}'));
    end;
    Compara := not Elevated;
    Compara := Compara and not TestCreatable;
  if (Compara) then
  begin
    // pass the already selected directory to the executing parameters and
    // include our own custom /ELEVATE parameter which is used to tell the
    // setup to skip all the pages and get to the directory selection page
	if TestCreatable then
    begin
	  Params := ExpandConstant('/DIR="{app}"');
    end
	else
    begin
      Params := ExpandConstant('/DIR="{app}" /ELEVATE');
    end
    // because executing of the setup loader is not possible with ShellExec
    // function, we need to use a WinAPI workaround
    RetVal := ShellExecute(WizardForm.Handle, 'runas',
      ExpandConstant('{srcexe}'), Params, '', SW_SHOW);
    // if elevated executing of this setup succeeded, then...
    if RetVal > 32 then
    begin
      // exit this non-elevated setup instance
      ExitProcess(0);
    end
    else
    // executing of this setup failed for some reason; one common reason may
    // be simply closing the UAC dialog
    begin
      // handling of this situation is upon you, this line forces the wizard
      // stay on the current page
      Result := False;
      // and possibly show some error message to the user
      MsgBox(Format('Cannot install at specified path with current privilege settings. Error code: %d.', [RetVal]),
        mbError, MB_OK);
    end;
  end;
end;
end;

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; \
  GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1;
Name: "msvc2017redist"; Description: "Install Microsoft VS2017 Redistributable";

[Files]
Source: "REDIST/VC_redist.x86.exe"; DestDir: {tmp}; Flags: deleteafterinstall
Source: "../Release/TetraiderEngine.exe"; DestDir: "{app}"; DestName: "Eggnapped.exe"; Flags: ignoreversion
Source: "{#MyAppIcoName}"; DestDir: "{app}"
Source: "fmod.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "fmodL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "fmodstudioL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "glew32.dll"; DestDir: "{app}"; Flags: ignoreversion
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
IconFilename: "{app}\{#MyAppIcoName}"; Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; 
IconFilename: "{app}\{#MyAppIcoName}"; Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"; 
IconFilename: "{app}\{#MyAppIcoName}"; Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon;
IconFilename: "{app}\{#MyAppIcoName}"; Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon; 

[Run]
; add the Parameters, WorkingDir and StatusMsg as you wish, just keep here
; the conditional installation Check
Filename: "{tmp}\VC_redist.x86.exe"; Parameters: /silent /norestart; \
  StatusMsg: "Installing Microsoft VS2017 Redistributable"; Tasks: msvc2017redist;
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; \
  Flags: shellexec postinstall skipifsilent;
Filename: "{app}\README.txt"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\*.*"

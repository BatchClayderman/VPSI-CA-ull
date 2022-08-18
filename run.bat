@ECHO OFF
CD /D "%~DP0Results"
FOR /F %%I IN ('DIR /A-D /B "%~DP0x64\Release" ^| FINDSTR /I ".EXE"') DO (START /REALTIME "" "%~DP0x64\Release\%%I")
SET /P N=Çë¼üÈë n Öµ£º
FOR /F %%I IN ('DIR /A-D /B "%~DP0Results" ^| FINDSTR /I ".TMP"') DO (
	IF EXIST "%%~NI_%N%.txt" (DEL /Q "%%~NI_%N%.txt")
	ECHO %%~NI_%N%.txt
	REN "%~DP0Results\%%~NI.tmp" "%%~NI_%N%.txt"
)
PAUSE>NUL
EXIT
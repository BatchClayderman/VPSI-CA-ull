@ECHO OFF
CD /D "%~DP0Results"
FOR /F %%I IN ('DIR /A-D /B "%~DP0x64\Release" ^| FINDSTR /I ".EXE"') DO (START /REALTIME /MIN "" "%~DP0x64\Release\%%I")
SET /P M=����� m ֵ��
FOR /F %%I IN ('DIR /A-D /B "%~DP0Results" ^| FINDSTR /I ".TMP"') DO (
	IF EXIST "%%~NI_%M%.txt" (DEL /Q "%%~NI_%M%.txt")
	ECHO %%~NI_%M%.txt
	REN "%~DP0Results\%%~NI.tmp" "%%~NI_%M%.txt"
)
PAUSE>NUL
EXIT
@ECHO OFF

SETLOCAL

SET VERSION_VER=%1\Res\Version.Ver

FOR /F "usebackq" %%i IN (`svnversion`) DO SET BUILDNUMBER=%%i

FOR /F "tokens=1-5 delims=/: " %%J IN ('DATE /T') DO SET DATESTAMP=%%M/%%K/%%L
FOR /F "tokens=1-5 delims=/: " %%J IN ('TIME /T') DO SET TIMESTAMP=%%J:%%K%%L


ECHO Version Number for %1 is %BUILDNUMBER%

ECHO #define VERSION_BUILD %BUILDNUMBER% > %VERSION_VER%
ECHO #define VERSION_QFE 0 >> %VERSION_VER%
ECHO #define VERSION_BUILD_DATETIME %DATESTAMP% %TIMESTAMP% >> %VERSION_VER%


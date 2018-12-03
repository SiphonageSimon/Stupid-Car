::@echo off 

@for /d /r %%c in (Exe,settings,List,Obj) do @if exist %%c ( rd /s /q "%%c" & echo     É¾³ýÄ¿Â¼%%c) 

@for /r  %%c in (*.tmp,*.bak ,*.dep,*.sfr ) do del "%%c"
pause
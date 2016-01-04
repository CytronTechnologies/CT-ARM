@echo off
cd %1
%1\NuLink -e aprom
%1\NuLink -w aprom %2
%1\NuLink -reset
exit 0

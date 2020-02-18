@echo off
:: appname ������
:: inputname �����ļ���
:: outputname ����ļ���
:: resultname �������̨����ض����ļ���

set appname="C:\\Qt\\ComputerNetwork-Lab\\Lab2-RdtTrans\\Lab2-RdtTrans-gbn.exe"
set inputname="C:\\Qt\\ComputerNetwork-Lab\\Lab2-RdtTrans\\input.txt"
set outputname="C:\\Qt\\ComputerNetwork-Lab\\Lab2-RdtTrans\\log\\gbn_output.txt"
set resultname="C:\\Qt\\ComputerNetwork-Lab\\Lab2-RdtTrans\\log\\gbn_out.log"

for /l %%i in (1,1,10) do (
    echo Test %appname% %%i:
    %appname% > %resultname% 2>&1
    fc /N %inputname% %outputname%
)
pause
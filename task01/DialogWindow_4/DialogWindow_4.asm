include 'win32ax.inc'

.code
start:
        invoke MessageBox, HWND_DESKTOP, '�� �������� �� ������ ��������� �������?', invoke GetCommandLine, MB_YESNO
        .if eax = IDYES
            invoke MessageBox, HWND_DESKTOP, '�������� ������ � ���������� �������������', '��������� �������� �������...', MB_OK
        .endif
        .if eax = IDNO
            invoke MessageBox, HWND_DESKTOP, '10. ��� ������������.',  '�������.', MB_OK
        .endif


        invoke ExitProcess, 0
.end start